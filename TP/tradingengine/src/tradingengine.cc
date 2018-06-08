//
// Created by alex on 2/27/17.
//

#include <cstring>
#include <unistd.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include "xlogger.h"
#include "queuemanager.h"
#include "minheap.h"
#include <uuid/uuid.h>

using namespace snetwork_tradingengine;
#define SECOND_TO_USECOND 1000000
#define SEND_ORDER_ANS_SIZE 500
#define SEND_ORDER_ANS "{\"sessionid\":\"%s\",\"reqid\":\"%s\",\"groupid\":%u,\"clientid\":%u,\"orderid\":\"%s\",\"orderstatus\":%u,\"assetid\":%u,\"direction\":%u,\"amount\":%d,\"expiryperiod\":%u,\"opentime\":%lu,\"openprice\":%d,\"closeprice\":%d,\"payout\":%ld,\"sgid\":%u,\"teid\":%u}"

static CurrencySymbolManager   *g_symbol_prices = nullptr;

static SendOrder  *g_order_queue = nullptr;
static QueueManager  order_queue;
static uint32_t   korder_queue_len = 10 * 1024;
static std::mutex order_queue_lock;

static OrderAns  *g_order_ans_queue = nullptr;
static QueueManager  order_ans_queue;
static uint32_t   korder_ans_queue_len = 10 * 1024;
static std::mutex order_ans_queue_lock;

static SendOrder *g_head = nullptr;
static SendOrder *g_tail = nullptr;

static OrderAns *g_order_ans_head = nullptr;
static OrderAns *g_order_ans_tail = nullptr;

static MinHeap  g_min_heap_queue(korder_ans_queue_len);
static bool m_botrading = true;
static float s_payratio = 0.0;
static std::mutex min_heap_lock;
static UpdateGroupConsumer *group_consumer_ = nullptr;
SymbolDrive symbol[] = {
    {10100,"AUDCAD"},{11000,"AUDSGD"},{11001,"CADSGD"},{10104,"EURCAD"},{10805,"GBPJPY"},{10809,"NZDJPY"},{10810,"SGDJPY"},
    {10812,"USDJPY"},{10813,"ZARJPY"},{10200,"AUDCHF"},{11200,"AUDUSD"},{10204,"EURCHF"},{10005,"GBPAUD"},{10905,"GBPNZD"},
    {10811,"TRYJPY"},{11012,"USDSGD"},{10201,"CADCHF"},{10802,"CHFJPY"},{10804,"EURJPY"},{11004,"EURSGD"},{10105,"GBPCAD"},
    {10109,"NZDCAD"},{11209,"NZDUSD"},{10112,"USDCAD"},{10612,"USDHKD"},{10800,"AUDJPY"},{11104,"EURTRY"},{10205,"GBPCHF"},
    {11005,"GBPSGD"},{10209,"NZDCHF"},{10212,"USDCHF"},{11112,"USDTRY"},{10900,"AUDNZD"},{10801,"CADJPY"},{10004,"EURAUD"},
    {10504,"EURGBP"},{10904,"EURNZD"},{11204,"EURUSD"},{11205,"GBPUSD"},{10312,"USDCNH"},{10712,"USDINR"}
};

SymbolDrive symbolBo[] = {
    {20100,"AUDCADbo"},{21000,"AUDSGDbo"},{21001,"CADSGDbo"},{20104,"EURCADbo"},{20805,"GBPJPYbo"},{20809,"NZDJPYbo"},{20810,"SGDJPYbo"},
    {20812,"USDJPYbo"},{20813,"ZARJPYbo"},{20200,"AUDCHFbo"},{21200,"AUDUSDbo"},{20204,"EURCHFbo"},{20005,"GBPAUDbo"},{20905,"GBPNZDbo"},
    {20811,"TRYJPYbo"},{21012,"USDSGDbo"},{20201,"CADCHFbo"},{20802,"CHFJPYbo"},{20804,"EURJPYbo"},{21004,"EURSGDbo"},{20105,"GBPCADbo"},
    {20109,"NZDCADbo"},{21209,"NZDUSDbo"},{20112,"USDCADbo"},{20612,"USDHKDbo"},{20800,"AUDJPYbo"},{21104,"EURTRYbo"},{20205,"GBPCHFbo"},
    {21005,"GBPSGDbo"},{20209,"NZDCHFbo"},{20212,"USDCHFbo"},{21112,"USDTRYbo"},{20900,"AUDNZDbo"},{20801,"CADJPYbo"},{20004,"EURAUDbo"},
    {20504,"EURGBPbo"},{20904,"EURNZDbo"},{21204,"EURUSDbo"},{21205,"GBPUSDbo"},{20312,"USDCNHbo"},{20712,"USDINRbo"}
};
const uint32_t  symbol_size = sizeof(symbol)/sizeof(SymbolDrive);

static  uint64_t GetCurrentTimestamp(){
    struct timespec ts;
    uint64_t tmp;

    clock_gettime(CLOCK_REALTIME, &ts);

    tmp = (uint64_t)(ts.tv_nsec / 1000);
    tmp += ts.tv_sec * 1000000;

    return tmp;
}

char hexLookupTable[513] = {
    "000102030405060708090a0b0c0d0e0f"
    "101112131415161718191a1b1c1d1e1f"
    "202122232425262728292a2b2c2d2e2f"
    "303132333435363738393a3b3c3d3e3f"
    "404142434445464748494a4b4c4d4e4f"
    "505152535455565758595a5b5c5d5e5f"
    "606162636465666768696a6b6c6d6e6f"
    "707172737475767778797a7b7c7d7e7f"
    "808182838485868788898a8b8c8d8e8f"
    "909192939495969798999a9b9c9d9e9f"
    "a0a1a2a3a4a5a6a7a8a9aaabacadaeaf"
    "b0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
    "c0c1c2c3c4c5c6c7c8c9cacbcccdcecf"
    "d0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
    "e0e1e2e3e4e5e6e7e8e9eaebecedeeef"
    "f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff"
};

char HexLookupTable[513] = {
    "000102030405060708090A0B0C0D0E0F"
    "101112131415161718191A1B1C1D1E1F"
    "202122232425262728292A2B2C2D2E2F"
    "303132333435363738393A3B3C3D3E3F"
    "404142434445464748494A4B4C4D4E4F"
    "505152535455565758595A5B5C5D5E5F"
    "606162636465666768696A6B6C6D6E6F"
    "707172737475767778797A7B7C7D7E7F"
    "808182838485868788898A8B8C8D8E8F"
    "909192939495969798999A9B9C9D9E9F"
    "A0A1A2A3A4A5A6A7A8A9AAABACADAEAF"
    "B0B1B2B3B4B5B6B7B8B9BABBBCBDBEBF"
    "C0C1C2C3C4C5C6C7C8C9CACBCCCDCECF"
    "D0D1D2D3D4D5D6D7D8D9DADBDCDDDEDF"
    "E0E1E2E3E4E5E6E7E8E9EAEBECEDEEEF"
    "F0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF"
};

void DecToHex(char *dst, char *src, size_t len = 10, bool lower = true) {
    if (dst==nullptr || src == nullptr || len == 0) {
        return;
    }

    char* pHex = nullptr;
    if (lower) {
        pHex = hexLookupTable;
    } else {
        pHex = HexLookupTable;
    }
    char* pDst = dst;
    char* pSrc = src;
    int pos = 0;

    for (size_t i=0; i<len; ++i) {
        pos = (*(unsigned char*)pSrc << 1);
        *pDst++ = pHex[pos];
        *pDst = pHex[pos+1];
        ++pDst;
        ++pSrc;
    }

    strcpy(pDst, "123456789abc");
    strfry(pDst);
    dst[32] = 0;
}

