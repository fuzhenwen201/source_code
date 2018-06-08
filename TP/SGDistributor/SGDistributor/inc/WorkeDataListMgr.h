
/*******************************************************************************************

*Department:	TP

*Decrible:  	数据工作队列管理类
				1. 读模块 将数据节点放入到此模块中
				2. worker模块 从此模块中读数据
				
				
*Auhor:			Savin

*Createdtime:	2017-04-19

*ModifyTime:	


********************************************************************************************/


#ifndef WORKDATALISTMGR_HEAD_H
#define WORKDATALISTMGR_HEAD_H

#include "Public.h"

class CWorkDataListMgr
{
public:
	static CWorkDataListMgr *GetInstance();	

	STDataNode *			GetDataNode();
	void 					PushDataNode(STDataNode *pNode);


private:

	CWorkDataListMgr();
	
	list<STDataNode *>			m_WorkDataList1;
	list<STDataNode *>			*m_pWWorkDataList;
	pthread_mutex_t				m_wListLock;

	list<STDataNode *>			m_WorkDataList2;
	list<STDataNode *>			*m_pRWorkDataList;
	pthread_mutex_t				m_rListLock;

	static CWorkDataListMgr	   	*m_pInstance;

};




#endif //WORKDATALISTMGR_HEAD_H

