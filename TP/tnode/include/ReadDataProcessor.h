/*******************************************************************************************

*Department:	TP

*Decrible:  	TNode 新增读事件回调处理模块
				
*Auhor:			Savin

*Createdtime:	2017-06-09

*ModifyTime:	


********************************************************************************************/


#ifndef TNODE_READDATA_H
#define TNODE_READDATA_H

#include <pthread.h>
#include <list>
#include "BaseThread.h"
#include "tnodeconsumer.h"


using namespace std;
using namespace snetwork_xservice_tnode ;



typedef struct _STTnodeReadDataHeader
{
    unsigned short  	nTag ;
    unsigned int    	nDataLen;
    TNodeConsumer   	*pConsumer;
}__attribute__((packed)) STTnodeReadDataHeader;


typedef struct _STTnodeReadData
{
    STTnodeReadDataHeader 	header;
    char            		*pData;

} __attribute__((packed)) STTnodeReadData;




class CReadDataProcessor   : public CBaseThread
{  
public:  
   virtual void 							Run() ;  
   

	static void 							StartBatchSender();
	static void 							PushReadData(uint16 tag, char *event, uint32 eventlen,TNodeConsumer *consumer);
	static STTnodeReadData * 				GetReadData();
	
  
private:  
    

	static list<STTnodeReadData *>			m_RealtimeDataList1;
	static list<STTnodeReadData *>			*m_pRealtimeDataList_w;
	static pthread_mutex_t					m_wListLock;

	static list<STTnodeReadData *>			m_RealtimeDataList2;
	static list<STTnodeReadData *>			*m_pRealtimeDataList_r;
	static pthread_mutex_t					m_rListLock;
	
};




#endif //TNODE_READDATA_H

