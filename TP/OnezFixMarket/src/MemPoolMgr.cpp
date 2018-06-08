
#include "MemPoolMgr.h"
#include "Public.h"





CMemPoolMgr * CMemPoolMgr::m_pInstance = NULL;



CMemPoolMgr::CMemPoolMgr()
{
	pthread_mutex_init(&m_DataLock,NULL);
	
	m_pInstance = NULL;
}



int CMemPoolMgr::Init()
{

	int i = 0;
	for(i=0;i<stServiceConfig.nMempoolNodeCount;i++)
	{
	
		STQuoteRequest44 *p = new STQuoteRequest44();
		assert(p!=NULL);
		memset(p,0,sizeof(STQuoteRequest44));
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



STQuoteRequest44 *	CMemPoolMgr::GetDataNode()
{
	STQuoteRequest44 *pNode = NULL; 
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
	

	return pNode;
	
}



void CMemPoolMgr::PushDataNode(STQuoteRequest44 *pNode)
{
	assert(pNode!=NULL);

	memset(pNode,0,sizeof(STQuoteRequest44));
	pthread_mutex_lock(&m_DataLock);
	{
		m_DataMemPool.push_back(pNode);
	}
	pthread_mutex_unlock(&m_DataLock);
}



