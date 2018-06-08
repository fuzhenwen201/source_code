/*************************************************************************
    > File Name: Subscriptionmsg.cc
    > Author: Kevin
    > Mail: kevin.fu@fintechstar.cn
    > Created Time: Wed 24 Jan 2018 04:10:35 PM CST
 ************************************************************************/

#include <sys/time.h>
#include "datastorage.h"
#include "mysqlconnect.h"
#include "Ringbuffer.h"

using namespace snetwork_xservice_xflagger;

static snetwork_xservice_tnode::TNode *m_tnode = nullptr;
static ConnectionPool *pool = nullptr;
static XRingBuffer* m_SubOrd_ringBuffer = nullptr;
static XRingBuffer* m_OrdDeal_ringBuffer = nullptr;
static redisContext *m_pRedisContext1 = nullptr;
static redisContext *m_pRedisContext2 = nullptr;

const uint32_t size1 = sizeof(EntrustStruct);
const uint32_t size2 = sizeof(DealOrdSruct);
static char g_hostname[128] = {};

static std::string GetCurrentDate(){

    std::chrono::system_clock::time_point today = std::chrono::system_clock::now();
    auto tt = std::chrono::system_clock::to_time_t(today);
    char tmp[64];
    strftime(tmp,sizeof(tmp),"%Y%m%d",std::gmtime(&tt));
    return tmp;
}

struct Mytime {
    uint64_t year;
    uint64_t month;
    uint64_t day;
};

static Mytime GetCurrentDatetime(){
    std::chrono::system_clock::time_point today = std::chrono::system_clock::now();
    auto tt = std::chrono::system_clock::to_time_t(today);
    std::tm * local = std::gmtime(&tt);
    Mytime times = {};
    times.year = local->tm_year - 100;
    times.month = local->tm_mon + 1;
    times.day = local->tm_mday;

    return times;
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
    loginfo->set_chmoduleindex(4);
    loginfo->set_uireptime(GetCurrentTimestamp());
    loginfo->set_strlogcontent(text);
    logInterHeader.set_allocated_stmaintainlog(loginfo);
    std::string buff;
    logInterHeader.SerializeToString(&buff);

    size_t len = buff.size();
    char buffer[2048] = {};
    memcpy(buffer, buff.c_str(), len);
    char binding_key[128] = {};
    sprintf(binding_key, "_MaintainInfo.Rep.4.%s", g_hostname);

    //将订单信息发送到MQ中
    if (tnode->PublishToMQ(binding_key, buffer, len) != 0) {
        XERROR("Push new order to MQ failed.");
    }

    return true;
}

DataStorageService::DataStorageService() = default;

DataStorageService::~DataStorageService(){
    delete m_SubOrd_ringBuffer;
    delete m_OrdDeal_ringBuffer;
}

void DataStorageService::Init(int argc, char** argv) {

    STTnodeConfig tnodecfg;
    Config config;
    config.ParseCommandLine(&argc, &argv, false);
    XLOGGER(config.Getconfig("loggerfile").c_str())
            .StdErr(config.Getboolconfig("screen"))
            .Colour(config.Getboolconfig("color"))
            .Dir(config.Getconfig("loggerdir"));

    tnodecfg.mq_exchange_group = config.Getconfig("mqExchangeGroup");
    tnodecfg.mq_vhost = config.Getconfig("mqvhost");
    tnodecfg.mq_host = config.Getconfig("mqAddress");
    tnodecfg.mq_passwd = config.Getconfig("mqPassword");
    tnodecfg.mq_port = config.Getintconfig("mqPort");
    tnodecfg.mq_user = config.Getconfig("mqUser");
    std::string mysqluser = config.Getconfig("mysqluser");
    std::string mysqlpwd = config.Getconfig("mysqlpwd");
    std::string mysqlurl = config.Getconfig("mysqlurl");
    int32_t maxsize = config.Getintconfig("maxsize");

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

    //初始化MQ并订阅消息
    m_tnode = new snetwork_xservice_tnode::TNode(tnodecfg);

    m_tnode->Init();

    auto *entrustOrd = new EntrustOrdConsumer("_OrderInfo.Sub.*", "DATASTORAGE_OrderInfo");
    m_tnode->AddTNodeConsumer(entrustOrd);

    auto *dealOrd = new DealOrderConsumer("_MarketData.Trade.*", "DATASTORAGE_MarketData");
    m_tnode->AddTNodeConsumer(dealOrd);

    m_tnode->RunConsumer();

    pool = ConnectionPool::getInstance(mysqluser, mysqlpwd, mysqlurl, maxsize);
    m_SubOrd_ringBuffer = new XRingBuffer(8 * 1024 *1024);
    m_OrdDeal_ringBuffer = new XRingBuffer(8 * 1024 * 1024);
    gethostname(g_hostname, sizeof(g_hostname));
}

