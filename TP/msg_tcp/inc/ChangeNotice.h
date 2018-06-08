
/*******************************************************************************************

*Department:	TP

*Decrible:  	spread change notice
				
*Auhor:			Savin

*Createdtime:	2017-08-21

*ModifyTime:	


********************************************************************************************/
#ifndef CHANGE_NOTICE_HEAD_H
#define CHANGE_NOTICE_HEAD_H

#include "tnode.h"


using namespace snetwork_xservice_tnode;


int ChangeNoticeInit();



class CChangeNoticeConsumer : public TNodeConsumer
{
public:
	CChangeNoticeConsumer(const char *bindingkey, const char *queuename  )
	{
		
		m_bindingkey = bindingkey ;
		m_queuename = queuename;
	}
	CChangeNoticeConsumer(){}
	
	std::string GetBindingkey(){ return m_bindingkey ; }
	std::string GetQueueName(){ return m_queuename ; }

	
	 unsigned int ConsumerData(STTnodeDataPacketHeader *pHeader, char *pBody);
	
	

private:
	
	std::string m_bindingkey ;
	std::string m_queuename  ;

};



#endif //CHANGE_NOTICE_HEAD_H


