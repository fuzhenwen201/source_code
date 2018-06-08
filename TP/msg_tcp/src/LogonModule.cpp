
#include "Public.h"
#include "LogonModule.h"
#include "MemPoolMgr.h"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;


using namespace snetwork_xservice_tnode;


list<STDataNode *>					CLogonProcee::m_LogonList;
pthread_mutex_t						CLogonProcee::m_LogonListLock  = PTHREAD_MUTEX_INITIALIZER  ;
	
map<unsigned long,STDataNode *> 	CLogonProcee::m_LogonAuthList;
pthread_mutex_t						CLogonProcee::m_LogonAuthListLock  = PTHREAD_MUTEX_INITIALIZER ;

list<string> 						CLogonProcee::m_LogonAuthedList;
pthread_mutex_t						CLogonProcee::m_LogonAuthedListLock  = PTHREAD_MUTEX_INITIALIZER ;



int CLogonProcee::Init()
{
	

	STTnodeConfig tnodeconfig ;
	tnodeconfig.mq_vhost= stConfinfo.mq_vhost_lg;
	tnodeconfig.mq_exchange_group = stConfinfo.mq_exchangeGroup_lg ;
	tnodeconfig.mq_host = stConfinfo.mq_address_lg ;
	tnodeconfig.mq_port= atoi(stConfinfo.mq_port_lg.c_str()) ;
	tnodeconfig.mq_user = stConfinfo.mq_user_lg ;
	tnodeconfig.mq_passwd= stConfinfo.mq_password_lg;


	
	m_pTnode = new  TNode(tnodeconfig);
	if(-1 == m_pTnode->Init() )
	{
		XERROR("logon tnode init fail , pls check ! \n");
		return -1;
	}

	char consumer_bindingkey[128]={0} ;
	snprintf(consumer_bindingkey,127,"%s.%d",stConfinfo.bindingkey_lg.c_str(),stConfinfo.queueid_lg);

	char  consumer_queue[32] = {0};
	snprintf(consumer_queue,31,"msg_logon.%d",stConfinfo.queueid_lg);
	
	LogonConsumer *pConsumer = new LogonConsumer(consumer_bindingkey, consumer_queue);
	if(-1 == m_pTnode->AddTNodeConsumer(pConsumer) )
	{
		XERROR("logon AddTNodeConsumer  ERROR \n");
		m_pTnode->Close();
		return -1;
	}

	m_pTnode->RunConsumer();

	XINFO("logon module init ....\n");

	
	return 0;	
}



CLogonProcee::CLogonProcee()
{
	m_pGroupSpread = new CGroupSpread;
	pthread_mutex_init(&m_LogonListLock,NULL);
	pthread_mutex_init(&m_LogonAuthListLock,NULL);
	pthread_mutex_init(&m_LogonAuthedListLock,NULL);
	
}





static void * LogonSendToClientThread(void *arg)
{
	CLogonProcee *pWorker = new CLogonProcee();
	pWorker->Sender();

	return NULL;
}



static int LogonStartSender()
{
	pthread_t nThreadID;
	pthread_attr_t attr;
	pthread_attr_init (&attr);   
  	pthread_attr_setscope (&attr, PTHREAD_SCOPE_SYSTEM);   
  	pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);  
	
	if (0 != pthread_create(&nThreadID, &attr, LogonSendToClientThread, NULL) )
	{
		XERROR("pthread_create error:%s\n",strerror(errno));
		return -1;
	}

	return 0;

}




void	CLogonProcee::LogonStart()
{
	 CLogonProcee *pWorker = new CLogonProcee();
 	 pWorker->Init();
     pWorker->Start();

	 LogonStartSender();

	  XINFO("logon module start ....\n");
}

