/*******************************************************************************************

*Department:	TP

*Decrible:  	实时数据发往MQ
				
				


*Auhor:			Savin

*Createdtime:	2017-06-07

*ModifyTime:	


********************************************************************************************/


#ifndef REALTIMESEND_HEAD_H
#define REALTIMESEND_HEAD_H

#include <pthread.h>
#include <list>
#include "BaseThread.h"
#include "quickfix/STQuoteRequest44.h"
#include "tnode.h"
//#include "tnodechannel.h"

#include "Group_Spread.h"
#include "ChangeNotice.h"


using namespace std;
using namespace snetwork_xservice_tnode ;

class CRealtimeSender   : public CBaseThread
{  
public:  
	CRealtimeSender();
	
   virtual void 							Run() ;  
   

	static void 							StartBatchSender();
	static void 							PushRealtimeData(STQuoteRequest44 *pData);
	static STQuoteRequest44 * 				GetRealtimeData();

	CGroupSpread 							*m_pGroupSpread ;
  
private:  
    CGroupSpread 							spreadlist;
	
	static list<STQuoteRequest44 *>			m_RealtimeDataList1;
	static list<STQuoteRequest44 *>			*m_pRealtimeDataList_w;
	static pthread_mutex_t					m_wListLock;

	static list<STQuoteRequest44 *>			m_RealtimeDataList2;
	static list<STQuoteRequest44 *>			*m_pRealtimeDataList_r;
	static pthread_mutex_t					m_rListLock;


};




#endif //REALTIMESEND_HEAD_H

