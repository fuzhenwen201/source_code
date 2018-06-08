/*************************************************************************
    > File Name: SubscriptionService.cc
    > Author: Kevin
    > Mail: kevin.fu@fintechstar.cn
    > Created Time: Wed 24 Jan 2018 04:10:35 PM CST
 ************************************************************************/

#include "SubscriptionService.h"
#include "quickfix/fix44/ExecutionReport.h"

pthread_cond_t g_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t g_sessionlock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t userMaptnodes_lock = PTHREAD_MUTEX_INITIALIZER;
std::map<std::string, StUserLogonResp> g_LogonResp = {};
std::map<FIX::SessionID,StUserLogonResp > g_sessionIds = {};
snetwork_xservice_tnode::TNode *g_tnode = nullptr;
char g_hostname[128] = {};
std::map<uint32_t,RoutingAndTnode> userMaptnodes = {};
static STTnodeConfig tnodeConfig = {};
static RedisConfigInfo redisConfig = {};
static std::map<uint32_t,redisContext*> userMapRedisContext;

SubscriptionService::SubscriptionService() = default;

SubscriptionService::~SubscriptionService() = default;

static  uint64_t GetCurrentTimestamp(){
    struct timespec ts;
    uint64_t tmp;

    clock_gettime(CLOCK_REALTIME, &ts);

    tmp = (uint64_t)(ts.tv_nsec / 1000);
    tmp += ts.tv_sec * 1000000;

    return tmp;
}

bool SendLog2Server(uint32_t loglevel, std::string text, snetwork_xservice_tnode::TNode* tnode){
    MEProInterface::IntervalMsgHeader logInterHeader;
    logInterHeader.set_iflownumber(1);
    logInterHeader.set_icmd(0);
    logInterHeader.set_imsgtype(0);
    auto loginfo = new MEProInterface::ST_MAINTAIN_LOG;

    loginfo->set_strhostname(g_hostname);
    loginfo->set_chloglevel(loglevel);
    loginfo->set_chmoduleindex(6);
    loginfo->set_uireptime(GetCurrentTimestamp());
    loginfo->set_strlogcontent(text);
    logInterHeader.set_allocated_stmaintainlog(loginfo);
    std::string buff;
    logInterHeader.SerializeToString(&buff);

    size_t len = buff.size();
    char buffer[2048] = {};
    memcpy(buffer, buff.c_str(), len);
    char binding_key[128] = {};
    sprintf(binding_key, "_MaintainInfo.Rep.6.%s", g_hostname);

    //将订单信息发送到MQ中
    if (tnode->PublishToMQ(binding_key, buffer, len) != 0) {
        XERROR("Push new order to MQ failed.");
    }

    return true;
}

void SubscriptionService::Init(STTnodeConfig& config, RedisConfigInfo& redisconfig, std::string& stid) {
    tnodeConfig.mq_exchange_group = config.mq_exchange_group;
    tnodeConfig.mq_vhost = config.mq_vhost;
    tnodeConfig.mq_host = config.mq_host;
    tnodeConfig.mq_passwd = config.mq_passwd;
    tnodeConfig.mq_port = config.mq_port;
    tnodeConfig.mq_user = config.mq_user;

    redisConfig.redisaddr = redisconfig.redisaddr;
    redisConfig.redispass = redisconfig.redispass;
    redisConfig.redisport = redisconfig.redisport;
    //初始化MQ并订阅消息
    g_tnode = new snetwork_xservice_tnode::TNode(config);

    g_tnode->Init();

    std::string queue_name = "SUBAPI_USERLOGINRESP" + stid;

    auto *logonInfo = new LogonInfoConsumer("_USR_LOGIN.Resp.Subscripte", queue_name.c_str());
    g_tnode->AddTNodeConsumer(logonInfo);

    g_tnode->RunConsumer();
    gethostname(g_hostname, sizeof(g_hostname));
}

void SubscriptionService::Run(){
    pause();
}

LogonInfoConsumer::LogonInfoConsumer(const char *bindingkey, const char *queuename) :
    m_bindingkey(bindingkey),m_queuename(queuename){

}

LogonInfoConsumer::~LogonInfoConsumer(){

};

std::string LogonInfoConsumer::GetQueueName() {
    return m_queuename;
}

