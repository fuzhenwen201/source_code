/*************************************************************************
    > File Name: Subscriptionmsg.cc
    > Author: Kevin
    > Mail: kevin.fu@fintechstar.cn
    > Created Time: Wed 24 Jan 2018 04:10:35 PM CST
 ************************************************************************/

#include "Subscriptionmsg.h"
#include "quickfix/fix44/ExecutionReport.h"
#include "quickfix/fix44/OrderCancelReject.h"

pthread_cond_t g_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t g_nasymbolock = PTHREAD_MUTEX_INITIALIZER;
std::map<std::string, std::string> stop_symbols = {};
std::map<std::string, StUserLogonResp> g_LogonResp = {};

snetwork_xservice_tnode::TNode *g_tnode = nullptr;
std::map<uint32_t,RoutingAndTnode> userMaptnodes = {};
std::map<uint32_t, FIX::SessionID> userIdMapSessionId;
std::map<uint32_t, std::vector<StSymbolInfo>> userIdMapSymbolArry;
pthread_mutex_t userIdMapSessionId_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t userIdMapSymbolArry_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t userMaptnodes_lock = PTHREAD_MUTEX_INITIALIZER;
static STTnodeConfig tnodeConfig = {};
static RedisConfigInfo redisConfig = {};
static std::map<uint32_t,redisContext*> userMapRedisContext;

char g_hostname[128] = {};

redisContext *g_pRedisContext = nullptr;

static std::string GetCurrentDate(){

    std::chrono::system_clock::time_point today = std::chrono::system_clock::now();
    auto tt = std::chrono::system_clock::to_time_t(today);
    char tmp[64];
    strftime(tmp,sizeof(tmp),"%Y%m%d",std::gmtime(&tt));
    return tmp;
}
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
    loginfo->set_chmoduleindex(1);
    loginfo->set_uireptime(GetCurrentTimestamp());
    loginfo->set_strlogcontent(text);
    logInterHeader.set_allocated_stmaintainlog(loginfo);
    std::string buff;
    logInterHeader.SerializeToString(&buff);

    size_t len = buff.size();
    char buffer[2048] = {};
    memcpy(buffer, buff.c_str(), len);
    char binding_key[128] = {};
    sprintf(binding_key, "_MaintainInfo.Rep.1.%s", g_hostname);

    //将订单信息发送到MQ中
    if (tnode->PublishToMQ(binding_key, buffer, len) != 0) {
        XERROR("Push new order to MQ failed.");
    }

    return true;
}

OrderService::OrderService() = default;

OrderService::~OrderService() = default;

void OrderService::Init(STTnodeConfig& config, RedisConfigInfo& redisconfig, std::string& stid) {

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
    g_tnode = new snetwork_xservice_tnode::TNode(tnodeConfig);

    g_tnode->Init();

    std::string queue_name = "ORDAPI_USERLOGINRESP" + stid;

    auto *logonInfo = new LogonInfoConsumer("_USR_LOGIN.Resp", queue_name.c_str());
    g_tnode->AddTNodeConsumer(logonInfo);

    g_tnode->RunConsumer();

}

static std::map<std::string, std::string> runStatus = {};

void split( const string &str, const string &delimiters, std::map<std::string, std::string> &elems) {
    string::size_type pos, prev = 0;
    while ( ( pos = str.find_first_of(delimiters, prev) ) != string::npos ) {

        if ( pos > prev ) {

            if(str[pos] == '(') {

                std::string userid = str.substr(prev, pos - prev);
                prev = pos + 1;
                size_t  tmp_post = str.find(':', prev);
                size_t  tmp_post2 = str.find(',', prev);
                prev = tmp_post + 1;
                std::string symbol, symbols;
                if(tmp_post2 == std::string::npos){
                    symbol = str.substr(prev);
                }else {
                    symbol = str.substr(prev, tmp_post2 - prev);
                    pos = tmp_post2;
                }
                symbols = elems[userid];
                elems[userid] = symbols + symbol + ",";
            }
        }
        prev = pos + 1;
    }
}