int CLogonProcee::SendAuthResponse(STDataNode* &pData)
{
	
	STLogonNodeAck ack;
	int datalen = 0;

	if(-1 == pData->errNo)
	{
		datalen = 2;
		strncpy(ack.body.symbollist,"-1",datalen);
	}
	else
	{
		int fxret = m_pGroupSpread->GetSymbolsFromGroupid(1,pData->pLogon->body.groupid,pData->pLogon->header.requestid,ack.body.symbollist,COINNUMBER*COINMAXLEN, datalen) ;

		if(datalen>5)
		{
			(ack.body.symbollist)[datalen]=';';
			datalen += 1 ;
		}
		char *pBuff = ack.body.symbollist+datalen ;

		int len = 0, spacelen = COINNUMBER*COINMAXLEN - datalen ;
		int boret = m_pGroupSpread->GetSymbolsFromGroupid(0,pData->pLogon->body.groupid,pData->pLogon->header.requestid,pBuff,spacelen, len) ;
		datalen += len ;

		if(-1 == fxret && -1 == boret)
		{
			datalen = 2;
			strncpy(ack.body.symbollist,"-1",datalen);
		}
	}

	int sendlen = sizeof(STMsgHeader) + sizeof(unsigned int) + sizeof(unsigned int) + datalen ;
	
	SetMsgHeader(ack.header,sendlen,SRVVERSION,LOGIN_REQUEST_ACK,pData->pLogon->header.requestid) ;
	ack.body.groupid = htonl(pData->pLogon->body.groupid);
	ack.body.clientid = htonl(pData->pLogon->body.clientid);
	//ack.body.errNo = htonl(pData->errNo);
	
	

	SendToClient(pData,(void *) &ack,sendlen,pData->pLogon->header.requestid);
	
	CMemPoolMgr::GetInstance()->PushDataNode(pData); 

	return 0;
}

