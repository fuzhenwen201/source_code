
#include <stdlib.h>


#include "WokerModule.h"

#include "MemPoolMgr.h"

#include "realtime/MessageHeader.h"
#include "realtime/Realtimedatas.h"






MarketDataAgent marketAgent;


list<CWorkerProcee *> CWorkerProcee::m_WorkerList;



typedef struct _STRealtimeData
{
    unsigned int    status;
    int             bid;
    int             ask;
	int 			spread;
    unsigned long   timestamp;
    char            instrument[8];

    _STRealtimeData(unsigned int _status,int _bid,int _ask,int _spread,unsigned long int _timestamp,const char *_instrument)
    {
        status = _status;
        bid = _bid;
        ask = _ask;
		spread = _spread ;
        timestamp = _timestamp;
        memcpy(instrument,_instrument,8);
        
    }

    _STRealtimeData(const _STRealtimeData &thr)
    {
        status = thr.status;
        bid = thr.bid;
        ask = thr.ask;
		spread = thr.spread;
        timestamp =  thr.timestamp;
        memcpy(instrument, thr.instrument,8);
       
		
    }

} __attribute__((packed)) STRealtimeData ;





CWorkerProcee::CWorkerProcee()
{
	m_pGroupSpread = new CGroupSpread;
	
	m_nClientNumber = 0;
	pthread_mutex_init(&m_RTOperateListLock,NULL);
}






CWorkerProcee::~CWorkerProcee()
{
	XINFO("worker exit ....\n");
}








// 在创建worker时,调用,  该list用于 Inservice 分发内部msg到每个worker用
void CWorkerProcee::AddWorkerToList()
{
	m_WorkerList.push_back(this);
}





int CWorkerProcee::DeleteOrUpdateorAdd(STDataNode* &pData, int number)
{
	pthread_mutex_lock(&m_RTOperateListLock);
	m_nClientNumber += number;  
	m_RTOperateList.push_back(pData);
	pthread_mutex_unlock(&m_RTOperateListLock);

	return 0;
}





int CWorkerProcee::LRModuleProcessRequest(STDataNode* &pData)
{

	STRTMsgRequest *pRTRequest = (STRTMsgRequest *)(pData->szData);
	unsigned short optype =  ntohs(pRTRequest->body.optype);

	//unsigned short optype = ntohs(*((unsigned short *)(pData->szData + sizeof(STMsgHeader))));
	//XINFO("LRModule  Process Recv Realtime Request OPTYE:%d\n",optype);
	XINFO("LRModule  Process Recv Realtime Request: requestid:%s, groupid:%d,clientid:%d, OPTYE:%d, symbol number:%d, symbol list:%s\n", 
			pRTRequest->header.requestid, ntohl(pRTRequest->body.groupid),ntohl(pRTRequest->body.clientid),
			optype, ntohs(pRTRequest->body.msgnumber) ,pRTRequest->body.coinarray);

	if(0 == m_WorkerList.size())
	{
		XERROR("LRModuleProcessRequest no worker module, it  is a serious mistake  !!!!!");
		exit(-1);
	}

	if(RT_MSG_ADD != optype)
	{
		list<CWorkerProcee *>::iterator itr = m_WorkerList.begin();
		for(;itr !=m_WorkerList.end(); itr ++  )
		{
			CWorkerProcee *p = (*itr);

			STDataNode *pDataNode = CMemPoolMgr::GetInstance()->GetDataNode();
			if( NULL == pDataNode )
			{
				XERROR("GetDataNode mem node is zero , close client socket:%d\n",pData->objConn.socket); 
				CMemPoolMgr::GetInstance()->PushDataNode(pData);
				continue;
			}
			*pDataNode = *pData	;
			p->DeleteOrUpdateorAdd(pDataNode,0);
		}
		
		CMemPoolMgr::GetInstance()->PushDataNode(pData);
		return 0;
	}
	
	

	int clientnumber = 0;
	int i = 0;
	list<CWorkerProcee *>::iterator itr_tmp ;
	
	list<CWorkerProcee *>::iterator itr = m_WorkerList.begin();
	CWorkerProcee *pFind = (*itr);
	clientnumber = pFind->GetClientNumber();
	
	for(;itr !=m_WorkerList.end(); itr ++ , i++ )
	{
		CWorkerProcee *p = (*itr);
		if(clientnumber <=  p->GetClientNumber()) continue;
		
		clientnumber = p->GetClientNumber();
		pFind = (*itr);  
	}

	
	pFind->DeleteOrUpdateorAdd(pData, 1);

	return 0;

}




