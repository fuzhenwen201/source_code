/*************************************************************************
    > File Name: Subscriptionmsg.cc
    > Author: Kevin
    > Mail: kevin.fu@fintechstar.cn
    > Created Time: Wed 24 Jan 2018 04:10:35 PM CST
 ************************************************************************/

#include <sys/time.h>
#include "extractdata.h"
#include "mysqlconnect.h"
#include "Ringbuffer.h"

using namespace snetwork_xservice_xflagger;

static ConnectionPool *pool = nullptr;
static snetwork_xservice_tnode::TNode* m_tnode = nullptr;
static redisContext *m_pRedisContext1 = nullptr;
static redisContext *m_pRedisContext2 = nullptr;
static XRingBuffer* m_SubOrd_ringBuffer = nullptr;
static XRingBuffer* m_SubOrd_ringBuffer2 = nullptr;
static XRingBuffer* m_OrdDeal_ringBuffer = nullptr;

const uint32_t size1 = sizeof(EntrustStruct);
const uint32_t size2 = sizeof(DealOrdSruct);

static std::string GetCurrentDate(){

    std::chrono::system_clock::time_point today = std::chrono::system_clock::now();
    auto tt = std::chrono::system_clock::to_time_t(today);
    char tmp[64];
    strftime(tmp,sizeof(tmp),"%Y%m%d",std::gmtime(&tt));
    return tmp;
}

static std::string GetYesterdayDate(){
    std::chrono::duration<int,std::ratio<60*60*24> > one_day (1);
    std::chrono::system_clock::time_point today = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point yesterday = today + one_day;
    auto tt = std::chrono::system_clock::to_time_t(yesterday);
    char tmp[64];
    strftime(tmp,sizeof(tmp),"%Y%m%d",std::gmtime(&tt));
    return tmp;
}

static int32_t GetCurrentTime(){
    std::chrono::system_clock::time_point today = std::chrono::system_clock::now();
    auto tt = std::chrono::system_clock::to_time_t(today);
    auto tmp = std::gmtime(&tt);
    return ((tmp->tm_hour * 60 + tmp->tm_min) * 60 + tmp->tm_sec);
}

ExtractDataService::ExtractDataService() = default;

ExtractDataService::~ExtractDataService(){
    delete m_SubOrd_ringBuffer;
    delete m_SubOrd_ringBuffer2;
    delete m_OrdDeal_ringBuffer;
}

