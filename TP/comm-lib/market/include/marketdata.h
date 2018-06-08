//
// Created by alex on 1/13/17.
//

#ifndef MARKET_MARKETDATA_H
#define MARKET_MARKETDATA_H

#include <stdint-gcc.h>
#include <string>
#include <mutex>
#include <map>
#include <vector>
#include "tnode.h"
#include "tnodechannel.h"
#include "marketerrno.h"


namespace snetwork_signalengine_marketdata {

#ifdef MARKET_SYMBOL_SIZE
#error      "MARKET_SYMBOL_SIZE has been defined"
#endif

#define     MARKET_SYMBOL_SIZE  8

enum MarketSymbol{
    AUD_CAD = 0,
    AUD_CHF,
    AUD_HKD,
    AUD_JPY,
    AUD_NZD,
    AUD_SGD,
    AUD_USD,
    CAD_CHF,
    CAD_HKD,
    CAD_JPY,
    CAD_SGD,
    CHF_HKD,
    CHF_JPY,
    CHF_ZAR,
    EUR_AUD,
    EUR_CAD,
    EUR_CHF,
    EUR_CZK,
    EUR_DKK,
    EUR_GBP,
    EUR_HKD,
    EUR_HUF,
    EUR_JPY,
    EUR_NOK,
    EUR_NZD,
    EUR_PLN,
    EUR_SEK,
    EUR_SGD,
    EUR_TRY,
    EUR_USD,
    EUR_ZAR,
    GBP_AUD,
    GBP_CAD,
    GBP_CHF,
    GBP_HKD,
    GBP_JPY,
    GBP_NZD,
    GBP_PLN,
    GBP_SGD,
    GBP_USD,
    GBP_ZAR,
    HKD_JPY,
    NZD_CAD,
    NZD_CHF,
    NZD_HKD,
    NZD_JPY,
    NZD_SGD,
    NZD_USD,
    SGD_CHF,
    SGD_HKD,
    SGD_JPY,
    TRY_JPY,
    USD_CAD,
    USD_CHF,
    USD_CNH,
    USD_CZK,
    USD_DKK,
    USD_HKD,
    USD_HUF,
    USD_INR,
    USD_JPY,
    USD_MXN,
    USD_NOK,
    USD_PLN,
    USD_SAR,
    USD_SEK,
    USD_SGD,
    USD_THB,
    USD_TRY,
    USD_ZAR,
    ZAR_JPY,
    FINCOIN_USD,
    SymbolBUTT
};

enum CurrencySymbolStatus{
    CURRENCY_SYMBOL_SYSTEM_INIT = 0,
    CURRENCY_SYMBOL_NORMAL,
    CURRENCY_SYMBOL_EXCPTION,
    CURRENCY_SYMBOL_HALTED
};

typedef struct CurrencySymbolStruct{
    uint32_t instrument;
    uint32_t status;
    int32_t bid;
    int32_t ask;
    uint64_t timestamp;
} CurrencySymbol;

typedef struct MessageBufferStruct{
    char *buff;
    uint32_t size;
    uint32_t used;
} MessageBuffer;

enum MonitorStatus{
    UNKNOWN = 0,
    INITIATE,
    BOOTING,
    RUNNING,
    STOPPING,
    STOPPED
};

class MarketData {
public:
    MarketData();
    virtual ~MarketData();

    class MonitorConsumer : public snetwork_xservice_tnode::TNodeConsumer{
    public:
        MonitorConsumer(const std::string &instance_id, snetwork_xservice_tnode::TNodeChannel &channel);
        ~MonitorConsumer();

        std::string GetEventName();

        uint32_t EventHandle(u_int16_t tag, char *event, uint32_t eventlen);

        uint32_t ServiceHandle(u_int16_t tag, char *reqbody, uint32_t reqlen, snetwork_xservice_tnode::TNodeDataSend send);

    private:
        uint64_t start_times_;
        std::string instance_id_;
        snetwork_xservice_tnode::TNodeChannel channel_;
    };

    uint32_t Init(snetwork_xservice_tnode::TNodeConfig &cfg);
    uint32_t Run();
    uint32_t Stop();

    void SetAccessToken(const std::string &token);
    void SetDomain(const std::string &domain);

private:
    void PublishMarketDataForNormal(char *buff, snetwork_xservice_tnode::TNodeChannel &channel, std::string &event);
    void PublishMarketDataForException(snetwork_xservice_tnode::TNodeChannel &channel, std::string &event);


    std::string domain_;
    std::string accessToken_;
    std::vector<std::string>  symbols_;

    uint32_t frequency_;
    MessageBuffer message_buffer_;

    void *curl_;
    struct curl_slist *chunk_;
    snetwork_xservice_tnode::TNode tnode_;

    bool run_flag_;
    bool stop_flag_;
    MonitorConsumer *monitor_consumer_;
};


}

#endif //MARKET_MARKETDATA_H
