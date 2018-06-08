#include <time.h>

#include "ChangeNotice.h"
#include "xlogger.h"

#include "Group_Spread.h"


using namespace std;




int ChangeNoticeInit()
{

	STTnodeConfig tnodeconfig ;
	tnodeconfig.mq_vhost= stServiceConfig.mq_vhost_chge;
	tnodeconfig.mq_exchange_group = stServiceConfig.mq_exchangeGroup_chge;
	tnodeconfig.mq_host = stServiceConfig.mq_address_chge;
	tnodeconfig.mq_port= atoi(stServiceConfig.mq_port_chge.c_str()) ;
	tnodeconfig.mq_user = stServiceConfig.mq_user_chge;
	tnodeconfig.mq_passwd= stServiceConfig.mq_password_chge;


	
	TNode *pNode = new  TNode(tnodeconfig);
	if(-1 == pNode->Init() )
	{
		XERROR("Change Notice Init fail , pls check ! \n");
		return -1;
	}

	
	char  consumer_queue[32] = {0};
	snprintf(consumer_queue,31,"fix_se.%d",stServiceConfig.mq_queueid_chge);
	
	CChangeNoticeConsumer *pConsumer = new CChangeNoticeConsumer(stServiceConfig.mq_bindingkey_chge.c_str(),consumer_queue);
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

	CGroupSpread::m_nChangeTime = time(NULL);

	return 0;
}