TradingBO::TradingBO(){
    run_flag_ = false;
}

TradingBO::~TradingBO(){

}

uint32_t TradingBO::Init(bool Botrading){
    if (nullptr != g_symbol_prices){
        XERROR("ERRNO %d, Repeat initialization", UNIVERSAL_TRADING_REPEAT_INIT_ERR);
        return UNIVERSAL_TRADING_REPEAT_INIT_ERR;
    }

    m_botrading = Botrading;
    g_symbol_prices = (CurrencySymbolManager*)new char[sizeof(CurrencySymbolManager) * symbol_size];

    memset(g_symbol_prices, 0, sizeof(CurrencySymbolManager) * symbol_size);

    for(uint32_t i = 0; i < symbol_size; ++i){
        g_symbol_prices[i].mutex = new std::mutex();
    }

    g_order_queue = new SendOrder[korder_queue_len];

    memset(g_order_queue, 0, sizeof(SendOrder) * korder_queue_len);

    if (UNIVERSAL_TRADING_SUCCESS != order_queue.Init(korder_queue_len)){
        XERROR("ERRNO %d, order queue init failed", UNIVERSAL_TRADING_ORDER_QUEUE_INIT_ERR);
        return UNIVERSAL_TRADING_ORDER_QUEUE_INIT_ERR;
    }

    g_order_ans_queue = new OrderAns[korder_ans_queue_len];

    memset(g_order_ans_queue, 0, sizeof(OrderAns) * korder_ans_queue_len);

    if (UNIVERSAL_TRADING_SUCCESS != order_ans_queue.Init(korder_ans_queue_len)){
        XERROR("ERRNO %d, order ans queue init failed", UNIVERSAL_TRADING_ORDER_ANS_QUEUE_INIT_ERR);
        return UNIVERSAL_TRADING_ORDER_ANS_QUEUE_INIT_ERR;
    }

    return UNIVERSAL_TRADING_SUCCESS;
}

uint32_t TradingBO::Run(snetwork_xservice_tnode::TNodeConfig &quota_cfg, snetwork_xservice_tnode::TNodeConfig &order_cfg, const std::string &id){
    uint32_t te_id = (uint32_t)atoi(id.c_str());

    quota_tnode_.Config(quota_cfg);
    quota_tnode_.Bind(1);
    quota_tnode_.Run();

    order_tnode_.Config(order_cfg);
    order_tnode_.Bind(2);
    order_tnode_.Run();

    quota_consumer_ = new QuotaConsumer(id);
    snetwork_xservice_tnode::TNodeChannel channel = quota_tnode_.GetChannel(0);

#if 1
    // modify by savin
    char routingkey_v03[64]={0};
    for(uint32_t i = 0; i < symbol_size; ++i)
    {
        memset(routingkey_v03,0,64);
        snprintf(routingkey_v03,63,"realtime_v03.%s",symbol[i].symbol.c_str());
        channel.CreateGroup(routingkey_v03);
        channel.AddConsumerToGroup(routingkey_v03, quota_consumer_);
    }
    // end savin
#else
    for(uint32_t i = 0; i < symbol_size; ++i){
        channel.CreateGroup(symbol[i].symbol.c_str());
        channel.AddConsumerToGroup(symbol[i].symbol.c_str(), quota_consumer_);
    }
#endif

    channel = order_tnode_.GetChannel(0);
    acknowledge_consumer_ = new AcknowledgeConsumer(id, channel);
    channel.CreateGroup("TRADINGENGIN");
    channel.AddConsumerToGroup("TRADINGENGIN", acknowledge_consumer_);

    validate_consumer_ = new ValidateConsumer(id, channel);
    channel.CreateGroup("VALIDATE");
    channel.AddConsumerToGroup("VALIDATE", validate_consumer_);

    order_consumer_ = new OrderConsumer(id, channel, "TRADINGENGIN");
    channel.CreateGroup("SENDORDER");
    channel.AddConsumerToGroup("SENDORDER", order_consumer_);

    monitor_consumer_ = new MonitorConsumer(id, channel);
    channel.CreateGroup("MONITOR");
    channel.AddConsumerToGroup("MONITOR", monitor_consumer_);

    group_consumer_ = new UpdateGroupConsumer(id, channel);
    channel.CreateGroup("group_info_changed_old");
    channel.AddConsumerToGroup("group_info_changed_old", group_consumer_);

    if(-1 ==group_consumer_->ReadGroupSymbolsList(m_botrading)){
        XERROR("Failed to read the sqlites database!");
        return UNIVERSAL_TRADING_READ_SQLITE_ERR;
    }
    group_consumer_->ShowGroupSymbolsList(m_botrading);

    run_flag_ = true;

    OrderAns *tmp = nullptr;
    uint64_t time_curr_stamp = 0;
    long sleep_time = 0;
    bool flag = false;

    while (run_flag_){
        min_heap_lock.lock();

        tmp = g_min_heap_queue.top();
        while (nullptr != tmp) {
            time_curr_stamp = GetCurrentTimestamp();

	        sleep_time = tmp->closetime - time_curr_stamp;
            if ( sleep_time >= 100) {
		        flag = true;

		        break;
	        }

            g_min_heap_queue.PopOrder();

            snetwork_signalengine_marketdata::CurrencySymbol *local_symbol;
            int32_t index = group_consumer_->SymbolInDrive(m_botrading,tmp->assetid, symbol_size);
            CurrencySymbolManager &symbol_manager = g_symbol_prices[index];
            local_symbol = &symbol_manager.symbol_price;

            symbol_manager.mutex->lock();
            if (snetwork_signalengine_marketdata::CURRENCY_SYMBOL_NORMAL != local_symbol->status){
                uint32_t tmp_status = local_symbol->status;
                symbol_manager.mutex->unlock();
                //Trading Engine Reject
                tmp->orderstatus = (ORDER_CLOSE_PRICE | ORDER_REJECT | ORDER_INVALID);
                SendClosePriceMsg(te_id, channel, tmp);

                order_ans_queue_lock.lock();

                tmp->link.next = nullptr;
                tmp->link.pre = g_order_ans_tail;

                if (nullptr != g_order_ans_tail){
                    g_order_ans_tail->link.next = tmp;
                }
                g_order_ans_tail = tmp;

                if (nullptr == g_order_ans_head){
                    g_order_ans_head = tmp;
                }

                order_ans_queue_lock.unlock();

                tmp = g_min_heap_queue.top();

                XERROR("ERRNO %d, market data status is %d", UNIVERSAL_TRADING_MARKET_DATA_INVALID_ERR, tmp_status);
                continue;
            }
            int32_t bid = local_symbol->bid;
            int32_t ask = local_symbol->ask;
            GroupStruct groupspread = {0,0,0,0};

            std::string tpm_symbol = group_consumer_->GetSymbolfromDrive(m_botrading, tmp->assetid, symbol_size);

            group_consumer_->GetGroupSpread(m_botrading, tmp->groupid, tpm_symbol.c_str(), groupspread);

            group_consumer_->CalAskBidSpread(m_botrading, ask, bid, groupspread.bo_caltype);
            double  m_payratio = groupspread.profit / 100.0;
            tmp->closeprice = bid;

            if (tmp->closeprice != tmp->openprice){

                if (0 == tmp->direction){
                    if (tmp->closeprice < tmp->openprice){
                        tmp->payout = tmp->amount * m_payratio;

                        tmp->orderstatus = (ORDER_CLOSE_PRICE | ORDER_WIN);
                    } else {
                        tmp->payout = -(tmp->amount);

                        tmp->orderstatus = (ORDER_CLOSE_PRICE | ORDER_LOSS);
                    }
                } else {
                    if (tmp->closeprice > tmp->openprice){
                        tmp->payout = tmp->amount * m_payratio;

                        tmp->orderstatus = (ORDER_CLOSE_PRICE | ORDER_WIN);
                    } else {
                        tmp->payout = -(tmp->amount);

                        tmp->orderstatus = (ORDER_CLOSE_PRICE | ORDER_LOSS);
                    }
                }
            } else {
                tmp->payout = 0;

                tmp->orderstatus = (ORDER_CLOSE_PRICE | ORDER_DRAWN);
            }

            symbol_manager.mutex->unlock();

            SendClosePriceMsg(te_id, channel, tmp);

            order_ans_queue_lock.lock();

            tmp->link.next = nullptr;
            tmp->link.pre = g_order_ans_tail;

            if (nullptr != g_order_ans_tail){
                g_order_ans_tail->link.next = tmp;
            }
            g_order_ans_tail = tmp;

            if (nullptr == g_order_ans_head){
                g_order_ans_head = tmp;
            }

            order_ans_queue_lock.unlock();

            tmp = g_min_heap_queue.top();
	    }

        min_heap_lock.unlock();
        if (flag) {
            flag = false;
            if (sleep_time > 3000000) {
                sleep(3);
            } else {
                usleep(sleep_time);
            }
        } else if (sleep_time < 0) {
                sleep(3);
        } else {
                usleep(100000);
        }

    }

    return UNIVERSAL_TRADING_SUCCESS;
}