void ExtractDataService::Init(int argc, char** argv) {

    Config config;
    config.ParseCommandLine(&argc, &argv, false);
    XLOGGER(config.Getconfig("loggerfile").c_str())
            .StdErr(config.Getboolconfig("screen"))
            .Colour(config.Getboolconfig("color"))
            .Dir(config.Getconfig("loggerdir").c_str());

    tnodecfg.mq_exchange_group = config.Getconfig("mqExchangeGroup");
    tnodecfg.mq_vhost = config.Getconfig("mqvhost");
    tnodecfg.mq_host = config.Getconfig("mqAddress");
    tnodecfg.mq_passwd = config.Getconfig("mqPassword");
    tnodecfg.mq_port = config.Getintconfig("mqPort");
    tnodecfg.mq_user = config.Getconfig("mqUser");

    std::string redisaddr = config.Getconfig("redisIP");
    std::string redispass = config.Getconfig("redisPass");
    int32_t redisport = config.Getintconfig("redisPort");
    struct timeval timeout = {2, 0};    //2s的超时时间
    m_pRedisContext1 = redisConnectWithTimeout(redisaddr.c_str(), redisport, timeout);
    if((nullptr == m_pRedisContext1) || m_pRedisContext1->err){
        if(m_pRedisContext1)
            XERROR("connect redis1 error:%s", m_pRedisContext1->errstr);
        else
            XERROR("connect redis1 error: can't allocate redis context.");
        return;
    }

    auto *pRedisReply = (redisReply*)redisCommand(m_pRedisContext1, "AUTH %s", redispass.c_str());  //执行AUTH命令
    if(pRedisReply->type == 6){
        XERROR("Redis1 authentication failed.");
        freeReplyObject(pRedisReply);
        return;
    }else{
        XINFO("Redis1 certification is successful.");
    }
    freeReplyObject(pRedisReply);

    m_pRedisContext2 = redisConnectWithTimeout(redisaddr.c_str(), redisport, timeout);
    if((nullptr == m_pRedisContext2) || m_pRedisContext2->err){
        if(m_pRedisContext2)
            XERROR("connect redis2 error:%s", m_pRedisContext2->errstr);
        else
            XERROR("connect redis2 error: can't allocate redis context.");
        return;
    }

    pRedisReply = (redisReply*)redisCommand(m_pRedisContext2, "AUTH %s", redispass.c_str());  //执行AUTH命令
    if(pRedisReply->type == 6){
        XERROR("Redis2 authentication failed.");
        freeReplyObject(pRedisReply);
        return;
    }else{
        XINFO("Redis2 certification is successful.");
    }
    freeReplyObject(pRedisReply);

    std::string mysqluser = config.Getconfig("mysqluser");
    std::string mysqlpwd = config.Getconfig("mysqlpwd");
    std::string mysqlurl = config.Getconfig("mysqlurl");
    int32_t maxsize = config.Getintconfig("maxsize");

    m_tnode = new snetwork_xservice_tnode::TNode(tnodecfg);
    m_tnode->Init();
    auto * retransReq = new RetransReqConsumer("RepTrans.MarketData.Trade.*", "demoREPTRANS");
    m_tnode->AddTNodeConsumer(retransReq);
    m_tnode->RunConsumer();

    pool = ConnectionPool::getInstance(mysqluser, mysqlpwd, mysqlurl, maxsize);
    m_SubOrd_ringBuffer = new XRingBuffer(8 * 1024 *1024);
    m_SubOrd_ringBuffer2 = new XRingBuffer(8 * 1024 *1024);
    m_OrdDeal_ringBuffer = new XRingBuffer(8 * 1024 * 1024);
}

void SendEntrustOrd2ME(snetwork_xservice_tnode::TNode* tnode){
    EntrustStruct entrust = {};
    char buffer[2048] ={};
    uint32_t usercount = 0;
    while(m_SubOrd_ringBuffer2->popData(&entrust, size1, size1)) {

        MEProInterface::IntervalMsgHeader intervalMsgHeader;
        intervalMsgHeader.set_iflownumber(1);
        intervalMsgHeader.set_icmd(1);
        intervalMsgHeader.set_imsgtype(2);

        if(entrust.SubmitType == 0) {
            auto submitOrder = new MEProInterface::ST_SUBMIT_ORDER;
            submitOrder->set_strorderid(entrust.UserOrderID);
            submitOrder->set_lsubmittime(entrust.SubmitTime);
            submitOrder->set_uctranstype(entrust.TransType);
            submitOrder->set_ucordertype(entrust.OrderType);
            submitOrder->set_uiuserid(entrust.UserId);
            submitOrder->set_uiexchobjid(entrust.Symbol);
            submitOrder->set_lprice(entrust.OrderPrice);
            submitOrder->set_lnumber(entrust.OrderNumber);
            submitOrder->set_uiorderindex(entrust.OrderIndex);
            submitOrder->set_ucrecordid(entrust.SubOrderID);
            intervalMsgHeader.set_allocated_stsubmitorder(submitOrder);

        }else{
            auto cancelorder = new  MEProInterface::ST_SUBMIT_CAMCEL_ORDER;
            cancelorder->set_strorderid(entrust.UserOrderID);
            cancelorder->set_lorderidfrme(entrust.SubOrderID);
            cancelorder->set_uiorderindex(entrust.OrderIndex);
            cancelorder->set_uiuserid(entrust.UserId);
            cancelorder->set_uiexchobjid(entrust.Symbol);
            cancelorder->set_uctranstype(entrust.TransType);
            cancelorder->set_lprice(entrust.OrderPrice);
            intervalMsgHeader.set_allocated_stcancelorder(cancelorder);
        }

        std::string buff;
        intervalMsgHeader.SerializeToString(&buff);

        size_t len = buff.size();
        usercount++;

        memset(buffer, 0, sizeof(buffer));
        memcpy(buffer, buff.c_str(), len);
        std::string routing_key = "_OrderInfo.Sub.FrTable." + std::to_string(entrust.UserId);
        if(tnode->PublishToMQ(routing_key.c_str(), buffer, len) != 0){
            XERROR("Push _OrderInfo.Sub.FrTable fails");
        }
        if(usercount == 100)
            break;

    }
}

