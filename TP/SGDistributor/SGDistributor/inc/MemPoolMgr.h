/*******************************************************************************************

*Department:	TP

*Decrible:  	数据内存池
				1. 先采用固态内存池,后期可以扩展为动态内存池
				
*Auhor:			Savin

*Createdtime:	2017-04-19

*ModifyTime:	


********************************************************************************************/


#ifndef DATAPOOL_HEAD_H
#define DATAPOOL_HEAD_H

#include "Public.h"

#include <pthread.h>
#include <list>
#include <stdlib.h>
#include <string.h>


using namespace std;





class CMemPoolMgr
{
public:
	CMemPoolMgr();
	~CMemPoolMgr();
	static CMemPoolMgr *GetInstance();
	int Init();
	void SetInstance() ;
	STDataNode *			GetDataNode();
	void 					PushDataNode(STDataNode *pNode);

	STConnNode *			GetConnNode();
	void 					PushConnNode(STConnNode *pNode);
	
private:


	list<STDataNode *>		m_DataMemPool;
	pthread_mutex_t			m_DataLock;

	list<STConnNode *>		m_ConnMemPool;
	pthread_mutex_t			m_ConnLock;
	
	static CMemPoolMgr *m_pInstance;

};



#endif // DATAPOOL_HEAD_H