uint32_t TradingBO::Stop(){
    run_flag_ = false;

    return UNIVERSAL_TRADING_SUCCESS;
}

uint32_t TradingBO::SetPayRatio(uint32_t symbol, uint32_t percentages){
    if (symbol >= symbol_size){
        XERROR("ERRNO %d, trading symbol is not exist", UNIVERSAL_TRADING_SYMBOL_ERR);
        return UNIVERSAL_TRADING_SYMBOL_ERR;
    }

    if (nullptr == g_symbol_prices){
        XERROR("ERRNO %d, TradingBO is not init", UNIVERSAL_TRADING_NO_INIT_ERR);
        return UNIVERSAL_TRADING_NO_INIT_ERR;
    }

    if (percentages > 100){
        XERROR("ERRNO %d, percentages is invalid", UNIVERSAL_TRADING_CONFIG_PARAMETER_ERR);
        return UNIVERSAL_TRADING_CONFIG_PARAMETER_ERR;
    }

    CurrencySymbolManager &symbol_manager = g_symbol_prices[symbol];

    symbol_manager.mutex->lock();

    symbol_manager.pay_ratio = percentages;
    s_payratio = symbol_manager.pay_ratio / 100.0;

    symbol_manager.mutex->unlock();

    return UNIVERSAL_TRADING_SUCCESS;
}

uint32_t TradingBO::SendClosePriceMsg(uint32_t teid, snetwork_xservice_tnode::TNodeChannel &channel, OrderAns *order){
    assert(nullptr != order);
#if 0
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

    writer.StartObject();
    writer.Key("sessionid");
    writer.String(order->sessionid, 32, true);

    writer.Key("reqid");
    writer.String(order->reqid, 32, true);

    writer.Key("groupid");
    writer.Uint(order->groupid);

    writer.Key("clientid");
    writer.Uint(order->clientid);

    writer.Key("orderid");
    writer.String(order->orderid, 32, true);

    writer.Key("orderstatus");
    writer.Uint(order->orderstatus);

    writer.Key("assetid");
    writer.Uint(order->assetid);

    writer.Key("direction");
    writer.Uint(order->direction);

    writer.Key("amount");
    writer.Int(order->amount);

    writer.Key("expiryperiod");
    writer.Uint(order->expiryperiod);

    writer.Key("opentime");
    writer.Uint64(order->opentime);

    writer.Key("openprice");
    writer.Int(order->openprice);

    writer.Key("closeprice");
    writer.Int(order->closeprice);

    writer.Key("payout");
    writer.Int64(order->payout);

    writer.Key("sgid");
    writer.Uint(order->sgid);

    writer.Key("teid");
    writer.Uint(teid);

    writer.EndObject();

    char tmp_buff[24];
    sprintf(tmp_buff, "MT.RM.POSITION.%d", order->sgid);

    uint32_t msg_len = (uint32_t)(buffer.GetLength() + 1);
    char *buff = channel.AllocDataBuff(msg_len);
    if (nullptr == buff){
        XERROR("ERRNO %d, alloc data buff error", UNIVERSAL_TRADING_ALLOC_DATABUFF_ERR);
        return UNIVERSAL_TRADING_ALLOC_DATABUFF_ERR;
    }

    memcpy(buff, buffer.GetString(), msg_len);
    channel.PublishGroup("ORDERANS", tmp_buff, buff, msg_len, ORDERANS);
    XINFO("Send to ORDERANS %s ClosePrice Msg:\n %s", tmp_buff, buffer.GetString());
#else
char tmp_buff[24];
    sprintf(tmp_buff, "MT.RM.POSITION.%d", order->sgid);

    char *buff = channel.AllocDataBuff(SEND_ORDER_ANS_SIZE);
    if (nullptr == buff){
        XERROR("ERRNO %d, alloc data buff error", UNIVERSAL_TRADING_ALLOC_DATABUFF_ERR);
        return UNIVERSAL_TRADING_ALLOC_DATABUFF_ERR;
    }
    sprintf(buff,SEND_ORDER_ANS, order->sessionid,order->reqid,order->groupid,order->clientid,order->orderid
            ,order->orderstatus,order->assetid,order->direction,order->amount,order->expiryperiod,order->opentime,
            order->openprice,order->closeprice,order->payout,order->sgid,teid);

    XINFO("Send to ORDERANS %s ClosePrice Msg:\n %s", tmp_buff, buff);
    channel.PublishGroup("ORDERANS", tmp_buff, buff, strlen(buff) + 1, ORDERANS);

#endif
    return UNIVERSAL_TRADING_SUCCESS;
}

TradingBO::OrderConsumer::OrderConsumer(const std::string &instance_id, snetwork_xservice_tnode::TNodeChannel &channel, const std::string &event) : channel_(channel){
    instance_id_ = instance_id;
    num_id_ = (uint32_t)atoi(instance_id.c_str());
    event_ = event;
}

TradingBO::OrderConsumer::~OrderConsumer(){

}

std::string TradingBO::OrderConsumer::GetEventName(){
    return event_;
}

