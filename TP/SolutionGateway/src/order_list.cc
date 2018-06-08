/*************************************************************************
	> File Name: order_list.cc
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Wed 22 Feb 2017 05:38:00 PM CST
 ************************************************************************/
#include<chrono>
#include"define.h"
#include"order_list.h"
#include<uuid/uuid.h> /* for uuid */

/* SendOrderListNode  begin****/
SendOrderListNode::SendOrderListNode() :
	m_reqid_flag(true),
	m_pm_user(0),
	m_te_user(0),
	m_mt_user(0),
	m_pm_valid(0),
	m_pm_orderans(0),
	m_te_orderans_openprice(0),
	m_te_orderans_closeprice(0),
	m_cm_login(0),
	m_sgID(0),
	m_reqID(nullptr),
	m_sessionID(nullptr),
	m_http_request(nullptr),
	m_expiryPeriod(0),
	m_openTime(0),
	//m_closeTime{},
	m_stamp(GetMicroSecond()) {
	m_reqID = (char*) malloc(REQUEST_ID_LEN * sizeof(char));
	memset(m_reqID, 0, REQUEST_ID_LEN * sizeof(char));
	m_sessionID = (char*) malloc(SESSION_ID_LEN * sizeof(char));
	memset(m_sessionID, 0, SESSION_ID_LEN * sizeof(char));
}

SendOrderListNode::SendOrderListNode(const char* reqID, unsigned int sgid) :
	m_reqid_flag(true),
	m_pm_user(0),
	m_te_user(0),
	m_mt_user(0),
	m_pm_valid(0),
	m_pm_orderans(0),
	m_te_orderans_openprice(0),
	m_te_orderans_closeprice(0),
	m_cm_login(0),
	m_cm_logout(0),
	m_sgID(sgid),
	m_reqID(nullptr),
	m_sessionID(nullptr),
	m_groupID(0),
	m_clientID(0),
	m_http_request(nullptr),
	m_expiryPeriod(0),
	m_openTime(0),
	//m_closeTime{},
	m_stamp(GetMicroSecond()) {
	m_reqID = (char*) malloc(REQUEST_ID_LEN * sizeof(char));
	memset(m_reqID, 0, REQUEST_ID_LEN * sizeof(char));
	memcpy(m_reqID, reqID, REQUEST_ID_LEN);
	m_sessionID = (char*) malloc(SESSION_ID_LEN * sizeof(char));
	memset(m_sessionID, 0, SESSION_ID_LEN * sizeof(char));
	//m_http_request = (http_request_t*) malloc(sizeof(http_request_t));
}

SendOrderListNode::SendOrderListNode(const char* sessionid, int groupid, int clientid, unsigned int sgid) :
	m_reqid_flag(true),
	m_pm_user(0),
	m_te_user(0),
	m_mt_user(0),
	m_pm_valid(0),
	m_pm_orderans(0),
	m_te_orderans_openprice(0),
	m_te_orderans_closeprice(0),
	m_cm_login(0),
	m_cm_logout(0),
	m_sgID(sgid),
	m_reqID(nullptr),
	m_sessionID(nullptr),
	m_groupID(groupid),
	m_clientID(clientid),
	m_http_request(nullptr),
	m_expiryPeriod(0),
	m_openTime(0),
	m_stamp(GetMicroSecond()) {
	m_reqID = (char*) malloc(REQUEST_ID_LEN * sizeof(char));
	memset(m_reqID, 0, REQUEST_ID_LEN * sizeof(char));
	m_sessionID = (char*) malloc(SESSION_ID_LEN * sizeof(char));
	memset(m_sessionID, 0, SESSION_ID_LEN * sizeof(char));
	memcpy(m_sessionID, sessionid, SESSION_ID_LEN);
}

SendOrderListNode::~SendOrderListNode() {
	if (m_reqID != nullptr) {
		free(m_reqID);
		m_reqID = nullptr;
	}
	
	if (m_sessionID != nullptr) {
		free(m_sessionID);
		m_sessionID = nullptr;
	}
#if 0
	if (m_http_request != nullptr) {
		free(m_http_request);
		m_http_request = nullptr;
	}
#endif
}