std::string LogonInfoConsumer::GetBindingkey() {
    return m_bindingkey;
}

uint32_t LogonInfoConsumer::ConsumerData(char* pMsg, int32_t nMsgLen){
    assert(pMsg != nullptr);

    StUserLogonResp userlogonResp = {};
    MEProInterface::IntervalMsgHeader intervalMsgHeader;
    intervalMsgHeader.ParseFromArray(pMsg, nMsgLen);
    if(!intervalMsgHeader.has_stloginresp()){
        XERROR("There have no LOGIN RESP information in the message.");
        return 1;
    }
    MEProInterface::ST_USER_LOGIN_RESP userLogonResp = intervalMsgHeader.stloginresp();

    std::string name = userLogonResp.chusername();

    uint32_t i = 0;
    pthread_mutex_lock(&g_usernamelock);
    for(i = 0; i < g_username.size(); i++){

        if(g_username[i] == name){
            break;
        }
    }
    //未找到验证的用户
    if(i == g_username.size()){
        pthread_mutex_unlock(&g_usernamelock);

        return 0;
    }
    g_username.erase(g_username.begin() + i);
    pthread_mutex_unlock(&g_usernamelock);

    userlogonResp.ucLoginRsp = userLogonResp.ucloginrsp();
    userlogonResp.uiUserID = userLogonResp.uiuserid();

    uint32_t ipnum = userLogonResp.uiiplisynum();
    userlogonResp.uiIPLisyNum = ipnum;
    userlogonResp.chUserName = name;

    if(userlogonResp.ucLoginRsp == 0) {
        auto tnodeit = userMaptnodes.find(userlogonResp.uiUserID);
        if (tnodeit == userMaptnodes.end()) {
            auto newtnode = new snetwork_xservice_tnode::TNode(tnodeConfig);
            newtnode->Init();
            std::string routingkey1 = "_MarketData.Trade.FrTable." + std::to_string(userlogonResp.uiUserID);
            std::string queuename1 = "SUBAPI_Trade_MarketData" + std::to_string(userlogonResp.uiUserID);
            auto *orderStatus = new OrderStatusConsumer(routingkey1.c_str(), queuename1.c_str());
            newtnode->AddTNodeConsumer(orderStatus);

            std::string routingkey2 = "_QryOrder.Resp.History." + std::to_string(userlogonResp.uiUserID);
            std::string queuename2 = "SUBAPI_QryOrderResp" + std::to_string(userlogonResp.uiUserID);
            auto *histquot = new HistQuotConsumer(routingkey2.c_str(), queuename2.c_str());
            newtnode->AddTNodeConsumer(histquot);

            std::string routingkey3 = "_MarketData.RepTransTrade." + std::to_string(userlogonResp.uiUserID);
            std::string queuename3 = "SUBAPI_RepTransTrade" + std::to_string(userlogonResp.uiUserID);
            auto *repTrans = new RepTransConsumer(routingkey3.c_str(), queuename3.c_str());
            newtnode->AddTNodeConsumer(repTrans);

            std::string routingkey4 = "_OrderBook.Trade." + std::to_string(userlogonResp.uiUserID);
            std::string queuename4 = "SUBAPI_OrderBook" + std::to_string(userlogonResp.uiUserID);
            auto *orderBook = new OrderBooksConsumer(routingkey4.c_str(), queuename4.c_str());
            newtnode->AddTNodeConsumer(orderBook);

            newtnode->RunConsumer();

            RoutingAndTnode tnode_routingkey = {};
            tnode_routingkey.tnode = newtnode;
            tnode_routingkey.routingKey1 = routingkey1;
            tnode_routingkey.routingKey2 = routingkey2;
            tnode_routingkey.routingKey3 = routingkey3;
            tnode_routingkey.routingKey4 = routingkey4;
            pthread_mutex_lock(&userMaptnodes_lock);
            userMaptnodes.insert(pair<uint32_t, RoutingAndTnode>(userlogonResp.uiUserID, tnode_routingkey));
            pthread_mutex_unlock(&userMaptnodes_lock);

            struct timeval timeout = {2, 0};    //2s的超时时间
            auto pRedisContext = redisConnectWithTimeout(redisConfig.redisaddr.c_str(), redisConfig.redisport, timeout);
            if ((nullptr == pRedisContext) || pRedisContext->err) {
                if (pRedisContext)
                    XERROR("connect redis error:%s", pRedisContext->errstr);
                else
                    XERROR("connect redis error: can't allocate redis context.");
                return 1;
            }

            auto *pRedisReply = (redisReply *) redisCommand(pRedisContext, "AUTH %s",
                                                            redisConfig.redispass.c_str());  //执行AUTH命令
            if (pRedisReply->type == 6) {
                XERROR("Redis authentication failed.");
                freeReplyObject(pRedisReply);
                return 1;
            } else {
                XINFO("Redis certification is successful.");
            }
            freeReplyObject(pRedisReply);

            userMapRedisContext.insert(pair<uint32_t, redisContext *>(userlogonResp.uiUserID, pRedisContext));
        }
    }

    std::string log = name + ":" + std::to_string(userlogonResp.uiUserID) + " login authentication result" +
                      std::to_string(userlogonResp.ucLoginRsp) + " message from MQ.";
    SendLog2Server(2, log, g_tnode);

    for (uint32_t j = 0; j < ipnum; ++j) {
        StSigIpinfo sigipinfo = {};
        sigipinfo.wIpType =  userLogonResp.stipinfo(j).wiptype();
        sigipinfo.ucIpAddress = userLogonResp.stipinfo(j).ucipaddress();
        userlogonResp.stIPInfo.push_back(sigipinfo);
    }

    ipnum = userLogonResp.uisymbolnum();
    userlogonResp.uiSymbolNum = ipnum;
    for (uint32_t k = 0; k < ipnum; ++k) {
        StSymbolInfo symbolinfo = {};
        symbolinfo.uiSymbolID = userLogonResp.stsymbolinfo(k).uisymbolid();
        symbolinfo.uiSymbolName = userLogonResp.stsymbolinfo(k).uisymbolname();
        userlogonResp.stSymbolInfo.push_back(symbolinfo);
    }

    pthread_mutex_lock(&g_lock);
    auto iter = g_LogonResp.find(name);
    if(iter != g_LogonResp.end()){
        g_LogonResp.erase(iter);
    }
    g_LogonResp.insert(std::pair<std::string, StUserLogonResp>(name, userlogonResp));

    pthread_mutex_unlock(&g_lock);
    pthread_cond_broadcast(&g_cond);
    return 0;
}

