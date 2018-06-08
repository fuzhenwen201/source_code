
#include "Public.h"
#include "HistDataModule.h"
#include "MemPoolMgr.h"




using namespace snetwork_xservice_tnode;


list<STDataNode *>	CHistDataProcess::m_HistDataList;
pthread_mutex_t		CHistDataProcess::m_HistDataListLock  = PTHREAD_MUTEX_INITIALIZER ;

list<STDataNode *>  CHistDataProcess::m_HistDataProcessList;
pthread_mutex_t 	CHistDataProcess::m_HistDataProcessListLock = PTHREAD_MUTEX_INITIALIZER ;

list<string>		CHistDataProcess::m_HistDataProcessedList;
pthread_mutex_t	 	CHistDataProcess::m_HistDataProcessedListLock = PTHREAD_MUTEX_INITIALIZER ;




int CHistDataProcess::Init()
{

	STTnodeConfig tnodeconfig ;
	tnodeconfig.mq_vhost= stConfinfo.mq_vhost_hist;
	tnodeconfig.mq_exchange_group = stConfinfo.mq_exchangeGroup_hist;
	tnodeconfig.mq_host = stConfinfo.mq_address_hist;
	tnodeconfig.mq_port= atoi(stConfinfo.mq_port_hist.c_str()) ;
	tnodeconfig.mq_user = stConfinfo.mq_user_hist;
	tnodeconfig.mq_passwd= stConfinfo.mq_password_hist;


	
	
	m_pTnode = new  TNode(tnodeconfig);
	if(-1 == m_pTnode->Init() )
	{
		XERROR("hist tnode init fail , pls check ! \n");
		return -1;
	}

	char consumer_bindingkey[128]={0} ;
	snprintf(consumer_bindingkey,127,"%s.%d",stConfinfo.bindingkey_hist.c_str(),stConfinfo.queueid_hist);

	char  consumer_queue[32] = {0};
	snprintf(consumer_queue,31,"msg_hist.%d",stConfinfo.queueid_hist);

	XINFO("HistModule:consumer_bindingkey:%s,consumer_queue:%s\n",consumer_bindingkey,consumer_queue);

	
	CHistDataConsumer *pConsumer = new CHistDataConsumer(consumer_bindingkey, consumer_queue);
	if(-1 == m_pTnode->AddTNodeConsumer(pConsumer) )
	{
		XERROR("AddTNodeConsumer hist consumber  ERROR \n");
		m_pTnode->Close();
		return -1;
	}

	m_pTnode->RunConsumer();

	XINFO("Histdata tnode module init ....\n");
	
	return 0;	
}



CHistDataProcess::CHistDataProcess()
{	
	m_pGroupSpread = new CGroupSpread;
	
}




static void * SendToClientThread(void *arg)
{
	CHistDataProcess *pWorker = new CHistDataProcess();
	pWorker->Sender();

	return NULL;
}


static int StartSender()
{
	pthread_t nThreadID;
	pthread_attr_t attr;
	pthread_attr_init (&attr);   
  	pthread_attr_setscope (&attr, PTHREAD_SCOPE_SYSTEM);   
  	pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);  
	
	if (0 != pthread_create(&nThreadID, &attr, SendToClientThread, NULL) )
	{
		XERROR("pthread_create error:%s\n",strerror(errno));
		return -1;
	}

	return 0;

}



void	CHistDataProcess::HistDataStart()
{
	 CHistDataProcess *pWorker = new CHistDataProcess();
 	 pWorker->Init();
     pWorker->Start();
	 StartSender();
	 XINFO("History module start ....\n");
}



int CHistDataProcess::SendHistDataResponse(STDataNode* &pData)
{

	
	STHistDataResponse ack;

	
	
	int sendlen = sizeof(ack.header)+sizeof(ack.body)-sizeof(ack.body.data);
	SetMsgHeader(ack.header,sendlen,SRVVERSION,HISTMSG_REQUEST_ACK,pData->pHistData->header.requestid) ;
	ack.body.groupid = htonl(pData->pHistData->body.groupid);
	ack.body.clientid = htonl(pData->pHistData->body.clientid);
	ack.body.errNo = htonl(pData->errNo);

	
	char *pBody = pData->szData + sizeof(STMsgHeader) + 8;
	memcpy(pBody+12,pBody,16);
	ack.body.datalen= htonl(0);

	SendToClient(pData,(void *) &ack,sendlen,pData->pHistData->header.requestid);
	
	CMemPoolMgr::GetInstance()->PushDataNode(pData); 

	return 0;
}



