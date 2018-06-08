/*************************************************************************
	> File Name: order_list.cc
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Wed 22 Feb 2017 05:38:00 PM CST
 ************************************************************************/
#include<chrono>
#include"define.h"
#include"session_list.h"
#include<uuid/uuid.h> /* for uuid */

/* CMTSessionListNode  begin****/
CMTSessionListNode::CMTSessionListNode() :
	m_status(0),
	m_sgID(0),
	m_json(nullptr),
	m_reqID(nullptr),
	m_sessionID(nullptr),
	m_managerID(0),
	m_groupID(0),
	m_clientID(0),
	m_stream(nullptr),
	m_loginTime(GetMicroSecond()) {
	m_reqID = (char*) malloc(REQUEST_ID_LEN * sizeof(char));
	memset(m_reqID, 0, REQUEST_ID_LEN* sizeof(char));

	m_sessionID = (char*) malloc(SESSION_ID_LEN * sizeof(char));
	memset(m_sessionID, 0, SESSION_ID_LEN * sizeof(char));
	m_clienttype = 0;
}

CMTSessionListNode::CMTSessionListNode(const char* sessionid, unsigned int sgid, int managerid, int groupid, int clientid) :
	m_status(0),
	m_sgID(sgid),
	m_json(nullptr),
	m_reqID(nullptr),
	m_sessionID(nullptr),
	m_managerID(managerid),
	m_groupID(groupid),
	m_clientID(clientid),
	m_stream(nullptr),
	m_loginTime(GetMicroSecond()) {
	m_sessionID = (char*) malloc(SESSION_ID_LEN * sizeof(char));
	memset(m_sessionID, 0, SESSION_ID_LEN * sizeof(char));
	memcpy(m_sessionID, sessionid, SESSION_ID_LEN);

	m_reqID = (char*) malloc(REQUEST_ID_LEN * sizeof(char));
	memset(m_reqID, 0, REQUEST_ID_LEN* sizeof(char));
	//memcpy(m_reqID, reqid, REQUEST_ID_LEN);
	m_clienttype = 0;
}

CMTSessionListNode::~CMTSessionListNode() {
	if (m_reqID != nullptr) {
		free(m_reqID);
		m_reqID = nullptr;
	}

	if (m_sessionID != nullptr) {
		free(m_sessionID);
		m_sessionID = nullptr;
	}

	if (m_json != nullptr) {
		free(m_json);
		m_json = nullptr;
	}

	// if(m_stream != nullptr) {
	// 	free(m_stream);
	// 	m_stream = nullptr;
	// }
}


CMTSessionListNode::CMTSessionListNode(CMTSessionListNode&& r) {
	m_status = r.m_status;
	m_sgID = r.m_sgID;
	m_reqID = r.m_reqID;
	r.m_reqID = nullptr;
	m_sessionID = r.m_sessionID;
	r.m_sessionID = nullptr;
	m_json = r.m_json;
	r.m_json = nullptr;
	m_managerID = r.m_managerID;
	m_groupID = r.m_groupID;
	m_clientID = r.m_clientID;
	if (r.m_stream && 
		m_stream == nullptr) {
		m_stream = r.m_stream;
		r.m_stream = nullptr;
	}

	m_loginTime = r.m_loginTime;
	m_clienttype = r.m_clienttype;
	m_subscribtion = r.m_subscribtion;
	m_subs = r.m_subs;

	r.m_status = 0;
	r.m_sgID = 0;
	r.m_managerID = 0;
	r.m_groupID = 0;
	r.m_clientID = 0;
	r.m_loginTime = 0;
	r.m_clienttype = 0;
	//r.m_subscribtion = 0;
}

