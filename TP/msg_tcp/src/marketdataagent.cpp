//
// Created by alex on 2/10/17. tnode v0.3
// modify by savin on 2017-08-21 base on new tnode v2.0.0

#include <cstring>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include "xlogger.h"
#include "marketdataagent.h"

using namespace snetwork_signalengine_marketdata;

static CurrencySymbolQueue   *symbol_prices = nullptr;

MarketDataAgent::MarketDataAgent(){
    //consumer_ = nullptr;

    symbols_ = 
    {
"AUDCAD",
"AUDSGD",
"CADSGD",
"EURCAD",
"GBPJPY",
"NZDJPY",
"SGDJPY",
"USDJPY",
"ZARJPY",
"AUDCHF",
"AUDUSD",
"EURCHF",
"GBPAUD",
"GBPNZD",
"TRYJPY",
"USDSGD",
"CADCHF",
"CHFJPY",
"EURJPY",
"EURSGD",
"GBPCAD",
"NZDCAD",
"NZDUSD",
"USDCAD",
"USDHKD",
"AUDJPY",
"EURTRY",
"GBPCHF",
"GBPSGD",
"NZDCHF",
"USDCHF",
"USDTRY",
"AUDNZD",
"CADJPY",
"EURAUD",
"EURGBP",
"EURNZD",
"EURUSD",
"GBPUSD",
"USDCNH",
"USDINR"
	};
}


MarketDataAgent::~MarketDataAgent(){
    if (nullptr != symbol_prices)
	{
		/*
        snetwork_xservice_tnode::TNodeChannel channel = tnode_.GetChannel(0);
        for(uint32_t i = 0; i < SymbolBUTT; ++i){
            channel.DeleteConsumerFromGroup(symbols_[i], consumer_);
            channel.DeleteGroup(symbols_[i]);
        }
        */

        for (MarketMap::iterator i=map_.begin(); i!=map_.end(); )
        {
            map_.erase(i++);
        }

        //delete  consumer_;

        for(uint32_t i = 0; i < SymbolBUTT; ++i)
		{
            if (nullptr != symbol_prices[i].mutex)
			{
                delete symbol_prices[i].mutex;
            }
        }

        delete symbol_prices;
        symbol_prices = nullptr;
    }
}

uint32_t MarketDataAgent::Init(snetwork_xservice_tnode::TNode &tnode, int &id)
{
    if (nullptr != symbol_prices){
        XERROR("\nERRNO %d, Repeat initialization\n", UNIVERSAL_MARKET_REPEAT_INIT_ERR);
        return UNIVERSAL_MARKET_REPEAT_INIT_ERR;
    }

    symbol_prices = new CurrencySymbolQueue[SymbolBUTT];

    memset(symbol_prices, 0, sizeof(CurrencySymbolQueue) * SymbolBUTT);

    for(uint32_t i = 0; i < SymbolBUTT; ++i){
        symbol_prices[i].mutex = new std::mutex();
    }

    //consumer_ = new MarketDataConsumer(id);
    //snetwork_xservice_tnode::TNodeChannel channel = tnode.GetChannel(0);

	
    for(uint32_t i = 0; i < SymbolBUTT; ++i)
	{

		//string queuename = 	symbols_[i] + ".msg.%d" + id ;
	
		char  consumer_queue[64] = {0};
		snprintf(consumer_queue,63,"%s.msg_rt.%d",symbols_[i].c_str(),id);
		
		char exchangestr[64] = {0} ;
        //snprintf(exchangestr,63,"realtime_test.%s",symbols_[i].c_str());
        snprintf(exchangestr,63,"realtime_onez.%s",symbols_[i].c_str());

        MarketDataConsumer *pConsumer = new MarketDataConsumer( exchangestr, consumer_queue);
        printf("comsumer:bindingkey:%s,queue:%s\n",exchangestr, consumer_queue);
        tnode.AddTNodeConsumer(pConsumer) ;

	
        //channel.CreateGroup(symbols_[i]);
        //channel.AddConsumerToGroup(symbols_[i], consumer_);
        map_[symbols_[i]] = i;
    }

	tnode.RunConsumer();

    //tnode_ = tnode;

    return UNIVERSAL_MARKET_SUCCESS;
}


uint32_t MarketDataAgent::GetMarketData(const std::string &symbol, int32_t *bid,int32_t *ask,uint64_t *timestamp)
{
    uint32_t pos;
    MarketMap::iterator  it;
	/*
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	*/
    it = map_.find(symbol);
    if (map_.end() == it){
        XERROR("\nERRNO %d, Not find symbol\n", UNIVERSAL_MARKET_NO_SUPPORT_SYMBOL_ERR);
        return UNIVERSAL_MARKET_NO_SUPPORT_SYMBOL_ERR;
    }

    pos = it->second;

    CurrencySymbolQueue &symbol_manager = symbol_prices[pos];

	
    symbol_manager.mutex->lock();
    CurrencySymbol &symbolprice = symbol_manager.symbol_price;
	
    //writer.StartObject();
    //writer.Key("instrument");
    //writer.String(symbol.c_str());

    //writer.Key("status");
    //writer.Uint(symbolprice.status);

    //writer.Key("bid");
    //writer.Int(symbolprice.bid);
	*bid = symbolprice.bid ;

   // writer.Key("ask");
   // writer.Int(symbolprice.ask);
	*ask = symbolprice.ask;

    //writer.Key("timestamp");
    //writer.Uint64(symbolprice.timestamp);
	*timestamp = symbolprice.timestamp ;

    //writer.EndObject();

    //price = buffer.GetString();
    
    symbol_manager.mutex->unlock();

    return UNIVERSAL_MARKET_SUCCESS;
}


unsigned int MarketDataConsumer::ConsumerData(STTnodeDataPacketHeader *pHeader, char *pBody)
{
	CurrencySymbol *symbol;
    CurrencySymbol *local_symbol;

 
    symbol = (CurrencySymbol*)pBody;
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

	return 0;
	
}




/*

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

*/
