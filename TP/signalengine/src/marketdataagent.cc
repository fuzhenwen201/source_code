//
// Created by alex on 2/10/17.
//

#include <cstring>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include "xlogger.h"
#include "marketdataagent.h"

using namespace snetwork_signalengine_marketdata;

static CurrencySymbolQueue   *symbol_prices = nullptr;

MarketDataAgent::MarketDataAgent(){
    consumer_ = nullptr;

    symbols_ = {"AUD_CAD","AUD_CHF","AUD_HKD","AUD_JPY","AUD_NZD","AUD_SGD","AUD_USD","CAD_CHF","CAD_HKD","CAD_JPY","CAD_SGD","CHF_HKD","CHF_JPY",\
    "CHF_ZAR","EUR_AUD","EUR_CAD","EUR_CHF","EUR_CZK","EUR_DKK","EUR_GBP","EUR_HKD","EUR_HUF","EUR_JPY","EUR_NOK","EUR_NZD","EUR_PLN","EUR_SEK","EUR_SGD",\
    "EUR_TRY","EUR_USD","EUR_ZAR","GBP_AUD","GBP_CAD","GBP_CHF","GBP_HKD","GBP_JPY","GBP_NZD","GBP_PLN","GBP_SGD","GBP_USD","GBP_ZAR","HKD_JPY","NZD_CAD",\
    "NZD_CHF","NZD_HKD","NZD_JPY","NZD_SGD","NZD_USD","SGD_CHF","SGD_HKD","SGD_JPY","TRY_JPY","USD_CAD","USD_CHF","USD_CNH","USD_CZK","USD_DKK","USD_HKD",\
    "USD_HUF","USD_INR","USD_JPY","USD_MXN","USD_NOK","USD_PLN","USD_SAR","USD_SEK","USD_SGD","USD_THB","USD_TRY","USD_ZAR","ZAR_JPY","FINCOIN_USD"};
}


MarketDataAgent::~MarketDataAgent(){
    if (nullptr != symbol_prices){
        snetwork_xservice_tnode::TNodeChannel channel = tnode_.GetChannel(0);
        for(uint32_t i = 0; i < SymbolBUTT; ++i){
            channel.DeleteConsumerFromGroup(symbols_[i], consumer_);
            channel.DeleteGroup(symbols_[i]);
        }

        for (MarketMap::iterator i=map_.begin(); i!=map_.end(); )
        {
            map_.erase(i++);
        }

        delete  consumer_;

        for(uint32_t i = 0; i < SymbolBUTT; ++i){
            if (nullptr != symbol_prices[i].mutex){
                delete symbol_prices[i].mutex;
            }
        }

        delete symbol_prices;
        symbol_prices = nullptr;
    }
}

uint32_t MarketDataAgent::Init(snetwork_xservice_tnode::TNode &tnode, const std::string &id){
    if (nullptr != symbol_prices){
        XERROR("\nERRNO %d, Repeat initialization\n", UNIVERSAL_MARKET_REPEAT_INIT_ERR);
        return UNIVERSAL_MARKET_REPEAT_INIT_ERR;
    }

    symbol_prices = new CurrencySymbolQueue[SymbolBUTT];

    memset(symbol_prices, 0, sizeof(CurrencySymbolQueue) * SymbolBUTT);

    for(uint32_t i = 0; i < SymbolBUTT; ++i){
        symbol_prices[i].mutex = new std::mutex();
    }

    consumer_ = new MarketDataConsumer(id);
    snetwork_xservice_tnode::TNodeChannel channel = tnode.GetChannel(0);
    for(uint32_t i = 0; i < SymbolBUTT; ++i){
        channel.CreateGroup(symbols_[i]);
        channel.AddConsumerToGroup(symbols_[i], consumer_);
        map_[symbols_[i]] = i;
    }

    tnode_ = tnode;

    return UNIVERSAL_MARKET_SUCCESS;
}


uint32_t MarketDataAgent::GetMarketData(const std::string &symbol, std::string &price){
    uint32_t pos;
    MarketMap::iterator  it;
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

    it = map_.find(symbol);
    if (map_.end() == it){
        XERROR("\nERRNO %d, Not find symbol\n", UNIVERSAL_MARKET_NO_SUPPORT_SYMBOL_ERR);
        return UNIVERSAL_MARKET_NO_SUPPORT_SYMBOL_ERR;
    }

    pos = it->second;

    CurrencySymbolQueue &symbol_manager = symbol_prices[pos];

    symbol_manager.mutex->lock();
    CurrencySymbol &symbolprice = symbol_manager.symbol_price;

    writer.StartObject();
    writer.Key("instrument");
    writer.String(symbol.c_str());

    writer.Key("status");
    writer.Uint(symbolprice.status);

    writer.Key("bid");
    writer.Int(symbolprice.bid);

    writer.Key("ask");
    writer.Int(symbolprice.ask);

    writer.Key("timestamp");
    writer.Uint64(symbolprice.timestamp);

    writer.EndObject();

    price = buffer.GetString();
    symbol_manager.mutex->unlock();

    return UNIVERSAL_MARKET_SUCCESS;
}

MarketDataConsumer::MarketDataConsumer(const std::string &instance_id){
    instance_id_ = instance_id;
}

MarketDataConsumer::~MarketDataConsumer(){

}

std::string MarketDataConsumer::GetEventName(){
    return instance_id_;
}

uint32_t MarketDataConsumer::EventHandle(u_int16_t tag, char *event, uint32_t eventlen){
    CurrencySymbol *symbol;
    CurrencySymbol *local_symbol;

    if (0 != tag || nullptr == event || sizeof(CurrencySymbol) != eventlen){
        XERROR("\nERRNO %d, message error\n", UNIVERSAL_MARKET_MESSAGE_DATA_ERR);
        return UNIVERSAL_MARKET_MESSAGE_DATA_ERR;
    }

    symbol = (CurrencySymbol*)event;
    symbol->instrument = be32toh(symbol->instrument);
    CurrencySymbolQueue &symbol_manager = symbol_prices[symbol->instrument];
    local_symbol = &symbol_manager.symbol_price;

    symbol_manager.mutex->lock();

    local_symbol->instrument = symbol->instrument;
    local_symbol->status = be32toh(symbol->status);
    local_symbol->ask = be32toh((uint32_t)symbol->ask);
    local_symbol->bid = be32toh((uint32_t)symbol->bid);
    local_symbol->timestamp = be64toh(symbol->timestamp);

    symbol_manager.mutex->unlock();

    return UNIVERSAL_MARKET_SUCCESS;
}

uint32_t MarketDataConsumer::ServiceHandle(u_int16_t tag, char *reqbody, uint32_t reqlen, TNodeDataSend send){
    XERROR("\nERRNO %d, No implement\n", UNIVERSAL_MARKET_NO_IMPLEMENT_ERR);
    return UNIVERSAL_MARKET_NO_IMPLEMENT_ERR;
}
