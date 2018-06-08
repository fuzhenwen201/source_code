/*************************************************************************
    > File Name: Orderexternal.cc
    > Author: Kevin
    > Mail: kevin.fu@fintechstar.cn
    > Created Time: Thu 11 Jan 2018 12:17:07 PM CST
 ************************************************************************/

#include "Subscriptionmsg.h"
#include "quickfix/fix44/ExecutionReport.h"
#include "quickfix/fix44/NewOrderSingle.h"
#include "quickfix/fix44/OrderCancelRequest.h"
#include "quickfix/fix44/OrderStatusRequest.h"


static std::map<std::string, uint32_t> order_index;
static RedisConfigInfo redisConfig = {};
static std::map<uint32_t,redisContext*> userMapRedisContext;
static std::map<std::string, std::vector<StSigIpinfo>> userMapIpinfo = {};
std::vector<std::string> g_username;
pthread_mutex_t g_usernamelock = PTHREAD_MUTEX_INITIALIZER;

static uint64_t GetCurrentTimestamp(){
    struct timespec ts;
    uint64_t tmp;

    clock_gettime(CLOCK_REALTIME, &ts);

    tmp = (uint64_t)(ts.tv_nsec / 1000);
    tmp += ts.tv_sec * 1000000;

    return tmp;
}

static std::string GetCurrentDate(){

    std::chrono::system_clock::time_point today = std::chrono::system_clock::now();
    auto tt = std::chrono::system_clock::to_time_t(today);
    char tmp[64];
    strftime(tmp,sizeof(tmp),"%Y%m%d",std::gmtime(&tt));
    return tmp;
}

Applications::Applications() {

}

Applications::~Applications() {
    if(NULL != pAcceptor_)
        pAcceptor_->stop();
    delete pAcceptor_;
    delete pLogFactory_;
    delete pMessageStoreFactory_;
    delete pSessionSettings_;
}

void Applications::onCreate( const FIX::SessionID& sessionID){
    //std::cout << "0 on Create:" << sessionID << std::endl;
}

void Applications::init(const std::string &configFile, STTnodeConfig& config, RedisConfigInfo& redisconfig) {
    try {

        redisConfig.redisport = redisconfig.redisport;
        redisConfig.redispass = redisconfig.redispass;
        redisConfig.redisaddr = redisconfig.redisaddr;

        pSessionSettings_ = new FIX::SessionSettings(configFile);

        pMessageStoreFactory_ = new FIX::FileStoreFactory(*pSessionSettings_);
        pLogFactory_ = new FIX::FileLogFactory(*pSessionSettings_);
        pAcceptor_ = new FIX::ThreadedSocketAcceptor(*this, *pMessageStoreFactory_, *pSessionSettings_, *pLogFactory_);
        pAcceptor_->start();
    }catch (FIX::ConfigError error ) {
        std::cout << error.what() << std::endl;
    }
}

void Applications::fromAdmin(const FIX::Message& message, const FIX::SessionID& sessionID)
    throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon )
{
    //std::cout << "1 from Admin:" << message << "\n";
    crack( message, sessionID );
}

void Applications::toAdmin( FIX::Message& message, const FIX::SessionID& sessionID) {
    const std::string& field = FIELD_GET_REF( message.getHeader(), MsgType);
    if(FIX::MsgType_Logon == field) {
        message.setField(FIX::NextExpectedMsgSeqNum(2));
    }
   // cout << "2 to Admin" << message << "\n";
}

void Applications::onLogon( const FIX::SessionID& sessionID ) {
    std::cout << "3 Logon - " << sessionID << std::endl;
}

void Applications::toApp( FIX::Message& message, const FIX::SessionID& sessionID )
    throw( FIX::DoNotSend ) {
    //std::cout << "4 to app: " << message << std::endl;
}

void Applications::fromApp( const FIX::Message& message, const FIX::SessionID& sessionID )
    throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType ) {
    //std::cout << "5 from app: " << message << std::endl;
    crack( message, sessionID );

}