void WriteEntrustOrdtomysql(){

    std::shared_ptr<Connection> con;
    Statement *state = nullptr;
    ResultSet *result = nullptr;

    //获得一个连接
    con = pool->getConnect();

    //获得一个数据库连接对象
    state = con->createStatement();
    state->execute("use matching_engine");

    EntrustStruct entrust = {};
    std::vector<std::string> insertbuff = {};
    std::string today = GetCurrentDate();
    Mytime date =  GetCurrentDatetime();
    uint32_t counts = 0;
    std::string insert = {};
    std::string strvalues = {};
    while(m_SubOrd_ringBuffer->popData(&entrust, size1, size1)){
        uint64_t subOrderID = 0;//订单委托编号

        std::string tablename = "TbSubmitOrder_" + std::to_string(entrust.uiuserId) + "_" + today;

        //表不存在，创建表
        std::string creatable = "CREATE TABLE IF NOT EXISTS `matching_engine`.`" + tablename + "` ("
                "  `SubOrderID` bigint(8) NOT NULL,"
                "  `UserOrderID` varchar(35) NOT NULL,"
                "  `UserID` int(4) unsigned NOT NULL,"
                "  `SubmitTime` bigint(8) NOT NULL,"
                "  `SubmitType` tinyint(4) NOT NULL,"
                "  `OrderType` tinyint(4) NOT NULL,"
                "  `TransType` tinyint(4) NOT NULL,"
                "  `Symbol` int(4) unsigned NOT NULL,"
                "  `OrderPrice` bigint(8) NOT NULL,"
                "  `OrderNumber` bigint(8) NOT NULL,"
                "  `OrderIndex` int(4) unsigned NOT NULL,"
                "  PRIMARY KEY (`SubOrderID`,`UserOrderID`)"
                ") ENGINE=InnoDB DEFAULT CHARSET=utf8;";
        state->executeUpdate(creatable);

        auto result_redis = (redisReply*)redisCommand(m_pRedisContext1, "HGET Storage%s %s", today.c_str(), tablename.c_str());
        if(result_redis->type == REDIS_REPLY_NIL){
            subOrderID = (date.year << 56) | (date.month << 48) | (date.day << 40) | 1;
        }else if(result_redis->type == REDIS_REPLY_STRING && result_redis->str != nullptr){
            subOrderID = stoull(result_redis->str) + 1;
        }else if(result_redis->type == REDIS_REPLY_INTEGER){
            subOrderID = (uint64_t)result_redis->integer + 1;
        }
        freeReplyObject(result_redis);

        result_redis = (redisReply*)redisCommand(m_pRedisContext1, "HSET Storage%s %s %llu", today.c_str(), tablename.c_str(), subOrderID);
        freeReplyObject(result_redis);
        counts++;
        if(counts == 1){
            state->execute("START TRANSACTION");
        }

        if(counts % 10 == 1) {
            insert = "insert into matching_engine." + tablename + "(SubOrderID,UserOrderID,UserID,SubmitTime,"
                    "SubmitType,OrderType,TransType,Symbol,OrderPrice,OrderNumber,OrderIndex) value";
        }
        char value[256] = {};
        if(counts % 10 == 0){
            sprintf(value, ",(%lu,\"%s\",%u,%lu,%u,%u,%u,%u,%ld,%ld,%u);", subOrderID, entrust.strordId,
                    entrust.uiuserId, entrust.lsubmitTime, entrust.ucsubmitype, entrust.ucordType, entrust.uctransType,
                    entrust.uiexchobjId, entrust.lprice, entrust.lnumber, entrust.uiordIndex);
            strvalues += value;
        }else if(counts % 10 == 1){
            strvalues.clear();
            sprintf(value, "(%lu,\"%s\",%u,%lu,%u,%u,%u,%u,%ld,%ld,%u)", subOrderID, entrust.strordId,
                    entrust.uiuserId, entrust.lsubmitTime, entrust.ucsubmitype, entrust.ucordType, entrust.uctransType,
                    entrust.uiexchobjId, entrust.lprice, entrust.lnumber, entrust.uiordIndex);
            strvalues += value;
        } else{
            sprintf(value, ",(%lu,\"%s\",%u,%lu,%u,%u,%u,%u,%ld,%ld,%u)", subOrderID, entrust.strordId,
                    entrust.uiuserId, entrust.lsubmitTime, entrust.ucsubmitype, entrust.ucordType, entrust.uctransType,
                    entrust.uiexchobjId, entrust.lprice, entrust.lnumber, entrust.uiordIndex);
            strvalues += value;
        }

        if(counts % 10 == 0){
            std::string str = insert + strvalues;
            state->executeUpdate(str);
        }

        if(counts == 500){
            break;
        }
    }

    if(counts > 0) {
        try {
            if (0 != counts % 10) {
                std::string str = insert + strvalues + ";";
                state->executeUpdate(str);
            }
            state->execute("COMMIT");
        }catch (std::exception & e){
            std::string log = e.what();
            SendLog2Server(4, log, m_tnode);
        }
    }

    delete result;
    delete state;

    pool->retConnect(con);

}

