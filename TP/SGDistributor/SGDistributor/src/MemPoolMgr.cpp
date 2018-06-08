
#include "MemPoolMgr.h"




CMemPoolMgr * CMemPoolMgr::m_pInstance = NULL;



CMemPoolMgr::CMemPoolMgr()
{
	pthread_mutex_init(&m_DataLock,NULL);
	pthread_mutex_init(&m_ConnLock,NULL);
	
	m_pInstance = NULL;
}



int CMemPoolMgr::Init()
{

	int i = 0;
	for(i=0;i<stConfinfo.nMemNodeCount;i++)
	{
		STConnNode *pConn = new STConnNode();
		if(NULL == pConn)return -1;
		pConn->Clean();
		pConn->pClientEvent = (struct event *)malloc(sizeof(struct event));
		if(NULL == pConn->pClientEvent)return -1;
		m_ConnMemPool.push_back(pConn);
	
		STDataNode *p = new STDataNode();
		if(NULL == p)return -1;
		p->Clean();
		m_DataMemPool.push_back(p);
	}

	SetInstance();

	return 0;
}



void CMemPoolMgr::SetInstance()
{	
	m_pInstance = this;
}



CMemPoolMgr *CMemPoolMgr::GetInstance()
{
	if(NULL != m_pInstance) 
		return m_pInstance;
	else
		return NULL;
}



STDataNode *	CMemPoolMgr::GetDataNode()
{
	STDataNode *pNode = NULL; 
	pthread_mutex_lock(&m_DataLock);
	{
		if(0 == m_DataMemPool.size())
		{
			pthread_mutex_unlock(&m_DataLock);
			return NULL;
		}

		
		pNode = m_DataMemPool.front();
		m_DataMemPool.pop_front();
	}
	pthread_mutex_unlock(&m_DataLock);
	
	pNode->Clean();
	return pNode;
	
}



void CMemPoolMgr::PushDataNode(STDataNode *pNode)
{
	assert(pNode!=NULL);
	
	pthread_mutex_lock(&m_DataLock);
	{
		pNode->Clean();
		m_DataMemPool.push_back(pNode);
	}
	pthread_mutex_unlock(&m_DataLock);
}





STConnNode *	CMemPoolMgr::GetConnNode()
{
	STConnNode *pNode= NULL;
	pthread_mutex_lock(&m_ConnLock);
	{
		if(0 == m_ConnMemPool.size())
		{
			pthread_mutex_unlock(&m_ConnLock);
			return NULL;
		}

		
		pNode = m_ConnMemPool.front();
		m_ConnMemPool.pop_front();
		
	}
	pthread_mutex_unlock(&m_ConnLock);

	pNode->Clean();
	return pNode;
}



void CMemPoolMgr::PushConnNode(STConnNode *pNode)
{
	assert(pNode!=NULL);
	
	pthread_mutex_lock(&m_ConnLock);
	{
		pNode->Clean();
		memset(pNode->pClientEvent,0,sizeof(struct event));
		m_ConnMemPool.push_back(pNode);
		
	}
	pthread_mutex_unlock(&m_ConnLock);


}





