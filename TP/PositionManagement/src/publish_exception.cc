/*************************************************************************
	> File Name: publish_exception.cc
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Wed 31 May 2017 04:46:11 PM CST
 ************************************************************************/
#include"publish_exception.h"
#include"tnode_adapter.h"

PMException::PMException(const char* body, int errNo, const char* errmsg, 
						 int headTag, 
						 const char* queue, 
						 const char* module, 
						 const char* group, 
						 const char* event, 
						 int tag) :
	m_headTag(headTag),
	m_headErrNo(errNo),
	m_tag(tag),
	m_headQueue(queue),
	m_headModule(module),
	m_headErrMsg(errmsg),
	m_headTime(0),
	m_body(body),
	m_group(group),
	m_event(event),
	m_json(nullptr) {
	//m_headTime = GetMircoSecond();
}

PMException::~PMException() {
	if (nullptr != m_json) {
		free(m_json);
		m_json = nullptr;
	}
}

void PMException::PublishException(bool update) {
	if (true == update && 
		nullptr != m_json) {
		free(m_json);
		m_json = nullptr;
	}

	size_t length = asprintf(&m_json, "{\"tag\":%d,\"time\":%lu,\"queue\":\"%s\",\"module\":\"%s\",\"errno\":%d,\"errmsg\":\"%s\",\"body\":%s}",
						m_headTag,
						//m_headTime,
						GetMicroSecond(),
						m_headQueue,
						m_headModule,
						m_headErrNo,
						m_headErrMsg,
						m_body);

	TNodeAministrator::GetInstance()->PublishGroup(m_group,
												   m_event,
												   m_json,
												   ++length,
												   m_tag);
}


