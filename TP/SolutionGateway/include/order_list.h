/*************************************************************************
	> File Name: ../include/order_list.h
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Wed 22 Feb 2017 05:40:09 PM CST
 ************************************************************************/
#ifndef SOLUTIONGATEWAY_ORDER_LIST_H_
#define SOLUTIONGATEWAY_ORDER_LIST_H_

#include<stdlib.h>
#include<string.h>
#include<uv.h>
#include"http_parser.h"
#include"define.h"
#include<sys/time.h>
#include<time.h>
#include<memory>

typedef struct {
	const char* field;
	const char* value;
	//char* field;
	//char* value;
	size_t field_length;
	size_t value_length;
} http_header_t;

typedef struct {
#if defined(SIGNALENGINE)
	uv_write_t req;
#endif
	uv_stream_t stream;
#if defined(SIGNALENGINE)
	http_parser parser;
	char* url;
	char* method;
	int header_lines;
	http_header_t headers[MAX_HTTP_HEADERS];
	char* body;
	struct sockaddr_in peer_name;
	struct timespec accept;
	struct timespec read;
	struct timespec parse_end;
	struct timespec query_begin;
	struct timespec query_end;
	struct timespec end;
#endif
} http_request_t;

typedef struct {
	uv_write_t req;
	uv_buf_t buf;
}write_req_t;

#define ASYNC_T_SIZE 30
struct AsyncT {
	public:
		//AsyncT();

	public:
		uv_async_t data;
};

template<typename Node, size_t SIZE=2>
struct WriteReq{
	uv_write_t req;
	uv_buf_t buf[SIZE];
	Node* node;
	std::shared_ptr<void> header;
	std::shared_ptr<void> event;
};

/* SendOrderListNode  begin****/
struct SendOrderListNode {
	public:
		SendOrderListNode();
		SendOrderListNode(const char* reqID, unsigned int sgid = 1);
		SendOrderListNode(const char* sessiond, int groupid, int clientid, unsigned int sgid);

		~SendOrderListNode();
		SendOrderListNode(const SendOrderListNode& r) = delete;
		SendOrderListNode& operator=(const SendOrderListNode& r) = delete; 
		SendOrderListNode(SendOrderListNode&& r);
		SendOrderListNode& operator=(SendOrderListNode&& r);

#if 0
		bool operator()(size_t sec) {
			return  sec - std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - m_stamp).count() ;
		}
#endif
		friend bool operator==(const SendOrderListNode& l, const SendOrderListNode& r);

	public:
		bool m_reqid_flag;
		/*
		 RequestStatus 
		 ACK: 1, REJECT: 2
		 SG bit2: 0: 接收者 1: 发送者
		 PM bit3: 0: 接收者 1: 发送者
		 TE bit4: 0: 接收者 1: 发送者
		 MT bit5: 0: 接收者 1: 发送者
		 CM bit6: 0: 接收者 1: 发送者
		 */
		unsigned char m_pm_user: 1; /*PM -> SG: user.sgid */ 
		unsigned char m_te_user: 1; /*TE -> SG: user.sgid */ 
		unsigned char m_mt_user: 1; /* MT -> SG: user.sgid */ 
		unsigned char m_pm_orderans: 1; /*PM -> SG: orderans.sgig: 1: invaliad, 0: valid*/ 
		unsigned char m_te_orderans_openprice: 1; /*TE -> SG: orderans.sgid: openpice */ 
		unsigned char m_te_orderans_closeprice: 1; /*TE -> SG: orderans.sgid closeprice */ 
		unsigned char : 0; /*forced alignment */ 
		unsigned char m_pm_valid: 2; /*pm -> SG: 1: openprice validate 2:closeprice validate*/ 
		unsigned char m_cm_login: 2; /* cm->sg login.clientmangent: login status: 1->success 0: fail*/
		unsigned char m_cm_logout: 2; /* cm->sg login.clientmangent*/
		unsigned char : 0; /*forced alignment */ 
		unsigned int m_sgID;
		char* m_reqID;
		char* m_sessionID;
		int m_groupID;
		int m_clientID;
		http_request_t* m_http_request;

		//unsigned int m_stamp; /*send order unix time stamp */
		unsigned int m_expiryPeriod;
		//std::chrono::system_clock::time_point m_openTime;
		long m_openTime;
		//std::chrono::system_clock::time_point m_closeTime;
		//std::chrono::system_clock::time_point m_stamp;
		long m_stamp;
};
/* SendOrderListNode  end****/

/* SessionListNode begin */
struct SessionListNode {
	public:
		SessionListNode();
		SessionListNode(const char* sessiond, int groupid = 0, int clientid = 0, unsigned int sgid = 0);

		~SessionListNode();
		SessionListNode(const SessionListNode& r) = delete;
		SessionListNode& operator=(const SessionListNode& r) = delete; 

		SessionListNode(SessionListNode&& r);
		SessionListNode& operator=(SessionListNode&& r);

		const char* ToJson();

		friend bool operator==(const SessionListNode& l, const SessionListNode& r);

	public:
		int m_status;
		int m_sgID;
		char* m_sessionID;
		char* m_json;
		int m_groupID;
		int m_clientID;
		http_request_t* m_http_request;
		long m_loginTime;
};
/* SessionListNode end */
#endif