uint32_t TradingBO::OrderConsumer::EventHandle(u_int16_t tag, char *event, uint32_t eventlen){
    assert(event != nullptr);

    if (SENDORDER != tag || 0 == eventlen){
        XERROR("ERRNO %d, message error tag:%d", UNIVERSAL_TRADING_MESSAGE_DATA_ERR,tag);
        return UNIVERSAL_TRADING_MESSAGE_DATA_ERR;
    }

    XINFO("Received SendOrder Msg: %s", event);

    rapidjson::Document doc;
    doc.Parse((const char*)event);
    if (doc.HasParseError() || !doc.IsObject()) {
        rapidjson::ParseErrorCode code = doc.GetParseError();
        XERROR("ERRNO %d, json format error, parse code %d", UNIVERSAL_TRADING_SEND_ORDER_MSG_ERR, code);
        return UNIVERSAL_TRADING_SEND_ORDER_MSG_ERR;
    }

    uint32_t id = order_queue.Alloc();
    if (id >= korder_queue_len){
        XERROR("ERRNO %d, alloc queue error", UNIVERSAL_TRADING_ALLOC_QUEUE_ERR);
        return UNIVERSAL_TRADING_ALLOC_QUEUE_ERR;
    }

    SendOrder *rcv_order = &g_order_queue[id];
#if 1
    do {
        char reqid[32] = {};
        rapidjson::Value::ConstMemberIterator it;
        if ((it = doc.FindMember("reqid")) == doc.MemberEnd() || !it->value.IsString()){
            XERROR("ERRNO %d, There are no reqid members in the message!", UNIVERSAL_TRADING_SEND_ORDER_MSG_ERR);
            break;
        }

        strncpy(rcv_order->reqid, it->value.GetString(), 32);
        strncpy(reqid, it->value.GetString(), 32);

        if ((it = doc.FindMember("sessionid")) == doc.MemberEnd() || !it->value.IsString()){
            XERROR("ERRNO %d, There are no sessionid members in the message!", UNIVERSAL_TRADING_SEND_ORDER_MSG_ERR);
            break;
        }
        strncpy(rcv_order->sessionid, it->value.GetString(), 32);

        if ((it = doc.FindMember("groupid")) == doc.MemberEnd() || !it->value.IsUint()) {
            XERROR("ERRNO %d, There are no groupid members in the message!", UNIVERSAL_TRADING_SEND_ORDER_MSG_ERR);
            break;
        }
        rcv_order->groupid = it->value.GetUint();

        if ((it = doc.FindMember("clientid")) == doc.MemberEnd() || !it->value.IsUint()){
            XERROR("ERRNO %d, clientid not uint32_t format!", UNIVERSAL_TRADING_SEND_ORDER_MSG_ERR);
            break;
        }
        rcv_order->clientid = it->value.GetUint();

        if ((it = doc.FindMember("assetid")) == doc.MemberEnd() || !it->value.IsUint()){
            XERROR("ERRNO %d, assetid not uint32_t format!", UNIVERSAL_TRADING_SEND_ORDER_MSG_ERR);
            break;
        }
        rcv_order->assetid = it->value.GetUint();

        if ( (it = doc.FindMember("direction")) == doc.MemberEnd() || !it->value.IsUint()){
            XERROR("ERRNO %d, direction not uint32_t format!", UNIVERSAL_TRADING_SEND_ORDER_MSG_ERR);
            break;
        }
        rcv_order->direction = (uint8_t)it->value.GetUint();

        if ((it = doc.FindMember("expiryperiod")) == doc.MemberEnd() || !it->value.IsUint()){
            XERROR("ERRNO %d, expiryperiod not uint32_t format!", UNIVERSAL_TRADING_SEND_ORDER_MSG_ERR);
            break;
        }
        rcv_order->expiryperiod  = (uint16_t)it->value.GetUint();

        if ((it = doc.FindMember("amount")) == doc.MemberEnd() || !it->value.IsInt()){
            XERROR("ERRNO %d, amount not int32_t format!", UNIVERSAL_TRADING_SEND_ORDER_MSG_ERR);
            break;
        }
        rcv_order->amount = it->value.GetInt();

        if ((it = doc.FindMember("sgid")) == doc.MemberEnd() || !it->value.IsUint()){
            XERROR("ERRNO %d, sgid not uint32_t format!", UNIVERSAL_TRADING_SEND_ORDER_MSG_ERR);
            break;
        }
        rcv_order->sgid = it->value.GetUint();

        rcv_order->transfer = 0;

        uint32_t tmp_sgid = rcv_order->sgid;
        uint64_t time = GetCurrentTimestamp();
        rcv_order->ordertime = time + 5 * SECOND_TO_USECOND;

        order_queue_lock.lock();

        rcv_order->link.next = nullptr;
        rcv_order->link.pre = g_tail;

        if (nullptr != g_tail){
            g_tail->link.next = rcv_order;
        }
        g_tail = rcv_order;

        if (nullptr == g_head){
            g_head = rcv_order;
        }

        order_queue_lock.unlock();

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

        writer.StartObject();
        writer.Key("reqid");
        writer.String(reqid, 32, true);

        writer.Key("reqstatus");
        //Trading Engine Ack
        writer.Uint(REQ_TE | REQ_ACK);

        writer.Key("sgid_teid");
        writer.Int(num_id_);

        writer.EndObject();

        char tmp_buff[12];
        sprintf(tmp_buff, "%d", tmp_sgid);

        uint32_t msg_len = (uint32_t)(buffer.GetLength() + 1);
        char *buff = channel_.AllocDataBuff(msg_len);
        if (nullptr == buff){
            XERROR("ERRNO %d, alloc data buff error", UNIVERSAL_TRADING_ALLOC_DATABUFF_ERR);
            return UNIVERSAL_TRADING_ALLOC_DATABUFF_ERR;
        }

        memcpy(buff, buffer.GetString(), msg_len);
        // Ack tag 0x0004
        channel_.PublishGroup("USER", tmp_buff, buff, msg_len, ACKNOWLEDGE);

        XINFO("Send to SG %s SendOrder Ack Msg: %s", tmp_buff, buffer.GetString());

        return UNIVERSAL_TRADING_SUCCESS;
    } while (false);

#endif
    order_queue.Free(id);
    XERROR("ERRNO %d, msg error, missing parameter", UNIVERSAL_TRADING_SEND_ORDER_MSG_ERR);
    return UNIVERSAL_TRADING_SEND_ORDER_MSG_ERR;
}

uint32_t TradingBO::OrderConsumer::ServiceHandle(u_int16_t tag, char *reqbody, uint32_t reqlen, snetwork_xservice_tnode::TNodeDataSend send){
    XERROR("ERRNO %d, No implement", UNIVERSAL_TRADING_NO_IMPLEMENT_ERR);
    return UNIVERSAL_TRADING_NO_IMPLEMENT_ERR;
}

TradingBO::ValidateConsumer::ValidateConsumer(const std::string &instance_id, snetwork_xservice_tnode::TNodeChannel &channel) : channel_(channel){
    instance_id_ = instance_id;
    num_id_ = (uint32_t)atoi(instance_id.c_str());
}

TradingBO::ValidateConsumer::~ValidateConsumer(){

}

std::string TradingBO::ValidateConsumer::GetEventName(){
    return instance_id_;
}

