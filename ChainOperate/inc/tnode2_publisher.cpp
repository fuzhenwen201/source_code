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

	

	int i = 0;
	for(i = 1; i< 10000000; i++)
	{	
		char pBuf2[64]={0};
		sprintf(pBuf2,"SAVIN_%d",i);
		int len = strlen(pBuf2)+1 ;
		if( -1 == tnode.PublishToMQ( proutingkey, pBuf2,len) )
	    {
	    	tnode.Close();
	        printf("tnode PublishToMQ fail , pls check ! \n");
	        return -1;
	    }
		//sleep(1);
    	printf("send to mq  :%d,len:%d,[%s]...\n",i,len,pBuf2);
	}

	
	tnode.Close();

	
    return 0;  
}  