void *TimedSendEntrustOrder2ME(void* param){
    STTnodeConfig* tnodeconf = (STTnodeConfig*)param;

    //初始化MQ并订阅消息
    auto tnode = new snetwork_xservice_tnode::TNode(*tnodeconf);
    tnode->Init();
    tnode->RunConsumer();

    while(1){
        SendEntrustOrd2ME(tnode);
        usleep(30000);
    }
    return nullptr;
}

void SendEntrustOrd2API(snetwork_xservice_tnode::TNode* tnode){
    EntrustStruct entrust = {};
    char buffer[2048] ={};
    uint32_t usercounts = 0;

    while(m_SubOrd_ringBuffer->popData(&entrust, size1, size1)) {

        MEProInterface::IntervalMsgHeader intervalMsgHeader2;
        intervalMsgHeader2.set_iflownumber(1);
        intervalMsgHeader2.set_icmd(1);
        intervalMsgHeader2.set_imsgtype(2);

        auto stSubmitOrderResp = new MEProInterface::ST_SUBMIT_ORDER_RESP;
        stSubmitOrderResp->set_strorderid(entrust.UserOrderID);
        stSubmitOrderResp->set_lorderidfrme(entrust.SubOrderID);
        stSubmitOrderResp->set_uiorderindex(entrust.OrderIndex);
        stSubmitOrderResp->set_uiorderstatus(0);
        stSubmitOrderResp->set_uiuserid(entrust.UserId);
        stSubmitOrderResp->set_uctranstype(entrust.TransType);

        if(entrust.SubmitType == 0) {
            stSubmitOrderResp->set_uisubmitorderiype(0);
        }else{
            stSubmitOrderResp->set_uisubmitorderiype(1);
        }

        intervalMsgHeader2.set_allocated_storderresp(stSubmitOrderResp);

        std::string buff;
        intervalMsgHeader2.SerializeToString(&buff);

        size_t len = buff.size();

        memset(buffer, 0, sizeof(buffer));
        memcpy(buffer, buff.c_str(), len);
        usercounts++;
        if(usercounts == 100) {
            XINFO("Send %s order ack to OrderAPI.", entrust.UserOrderID);
        }
        std::string routing_key2 = "_OrderInfo.Resp." + std::to_string(entrust.UserId);
        if(tnode->PublishToMQ(routing_key2.c_str(), buffer, len) != 0){
            XERROR("Push _OrderInfo.Resp fails");
        }
        if(usercounts == 100)
            break;
    }
}

void *TimedSendEntrustOrder2API(void* param){
    STTnodeConfig* tnodeconf = (STTnodeConfig*)param;

    //初始化MQ并订阅消息
    auto tnode = new snetwork_xservice_tnode::TNode(*tnodeconf);
    tnode->Init();
    tnode->RunConsumer();

    while(1){
        SendEntrustOrd2API(tnode);
        usleep(30000);
    }
    return nullptr;
}