uint32_t TradingBO::ValidateConsumer::EventHandle(u_int16_t tag, char *event, uint32_t eventlen){
    assert(event != nullptr);

    if (VALIDATE != tag || 0 == eventlen){
        XERROR("ERRNO %d, message error", UNIVERSAL_TRADING_MESSAGE_DATA_ERR);
        return UNIVERSAL_TRADING_MESSAGE_DATA_ERR;
    }

    XINFO("Received Validate Msg: %s", event);

    order_queue_lock.lock();
    SendOrder *tmp = g_head;
    SendOrder *tmp_order = nullptr;
    if(nullptr != g_head) {
        tmp_order = (SendOrder *) g_head->link.next;
    }
    while(nullptr != tmp){
        if(tmp->ordertime <= GetCurrentTimestamp()) {
            if (nullptr != tmp->link.pre) {
                ((SendOrder *) tmp->link.pre)->link.next = tmp->link.next;
            } else {
                g_head = (SendOrder *) tmp->link.next;
                if (nullptr != g_head) {
                    g_head->link.pre = nullptr;
                }
            }

            if (nullptr != tmp->link.next) {
                ((SendOrder *) tmp->link.next)->link.pre = tmp->link.pre;
            } else {
                g_tail = (SendOrder *) tmp->link.pre;
                if (nullptr != g_tail) {
                    g_tail->link.next = nullptr;
                }
            }

            tmp->link.pre = nullptr;
            tmp->link.next = nullptr;
            uuid_t uuid;
            char orderid[33];
            uuid_generate_time_safe(uuid);
            DecToHex(orderid, (char*)&uuid);
            orderid[32] = 0;

            OrderAns orderans;
            memset(&orderans,0, sizeof(orderans));
            memcpy(orderans.sessionid, tmp->sessionid, 32);
            memcpy(orderans.reqid, tmp->reqid, 32);
            orderans.groupid = tmp->groupid;
            orderans.clientid = tmp->clientid;
            memcpy(orderans.orderid, orderid, 32);
            orderans.assetid = tmp->assetid;
            orderans.direction = tmp->direction;
            orderans.expiryperiod = tmp->expiryperiod;
            orderans.amount = tmp->amount;
            orderans.sgid = tmp->sgid;
            orderans.opentime = 0;
            orderans.closetime = 0;
            orderans.openprice = 0;
            orderans.closeprice = 0;
            orderans.payout = 0;
            orderans.orderstatus = (ORDER_REJECT | ORDER_CLOSE_PRICE);
            SendOpenPriceMsg(&orderans);
            order_queue.Free((uint32_t) (tmp - g_order_queue));
        }
        tmp = tmp_order;
        if(tmp_order != nullptr)
            tmp_order = (SendOrder*)tmp_order->link.next;
    }
    order_queue_lock.unlock();

    uuid_t uuid;
    char orderid[33];
    uuid_generate_time_safe(uuid);
    DecToHex(orderid, (char*)&uuid);
    orderid[32] = 0;

    rapidjson::Document doc;
    doc.Parse((const char*)event);
    if (doc.HasParseError() || !doc.IsObject()) {
        rapidjson::ParseErrorCode code = doc.GetParseError();
        XERROR("ERRNO %d, json format error, parse code %d", UNIVERSAL_TRADING_VALIDATE_MSG_ERR, code);
        return UNIVERSAL_TRADING_VALIDATE_MSG_ERR;
    }

    uint32_t id = order_ans_queue.Alloc();
    if (id >= korder_ans_queue_len){
        XERROR("ERRNO %d, alloc queue error", UNIVERSAL_TRADING_ALLOC_QUEUE_ERR);
        return UNIVERSAL_TRADING_ALLOC_QUEUE_ERR;
    }

    OrderAns *rcv_order = &g_order_ans_queue[id];
#if 1
    do {
        rapidjson::Value::ConstMemberIterator it;
        if ((it = doc.FindMember("reqid")) == doc.MemberEnd() || !it->value.IsString()) {
            XERROR("ERRNO %d, There are no reqid members in the message!", UNIVERSAL_TRADING_VALIDATE_MSG_ERR);
            break;
        }
        strncpy(rcv_order->reqid, it->value.GetString(), 32);

        order_queue_lock.lock();
        SendOrder *tmp_send_order = g_head;

        while (nullptr != tmp_send_order) {
            if (0 == strncmp(tmp_send_order->reqid, rcv_order->reqid, 32)) {
                break;
            }

            tmp_send_order = (SendOrder *) tmp_send_order->link.next;
        }

        if (nullptr == tmp_send_order) {
            XINFO("reject this order");
            order_queue_lock.unlock();
            order_ans_queue.Free(id);
            return UNIVERSAL_TRADING_SUCCESS;
        }

        if (nullptr != tmp_send_order->link.pre) {
            ((SendOrder *) tmp_send_order->link.pre)->link.next = tmp_send_order->link.next;
        } else {
            g_head = (SendOrder *) tmp_send_order->link.next;
            if (nullptr != g_head) {
                g_head->link.pre = nullptr;
            }
        }

        if (nullptr != tmp_send_order->link.next) {
            ((SendOrder *) tmp_send_order->link.next)->link.pre = tmp_send_order->link.pre;
        } else {
            g_tail = (SendOrder *) tmp_send_order->link.pre;
            if (nullptr != g_tail) {
                g_tail->link.next = nullptr;
            }
        }

        tmp_send_order->link.pre = nullptr;
        tmp_send_order->link.next = nullptr;

        order_queue_lock.unlock();

        order_queue.Free((uint32_t) (tmp_send_order - g_order_queue));

        if ((it = doc.FindMember("sessionid")) == doc.MemberEnd() || !it->value.IsString()) {
            XERROR("ERRNO %d, There are no sessionid members in the message!", UNIVERSAL_TRADING_VALIDATE_MSG_ERR);
            break;
        }
        strncpy(rcv_order->sessionid, it->value.GetString(), 32);

        if ((it = doc.FindMember("groupid")) == doc.MemberEnd() || !it->value.IsUint()) {
            XERROR("ERRNO %d, There are no groupid members in the message!", UNIVERSAL_TRADING_VALIDATE_MSG_ERR);
            break;
        }
        rcv_order->groupid = it->value.GetUint();

        if ((it = doc.FindMember("clientid")) == doc.MemberEnd() || !it->value.IsUint()) {
            XERROR("ERRNO %d, There are no clientid members in the message!", UNIVERSAL_TRADING_VALIDATE_MSG_ERR);
            break;
        }
        rcv_order->clientid = it->value.GetUint();

        if ((it = doc.FindMember("assetid")) == doc.MemberEnd() || !it->value.IsUint()) {
            XERROR("ERRNO %d, There are no assetid members in the message!", UNIVERSAL_TRADING_VALIDATE_MSG_ERR);
            break;
        }
        rcv_order->assetid = it->value.GetUint();
        bool reject_symbol = false;
        GroupStruct groupspread;
        groupspread.bo_caltype = 0;
        groupspread.maximum_amounts = 0;
        groupspread.minimum_amounts = 0;
        groupspread.profit = 0;
        groupspread.condition.clear();

        std::string tpm_symbol = group_consumer_->GetSymbolfromDrive(m_botrading, rcv_order->assetid, symbol_size);
        if(tpm_symbol.empty()){
            XERROR("This currency is not supported!%d",rcv_order->assetid);
            reject_symbol = true;
        }else {
           int spread = group_consumer_->GetGroupSpread(m_botrading, rcv_order->groupid, tpm_symbol.c_str(), groupspread);
            if (-1 == spread) {
                XERROR("Currency pairs that do not support transactions in this group");
                reject_symbol = true;
            }
        }

        if ((it = doc.FindMember("direction")) == doc.MemberEnd() || !it->value.IsUint()) {
            XERROR("ERRNO %d, There are no direction members in the message!", UNIVERSAL_TRADING_VALIDATE_MSG_ERR);
            break;
        }
        rcv_order->direction = (uint8_t)it->value.GetUint();

        if ((it = doc.FindMember("expiryperiod")) == doc.MemberEnd() || !it->value.IsUint()) {
            XERROR("ERRNO %d, There are no expiryperiod members in the message!", UNIVERSAL_TRADING_VALIDATE_MSG_ERR);
            break;
        }
        rcv_order->expiryperiod = (uint16_t)it->value.GetUint();
        if(reject_symbol == false && groupspread.condition[rcv_order->expiryperiod] != 1){
            XERROR("The current period is outside the scope of allowable in this group");
            reject_symbol = true;
        }

        if ((it = doc.FindMember("amount")) == doc.MemberEnd() || !it->value.IsInt()) {
            XERROR("ERRNO %d, There are no amount members in the message!", UNIVERSAL_TRADING_VALIDATE_MSG_ERR);
            break;
        }
        rcv_order->amount = it->value.GetInt();
        if(reject_symbol == false) {
            if (rcv_order->amount > groupspread.maximum_amounts || rcv_order->amount < groupspread.minimum_amounts ||
                rcv_order->amount % groupspread.minimum_amounts != 0) {
                XERROR("The current amount is outside the scope of allowable in this group");
                reject_symbol = true;
            }
        }

        if ((it = doc.FindMember("sgid")) == doc.MemberEnd() || !it->value.IsUint()) {
            XERROR("ERRNO %d, There are no sgid members in the message!", UNIVERSAL_TRADING_VALIDATE_MSG_ERR);
            break;
        }
        rcv_order->sgid = it->value.GetUint();

        if ((it = doc.FindMember("pmid")) == doc.MemberEnd() || !it->value.IsUint()) {
            XERROR("ERRNO %d, There are no pmid members in the message!", UNIVERSAL_TRADING_VALIDATE_MSG_ERR);
            break;
        }
       uint32_t pmid = it->value.GetUint();

        if ((it = doc.FindMember("orderstatus")) == doc.MemberEnd() || !it->value.IsUint()) {
            XERROR("ERRNO %d, There are no orderstatus members in the message!", UNIVERSAL_TRADING_VALIDATE_MSG_ERR);
            break;
        }
        rcv_order->orderstatus = (uint8_t)it->value.GetUint();

        if (0 != rcv_order->orderstatus) {
            //Trading Engine Ack
            SendAckMsg(rcv_order->reqid, 32, REQ_TE | REQ_ACK, pmid);
            order_ans_queue.Free(id);
            XERROR("ERRNO %d, validate failure",
                   UNIVERSAL_TRADING_VALIDATE_REJECT_ERR);
            return UNIVERSAL_TRADING_VALIDATE_REJECT_ERR;
        }

        int32_t index = group_consumer_->SymbolInDrive(m_botrading,rcv_order->assetid, symbol_size);
        if (-1 == index || reject_symbol == true) {
            //Trading Engine Reject
            SendAckMsg(rcv_order->reqid, 32, REQ_TE | REQ_ACK | REQ_REJECT, pmid);
            rcv_order->opentime = GetCurrentTimestamp();
            rcv_order->closetime = 0;
            rcv_order->openprice = 0;
            rcv_order->closeprice = 0;
            memcpy(rcv_order->orderid, orderid, 32);
            rcv_order->payout = 0;
            rcv_order->orderstatus = (ORDER_REJECT | ORDER_CLOSE_PRICE);
            SendOpenPriceMsg(rcv_order);

            order_ans_queue.Free(id);
            //XERROR("ERRNO %d, symbol not exist or not supprot!", UNIVERSAL_TRADING_SYMBOL_ERR);
            return UNIVERSAL_TRADING_SYMBOL_ERR;
        }

        snetwork_signalengine_marketdata::CurrencySymbol *local_symbol;
        CurrencySymbolManager &symbol_manager = g_symbol_prices[index];
        local_symbol = &symbol_manager.symbol_price;

        symbol_manager.mutex->lock();
        if (snetwork_signalengine_marketdata::CURRENCY_SYMBOL_NORMAL != local_symbol->status) {
            uint32_t tmp_status = local_symbol->status;
            symbol_manager.mutex->unlock();
            //Trading Engine Reject
            SendAckMsg(rcv_order->reqid, 32, REQ_TE | REQ_ACK | REQ_REJECT, pmid);

            rcv_order->opentime = GetCurrentTimestamp();
            rcv_order->closetime = 0;
            rcv_order->openprice = 0;
            rcv_order->closeprice = 0;
            memcpy(rcv_order->orderid, orderid, 32);
            rcv_order->payout = 0;
            rcv_order->orderstatus = (ORDER_REJECT | ORDER_CLOSE_PRICE);
            SendOpenPriceMsg(rcv_order);

            order_ans_queue.Free(id);

            XERROR("ERRNO %d, market data status is %d",
                   UNIVERSAL_TRADING_MARKET_DATA_INVALID_ERR, tmp_status);
            return UNIVERSAL_TRADING_MARKET_DATA_INVALID_ERR;
        }
        //Trading Engine Ack
        SendAckMsg(rcv_order->reqid, 32, REQ_TE | REQ_ACK, pmid);
        rcv_order->opentime = GetCurrentTimestamp();
        rcv_order->closetime = rcv_order->opentime + rcv_order->expiryperiod * 1000000;

        int32_t bid = local_symbol->bid;
        int32_t ask = local_symbol->ask;

        group_consumer_->CalAskBidSpread(m_botrading, ask, bid, groupspread.bo_caltype);

        rcv_order->openprice = bid;
        uint64_t order_time = GetCurrentTimestamp();
        rcv_order->ordertime = order_time + (rcv_order->expiryperiod + 2) * SECOND_TO_USECOND;
        symbol_manager.mutex->unlock();

        rcv_order->closeprice = 0;
        memcpy(rcv_order->orderid, orderid, 32);
        rcv_order->payout = 0;

        SendOpenPriceMsg(rcv_order);

        g_min_heap_queue.PushOrder(rcv_order);

        return UNIVERSAL_TRADING_SUCCESS;

    } while(false);
    order_ans_queue.Free(id);
    XERROR("ERRNO %d, msg error, missing parameter", UNIVERSAL_TRADING_VALIDATE_MSG_ERR);
    return UNIVERSAL_TRADING_VALIDATE_MSG_ERR;

#endif
}