int CWorkerProcee::ProcessRTDelUpdateAddRequest(STDataNode *pData,STMsgRequestAck &ack)
{

	XINFO("Process RT Del Update Add Request:groupid:%d,clientid:%d,socket:%d,requestid:%s\n",
		pData->pRequest->body.groupid , pData->pRequest->body.clientid,pData->objConn.socket,pData->pRequest->header.requestid);
	
	map<unsigned long,STDataNode *>::iterator itr ;
	itr = m_RTWorkerList.find(pData->pRequest->body.groupid + pData->pRequest->body.clientid + pData->objConn.socket);
	if(itr != m_RTWorkerList.end())
	{
		STDataNode *pInData = (STDataNode *)(itr->second);
		m_RTWorkerList.erase(itr);
		CMemPoolMgr::GetInstance()->PushDataNode(pInData); 
	}


	if(itr != m_RTWorkerList.end() &&  RT_MSG_DELETE ==  pData->pRequest->body.optype  )
	{
		//SendToClient(pData,&ack,sizeof(ack));
	}
	
	if( RT_MSG_DELETE ==  pData->pRequest->body.optype	 )
	{
		CMemPoolMgr::GetInstance()->PushDataNode(pData); 
		return 0;
	}
	
	if(itr != m_RTWorkerList.end() &&  RT_MSG_UPDATE ==  pData->pRequest->body.optype )
	{
		//SendToClient(pData,&ack,sizeof(ack));
		m_RTWorkerList.insert(pair<unsigned long,STDataNode *>(pData->pRequest->body.groupid + pData->pRequest->body.clientid,pData));
	}
	if( itr == m_RTWorkerList.end() && RT_MSG_UPDATE ==  pData->pRequest->body.optype )
	{
		CMemPoolMgr::GetInstance()->PushDataNode(pData); 
		return 0;
	}
	


	if( RT_MSG_ADD ==  pData->pRequest->body.optype)
	{
		//SendToClient(pData,&ack,sizeof(ack));
		m_RTWorkerList.insert(pair<unsigned long,STDataNode *>(pData->pRequest->body.groupid + pData->pRequest->body.clientid,pData));
		
	}
	
	return 0;




	
}




int CWorkerProcee::ParseCoinName(STDataNode *pData)
{
	
	
	char *buf = pData->pRequest->body.coinarray;
 	char *pNew = NULL;
	char *pCoin = NULL;
	while((pCoin = strtok_r(buf, ";", &pNew)) != NULL)
	{
		STCoinPairInfo coininfo;
		pData->coinlist.insert(pair<string,STCoinPairInfo>(pCoin,coininfo));
		buf = NULL;
	}


	return 0;
}





