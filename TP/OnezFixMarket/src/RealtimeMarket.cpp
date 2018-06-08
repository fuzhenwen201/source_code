

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <algorithm> 

#include "RealtimeMarket.h"
#include "xlogger.h"
#include "MemPoolMgr.h"

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 )
#else
#include "config.h"
#endif

#include "quickfix/Session.h"
#include <iostream>
#include "RealtimeSender.h"
#include "Public.h"



void CRealtimeMarket::onLogon( const FIX::SessionID& sessionID )
{
  	XINFO("Logon sucess \n\n");
 
 	ReadCoinpairConfig();

	
	try
	{
		SubscribeQuoteRequest();
	} 
	catch ( std::exception & e )
	{
		cout <<  "Message Not Sent: %s" << e.what() << std::endl;
	}
	



  
}

void CRealtimeMarket::onLogout( const FIX::SessionID& sessionID )
{
  	XERROR("Logout, pls check network/ip/port/user/passwd ... etc \n");
}

void CRealtimeMarket::fromApp( const FIX::Message& message, const FIX::SessionID& sessionID )
throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType )
{

	unsigned long int recvtime = GetUTCCurTimestamp();
	
	
	STQuoteRequest44 *pNode = CMemPoolMgr::GetInstance()->GetDataNode();
	if(NULL == pNode)
	{
		XERROR("Get  DataNode from mempool fail \n ");
		return;
	}
	pNode->RecvTime = recvtime;
	message.FullMarkDataResponeData(pNode);
	unsigned long int sendtime = GetTimestamp(pNode->SendingTime);
	
	
	
	 CRealtimeSender::PushRealtimeData(pNode);
	
	
}

void CRealtimeMarket::toApp( FIX::Message& message, const FIX::SessionID& sessionID )
throw( FIX::DoNotSend )
{
  try
  {
    FIX::PossDupFlag possDupFlag;
    message.getHeader().getField( possDupFlag );
    if ( possDupFlag ) throw FIX::DoNotSend();
  }
  catch ( FIX::FieldNotFound& ) {}

 //	std::cout << std::endl << "OUT: " << message << std::endl;

}



int CRealtimeMarket::ReadCoinpairConfig()
{
	
	CoinPairAddList.clear();

	FILE* fp = fopen(COINPAIRECONFIG, "r");
	if (!fp)
	{
		XERROR("can not open %s", COINPAIRECONFIG);
		exit(-1);
	}

	char buf[16];
	for (;;)
	{
		memset(buf, 0, 16);
		char* p = fgets(buf, 16, fp);
		if (!p)break;
		buf[6]='\0';
		CoinPairAddList.push_back(buf);
	}


	fclose(fp);

	

	return 0;
}



 

void CRealtimeMarket::run()
{
	
	sleep(2);
	

	while(1)
	{
		sleep(stServiceConfig.nReadCoinfileInterTime);
	}
	

}



int CRealtimeMarket::SubscribeQuoteRequest()
{
	
	int i = 0;
	list<string>::iterator itr;
	for (itr=CoinPairAddList.begin(); itr != CoinPairAddList.end(); ++itr)
	{
		  		
		FIX::Message md = SendMarketDataRequest44((*itr).c_str());
		FIX::Session::sendToTarget( md );	

		 XINFO("SendMarketDataRequest44:[%d],[%s]\n", ++i ,md.toString().c_str() );
		
		usleep(200*1000);
	}
	
	return 0;
}
int CRealtimeMarket::UnSubscribeQuoteRequest()
{
	list<string>::iterator itr;
	for (itr=CoinPairDelList.begin(); itr != CoinPairDelList.end(); ++itr)
	{
		FIX::Message md = SendQuoteRequest44(2,(*itr).c_str());
  		FIX::Session::sendToTarget( md );	
		usleep(200*1000);
	}

	return 0;
}





FIX44::QuoteRequest CRealtimeMarket::SendQuoteRequest44(int subflag,const char *pCoinPire)
{
	FIX44::QuoteRequest::NoRelatedSym noRelatedSym;
    noRelatedSym.set( FIX::Symbol(pCoinPire) );
    noRelatedSym.set( FIX::QuoteRequestType(subflag) );
 
    FIX44::QuoteRequest quoteRequestMsg(FIX::QuoteReqID("JUNXING_1"));
	
    quoteRequestMsg.addGroup( noRelatedSym );

     {
        FIX::Header &head = quoteRequestMsg.getHeader();


         head.setField( FIX::SenderCompID( stServiceConfig.sSender) );
         head.setField( FIX::TargetCompID(stServiceConfig.sTarget) );
      }
      
      XINFO("SendQuoteRequest44:[%s]\n", quoteRequestMsg.toString().c_str() );

      return quoteRequestMsg;
	
}	