void* TimedEntrustOrd(void* param){
    cpu_set_t mask = {};
    CPU_ZERO(&mask);
    CPU_SET(0,&mask);
    sched_setaffinity(0, sizeof(mask), &mask);
    while(1){
        WriteEntrustOrdtomysql();
        usleep(3000);
    }

    return nullptr;
}

void WriteDealOrdertomysql(){

    std::shared_ptr<Connection>con;
    Statement *state = nullptr;
    ResultSet *result = nullptr;

    //获得一个连接
    con = pool->getConnect();

    //获得一个数据库连接对象
    state = con->createStatement();
    state->execute("use matching_engine");

    DealOrdSruct dealord = {};
    std::vector<std::string> insertbuff = {};
    std::string today = GetCurrentDate();
    Mytime date =  GetCurrentDatetime();
    uint32_t counts = 0;
    while(m_OrdDeal_ringBuffer->popData(&dealord, size2, size2)) {
        std::string tablename;
        uint32_t userid = 0;
        uint64_t subOrderID = 0;//订单委托编号

        if(dealord.askuserid == 0 && dealord.biduserid != 0) {
            tablename = "TbOrderDeal_" + std::to_string(dealord.biduserid) + "_" + today;
            userid = dealord.biduserid;
        } else if (dealord.biduserid == 0 && dealord.askuserid != 0) {
            tablename = "TbOrderDeal_" + std::to_string(dealord.askuserid) + "_" + today;
            userid = dealord.askuserid;
        } else if (dealord.biduserid == dealord.askuserid) {
            tablename = "TbOrderDeal_" + std::to_string(dealord.askuserid) + "_" + today;
            userid = dealord.askuserid;
        }

        auto result_redis = (redisReply*)redisCommand(m_pRedisContext2, "HGET Storage%s %s", today.c_str(), tablename.c_str());
        if(result_redis->type == REDIS_REPLY_NIL){
            subOrderID = (date.year << 56) | (date.month << 48) | (date.day << 40) | 1;
        }else if(result_redis->type == REDIS_REPLY_STRING && result_redis->str != nullptr){
            subOrderID = stoull(result_redis->str) + 1;
        }else if(result_redis->type == REDIS_REPLY_INTEGER){
            subOrderID = (uint64_t)result_redis->integer + 1;
        }
        freeReplyObject(result_redis);

        result_redis = (redisReply*)redisCommand(m_pRedisContext2, "HSET Storage%s %s %llu", today.c_str(), tablename.c_str(), subOrderID);
        freeReplyObject(result_redis);

        //表不存在，创建表
        std::string creatable = "CREATE TABLE IF NOT EXISTS `matching_engine`.`" + tablename  +"` ("
                                        "`DealID` bigint(8) NOT NULL,"
                                        "`UsrID` int(4) unsigned NOT NULL,"
                                        "`SymbolID` int(4) unsigned NOT NULL,"
                                        "`TransType` tinyint(4) NOT NULL,"
                                        "`BidOrderID` bigint(8) NOT NULL,"
                                        "`AskOrderID` bigint(8) NOT NULL,"
                                        "`UsrBidOrderID` varchar(35) NOT NULL,"
                                        "`UsrAskOrderID` varchar(35) NOT NULL,"
                                        "`DealPrice` bigint(8) NOT NULL,"
                                        "`DealNumber` bigint(8) NOT NULL,"
                                        "PRIMARY KEY (`DealID`)"
                                        ") ENGINE=InnoDB DEFAULT CHARSET=utf8;";
        state->executeUpdate(creatable);

        std::string insert = "insert into matching_engine." + tablename + "(DealID,UsrID,SymbolID,TransType,"
                "BidOrderID,AskOrderID,UsrBidOrderID,UsrAskOrderID,DealPrice,DealNumber) ";

        char value[256] = {};
        sprintf(value, "value(%lu,%u,%u,%u,%lu,%lu,\"%s\",\"%s\",%ld,%ld);",subOrderID,userid,dealord.exchobjid,
                dealord.exchtype,dealord.mebidordid,dealord.measkordid,dealord.bidorderid,dealord.askorderid,dealord.price,dealord.number);
        std::string str = insert + value;

        insertbuff.push_back(str);
        counts++;
        dealord = {};
        if(counts == 200){
            break;
        }
    }

    if(!insertbuff.empty()) {
        try {
            std::vector<std::string>::iterator iter;
            state->execute("START TRANSACTION");
            for (iter = insertbuff.begin(); iter != insertbuff.end(); iter++) {
                state->executeUpdate(*iter);
            }
            state->execute("COMMIT");
        }catch (std::exception & e){
            std::string log = e.what();
            SendLog2Server(4, log, m_tnode);
        }
    }

    delete result;
    delete state;

    pool->retConnect(con);

}