int CLogonProcee::PushDataToMQ()
{
	
	STDataNode *pData ;
	pthread_mutex_lock(&m_LogonListLock);
	pData = m_LogonList.front();
	m_LogonList.pop_front();
	pthread_mutex_unlock(&m_LogonListLock);

	pData->pLogon = (STLogonNode *)pData->szData;

	pData->pLogon->header.starter = ntohl(pData->pLogon->header.starter);
	pData->pLogon->header.version = ntohs(pData->pLogon->header.version );
	pData->pLogon->header.msgid   =  ntohl(pData->pLogon->header.msgid );
	pData->pLogon->body.groupid = ntohl(pData->pLogon->body.groupid );
	pData->pLogon->body.clientid = ntohl(pData->pLogon->body.clientid);

	
	if( STARTER != pData->pLogon->header.starter)
	{
		XERROR("PushDataToMQ  fail, srv self startchar:0x%04x,client send startchar :0x%04x,out client ip:%s,out client port:%d,socket:%d,requestid:%s\n",
				STARTER,pData->pLogon->header.starter,iptostrip(htonl(pData->objConn.ip)), pData->objConn.port,pData->objConn.socket,pData->pLogon->header.requestid);
		pData->errNo = -1;
		SendAuthResponse(pData);
		return -1;
	}

	if( SRVVERSION != pData->pLogon->header.version)
	{
		XERROR("PushDataToMQ  fail, srv self version:%d,client send version :%d,out client ip:%s,out client port:%d,socket:%d,requestid:%s\n",
				SRVVERSION,pData->pLogon->header.version,iptostrip(htonl(pData->objConn.ip)), pData->objConn.port,pData->objConn.socket,pData->pLogon->header.requestid);
		pData->errNo = -1;
		SendAuthResponse(pData);
		return -1;
	}

	
	// 目前先不认证, 在这里就认为认证成功, 根据錱roupid取出symbol list 发给用户
	// 以后需要认证模块后, 把这一步移到 SendAuthResponseToClient 
	{
		pData->errNo = 0;
		SendAuthResponse(pData);
		return 0;
	}
	
	
	/*
	if( pData->pLogon->body.groupid == 0 || pData->pLogon->body.clientid == 0 )
	{
		XERROR("PushDataToMQ  fail, userid error:: groupid:%d, clientid:%d,out client ip:%s,out client port:%d,socket:%d,requestid:%s\n",
				pData->pLogon->body.groupid,pData->pLogon->body.clientid,iptostrip(htonl(pData->objConn.ip)), pData->objConn.port,pData->objConn.socket,pData->pLogon->header.requestid);
		pData->errNo = -1;
		SendAuthResponse(pData);
		return -1;
	}
	*/

	map<unsigned long,STDataNode *>::iterator itr ;
	itr = m_LogonAuthList.find(pData->pLogon->body.groupid + pData->pLogon->body.clientid);
	if(itr != m_LogonAuthList.end())
	{
		
		XINFO("PushDataToMQ  user multi logon !!!:: groupid:%d, clientid:%d,out client ip:%s,out client port:%d,socket:%d,requestid:%s\n",
				pData->pLogon->body.groupid,pData->pLogon->body.clientid,iptostrip(htonl(pData->objConn.ip)), pData->objConn.port,pData->objConn.socket,pData->pLogon->header.requestid);
		CMemPoolMgr::GetInstance()->PushDataNode(pData); 
		return 0;
	}

	//后期优化成不用json
	char requestid[37]={0};
	memcpy(requestid,pData->pLogon->header.requestid, sizeof(pData->pLogon->header.requestid) );
	char data[512]={0};
	snprintf(data,511,"{\"password\":\"%s\",\"sgid\":%d,\"clientId\":%d,\"groupId\":%d,\"content\":\"%s\"}", 
			pData->pLogon->body.password,stConfinfo.queueid_lg, pData->pLogon->body.clientid,pData->pLogon->body.groupid,requestid);

	
	
	int len = strlen(data)+1;
	char *  buff = m_pTnode->AllocDataBuff(len);
	memset(buff,0,len );
    memcpy(buff, data, len-1 );
	
	
	m_pTnode->PublishToMQ( stConfinfo.routingkey_lg.c_str(), buff, len , 
					pData->pLogon->header.msgid, pData->pLogon->header.requestid,"sessionid_123456789");
	
	//pTnode->PublishToMQ(stConfinfo.mq_queue_lg_push_grp,stConfinfo.mq_queue_lg_push_grp_name, buff, len + 1, pData->pLogon->header.msgid);
	//PrintCurrTime("LogonPushMQ");
	

	XINFO("PushDataToMQ LogonData : msgid:0x%04x  groupid:%d, clientid:%d,password:%s,out client ip:%s,out client port:%d,socket:%d,data:[%s],requestid:[%s],len:%d\n",
				pData->pLogon->header.msgid,pData->pLogon->body.groupid,pData->pLogon->body.clientid,pData->pLogon->body.password,
				iptostrip(htonl(pData->objConn.ip)), pData->objConn.port,pData->objConn.socket,data,pData->pLogon->header.requestid,len);
	
	m_LogonAuthList.insert(pair<unsigned long,STDataNode *>(pData->pLogon->body.groupid + pData->pLogon->body.clientid,pData));


	return 0;
	
}




void CLogonProcee::PushAuthedData( char *event)
{
	pthread_mutex_lock(&m_LogonAuthedListLock);
	m_LogonAuthedList.push_back(event);
	pthread_mutex_unlock(&m_LogonAuthedListLock);
	
}



int 	CLogonProcee::ConsumerProcessData(uint16 tag, char *event, uint32 eventlen)
{
	/*
	if(LOGIN_REQUEST_ACK != tag)
	{
		XERROR("ConsumerProcessData,from mq tag:0x%04x is not 0x%04x , data:%s\n",tag, LOGIN_REQUEST_ACK, event);
		return -1;
	}
	*/


	XINFO("ConsumerProcessData Recv data:[%s]\n",event);

	CLogonProcee::PushAuthedData(event);

	return 0;

}


