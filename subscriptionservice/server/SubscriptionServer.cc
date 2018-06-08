/*************************************************************************
    > File Name: SubscriptionServer.cc
    > Author: Kevin
    > Mail: kevin.fu@fintechstar.cn
    > Created Time: Thu 11 Jan 2018 12:17:07 PM CST
 ************************************************************************/

#include "SubscriptionService.h"
#include "quickfix/fix44/Logout.h"

std::vector<std::string> g_username;
pthread_mutex_t g_usernamelock = PTHREAD_MUTEX_INITIALIZER;
static std::map<std::string, std::vector<StSigIpinfo>> userMapIpinfo = {};

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

void Applications::init(const std::string &configFile, STTnodeConfig config) {
    try {
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
   // std::cout << "1 from Admin:" << message << "\n";
    crack( message, sessionID );
}

void Applications::toAdmin( FIX::Message& message, const FIX::SessionID& sessionID) {
    const std::string& field = FIELD_GET_REF( message.getHeader(), MsgType);
    if(FIX::MsgType_Logon == field) {
        message.setField(FIX::NextExpectedMsgSeqNum(2));
    }else if(FIX::MsgType_Logout == field){

    }
    //cout << "2 to Admin" << message << "\n";
}

void Applications::onLogon( const FIX::SessionID& sessionID ) {
    std::cout << "3 Logon - " << sessionID << std::endl;
}

void Applications::toApp( FIX::Message& message, const FIX::SessionID& sessionID )
    throw( FIX::DoNotSend ) {
    //std::cout << "4 OUT: " << message << std::endl;
}

void Applications::fromApp( const FIX::Message& message, const FIX::SessionID& sessionID )
    throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType ) {
   // std::cout << "5 IN: " << message << std::endl;
    crack( message, sessionID );

}

void Applications::onLogout( const FIX::SessionID& sessionID ) {
    FIX::SessionID sessionid(sessionID.getBeginString(),sessionID.getSenderCompID(),sessionID.getTargetCompID(),
                             sessionID.getSessionQualifier());
    auto iter = g_sessionIds.find(sessionid);
    if(iter != g_sessionIds.end()){
        pthread_mutex_lock(&userMaptnodes_lock);
        auto tnodeiter = userMaptnodes.find(iter->second.uiUserID);
        if(tnodeiter != userMaptnodes.end()){
            tnodeiter->second.tnode->StopConsumer();
            //tnode目前需要先收到消息才会执行stop
            tnodeiter->second.tnode->PublishToMQ(tnodeiter->second.routingKey1.c_str(), const_cast<char *>("stop"), 4);
            tnodeiter->second.tnode->PublishToMQ(tnodeiter->second.routingKey2.c_str(), const_cast<char *>("stop"), 4);
            tnodeiter->second.tnode->PublishToMQ(tnodeiter->second.routingKey3.c_str(), const_cast<char *>("stop"), 4);
            tnodeiter->second.tnode->PublishToMQ(tnodeiter->second.routingKey4.c_str(), const_cast<char *>("stop"), 4);
            sleep(5);
            tnodeiter->second.tnode->Close();
            delete(tnodeiter->second.tnode);
            userMaptnodes.erase(tnodeiter);
        }
        pthread_mutex_unlock(&userMaptnodes_lock);
        g_sessionIds.erase(iter);
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

    auto *userLogonInfo = new MEProInterface::ST_USER_LOGIN_INFO;
    userLogonInfo->set_chusername(m_name);
    userLogonInfo->set_chpasswordtxt(password.getString());
    userLogonInfo->set_chaddressip(clientIp);
    intervalMsgHeader.set_allocated_stsubcriuserlogin(userLogonInfo);

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
    if(g_tnode->PublishToMQ("_USR_LOGIN.Req.Subscripte", buffer, len) != 0){
        XERROR("Push _USR_LOGIN.Req.Subscripte to MQ failed.");
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

            userMapIpinfo.insert(pair<std::string, std::vector<StSigIpinfo>>(m_name, logonResp.stIPInfo));

            FIX::SessionID sessionid(sessionID.getBeginString(),sessionID.getSenderCompID(),sessionID.getTargetCompID(),
                                     sessionID.getSessionQualifier());
            pthread_mutex_lock(&g_sessionlock);
            g_sessionIds.insert(std::pair<FIX::SessionID,StUserLogonResp>(sessionid, logonResp));
            pthread_mutex_unlock(&g_sessionlock);

        }else{
            g_LogonResp.erase(iter);
            pthread_mutex_unlock(&g_lock);

            throw FIX::RejectLogon("Invalid credentials.");
        }
    }else{
        pthread_mutex_unlock(&g_lock);

        FIX::Session *se = FIX::Session::lookupSession(sessionID);
        se->disconnect();
        throw FIX::RejectLogon("The user was not found in the authentication information.");
    }

}

