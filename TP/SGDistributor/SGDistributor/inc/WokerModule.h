/*******************************************************************************************

*Department:	TP

*Decrible:  	worker 模块
				业务处理的模块

		
*Auhor:			Savin Chen

*Createdtime:	2017-04-21

*ModifyTime:	2017-08-15 添加SG域名映射功能


********************************************************************************************/




#ifndef WORKER_HEAD_H
#define WORKER_HEAD_H

#include "Public.h"
#include "BaseThread.h"
#include "URLMap.h"  

class CWorkerProcee : public CBaseThread
{
public:
	
	static void 	WorkerBatchStart();

					CWorkerProcee();
	virtual 		~CWorkerProcee();

	void 			AddWorkerToList();
	
	// InService 分发路由
	static int 		InserviceDistributeGWMsg(STINServiceGWBody &msg);
	int 			DistributeGWMsg(STINServiceGWBody &msg);
	
	static int 		LRModuleDeleteGWMsg(STConnNode *&pConnNode);
	int 			DeleteGWMsg(STConnNode *&pConnNode);
	
	virtual void	Run() ;  

	CSgIpUrlMap		m_SgUrlMap;

private:

	void 			ShowGwMap();

	// 解析外部协议
	int 			ParseOutProtocol(STDataNode * &pNode,STOutAppMsgRequest &appMsg,STOutAppMsgAck &appAckMsg);

	// 获取GW路由
	int 			GetGwInfo(STOutAppMsgRequest &appMsg,STOutAppMsgAck &outCliAckMsg) ;
	int 			GetGWIPPort_OldRoute(STOutAppMsgRequest &appMsg,STOutAppMsgAck &outCliAckMsg);
	int 			GetGWIPPort_Aerage(STOutAppMsgRequest &appMsg,STOutAppMsgAck &outCliAckMsg);
	
	// 路由表
	map<unsigned long, STINServiceGWBody>  			m_INServiceGWMap;
	pthread_mutex_t								 	m_INServiceGWMapLock;

	static list<CWorkerProcee *> 					m_WorkerList;
	static pthread_mutex_t							m_WorkerListLock;

};


#endif //DISTRIBUTE_PUBLIC_HEAD_H