int CWorkerProcee::ParseRTRequest(STDataNode *pData,STMsgRequestAck &ack)
{
	unsigned int msmlen = sizeof(pData->szData);
	if(pData->nMsgLen > msmlen)
	{
		ack.errNo = htonl(ERRCODE_PARSEERR);
		XERROR("ProcessRTRequest  fail, srv self msglen:%d,client send msglen :%d,out client ip:%s,out client port:%d,requestid:%s\n",
				pData->nMsgLen,msmlen,iptostrip(htonl(pData->objConn.ip)), pData->objConn.port,pData->pRequest->header.requestid);
		return -1;

	}

	pData->pRequest = (STRTMsgRequest *)pData->szData;
	pData->pRequest->header.starter = ntohl(pData->pRequest->header.starter);
	pData->pRequest->header.msglen  = ntohs(pData->pRequest->header.msglen);
	pData->pRequest->header.version = ntohs(pData->pRequest->header.version);
	pData->pRequest->header.msgid   = ntohl(pData->pRequest->header.msgid);

	pData->pRequest->body.optype 	= ntohs(pData->pRequest->body.optype);
	pData->pRequest->body.msgnumber = ntohs(pData->pRequest->body.msgnumber);
	pData->pRequest->body.groupid	= ntohl(pData->pRequest->body.groupid);
	pData->pRequest->body.clientid 	= ntohl(pData->pRequest->body.clientid);

	ack.groupid = htonl(pData->pRequest->body.groupid);
	ack.clientid =  htonl(pData->pRequest->body.clientid);


	SetMsgHeader(ack.header,sizeof(ack),SRVVERSION,RTMSG_REQUEST_ACK,pData->pRequest->header.requestid);

	if( STARTER != pData->pRequest->header.starter)
	{
		ack.errNo = htonl(ERRCODE_PARSEERR);
		XERROR("ProcessRTRequest  fail, srv self startchar:0x%04x,client send startchar :0x%04x,out client ip:%s,out client port:%d,requestid:%s\n",
				STARTER,pData->pRequest->header.starter,iptostrip(htonl(pData->objConn.ip)), pData->objConn.port,pData->pRequest->header.requestid);
		return -1;
	}

	if( SRVVERSION != pData->pRequest->header.version)
	{
		ack.errNo = htonl(ERRCODE_PARSEERR);
		XERROR("ProcessRTRequest  fail, srv self version:%d,client send version :%d,out client ip:%s,out client port:%d,requestid:%s\n",
				SRVVERSION,pData->pRequest->header.version,iptostrip(htonl(pData->objConn.ip)), pData->objConn.port,pData->pRequest->header.requestid);
		return -1;
	}

	if( RTMSG_REQUEST != pData->pRequest->header.msgid)
	{
		XERROR("ProcessRTRequest fail, srv self msgid:%d,client send msgid :%d,out client ip:%s,out client port:%d,requestid:%s\n",
				RTMSG_REQUEST,pData->pRequest->header.msgid,iptostrip(htonl(pData->objConn.ip)), pData->objConn.port,pData->pRequest->header.requestid);
		ack.errNo = htonl(ERRCODE_PARSEERR);
		return -1;
	}


	unsigned short optype = pData->pRequest->body.optype ;
	if( optype != RT_MSG_DELETE &&  optype != RT_MSG_ADD && optype != RT_MSG_UPDATE )
	{
		XERROR("ProcessRTRequest fail, client send body optype :%d error,out client ip:%s,out client port:%d,requestid:%s\n",
				optype,iptostrip(htonl(pData->objConn.ip)), pData->objConn.port,pData->pRequest->header.requestid);
		ack.errNo = htonl(ERRCODE_PARSEERR);
		return -1;
	}

	if(optype == RT_MSG_DELETE)
	{
		ack.errNo = htonl(ERRCODE_OK);
		return 0;
	}


	unsigned short msgnumber =  pData->pRequest->body.msgnumber; 
	if( 0 >= msgnumber || msgnumber > COINNUMBER )
	{
		XERROR("ProcessRTRequest fail, client send msgnumber :%d,out client ip:%s,out client port:%d,requestid:%s\n",
				pData->pRequest->body.msgnumber,iptostrip(htonl(pData->objConn.ip)), pData->objConn.port,pData->pRequest->header.requestid);
		ack.errNo = htonl(ERRCODE_PARSEERR);
		return -1;
	}

	

	int msgdatalen = pData->pRequest->header.msglen - sizeof(pData->pRequest->header)-12;
	int recvdatalen = strlen( pData->pRequest->body.coinarray);
	if( msgdatalen != recvdatalen )
	{
		XERROR("ProcessRTRequest fail, client send body coinarray :[%s] recvdatalen:%d != msgdatalen:%d,out client ip:%s,out client port:%d,requestid:%s\n",
				pData->pRequest->body.coinarray,recvdatalen, msgdatalen ,iptostrip(htonl(pData->objConn.ip)), pData->objConn.port,pData->pRequest->header.requestid);
		ack.errNo = htonl(ERRCODE_PARSEERR);
		return -1;
	}

	ack.errNo = htonl(ERRCODE_OK);

	ParseCoinName(pData);
	
	return 0;
	
}





