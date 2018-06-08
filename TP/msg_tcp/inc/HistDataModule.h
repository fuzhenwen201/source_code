/*******************************************************************************************

*Department:	TP

*Decrible:  	新增Histroy market price 模块, 处理客户端的历史行情的查询
				包括CHistData,HistConsumer 类
				
*Auhor:			Savin Chen

*Createdtime:	2017-05-31

*ModifyTime:	


********************************************************************************************/




#ifndef HISTORYDATA_HEAD_H
#define HISTORYDATA_HEAD_H

#include "Public.h"
#include "BaseThread.h"
#include "marketdataagent.h"
#include "xlogger.h"
#include "Group_Spread.h"

#include "tnode.h"
//#include "tnodeconfig_v0.h"

using namespace snetwork_xservice_tnode;



class CHistDataProcess : public CBaseThread
{
public:
									CHistDataProcess();
	virtual 						~CHistDataProcess(){}

	int 							Init();

	static void 					HistDataStart();
	static int 						LRModuleProcessHistDataRequest(STDataNode* &pData);
	static int 						ConsumerProcessHistData(uint16 tag, char *event, uint32 eventlen);


	
	virtual void 					Run() ;  
	void 							Sender();

	static void 					PushFromMQHistData(char *data,uint32 eventlen);
	static void 					LRModulePushHistData( STDataNode* &pData);

	static int 						ConsumerProcessData(uint16 tag, char *event, uint32 eventlen);
	CGroupSpread 					*m_pGroupSpread ;

private:

	int 							PushDataToMQ();
	int 							SendHistDataResponse(STDataNode* &pData,string *pBuf);
	int 							SendHistDataResponse(STDataNode* &pData);
	int								SendHistDataResponseToClient();
	int								FindClientInfo(char *pSrcs, const char *pDests);

	
	static list<STDataNode *> 						m_HistDataList;
	static pthread_mutex_t							m_HistDataListLock;
	
	static list<STDataNode *> 						m_HistDataProcessList;
	static pthread_mutex_t							m_HistDataProcessListLock;

	static list<string> 							m_HistDataProcessedList;
	static pthread_mutex_t							m_HistDataProcessedListLock;

	TNode 											*m_pTnode;

	
};




class CHistDataConsumer : public TNodeConsumer
{
public:
	CHistDataConsumer(const char *bindingkey, const char *queuename  )
	{
		
		m_bindingkey = bindingkey ;
		m_queuename = queuename;
	}
	CHistDataConsumer(){}
	
	std::string GetBindingkey(){ return m_bindingkey ; }
	std::string GetQueueName(){ return m_queuename ; }

	
	 unsigned int ConsumerData(STTnodeDataPacketHeader *pHeader, char *pBody);
	
	

private:
	
	std::string m_bindingkey ;
	std::string m_queuename  ;

};








#endif //HISTORYDATA_HEAD_H