uint32_t TradingBO::ValidateConsumer::ServiceHandle(u_int16_t tag, char *reqbody, uint32_t reqlen, snetwork_xservice_tnode::TNodeDataSend send){
    XERROR("ERRNO %d, No implement", UNIVERSAL_TRADING_NO_IMPLEMENT_ERR);
    return UNIVERSAL_TRADING_NO_IMPLEMENT_ERR;
}

uint32_t TradingBO::ValidateConsumer::SendAckMsg(char *reqid, uint32_t len, uint32_t reqstatus, uint32_t pmid){

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

    writer.StartObject();
    writer.Key("reqid");
    writer.String(reqid, len, true);

    writer.Key("reqstatus");
    //Trading Engine Ack
    writer.Uint(reqstatus);

    writer.Key("sgid_teid");
    writer.Uint(num_id_);

    writer.EndObject();
    char tmp_buff[12];
    sprintf(tmp_buff, "%d", pmid);
    uint32_t msg_len = (uint32_t)(buffer.GetLength() + 1);
    char *buff = channel_.AllocDataBuff(msg_len);
    if (nullptr == buff){
        XERROR("ERRNO %d, alloc data buff error", UNIVERSAL_TRADING_ALLOC_DATABUFF_ERR);
        return UNIVERSAL_TRADING_ALLOC_DATABUFF_ERR;
    }

    memcpy(buff, buffer.GetString(), msg_len);
    // Ack tag 0x0004
    channel_.PublishGroup("SYSTEM", tmp_buff, buff, msg_len, ACKNOWLEDGE);

    XINFO("Send to SYSTEM.%s ACK Msg: %s", tmp_buff, buffer.GetString());

    return UNIVERSAL_TRADING_SUCCESS;
}

