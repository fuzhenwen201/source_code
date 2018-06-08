/*******************************************************************************************

*Department:	TP

*Decrible:  	SDGDistrubute 内部服务 数据处理类
				1. 来自内部服务, 比如说GW,MONITOR等等
				2. 鉴于目前内部服务不多, 其到SDGDistribute交互不多, 使用单线程处理设计
				
*Auhor:			Savin Chen

*Createdtime:	2017-04-20

*ModifyTime:	


********************************************************************************************/

#ifndef INSERVICEWORK_HEAD_H
#define INSERVICEWORK_HEAD_H


#include "Public.h"
#include "BaseThread.h"

using namespace std;


class CInserviceWork :public CBaseThread
{
public:
	virtual 					~CInserviceWork(){}
	static 	CInserviceWork * 	GetInstance();
	virtual void 				Run() ;  
	int 						ProcessInData(STDataNode * &pNode);
	int 						PushDataNode(STDataNode * pNode);

	CSgIpUrlMap					m_SgUrlMap;

private:

	CInserviceWork();

	int  ProcessGwMsg(STDataNode * &pNode,STInServiceAck &ack);
	int  ProcessDistributeMsg(STDataNode * &pNode,STInServiceAck &ack);
	int  ProcessGw(STDataNode * &pNode, char *pData,STInServiceAck & ack,unsigned char inclient_type) ;


	static CInserviceWork 						*m_pInstance;
	static list<STDataNode *> 					 m_InRequestList; 
	static pthread_mutex_t						 m_InRequestListLock;

	

};


#endif  //INSERVICEWORK_HEAD_H

