//
// Created by alex on 2/10/17.
//

#ifndef MARKET_MARKETDATAAGENT_H
#define MARKET_MARKETDATAAGENT_H

#include "marketdata.h"


namespace snetwork_signalengine_marketdata {

using snetwork_xservice_tnode::TNodeDataSend;

typedef struct CurrencySymbolQueueStruct{
    std::mutex  *mutex;
    CurrencySymbol symbol_price;
} CurrencySymbolQueue;


class MarketDataConsumer : public snetwork_xservice_tnode::TNodeConsumer{
public:
    MarketDataConsumer(const std::string &instance_id);
    ~MarketDataConsumer();

    std::string GetEventName();

    uint32_t EventHandle(u_int16_t tag, char *event, uint32_t eventlen);

    uint32_t ServiceHandle(u_int16_t tag, char *reqbody, uint32_t reqlen, TNodeDataSend send);

private:
    std::string instance_id_;
};

typedef std::map <std::string, uint32_t> MarketMap;

class MarketDataAgent {
public:
    MarketDataAgent();
    ~MarketDataAgent();

    uint32_t Init(snetwork_xservice_tnode::TNode &tnode, const std::string &id);
    uint32_t GetMarketData(const std::string &symbol, std::string &price);

private:
    MarketMap map_;
    std::vector<std::string>  symbols_;
    MarketDataConsumer    *consumer_;
    snetwork_xservice_tnode::TNode tnode_;
};

}


#endif //MARKET_MARKETDATAAGENT_H