int	CWorkerProcee::ProcessDeleteOrUpdateOrAdd()
{

	int ret = -1;
	do
	{
		if(0 == m_RTOperateList.size()) return ERRCODE_OK;

		STDataNode *pData = NULL;
		
		pthread_mutex_lock(&m_RTOperateListLock);
		pData = m_RTOperateList.front();
		m_RTOperateList.pop_front();
		pthread_mutex_unlock(&m_RTOperateListLock);


		STMsgRequestAck ack;

		ret =ParseRTRequest(pData,ack);
		if( 0 == ret )
		{	
			ProcessRTDelUpdateAddRequest(pData,ack); 
		}
		else
		{
			//SendToClient(pData,&ack,sizeof(ack));
			CMemPoolMgr::GetInstance()->PushDataNode(pData); 
		}
		
	}while(1);

	return 0;
	
}








static size_t EncodeRealtimeHdr(realtime::MessageHeader &hdr, char *buffer, std::uint64_t offset, 
std::uint64_t bufferLength)
{
    // encode the header
    hdr.wrap(buffer, offset, messageHeaderVersion, bufferLength)
       .blockLength(realtime::Realtimedatas::sbeBlockLength())
       .templateId(realtime::Realtimedatas::sbeTemplateId())
       .schemaId(realtime::Realtimedatas::sbeSchemaId())
       .version(realtime::Realtimedatas::sbeSchemaVersion());


    return hdr.encodedLength();
}




static std::size_t EncodeRealtimeBody(realtime::Realtimedatas &realtime, list<STRealtimeData> &_realtimelist , char *buffer, std::uint64_t offset, std::uint64_t bufferLength)
{
    realtime.wrapForEncode(buffer, offset, bufferLength) ;

    realtime::Realtimedatas::RealtimeDataList &st_realtime = realtime.realtimeDataListCount(_realtimelist.size());

    list<STRealtimeData>::iterator itr;
    for(itr=_realtimelist.begin();itr!=_realtimelist.end();itr++)
    {
        STRealtimeData *p = (STRealtimeData *)(&(*itr));
        st_realtime.next().status(p->status).bid(p->bid).ask(p->ask).timestamp(p->timestamp).putInstrument(p->instrument);
    }

    return realtime.encodedLength();
}





static size_t EncodeRealtimeData(list<STRealtimeData> &realtimelist , char *buffer, size_t bufferlen )
{
	realtime::MessageHeader hdr;
	realtime::Realtimedatas realtime;
	
	std::size_t encodeHdrLength = EncodeRealtimeHdr(hdr,  buffer, 0, bufferlen);
	
	std::size_t encodeMsgLength = EncodeRealtimeBody(realtime,realtimelist, buffer, hdr.encodedLength(), bufferlen);

	return  encodeHdrLength + encodeMsgLength ;
}





