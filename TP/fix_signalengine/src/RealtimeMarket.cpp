

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
  XINFO("Logon\n\n");
}

void CRealtimeMarket::onLogout( const FIX::SessionID& sessionID )
{
  XERROR("Logout, pls check network/ip/port/user/passwd ... etc \n");
}

void CRealtimeMarket::fromApp( const FIX::Message& message, const FIX::SessionID& sessionID )
throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType )
{
	
	struct timeval  curtime;
    gettimeofday(&curtime,NULL);
	XINFO("\n\nFix_SE recv data:[%s]\n",message.toString().c_str());		

	STQuoteRequest44 *pNode = CMemPoolMgr::GetInstance()->GetDataNode();
	if(NULL == pNode)
	{
		XERROR("Get  DataNode from mempool fail \n ");
		return;
	}

	message.FullQuoteRequestData(pNode);
	CRealtimeSender::PushRealtimeData(pNode);
	
	
	//
	
	
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


 

void CRealtimeMarket::run()
{

	sleep(2);

	while(1)
	{
		ProcessCoinpairConfig();

		ProcessCoinpair();

		try
		{
			SubscribeQuoteRequest();
			UnSubscribeQuoteRequest();
		} 
		catch ( std::exception & e )
	    {
	      cout <<  "Message Not Sent: %s" << e.what() << std::endl;
	    }
		

		sleep(stServiceConfig.nReadCoinfileInterTime);
	}
	

}



int CRealtimeMarket::ProcessCoinpair()
{
	
	if(CoinPairList.size() == 0)
	{
		CoinPairList = CoinPairTmpList;
		CoinPairAddList = CoinPairTmpList;
		return 0;
	}

	//find add coin pair
	{
		CoinPairAddList.clear();
		list<string>::iterator itr;
		for (itr=CoinPairTmpList.begin(); itr != CoinPairTmpList.end(); ++itr)
		{
			list<string>::iterator itr2;
			itr2 = find(CoinPairList.begin(),CoinPairList.end(),*itr) ;
			if(itr2 == CoinPairList.end())
			{
				CoinPairAddList.push_back(*itr);
			}
		}

		for (itr=CoinPairAddList.begin(); itr != CoinPairAddList.end(); ++itr)
		{
			CoinPairList.push_back(*itr);
		}
		
	}	
	
	//find del coin pair
	{
		CoinPairDelList.clear();
		list<string>::iterator itr;
		for (itr=CoinPairList.begin(); itr != CoinPairList.end(); ++itr)
		{
			list<string>::iterator itr2;
			itr2 = find(CoinPairTmpList.begin(),CoinPairTmpList.end(),*itr);
			if(itr2 == CoinPairList.end())
			{
				CoinPairDelList.push_back(*itr);	
			}
		}

		for (itr=CoinPairDelList.begin(); itr != CoinPairDelList.end(); ++itr)
		{
			CoinPairList.remove(*itr);
		}
	}	

	return 0;

}



int CRealtimeMarket::CheckCoinpairFile()
{
	if(nCoinfileTime!=0)
	{
		struct stat st;
		if(-1 == stat(COINPAIRECONFIG,&st) )
		{
			XERROR("%s file stat fail:%s\n",strerror(errno));
			exit(-1);
		}

		if(nCoinfileTime == st.st_mtime)
			return -1;
		
		
		nCoinfileTime = st.st_mtime;
	}
	
	return 0;

}




int CRealtimeMarket::ProcessCoinpairConfig()
{
	if(-1 == CheckCoinpairFile() )return -1;

	CoinPairTmpList.clear();

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
		if (!p)
			break;
		buf[7]='\0';
		CoinPairTmpList.push_back(buf);
	}


	fclose(fp);



	return 0;
}

int CRealtimeMarket::SubscribeQuoteRequest()
{
	list<string>::iterator itr;
	for (itr=CoinPairAddList.begin(); itr != CoinPairAddList.end(); ++itr)
	{
		FIX::Message md = SendQuoteRequest44(1,(*itr).c_str());
  		FIX::Session::sendToTarget( md );	
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