SendOrderListNode::SendOrderListNode(SendOrderListNode&& r) {
	m_reqid_flag = r.m_reqid_flag;
	m_pm_user = r.m_pm_user;
	m_te_user = r.m_te_user;
	m_mt_user = r.m_mt_user;
	m_pm_valid = r.m_pm_valid;
	m_pm_orderans = r.m_pm_orderans;
	m_te_orderans_openprice = r.m_te_orderans_openprice;
	m_te_orderans_closeprice = r.m_te_orderans_closeprice,
	m_cm_login = r.m_cm_login,
	m_cm_logout = r.m_cm_logout,
	m_reqID = r.m_reqID;
	r.m_reqID = nullptr;
	m_sessionID = r.m_sessionID;
	r.m_sessionID = nullptr;
	m_groupID = r.m_groupID;
	m_clientID = r.m_clientID;
	m_expiryPeriod = r.m_expiryPeriod;
	m_openTime = r.m_openTime;
	//m_closeTime = std::move(r.m_closeTime);
	m_stamp = r.m_stamp;

	m_sgID = r.m_sgID;
	m_http_request = r.m_http_request;
	r.m_http_request = nullptr;

	r.m_pm_user = 0;
	r.m_te_user = 0;
	r.m_mt_user = 0;
	r.m_pm_valid = 0;
	r.m_pm_orderans = 0;
	r.m_te_orderans_openprice = 0;
	r.m_te_orderans_closeprice = 0;
	r.m_cm_login = 0;
	r.m_cm_logout = 0;
	r.m_sgID = 0;
	r.m_groupID = 0;
	r.m_clientID = 0;
	r.m_expiryPeriod = 0;
	r.m_openTime = 0;
	//r.m_closeTime = {};
	r.m_stamp = 0;
}

SendOrderListNode& SendOrderListNode::operator=(SendOrderListNode&& r) {
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

	m_reqid_flag = r.m_reqid_flag;
	m_pm_user = r.m_pm_user;
	m_te_user = r.m_te_user;
	m_mt_user = r.m_mt_user;
	m_pm_valid = r.m_pm_valid;
	m_pm_orderans = r.m_pm_orderans;
	m_te_orderans_openprice = r.m_te_orderans_openprice;
	m_te_orderans_closeprice = r.m_te_orderans_closeprice;
	m_cm_login = r.m_cm_login;
	m_cm_logout = r.m_cm_logout;
	m_reqID = r.m_reqID;
	r.m_reqID = nullptr;
	m_sessionID = r.m_sessionID;
	r.m_sessionID = nullptr;
	m_groupID = r.m_groupID;
	m_clientID = r.m_clientID;
	m_expiryPeriod = r.m_expiryPeriod;
	m_openTime = r.m_openTime;
	//m_closeTime = std::move(r.m_closeTime);
	m_stamp = r.m_stamp;

	m_sgID = r.m_sgID;
	m_http_request = r.m_http_request;
	r.m_http_request = nullptr;

	r.m_pm_user = 0;
	r.m_te_user = 0;
	r.m_mt_user = 0;
	r.m_pm_valid = 0;
	r.m_pm_orderans = 0;
	r.m_te_orderans_openprice = 0;
	r.m_te_orderans_closeprice = 0;
	r.m_cm_login = 0,
	r.m_groupID = 0;
	r.m_clientID = 0;
	r.m_sgID = 0;
	r.m_expiryPeriod = 0;
	r.m_openTime = 0;
	//r.m_closeTime = {};
	r.m_stamp = 0;

	return *this;
}

bool operator==(const SendOrderListNode& l, const SendOrderListNode& r) {
	if (memcmp(l.m_reqID, r.m_reqID, REQUEST_ID_LEN) == 0
		&& l.m_sgID == r.m_sgID) {
		return true;
	}

	if (l.m_groupID == r.m_groupID &&
		l.m_clientID == r.m_clientID &&
		memcpy(l.m_sessionID, r.m_sessionID, SESSION_ID_LEN) == 0) {
		return true;
	}

	return false;
}
/* SendOrderListNode  end****/