void SendDealOrder2MQ(snetwork_xservice_tnode::TNode* tnode){
    DealOrdSruct dealOrdSruct = {};
    char buffer[2048] ={};
    uint32_t usercount = 0;
    while(m_OrdDeal_ringBuffer->popData(&dealOrdSruct, size2, size2)) {

        MEProInterface::IntervalMsgHeader intervalMsgHeader;
        intervalMsgHeader.set_iflownumber(1);
        intervalMsgHeader.set_icmd(4);
        intervalMsgHeader.set_imsgtype(2);

        auto transOrd = new MEProInterface::ST_TRANS_ORDER_INFO;
        transOrd->set_strbidorderid(dealOrdSruct.UsrBidOrderId);
        transOrd->set_straskorderid(dealOrdSruct.UsrAskOrderId);
        transOrd->set_uibiduserid(dealOrdSruct.UserId);
        transOrd->set_uiaskuserid(dealOrdSruct.UserId);
        transOrd->set_fprice(dealOrdSruct.DealPrice);
        transOrd->set_fnumber(dealOrdSruct.DealNum);
        transOrd->set_ucexchtype(dealOrdSruct.TransType);
        transOrd->set_ucrecordid(dealOrdSruct.DealID);
        transOrd->set_ucorderstatus(2);
        transOrd->set_uiexchobjid(dealOrdSruct.Symbol);
        transOrd->set_mebidorderid(0);
        transOrd->set_measkorderid(0);

        intervalMsgHeader.set_allocated_sttransorder(transOrd);

        std::string buff;
        intervalMsgHeader.SerializeToString(&buff);

        size_t len = buff.size();
        usercount++;
        memset(buffer, 0, sizeof(buffer));
        memcpy(buffer, buff.c_str(), len);

        std::string routing_key = "_MarketData.Trade.FrTable." + std::to_string(dealOrdSruct.UserId);
        if(tnode->PublishToMQ(routing_key.c_str(), buffer, len) != 0){
            XERROR("Push _MarketData.Trade.FrTable failed.");
        }
        if(usercount == 100)
            break;
    }
}

void *TimedSendDealOrder2MQ(void* param){
    auto tnodeconf = (STTnodeConfig*)param;

    //初始化MQ并订阅消息
    auto tnode = new snetwork_xservice_tnode::TNode(*tnodeconf);
    tnode->Init();
    tnode->RunConsumer();

    while(1){
        SendDealOrder2MQ(tnode);
        usleep(30000);
    }

    return nullptr;
}

void PushEntrustOrdtoBuff(Statement* state){

    ResultSet *result = nullptr;

    std::string str;
    int32_t time = GetCurrentTime();
    std::string today = GetCurrentDate();
    std::string yesterday = GetYesterdayDate();

    if(time <= 60){
         str = "SELECT table_name FROM information_schema.TABLE_CONSTRAINTS WHERE table_name LIKE 'TbSubmitOrder_%_" +
                 yesterday + "';";
    }else {
         str = "SELECT table_name FROM information_schema.TABLE_CONSTRAINTS WHERE table_name LIKE 'TbSubmitOrder_%_" +
                 today + "';";
    }
    std::vector<std::string> tablenames = {};

    result = state->executeQuery(str);
    while(result->next()){
        std::string tablename = result->getString(1);
        tablenames.push_back(tablename);
    }
    delete result;

    for (auto iter = tablenames.begin(); iter != tablenames.end() ; ++iter) {
        std::string ststring;
        redisReply *redis_result = nullptr;
        std::string tablename = *iter;
        if(time <= 60)
            redis_result = (redisReply*)redisCommand(m_pRedisContext1, "HGET Extract%s %s", yesterday.c_str(), tablename.c_str());
        else
            redis_result = (redisReply*)redisCommand(m_pRedisContext1, "HGET Extract%s %s", today.c_str(), tablename.c_str());

        if(redis_result->type == REDIS_REPLY_NIL){
            ststring = "SELECT * FROM " + tablename + ";";
        }else if(redis_result->type == REDIS_REPLY_STRING && redis_result->str != nullptr){
            ststring = "SELECT * FROM " + tablename + " WHERE SubOrderID > " + redis_result->str + ";";
        }else if(redis_result->type == REDIS_REPLY_INTEGER){
            ststring = "SELECT * FROM " + tablename + " WHERE SubOrderID > " + std::to_string(redis_result->integer) + ";";
        }
        freeReplyObject(redis_result);

        result = state->executeQuery(ststring);
        while(result->next()){
            EntrustStruct entrustStruct = {};
            EntrustStruct entrustStruct2 = {};
            entrustStruct.SubOrderID = result->getUInt64(1);
            entrustStruct2.SubOrderID = entrustStruct.SubOrderID;
            redisReply *redis2_result = nullptr;
            if(time <= 60)
                redis2_result = (redisReply*)redisCommand(m_pRedisContext1, "HSET Extract%s %s %llu", yesterday.c_str(), tablename.c_str(), entrustStruct.SubOrderID);
            else
                redis2_result = (redisReply*)redisCommand(m_pRedisContext1, "HSET Extract%s %s %llu", today.c_str(), tablename.c_str(), entrustStruct.SubOrderID);
            freeReplyObject(redis2_result);

            strncpy(entrustStruct.UserOrderID, result->getString(2).c_str(), 35);
            strncpy(entrustStruct2.UserOrderID, entrustStruct.UserOrderID, 35);
            entrustStruct.UserId = result->getUInt(3);
            entrustStruct.SubmitTime = result->getUInt64(4);
            entrustStruct.SubmitType = result->getUInt(5);
            entrustStruct.OrderType = result->getUInt(6);
            entrustStruct.TransType = result->getUInt(7);
            entrustStruct.Symbol = result->getUInt(8);
            entrustStruct.OrderPrice = result->getUInt64(9);
            entrustStruct.OrderNumber = result->getUInt64(10);
            entrustStruct.OrderIndex = result->getUInt(11);

            entrustStruct2.UserId = entrustStruct.UserId;
            entrustStruct2.SubmitTime = entrustStruct.SubmitTime;
            entrustStruct2.SubmitType= entrustStruct.SubmitType;
            entrustStruct2.OrderType = entrustStruct.OrderType;
            entrustStruct2.TransType = entrustStruct.TransType;
            entrustStruct2.Symbol = entrustStruct.Symbol;
            entrustStruct2.OrderPrice = entrustStruct.OrderPrice;
            entrustStruct2.OrderNumber = entrustStruct.OrderNumber;
            entrustStruct2.OrderIndex = entrustStruct.OrderIndex;
            if (!(m_SubOrd_ringBuffer->pushData(&entrustStruct, sizeof(EntrustStruct)))) {
                XERROR("Push new Order to buffer failed .");
                return ;
            }

            if (!(m_SubOrd_ringBuffer2->pushData(&entrustStruct2, sizeof(EntrustStruct)))) {
                XERROR("Push new Order to buffer failed .");
                return ;
            }

        }
        delete result;
    }

}

