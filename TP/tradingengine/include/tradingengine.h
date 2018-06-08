//
// Created by alex on 2/27/17.
//

#ifndef TRADINGENGINE_H
#define TRADINGENGINE_H

#include <mutex>
#include "tnode.h"
#include "tradingerrno.h"
#include "marketdata.h"
#include "groupspread.h"

namespace snetwork_tradingengine {

enum TradingMsgType {
    ACKNOWLEDGE = 0x4,
    SENDORDER = 0x11,
    ORDERANS = 0x12,
    VALIDATE = 0x14,
    MONITOR = 0xA001,
    MONITORANS =0xA002,
    UPDATEGROUP = 0xA003,
    UPDATEGROUPANS = 0xA004
};

enum OrderStatusBitType{
    ORDER_ACK = 0x1,
    ORDER_INVALID = 0x2,
    ORDER_WIN = 0x4,
    ORDER_LOSS = 0x8,
    ORDER_REJECT = 0x10,
    ORDER_DRAWN = 0x20,
    ORDER_CLOSE_PRICE = 0x80
};

enum ReqStatusBitType{
    REQ_ACK = 0x1,
    REQ_REJECT = 0x2,
    REQ_SG = 0x4,
    REQ_PM = 0x8,
    REQ_TE = 0x10,
    REQ_MT = 0x20,
    REQ_CM = 0x40,
    REQ_CLOSE_PRICE = 0x80
};

enum MonitorStatus{
    UNKNOWN = 0,
    INITIATE,
    BOOTING,
    RUNNING,
    STOPPING,
    STOPPED
};
typedef struct CurrencySymbolManagerStruct {
    std::mutex *mutex;
    snetwork_signalengine_marketdata::CurrencySymbol symbol_price;
    int32_t    pay_ratio;
} CurrencySymbolManager;

typedef struct TradingLinkStruct{
    void *pre;
    void *next;
} TradingLink;

typedef struct SendOrderStruct{
    TradingLink  link;
    char reqid[32];
    char sessionid[32];
    uint32_t groupid;
    uint32_t clientid;
    uint32_t assetid;
    uint8_t  direction;
    uint8_t  transfer;
    uint16_t expiryperiod;
    int32_t amount;
    uint32_t sgid;
    uint64_t ordertime;
} SendOrder;

typedef struct OrderAnsStruct{
    TradingLink  link;
    char sessionid[33];
    char reqid[33];
    uint32_t groupid;
    uint32_t clientid;
    char orderid[33];
    uint32_t assetid;
    uint8_t  direction;
    uint8_t  orderstatus;
    uint16_t expiryperiod;
    uint64_t opentime;
    uint64_t closetime;
    int32_t openprice;
    int32_t closeprice;
    int32_t amount;
    uint32_t sgid;
    int64_t payout;
    uint64_t ordertime;
} OrderAns;


class TradingBO {
public:
    TradingBO();

    ~TradingBO();

    class OrderConsumer : public snetwork_xservice_tnode::TNodeConsumer{
    public:
        OrderConsumer(const std::string &instance_id, snetwork_xservice_tnode::TNodeChannel &channel, const std::string &event);
        virtual ~OrderConsumer();

        std::string GetEventName();

        uint32_t EventHandle(u_int16_t tag, char *event, uint32_t eventlen);

        uint32_t ServiceHandle(u_int16_t tag, char *reqbody, uint32_t reqlen, snetwork_xservice_tnode::TNodeDataSend send);

    private:
        uint32_t    num_id_;
        std::string instance_id_;
        std::string event_;
        snetwork_xservice_tnode::TNodeChannel channel_;
    };

    class ValidateConsumer : public snetwork_xservice_tnode::TNodeConsumer{
    public:
        ValidateConsumer(const std::string &instance_id, snetwork_xservice_tnode::TNodeChannel &channel);
        virtual ~ValidateConsumer();

        std::string GetEventName();

        uint32_t EventHandle(u_int16_t tag, char *event, uint32_t eventlen);

        uint32_t ServiceHandle(u_int16_t tag, char *reqbody, uint32_t reqlen, snetwork_xservice_tnode::TNodeDataSend send);

        uint32_t SendAckMsg(char *reqid, uint32_t len, uint32_t reqstatus, uint32_t pmid);

        uint32_t SendOpenPriceMsg(OrderAns *order);

    private:
        uint32_t    num_id_;
        std::string instance_id_;
        snetwork_xservice_tnode::TNodeChannel channel_;
    };

    class AcknowledgeConsumer : public snetwork_xservice_tnode::TNodeConsumer{
    public:
        AcknowledgeConsumer(const std::string &instance_id, snetwork_xservice_tnode::TNodeChannel &channel);
        virtual ~AcknowledgeConsumer();

        std::string GetEventName();

        uint32_t EventHandle(u_int16_t tag, char *event, uint32_t eventlen);

        uint32_t ServiceHandle(u_int16_t tag, char *reqbody, uint32_t reqlen, snetwork_xservice_tnode::TNodeDataSend send);

    private:
        std::string instance_id_;
        snetwork_xservice_tnode::TNodeChannel channel_;
    };

    class QuotaConsumer : public snetwork_xservice_tnode::TNodeConsumer{
    public:
        QuotaConsumer(const std::string &instance_id);
        virtual ~QuotaConsumer();

        std::string GetEventName();

        uint32_t EventHandle(u_int16_t tag, char *event, uint32_t eventlen);

        uint32_t ServiceHandle(u_int16_t tag, char *reqbody, uint32_t reqlen, snetwork_xservice_tnode::TNodeDataSend send);

    private:
        std::string instance_id_;
    };

    class MonitorConsumer : public snetwork_xservice_tnode::TNodeConsumer{
    public:
        MonitorConsumer(const std::string &instance_id, snetwork_xservice_tnode::TNodeChannel &channel);
        virtual ~MonitorConsumer();

        std::string GetEventName();

        uint32_t EventHandle(u_int16_t tag, char *event, uint32_t eventlen);

        uint32_t ServiceHandle(u_int16_t tag, char *reqbody, uint32_t reqlen, snetwork_xservice_tnode::TNodeDataSend send);

    private:
        uint64_t start_times_;
        std::string instance_id_;
        snetwork_xservice_tnode::TNodeChannel channel_;
    };

    uint32_t Init(bool Botrading);

    uint32_t Run(snetwork_xservice_tnode::TNodeConfig &quota_cfg, snetwork_xservice_tnode::TNodeConfig &order_cfg, const std::string &id);

    uint32_t Stop();

    uint32_t SetPayRatio(uint32_t symbol, uint32_t percentages);

    uint32_t SendClosePriceMsg(uint32_t teid, snetwork_xservice_tnode::TNodeChannel &channel, OrderAns *order);

private:
    std::vector<std::string> symbols_;
    snetwork_xservice_tnode::TNode quota_tnode_;
    snetwork_xservice_tnode::TNode order_tnode_;
    QuotaConsumer *quota_consumer_;
    OrderConsumer *order_consumer_;
    ValidateConsumer *validate_consumer_;
    AcknowledgeConsumer *acknowledge_consumer_;
    MonitorConsumer *monitor_consumer_;
    //UpdateGroupConsumer *group_consumer_;
    bool run_flag_;
};


}


#endif //TRADINGENGINE_H