OrderStatusConsumer::OrderStatusConsumer(const char *bindingkey, const char *queuename) :
        m_bindingkey(bindingkey),m_queuename(queuename){

}

OrderStatusConsumer::~OrderStatusConsumer(){

};

std::string OrderStatusConsumer::GetQueueName() {
    return m_queuename;
}

std::string OrderStatusConsumer::GetBindingkey() {
    return m_bindingkey;
}

uint32_t OrderStatusConsumer::ConsumerData(char* pMsg, int32_t nMsgLen) {
    assert(pMsg != nullptr);

    MEProInterface::IntervalMsgHeader intervalMsgHeader;
    intervalMsgHeader.ParseFromArray(pMsg, nMsgLen);
    if (!intervalMsgHeader.has_sttransorder()) {
        XERROR("There is no trans order information in the message.");
        return 1;
    }

    std::string log = "Receive a deal order information from MQ.";
    SendLog2Server(2, log, g_tnode);

    MEProInterface::ST_TRANS_ORDER_INFO m_transorderInfo = intervalMsgHeader.sttransorder();

    const auto &bidorderid = m_transorderInfo.strbidorderid();
    const auto &askorderid = m_transorderInfo.straskorderid();
    uint32_t biduserid = m_transorderInfo.uibiduserid();
    uint32_t askuserid = m_transorderInfo.uiaskuserid();
    auto price = m_transorderInfo.fprice();
    double_t dprice = price / 1000000.0;
    auto number = m_transorderInfo.fnumber();
    double_t dnumber = number / 1000000.0;
    uint32_t exchtype = m_transorderInfo.ucexchtype();
    uint64_t exchid = m_transorderInfo.ucrecordid();
    uint32_t orderstatus = m_transorderInfo.ucorderstatus();
    uint32_t exchobjid = m_transorderInfo.uiexchobjid();

    std::string bidkey = std::to_string(biduserid) + bidorderid;
    std::string askkey = std::to_string(askuserid) + askorderid;
    if (biduserid != 0) {
        auto iteredis = userMapRedisContext.find(biduserid);
        if(iteredis != userMapRedisContext.end()) {
            auto rediscontext = iteredis->second;
            auto pRedisReply = (redisReply *) redisCommand(rediscontext, "DEL %s", bidkey.c_str());
            freeReplyObject(pRedisReply);
        }
    }

    if (askuserid != 0) {
        auto iteredis = userMapRedisContext.find(biduserid);
        if(iteredis != userMapRedisContext.end()) {
            auto rediscontext = iteredis->second;
            auto pRedisReply = (redisReply *) redisCommand(rediscontext, "DEL %s", askkey.c_str());
            freeReplyObject(pRedisReply);
        }
    }

    pthread_mutex_lock(&g_sessionlock);

    for (auto &g_sessionId : g_sessionIds) {
        if(g_sessionId.second.uiUserID == biduserid || g_sessionId.second.uiUserID == askuserid){
            FIX44::ExecutionReport execreport;
            execreport.setField(37, std::to_string(exchid));

            if(exchtype == 0) {
                execreport.setField(FIX::ExecType('4'));
            } else {
                execreport.setField(FIX::ExecType('F'));
            }

            for (auto &i : g_sessionId.second.stSymbolInfo) {
                if(exchobjid == i.uiSymbolID){
                    execreport.setField(FIX::Symbol(i.uiSymbolName));
                    break;
                }
            }

            if(orderstatus == 1)
                execreport.setField(FIX::OrdStatus('1'));
            else
                execreport.setField(FIX::OrdStatus('2'));

            std::string OrdId = {};
            if(bidorderid.empty()){
                OrdId = askorderid;
                execreport.setField(FIX::Side('2'));
            }else if(askorderid.empty()){
                OrdId = bidorderid;
                execreport.setField(FIX::Side('1'));
            } else {
                OrdId = bidorderid + "," + askorderid;
                execreport.setField(FIX::Side('G'));
            }
            execreport.setField(FIX::ClOrdID(OrdId));

            execreport.setField(44, std::to_string(dprice));
            execreport.setField(38, std::to_string(dnumber));
            execreport.setField(FIX::AvgPx(0));
            execreport.setField(FIX::CumQty(0));
            execreport.setField((FIX::LeavesQty(0)));
            execreport.setField(FIX::TransactTime(true));

            FIX::Session::sendToTarget(execreport, g_sessionId.first);
        }
    }
    pthread_mutex_unlock(&g_sessionlock);

    return 0;
}

