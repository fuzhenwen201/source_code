/*************************************************************************
	> File Name: position_list.cc
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Tue 28 Feb 2017 06:32:31 PM CST
 ************************************************************************/
#include"position_list.h"

PositionListNode::PositionListNode(const char* reqID) :
	m_sg_system(0),
	m_te_system(0),
	m_mt_system(0),
	m_sg_sendorder(0),
	m_pm_orderans(0),
	m_te_orderans_openprice(0),
	m_te_orderans_closeprice(0),
	m_reqID(nullptr),
	m_expiryPeriod(0),
	m_openTime{},
	m_stamp(std::chrono::system_clock::now()) {
	m_reqID = (char*) malloc(SOLUTIONGATEWAY_REQUEST_ID_LEN * sizeof(char));
	memset(m_reqID, 0, SOLUTIONGATEWAY_REQUEST_ID_LEN * sizeof(char));
	if (reqID!=nullptr) {
		memcpy(m_reqID, reqID, SOLUTIONGATEWAY_REQUEST_ID_LEN);
	}
}

PositionListNode::~PositionListNode() {
	if (m_reqID != nullptr) {
		free(m_reqID);
		m_reqID = nullptr;
	}
}

PositionListNode::PositionListNode(PositionListNode&& r) {
	m_sg_system = r.m_sg_system;
	m_te_system = r.m_te_system;
	m_mt_system = r.m_mt_system;
	m_sg_system = r.m_sg_sendorder;
	m_pm_orderans = r.m_pm_orderans;
	m_te_orderans_openprice = r.m_te_orderans_openprice;
	m_te_orderans_closeprice = r.m_te_orderans_closeprice;
	m_reqID = r.m_reqID;
	m_expiryPeriod = r.m_expiryPeriod;
	m_openTime = std::move(r.m_openTime);
	m_stamp = std::move(r.m_stamp);

	r.m_sg_system = 0;
	r.m_te_system = 0;
	r.m_mt_system = 0;
	r.m_sg_sendorder = 0;
	r.m_pm_orderans = 0;
	r.m_te_orderans_openprice = 0;
	r.m_te_orderans_closeprice = 0;
	r.m_reqID = nullptr;
	r.m_expiryPeriod = 0;
	r.m_openTime = {};
	r.m_stamp = {};
}

PositionListNode& PositionListNode::operator=(PositionListNode&& r) {
	if (this == &r) {
		return *this;
	}

	if (m_reqID != nullptr) {
		free(m_reqID);
		m_reqID = nullptr;
	}

	m_sg_system = r.m_sg_system;
	m_te_system = r.m_te_system;
	m_mt_system = r.m_mt_system;
	m_sg_system = r.m_sg_sendorder;
	m_pm_orderans = r.m_pm_orderans;
	m_te_orderans_openprice = r.m_te_orderans_openprice;
	m_te_orderans_closeprice = r.m_te_orderans_closeprice;
	m_reqID = r.m_reqID;
	m_expiryPeriod = r.m_expiryPeriod;
	m_openTime = std::move(r.m_openTime);
	m_stamp = std::move(r.m_stamp);

	r.m_sg_system = 0;
	r.m_te_system = 0;
	r.m_mt_system = 0;
	r.m_sg_sendorder = 0;
	r.m_pm_orderans = 0;
	r.m_te_orderans_openprice = 0;
	r.m_te_orderans_closeprice = 0;
	r.m_reqID = nullptr;
	r.m_expiryPeriod = 0;
	r.m_openTime = {};
	r.m_stamp = {};

	return *this;
}