void Applications::onLogout( const FIX::SessionID& sessionID ) {

    for (auto iter = userIdMapSessionId.begin(); iter != userIdMapSessionId.end(); iter++){
        if(iter->second == sessionID){
            pthread_mutex_lock(&userMaptnodes_lock);
            auto tnodeiter = userMaptnodes.find(iter->first);
            if(tnodeiter != userMaptnodes.end()){
                tnodeiter->second.tnode->StopConsumer();

                //tnode目前需要先收到消息才会执行stop consumer
                tnodeiter->second.tnode->PublishToMQ(tnodeiter->second.routingKey1.c_str(), const_cast<char *>("stop"), 4);
                tnodeiter->second.tnode->PublishToMQ(tnodeiter->second.routingKey2.c_str(), const_cast<char *>("stop"), 4);
                sleep(5);

                tnodeiter->second.tnode->Close();

                delete(tnodeiter->second.tnode);

                userMaptnodes.erase(tnodeiter);
            }
            pthread_mutex_unlock(&userMaptnodes_lock);
            userIdMapSessionId.erase(iter);
        }
        break;
    }

    std::cout << "6 Logout - " << sessionID << std::endl;

}

void Applications::onMessage (const FIX44::Logon& message, const FIX::SessionID& sessionID){
    if(sessionID.getBeginString() != "FIX.4.4") {
        throw FIX::IncorrectTagValue();
    }

    FIX::Username username = message.getField(553);
    FIX::Password password = message.getField(554);

    const std::string &m_name = username.getString();
    const std::string &clientIp = sessionID.getClientIp();

    std::string log = "Received a login request from user " + m_name + " IP:" + clientIp;
    SendLog2Server(2, log, g_tnode);

    auto ipinfo = userMapIpinfo.find(m_name);
    if(ipinfo != userMapIpinfo.end()){
        auto ip_table = ipinfo->second;
        uint32_t i = 0;
        for(i = 0; i < ip_table.size(); i++){
            if(ip_table[i].ucIpAddress == clientIp){
                break;
            }
        }
        if(i == ip_table.size()){
            log = "This IP " + clientIp + "is not allowed to connect";
            SendLog2Server(4, log, g_tnode);
            throw FIX::RejectLogon("This IP is not allowed to connect");
            return;
        }
    }
    MEProInterface::IntervalMsgHeader intervalMsgHeader;
    intervalMsgHeader.set_iflownumber(1);
    intervalMsgHeader.set_icmd(0);
    intervalMsgHeader.set_imsgtype(0);

    auto userLogonInfo = new MEProInterface::ST_USER_LOGIN_INFO;
    userLogonInfo->set_chusername(m_name);
    userLogonInfo->set_chpasswordtxt(password.getString());
    userLogonInfo->set_chaddressip(clientIp);
    intervalMsgHeader.set_allocated_stuserlogin(userLogonInfo);

    uint32_t i = 0;
    pthread_mutex_lock(&g_usernamelock);
    for(i = 0; i < g_username.size(); i++){
        if(g_username[i] == m_name){
            break;
        }
    }
    if(i == g_username.size()){
        g_username.push_back(m_name);
    }
    pthread_mutex_unlock(&g_usernamelock);

    std::string buff;
    intervalMsgHeader.SerializeToString(&buff);
    size_t len = buff.size();

    auto *buffer = new char[len + 1]();
    memcpy(buffer, buff.c_str(), len);

    pthread_mutex_lock(&g_lock);

    if(g_tnode->PublishToMQ("_USR_LOGIN.Req", buffer, len) != 0){
        XERROR("Push _USR_LOGIN.Req to MQ failed.");
    }
    delete[] buffer;

    struct timeval now{};
    struct timespec outtime{};
    gettimeofday(&now, nullptr);
    outtime.tv_sec = now.tv_sec + 10;
    if(pthread_cond_timedwait(&g_cond, &g_lock, &outtime) == ETIMEDOUT){
        pthread_mutex_unlock(&g_lock);

        //超时退出前删除登陆账户名
        pthread_mutex_lock(&g_usernamelock);
        for(i = 0; i < g_username.size(); i++){
            if(g_username[i] == m_name){
                g_username.erase(g_username.begin() + i);
                break;
            }
        }
        pthread_mutex_unlock(&g_usernamelock);

        throw FIX::RejectLogon("Timeout.");
    }

    std::map<std::string, StUserLogonResp>::iterator iter;
    iter = g_LogonResp.find(m_name);
    if(iter != g_LogonResp.end()){

        if(iter->second.ucLoginRsp == 0){

            StUserLogonResp logonResp = iter->second;
            g_LogonResp.erase(iter);
            pthread_mutex_unlock(&g_lock);

            struct timeval timeout = {2, 0};    //2s的超时时间
            auto pRedisContext = redisConnectWithTimeout(redisConfig.redisaddr.c_str(), redisConfig.redisport, timeout);
            if((nullptr == pRedisContext) || pRedisContext->err){
                if(pRedisContext)
                    XERROR("connect redis error:%s", pRedisContext->errstr);
                else
                    XERROR("connect redis error: can't allocate redis context.");
                return;
            }

            auto *pRedisReply = (redisReply*)redisCommand(pRedisContext, "AUTH %s", redisConfig.redispass.c_str());  //执行AUTH命令
            if(pRedisReply->type == 6){
                XERROR("Redis authentication failed.");
                freeReplyObject(pRedisReply);
                return;
            }else{
                XINFO("Redis certification is successful.");
            }
            freeReplyObject(pRedisReply);

            userMapRedisContext.insert(pair<uint32_t,redisContext*>(logonResp.uiUserID, pRedisContext));
            userMapIpinfo.insert(pair<std::string, std::vector<StSigIpinfo>>(m_name, logonResp.stIPInfo));

            pthread_mutex_lock(&userIdMapSymbolArry_lock);
            userIdMapSymbolArry.insert(std::pair<uint32_t, std::vector<StSymbolInfo>>(logonResp.uiUserID, logonResp.stSymbolInfo));
            pthread_mutex_unlock(&userIdMapSymbolArry_lock);
            pthread_mutex_lock(&userIdMapSessionId_lock);
            FIX::SessionID sessionid(sessionID.getBeginString(),sessionID.getSenderCompID(),sessionID.getTargetCompID(),
                                     sessionID.getSessionQualifier());
            userIdMapSessionId.insert(std::pair<uint32_t, FIX::SessionID>(logonResp.uiUserID, sessionid));
            pthread_mutex_unlock(&userIdMapSessionId_lock);

        }else{
            g_LogonResp.erase(iter);
            pthread_mutex_unlock(&g_lock);

            throw FIX::RejectLogon("Invalid credentials.");
        }
    } else{
        pthread_mutex_unlock(&g_lock);

        FIX::Session *se = FIX::Session::lookupSession(sessionID);
        se->disconnect();
        throw FIX::RejectLogon("The user was not found in the authentication information.");
    }
}