void* TimedPushEntrustOrdtoBuff(void* param){

    std::shared_ptr<Connection> con;
    Statement *state = nullptr;
    //获得一个连接
    con = pool->getConnect();

    //获得一个数据库连接对象
    state = con->createStatement();
    state->execute("use matching_engine");

    while(1){
        PushEntrustOrdtoBuff(state);
        usleep(3000);
    }
    delete state;
    pool->retConnect(con);
    return nullptr;
}

void PushDealOrdertoBuff(Statement* state){

    ResultSet *result = nullptr;

    std::string str;
    int32_t time = GetCurrentTime();
    std::string today = GetCurrentDate();
    std::string yesterday = GetYesterdayDate();

    if(time <= 60){
        str = "SELECT table_name FROM information_schema.TABLE_CONSTRAINTS WHERE table_name LIKE 'TbOrderDeal_%_" +
                yesterday + "';";
    }else {
        str = "SELECT table_name FROM information_schema.TABLE_CONSTRAINTS WHERE table_name LIKE 'TbOrderDeal_%_" +
                today + "';";
    }
    std::vector<std::string> tablenames = {};

    result = state->executeQuery(str);
    while(result->next()){
        std::string tablename = result->getString(1);
        tablenames.push_back(tablename);
    }
    delete result;

    for (auto iter = tablenames.begin(); iter != tablenames.end() ; ++iter) {
        std::string ststring;
        redisReply *redis_result = nullptr;
        std::string tablename = *iter;
        if(time <= 60)
            redis_result = (redisReply*)redisCommand(m_pRedisContext2, "HGET Extract%s %s", yesterday.c_str(), tablename.c_str());
        else
            redis_result = (redisReply*)redisCommand(m_pRedisContext2, "HGET Extract%s %s", today.c_str(), tablename.c_str());

        if(redis_result->type == REDIS_REPLY_NIL){
            ststring = "SELECT * FROM " + tablename + ";";
        }else if(redis_result->type == REDIS_REPLY_STRING && redis_result->str != nullptr){
            ststring = "SELECT * FROM " + tablename + " WHERE DealID > " + redis_result->str + ";";
        }else if(redis_result->type == REDIS_REPLY_INTEGER){
            ststring = "SELECT * FROM " + tablename + " WHERE DealID > " + std::to_string(redis_result->integer) + ";";
        }

        freeReplyObject(redis_result);

        result = state->executeQuery(ststring);
        while(result->next()){
            DealOrdSruct dealOrd = {};
            dealOrd.DealID = result->getUInt64(1);
            redisReply *redis2_result = nullptr;
            if(time <= 60)
                redis2_result = (redisReply*)redisCommand(m_pRedisContext2, "HSET Extract%s %s %llu", yesterday.c_str(), tablename.c_str(), dealOrd.DealID);
            else
                redis2_result = (redisReply*)redisCommand(m_pRedisContext2, "HSET Extract%s %s %llu", today.c_str(), tablename.c_str(), dealOrd.DealID);
            freeReplyObject(redis2_result);

            dealOrd.UserId = result->getUInt(2);
            dealOrd.Symbol = result->getUInt(3);
            dealOrd.TransType = result->getUInt(4);
            strncpy(dealOrd.UsrBidOrderId,result->getString(7).c_str(), 35);
            strncpy(dealOrd.UsrAskOrderId,result->getString(8).c_str(), 35);
            dealOrd.DealPrice = result->getUInt64(9);
            dealOrd.DealNum = result->getUInt64(10);

            if (!(m_OrdDeal_ringBuffer->pushData(&dealOrd, sizeof(DealOrdSruct)))) {
                XERROR("Push deal Order to buffer failed .");
                return ;
            }
        }
        delete result;
    }
}