uint32_t TradingBO::ValidateConsumer::SendOpenPriceMsg(OrderAns *order){
    assert(nullptr != order);
#if 0
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

    writer.StartObject();
    writer.Key("sessionid");
    writer.String(order->sessionid, 32, true);

    writer.Key("reqid");
    writer.String(order->reqid, 32, true);

    writer.Key("groupid");
    writer.Uint(order->groupid);

    writer.Key("clientid");
    writer.Uint(order->clientid);

    writer.Key("orderid");
    writer.String(order->orderid, 32, true);

    writer.Key("orderstatus");
    writer.Uint(order->orderstatus);

    writer.Key("assetid");
    writer.Uint(order->assetid);

    writer.Key("direction");
    writer.Uint(order->direction);

    writer.Key("amount");
    writer.Int(order->amount);

    writer.Key("expiryperiod");
    writer.Uint(order->expiryperiod);

    writer.Key("opentime");
    writer.Uint64(order->opentime);

    writer.Key("openprice");
    writer.Int(order->openprice);

    writer.Key("closeprice");
    writer.Int(order->closeprice);

    writer.Key("payout");
    writer.Int64(order->payout);

    writer.Key("sgid");
    writer.Uint(order->sgid);

    writer.Key("teid");
    writer.Uint(num_id_);

    writer.EndObject();

    char tmp_buff[24];
    sprintf(tmp_buff, "MT.RM.POSITION.%d", order->sgid);

    uint32_t msg_len = (uint32_t)(buffer.GetLength() + 1);
    char *buff = channel_.AllocDataBuff(msg_len);
    if (nullptr == buff){
        XERROR("ERRNO %d, alloc data buff error", UNIVERSAL_TRADING_ALLOC_DATABUFF_ERR);
        return UNIVERSAL_TRADING_ALLOC_DATABUFF_ERR;
    }

    memcpy(buff, buffer.GetString(), msg_len);
#else
    char tmp_buff[24];
    sprintf(tmp_buff, "MT.RM.POSITION.%d", order->sgid);

    char *buff = channel_.AllocDataBuff(SEND_ORDER_ANS_SIZE);
    if (nullptr == buff){
        XERROR("ERRNO %d, alloc data buff error", UNIVERSAL_TRADING_ALLOC_DATABUFF_ERR);
        return UNIVERSAL_TRADING_ALLOC_DATABUFF_ERR;
    }
    sprintf(buff,SEND_ORDER_ANS, order->sessionid,order->reqid,order->groupid,order->clientid,order->orderid
            ,order->orderstatus,order->assetid,order->direction,order->amount,order->expiryperiod,order->opentime,
            order->openprice,order->closeprice,order->payout,order->sgid,num_id_);

    XINFO("Send to ORDERANS %s OpenPrice Msg:\n %s", tmp_buff, buff);
    channel_.PublishGroup("ORDERANS", tmp_buff, buff, strlen(buff) + 1, ORDERANS);
#endif
    return UNIVERSAL_TRADING_SUCCESS;
}

TradingBO::AcknowledgeConsumer::AcknowledgeConsumer(const std::string &instance_id, snetwork_xservice_tnode::TNodeChannel &channel) : channel_(channel){
    instance_id_ = instance_id;
}

TradingBO::AcknowledgeConsumer::~AcknowledgeConsumer(){

}

std::string TradingBO::AcknowledgeConsumer::GetEventName(){
    return instance_id_;
}

uint32_t TradingBO::AcknowledgeConsumer::EventHandle(u_int16_t tag, char *event, uint32_t eventlen){
    assert(event != nullptr);

    if (ACKNOWLEDGE != tag || 0 == eventlen){
        XERROR("ERRNO %d, message error", UNIVERSAL_TRADING_MESSAGE_DATA_ERR);
        return UNIVERSAL_TRADING_MESSAGE_DATA_ERR;
    }

    XINFO("Received Acknowledge Msg: %s", event);

   /* rapidjson::Document doc;
    doc.Parse((const char*)event);
    if (doc.HasParseError() || !doc.IsObject()) {
        rapidjson::ParseErrorCode code = doc.GetParseError();
        XERROR("ERRNO %d, json format error, parse code %d", UNIVERSAL_TRADING_ACKNOWLEDGE_MSG_ERR, code);
        return UNIVERSAL_TRADING_ACKNOWLEDGE_MSG_ERR;
    }

    char tmp_order_ans_reqid[32];
    uint8_t tmp_reqstatus;
    rapidjson::Value::ConstMemberIterator it;
    if ((it = doc.FindMember("reqid")) == doc.MemberEnd() || !it->value.IsString()) {
        XERROR("ERRNO %d, There are no reqid members in the message!", UNIVERSAL_TRADING_ACKNOWLEDGE_MSG_ERR);
        return UNIVERSAL_TRADING_ACKNOWLEDGE_MSG_ERR;
    }
    strncpy(tmp_order_ans_reqid, it->value.GetString(), 32);

    if ((it = doc.FindMember("reqstatus")) == doc.MemberEnd() || !it->value.IsUint()) {
        XERROR("ERRNO %d, There are no reqstatus members in the message!", UNIVERSAL_TRADING_ACKNOWLEDGE_MSG_ERR);
        return UNIVERSAL_TRADING_ACKNOWLEDGE_MSG_ERR;
    }
    tmp_reqstatus = (uint8_t)it->value.GetUint();

    if ((REQ_CLOSE_PRICE | REQ_SG | REQ_ACK) == tmp_reqstatus){
        order_ans_queue_lock.lock();

        OrderAns *tmp_order_ans = g_order_ans_head;

        while (nullptr != tmp_order_ans) {
            if (0 == strncmp(tmp_order_ans->reqid, tmp_order_ans_reqid, 32)) {
                break;
            }

            tmp_order_ans = (OrderAns *) tmp_order_ans->link.next;
        }

        if (nullptr == tmp_order_ans) {
            order_ans_queue_lock.unlock();
            XERROR("ERRNO %d, order answer reqid error, reqid is %32.32s", UNIVERSAL_TRADING_ORDER_ANS_REQID_ERR,
                   tmp_order_ans_reqid);
            return UNIVERSAL_TRADING_ORDER_ANS_REQID_ERR;

        }

        if (nullptr != tmp_order_ans->link.pre) {
            ((OrderAns *) tmp_order_ans->link.pre)->link.next = tmp_order_ans->link.next;
        } else {
            g_order_ans_head = (OrderAns *) tmp_order_ans->link.next;
            if (nullptr != g_order_ans_head) {
                g_order_ans_head->link.pre = nullptr;
            }
        }

        if (nullptr != tmp_order_ans->link.next) {
            ((OrderAns *) tmp_order_ans->link.next)->link.pre = tmp_order_ans->link.pre;
        } else {
            g_order_ans_tail = (OrderAns *) tmp_order_ans->link.pre;
            if (nullptr != g_order_ans_tail) {
                g_order_ans_tail->link.next = nullptr;
            }
        }

        tmp_order_ans->link.pre = nullptr;
        tmp_order_ans->link.next = nullptr;

        order_ans_queue_lock.unlock();

        order_ans_queue.Free((uint32_t) (tmp_order_ans - g_order_ans_queue));
    }*/

    order_ans_queue_lock.lock();
    OrderAns *tmp_order_ans = g_order_ans_head;
    OrderAns *tmp_ans = nullptr;

    if(nullptr != g_order_ans_head)
        tmp_ans = (OrderAns*)g_order_ans_head->link.next;
    while(nullptr != tmp_order_ans){
        if(tmp_order_ans->ordertime <= GetCurrentTimestamp()) {
            if (nullptr != tmp_order_ans->link.pre) {
                ((OrderAns *) tmp_order_ans->link.pre)->link.next = tmp_order_ans->link.next;
            } else {
                g_order_ans_head = (OrderAns *) tmp_order_ans->link.next;
                if (nullptr != g_order_ans_head) {
                    g_order_ans_head->link.pre = nullptr;
                }
            }

            if (nullptr != tmp_order_ans->link.next) {
                ((OrderAns *) tmp_order_ans->link.next)->link.pre = tmp_order_ans->link.pre;
            } else {
                g_order_ans_tail = (OrderAns *) tmp_order_ans->link.pre;
                if (nullptr != g_order_ans_tail) {
                    g_order_ans_tail->link.next = nullptr;
                }
            }

            tmp_order_ans->link.pre = nullptr;
            tmp_order_ans->link.next = nullptr;
            order_ans_queue.Free((uint32_t) (tmp_order_ans - g_order_ans_queue));
        }
        tmp_order_ans = tmp_ans;
        if(nullptr != tmp_ans)
            tmp_ans = (OrderAns*)tmp_ans->link.next;
    }
    order_ans_queue_lock.unlock();

    return UNIVERSAL_TRADING_SUCCESS;
}

