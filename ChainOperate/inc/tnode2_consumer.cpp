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

	unsigned int ConsumerData( char *pMsg,int nMsgLen)
	{
		XINFO("len:[%d],Msg:[%s]\n",nMsgLen,pMsg);		

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
	
	tnode.RunConsumer();

	
	while(1)
	{
		sleep(1000);
	}
    tnode.StopConsumer();	
	tnode.Close();

    return 0;  
}  
