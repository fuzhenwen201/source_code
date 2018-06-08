/*******************************************************************************************

*Department:	TP

*Decrible:  	worker 模块
				业务处理的模块

				
			
*Auhor:			Savin Chen

*Createdtime:	2017-05-08

*ModifyTime:	2017-07-27  新增分组模块,新增spread模块
	


********************************************************************************************/




#ifndef WORKER_HEAD_H
#define WORKER_HEAD_H

#include "Public.h"
#include "BaseThread.h"
#include "Group_Spread.h"  



class CWorkerProcee : public CBaseThread
{
public:
	CWorkerProcee();
	virtual 		~CWorkerProcee();

	static void 	WorkerBatchStart();
	static int 		LRModuleProcessRequest(STDataNode* &pData);
	
	virtual void 	Run() ;  
	void 			AddWorkerToList();
	int 			GetClientNumber(){return m_nClientNumber ;}

	CGroupSpread 	*m_pGroupSpread ;

private:
	int 			DeleteOrUpdateorAdd(STDataNode* &pData, int number);
	int 			ProcessDeleteOrUpdateOrAdd();
	int 			ParseRTRequest(STDataNode *pData,STMsgRequestAck &ack);
	int 			ProcessRTDelUpdateAddRequest(STDataNode *pData,STMsgRequestAck &ack);
	
	int 			GetMarketPrice(STDataNode *pData ,STRTMsgToClient &rtMsgToClient,STRTMsgToClient &rtMsgToClient_bo);
	int 			ParseCoinName(STDataNode *pData);
	
	
	
	map<unsigned long,STDataNode *> 				m_RTWorkerList;
	int												m_nClientNumber;

	list<STDataNode *> 								m_RTOperateList;
	pthread_mutex_t								 	m_RTOperateListLock;
	
	static list<CWorkerProcee *> 					m_WorkerList;
};


#endif //DISTRIBUTE_PUBLIC_HEAD_H
