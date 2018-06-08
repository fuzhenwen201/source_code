/*******************************************************************************************

*Department:	TP

*Decrible:  	FIX SiganlEngine
				
				


*Auhor:			Savin

*Createdtime:	2017-06-07

*ModifyTime:	


********************************************************************************************/


#ifndef FIXSE_PUBLIC_HEAD_H
#define FIXSE_PUBLIC_HEAD_H

#include <pthread.h>
#include <assert.h>
#include <list>
#include <vector>
#include <map>
#include <string>
#include "xlogger.h"


#define SERVICECONFIG "FIXSE_conf.cfg"
#define COINPAIRECONFIG "CoinPair_conf.cfg"
using namespace std;

using namespace snetwork_xservice_xflagger;


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

typedef std::map <std::string, uint32_t> MarketMap;
extern MarketMap CoinPairMap;

#define UNIVERSAL_MARKET                                        0x02010000


typedef struct _STServiceConfig
{
	// fix_se
	int nReadCoinfileInterTime ;
	int nMempoolNodeCount ;
	int nRealtimeSenderCount;

	
	//realtimedata mqinfo
    string  mq_vhost;
    string  mq_exchange;
    string  mq_exchangeGroup;
    string  mq_address;
    string  mq_port;
    string  mq_user;
    string  mq_password;
    string  mq_queue;

	// log info
	string 	logdir;
	bool 	onlyscreenshowlog;


	// sp
	string spUsername;
	string spPassword;
	string sTarget;
	string sSender;

}STServiceConfig;



typedef struct CurrencySymbolStruct{
    uint32_t instrument;
    uint32_t status;
    int32_t bid;
    int32_t ask;
    uint64_t timestamp;
} CurrencySymbol;



extern char 			fortexUserName[128];
extern char 			fortexPassword[128];

extern STServiceConfig 	stServiceConfig;

uint64_t 	GetTimestamp(const char *date);

int 		ReadServerConfig();

void 		InitCoinPair();

void 		InitLogSys();



#endif //FIXSE_PUBLIC_HEAD_H