int CWorkerProcee::GetMarketPrice(STDataNode *pData ,STRTMsgToClient &rtMsgToClient,STRTMsgToClient &rtMsgToClient_bo)
{
	map<string, STCoinPairInfo>::iterator itr ;
	if(NULL==pData->pRequest)return -1;
	
	rtMsgToClient.groupid = htonl(pData->pRequest->body.groupid);
	rtMsgToClient.clientid = htonl(pData->pRequest->body.clientid);

	rtMsgToClient_bo.groupid = htonl(pData->pRequest->body.groupid);
	rtMsgToClient_bo.clientid = htonl(pData->pRequest->body.clientid);
	

	int coinnumber = 0;
	int coinnumber_bo = 0;
	char askbid[128];
	char symbol[8]={0};

	int bid,ask;
	unsigned long int timestamp ;

	
	
	list<STRealtimeData> realtimelist;
	list<STRealtimeData> realtimelist_bo;
	
    for (itr=pData->coinlist.begin(); itr!=pData->coinlist.end(); )
    {
		const char *p = itr->first.c_str();
		
		int isBo = 0,ret = 0;;
		if( 'b' == p[6])
		{
			memset(symbol,0,8);
			memcpy(symbol,itr->first.c_str(),8);
			symbol[6]='\0';
			symbol[7]='\0';
			isBo = 1;
			ret = marketAgent.GetMarketData(symbol, &bid,&ask,&timestamp) ;
		}
		else
		{
			ret = marketAgent.GetMarketData(itr->first, &bid,&ask,&timestamp);
		}

		if( UNIVERSAL_MARKET_SUCCESS != ret )
		{
			XERROR("GetMarketPrice GetMarketPrice Coinname:[%s] is invalid, requestid:%s, remove it \n",(itr->first).c_str() ,pData->pRequest->header.requestid);
			pData->coinlist.erase(itr++);
			continue;
		}

		
		STCoinPairInfo *pCoinInfo = (STCoinPairInfo *)&(itr->second) ;
		
		if(pCoinInfo->bid == bid && pCoinInfo->ask == ask && pCoinInfo->timestamp == timestamp)
		{
			//XERROR("GetMarketPrice GetMarketPrice price equals last price:%s,don't send  \n",(itr->first).c_str());
			itr++ ;
			continue;
		}
		 
		
		pCoinInfo->bid = bid;
		pCoinInfo->ask = ask;
		pCoinInfo->timestamp = timestamp ;

		
		// bo
		if(1 == isBo)
		{
			int caltype = -1;
			int ret = m_pGroupSpread->GetValueFromGroupid(0, pData->pRequest->body.groupid, pData->pRequest->header.requestid,(itr->first).c_str(),caltype) ;
			if( 0 == ret)
			{
				int ask2 = ask,bid2=bid;
				m_pGroupSpread->CalBo_AskBid(caltype,ask2,bid2 );
				STRealtimeData realtimeData_bo(1,bid2,ask2,0,timestamp,(itr->first).c_str());
				realtimeData_bo.instrument[6]='b';
				realtimeData_bo.instrument[7]='o';
				realtimelist_bo.push_back(realtimeData_bo);
				coinnumber_bo ++;
			}
		}

		// fx
		if( 0 == isBo)
		{
			int groupspread = -1;
			ret = m_pGroupSpread->GetValueFromGroupid(1, pData->pRequest->body.groupid, pData->pRequest->header.requestid,(itr->first).c_str(),groupspread) ;
			if( 0 == ret)
			{
				
				m_pGroupSpread->CalAskBidSpread(ask,bid,groupspread);
				STRealtimeData realtimeData(1,bid,ask,groupspread,timestamp,(itr->first).c_str());
				realtimeData.instrument[6]='\0';
				realtimeData.instrument[7]='\0';
				realtimelist.push_back(realtimeData);
				coinnumber ++;
			}
		}

		itr ++;

    }

	// FX
	if(0 != coinnumber)
	{		
		rtMsgToClient.msgnumber = htons(coinnumber);
		rtMsgToClient.srv_seqid = htonl(++pData->objConn.srv_seqid );
		
		int spacelen = sizeof(rtMsgToClient.coinresponse);
		
		int contentlen =  EncodeRealtimeData(realtimelist, rtMsgToClient.coinresponse, spacelen );
		

		int sendlen = sizeof(rtMsgToClient.header)+18+contentlen;
		SetMsgHeader(rtMsgToClient.header,sendlen,SRVVERSION,RTMSG_PUSH,pData->pRequest->header.requestid);
	}

	
	// BO
	if(0 != coinnumber_bo)
	{
		rtMsgToClient_bo.msgnumber = htons(coinnumber_bo);
		rtMsgToClient_bo.srv_seqid = htonl(++pData->objConn.srv_seqid );
		
		int spacelen = sizeof(rtMsgToClient_bo.coinresponse);
		
		int contentlen =  EncodeRealtimeData(realtimelist_bo, rtMsgToClient_bo.coinresponse, spacelen );
		

		int sendlen = sizeof(rtMsgToClient_bo.header)+18+contentlen;
		SetMsgHeader(rtMsgToClient_bo.header,sendlen,SRVVERSION,RTMSG_BO_PUSH,pData->pRequest->header.requestid);
	}
	
	
	return 0;
}