HistQuotConsumer::HistQuotConsumer(const char *bindingkey, const char *queuename) :
        m_bindingkey(bindingkey),m_queuename(queuename){

}

HistQuotConsumer::~HistQuotConsumer(){

};

std::string HistQuotConsumer::GetQueueName() {
    return m_queuename;
}

std::string HistQuotConsumer::GetBindingkey() {
    return m_bindingkey;
}

uint32_t HistQuotConsumer::ConsumerData(char* pMsg, int32_t nMsgLen){
    assert(pMsg != nullptr);

    MEProInterface::IntervalMsgHeader intervalMsgHeader;
    intervalMsgHeader.ParseFromArray(pMsg, nMsgLen);
    if(!intervalMsgHeader.has_stqueryresp()){
        XERROR("There is no historical order response information in the message.");
        return 1;
    }
    MEProInterface::ST_QUERY_ORDER_RES queryOrdRes = intervalMsgHeader.stqueryresp();

    uint32_t userID = queryOrdRes.uiuserid();
    const std::string& orderID = queryOrdRes.strorderid();
    uint32_t symbolID = queryOrdRes.uiexchobjid();
    uint32_t tranStatus = queryOrdRes.wtransstatus();
    uint32_t transType = queryOrdRes.uctranstype();
    int64_t totalNumber = queryOrdRes.lorgnumber();
    int64_t execNumber = queryOrdRes.lavgtransfernum();
    double_t orgPrice = queryOrdRes.lorgprice() / 1000000.0;
    double_t orgNumber = totalNumber / 1000000.0;
    double_t avgPrice = queryOrdRes.lavgtransferprice() / 1000000.0;
    double_t avgNumber = execNumber / 1000000.0;

    std::string log = "Received the order query response message from MQ.";
    SendLog2Server(2, log, g_tnode);

    FIX44::HistQuotResp execreport;
    execreport.setField(11, orderID);

    if(tranStatus == 0){
        execreport.setField(39, "A");
    }else {
        execreport.setField(39, std::to_string(tranStatus));
    }

    if(transType == 0)
        execreport.setField(54, "0");
    else
        execreport.setField(54, "1");

    execreport.setField(44, std::to_string(orgPrice));
    execreport.setField(38, std::to_string(orgNumber));
    execreport.setField(6, std::to_string(avgPrice));
    execreport.setField(14,std::to_string(avgNumber));

    std::map<FIX::SessionID, StUserLogonResp>::iterator iter;
    pthread_mutex_lock(&g_sessionlock);
    for(iter = g_sessionIds.begin(); iter != g_sessionIds.end(); iter++){
        if(iter->second.uiUserID == userID){
            for(uint32_t i = 0; i < iter->second.uiSymbolNum; ++i){
                if(iter->second.stSymbolInfo[i].uiSymbolID == symbolID){
                    execreport.setField(FIX::Symbol(iter->second.stSymbolInfo[i].uiSymbolName));
                    execreport.setField(FIX::TransactTime(true));
                    FIX::Session::sendToTarget(execreport, iter->first);
                    pthread_mutex_unlock(&g_sessionlock);
                    return 0;
                }
            }
        }
    }
    pthread_mutex_unlock(&g_sessionlock);
    return 1;
}