//options: 1:不支持的symbol,2:暂停提供该symbol服务,3:重复订单
static void SendRefusedOrdInfo(const std::string& orderID,const std::string& side, const FIX::SessionID& sessionID, uint32_t options){

    FIX44::ExecutionReport execreport;
    execreport.setField(17, "0");
    execreport.setField(37, "0");
    execreport.setField(11, orderID);
    execreport.setField(39, "8");
    execreport.setField(150, "8");
    execreport.setField(54, side);
    execreport.setField(6, "0");
    execreport.setField(14, "0");
    execreport.setField(151, "0");
    execreport.setField(FIX::TransactTime(true));
    if(options == 1) {
        execreport.setField(58, "The user does not support this symbol transactions.");
    }else if(options == 2){
        execreport.setField(58, "Pause to provide this symbol service.");
    }else if(options == 3) {
        execreport.setField(58, "Please check the order.");
    }
    FIX::Session::sendToTarget(execreport, sessionID);
}

void Applications::onMessage(const FIX44::NewOrderSingle &message, const FIX::SessionID &sessionID){
    if(sessionID.getBeginString() != "FIX.4.4") {
        throw FIX::IncorrectTagValue();
    }

    const auto &accountID = message.getField(1);
    const auto &symbol = message.getField(55);
    const auto &side = message.getField(54);
    const auto &ciordID = message.getField(11);
    std::string log = "Received a new order from user " + accountID + " orderID:" + ciordID;
    SendLog2Server(2, log, g_tnode);
    //将收到的字符串symbol转换为对应的symbolId
    uint32_t symbolid = 0;
    bool isupport = false;

    std::map<uint32_t, std::vector<StSymbolInfo>>::iterator iter;
    pthread_mutex_lock(&userIdMapSymbolArry_lock);
    iter = userIdMapSymbolArry.find(atoi(accountID.c_str()));
    if(iter != userIdMapSymbolArry.end()){
        std::vector<StSymbolInfo> symbolinfo = iter->second;
        pthread_mutex_unlock(&userIdMapSymbolArry_lock);
        for(uint32_t i = 0; i< symbolinfo.size(); i++){
            if(symbolinfo[i].uiSymbolName == symbol){
                isupport = true;
                symbolid = symbolinfo[i].uiSymbolID;
                break;
            }
        }
    }else{
        pthread_mutex_unlock(&userIdMapSymbolArry_lock);
    }

    if(!isupport){
        SendRefusedOrdInfo(ciordID, side, sessionID, 1);
        return;
    }

    //判断是否停止服务
    pthread_mutex_lock(&g_nasymbolock);
    std::string str_symbols = stop_symbols[accountID];
    pthread_mutex_unlock(&g_nasymbolock);//unlock

    if(!str_symbols.empty()){
        size_t pos = str_symbols.find(symbol);
        if(pos != std::string::npos) {
            SendRefusedOrdInfo(ciordID, side, sessionID, 2);
            return;
        }
    }

    auto iteredis = userMapRedisContext.find(static_cast<const unsigned int &>(stoi(accountID)));
    if(iteredis != userMapRedisContext.end()) {
        auto rediscontext = iteredis->second;
        //检查是否为重复订单
        std::string repeatkey = accountID + GetCurrentDate();
        auto pRedisReply = (redisReply *) redisCommand(rediscontext, "HGET %s %s", repeatkey.c_str(),
                                                       ciordID.c_str());  //执行HGET命令
        if (pRedisReply->type == REDIS_REPLY_STRING) {
            if (strncpy(pRedisReply->str, "pending", 7) == 0) {
                log = "This order of " + ciordID + " is a repeat order.";
                SendLog2Server(3, log, g_tnode);
                freeReplyObject(pRedisReply);
                return;
            } else if (strncpy(pRedisReply->str, "received", 8) == 0) {
                //已发ACK到client,但client未收到,进行重新发送

                freeReplyObject(pRedisReply);
                std::string key = accountID + ciordID;

                FIX44::ExecutionReport execreport;
                auto redisRep = (redisReply *) redisCommand(rediscontext, "HMGET %s 17 37 11 54", key.c_str());  //执行HGET命令
                if(redisRep->type == REDIS_REPLY_ARRAY){
                    execreport.setField(17, redisRep->element[0]->str);
                    execreport.setField(37, redisRep->element[1]->str);
                    execreport.setField(11, redisRep->element[2]->str);
                    execreport.setField(54, redisRep->element[3]->str);
                }

                freeReplyObject(redisRep);

                execreport.setField(39, "0");
                execreport.setField(150, "0");
                execreport.setField(6, "0");
                execreport.setField(14, "0");
                execreport.setField(151, "0");
                execreport.setField(FIX::TransactTime(true));
                FIX::Session::sendToTarget(execreport, sessionID);
                return;
            }
        }
        freeReplyObject(pRedisReply);

        const auto &orderType = message.getField(40);
        const auto &price = message.getField(44);
        const auto &orderQty = message.getField(38);

        double tmpprice = stod(price);
        auto iprice = static_cast<int64_t>(tmpprice * 1000000);
        double tmpnumber = stod(orderQty);
        auto inumber = static_cast<int64_t>(tmpnumber * 1000000);

        auto uside = static_cast<uint32_t>(atoi(side.c_str()));
        auto uordertype = static_cast<uint32_t>(atoi(orderType.c_str()));

        if (iprice <= 0 || inumber <= 0 || (uside != 1 && uside != 2) || (uordertype != 1 && uordertype != 2)) {
            SendRefusedOrdInfo(ciordID, side, sessionID, 3);
            return;
        }

        //将新订单写入redis中
        pRedisReply = (redisReply *) redisCommand(rediscontext, "HSET %s %s \"pending\"", repeatkey.c_str(),
                                                  ciordID.c_str());  //执行HSET命令
        freeReplyObject(pRedisReply);

        MEProInterface::IntervalMsgHeader intervalMsgHeader;
        intervalMsgHeader.set_iflownumber(1);
        intervalMsgHeader.set_icmd(1);
        intervalMsgHeader.set_imsgtype(0);

        auto submitOrder = new MEProInterface::ST_SUBMIT_ORDER;

        submitOrder->set_strorderid(ciordID);
        submitOrder->set_lsubmittime(GetCurrentTimestamp());
        submitOrder->set_uctranstype(uside);
        submitOrder->set_ucordertype(uordertype);
        submitOrder->set_uiuserid(static_cast<google::uint32>(stoul(accountID)));
        submitOrder->set_uiexchobjid(symbolid);
        if (orderType == "2") {//1:市场单,2:限价单
            submitOrder->set_lprice(iprice);
        }
        submitOrder->set_lnumber(inumber);

        //为新订单打上OrderIndex
        std::map<std::string, uint32_t>::iterator index;
        index = order_index.find(accountID + symbol);
        if (index != order_index.end()) {
            ++index->second;
            submitOrder->set_uiorderindex(index->second);
        } else {
            submitOrder->set_uiorderindex(1);
            order_index.insert(pair<std::string, uint32_t>(accountID + symbol, 1));
        }
        intervalMsgHeader.set_allocated_stsubmitorder(submitOrder);

        std::string buff;
        intervalMsgHeader.SerializeToString(&buff);

        size_t len = buff.size();
        char buffer[2048] = {};
        memcpy(buffer, buff.c_str(), len);

        std::string binding_key = "_OrderInfo.Sub." + accountID;
        //将订单信息发送到MQ中
        if (g_tnode->PublishToMQ(binding_key.c_str(), buffer, len) != 0) {
            XERROR("Push new order to MQ failed.");
        }
    }
}