void* TimedDealOrder(void* param){

    while(1) {
        WriteDealOrdertomysql();
        usleep(3000);
    }
    return nullptr;
}

void DataStorageService::Run() {

    pthread_t threadId1 = 0;
    pthread_t threadId2 = 0;
    if(pthread_create(&threadId1, nullptr, TimedEntrustOrd, nullptr) != 0){
        XERROR("Failed to create thread threadId1.");
        return;
    }

    if(pthread_create(&threadId2, nullptr, TimedDealOrder, nullptr) != 0){
        XERROR("Failed to create thread threadId2.");
        return;
    }

    pthread_join(threadId1, nullptr);
    pthread_join(threadId2, nullptr);

}

EntrustOrdConsumer::EntrustOrdConsumer(const char *bindingkey, const char *queuename) :
    m_bindingkey(bindingkey),m_queuename(queuename){

}

EntrustOrdConsumer::~EntrustOrdConsumer(){

};

std::string EntrustOrdConsumer::GetQueueName() {
    return m_queuename;
}

std::string EntrustOrdConsumer::GetBindingkey() {
    return m_bindingkey;
}

uint32_t EntrustOrdConsumer::ConsumerData(char* pMsg, int32_t nMsgLen){
    assert(pMsg != nullptr);

    //解析protobuf消息
    EntrustStruct entrustStruct = {};
    MEProInterface::IntervalMsgHeader intervalMsgHeader;
    intervalMsgHeader.ParseFromArray(pMsg, nMsgLen);
    if(intervalMsgHeader.has_stsubmitorder()) {
        const auto &entrustord = intervalMsgHeader.stsubmitorder();

        strncpy(entrustStruct.strordId, entrustord.strorderid().c_str(), 35);

        entrustStruct.lsubmitTime = entrustord.lsubmittime();
        entrustStruct.uctransType = entrustord.uctranstype();
        entrustStruct.ucordType = entrustord.ucordertype();
        entrustStruct.uiuserId = entrustord.uiuserid();
        entrustStruct.uiexchobjId = entrustord.uiexchobjid();
        entrustStruct.lprice = entrustord.lprice();
        entrustStruct.lnumber = entrustord.lnumber();
        entrustStruct.uiordIndex = entrustord.uiorderindex();
		entrustStruct.ucsubmitype = 0;

    }else if(intervalMsgHeader.has_stcancelorder()){
        const auto &cancelord = intervalMsgHeader.stcancelorder();
        strncpy(entrustStruct.strordId, cancelord.strorderid().c_str(), 35);

        entrustStruct.uiordIndex = cancelord.uiorderindex();
        entrustStruct.uiuserId = cancelord.uiuserid();
        entrustStruct.uiexchobjId = cancelord.uiexchobjid();
        entrustStruct.lprice = cancelord.lprice();
        entrustStruct.uctransType = cancelord.uctranstype();
        entrustStruct.lsubmitTime = 0;
        entrustStruct.ucordType = 2;
        entrustStruct.lnumber = 0;
		entrustStruct.ucsubmitype = 1;
    }else {
        XERROR("It is neither a new order nor a cancel order request type.");
        return 1;
    }

    XINFO("Receive %s order to mysql.", entrustStruct.strordId);
    if (!(m_SubOrd_ringBuffer->pushData(&entrustStruct, sizeof(EntrustStruct)))) {
        XERROR("Push new Order to buffer failed .");
        return 1;
    }

    return 0;
}

