
#include <endian.h>
#include <arpa/inet.h>

#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include "RealtimeSender.h"
#include "xlogger.h"
#include "Public.h"
#include "MemPoolMgr.h"
#include "tnodeconfig_v0.h"
#include <list>





using namespace std;

list<STQuoteRequest44 *>	CRealtimeSender::m_RealtimeDataList1;
list<STQuoteRequest44 *>*	CRealtimeSender::m_pRealtimeDataList_w;
pthread_mutex_t				CRealtimeSender::m_wListLock;

list<STQuoteRequest44 *>	CRealtimeSender::m_RealtimeDataList2;
list<STQuoteRequest44 *>*	CRealtimeSender::m_pRealtimeDataList_r;
pthread_mutex_t				CRealtimeSender::m_rListLock;


CRealtimeSender::CRealtimeSender()
{
	m_pGroupSpread  = new CGroupSpread;

	//CGroupSpread spreadlist;
	//spreadlist.ReadSymbolSpreadList();
}



void CRealtimeSender::Run() 
{

	STTnodeConfig tnodeconfig ;
	tnodeconfig.mq_vhost= stServiceConfig.mq_vhost ;
	tnodeconfig.mq_exchange_group = stServiceConfig.mq_exchangeGroup ;
	tnodeconfig.mq_host = stServiceConfig.mq_address ;
	tnodeconfig.mq_port= atoi( (stServiceConfig.mq_port).c_str() );
	tnodeconfig.mq_user = stServiceConfig.mq_user ;
	tnodeconfig.mq_passwd= stServiceConfig.mq_password;



	TNode tnode(tnodeconfig);
	if(-1 == tnode.Init() )
	{
		XERROR("tnode init fail , pls check ! \n");
		return ;
	}


	int data_buffer_len = sizeof(CurrencySymbol) + 1;
	CurrencySymbol *data_buffer;
	data_buffer = (CurrencySymbol*)tnode.AllocDataBuff(data_buffer_len);
    if (NULL == data_buffer)
	{
        XERROR("tnode alloc data buffer failed\n");
        return ;
	}
	

	while(1)
	{
		m_pGroupSpread->ReadSymbolSpreadList();
	
		STQuoteRequest44 *pData  = 	CRealtimeSender::GetRealtimeData();
		if(NULL == pData)
		{
			usleep(3000);
			continue;
		}

		memset(data_buffer,0,data_buffer_len);

		//CurrencySymbol symbol ;
	
		MarketMap::iterator  itr;
	    itr = CoinPairMap.find(pData->Symbol);
	    if (CoinPairMap.end() == itr)
		{
	        XERROR("CRealtimeSender Not find symbol:%s\n", pData->Symbol);
	        CMemPoolMgr::GetInstance()->PushDataNode(pData);
			continue;
	    }
	
		uint64_t sendtime = GetTimestamp(pData->SendingTime);


	    data_buffer->instrument = htobe32(itr->second);
		data_buffer->status = htobe32(1);

		int value = (int) (atof(pData->BidPx) * 1000000);
		data_buffer->bid = ((uint32_t)value);
		value = (int) (atof(pData->OfferPx) * 1000000);
		data_buffer->ask =  ((uint32_t)value);
		data_buffer->timestamp  =  htobe64(sendtime);
		
		//pData->Symbol[3]='_' ;

		//printf("string :: bid:%s, ask:%s ; int:: bid:%d, ask:%d \n",pData->BidPx,pData->OfferPx,(data_buffer->bid),(data_buffer->ask));		

		
		int spread = 0;
		//m_pGroupSpread->GetSymbolSpread(pData->Symbol,spread);
		//m_pGroupSpread->CalAskBidSpread(data_buffer->ask,data_buffer->bid,spread);

		

		long int differencetime = pData->RecvTime-sendtime ;
		/*
		XINFO("PushMQ coinpair:%s, spread:%d, bid:%d, ask:%d,  sendtime:%lu,recvtime:%lu,Time difference:%ld\n"
			,pData->Symbol, spread ,(data_buffer->bid),(data_buffer->ask),sendtime,pData->RecvTime,differencetime );
		*/
		
		if(differencetime > stServiceConfig.nDelayMicrosecond)
		{
			XERROR("coinpair:%s,the delay time:%ld that fortex to fix_se for more than %d Milliseconds",pData->Symbol,differencetime, stServiceConfig.nDelayMicrosecond);
		}

		data_buffer->bid = htobe32(data_buffer->bid);
		data_buffer->ask = htobe32(data_buffer->ask);
		//channel.PublishGroup(pData->Symbol, event,(char*)data_buffer, sizeof(CurrencySymbol), 0);

		char routingkey[64]={0};
		snprintf(routingkey,63,"realtime_onez.%s",pData->Symbol);
		tnode.PublishToMQ( routingkey, (char*)data_buffer,sizeof(CurrencySymbol), 0, "requestid_122334545","sessionid_123456789" ) ;

		char routingkey_v03[64]={0};
		snprintf(routingkey_v03,63,"realtime_v03.%s.$",pData->Symbol);
		tnode.PublishToMQ_v03( routingkey_v03, (char*)data_buffer,sizeof(CurrencySymbol), 0, "requestid_122334545","sessionid_123456789" ) ;
		
		/*
		XINFO("Push to MQ: routingkey:%s, coinpair:%s, spread:%d, bid:%d, ask:%d,  sendtime:%lu,recvtime:%lu,Time difference:%ld\n",
			routingkey ,pData->Symbol, spread ,ntohl(data_buffer->bid),ntohl(data_buffer->ask),sendtime,pData->RecvTime,differencetime );
		*/
		
		CMemPoolMgr::GetInstance()->PushDataNode(pData);
		
	}

	tnode.FreeDataBuff((char *)data_buffer);
	
}
   

