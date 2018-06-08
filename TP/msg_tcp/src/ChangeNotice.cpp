#include <time.h>

#include "ChangeNotice.h"
#include "xlogger.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "Group_Spread.h"


using namespace std;
using namespace rapidjson;



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
	snprintf(consumer_queue,31,"msg_dbchge.%d",stConfinfo.mq_queueid_chge);

	
	XINFO("ChangeNotice :consumer_bindingkey:%s,consumer_queue:%s\n",stConfinfo.mq_bindingkey_chge.c_str(),consumer_queue);
	
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

	/*
	using rapidjson::Document;
	Document doc;
	doc.Parse<0>(pBody);
	if (doc.HasParseError()) 
	{
			XERROR("CChangeNoticeConsumer parse json fail,json:%s",pBody);
			return 0;
	}
	using rapidjson::Value;
	Value & v = doc["changed_groupid"];
	if (!v.IsInt()) 
	{
		XERROR("CChangeNoticeConsumer get groupid from json fail,json:%s",pBody);
		return 0;
	}

	printf("change groupid :%d\n",v.GetInt());
	*/
	
	XINFO("CChangeNoticeConsumer recv notice msg:%s",pBody);
	if( NULL != strstr(pBody,"group_security_changed"))
	{
		CGroupSpread::m_nChangeTime_grpsecurity= time(NULL);
	}
	else if( NULL != strstr(pBody,"group_boinfo_changed"))
	{
		CGroupSpread::m_nChangeTime_bosymbols= time(NULL);
	}
	else
	{
		XERROR("CChangeNoticeConsumer recv error notice msg:%s, msg don't support",pBody);
	}

	return 0;
}