CMTSessionListNode& CMTSessionListNode::operator=(CMTSessionListNode&& r) {
	if (this == &r) {
		return *this;
	}

	if (m_reqID != nullptr) {
		free(m_reqID);
		m_reqID = nullptr;
	}

	if (m_sessionID != nullptr) {
		free(m_sessionID);
		m_sessionID = nullptr;
	}

	if (m_json != nullptr) {
		free(m_json);
		m_json = nullptr;
	}

	m_status = r.m_status;
	m_sgID = r.m_sgID;
	m_reqID = r.m_reqID;
	r.m_reqID = nullptr;
	m_sessionID = r.m_sessionID;
	r.m_sessionID = nullptr;
	m_json = r.m_json;
	r.m_json = nullptr;
	m_managerID = r.m_managerID;
	m_groupID = r.m_groupID;
	m_clientID = r.m_clientID;
	if (r.m_stream && 
		m_stream == nullptr) {
		m_stream = r.m_stream;
		r.m_stream = nullptr;
	}
	//m_stream = r.m_stream;
	//r.m_stream = nullptr;
	m_loginTime = r.m_loginTime;
	m_clienttype = r.m_clienttype;
	m_subscribtion = r.m_subscribtion;
	m_subs = r.m_subs;

	r.m_status = 0;
	r.m_sgID = 0;
	r.m_managerID = 0;
	r.m_groupID = 0;
	r.m_clientID = 0;
	r.m_loginTime = 0;
	r.m_clienttype = 0;

	//XINFO("CMTSessionListNode& CMTSessionListNode::operator=(CMTSessionListNode&& r)");

	return *this;
}

const char* CMTSessionListNode::ToJson(void) {
	if (m_json == nullptr) {
		uuid_t uuid;
		char reqid[33];
		uuid_generate(uuid);
		DecToHex(reqid, (char*)&uuid);
		reqid[32] = 0;
		// asprintf(&m_json, "{\"reqid\":\"%s\",\"session\":\"%s\",\"managerid\":\"%d\",\"groupid\":%d,\"clientid\":%d,\"sgid\":%d,\"status\":0}",
		// 		 reqid, m_sessionID, m_managerID, m_groupID, m_clientID, m_sgID);
		// asprintf(&m_json, "{\"reqid\":\"%s\",\"session\":\"%s\",\"managerid\":\"%d\",\"adminid\":\"%d\",\"groupid\":%d,\"clientid\":%d,\"sgid\":%d,\"status\":0}",
		// 		 reqid, m_sessionID, m_managerID, m_managerID, m_groupID, m_clientID, m_sgID);
       if(m_clienttype == ECLIENTTYPE::ECMT) {
			asprintf(&m_json, "{\"reqid\":\"%s\",\"session\":\"%s\",\"managerid\":\"%d\",\"groupid\":%d,\"clientid\":%d,\"sgid\":%d,\"status\":0}",
					 reqid, m_sessionID, m_managerID, m_groupID, m_clientID, m_sgID);                   

       } else if(m_clienttype == ECLIENTTYPE::EADT) {
			// asprintf(&m_json, "{\"reqid\":\"%s\",\"session\":\"%s\",\"managerid\":\"%d\",\"adminid\":\"%d\",\"groupid\":%d,\"clientid\":%d,\"sgid\":%d,\"status\":0}",
			// 		 reqid, m_sessionID, m_managerID, m_managerID, m_groupID, m_clientID, m_sgID);
			asprintf(&m_json, "{\"reqid\":\"%s\",\"session\":\"%s\",\"adminid\":\"%d\",\"groupid\":%d,\"clientid\":%d,\"sgid\":%d,\"status\":0}",
		 			reqid, m_sessionID, m_managerID, m_groupID, m_clientID, m_sgID);

       } else {
               m_json = (char*)malloc(1);
               m_json[0] = 0;
       }

	}

	return m_json;
}

bool operator==(const CMTSessionListNode& l, const CMTSessionListNode& r) {
	// if (memcmp(l.m_sessionID, r.m_sessionID, SESSION_ID_LEN) == 0 && 
	// 	r.m_stream != nullptr) {
	// 	return true;
	// }

	// if (l.m_stream == r.m_stream) {
	// 	return true;
	// }

	if (memcmp(l.m_sessionID, r.m_sessionID, SESSION_ID_LEN) == 0) {
			return true;
	}

	size_t llen = strlen(l.m_sessionID);
	size_t rlen = strlen(r.m_sessionID);
	if (l.m_stream == r.m_stream &&
		llen != rlen) {
		return true;
	}

	return false;
}
/* CMTSessionListNode  end****/