void* TimedPushDealOrdertoBuff(void* param){
    std::shared_ptr<Connection> con;
    Statement *state = nullptr;
    //获得一个连接
    con = pool->getConnect();

    //获得一个数据库连接对象
    state = con->createStatement();
    state->execute("use matching_engine");
    while(1) {
        PushDealOrdertoBuff(state);
        usleep(3000);
    }
    delete state;
    pool->retConnect(con);
    return nullptr;
}

void ExtractDataService::Run() {

    pthread_t threadId1 = 0;
    pthread_t threadId2 = 0;
    pthread_t threadId3 = 0;
    pthread_t threadId4 = 0;
    pthread_t threadId5 = 0;
    if(pthread_create(&threadId1, nullptr, TimedPushEntrustOrdtoBuff, nullptr) != 0){
        XERROR("Failed to create the threadId1 thread.");
        return;
    }

    if(pthread_create(&threadId3, nullptr, TimedSendEntrustOrder2ME, &tnodecfg) != 0){
        XERROR("Failed to create the threadId3 thread.");
        return;
    }

    if(pthread_create(&threadId2, nullptr, TimedPushDealOrdertoBuff, nullptr) != 0){
        XERROR("Failed to create the threadId2 thread.");
        return;
    }

    if(pthread_create(&threadId4, nullptr, TimedSendDealOrder2MQ, &tnodecfg) != 0){
        XERROR("Failed to create the threadId4 thread.");
        return;
    }

    if(pthread_create(&threadId5, nullptr, TimedSendEntrustOrder2API, &tnodecfg) != 0){
        XERROR("Failed to create the threadId4 thread.");
        return;
    }

    pthread_join(threadId1, nullptr);
    pthread_join(threadId2, nullptr);
    pthread_join(threadId3, nullptr);
    pthread_join(threadId4, nullptr);
    pthread_join(threadId5, nullptr);
}

RetransReqConsumer::RetransReqConsumer(const char *bindingkey, const char *queuename) :
        m_bindingkey(bindingkey), m_queuename(queuename) {
}

RetransReqConsumer::~RetransReqConsumer() {

};

std::string RetransReqConsumer::GetQueueName() {
    return m_queuename;
}

std::string RetransReqConsumer::GetBindingkey() {
    return m_bindingkey;
}

