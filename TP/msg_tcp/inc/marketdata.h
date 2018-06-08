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
//#include "tnodechannel.h"
#include "marketerrno.h"


namespace snetwork_signalengine_marketdata 
{

#ifdef MARKET_SYMBOL_SIZE
#error      "MARKET_SYMBOL_SIZE has been defined"
#endif

#define     MARKET_SYMBOL_SIZE  8

enum MarketSymbol{
   AUDCAD=0,
AUDSGD,
CADSGD,
EURCAD,
GBPJPY,
NZDJPY,
SGDJPY,
USDJPY,
ZARJPY,
AUDCHF,
AUDUSD,
EURCHF,
GBPAUD,
GBPNZD,
TRYJPY,
USDSGD,
CADCHF,
CHFJPY,
EURJPY,
EURSGD,
GBPCAD,
NZDCAD,
NZDUSD,
USDCAD,
USDHKD,
AUDJPY,
EURTRY,
GBPCHF,
GBPSGD,
NZDCHF,
USDCHF,
USDTRY,
AUDNZD,
CADJPY,
EURAUD,
EURGBP,
EURNZD,
EURUSD,
GBPUSD,
USDCNH,
USDINR,
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

/*
class MarketData {
public:
    MarketData();
    ~MarketData();

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
};
*/

}

#endif //MARKET_MARKETDATA_H