/*

{
"errorCode":0/-1,
"groupId":1,
"clientId":2,
"sgid":1,
"requestid":"xxxxx"
}
*/
int CLogonProcee::SendAuthResponseToClient()
{
	

	if(0 == m_LogonAuthedList.size())return -1;

	
	
	string data;
	pthread_mutex_lock(&m_LogonAuthedListLock);
	data = m_LogonAuthedList.front();
	m_LogonAuthedList.pop_front();
	pthread_mutex_unlock(&m_LogonAuthedListLock);


	int errorCode =-1 ,groupId=-1,clientId=-1,sgid =-1;
	string requestid;
	
	using rapidjson::Document;
	Document doc;
	using rapidjson::Value;

	doc.Parse(data.c_str());
	
	Value & v = doc["errorCode"];
	errorCode = v.GetInt() ;
	
	Value & v2 = doc["groupId"];
	groupId = v2.GetInt() ;
	
	Value & v3 = doc["clientId"];
	clientId = v3.GetInt() ;
	
	Value & v4 = doc["sgid"];
	sgid = v4.GetInt() ;

	Value & v5 = doc["requestid"];
	requestid = v5.GetString() ;

	map<unsigned long,STDataNode *>::iterator itr ;
	itr = m_LogonAuthList.find(groupId + clientId);
	if(itr == m_LogonAuthList.end())
	{
		XERROR("SendAuthResponseToClient,from mq  user not find in local, groupid:%d, clientid:%d\n",groupId,clientId);
		return -1;
	}

	STDataNode *pData = (STDataNode *)(itr->second);
	m_LogonAuthList.erase(itr);
	pData->errNo = errorCode;

	if(0 == errorCode )pData->pConn->logon = 1;


	XINFO("Logon send to client::errNo:%d, groupid:%d, clientid:%d\n",errorCode,groupId,clientId);

	m_pGroupSpread->ShowGroupSymbolsList(1);
	SendAuthResponse(pData);


	return 0;
}





int 	CLogonProcee::LRModuleProcessLogonRequest(STDataNode* &pData)
{
	
	STLogonNode	*pLogon = (STLogonNode *)pData->szData;

	XINFO("LRModuleProcessLogonRequest :: recv logon packet, client ip:%s, client port:%d,socket : %d, requestid:%s, groupid:%d,clientid:%d,passwd:%s \n",
					iptostrip(htonl(pData->objConn.ip)), pData->objConn.port,pData->objConn.socket, 
					pLogon->header.requestid,  ntohl(pLogon->body.groupid), ntohl(pLogon->body.clientid),pLogon->body.password);

	
	
	pthread_mutex_lock(&m_LogonListLock);
	m_LogonList.push_back(pData);
	pthread_mutex_unlock(&m_LogonListLock);
	
	return 0;
}
	
void 	CLogonProcee::Run()
{

	
	
	while(1)
	{
		m_pGroupSpread->ReadGroupSymbolsList();
		m_pGroupSpread->ReadGroupBoSymbolsList();

		
		
		if(m_LogonList.size() != 0)
		{
			
			PushDataToMQ();
			continue;
		}
		
		usleep(2000);
			
	}
}


void 	CLogonProcee::Sender()
{
	while(1)
	{
		m_pGroupSpread->ReadGroupSymbolsList();
		m_pGroupSpread->ReadGroupBoSymbolsList();
		
		if( SendAuthResponseToClient() != 0 )
		{
			usleep(2000);
		}
	}

}





/*
uint32 LogonConsumer::EventHandle(uint16 tag, char *event, uint32 eventlen) 
{
	PrintCurrTime("LogonRecvMQ");
	CLogonProcee::ConsumerProcessData(tag,event,eventlen);
	
	return UNIVERSAL_TNODE_SUCCESS;
}

*/



uint32 LogonConsumer::ConsumerData(STTnodeDataPacketHeader *pHeader, char *pBody)
{


	XINFO("GetDataFromMQ LogonData: bodylen:%d,tag:%d,body:%s\n",pHeader->bodylen,pHeader->tag,pBody);
	
	CLogonProcee::ConsumerProcessData(pHeader->tag,pBody,pHeader->bodylen);

	return 0;
}