void Applications::onMessage(const FIX44::OrderCancelRequest &message, const FIX::SessionID &sessionID) {
    if(sessionID.getBeginString() != "FIX.4.4") {
        throw FIX::IncorrectTagValue();
    }

    const auto &orgorderID = message.getField(41);

    std::string log = "Received a request message from the user to cancel the order " + orgorderID;
    SendLog2Server(2, log, g_tnode);

    const auto &excelId = message.getField(17);
    const auto &orderID = message.getField(37);
    const auto &symbol = message.getField(55);
    const auto &side = message.getField(54);
    const auto &price = message.getField(44);

    double tmpprice = stod(price);
    auto lprice = static_cast<int64_t>(tmpprice * 1000000);

    auto uside = static_cast<uint32_t>(atoi(side.c_str()));
    if((uside != 1 && uside != 2) || lprice <= 0){
        SendRefusedOrdInfo(orgorderID, side, sessionID, 3);
        return;
    }

    MEProInterface::IntervalMsgHeader intervalMsgHeader;
    intervalMsgHeader.set_iflownumber(1);
    intervalMsgHeader.set_icmd(2);
    intervalMsgHeader.set_imsgtype(0);

    auto ordercancelreq = new MEProInterface::ST_SUBMIT_CAMCEL_ORDER;
    ordercancelreq->set_strorderid(orgorderID);
    ordercancelreq->set_lorderidfrme(stoull(orderID));
    ordercancelreq->set_uiorderindex(stoul(excelId));
    ordercancelreq->set_uctranstype(uside);

    ordercancelreq->set_lprice(lprice);

    uint32_t userId = 0;
    pthread_mutex_lock(&userIdMapSessionId_lock);
    for(auto itr = userIdMapSessionId.begin(); itr != userIdMapSessionId.end(); itr++){
        if(itr->second == sessionID){
            userId = itr->first;
            break;
        }
    }
    pthread_mutex_unlock(&userIdMapSessionId_lock);

    uint32_t symbolid = 0;
    std::map<uint32_t, std::vector<StSymbolInfo>>::iterator iter;
    pthread_mutex_lock(&userIdMapSymbolArry_lock);
    iter = userIdMapSymbolArry.find(userId);
    if(iter != userIdMapSymbolArry.end()) {
        std::vector<StSymbolInfo> symbolinfo = iter->second;
        pthread_mutex_unlock(&userIdMapSymbolArry_lock);
        for(uint32_t i = 0; i < symbolinfo.size(); i++) {
            if (symbolinfo[i].uiSymbolName == symbol) {
                symbolid = symbolinfo[i].uiSymbolID;
                break;
            }
        }

        ordercancelreq->set_uiexchobjid(symbolid);
        ordercancelreq->set_uiuserid(userId);
        intervalMsgHeader.set_allocated_stcancelorder(ordercancelreq);

        std::string buff;
        intervalMsgHeader.SerializeToString(&buff);

        size_t len = buff.size();
        char buffer[2048] = {};
        memcpy(buffer, buff.c_str(), len);

        std::string binding_key = "_OrderInfo.Sub." + std::to_string(userId);
        if(g_tnode->PublishToMQ(binding_key.c_str(), buffer, len) != 0){
            XERROR("push cancel order request to MQ failed.");
        }
        return;
    }
    pthread_mutex_unlock(&userIdMapSymbolArry_lock);
}