/* SessionListNode  begin****/
SessionListNode::SessionListNode() :
	m_status(0),
	m_sgID(0),
	m_json(nullptr),
	m_sessionID(nullptr),
	m_groupID(0),
	m_clientID(0),
	m_http_request(nullptr),
	m_loginTime(GetMicroSecond()) {
	m_sessionID = (char*) malloc(SESSION_ID_LEN * sizeof(char));
	memset(m_sessionID, 0, SESSION_ID_LEN * sizeof(char));
}

SessionListNode::SessionListNode(const char* sessionid, int groupid, int clientid, unsigned int sgid) :
	m_status(0),
	m_sgID(sgid),
	m_json(nullptr),
	m_sessionID(nullptr),
	m_groupID(groupid),
	m_clientID(clientid),
	m_http_request(nullptr),
	m_loginTime(GetMicroSecond()) {
	m_sessionID = (char*) malloc(SESSION_ID_LEN * sizeof(char));
	memset(m_sessionID, 0, SESSION_ID_LEN * sizeof(char));
	memcpy(m_sessionID, sessionid, SESSION_ID_LEN);
}

SessionListNode::~SessionListNode() {
	if (m_sessionID != nullptr) {
		free(m_sessionID);
		m_sessionID = nullptr;
	}

	if (m_json != nullptr) {
		free(m_json);
		m_json = nullptr;
	}
}


SessionListNode::SessionListNode(SessionListNode&& r) {
	m_status = r.m_status;
	m_sgID = r.m_sgID;
	m_sessionID = r.m_sessionID;
	r.m_sessionID = nullptr;
	m_json = r.m_json;
	r.m_json = nullptr;
	m_groupID = r.m_groupID;
	m_clientID = r.m_clientID;
	m_http_request = r.m_http_request;
	r.m_http_request = nullptr;
	m_loginTime = r.m_loginTime;

	r.m_status = 0;
	r.m_sgID = 0;
	r.m_groupID = 0;
	r.m_clientID = 0;
	r.m_loginTime = 0;
}

SessionListNode& SessionListNode::operator=(SessionListNode&& r) {
	if (this == &r) {
		return *this;
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
	m_sessionID = r.m_sessionID;
	r.m_sessionID = nullptr;
	m_json = r.m_json;
	r.m_json = nullptr;
	m_groupID = r.m_groupID;
	m_clientID = r.m_clientID;
	m_http_request = r.m_http_request;
	r.m_http_request = nullptr;
	m_loginTime = r.m_loginTime;

	r.m_status = 0;
	r.m_sgID = 0;
	r.m_groupID = 0;
	r.m_clientID = 0;
	r.m_loginTime = 0;

	return *this;
}

const char* SessionListNode::ToJson(void) {
	if (m_json == nullptr) {
		uuid_t uuid;
		char reqid[33];
		uuid_generate(uuid);
		DecToHex(reqid, (char*)&uuid);
		reqid[32] = 0;
		asprintf(&m_json, "{\"reqid\":\"%s\",\"sessionid\":\"%s\",\"groupid\":%d,\"clientid\":%d,\"sgid\":%d,\"status\":0}",
				 reqid, m_sessionID, m_groupID, m_clientID, m_sgID);
	}

	return m_json;
}

bool operator==(const SessionListNode& l, const SessionListNode& r) {
	//if (memcmp(l.m_sessionID, r.m_sessionID, SESSION_ID_LEN) == 0) {
	if (memcmp(l.m_sessionID, r.m_sessionID, SESSION_ID_LEN) == 0 && //for normal
		r.m_http_request != nullptr) {
		return true;
	}

	if (l.m_http_request == r.m_http_request && //for free tcp in OnClose
	    r.m_http_request != nullptr) {
		return true;
	}

#if 0
	if (l.m_clientID == r.m_clientID &&
		l.m_groupID == r.m_groupID &&
		l.m_http_request != nullptr) { /* for forcelogout */
		return true;
	}
#endif

	if (l.m_clientID == r.m_clientID && //for orderans back to TT correctly
		l.m_groupID == r.m_groupID &&
		r.m_http_request != nullptr &&
		l.m_status != ELogin::ELOGINLOGOUT) {
		return true;
	}

	return false;
}
/* SessionListNode  end****/
