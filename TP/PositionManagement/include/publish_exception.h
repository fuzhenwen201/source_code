/*************************************************************************
	> File Name: publish_exception.h
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Wed 31 May 2017 03:46:36 PM CST
 ************************************************************************/
#ifndef POSITION_PUBLISH_EXCEPTION_H_
#define POSITION_PUBLISH_EXCEPTION_H_
#include"define.h"

/* {"tag":uint16,"time":uint64,"queue":"queuename","module":"PM","errno":int,
 * "errmsg":"message of error","body":{...}} */
class PMException {
	public:
		PMException(const char* body, 
					int errNo, 
					const char* errmsg, 
					int headTag=ETag::EPULL, 
					const char* queue=POSITIONMANAGEMENT_PUBLISH_QUENUENAME, 
					const char* module="PM", 
					const char* group=POSITIONMANAGEMENT_PUBLISH_NAME_CLG, 
					const char* event=POSITIONMANAGEMENT_PUBLISH_EVENT_CLG, 
					int tag=ETag::ECENTERLOG); 
		~PMException();
		PMException(const PMException& r) = delete;
		PMException& operator=(const PMException& r) = delete;
		PMException(PMException&& r) = delete;
		PMException& operator=(PMException&& r) = delete;

	public:
		void PublishException(bool update=false);

	private:
		int m_headTag;
		int m_headErrNo;
		int m_tag;
		const char* m_headQueue;
		const char* m_headModule;
		const char* m_headErrMsg;
		unsigned long m_headTime;
		const char* m_body;
		const char* m_group;
		const char* m_event;
		char* m_json;
};

#endif

