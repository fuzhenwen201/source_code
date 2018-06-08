
#include <endian.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include "ReadDataProcessor.h"


#include <list>

using namespace std;

list<STTnodeReadData *>		CReadDataProcessor::m_RealtimeDataList1;
list<STTnodeReadData *>*	CReadDataProcessor::m_pRealtimeDataList_w;
pthread_mutex_t				CReadDataProcessor::m_wListLock;

list<STTnodeReadData *>		CReadDataProcessor::m_RealtimeDataList2;
list<STTnodeReadData *>*	CReadDataProcessor::m_pRealtimeDataList_r;
pthread_mutex_t				CReadDataProcessor::m_rListLock;




int g_nReadDataProcessor = 2;


void CReadDataProcessor::Run() 
{

	while(1)
	{
		STTnodeReadData *pData  = 	CReadDataProcessor::GetReadData();
		if(NULL == pData)
		{
			sleep(1);
			continue;
		}

		pData->header.pConsumer->EventHandle(pData->header.nTag, pData->pData, pData->header.nDataLen);
		if(pData->pData!=NULL)free(pData->pData);
		free(pData);
	}
}
   

void 	CReadDataProcessor::StartBatchSender()
{
	m_pRealtimeDataList_w = &m_RealtimeDataList1;
	m_pRealtimeDataList_r = &m_RealtimeDataList2;

	pthread_mutex_init(&m_wListLock,NULL);
	pthread_mutex_init(&m_rListLock,NULL);

	int i = 0;
    for(i = 0; i < g_nReadDataProcessor; i++ )
    {
        CReadDataProcessor *pWorker = new CReadDataProcessor();
        pWorker->Start();
    }

}


void    CReadDataProcessor::PushReadData(uint16 tag, char *event, uint32 eventlen,TNodeConsumer *consumer)
{
        STTnodeReadData *p = (STTnodeReadData *)malloc(sizeof(STTnodeReadData));
        assert(p!=NULL);

        p->pData = (char *)malloc(eventlen+1);
        assert(p->pData!=NULL);

        p->header.nTag = tag;
        p->header.nDataLen = eventlen;
        p->header.pConsumer = consumer;
        
        memcpy(p->pData,event,eventlen);
        (p->pData)[eventlen]='\0';


        pthread_mutex_lock(&m_wListLock);
        m_pRealtimeDataList_w->push_back(p);
		
        pthread_mutex_unlock(&m_wListLock);

}



STTnodeReadData * 	CReadDataProcessor::GetReadData()
{
	pthread_mutex_lock(&m_rListLock);
	{
	
		do
		{
			if(0 < m_pRealtimeDataList_r->size())
			{
				STTnodeReadData *pNode = m_pRealtimeDataList_r->front();
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
					return NULL;
				}
				
				list<STTnodeReadData *>	*p = m_pRealtimeDataList_r;
				m_pRealtimeDataList_r = m_pRealtimeDataList_w;
				m_pRealtimeDataList_w = p;
			
			}
			pthread_mutex_unlock(&m_wListLock);
			
			continue;
			
		}while(1);
	}
	pthread_mutex_unlock(&m_rListLock);

}


