#include <time.h>

#include "ChangeNotice.h"
#include "xlogger.h"
#include "URLMap.h"




using namespace std;





int ChangeNoticeInit()
{

	STTnodeConfig tnodeconfig ;
	tnodeconfig.mq_vhost= stConfinfo.mq_vhost_chge;
	tnodeconfig.mq_exchange_group = stConfinfo.mq_exchangeGroup_chge;
	tnodeconfig.mq_host = stConfinfo.mq_address_chge;
	tnodeconfig.mq_port= atoi(stConfinfo.mq_port_chge.c_str()) ;
	tnodeconfig.mq_user = stConfinfo.mq_user_chge;
	tnodeconfig.mq_passwd= stConfinfo.mq_password_chge;


	
	TNode *pNode = new  TNode(tnodeconfig);
	if(-1 == pNode->Init() )
	{
		XERROR("Change Notice Init fail , pls check ! \n");
		return -1;
	}

	
	char  consumer_queue[32] = {0};
	snprintf(consumer_queue,31,"sgd.%d",stConfinfo.mq_queueid_chge);
	
	CChangeNoticeConsumer *pConsumer = new CChangeNoticeConsumer(stConfinfo.mq_bindingkey_chge.c_str(),consumer_queue);
	if(-1 == pNode->AddTNodeConsumer(pConsumer) )
	{
		XERROR("AddTNodeConsumer hist consumber  ERROR \n");
		pNode->Close();
		return -1;
	}

	pNode->RunConsumer();

	XINFO("Change Notice Init ok ....\n");
	
	return 0;	
}




uint32 CChangeNoticeConsumer::ConsumerData(STTnodeDataPacketHeader *pHeader, char *pBody)
{

	CSgIpUrlMap::m_nChangeTime = time(NULL);

	return 0;
}




