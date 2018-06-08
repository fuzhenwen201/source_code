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


  
int main(int argc, char *argv[])  
{  
	if(argc!=5)
	{
		printf("Usage:./tnode2_publisher ip exchange routingkey content\n");
		return -1;
	}
	char ip[32]={0};
	strncpy(ip,argv[1],31);

	char proutingkey[128]={0};
	strncpy(proutingkey,argv[3],127);

	char content[256]={0};
    strncpy(content,argv[4],255);

	
	printf("./tnode2_publisher mq_exchange_group:%s, proutingkey:%s, content:%s\n",ip,argv[2],proutingkey,content);


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

	unsigned short tag = 10 ; 

	int bodymemlen =strlen(content)+1; 
    char *pBuf =tnode.AllocDataBuff(bodymemlen);
    if(NULL == pBuf)
    {
        tnode.Close();
        printf("tnode malloc fail , pls check ! \n");
        return -1;
    }
	strncpy(pBuf,content,bodymemlen-1);

	int i = 0;
	//for(i = 0; i< 10000; i++)
	{	
	if( -1 == tnode.PublishToMQ( proutingkey, pBuf,bodymemlen, tag, "requestid_122334545","sessionid_123456789" ) )
		return 0;
	if( -1 == tnode.PublishToMQ( proutingkey, pBuf,bodymemlen, tag) )
    {
    	tnode.Close();
        tnode.FreeDataBuff(pBuf);
        printf("tnode PublishToMQ fail , pls check ! \n");
        return -1;
    }
	sleep(1);
    printf("send to mq  :%d...\n",i);
	}

	

	/*
	int bodymemlen =512; 
	char *pBuf =tnode.AllocDataBuff(bodymemlen);
	if(NULL == pBuf)
	{
		tnode.Close();
		printf("tnode malloc fail , pls check ! \n");
		return -1;
	}
	//strcpy(pBuf,"abc1234567");
	memset(pBuf,'a',bodymemlen-sizeof(STTnodeDataPacketHeader));
		
		
	int i = 0;
	XINFO("begin publish ...\n");
	for(i = 0; i< 100000; i++)
	{
		
		char *pBuf2 = pBuf + (bodymemlen - 20) ;
		
		sprintf(pBuf2,"SAVIN_%d",i);
		if( -1 == tnode.PublishToMQ( proutingkey, pBuf,bodymemlen, tag, "requestid_122334545","sessionid_123456789" ) )
		{
			tnode.Close();
			tnode.FreeDataBuff(pBuf);
			printf("tnode PublishToMQ fail , pls check ! \n");
			return -1;
		}
		//printf("send to mq [%d] ....\n",i);

		
		
		//sleep(1);
	}
	XINFO("end publish ...\n");
	tnode.FreeDataBuff(pBuf);
	*/

	/*
	int j = 0;
	for(j;j<1000;j++)
	{
	
		int i = 0;
		
		for(i = 0; i< 3; i++)
		{
			int bodymemlen =128; 
			char *pBuf =tnode.AllocDataBuff(bodymemlen);
			memset(pBuf,'a',bodymemlen-sizeof(STTnodeDataPacketHeader));
			char *pBuf2 = pBuf + (bodymemlen - 20) ;
			const char *proutingkey = "AUD_CAD";
			sprintf(pBuf2,"%s_%d_%d",proutingkey,i,j);
			tnode.PublishToMQ( proutingkey, pBuf,bodymemlen, tag, "reqid_AUD_CAD","sessionid_123456789" ) ;
			printf("send to mq [%d]  AUD_CAD....\n",i);
			tnode.FreeDataBuff(pBuf);
		}

		for(i = 0; i< 5; i++)
		{
			int bodymemlen =256; ; 
			char *pBuf =tnode.AllocDataBuff(bodymemlen);
			memset(pBuf,'a',bodymemlen-sizeof(STTnodeDataPacketHeader));
			char *pBuf2 = pBuf + (bodymemlen - 20) ;
			const char *proutingkey = "AUD_CHF";
			sprintf(pBuf2,"%s_%d_%d",proutingkey,i,j);
			tnode.PublishToMQ( proutingkey, pBuf,bodymemlen, tag, "reqid_AUD_CHF","sessionid_123456789" ) ;
			printf("send to mq [%d] AUD_CHF....\n",i);
			tnode.FreeDataBuff(pBuf);
		}

		for(i = 0; i< 2; i++)
		{
			int bodymemlen =512; 
			char *pBuf =tnode.AllocDataBuff(bodymemlen);
			memset(pBuf,'a',bodymemlen-sizeof(STTnodeDataPacketHeader));
			char *pBuf2 = pBuf + (bodymemlen - 20) ;
			const char *proutingkey = "AUD_HKD";
			sprintf(pBuf2,"%s_%d_%d",proutingkey,i,j);
			tnode.PublishToMQ( proutingkey, pBuf,bodymemlen, tag, "reqid_AUD_HKD","sessionid_123456789" ) ;
			printf("send to mq [%d] AUD_HKD....\n",i);
			tnode.FreeDataBuff(pBuf);
		}
		
		
	}
	*/

	/*
	std::vector<std::string>  symbols_ = {"AUD_CAD","AUD_CHF","AUD_HKD","AUD_JPY","AUD_NZD","AUD_SGD","AUD_USD","CAD_CHF","CAD_HKD","CAD_JPY","CAD_SGD","CHF_HKD","CHF_JPY",\
    "CHF_ZAR","EUR_AUD","EUR_CAD","EUR_CHF","EUR_CZK","EUR_DKK","EUR_GBP","EUR_HKD","EUR_HUF","EUR_JPY","EUR_NOK","EUR_NZD","EUR_PLN","EUR_SEK","EUR_SGD",\
    "EUR_TRY","EUR_USD","EUR_ZAR","GBP_AUD","GBP_CAD","GBP_CHF","GBP_HKD","GBP_JPY","GBP_NZD","GBP_PLN","GBP_SGD","GBP_USD","GBP_ZAR","HKD_JPY","NZD_CAD",\
    "NZD_CHF","NZD_HKD","NZD_JPY","NZD_SGD","NZD_USD","SGD_CHF","SGD_HKD","SGD_JPY","TRY_JPY","USD_CAD","USD_CHF","USD_CNH","USD_CZK","USD_DKK","USD_HKD",\
    "USD_HUF","USD_INR","USD_JPY","USD_MXN","USD_NOK","USD_PLN","USD_SAR","USD_SEK","USD_SGD","USD_THB","USD_TRY","USD_ZAR","ZAR_JPY"};

	XINFO("begin publish ...\n");
	int j = 0;
	for(j;j<20000;j++)
	{
	
		int i = 0;
		for(i;i<symbols_.size();i++)
		{
			int bodymemlen =512; 
			char *pBuf =tnode.AllocDataBuff(bodymemlen);
			memset(pBuf,'a',bodymemlen-sizeof(STTnodeDataPacketHeader));
			char *pBuf2 = pBuf + (bodymemlen - 20) ;
			const char *proutingkey =symbols_[i].c_str();
			sprintf(pBuf2,"%s_%d_%d",proutingkey,j,i);

			char requestid[17]={0};
			sprintf(requestid,"reqid_%s",symbols_[i].c_str());
			
			tnode.PublishToMQ( proutingkey, pBuf,bodymemlen, tag, requestid,"sessionid_123456789" ) ;
			tnode.FreeDataBuff(pBuf);
			printf("send to mq [%d] ,%s....\n",j,symbols_[i].c_str());
		}
	}
	XINFO("%d published ...\n",j);
	
	*/
	


	//sleep(30000);

	tnode.Close();

	
    return 0;  
}  