static void SendRunStatusInfo(vector<std::string>& elements){
    for(uint32_t i = 0; i < elements.size(); i++) {
        std::string m_hostname = elements[i];
        auto pRedisReply = (redisReply *) redisCommand(g_pRedisContext, "HGET ME_%s RunStatus", m_hostname.c_str());
        if(pRedisReply->type == REDIS_REPLY_NIL) {
            std::string log = m_hostname + " server did not find the RunStatus setting.";
            SendLog2Server(4, log, g_tnode);
            freeReplyObject(pRedisReply);
            continue;
        }
        std::string run_status = pRedisReply->str;
        freeReplyObject(pRedisReply);

        std::map<std::string, std::string>::iterator iter;
        iter = runStatus.find(m_hostname);
        if(iter == runStatus.end()) {

            runStatus[m_hostname] = run_status;
        }else{

            if(run_status == runStatus[m_hostname]){
                continue;
            }

            runStatus[m_hostname] = run_status;

            auto redisre = (redisReply *) redisCommand(g_pRedisContext, "HGET ME_%s ME_Server", m_hostname.c_str());
            if(redisre->type == REDIS_REPLY_NIL){
                std::string log = m_hostname + " server did not find the ME_Server setting.";
                SendLog2Server(4, log, g_tnode);
                freeReplyObject(redisre);
                continue;
            }
            std::string getsymbols = redisre->str;
            freeReplyObject(redisre);

            std::map<std::string, std::string> restart_symbols;
            if(run_status == "1"){
               split(getsymbols, "(", restart_symbols);
               for (auto &restart_symbol : restart_symbols) {
                   std::string symbo = restart_symbol.second;

                   //lock name_symbols
                   pthread_mutex_lock(&g_nasymbolock);
                   size_t position = stop_symbols[restart_symbol.first].find(symbo);
                   stop_symbols[restart_symbol.first].erase(position, symbo.length());
                   pthread_mutex_unlock(&g_nasymbolock);//unlock
               }
            }else{
                //lock name_symbols
                pthread_mutex_lock(&g_nasymbolock);
                //name_symbols.clear();
                split(getsymbols, "(", stop_symbols);
                pthread_mutex_unlock(&g_nasymbolock);//unlock
            }

            FIX::Message message;
            FIX::Header &header = message.getHeader();

            //lock g_sessionlock
            pthread_mutex_lock(&userIdMapSessionId_lock);
            for(auto iter2 = userIdMapSessionId.begin(); iter2 != userIdMapSessionId.end(); iter2++) {
                FIX::SessionID sessionID = iter2->second;

                if(run_status == "1") {
                    std::string symbolstr = restart_symbols[std::to_string(iter2->first)];
                    if (symbolstr.empty()) {
                        continue;
                    }
                    message.setField(55, symbolstr);
                    message.setField(58, "Open the service.");
                } else {
                    std::string symbolstr = stop_symbols[std::to_string(iter2->first)];
                    if (symbolstr.empty()) {
                        continue;
                    }
                    message.setField(55, symbolstr);
                    message.setField(58, "Close the service.");
                }

                header.setField(FIX::BeginString(sessionID.getBeginString()));
                header.setField(FIX::SenderCompID(sessionID.getSenderCompID()));
                header.setField(FIX::TargetCompID(sessionID.getTargetCompID()));
                header.setField(FIX::MsgType("G1005"));

                message.setField(146, "0");

                FIX::Session::sendToTarget(message, sessionID);

            }
            pthread_mutex_unlock(&userIdMapSessionId_lock);//unlock

        }
    }
}