RepTransConsumer::RepTransConsumer(const char *bindingkey, const char *queuename) :
        m_bindingkey(bindingkey),m_queuename(queuename){

}

RepTransConsumer::~RepTransConsumer(){

};

std::string RepTransConsumer::GetQueueName() {
    return m_queuename;
}

std::string RepTransConsumer::GetBindingkey() {
    return m_bindingkey;
}

uint32_t RepTransConsumer::ConsumerData(char* pMsg, int32_t nMsgLen){
    assert(pMsg != nullptr);

    MEProInterface::IntervalMsgHeader intervalMsgHeader;
    intervalMsgHeader.ParseFromArray(pMsg, nMsgLen);
    if(!intervalMsgHeader.has_sttransorder()){
        XERROR("There is no Retrans order response information in the message.");
        return 1;
    }
    MEProInterface::ST_TRANS_ORDER_INFO m_transorderInfo = intervalMsgHeader.sttransorder();

    const auto& bidorderid = m_transorderInfo.strbidorderid();
    const auto& askorderid = m_transorderInfo.straskorderid();
    uint32_t biduserid = m_transorderInfo.uibiduserid();
    uint32_t askuserid = m_transorderInfo.uiaskuserid();
    auto price = m_transorderInfo.fprice();
    double_t dprice = price / 1000000.0;
    auto number = m_transorderInfo.fnumber();
    double_t dnumber = number / 1000000.0;
    uint32_t exchtype = m_transorderInfo.ucexchtype();
    uint64_t exchid = m_transorderInfo.ucrecordid();
    uint32_t orderstatus = m_transorderInfo.ucorderstatus();
    uint32_t exchobjid = m_transorderInfo.uiexchobjid();

    std::string log = "Received the order Retrans query response message from MQ.";
    SendLog2Server(2, log, g_tnode);

    pthread_mutex_lock(&g_sessionlock);

    for (auto &g_sessionId : g_sessionIds) {
        if(g_sessionId.second.uiUserID == biduserid || g_sessionId.second.uiUserID == askuserid){
            FIX44::OrdRetransResp execreport;
            execreport.setField(37, std::to_string(exchid));

            if(exchtype == 0) {
                execreport.setField(FIX::ExecType('4'));
            } else {
                execreport.setField(FIX::ExecType('F'));
            }

            for (auto &i : g_sessionId.second.stSymbolInfo) {
                if(exchobjid == i.uiSymbolID){
                    execreport.setField(FIX::Symbol(i.uiSymbolName));
                    break;
                }
            }

            if(orderstatus == 1)
                execreport.setField(FIX::OrdStatus('1'));
            else
                execreport.setField(FIX::OrdStatus('2'));

            execreport.setField(FIX::Side('H'));
            std::string OrdId = {};
            OrdId = bidorderid + ",";
            OrdId += askorderid;
            execreport.setField(FIX::ClOrdID(OrdId));

            execreport.setField(44, std::to_string(dprice));
            execreport.setField(38, std::to_string(dnumber));
            execreport.setField(FIX::TransactTime(true));

            FIX::Session::sendToTarget(execreport, g_sessionId.first);
        }
    }
    pthread_mutex_unlock(&g_sessionlock);

    return 0;
}

