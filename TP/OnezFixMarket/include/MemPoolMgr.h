/*******************************************************************************************

*Department:	TP

*Decrible:  	数据内存池
				
				


*Auhor:			Savin

*Createdtime:	2017-05-08

*ModifyTime:	


********************************************************************************************/


#ifndef DATAPOOL_HEAD_H
#define DATAPOOL_HEAD_H


#include <pthread.h>
#include <list>
#include <stdlib.h>
#include <string.h>

#include "quickfix/STQuoteRequest44.h"


using namespace std;





class CMemPoolMgr
{
public:
	CMemPoolMgr();
	~CMemPoolMgr();
	static CMemPoolMgr *	GetInstance();
	int 					Init();
	void 					SetInstance() ;
	STQuoteRequest44 *		GetDataNode();
	void 					PushDataNode(STQuoteRequest44 *pNode);

	
	
private:


	list<STQuoteRequest44 *>m_DataMemPool;
	pthread_mutex_t			m_DataLock;

	
	static CMemPoolMgr *	m_pInstance;

};



#endif // DATAPOOL_HEAD_H