void Applications::onMessage(const FIX44::OrderStatusRequest &message, const FIX::SessionID &sessionID) {
    if(sessionID.getBeginString() != "FIX.4.4") {
        throw FIX::IncorrectTagValue();
    }

    const auto &accoutid = message.getField(1);
    const auto &ciorderID = message.getField(11);
    const auto &orderID = message.getField(37);
    const auto &symbol = message.getField(55);

    std::string log = "Received a request message from the user for a query order " + ciorderID;
    SendLog2Server(2, log, g_tnode);

    MEProInterface::IntervalMsgHeader intervalMsgHeader;
    intervalMsgHeader.set_iflownumber(1);
    intervalMsgHeader.set_icmd(3);
    intervalMsgHeader.set_imsgtype(0);

    auto orderstatusreq = new MEProInterface::ST_QUERY_ORDER_INFO;
    orderstatusreq->set_uiuserid(stoul(accoutid));
    orderstatusreq->set_strorderid(ciorderID);
    orderstatusreq->set_lorderidfrme(stoull(orderID));

    uint32_t symbolid = 0;
    std::map<uint32_t, std::vector<StSymbolInfo>>::iterator iter;
    pthread_mutex_lock(&userIdMapSymbolArry_lock);
    iter = userIdMapSymbolArry.find(stoul(accoutid));
    if(iter == userIdMapSymbolArry.end()){
        pthread_mutex_unlock(&userIdMapSymbolArry_lock);
        log = accoutid  + " is not found in the connection session.";
        SendLog2Server(4, log, g_tnode);
    }else{
        std::vector<StSymbolInfo> symbolinfo = iter->second;
        pthread_mutex_unlock(&userIdMapSymbolArry_lock);

        for(uint32_t i = 0; i < symbolinfo.size(); i++) {
            if (symbol == symbolinfo[i].uiSymbolName) {
                symbolid = symbolinfo[i].uiSymbolID;
            }
        }

        orderstatusreq->set_uiexchobjid(symbolid);
        intervalMsgHeader.set_allocated_stqueryorder(orderstatusreq);

        std::string buff;
        intervalMsgHeader.SerializeToString(&buff);

        size_t len = buff.size();
        char buffer[512] = {};
        memcpy(buffer, buff.c_str(), len);
        std::string routing_key = "_QryOrder.Req." + accoutid;
        if(g_tnode->PublishToMQ(routing_key.c_str(), buffer, len) != 0){
            XERROR("Push query order to MQ failed.");
        }
    }
}