OrderBooksConsumer::OrderBooksConsumer(const char *bindingkey, const char *queuename) :
        m_bindingkey(bindingkey),m_queuename(queuename){

}

OrderBooksConsumer::~OrderBooksConsumer(){

};

std::string OrderBooksConsumer::GetQueueName() {
    return m_queuename;
}

std::string OrderBooksConsumer::GetBindingkey() {
    return m_bindingkey;
}

uint32_t OrderBooksConsumer::ConsumerData(char* pMsg, int32_t nMsgLen) {
    assert(pMsg != nullptr);

    MEProInterface::IntervalMsgHeader intervalMsgHeader;
    intervalMsgHeader.ParseFromArray(pMsg, nMsgLen);
    if (!intervalMsgHeader.has_stmarketorderbook()) {
        XERROR("There is no order books information in the message.");
        return 1;
    }

    std::string log = "Receive order book information from MQ.";
    SendLog2Server(2, log, g_tnode);

    MEProInterface::ST_MARKET_ORDERBOOK market_orderbook = intervalMsgHeader.stmarketorderbook();
    uint32_t userid = market_orderbook.uiuserid();
    uint32_t symbolid = market_orderbook.uiexchobjid();
    uint32_t bidordnum = market_orderbook.uibidordernum();
    uint32_t askordnum = market_orderbook.uiaskordernum();
    std::string orderbook = "{\"Buy\":[";

    if(bidordnum == 0){
        orderbook += "{\"Price\":0.000000,\"Amount\":0.000000}],\"Sell\":[";
    }else {
        uint32_t tmp_bid = bidordnum - 1;
        for (uint32_t i = 0; i < bidordnum; i++) {
            auto price = market_orderbook.stbidorder(i).lprice();
            auto number = market_orderbook.stbidorder(i).lnumber();
            double_t dprice = price / 1000000.0;
            double_t dnumber = number / 1000000.0;

            if (i != tmp_bid)
                orderbook += "{\"Price\":" + std::to_string(dprice) + ",\"Amount\":" + std::to_string(dnumber) + "},";
            else
                orderbook += "{\"Price\":" + std::to_string(dprice) + ",\"Amount\":" + std::to_string(dnumber) +
                             "}],\"Sell\":[";
        }
    }
    if(askordnum == 0){
        orderbook += "{\"Price\":0.000000,\"Amount\":0.000000}]}";
    }else {
        uint32_t tmp_ask = askordnum - 1;
        for (uint32_t j = 0; j < askordnum; ++j) {
            auto price = market_orderbook.staskorder(j).lprice();
            auto number = market_orderbook.staskorder(j).lnumber();
            double_t dprice = price / 1000000.0;
            double_t dnumber = number / 1000000.0;

            if (j != tmp_ask)
                orderbook += "{\"Price\":" + std::to_string(dprice) + ",\"Amount\":" + std::to_string(dnumber) + "},";
            else
                orderbook += "{\"Price\":" + std::to_string(dprice) + ",\"Amount\":" + std::to_string(dnumber) + "}]}";
        }
    }
    //XINFO("%s",orderbook.c_str());

    pthread_mutex_lock(&g_sessionlock);

    for (auto &g_sessionId : g_sessionIds) {
        if(g_sessionId.second.uiUserID == userid){
            FIX44::OrderBooks orderBooks;
            orderBooks.setField(FIX::Account(std::to_string(userid)));

            for (auto &i : g_sessionId.second.stSymbolInfo) {
                if(symbolid == i.uiSymbolID){
                    orderBooks.setField(FIX::Symbol(i.uiSymbolName));
                    break;
                }
            }

            orderBooks.setField(58, orderbook);
            orderBooks.setField(FIX::TransactTime(true));
            FIX::Session::sendToTarget(orderBooks, g_sessionId.first);
        }
    }
    pthread_mutex_unlock(&g_sessionlock);

    return 0;
}
