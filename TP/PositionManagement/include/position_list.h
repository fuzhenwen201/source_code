/*************************************************************************
	> File Name: position_list.h
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Tue 28 Feb 2017 06:32:53 PM CST
 ************************************************************************/
#ifndef POSITION_MANAGEMENT_POSITION_LIST_H_
#define POSITION_MANAGEMENT_POSITION_LIST_H_

#include<stdlib.h>
#include<string.h>
#include"define.h"

//namespace 
struct PositionListNode {
	public:
		PositionListNode(const char* reqID=nullptr);
		~PositionListNode();

		PositionListNode(const PositionListNode& r) = delete;
		PositionListNode& operator=(const PositionListNode& r) = delete; 
		PositionListNode(PositionListNode&& r);
		PositionListNode& operator=(PositionListNode&& r);

		friend bool operator==(const PositionListNode& l, const PositionListNode& r);

	public:
		/*
		 RequestStatus 
		 ACK: 1, REJECT: 2
		 SG bit2: 0: 接收者 1: 发送者
		 PM bit3: 0: 接收者 1: 发送者
		 TE bit4: 0: 接收者 1: 发送者
		 MT bit5: 0: 接收者 1: 发送者
		 CM bit6: 0: 接收者 1: 发送者
		 */
		unsigned char m_sg_system: 1; /* SG->PM: valid.sgid */ 
		unsigned char m_te_system: 1; /* TE->PM: validate.$ */ 
		unsigned char m_mt_system: 1; /* MT->PM: */ 
		unsigned char m_sg_sendorder: 1; /* SG->PM: recive SENDORDER.POSITION*/ 
		unsigned char m_pm_orderans: 1; /* PM->SG: sent ORDERANS.sgid*/ 
		unsigned char m_te_orderans_openprice: 1; /*TE -> SG: orderans.sgid: openpice */ 
		unsigned char m_te_orderans_closeprice: 1; /*TE -> SG: orderans.sgid closeprice */ 
		unsigned char : 0; /*forced alignment */ 
		char* m_reqID;

		//unsigned int m_stamp; /*send order unix time stamp */
		unsigned int m_expiryPeriod;
		std::chrono::system_clock::time_point m_openTime;
		std::chrono::system_clock::time_point m_stamp;

};

#endif



