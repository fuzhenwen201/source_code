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

#include "tnode.h"
#include "xlogger.h"



using namespace snetwork_xservice_tnode;
using namespace snetwork_xservice_xflagger;


void InitLogSys()
{
	(*XLogger::GetObject("new_node_test")).StdErr(true).Colour(true).Dir("./logs").BuffDelay(0);
	XINFO("InitLogSys ....");
}



class CTestTNodeConsumer : public CTNodeConsumer
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
	 	static int i = 0;
	 	time_t my = time(NULL);
	 	
		printf("data:[%s]\ni:%d,time:%ld,ConsumerData: starter:0x%04X,tag:%d,version:%d,requestid:%s,sessionid:%s,bodylen:%d,\n",pBody,++i,my,
			(pHeader->starter) ,(pHeader->tag),(pHeader->version),pHeader->requestid,pHeader->sessionid, (pHeader->bodylen)
			);
		return 0;
	 }
	

private:
	std::string	m_exchange   ;
	std::string	m_bindingkey ;
	std::string	m_queuename  ;

};

  
int main(int argc, char *argv[])  
{  
	InitLogSys();

	STTnodeConfig tnodeconfig ;
	tnodeconfig.mq_vhost= "/demo" ;
	tnodeconfig.mq_exchange_group = "t_node_grp" ;
	tnodeconfig.mq_host = "10.10.10.91" ;
	tnodeconfig.mq_port= 5672 ;
	tnodeconfig.mq_user = "demo" ;
	tnodeconfig.mq_passwd= "demo" ;




	const char *pbindingkey = "realtimedata" ;
	const char *pqueuename = "SGD_JPY.1" 	;
	const char *pqueuename2 = "SGD_JPY.2" 	;

	CTnode tnode(tnodeconfig);
	if(-1 == tnode.Init() )
	{
		printf("tnode init fail , pls check ! \n");
		return -1;
	}


	
	CTestTNodeConsumer *pConsumer = new CTestTNodeConsumer( pbindingkey, pqueuename);
	tnode.AddTNodeConsumer(pConsumer);

	CTestTNodeConsumer *pConsumer2 = new CTestTNodeConsumer( pbindingkey, pqueuename2);
	tnode.AddTNodeConsumer(pConsumer2);
	
	tnode.RunConsumer();
	

	char *pBuf =tnode.AllocDataBuff(32);
	if(NULL == pBuf)
	{
		tnode.Close();
		printf("tnode malloc fail , pls check ! \n");
		return -1;
	}
	strcpy(pBuf,"hello,world");
	
	
	
	const char *proutingkey = pbindingkey; 
	unsigned short tag = 10 ; 

	int i = 0;
	for(i = 0; i< 1000; i++)
	{
		if( -1 == tnode.PublishToMQ( proutingkey, pBuf,strlen(pBuf), tag, "requestid_122334545","sessionid_123456789" ) )
		{
			tnode.Close();
			printf("tnode PublishToMQ fail , pls check ! \n");
			return -1;
		}
		sleep(1);
	}
	

	sleep(30000);

	tnode.Close();

	
    return 0;  
}  