int CHistDataProcess::SendHistDataResponse(STDataNode* &pData,string *pBuf)
{
	
	
	STHistDataResponse ack;

	int bodylen = pBuf->size();
	
	if( bodylen > MAXHISTORYDATA-1 )
	{
		XERROR("mq body len of History reponse packet  is too long:%d, bufflen:%d\n",bodylen,MAXHISTORYDATA-1);
		CMemPoolMgr::GetInstance()->PushDataNode(pData); 
		return -1;
	}	
	int sendlen = sizeof(ack.header)+12+bodylen-44 ; // 44 : pbuf :  requestid(36) + grouid(4) + client(4)  = total size 44

	//printf("SendHistDataResponse::bodylen:%ld,sendlen:%d\n",bodylen,sendlen);

	SetMsgHeader(ack.header,sendlen,SRVVERSION,HISTMSG_REQUEST_ACK,pData->pHistData->header.requestid) ;
	ack.body.groupid = htonl(pData->pHistData->body.groupid);
	ack.body.clientid = htonl(pData->pHistData->body.clientid);
	ack.body.errNo = htonl(pData->errNo);

	
	unsigned char *pBody = (unsigned char *)&(ack.body);
	memcpy(pBody+12,pBuf->c_str()+44,bodylen-44);


	XINFO("SendHistDataResponse: groupid:%d, clientid:%d,out client ip:%s,out client port:%d,socket:%d,requestid:%s\n",
				pData->pHistData->body.groupid,pData->pHistData->body.clientid,
				iptostrip(htonl(pData->objConn.ip)), pData->objConn.port,pData->objConn.socket,pData->pHistData->header.requestid);


	SendToClient(pData,(void *) &ack,sendlen, pData->pHistData->header.requestid);
	
	CMemPoolMgr::GetInstance()->PushDataNode(pData); 

	return 0;
}

int CHistDataProcess::PushDataToMQ()
{
	if(m_HistDataList.size() == 0) 
	{
		return -1;
	}
	STDataNode *pData ;
	pthread_mutex_lock(&m_HistDataListLock);
	pData = m_HistDataList.front();
	m_HistDataList.pop_front();
	pthread_mutex_unlock(&m_HistDataListLock);

	pData->pHistData = (STHistDataRequest *)pData->szData;

	pData->pHistData->header.starter = ntohl(pData->pHistData->header.starter);
	pData->pHistData->header.version = ntohs(pData->pHistData->header.version );
	pData->pHistData->header.msgid   =  ntohl(pData->pHistData->header.msgid );
	pData->pHistData->body.groupid = ntohl(pData->pHistData->body.groupid );
	pData->pHistData->body.clientid = ntohl(pData->pHistData->body.clientid);


	
	char *pSymbol = pData->pHistData->body.coinname; 

	// BO
	if( ( pSymbol[6] =='b' && pSymbol[7]=='o' ) )
	{
		char symbol[12]={0};
		memcpy(symbol, pSymbol, 8 );
		int value ;
		int ret = m_pGroupSpread->GetValueFromGroupid(0, pData->pHistData->body.groupid, pData->pHistData->header.requestid,symbol,value) ;
		if(-1 == ret)
		{
			pData->errNo = -1;
			SendHistDataResponse(pData);
			return -1;
		}
	}
	else // FX
	{
		int value ;
		int ret = m_pGroupSpread->GetValueFromGroupid(1, pData->pHistData->body.groupid, pData->pHistData->header.requestid,pSymbol,value) ;
		if(-1 == ret)
		{
			pData->errNo = -1;
			SendHistDataResponse(pData);
			return -1;
		}
	}

	
	if( STARTER != pData->pHistData->header.starter)
	{
		XERROR("PushDataToMQ  fail, srv self startchar:0x%04x,client send startchar :0x%04x,out client ip:%s,out client port:%d,socket:%d,requestid:%s\n",
				STARTER,pData->pHistData->header.starter,iptostrip(htonl(pData->objConn.ip)), pData->objConn.port,pData->objConn.socket,pData->pHistData->header.requestid);
		pData->errNo = -1;
		SendHistDataResponse(pData);
		return -1;
	}

	if( SRVVERSION != pData->pHistData->header.version)
	{
		XERROR("PushDataToMQ  fail, srv self version:%d,client send version :%d,out client ip:%s,out client port:%d,socket:%d,requestid:%s\n",
				SRVVERSION,pData->pHistData->header.version,iptostrip(htonl(pData->objConn.ip)), pData->objConn.port,pData->objConn.socket,pData->pHistData->header.requestid);
		pData->errNo = -1;
		SendHistDataResponse(pData);
		return -1;
	}



	int len = sizeof(STMsgToDC) ;
	char *  buff = m_pTnode->AllocDataBuff(len +  1);
	memset(buff,0,len + 1);
	memcpy(buff, pData->szData+12, len-4);
	memcpy(buff +len-4, &(stConfinfo.queueid_hist),4); // 提醒DC msg的消费queue 的全名是 msg.queueid , msg.1 ,msg.2 .....

	
	
	STMsgToDC *pSTToDc = (STMsgToDC *)buff;
	pSTToDc->groupid = htonl(pSTToDc->groupid);
	pSTToDc->clientid = htonl(pSTToDc->clientid);
	pSTToDc->bodylen = htonl(pSTToDc->bodylen); // msg index 
	
	//pTnode->GetChannel().PublishGroup(stConfinfo.mq_queue_hist_push_grp,stConfinfo.mq_queue_hist_push_grp_name, buff, len + 1, pData->pHistData->header.msgid);

	m_pTnode->PublishToMQ( stConfinfo.routingkey_hist.c_str(), buff, len + 1, 
					pData->pHistData->header.msgid, pData->pHistData->header.requestid,"sessionid_123456789");

	
	XINFO("PushDataToMQ HistData ok:send len:%d, groupid:%d, clientid:%d, requestid:%s, symbol_time:%s,candlecounts:%u",len + 1,
		ntohl(pSTToDc->groupid),ntohl(pSTToDc->clientid),pSTToDc->requestid,
		pSTToDc->coinname, ntohl(pSTToDc->candlecounts)
	);

	

	pthread_mutex_lock(&m_HistDataProcessListLock);
	m_HistDataProcessList.push_back(pData);
	pthread_mutex_unlock(&m_HistDataProcessListLock);

	return 0;
	
}