FIX44::MarketDataRequest CRealtimeMarket::SendMarketDataRequest44(const char *pCoinPire)
{
  FIX::MDReqID mdReqID( pCoinPire );
  //FIX::SubscriptionRequestType subType( FIX::SubscriptionRequestType_SNAPSHOT );
    const char SubscriptionRequestType_SNAPSHOT_1 = '1';
  FIX::SubscriptionRequestType subType( SubscriptionRequestType_SNAPSHOT_1 );
  FIX::MarketDepth marketDepth( 0 );

  
  FIX44::MarketDataRequest::NoMDEntryTypes marketDataEntryGroup;
  FIX::MDEntryType mdEntryType( FIX::MDEntryType_BID );
  marketDataEntryGroup.set( mdEntryType );

  FIX44::MarketDataRequest::NoRelatedSym symbolGroup;
  //FIX::Symbol symbol( "LNUX" );
  FIX::Symbol symbol( pCoinPire );
  symbolGroup.set( symbol );

  FIX44::MarketDataRequest message( mdReqID, subType, marketDepth );
  message.addGroup( marketDataEntryGroup );
  message.addGroup( symbolGroup );

  // savin
  FIX::MDUpdateType mdupdatetype(0);
  message.set(mdupdatetype); // savin

  FIX::Header &head = message.getHeader();
  head.setField( FIX::SenderCompID( stServiceConfig.sSender) );
  head.setField( FIX::TargetCompID(stServiceConfig.sTarget) );
 
	
  return message;
}




FIX44::NewOrderSingle CRealtimeMarket::SendNewOrderSingle44()
{
  	FIX::OrdType ordType = FIX::OrdType( FIX::OrdType_MARKET ) ;

  	FIX::ClOrdID ciorderid = FIX::ClOrdID( "9994c4628e6f11e7aaf71c8269ba3574" );
	

  	FIX44::NewOrderSingle newOrderSingle(
    ciorderid, 
    FIX::Side(FIX::Side_BUY ),
    FIX::TransactTime(), 
    ordType
    );

  	newOrderSingle.set( FIX::HandlInst('1') );
  	newOrderSingle.set( FIX::Symbol("AUDJPY") );
  	newOrderSingle.set( FIX::OrderQty(100) );
  	newOrderSingle.set( FIX::TimeInForce( FIX::TimeInForce_GOOD_TILL_CANCEL ) );
	
  	if ( ordType == FIX::OrdType_LIMIT || ordType == FIX::OrdType_STOP_LIMIT )
    newOrderSingle.set( FIX::Price(200) );
  	if ( ordType == FIX::OrdType_STOP || ordType == FIX::OrdType_STOP_LIMIT )
    newOrderSingle.set( FIX::StopPx( 1500 ) );

	FIX::Header &head = newOrderSingle.getHeader();
  	head.setField( FIX::SenderCompID( stServiceConfig.sSender) );
  	head.setField( FIX::TargetCompID(stServiceConfig.sTarget) );

	XINFO("SendNewOrderSingle44:[%s]\n", newOrderSingle.toString().c_str() );
	XINFO("SendNewOrderSingle44:[%s]\n", newOrderSingle.toXML().c_str() );
  	return newOrderSingle;
}



FIX44::OrderCancelRequest CRealtimeMarket::SendOrderCancelRequest44()
{
  	FIX44::OrderCancelRequest orderCancelRequest( 
	  	FIX::OrigClOrdID( "9994c4628e6f11e7aaf71c8269ba3574" ),
	   	FIX::ClOrdID( "123456abc" ), 
	    FIX::Side(FIX::Side_BUY ), 
	    FIX::TransactTime() 
	);


  	orderCancelRequest.set( FIX::Symbol("AUDJPY") );
  	orderCancelRequest.set( FIX::OrderQty(100) );
  
 	FIX::Header &head = orderCancelRequest.getHeader();
  	head.setField( FIX::SenderCompID( stServiceConfig.sSender) );
  	head.setField( FIX::TargetCompID(stServiceConfig.sTarget) );

	XINFO("orderCancelRequest44:[%s]\n", orderCancelRequest.toString().c_str() );
	XINFO("orderCancelRequest44:[%s]\n", orderCancelRequest.toXML().c_str() );

  return orderCancelRequest;
}