void Applications::onMessage (const FIX44::HistQuotReq& message, const FIX::SessionID& sessionID){
    if(sessionID.getBeginString() != "FIX.4.4") {
        throw FIX::IncorrectTagValue();
    }

    const auto& accoutId = message.getField(1);
    const auto& clordId = message.getField(11);
    const auto& orderId = message.getField(37);
    const auto& symbol = message.getField(55);

    std::string log = "Received a request about the history order " + clordId;
    SendLog2Server(2, log, g_tnode);

    MEProInterface::IntervalMsgHeader intervalMsgHeader;
    intervalMsgHeader.set_iflownumber(1);
    intervalMsgHeader.set_icmd(7);
    intervalMsgHeader.set_imsgtype(0);

    auto queryOrderInfo = new MEProInterface::ST_QUERY_ORDER_INFO;
    queryOrderInfo->set_uiuserid(stoul(accoutId.c_str()));
    queryOrderInfo->set_strorderid(clordId);
    queryOrderInfo->set_lorderidfrme(stoull(orderId));

    std::map<FIX::SessionID, StUserLogonResp>::iterator iter;
    iter = g_sessionIds.find(sessionID);
    uint32_t symbolid = 0;

    if(iter != g_sessionIds.end()){
        StUserLogonResp logonResp = iter->second;
        for(uint32_t i = 0; i < logonResp.stSymbolInfo.size(); i++) {
            if(iter->second.stSymbolInfo[i].uiSymbolName == symbol){
                symbolid = iter->second.stSymbolInfo[i].uiSymbolID;
                queryOrderInfo->set_uiexchobjid(symbolid);
                intervalMsgHeader.set_allocated_stqueryhistoryorder(queryOrderInfo);

                std::string buff;
                intervalMsgHeader.SerializeToString(&buff);

                size_t len = buff.size();
                auto *buffer = new char[len + 1]();
                memcpy(buffer, buff.c_str(), len);

                std::string routing_key = "_QryOrder.Req." + accoutId;
                if(g_tnode->PublishToMQ(routing_key.c_str(), buffer, len) != 0){
                    XERROR("Push _QryOrder.Req to MQ failed.");
                }
                delete[] buffer;
                return;
            }
        }
    }
}

void Applications::onMessage (const FIX44::OrdRetransReq& message, const FIX::SessionID& sessionID){
    if(sessionID.getBeginString() != "FIX.4.4") {
        throw FIX::IncorrectTagValue();
    }

    const auto& accoutId = message.getField(1);
    const auto& orderId = message.getField(37);
    const auto& symbol = message.getField(55);

    std::string log = "Received a retransmission request for a transaction order " + orderId;
    SendLog2Server(2, log, g_tnode);

    MEProInterface::IntervalMsgHeader intervalMsgHeader;
    intervalMsgHeader.set_iflownumber(1);
    intervalMsgHeader.set_icmd(8);
    intervalMsgHeader.set_imsgtype(0);

    auto repTransOrderRequest = new MEProInterface::ST_REP_TRANS_ORDER_REQUEST;
    repTransOrderRequest->set_uiuserid(stoul(accoutId));
    repTransOrderRequest->set_ucexchid(stoull(orderId));
    std::map<FIX::SessionID, StUserLogonResp>::iterator iter;
    iter = g_sessionIds.find(sessionID);
    uint32_t symbolid = 0;

    if(iter != g_sessionIds.end()){

        StUserLogonResp logonResp = iter->second;
        for(uint32_t i = 0; i < logonResp.stSymbolInfo.size(); i++) {
            if(logonResp.stSymbolInfo[i].uiSymbolName == symbol){

                symbolid = iter->second.stSymbolInfo[i].uiSymbolID;
                repTransOrderRequest->set_uiexchobjid(symbolid);
                intervalMsgHeader.set_allocated_streptransreq(repTransOrderRequest);

                std::string buff;
                intervalMsgHeader.SerializeToString(&buff);

                size_t len = buff.size();
                auto *buffer = new char[len + 1]();
                memcpy(buffer, buff.c_str(), len);

                std::string routing_key = "RepTrans.MarketData.Trade." + accoutId;
                if(g_tnode->PublishToMQ(routing_key.c_str(), buffer, len) != 0){
                    XERROR("Push RepTrans.MarketData.Trade to MQ failed.");
                }
                delete[] buffer;
                return;
            }
        }
    }
}
