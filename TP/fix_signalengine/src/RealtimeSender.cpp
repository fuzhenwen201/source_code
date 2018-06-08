
#include <endian.h>

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



void CRealtimeSender::Run() 
{

	snetwork_xservice_tnode::TNodeConfigV0 cfg;
		cfg
		.VHost(stServiceConfig.mq_vhost)
		.Exchange(stServiceConfig.mq_exchange)
		.ExchangeGroup(stServiceConfig.mq_exchangeGroup)
		.Address(stServiceConfig.mq_address)
		.Port(stServiceConfig.mq_port)
		.User(stServiceConfig.mq_user)
		.Password(stServiceConfig.mq_password);
	
	tnode_.Config(cfg);
    tnode_.Bind(UNIVERSAL_MARKET);
    //tnode_.Bind(1);
    tnode_.Run();
	sleep(5);	
	string event = "$";

	while(1)
	{
		STQuoteRequest44 *pData  = 	CRealtimeSender::GetRealtimeData();
		if(NULL == pData)
		{
			sleep(1);
			continue;
		}

		snetwork_xservice_tnode::TNodeChannel channel = tnode_.GetChannel();

		 CurrencySymbol *data_buffer;
		 data_buffer = (CurrencySymbol*)channel.AllocDataBuff(sizeof(CurrencySymbol));
         if (nullptr == data_buffer)
		 {
		 		CMemPoolMgr::GetInstance()->PushDataNode(pData);
                XERROR("tnode alloc data buffer failed\n");
                continue;
         }


		

		CurrencySymbol symbol ;
	
		MarketMap::iterator  itr;
	    itr = CoinPairMap.find(pData->Symbol);
	    if (CoinPairMap.end() == itr)
		{
	        XERROR("CRealtimeSender Not find symbol:%s\n", pData->Symbol);
	        CMemPoolMgr::GetInstance()->PushDataNode(pData);
			continue;
	    }
	


	    data_buffer->instrument = htobe32(itr->second);
		data_buffer->status = htobe32(1);

		int value = (int) (atof(pData->BidPx) * 1000000);
		data_buffer->bid = htobe32((uint32_t)value);
		value = (int) (atof(pData->OfferPx) * 1000000);
		data_buffer->ask =  htobe32((uint32_t)value);
		data_buffer->timestamp  =  htobe64(GetTimestamp(pData->SendingTime));
		
		pData->Symbol[3]='_' ;
				

		XINFO("PushMQ:{\"instrument\":\"%s\",\"status\":1,\"bid\":%s,\"ask\":%s,\"timestamp\":%s}\n",pData->Symbol,pData->BidPx,pData->OfferPx,pData->SendingTime);	

		channel.PublishGroup(pData->Symbol, event,(char*)data_buffer, sizeof(CurrencySymbol), 0);
		
	}
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