void CWorkerProcee::Run()
{
	int ret = 0;
	STRTMsgToClient rtMsgToClient;
	STRTMsgToClient rtMsgToClient_bo;

	
	
	while(1)
	{
		m_pGroupSpread->ReadGroupSymbolsList();
		m_pGroupSpread->ReadGroupBoSymbolsList();
		
		ProcessDeleteOrUpdateOrAdd(); 	
		
		if(0 == m_RTWorkerList.size())
		{
			usleep(5000);
			continue;
		}

		
		map<unsigned long,STDataNode *>::iterator itr = m_RTWorkerList.begin();
		for( ; itr!=m_RTWorkerList.end(); )
		{
			ret = 0;
			
			m_pGroupSpread->ReadGroupSymbolsList();
			
			STDataNode *pData = (STDataNode *)(itr->second);
			memset(&rtMsgToClient,0,sizeof(rtMsgToClient));	
			memset(&rtMsgToClient_bo,0,sizeof(rtMsgToClient_bo));	



			if( (pData->pConn->conntime.tv_sec != pData->objConn.conntime.tv_sec) 
				&& ( pData->pConn->conntime.tv_usec != pData->objConn.conntime.tv_usec ) )
			{
				XINFO("RealtimeData socket closed,requestid:%s",pData->pRequest->header.requestid);
				CMemPoolMgr::GetInstance()->PushDataNode(pData); 
				m_nClientNumber --;
				m_RTWorkerList.erase(itr++);
				continue;
			}
		
			GetMarketPrice(pData,rtMsgToClient,rtMsgToClient_bo);
			if(0 != rtMsgToClient.header.starter)
			{
				ret = SendToClient(pData,&rtMsgToClient,ntohs(rtMsgToClient.header.msglen),pData->pRequest->header.requestid) ;
				if ( -1 == ret  || pData->objConn.sendfailtimes > SENDFAILTIMES)
				{
					XERROR("Realtime Data send error, requestid:%s \n" ,pData->pRequest->header.requestid);
					CMemPoolMgr::GetInstance()->PushDataNode(pData); 
					
					m_nClientNumber --;
					m_RTWorkerList.erase(itr++);
					continue;
				}	
			}

			if( ret != -1 && 0 != rtMsgToClient_bo.header.starter)
			{
				ret = SendToClient(pData,&rtMsgToClient_bo,ntohs(rtMsgToClient_bo.header.msglen),pData->pRequest->header.requestid) ;
				if ( -1 == ret || pData->objConn.sendfailtimes > SENDFAILTIMES)
				{
					XERROR("Realtime BoData send error, requestid:%s \n" ,pData->pRequest->header.requestid);
					CMemPoolMgr::GetInstance()->PushDataNode(pData); 
					
					m_nClientNumber --;
					m_RTWorkerList.erase(itr++);
					continue;
				}	
			}

			itr ++;
			
			
		}

		usleep(stConfinfo.send_intertime*1000);


	}

}







int InitMarketPrice()
{
	int ret = -1;

	STTnodeConfig tnodeconfig ;
	tnodeconfig.mq_vhost= stConfinfo.mq_vhost;
	tnodeconfig.mq_exchange_group = stConfinfo.mq_exchangeGroup ;
	tnodeconfig.mq_host = stConfinfo.mq_address ;
	tnodeconfig.mq_port= atoi(stConfinfo.mq_port.c_str()) ;
	tnodeconfig.mq_user = stConfinfo.mq_user ;
	tnodeconfig.mq_passwd= stConfinfo.mq_password ;


	
	TNode *pNode = new  TNode(tnodeconfig);
	if(-1 == pNode->Init() )
	{
		printf("tnode init fail , pls check ! \n");
		return -1;
	}
	
	ret = marketAgent.Init(*pNode, stConfinfo.mq_queueid);
	
	return ret;
}



void CWorkerProcee::WorkerBatchStart()
{
	printf("InitMarketPrice ....\n");
	if(0 != InitMarketPrice())
	{
		XERROR("Init MarketPrice fail.\n");
		exit(-1); ;
	}

    int i = 0;
    for(i = 0; i < stConfinfo.worker_count; i++ )
    {
        CWorkerProcee *pWorker = new CWorkerProcee();
        pWorker->AddWorkerToList();
        pWorker->Start();
    }

	XINFO("worker %d module run ...\n", stConfinfo.worker_count);
}