void 	CRealtimeSender::StartBatchSender()
{
	m_pRealtimeDataList_w = &m_RealtimeDataList1;
	m_pRealtimeDataList_r = &m_RealtimeDataList2;

	pthread_mutex_init(&m_wListLock,NULL);
	pthread_mutex_init(&m_rListLock,NULL);

	int i = 0;
    for(i = 0; i < stServiceConfig.nRealtimeSenderCount; i++ )
    {
        CRealtimeSender *pWorker = new CRealtimeSender();
        pWorker->Start();
    }

	

	XINFO("sender %d module run ...\n",stServiceConfig.nRealtimeSenderCount);

}


void 	CRealtimeSender::PushRealtimeData(STQuoteRequest44 *pData)
{
	assert(pData!=NULL);

	pthread_mutex_lock(&m_wListLock);
	m_pRealtimeDataList_w->push_back(pData);	
	pthread_mutex_unlock(&m_wListLock);	
}



STQuoteRequest44 * 	CRealtimeSender::GetRealtimeData()
{
	pthread_mutex_lock(&m_rListLock);
	{
	
		do
		{
			if(0 < m_pRealtimeDataList_r->size())
			{
				STQuoteRequest44 *pNode = m_pRealtimeDataList_r->front();
				m_pRealtimeDataList_r->pop_front();
				pthread_mutex_unlock(&m_rListLock);
				return pNode;
			}

			pthread_mutex_lock(&m_wListLock);
			{
				if(0 == m_pRealtimeDataList_w->size())
				{
					pthread_mutex_unlock(&m_wListLock);
					pthread_mutex_unlock(&m_rListLock);
					//XINFO("CWorkDataListMgr GetDataNode: list is empty ! \n");
					return NULL;
				}
				
				list<STQuoteRequest44 *>	*p = m_pRealtimeDataList_r;
				m_pRealtimeDataList_r = m_pRealtimeDataList_w;
				m_pRealtimeDataList_w = p;
			
			}
			pthread_mutex_unlock(&m_wListLock);
			
			continue;
			
		}while(1);
	}
	pthread_mutex_unlock(&m_rListLock);

}


