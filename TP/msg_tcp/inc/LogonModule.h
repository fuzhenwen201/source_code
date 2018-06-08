/*******************************************************************************************

*Department:	TP

*Decrible:  	新增logon模块 , 处理客户端 登录认证
				包括logon,consumer 类
				
*Auhor:			Savin Chen

*Createdtime:	2017-05-26

*ModifyTime:	2017-08-04    新增 认证成功后返回 货币列表  


********************************************************************************************/




#ifndef LOGON_HEAD_H
#define LOGON_HEAD_H

#include "Public.h"
#include "BaseThread.h"
#include "marketdataagent.h"
#include "xlogger.h"
#include "Group_Spread.h"



using namespace snetwork_xservice_tnode;

typedef unsigned int uint32;
typedef unsigned short uint16;

class CLogonProcee : public CBaseThread
{
public:
						CLogonProcee();
	virtual 			~CLogonProcee(){};

	int 				Init();

	static void 		LogonStart();
	static int 			LRModuleProcessLogonRequest(STDataNode* &pData);
	static int 			ConsumerProcessData(uint16 tag, char *event, uint32 eventlen);
	
	virtual void 		Run() ;  

	static void 		PushAuthedData(char *data);
	void 				Sender();


private:

	int 				PushDataToMQ();
	int 				SendAuthResponse(STDataNode* &pData);
	int					SendAuthResponseToClient();

	
	static list<STDataNode *> 							m_LogonList;
	static pthread_mutex_t								m_LogonListLock;
	
	static map<unsigned long,STDataNode *> 				m_LogonAuthList;
	static pthread_mutex_t								m_LogonAuthListLock;

	static list<string> 								m_LogonAuthedList;
	static pthread_mutex_t								m_LogonAuthedListLock;

	TNode 												*m_pTnode;

	CGroupSpread 										*m_pGroupSpread ;
	
};




class LogonConsumer : public TNodeConsumer
{
public:
	LogonConsumer(const char *bindingkey, const char *queuename  )
	{
		
		m_bindingkey = bindingkey ;
		m_queuename = queuename;
	}
	LogonConsumer(){}
	
	std::string GetBindingkey(){ return m_bindingkey ; }
	std::string GetQueueName(){ return m_queuename ; }

	
	 unsigned int ConsumerData(STTnodeDataPacketHeader *pHeader, char *pBody);
	
	

private:
	
	std::string m_bindingkey ;
	std::string m_queuename  ;

};







#endif //LOGON_HEAD_H