int 	CHistDataProcess::LRModuleProcessHistDataRequest(STDataNode* &pData)
{
	
	socklen_t snd_size = 300000;
	socklen_t optlen = sizeof(snd_size);
	setsockopt(pData->objConn.socket, SOL_SOCKET, SO_SNDBUF, &snd_size, optlen);
	
	STHistDataRequest *pHistData = (STHistDataRequest *)pData->szData ;

	XINFO("recv hist packet request, requestid:%s,groupid:%u,clientid:%u, symbol:%s,candlecounts:%d, socket:%d\n",
					pHistData->header.requestid, ntohl(pHistData->body.groupid), ntohl(pHistData->body.clientid),   pHistData->body.coinname, 
					ntohl(pHistData->body.candlecounts),
					pData->objConn.socket);
	
	
	CHistDataProcess::LRModulePushHistData(pData);

	return 0;
}


void CHistDataProcess::LRModulePushHistData( STDataNode* &pData)
{
	pthread_mutex_lock(&m_HistDataListLock);
	m_HistDataList.push_back(pData);
	XINFO("Recv client historydata and push buffer:%ld\n",m_HistDataList.size());
	pthread_mutex_unlock(&m_HistDataListLock);
}




void CHistDataProcess::PushFromMQHistData( char *event,uint32 eventlen)
{
	string data(event,eventlen);
	pthread_mutex_lock(&m_HistDataProcessedListLock);
	m_HistDataProcessedList.push_back(data);
	pthread_mutex_unlock(&m_HistDataProcessedListLock);
}



int 	CHistDataProcess::ConsumerProcessData(uint16 tag, char *event, uint32 eventlen)
{

		
	if(HISTMSG_REQUEST_ACK != tag)
	{
		XERROR("hist data ConsumerProcessData,from mq tag:0x%04x is not 0x%04x , data:%s\n",tag, HISTMSG_REQUEST_ACK, event);
		return -1;
	}


	CHistDataProcess::PushFromMQHistData(event,eventlen);

	return 0;

}





