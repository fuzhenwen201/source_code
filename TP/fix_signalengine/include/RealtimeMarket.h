/*******************************************************************************************

*Department:	TP

*Decrible:  	
				
				


*Auhor:			Savin

*Createdtime:	2017-06-07

*ModifyTime:	


********************************************************************************************/


#ifndef REALTIMEMARKET_HEAD_H
#define REALTIMEMARKET_HEAD_H




#include "quickfix/Application.h"
#include "quickfix/MessageCracker.h"
#include "quickfix/Values.h"
#include "quickfix/Mutex.h"
#include "quickfix/fix44/QuoteRequest.h"

#include <queue>
#include <list>

using namespace std;

class CRealtimeMarket :
      public FIX::Application,
      public FIX::MessageCracker
{
public:
	CRealtimeMarket(){nCoinfileTime = 0;}
 	void run();

private:
  	void 	onCreate( const FIX::SessionID& ) {}
  	void 	onLogon( const FIX::SessionID& sessionID );
  	void 	onLogout( const FIX::SessionID& sessionID );
  	void 	toAdmin( FIX::Message&, const FIX::SessionID& ) {}
  	void 	toApp( FIX::Message&, const FIX::SessionID& )throw( FIX::DoNotSend );
  	void 	fromAdmin( const FIX::Message&, const FIX::SessionID& )throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon ) {}
  	void 	fromApp( const FIX::Message& message, const FIX::SessionID& sessionID )throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType );

  

	
	int 	ProcessCoinpairConfig();
	int 	SubscribeQuoteRequest();
	int 	UnSubscribeQuoteRequest();

	int 	ReadCoinpairConfig();
	int		ProcessCoinpair();
	int 	CheckCoinpairFile();
	
	FIX44::QuoteRequest SendQuoteRequest44(int subflag,const char *pCoinPire);


private:
	time_t 			nCoinfileTime ;
	list<string> 	CoinPairList;
	list<string> 	CoinPairTmpList;
	list<string> 	CoinPairAddList;
	list<string> 	CoinPairDelList;
	
 
};





#endif //REALTIMEMARKET_HEAD_H

