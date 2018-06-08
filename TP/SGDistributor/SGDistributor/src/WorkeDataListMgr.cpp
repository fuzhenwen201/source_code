

#include "WorkeDataListMgr.h"


CWorkDataListMgr*  CWorkDataListMgr::m_pInstance = NULL;


CWorkDataListMgr::CWorkDataListMgr()
{
	m_pWWorkDataList = &m_WorkDataList1;
	m_pRWorkDataList = &m_WorkDataList2;

	pthread_mutex_init(&m_wListLock,NULL);
	pthread_mutex_init(&m_rListLock,NULL);

}




CWorkDataListMgr *CWorkDataListMgr::GetInstance()
{
	if(NULL != m_pInstance) return m_pInstance;

	m_pInstance = new CWorkDataListMgr();
	assert(m_pInstance!=NULL);

	return m_pInstance;	
}



STDataNode *CWorkDataListMgr::GetDataNode()
{
	pthread_mutex_lock(&m_rListLock);
	{
	
		do
		{
			if(0 < m_pRWorkDataList->size())
			{
				STDataNode *pNode = m_pRWorkDataList->front();
				m_pRWorkDataList->pop_front();
				pthread_mutex_unlock(&m_rListLock);
				return pNode;
			}

			pthread_mutex_lock(&m_wListLock);
			{
				if(0 == m_pWWorkDataList->size())
				{
					pthread_mutex_unlock(&m_wListLock);
					pthread_mutex_unlock(&m_rListLock);
					//XINFO("CWorkDataListMgr GetDataNode: list is empty ! \n");
					return NULL;
				}
				
				list<STDataNode *>	*p = m_pRWorkDataList;
				m_pRWorkDataList = m_pWWorkDataList;
				m_pWWorkDataList = p;
			
			}
			pthread_mutex_unlock(&m_wListLock);
			
			continue;
			
		}while(1);
	}
	pthread_mutex_unlock(&m_rListLock);

}



void	CWorkDataListMgr::PushDataNode(STDataNode *pNode)
{
	pthread_mutex_lock(&m_wListLock);
	{
		m_pWWorkDataList->push_back(pNode);
	}
	pthread_mutex_unlock(&m_wListLock);

}