uint32_t TradingBO::AcknowledgeConsumer::ServiceHandle(u_int16_t tag, char *reqbody, uint32_t reqlen, snetwork_xservice_tnode::TNodeDataSend send){
    XERROR("ERRNO %d, No implement", UNIVERSAL_TRADING_NO_IMPLEMENT_ERR);
    return UNIVERSAL_TRADING_NO_IMPLEMENT_ERR;
}

TradingBO::QuotaConsumer::QuotaConsumer(const std::string &instance_id){
    instance_id_ = instance_id;
}

TradingBO::QuotaConsumer::~QuotaConsumer(){

}

std::string TradingBO::QuotaConsumer::GetEventName(){
    return instance_id_;
}

uint32_t TradingBO::QuotaConsumer::EventHandle(u_int16_t tag, char *event, uint32_t eventlen){
    snetwork_signalengine_marketdata::CurrencySymbol *symbol;
    snetwork_signalengine_marketdata::CurrencySymbol *local_symbol;

    if (0 != tag || nullptr == event || sizeof(snetwork_signalengine_marketdata::CurrencySymbol) != eventlen){
        XERROR("ERRNO %d, message error", UNIVERSAL_TRADING_MESSAGE_DATA_ERR);
        return UNIVERSAL_TRADING_MESSAGE_DATA_ERR;
    }

    symbol = (snetwork_signalengine_marketdata::CurrencySymbol*)event;
    symbol->instrument = be32toh(symbol->instrument);
    CurrencySymbolManager &symbol_manager = g_symbol_prices[symbol->instrument];
    local_symbol = &symbol_manager.symbol_price;

    symbol_manager.mutex->lock();

    local_symbol->instrument = symbol->instrument;
    local_symbol->status = be32toh(symbol->status);
    local_symbol->ask = be32toh((uint32_t)symbol->ask);
    local_symbol->bid = be32toh((uint32_t)symbol->bid);
    local_symbol->timestamp = be64toh(symbol->timestamp);

    symbol_manager.mutex->unlock();

    return UNIVERSAL_TRADING_SUCCESS;
}

uint32_t TradingBO::QuotaConsumer::ServiceHandle(u_int16_t tag, char *reqbody, uint32_t reqlen, snetwork_xservice_tnode::TNodeDataSend send){
    XERROR("ERRNO %d, No implement", UNIVERSAL_TRADING_NO_IMPLEMENT_ERR);
    return UNIVERSAL_TRADING_NO_IMPLEMENT_ERR;
}

TradingBO::MonitorConsumer::MonitorConsumer(const std::string &instance_id, snetwork_xservice_tnode::TNodeChannel &channel) : channel_(channel){
    instance_id_ = instance_id;
    start_times_ = GetCurrentTimestamp();
}

TradingBO::MonitorConsumer::~MonitorConsumer(){

}

std::string TradingBO::MonitorConsumer::GetEventName(){
    return instance_id_;
}

uint32_t TradingBO::MonitorConsumer::EventHandle(u_int16_t tag, char *event, uint32_t eventlen) {
    assert(event != nullptr);
    
    if (MONITOR != tag || 0 == eventlen){
        XERROR("ERRNO %d, message error", UNIVERSAL_TRADING_MESSAGE_DATA_ERR);
        return UNIVERSAL_TRADING_MESSAGE_DATA_ERR;
    }

    XINFO("Received Monitor Msg: %s", event);
    do {
        rapidjson::Document doc;
        doc.Parse((const char*)event);
        if (doc.HasParseError() || !doc.IsObject()) {
            rapidjson::ParseErrorCode code = doc.GetParseError();
            XERROR("ERRNO %d, json format error, parse code %d", UNIVERSAL_TRADING_MONITOR_MSG_ERR, code);
            break;
        }

        rapidjson::Value::ConstMemberIterator it;
        char tmp_monitor_ans_reqid[32];
        if ((it = doc.FindMember("reqid")) == doc.MemberEnd() || !it->value.IsString()) {
            XERROR("ERRNO %d, There are no reqid members in the message!", UNIVERSAL_TRADING_ACKNOWLEDGE_MSG_ERR);
            return UNIVERSAL_TRADING_ACKNOWLEDGE_MSG_ERR;
        }
        strncpy(tmp_monitor_ans_reqid, it->value.GetString(), 32);

        if ((it = doc.FindMember("mid")) == doc.MemberEnd() || !it->value.IsUint()) {
            XERROR("ERRNO %d, There are no mid members in the message!", UNIVERSAL_TRADING_MONITOR_MSG_ERR);
            return UNIVERSAL_TRADING_MONITOR_MSG_ERR;
        }
        uint32_t monitorid = it->value.GetUint();

        if ((it = doc.FindMember("timestamp")) == doc.MemberEnd() || !it->value.IsUint64()) {
            XERROR("ERRNO %d, There are no timestamp members in the message!", UNIVERSAL_TRADING_MONITOR_MSG_ERR);
            return UNIVERSAL_TRADING_MONITOR_MSG_ERR;
        }
        uint64_t timestamp = it->value.GetUint64();

        uint32_t pid = (uint32_t)getpid();
        uint32_t mid = atoi(instance_id_.c_str());

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

        writer.StartObject();

        writer.Key("reqid");
        writer.String(tmp_monitor_ans_reqid, 32, true);

        writer.Key("status");
        writer.Uint(RUNNING);

        writer.Key("name");
        writer.String("tradingengine");

        writer.Key("pid");
        writer.Uint(pid);

        writer.Key("mid");
        writer.Uint(mid);

        writer.Key("timestamp");
        writer.Uint64(timestamp);

        writer.Key("starttimes");
        writer.Uint64(start_times_);

        writer.EndObject();

        char tmp_buff[12] = {};
        snprintf(tmp_buff, 12, "%d", monitorid);

        uint32_t msg_len = (uint32_t)(buffer.GetLength() + 1);
        char *buff = channel_.AllocDataBuff(msg_len);
        if (nullptr == buff){
            XERROR("ERRNO %d, alloc data buff error", UNIVERSAL_TRADING_ALLOC_DATABUFF_ERR);
            return UNIVERSAL_TRADING_ALLOC_DATABUFF_ERR;
        }

        memcpy(buff, buffer.GetString(), msg_len);

        channel_.PublishGroup("MONITORANS", tmp_buff, buff, msg_len, MONITORANS);

        XINFO("Send to Monitor %s monitor Ack Msg: %s", tmp_buff, buffer.GetString());

        return UNIVERSAL_TRADING_SUCCESS;

    } while (false);

    return UNIVERSAL_TRADING_MONITOR_MSG_ERR;
}

uint32_t TradingBO::MonitorConsumer::ServiceHandle(u_int16_t tag, char *reqbody, uint32_t reqlen, snetwork_xservice_tnode::TNodeDataSend send){
    XERROR("ERRNO %d, No implement", UNIVERSAL_TRADING_NO_IMPLEMENT_ERR);
    return UNIVERSAL_TRADING_NO_IMPLEMENT_ERR;
}