uint32_t RetransReqConsumer::ConsumerData(char *pMsg, int32_t nMsgLen) {
    assert(pMsg != nullptr);

    std::shared_ptr<Connection> con;
    Statement *state = nullptr;
    ResultSet *result = nullptr;

    //获得一个连接
    con = pool->getConnect();

    //获得一个数据库连接对象
    state = con->createStatement();
    state->execute("use matching_engine");
    
    MEProInterface::IntervalMsgHeader intervalMsgHeader1;
    intervalMsgHeader1.ParseFromArray(pMsg, nMsgLen);
    if(!intervalMsgHeader1.has_streptransreq()){
        XERROR("There is no retransmission request information in the message.");
        return 1;
    }

    MEProInterface::ST_REP_TRANS_ORDER_REQUEST submitOrder = intervalMsgHeader1.streptransreq();
    const auto& meOrdId = submitOrder.ucexchid();
    const auto& userId = submitOrder.uiuserid();
    const auto& exchobjId = submitOrder.uiexchobjid();

    XINFO("Received the retransmission %ull order request.", meOrdId);

    uint64_t year = (meOrdId & 0xff00000000000000) >> 56;
    uint64_t month = (meOrdId & 0x00ff000000000000) >> 48;
    uint64_t day = (meOrdId & 0x0000ff0000000000) >> 40;
    std::string strmonth;
    std::string strday;
    if(month < 10)
        strmonth = "0" + std::to_string(month);
    else
        strmonth = std::to_string(month);

    if(day < 10)
        strday = "0" + std::to_string(day);
    else
        strday = std::to_string(day);

    std::string tablename = "TbOrderDeal_" + std::to_string(userId) + "_20" + std::to_string(year) + strmonth + strday;

    std::string str_select = "SELECT COUNT(1) FROM information_schema.TABLE_CONSTRAINTS WHERE table_name = '" + tablename +"';";
    result = state->executeQuery(str_select);
    while(result->next()){
        if(1 != result->getUInt(1)){
            XERROR("Table 'matching_engine.%s' doesn't exist", tablename.c_str());
            return 1;
        }
    }

    std::string tab = "select * from " + tablename + " where DealID = " + std::to_string(meOrdId) + " AND SymbolID = " + std::to_string(exchobjId) + ";";
    result = state->executeQuery(tab);
    while(result->next()){
        auto DealID = result->getUInt64(1);
        auto UserId = result->getUInt(2);
        auto Symbol = result->getUInt(3);
        auto TransType = result->getUInt(4);
        auto UsrBidOrderId = result->getString(7);
        auto UsrAskOrderId = result->getString(8);
        auto DealPrice = result->getUInt64(9);
        auto DealNum = result->getUInt64(10);

        MEProInterface::IntervalMsgHeader intervalMsgHeader;
        intervalMsgHeader.set_iflownumber(1);
        intervalMsgHeader.set_icmd(4);
        intervalMsgHeader.set_imsgtype(2);

        auto transOrd = new MEProInterface::ST_TRANS_ORDER_INFO;
        transOrd->set_strbidorderid(UsrBidOrderId);
        transOrd->set_straskorderid(UsrAskOrderId);
        transOrd->set_uibiduserid(UserId);
        transOrd->set_uiaskuserid(UserId);
        transOrd->set_fprice(DealPrice);
        transOrd->set_fnumber(DealNum);
        transOrd->set_ucexchtype(TransType);
        transOrd->set_ucrecordid(DealID);
        transOrd->set_ucorderstatus(2);
        transOrd->set_uiexchobjid(Symbol);
        transOrd->set_mebidorderid(0);
        transOrd->set_measkorderid(0);

        intervalMsgHeader.set_allocated_sttransorder(transOrd);

        std::string buff;
        intervalMsgHeader.SerializeToString(&buff);

        size_t len = buff.size();
        auto *buffer = new char[len + 1]();
        memcpy(buffer, buff.c_str(), len);

        std::string routingkey = "_MarketData.RepTransTrade." + std::to_string(UserId);

        if(m_tnode->PublishToMQ(routingkey.c_str(), buffer, len) != 0){
            XERROR("Push _MarketData.RepTransTrade failed.");
        }

        delete[] buffer;
        break;
    }

    return 0;
}
