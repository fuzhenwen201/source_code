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


#define SERVICECONFIG "OnezFixMarket.cfg"
#define COINPAIRECONFIG "OnezSymbolList.cfg"
using namespace std;

using namespace snetwork_xservice_xflagger;


typedef unsigned int uint32;
typedef unsigned short uint16;




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

typedef std::map <std::string, uint32_t> MarketMap;
extern MarketMap CoinPairMap;

#define UNIVERSAL_MARKET                                        0x02010000


typedef struct _STServiceConfig
{
	// fix_se
	int 	nReadCoinfileInterTime ;
	int 	nReadDBInterTime ;
	int 	nMempoolNodeCount ;
	int 	nRealtimeSenderCount;
	int  	nDelayMicrosecond;
	
		
	
	//realtimedata mqinfo
    string  mq_vhost;
    string  mq_exchange;
    string  mq_exchangeGroup;
    string  mq_address;
    string  mq_port;
    string  mq_user;
    string  mq_password;
    string  mq_queue;

	// db change mq info
	string 	mq_vhost_chge;
	string 	mq_exchange_chge;
	string 	mq_exchangeGroup_chge;
	string 	mq_address_chge;
	string 	mq_port_chge;
	string 	mq_user_chge;
	string 	mq_password_chge;
	string  mq_bindingkey_chge;
	int 	mq_queueid_chge;


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

uint64_t 	GetUTCCurTimestamp() ;

int 		ReadServerConfig();

void 		InitCoinPair();

void 		InitLogSys();



#endif //FIXSE_PUBLIC_HEAD_H