DealOrderConsumer::DealOrderConsumer(const char *bindingkey, const char *queuename) :
        m_bindingkey(bindingkey),m_queuename(queuename){

}

DealOrderConsumer::~DealOrderConsumer(){

};

std::string DealOrderConsumer::GetQueueName() {
    return m_queuename;
}

std::string DealOrderConsumer::GetBindingkey() {
    return m_bindingkey;
}

uint32_t DealOrderConsumer::ConsumerData(char* pMsg, int32_t nMsgLen){
    assert(pMsg != nullptr);

    XINFO("Received a deal order information.");

    //解析protobuf消息

    MEProInterface::IntervalMsgHeader intervalMsgHeader;
    intervalMsgHeader.ParseFromArray(pMsg, nMsgLen);
    if(!intervalMsgHeader.has_sttransorder()) {
        XERROR("There is no deal order information in the message.");
        return 1;
    }
    const auto& dealorder = intervalMsgHeader.sttransorder();

    DealOrdSruct dealOrd = {};
    strncpy(dealOrd.bidorderid, dealorder.strbidorderid().c_str(), 35);
    strncpy(dealOrd.askorderid, dealorder.straskorderid().c_str(), 35);
    dealOrd.biduserid = dealorder.uibiduserid();
    dealOrd.askuserid = dealorder.uiaskuserid();
    dealOrd.price = dealorder.fprice();
    dealOrd.number = dealorder.fnumber();
    dealOrd.exchtype = dealorder.ucexchtype();
    dealOrd.exchobjid = dealorder.uiexchobjid();
    dealOrd.mebidordid = dealorder.mebidorderid();
    dealOrd.measkordid = dealorder.measkorderid();
    if (!(m_OrdDeal_ringBuffer->pushData(&dealOrd, sizeof(DealOrdSruct)))) {
        XERROR("Push Deal Order to buffer failed .");
        return 1;
    }
    return 0;
}
