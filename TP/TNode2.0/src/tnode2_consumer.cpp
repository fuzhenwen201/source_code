/*******************************************************************************************

*Department:	TP

*Decrible:  	new tnode 测试程序
				

*Auhor:			Savin Chen

*Createdtime:	2017-08-08

*ModifyTime:	


********************************************************************************************/

#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>

#include "tnode.h"
#include "xlogger.h"



using namespace snetwork_xservice_tnode;
using namespace snetwork_xservice_xflagger;


void InitLogSys()
{
	(*XLogger::GetObject("new_node_test")).StdErr(true).Colour(true).Dir("./logs").BuffDelay(0);
	XINFO("InitLogSys ....");
}



class CTestTNodeConsumer : public TNodeConsumer
{
public:
	CTestTNodeConsumer(const char *bindingkey, const char *queuename  )
	{
		
		m_bindingkey = bindingkey ;
		m_queuename = queuename;
	}
	~CTestTNodeConsumer(){}
	
	std::string GetBindingkey(){ return m_bindingkey ; }
	std::string GetQueueName(){ return m_queuename ; }

	
	 unsigned int ConsumerData(STTnodeDataPacketHeader *pHeader, char *pBody)
	 {
	 	static int i = 0 ;
		
		//char *pBuf = pBody+ (pHeader->bodylen - 20);

		
		XINFO("pbuf:[%s],i:%d,routingkey:%s,ConsumerData: starter:0x%04x,tag:%d,version:%d,requestid:%s,sessionid:%s,bodylen:%d,queue:%s\n",
			pBody,++i,GetBindingkey().c_str(),
			(pHeader->starter) ,(pHeader->tag),(pHeader->version),pHeader->requestid,pHeader->sessionid, (pHeader->bodylen),m_queuename.c_str()
			);
		
		
		return 0;
	 }
	

private:
	
	std::string	m_bindingkey ;
	std::string	m_queuename  ;

};

  
int main(int argc, char *argv[])  
{  

	if(argc!=5)
	{
		printf("Usage:./tnode2_consumer ip exchange bindkingkey queuename\n");
		return -1;
	}

	char ip[32]={0};
	strncpy(ip,argv[1],31);

	char pbindingkey[128]={0};
	strncpy(pbindingkey,argv[3],127);
	char pqueuename[128]={0};
	strncpy(pqueuename,argv[4],127);

	
	printf("./tnode2_consumer  ip:%s, mq_exchange_group:%s, routingkey:%s,queuename:%s\n",ip,argv[2],pbindingkey,pqueuename);


	InitLogSys();

	
	STTnodeConfig tnodeconfig ;
	tnodeconfig.mq_vhost= "/demo" ;
	//tnodeconfig.mq_exchange_group = "new_tnode_test_exchange" ;
	tnodeconfig.mq_exchange_group = argv[2];
	tnodeconfig.mq_host = ip ;
	tnodeconfig.mq_port= 5672 ;
	tnodeconfig.mq_user = "demo" ;
	tnodeconfig.mq_passwd= "demo" ;

	TNode tnode(tnodeconfig);
	if(-1 == tnode.Init() )
	{
		printf("tnode init fail , pls check ! \n");
		return -1;
	}


	CTestTNodeConsumer *pConsumer = new CTestTNodeConsumer( pbindingkey, pqueuename);
	if(-1 == tnode.AddTNodeConsumer(pConsumer) )
	{
		XERROR("AddTNodeConsumer  ERROR \n");
		tnode.Close();
		return -1;
	}


	/*
	CTestTNodeConsumer *pConsumer = new CTestTNodeConsumer( "AUD_CAD", "AUD_CAD.msg");
	if(-1 == tnode.AddTNodeConsumer(pConsumer) )
	{
		XERROR("AddTNodeConsumer  ERROR \n");
		tnode.Close();
		return -1;
	}

	
	CTestTNodeConsumer *pConsumer2 = new CTestTNodeConsumer( "AUD_CHF", "AUD_CHF.msg");
	if(-1 == tnode.AddTNodeConsumer(pConsumer2) )
	{
		XERROR("AddTNodeConsumer ERROR \n");
		tnode.Close();
		return -1;
	}	


	CTestTNodeConsumer *pConsumer3 = new CTestTNodeConsumer( "AUD_HKD", "AUD_HKD.msg");
	if(-1 == tnode.AddTNodeConsumer(pConsumer3) )
	{
		XERROR("AddTNodeConsumer ERROR \n");
		tnode.Close();
		return -1;
	}	

	*/


	
	/*
	{
		
		std::vector<std::string>  symbols_ = {"AUD_CAD","AUD_CHF","AUD_HKD","AUD_JPY","AUD_NZD","AUD_SGD","AUD_USD","CAD_CHF","CAD_HKD","CAD_JPY","CAD_SGD","CHF_HKD","CHF_JPY",\
			"CHF_ZAR","EUR_AUD","EUR_CAD","EUR_CHF","EUR_CZK","EUR_DKK","EUR_GBP","EUR_HKD","EUR_HUF","EUR_JPY","EUR_NOK","EUR_NZD","EUR_PLN","EUR_SEK","EUR_SGD",\
			"EUR_TRY","EUR_USD","EUR_ZAR","GBP_AUD","GBP_CAD","GBP_CHF","GBP_HKD","GBP_JPY","GBP_NZD","GBP_PLN","GBP_SGD","GBP_USD","GBP_ZAR","HKD_JPY","NZD_CAD",\
			"NZD_CHF","NZD_HKD","NZD_JPY","NZD_SGD","NZD_USD","SGD_CHF","SGD_HKD","SGD_JPY","TRY_JPY","USD_CAD","USD_CHF","USD_CNH","USD_CZK","USD_DKK","USD_HKD",\
			"USD_HUF","USD_INR","USD_JPY","USD_MXN","USD_NOK","USD_PLN","USD_SAR","USD_SEK","USD_SGD","USD_THB","USD_TRY","USD_ZAR","ZAR_JPY"};
		

		int i = 0;
		for(i;i<symbols_.size();i++)
		{
			char queuename[17]={0};
			sprintf(queuename,"%s.msg",symbols_[i].c_str());
			
			CTestTNodeConsumer *pConsumer = new CTestTNodeConsumer( symbols_[i].c_str(), queuename);
			if(-1 == tnode.AddTNodeConsumer(pConsumer) )
			{
				XERROR("AddTNodeConsumer  ERROR \n");
				tnode.Close();
				return -1;
			}
		}


	}
	*/
	
		
	tnode.RunConsumer();

	

	

	sleep(30000);

	tnode.Close();

	
    return 0;  
}  