void OrderService::Run() {

    gethostname(g_hostname, sizeof(g_hostname));

    auto *pRedisReply = (redisReply *)redisCommand(g_pRedisContext, "HGET OrderAPI_%s_0 LastRunTime",g_hostname);  //执行HGET命令
    if(pRedisReply->type == REDIS_REPLY_NIL){
        std::string log = "The backoffice does not configure this server.";
        SendLog2Server(4, log, g_tnode);
        return;
    }
    freeReplyObject(pRedisReply);

    while (1) {
        time_t now = time(0);
        tm *gmtm = gmtime(&now);
        char *dt = asctime(gmtm);
        char buff[28] = {};
        strncpy(buff, dt, 24);

        //定时上报自身的服务器状态
        pRedisReply = (redisReply *) redisCommand(g_pRedisContext, "HSET OrderAPI_%s_0 LastRunTime %s",
                                                  g_hostname, buff);  //执行HSET命令
        freeReplyObject(pRedisReply);

        //获取所有ME服务器的hostname
        std::string pausesymbol = {};

        vector<std::string> elements = {};
        pRedisReply = (redisReply *)redisCommand(g_pRedisContext, "SMEMBERS _ME_");  //执行SMEMBERS命令
        if(pRedisReply->type == REDIS_REPLY_ARRAY){
            for(uint32_t i = 0; i < pRedisReply->elements; i++){
                elements.emplace_back(pRedisReply->element[i]->str);
            }
        }
        freeReplyObject(pRedisReply);
        if(!elements.empty()){
            SendRunStatusInfo(elements);
        }

        sleep(2);
    }
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

    //解析protobuf消息
    StUserLogonResp userlogonResp = {};
    MEProInterface::IntervalMsgHeader intervalMsgHeader;
    intervalMsgHeader.ParseFromArray(pMsg, nMsgLen);
    if(!intervalMsgHeader.has_stloginresp()){
        XERROR("There is no LOGIN RESP information in the message.");
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

    auto tnodeit = userMaptnodes.find(userlogonResp.uiUserID);
    if(tnodeit != userMaptnodes.end()) {
        tnodeit->second.tnode->StopConsumer();

        //tnode目前需要先收到消息才会执行stop consumer
        tnodeit->second.tnode->PublishToMQ(tnodeit->second.routingKey1.c_str(), const_cast<char *>("stop"), 4);
        tnodeit->second.tnode->PublishToMQ(tnodeit->second.routingKey2.c_str(), const_cast<char *>("stop"), 4);
        sleep(3);

        tnodeit->second.tnode->Close();

        delete(tnodeit->second.tnode);

        userMaptnodes.erase(tnodeit);
    }

    if(userlogonResp.ucLoginRsp == 0) {
        auto newtnode = new snetwork_xservice_tnode::TNode(tnodeConfig);
        newtnode->Init();
        std::string routingkey1 = "_OrderInfo.Resp." + std::to_string(userlogonResp.uiUserID);
        std::string queuename1 = "ORDAPI_OrderInfoResp" + std::to_string(userlogonResp.uiUserID);
        auto *orderRespconsumer = new OrderRespConsumer(routingkey1.c_str(), queuename1.c_str());
        newtnode->AddTNodeConsumer(orderRespconsumer);

        std::string routingkey2 = "_QryOrder.Resp." + std::to_string(userlogonResp.uiUserID);
        std::string queuename2 = "ORDAPI_QryOrderResp" + std::to_string(userlogonResp.uiUserID);
        auto *queryRespconsumer = new QueryRespConsumer(routingkey2.c_str(), queuename2.c_str());
        newtnode->AddTNodeConsumer(queryRespconsumer);

        newtnode->RunConsumer();

        RoutingAndTnode tnode_routingkey = {};
        tnode_routingkey.tnode = newtnode;
        tnode_routingkey.routingKey1 = routingkey1;
        tnode_routingkey.routingKey2 = routingkey2;
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
    std::string log = name + ":" + std::to_string(userlogonResp.uiUserID) + " login authentication result" +
            std::to_string(userlogonResp.ucLoginRsp) + " message from MQ.";
    SendLog2Server(2, log, g_tnode);

    for (uint32_t j = 0; j < ipnum; ++j) {
        StSigIpinfo sigipinfo = {};
        sigipinfo.wIpType = userLogonResp.stipinfo(j).wiptype();
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
    if (iter != g_LogonResp.end()) {
        g_LogonResp.erase(iter);
    }
    g_LogonResp.insert(std::pair<std::string, StUserLogonResp>(name, userlogonResp));
    pthread_mutex_unlock(&g_lock);


    pthread_cond_broadcast(&g_cond);

    return 0;
}

OrderRespConsumer::OrderRespConsumer(const char *bindingkey, const char *queuename) :
        m_bindingkey(bindingkey),m_queuename(queuename){

}

OrderRespConsumer::~OrderRespConsumer(){

}

std::string OrderRespConsumer::GetQueueName() {
    return m_queuename;
}

std::string OrderRespConsumer::GetBindingkey() {
    return m_bindingkey;
}

uint32_t OrderRespConsumer::ConsumerData(char *pMsg, int32_t nMsgLen) {
    assert(pMsg != nullptr);


    //解析protobuf消息
    MEProInterface::IntervalMsgHeader intervalMsgHeader;
    intervalMsgHeader.ParseFromArray(pMsg, nMsgLen);
    if(!intervalMsgHeader.has_storderresp()){
        XERROR("There is no Order Response information in the message.");
        return 1;
    }

    MEProInterface::ST_SUBMIT_ORDER_RESP subOrderResp = intervalMsgHeader.storderresp();

    auto& strOrdID = subOrderResp.strorderid();
    uint32_t subordtype = subOrderResp.uisubmitorderiype();
    uint64_t ordIdFromMe = subOrderResp.lorderidfrme();
    uint32_t ordindex = subOrderResp.uiorderindex();
    uint32_t ordstatus = subOrderResp.uiorderstatus();
    auto& text = subOrderResp.strtext();
    uint32_t userId = subOrderResp.uiuserid();
    uint32_t side = subOrderResp.uctranstype();

    std::string log = "Received the " + strOrdID +" order response message from MQ.";
    SendLog2Server(2, log, g_tnode);

    FIX44::ExecutionReport execreport;
    FIX44::OrderCancelReject cancelReject;
    if(subordtype == 0){
        execreport.setField(17, std::to_string(ordindex));
        execreport.setField(37, std::to_string(ordIdFromMe));
        execreport.setField(11, strOrdID);
        if(ordstatus == 0) {
            execreport.setField(39, "0");
            execreport.setField(150, "M");
        }else{
            execreport.setField(39, "8");
            execreport.setField(150, "M");
            execreport.setField(58, text);
        }

        execreport.setField(54, std::to_string(side));
        execreport.setField(6, "0");
        execreport.setField(14, "0");
        execreport.setField(151, "0");
    }else{
        if(ordstatus == 0){
            execreport.setField(11, "0");
            execreport.setField(41, strOrdID);
            execreport.setField(17, "0");
            execreport.setField(37, std::to_string(ordIdFromMe));
            execreport.setField(39, "0");
            execreport.setField(150, "N");
            execreport.setField(54, std::to_string(side));
            execreport.setField(6, "0");
            execreport.setField(14, "0");
            execreport.setField(151, "0");
        }else{
            cancelReject.setField(11, "0");
            cancelReject.setField(41, strOrdID);
            cancelReject.setField(37, std::to_string(ordIdFromMe));
            cancelReject.setField(39, "8");
            cancelReject.setField(58, text);
            cancelReject.setField(434, "1");
        }
    }

    pthread_mutex_lock(&userIdMapSessionId_lock);
    auto iter = userIdMapSessionId.find(userId);
    if(iter != userIdMapSessionId.end()) {
        execreport.setField(FIX::TransactTime(true));
        if (subordtype != 0 && ordstatus != 0) {
            FIX::Session::sendToTarget(cancelReject, iter->second);
        }else {
            FIX::Session::sendToTarget(execreport, iter->second);
        }

        pthread_mutex_unlock(&userIdMapSessionId_lock);

        auto iteredis = userMapRedisContext.find(userId);
        if (iteredis != userMapRedisContext.end()) {
            auto rediscontext = iteredis->second;
            std::string repakey = std::to_string(userId) + GetCurrentDate();
            if (ordstatus == 8) {
                auto pRedisReply = (redisReply *) redisCommand(rediscontext, "HDEL %s %s", repakey.c_str(),
                                                               strOrdID.c_str());  //执行DEL命令
                freeReplyObject(pRedisReply);
            } else {
                //更新在redis中的订单状态
                auto pRedisReply = (redisReply *) redisCommand(rediscontext, "HSET %s %s \"received\"",
                                                               repakey.c_str(), strOrdID.c_str());  //执行HSET命令
                freeReplyObject(pRedisReply);

                std::string key = std::to_string(userId) + strOrdID;
                pRedisReply = (redisReply *) redisCommand(rediscontext, "HMSET %s 17 %s 37 %s 11 %s 54 %s", key.c_str(),
                                                          std::to_string(ordindex).c_str(),
                                                          std::to_string(ordIdFromMe).c_str(),
                                                          strOrdID.c_str(), std::to_string(side).c_str());
                freeReplyObject(pRedisReply);
            }
        }
        return 0;
    }
    pthread_mutex_unlock(&userIdMapSessionId_lock);

    return 1;

}

QueryRespConsumer::QueryRespConsumer(const char *bindingkey, const char *queuename) :
        m_bindingkey(bindingkey),m_queuename(queuename){

}

QueryRespConsumer::~QueryRespConsumer(){

}

std::string QueryRespConsumer::GetQueueName() {
    return m_queuename;
}

std::string QueryRespConsumer::GetBindingkey() {
    return m_bindingkey;
}

uint32_t QueryRespConsumer::ConsumerData(char *pMsg, int32_t nMsgLen) {
    assert(pMsg != nullptr);

    MEProInterface::IntervalMsgHeader intervalMsgHeader;
    intervalMsgHeader.ParseFromArray(pMsg, nMsgLen);
    if(!intervalMsgHeader.has_stqueryresp()){
        XERROR("There is no Query Response information in the message.");
        return 1;
    }
    MEProInterface::ST_QUERY_ORDER_RES queryOrdRes = intervalMsgHeader.stqueryresp();

    uint32_t userID = queryOrdRes.uiuserid();
    const auto& orderID = queryOrdRes.strorderid();
    uint32_t symbolID = queryOrdRes.uiexchobjid();
    uint32_t tranStatus = queryOrdRes.wtransstatus();
    uint32_t transType = queryOrdRes.uctranstype();
    int64_t totalNumber = queryOrdRes.lorgnumber();
    int64_t execNumber = queryOrdRes.lavgtransfernum();
    double_t orgPrice = queryOrdRes.lorgprice() / 1000000.0;
    double_t orgNumber = totalNumber / 1000000.0;
    double_t avgPrice = queryOrdRes.lavgtransferprice() / 1000000.0;
    double_t avgNumber = execNumber / 1000000.0;
    double_t unexecNum = (totalNumber - execNumber) / 1000000.0;
    uint64_t orderIDFrME = queryOrdRes.lorderidfrme();

    std::string log = "Received the " + orderID +" order query response message from MQ.";
    SendLog2Server(2, log, g_tnode);

    FIX44::ExecutionReport execreport;
    execreport.setField(11, orderID);
    execreport.setField(37, std::to_string(orderIDFrME));
    execreport.setField(17, "0");
    if(tranStatus == 0){
        execreport.setField(39, "A");
        execreport.setField(150, "A");
    }else {
        execreport.setField(39, std::to_string(tranStatus));
        execreport.setField(150, std::to_string(tranStatus));
    }

    if(transType == 0)
        execreport.setField(54, "0");
    else
        execreport.setField(54, "1");

    execreport.setField(44, std::to_string(orgPrice));
    execreport.setField(38, std::to_string(orgNumber));
    execreport.setField(6, std::to_string(avgPrice));
    execreport.setField(14,std::to_string(avgNumber));
    execreport.setField(151, std::to_string(unexecNum));

    std::string symbolname = {};
    std::map<uint32_t, std::vector<StSymbolInfo>>::iterator symiter;
    pthread_mutex_lock(&userIdMapSymbolArry_lock);
    symiter = userIdMapSymbolArry.find(userID);
    if(symiter != userIdMapSymbolArry.end()){
        std::vector<StSymbolInfo> symbolinfo = symiter->second;
        for (uint32_t i = 0; i < symbolinfo.size(); i++) {
            if(symbolinfo[i].uiSymbolID == symbolID) {
                symbolname = symbolinfo[i].uiSymbolName;
                break;
            }
        }
    }
    pthread_mutex_unlock(&userIdMapSymbolArry_lock);

    pthread_mutex_lock(&userIdMapSessionId_lock);
    auto iter = userIdMapSessionId.find(userID);
    if(iter != userIdMapSessionId.end()) {
        execreport.setField(FIX::Symbol(symbolname));
        execreport.setField(FIX::TransactTime(true));
        FIX::Session::sendToTarget(execreport, iter->second);
    }
    pthread_mutex_unlock(&userIdMapSessionId_lock);
    return 0;
}