int CHistDataProcess::FindClientInfo(char *pSrcs, const char *pDests)
{
	STMsgToDC *pSrc =  (STMsgToDC *)pSrcs;
	const STMsgToDC *pDest = (const STMsgToDC *)pDests;

	XINFO("SendHistDataResponseToClient From TT info: requestid:%s, groupid:%d, clientid:%d, candlecounts:%d, symbol:%s, granularity:%s  \n",
					pSrc->requestid, pSrc->groupid,pSrc->clientid, ntohl(pSrc->candlecounts),pSrc->coinname,  pSrc->granularity);

	if( 0 != memcmp(pSrc->requestid,pDest->requestid, sizeof(pSrc->requestid) ) )return -1;

	if( pSrc->groupid != pDest->groupid )return -1;
	
	if( pSrc->clientid != pDest->clientid )return -1;

	if( 0 != memcmp(pSrc->coinname,pDest->coinname, sizeof(pSrc->coinname) ) )return -1;

	if( 0 != memcmp(pSrc->granularity,pDest->granularity, sizeof(pSrc->granularity) ) )return -1;

	return 0;
}



int CHistDataProcess::SendHistDataResponseToClient()
{
		STDataNode *pData = NULL;
	
		
		if(0 == m_HistDataProcessedList.size())
		{
			return -1;
		}
		
		string data;
		pthread_mutex_lock(&m_HistDataProcessedListLock);
		data = m_HistDataProcessedList.front();
		m_HistDataProcessedList.pop_front();
		pthread_mutex_unlock(&m_HistDataProcessedListLock);
		
	
		int ret = -1;
		list<STDataNode*>::iterator itr;
		for (itr =m_HistDataProcessList.begin(); itr != m_HistDataProcessList.end(); )
		{
			pData = (STDataNode *)*itr;
			char *pBuf = pData->szData + 12;
			
			if( 0 == FindClientInfo(pBuf,data.c_str())) 
			{
				pthread_mutex_lock(&m_HistDataProcessListLock);
				m_HistDataProcessList.erase(itr++);
				pthread_mutex_unlock(&m_HistDataProcessListLock);
				
				pData->errNo = ERRCODE_OK ;
				SendHistDataResponse(pData,&data);
				ret = 0;
			}
			else
			{
				itr++ ;
			}
			
		}
		   
		if(-1 == ret)
		{
			STMsgToDC *pSTToDc = (STMsgToDC *) data.c_str();
	
			XINFO("SendHistDataResponseToClient not find client: requestid:%s, groupid:%d, clientid:%d, candlecounts:%d, symbol:%s, granularity:%s, bodylen:%d,size:%ld\n",
						pSTToDc->requestid, pSTToDc->groupid,pSTToDc->clientid,ntohl(pSTToDc->candlecounts),pSTToDc->coinname,pSTToDc->granularity,ntohl( pSTToDc->bodylen),data.size());
			
		}
		
	
		return 0;
}


	
void 	CHistDataProcess::Run()
{
	while(1)
	{
		m_pGroupSpread->ReadGroupSymbolsList();
		m_pGroupSpread->ReadGroupBoSymbolsList();
		
		if(PushDataToMQ() != 0)
		{
			usleep(2000);	
		}
	}
}



void 	CHistDataProcess::Sender()
{
	while(1)
	{
		if( SendHistDataResponseToClient() != 0 )
		{
			usleep(2000);
		}
	}

}




uint32 CHistDataConsumer::ConsumerData(STTnodeDataPacketHeader *pHeader, char *pBody)
{
	//printf("sbe len:%d\n",pHeader->bodylen - 64  );	
	//DecodeHistory(pBody+64,pHeader->bodylen - 64 );
	
	
	STMsgToDC *pSTToDc = (STMsgToDC *) pBody;

	
	pSTToDc->groupid = ntohl(pSTToDc->groupid);
	pSTToDc->clientid = ntohl(pSTToDc->clientid) ;

	XINFO("GetDataFromMQ HistData: groupid:%d, clientid:%d, requestid:%s, currency pair:%s,time:%s,candlecounts:%u,sbelen:%d,eventlen:%u\n",
				(pSTToDc->groupid),(pSTToDc->clientid),pSTToDc->requestid,
				pSTToDc->coinname,pSTToDc->granularity,ntohl(pSTToDc->candlecounts),ntohl( pSTToDc->bodylen),pHeader->bodylen);
	

	CHistDataProcess::ConsumerProcessData(pHeader->tag,pBody,pHeader->bodylen);


	return 0;
}


