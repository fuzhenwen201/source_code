/*************************************************************************
	> File Name: ../include/session_list.h
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Wed 22 Feb 2017 05:40:09 PM CST
 ************************************************************************/
#ifndef SOLUTIONGATEWAY_ORDER_LIST_H_
#define SOLUTIONGATEWAY_ORDER_LIST_H_

#include<stdlib.h>
#include<string.h>
#include<uv.h>
//#include"http_parser.h"
#include"define.h"
#include<sys/time.h>
#include<time.h>
#include<mutex>
#include <memory>
#include <bitset>
#include <map>

typedef struct {
	uv_write_t req;
	uv_buf_t buf;
}write_req_t;

#define ASYNC_T_SIZE 20
struct AsyncT {
	public:
		//AsyncT();

	public:
		uv_async_t data;
};

struct CustomSubscribe
{
	int direction;
	int assetid;
};

template<typename Node, size_t SIZE=2>
struct WriteReq{
	uv_write_t req;
	uv_buf_t buf[SIZE];
	Node* node;
	std::shared_ptr<void> header;
	std::shared_ptr<void> event;
	std::shared_ptr<void> subscribe;
};

enum ECLIENTTYPE
{
       EUNKNOW = 0x00,
       ETT = 0x01,
       ECMT = 0x02,
       EADT = 0x03,
};


/* CMTSessionListNode begin */
struct CMTSessionListNode {
	public:
		CMTSessionListNode();
		CMTSessionListNode(const char* sessionid, unsigned int sgid = 0, int managerid = 0, int groupid = 0, int clientid = 0);

		~CMTSessionListNode();
		CMTSessionListNode(const CMTSessionListNode& r) = delete;
		CMTSessionListNode& operator=(const CMTSessionListNode& r) = delete; 

		CMTSessionListNode(CMTSessionListNode&& r);
		CMTSessionListNode& operator=(CMTSessionListNode&& r);

		const char* ToJson();

		friend bool operator==(const CMTSessionListNode& l, const CMTSessionListNode& r);

	public:
		int m_status;
		int m_sgID;
		char* m_reqID;
		char* m_sessionID;
		char* m_json;
		int m_managerID;
		int m_groupID;
		int m_clientID;
		uv_stream_t* m_stream;
		long m_loginTime;
		char m_clienttype;
		std::bitset<2500> m_subscribtion;

		struct SUBSCRIBTION
		{
			std::bitset<72> subcustomsymbol[2];	
			std::map<int, bool> subcustomsymbolmap[2];
			bool subtop5bo[2];
			bool subsystemsymbol;
		};		
		SUBSCRIBTION m_subs;
};
/* CMTSessionListNode end */
#endif

