/*************************************************************************
	> File Name: solution_subscribe.cc
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Tue 28 Feb 2017 07:56:30 PM CST
 ************************************************************************/
#include"solution_subscribe.h"
#include<mutex>
#include<string.h>
#include<type_traits>
#include"tnode_adapter.h"
#include"define.h"
#include"ThreadPool.h"
#include<rapidjson/document.h>
#include<rapidjson/writer.h>
#include<rapidjson/rapidjson.h>
#include<rapidjson/stringbuffer.h>
#include<uv.h>

#include"order_list.h"
#include"bithack.h"
#include"connection.h"
//#include"mysqlop.h"
//#include"object_pool.h"
//#include"redisop.h"
#include"object_pool.h"
#include"safe_queue.h"
#include"order_list.h"
#include"solution_config.h"
#include"xdatetime.h"

#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

using namespace snetwork_xservice_xflagger;
using namespace snetwork_xservice_solutiongateway;
//using RedisPool = ObjectPool<snetwork_xservice_db::Redis, 5>;
//extern RedisPool* g_redisPool;
//SafeList<SendOrderListNode> g_solution_list;
SafeList<SessionListNode> g_solution_list;
extern ThreadPool* g_threadPool;
extern SXConfig* sxconfig;
const static size_t headerSize = sizeof(HeaderTag);
const static size_t writeReqSize = sizeof(WriteReq<SessionListNode, 2>);

extern uv_mutex_t g_uv_mutex;
extern uv_sem_t g_uv_sem;
extern uv_async_t g_uv_async;

using AsyncTPool = ObjectPool<AsyncT, ASYNC_T_SIZE>;
extern AsyncTPool* g_asyncTPool;
SafeQueue<AsyncT*>  g_asyncTQueue;

static void AsyncSend(std::shared_ptr<void> event, unsigned int eventLen, ETag tag, SessionListNode* node) {
	//WriteReq<SessionListNode, 2>* writeReq = (WriteReq<SessionListNode, 2>*)malloc(writeReqSize);
	WriteReq<SessionListNode, 2>* writeReq = new WriteReq<SessionListNode, 2>;
	std::shared_ptr<void> header(malloc(headerSize), [](void* p) {
		free(p);
	});

	HeaderTag* headerTag = (HeaderTag*)header.get();
	headerTag->head = 0x8F;
	//headerTag->tag = htons(ETag::EVALIDATEORDER);
	headerTag->tag = htons(tag);
	headerTag->length = htonl(eventLen);
	writeReq->buf[0].base = (char*)header.get();
	writeReq->buf[0].len = headerSize;

	SimpleXorEncrypt((char*)event.get(), eventLen, XOR_BASE);
	writeReq->buf[1].base = (char*)event.get();
	writeReq->buf[1].len = eventLen;
	writeReq->node = node;
	writeReq->header = header;
	writeReq->event = event;

	AsyncT* asyncT = g_asyncTPool->GetObject();
	asyncT->data.data = (void*)writeReq;
	g_asyncTQueue.Push(asyncT);
	uv_async_send(&asyncT->data);
}

/*SubscribeUser begin ****/
SubscribeUser::SubscribeUser(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeUser::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("msg:%s, len:%d\n", (char*)event.get(), eventLen);
#endif
	//free(event);
}
/* SubscribeUser end ****/

/*SubscribeValid begin ****/
SubscribeValid::SubscribeValid(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeValid::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("msg:%s, len:%d\n", (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() ||
		!d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();

	if(((it = d.FindMember("sessionid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* sessionid = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("pmid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int pmid = it->value.GetInt();
	char pmidTmp[10];
	sprintf(pmidTmp, "%d", pmid);

	if(((it = d.FindMember("orderstatus")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int orderstatus = it->value.GetInt();

	/*publish ACK: SYSTEM.POSITION */
	RequestAck ack;
	memcpy(ack.ReqID, reqid, REQUEST_ID_LEN);
	ack.SGID_TEID = sgid;
	if (B_IS_SET(orderstatus, 7)) { /* close price */
		ack.ReqStatus = (ACK_BIT_FLAG | SG_BIT_FLAG | PRICE_BIT_FLAG);
	} else {
		ack.ReqStatus = (ACK_BIT_FLAG | SG_BIT_FLAG);
	}
	const char* msg_ack = ack.ToJson();

#if defined(DEBUG)
	XINFO("publish ACK|SG->PM", msg_ack);
#endif

	TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_SYSTEM,
			//SOLUTIONGATEWAY_PUBLISH_SYSTEM_EVENT,
			pmidTmp,
			msg_ack, 
			strlen(msg_ack)+1, 
			ETag::EREQUESTACK);

	ValidateOrder validateOrder;
	memcpy(validateOrder.ReqID, reqid, REQUEST_ID_LEN);
	memcpy(validateOrder.SessionID, sessionid, SESSION_ID_LEN);
	validateOrder.OrderStatus = orderstatus;
  	if(((it = d.FindMember("groupid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	validateOrder.GroupID = it->value.GetInt();
	validateOrder.SGID = sgid;

  	if(((it = d.FindMember("clientid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	validateOrder.ClientID = it->value.GetInt();

  	if(((it = d.FindMember("accountid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	validateOrder.AccountID = it->value.GetInt();

  	if(((it = d.FindMember("boaccountid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	validateOrder.BOAccountID = it->value.GetInt();

  	if(((it = d.FindMember("assetid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	validateOrder.AssetID = it->value.GetInt();

  	if(((it = d.FindMember("direction")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	validateOrder.Direction = it->value.GetInt();

  	if(((it = d.FindMember("expiryperiod")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	validateOrder.ExpiryPeriod = it->value.GetInt();

  	if(((it = d.FindMember("amount")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	validateOrder.Amount = it->value.GetInt();

  	if(((it = d.FindMember("margin")) == d.MemberEnd()) || !it->value.IsInt64()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	validateOrder.Margin = it->value.GetInt64();

  	if(((it = d.FindMember("freemargin")) == d.MemberEnd()) || !it->value.IsInt64()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	validateOrder.FreeMargin = it->value.GetInt64();

  	if(((it = d.FindMember("profit")) == d.MemberEnd()) || !it->value.IsInt64()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	validateOrder.Profit = it->value.GetInt64();

  	if(((it = d.FindMember("prevbalance")) == d.MemberEnd()) || !it->value.IsInt64()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	validateOrder.PrevBalance = it->value.GetInt64();

  	if(((it = d.FindMember("changed")) == d.MemberEnd()) || !it->value.IsInt64()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	validateOrder.Changed = it->value.GetInt64();

  	if(((it = d.FindMember("closebalance")) == d.MemberEnd()) || !it->value.IsInt64()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	validateOrder.CloseBalance = it->value.GetInt64();

  	if(((it = d.FindMember("status")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	validateOrder.Status = it->value.GetInt();


	SessionListNode* node = new SessionListNode(sessionid, validateOrder.GroupID, validateOrder.ClientID, validateOrder.SGID);
	PositionAccount userInfo(validateOrder);
	if (0 == validateOrder.OrderStatus) {
		userInfo.Type = EPDEFAULT;
	} else {
		userInfo.Type = EPCLOSE;
	}

	const char* userInfoMsg = userInfo.ToJson();
	int validateLen = strlen(userInfoMsg) + 1 ;
#if 0
	char* buf1 = (char*)calloc(1, validateLen);
	strcpy(buf1, userInfoMsg);
#endif
	std::shared_ptr<void> pUserInfo (malloc(validateLen), free);
	memcpy(pUserInfo.get(), userInfoMsg, validateLen);

	TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_USERINFO_NAME,
												   SOLUTIONGATEWAY_PUBLISH_RISKMANAGEMENT_EVENT,
												   userInfoMsg, 
												   validateLen, 
												   ETag::EVALIDATEORDER);

	AsyncSend(pUserInfo, validateLen, ETag::EVALIDATEORDER, node);
#if 0
	WriteReq<SessionListNode, 2>* writeReq = (WriteReq<SessionListNode, 2>*)malloc(writeReqSize);
	HeaderTag* headerTag = (HeaderTag*)malloc(headerSize);

	headerTag->head = 0x8F;
	headerTag->tag = htons(ETag::EVALIDATEORDER);
	headerTag->length = htonl(validateLen);
	writeReq->buf[0].base = (char*)headerTag;
	writeReq->buf[0].len = headerSize;
	writeReq->buf[1].base = buf1;
	writeReq->buf[1].len = validateLen;
	writeReq->node = node;

	AsyncT* asyncT = g_asyncTPool->GetObject();
	asyncT->data.data = (void*)writeReq;
	g_asyncTQueue.Push(asyncT);
	uv_async_send(&asyncT->data);

	/* NOTE: should be free memory*/
	//free(event);
#endif
}
/*SubscribeValid end ****/

/*SubscribeAns begin ****/
SubscribeAns::SubscribeAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("msg:%s, len:%d\n", (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() ||
		!d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();

  	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int sgid = it->value.GetInt();

  	if(((it = d.FindMember("teid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int teid = it->value.GetInt();
	char teidTmp[10];
	memset(teidTmp, 0, sizeof(teidTmp));
	sprintf(teidTmp, "%d", teid);

	if(((it = d.FindMember("orderstatus")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int orderStatus = it->value.GetInt();

	/*publish ACK: USER.SGID */
	RequestAck ack;
	memcpy(ack.ReqID, reqid, REQUEST_ID_LEN);
	if (B_IS_SET(orderStatus, 7)) /* close price */ {
		ack.ReqStatus = (ACK_BIT_FLAG | SG_BIT_FLAG | PRICE_BIT_FLAG);
	} else { /* openprice */
		ack.ReqStatus = (ACK_BIT_FLAG | SG_BIT_FLAG);
	}

	if (B_IS_SET(orderStatus, 1)) {
		/* invalidate */
		ack.SGID_TEID=sgid;
		const char* msg_ack = ack.ToJson();
		TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_SYSTEM,
													   SOLUTIONGATEWAY_PUBLISH_SYSTEM_EVENT,
													   msg_ack, 
													   strlen(msg_ack)+1, 
													   ETag::EREQUESTACK);
#if defined(DEBUG)
		XINFO("publish ACK|SG->PM=%s", msg_ack);
#endif
	} else {
		ack.SGID_TEID=teid;
		const char* msg_ack = ack.ToJson();
		TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_TRADINGENGIN,
													   teidTmp,
													   msg_ack, 
													   strlen(msg_ack)+1, 
													   ETag::EREQUESTACK);
#if defined(DEBUG)
		XINFO("publish ACK|SG->TE=%s", msg_ack);
#endif
	}

	if(((it = d.FindMember("sessionid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* sessionid = it->value.GetString();

  	if(((it = d.FindMember("groupid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int groupid = it->value.GetInt();

  	if(((it = d.FindMember("clientid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int clientid = it->value.GetInt();

	//SendOrderListNode node(reqid, sgid);
	SessionListNode* node = new SessionListNode(sessionid, groupid, clientid, sgid);
	AsyncSend(event, eventLen, ETag::EORDERANS, node);
#if 0
	WriteReq<SessionListNode, 2>* writeReq = (WriteReq<SessionListNode, 2>*)malloc(writeReqSize);
	HeaderTag* headerTag = (HeaderTag*)malloc(headerSize);

	headerTag->head = 0x8F;
	headerTag->tag = htons(ETag::EORDERANS);
	headerTag->length = htonl(eventLen);
	writeReq->buf[0].base = (char*)headerTag;
	writeReq->buf[0].len = headerSize;
	writeReq->buf[1].base = event;
	writeReq->buf[1].len = eventLen;
	writeReq->node = node;

	AsyncT* asyncT = g_asyncTPool->GetObject();
	asyncT->data.data = (void*)writeReq;
	g_asyncTQueue.Push(asyncT);
	uv_async_send(&asyncT->data);
#endif

#if defined(DEBUG)
	//fprintf(stderr, "line:%d|fun:%s\n", __LINE__, __FUNCTION__);
#endif
}
/*SubscribeAns end ****/

/*SubscribeLogin begin ****/
SubscribeLogin::SubscribeLogin(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeLogin::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("msg:%s, len:%d\n", (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() ||
		!d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;

  	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int sgid = it->value.GetInt();

  	if(((it = d.FindMember("groupid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int groupid = it->value.GetInt();

  	if(((it = d.FindMember("clientid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int clientid = it->value.GetInt();

  	if(((it = d.FindMember("status")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int status = it->value.GetInt();

  	if(((it = d.FindMember("sessionid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* sessionid = it->value.GetString();

	SessionListNode* node = new SessionListNode(sessionid, groupid, clientid, sgid);

	if (status != ELogin::ELOGINSUCCESS) {
		/* invalid login, should close tcp */
		g_solution_list.Update(*node, false, [&](SessionListNode& dst) {
			/*respone http request*/
			dst.m_groupID = groupid;
			dst.m_status = ELogin::ELOGININVALID;
	});
	} else {
		g_solution_list.Update(*node, false, [&](SessionListNode& dst) {
			dst.m_groupID = groupid;
			dst.m_status = ELogin::ELOGINSUCCESS;
	});
	}

	AsyncSend(event, eventLen, ETag::ELOGINANS, node);
#if 0
	WriteReq<SessionListNode, 2>* writeReq = (WriteReq<SessionListNode, 2>*)malloc(writeReqSize);
	HeaderTag* headerTag = (HeaderTag*)malloc(headerSize);

	headerTag->head = 0x8F;
	headerTag->tag = htons(ETag::ELOGINANS);
	headerTag->length = htonl(eventLen);
	writeReq->buf[0].base = (char*)headerTag;
	writeReq->buf[0].len = headerSize;
	writeReq->buf[1].base = event;
	writeReq->buf[1].len = eventLen;
	writeReq->node = node;

	AsyncT* asyncT = g_asyncTPool->GetObject();
	asyncT->data.data = (void*)writeReq;
	g_asyncTQueue.Push(asyncT);
	uv_async_send(&asyncT->data);
#endif
#if defined(DEBUG)
	//fprintf(stderr, "line:%d|fun:%s\n", __LINE__, __FUNCTION__);
#endif
}
/*SubscribeLogin end ****/

/*SubscribeLogout begin ****/
SubscribeLogout::SubscribeLogout(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeLogout::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("msg:%s, len:%d\n", (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() ||
		!d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();

	if(((it = d.FindMember("sessionid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* sessionid = it->value.GetString();

  	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("groupid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int groupid = it->value.GetInt();

	if(((it = d.FindMember("clientid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int clientid = it->value.GetInt();

	if(((it = d.FindMember("status")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int status = it->value.GetInt();

	SessionListNode* node = new SessionListNode(sessionid, groupid, clientid, sgid);
	node->m_status = ELogin::ELOGINLOGOUT;
	g_solution_list.Update(*node, false, [&](SessionListNode& dst) {
		dst.m_status = ELogin::ELOGINLOGOUT;
	});
	AsyncSend(event, eventLen, ETag::ELOGOUTANS, node);
#if 0
	SessionListNode node(sessionid, groupid, clientid, sgid);
	node.m_status = ELogin::ELOGINLOGOUT;
	if (status != ELogin::ELOGINSUCCESS) {
		/* invalid logout, should close tcp */
		g_solution_list.Update(node, false, [&](SessionListNode& dst) {
			/*respone http request*/
			http_request_t* request = dst.m_http_request;
			if (request == nullptr) {
				return;
			}

			if (0 == uv_is_writable((uv_stream_t*)&request->stream)) {
				XWARNING("Stream was disconnect");
				dst.m_status = ELogin::ELOGININVALID;

				return;
			}

			if (uv_is_active((uv_handle_t*)&request->stream)) {
				uv_read_stop((uv_stream_t*)&request->stream);
			}

			if (uv_is_closing((uv_handle_t*)&request->stream) == 0) {
				/* return non-zero if handle is closing or closed, zero otherwise*/
				uv_close((uv_handle_t*)&request->stream, NULL);
			}
			dst.m_status = ELogin::ELOGINCLOSE;
	});
	} else {
		/* logout success */
		g_solution_list.Update(node, false, [&](SessionListNode& dst) {
			dst.m_status = ELogin::ELOGINLOGOUT;
			if (dst.m_http_request != nullptr) {
				uv_close((uv_handle_t*)&dst.m_http_request->stream, UVTcpServer::OnClose);
			}
	});
	}
#endif
}
/*SubscribeLogout end ****/

/*SubscribeUserInfo begin ****/
SubscribeUserInfo::SubscribeUserInfo(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeUserInfo::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("msg:%s, len:%d\n", (char*)event.get(), eventLen);
#endif
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() ||
		!d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}
	rapidjson::Value::ConstMemberIterator it;
	PositionAccount account_info;

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();

	if(((it = d.FindMember("sessionid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* sessionid = it->value.GetString();

  	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int sgid = it->value.GetInt();

	//size_t reqLen = strlen(reqid);
	memcpy(account_info.ReqID, reqid, REQUEST_ID_LEN);
  	if(((it = d.FindMember("groupid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	account_info.GroupID = it->value.GetInt();

  	if(((it = d.FindMember("clientid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	account_info.ClientID = it->value.GetInt();

	SessionListNode*  node = new SessionListNode(sessionid, account_info.GroupID, account_info.ClientID, sgid);
	AsyncSend(event, eventLen, ETag::EUSERINFON, node);
#if 0
	WriteReq<SessionListNode, 2>* writeReq = (WriteReq<SessionListNode, 2>*)malloc(writeReqSize);
	HeaderTag* headerTag = (HeaderTag*)malloc(headerSize);

	headerTag->head = 0x8F;
	headerTag->tag = htons(ETag::EUSERINFON);
	headerTag->length = htonl(eventLen);
	writeReq->buf[0].base = (char*)headerTag;
	writeReq->buf[0].len = headerSize;
	writeReq->buf[1].base = event;
	writeReq->buf[1].len = eventLen;
	writeReq->node = node;

	AsyncT* asyncT = g_asyncTPool->GetObject();
	asyncT->data.data = (void*)writeReq;
	g_asyncTQueue.Push(asyncT);
	uv_async_send(&asyncT->data);
#endif
}
/*SubscribeUserInfo end ****/

/*SubscribeUserInfo begin ****/
SubscribeOCPosition::SubscribeOCPosition(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeOCPosition::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("msg:%s, len:%d\n", (char*)event.get(), eventLen);
#endif
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() ||
		!d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;

	if(((it = d.FindMember("sessionid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* sessionid = it->value.GetString();

	if(((it = d.FindMember("groupid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int groupid = it->value.GetInt();

	if(((it = d.FindMember("clientid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int clientid = it->value.GetInt();

  	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int sgid = it->value.GetInt();

	SessionListNode* node = new SessionListNode(sessionid, groupid, clientid, sgid);
	AsyncSend(event, eventLen, ETag::EOCPOSITION, node);
#if 0
	WriteReq<SessionListNode, 2>* writeReq = (WriteReq<SessionListNode, 2>*)malloc(writeReqSize);
	HeaderTag* headerTag = (HeaderTag*)malloc(headerSize);

	headerTag->head = 0x8F;
	headerTag->tag = htons(ETag::EOCPOSITION);
	headerTag->length = htonl(eventLen);
	writeReq->buf[0].base = (char*)headerTag;
	writeReq->buf[0].len = headerSize;
	writeReq->buf[1].base = event;
	writeReq->buf[1].len = eventLen;
	writeReq->node = node;

	AsyncT* asyncT = g_asyncTPool->GetObject();
	asyncT->data.data = (void*)writeReq;
	g_asyncTQueue.Push(asyncT);
	uv_async_send(&asyncT->data);
#endif
}
/*SubscribeOCPosition end ****/

/*SubscribeForceLogout begin ****/
SubscribeForceLogout::SubscribeForceLogout(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeForceLogout::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("msg:%s, len:%d\n", (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() ||
		!d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();

	if(((it = d.FindMember("sessionid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* sessionid = it->value.GetString();

  	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("groupid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int groupid = it->value.GetInt();

	if(((it = d.FindMember("clientid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int clientid = it->value.GetInt();

	if(((it = d.FindMember("status")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int status = it->value.GetInt();

	SessionListNode* node = new SessionListNode(sessionid, groupid, clientid, sgid);
	node->m_status = ELogin::ELOGINLOGOUT;
	g_solution_list.Update(*node, false, [&](SessionListNode& dst) {
		dst.m_status = ELogin::ELOGINLOGOUT;
	});
	AsyncSend(event, eventLen, ETag::EFORCELOGOUTANS, node);
#if 0
	WriteReq<SessionListNode, 2>* writeReq = (WriteReq<SessionListNode, 2>*)malloc(writeReqSize);
	HeaderTag* headerTag = (HeaderTag*)malloc(headerSize);

	headerTag->head = 0x8F;
	headerTag->tag = htons(ETag::EFORCELOGOUTANS);
	headerTag->length = htonl(eventLen);
	writeReq->buf[0].base = (char*)headerTag;
	writeReq->buf[0].len = headerSize;
	writeReq->buf[1].base = event;
	writeReq->buf[1].len = eventLen;
	writeReq->node = node;

	AsyncT* asyncT = g_asyncTPool->GetObject();
	asyncT->data.data = (void*)writeReq;
	g_asyncTQueue.Push(asyncT);
	uv_async_send(&asyncT->data);
#endif

#if 0
	if (status != ELogin::ELOGINSUCCESS) {
		/* invalid logout, should close tcp */
		g_solution_list.Update(node, false, [&](SessionListNode& dst) {
			/*respone http request*/
			http_request_t* request = dst.m_http_request;
			/*If stream is writeable */
			if (0 == uv_is_writable((uv_stream_t*)&request->stream)) {
				XWA RNING("Stream was disconnect");
				//dst.m_status = ELogin::ELOGINUNWRITE;
				dst.m_status = ELogin::ELOGININVALID;
				return;
			}

			write_req_t* req = (write_req_t*)malloc(sizeof(write_req_t));
			size_t size = sizeof(HeaderTag);
			char* response = (char*)calloc(1, eventLen + size);
			memcpy(response, reinterpret_cast<char*>(&headerTag), size);
			memcpy(&response[size], event, eventLen);
			req->buf = uv_buf_init(response, size + eventLen);
			//uv_write(&req->req, (uv_stream_t*)&request->stream, &req->buf, 1, snetwork_xservice_solutiongateway::UVTcpServer::WriteCB);
			uv_write((uv_write_t*)req, (uv_stream_t*)&request->stream, &req->buf, 1, [](uv_write_t* req, int status) {
				write_req_t* wr = (write_req_t*) req;
				int tag = ntohs(*(unsigned short*)&wr->buf.base[2]);
				XINFO("SG->TT\ttag=0x%02X, status=%d|msg=%s",tag, status, &wr->buf.base[8]);

				free(wr->buf.base);
				free(wr);

			});

			if (uv_is_active((uv_handle_t*)&request->stream)) {
				uv_read_stop((uv_stream_t*)&request->stream);
			}
			//uv_close((uv_handle_t*)&request->stream, NULL);
			if (uv_is_closing((uv_handle_t*)&request->stream) == 0) {/* return non-zero if handle is closing or closed, zero otherwise*/
				uv_close((uv_handle_t*)&request->stream, NULL);
			}
			//dst.m_status = ELogin::ELOGININVALID;
			dst.m_status = ELogin::ELOGINCLOSE;
	});
	} else {
		/* logout success */
		g_solution_list.Update(node, false, [&](SessionListNode& dst) {
			/*respone http request*/
			http_request_t* request = dst.m_http_request;
			/*If stream is writeable */
			if (0 == uv_is_writable((uv_stream_t*)&request->stream)) {
				XWARNING("Stream was disconnect");
				dst.m_status = ELogin::ELOGININVALID;
				return;
			}

			write_req_t* req = (write_req_t*)malloc(sizeof(write_req_t));
			size_t size = sizeof(HeaderTag);
			char* response = (char*)calloc(1, eventLen + size);
			memcpy(response, reinterpret_cast<char*>(&headerTag), size);
			memcpy(&response[size], event, eventLen);
			req->buf = uv_buf_init(response, size + eventLen);
			//uv_write(&req->req, (uv_stream_t*)&request->stream, &req->buf, 1, [](uv_write_t* req, int status) {
			uv_write((uv_write_t*)req, (uv_stream_t*)&request->stream, &req->buf, 1, [](uv_write_t* req, int status) {
				write_req_t* wr = (write_req_t*) req;
				int tag = ntohs(*(unsigned short*)&wr->buf.base[2]);
				XINFO("SG->TT\ttag=0x%02X, status=%d|msg=%s",tag, status, &wr->buf.base[8]);

				free(wr->buf.base);
				free(wr);
			});
			
			if (uv_is_active((uv_handle_t*)&request->stream)) {
				uv_read_stop((uv_stream_t*)&request->stream);
			}

			//uv_close((uv_handle_t*)&request->stream, NULL);
			if (uv_is_closing((uv_handle_t*)&request->stream) == 0) {/* return non-zero if handle is closing or closed, zero otherwise*/
				uv_close((uv_handle_t*)&request->stream, NULL);
			}
			//dst.m_status = ELogin::ELOGINCLOSE;
			dst.m_status = ELogin::ELOGINLOGOUT;
	});

	}
#endif
}
/*SubscribeForceLogout end ****/

/*SubscribeChangePWD begin ****/
SubscribeChangePWD::SubscribeChangePWD(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeChangePWD::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("msg:%s, len:%d\n", (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() ||
		!d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();

	if(((it = d.FindMember("sessionid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* sessionid = it->value.GetString();

  	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("groupid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int groupid = it->value.GetInt();

	if(((it = d.FindMember("clientid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int clientid = it->value.GetInt();

	if(((it = d.FindMember("status")) == d.MemberEnd()) || !it->value.IsBool()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	bool status = it->value.GetBool();

	SessionListNode* node = new SessionListNode(sessionid, groupid, clientid, sgid);
	AsyncSend(event, eventLen, ETag::ECHANGEPWDANS, node);
}
/*SubscribeChangePWD end */

/*SubscribeTradeConditions begin ****/
SubscribeTradeConditions::SubscribeTradeConditions(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeTradeConditions::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("msg:%s, len:%d\n", (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() ||
		!d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();

	if(((it = d.FindMember("sessionid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* sessionid = it->value.GetString();

  	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("groupid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int groupid = it->value.GetInt();

	if(((it = d.FindMember("clientid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int clientid = it->value.GetInt();

	SessionListNode* node = new SessionListNode(sessionid, groupid, clientid, sgid);
	AsyncSend(event, eventLen, ETag::ETRADECONDITIONS, node);
}
/*SubscribeTradeConditions end */

/* SubscribeEchoReq begin ****/
SubscribeEchoReq::SubscribeEchoReq(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeEchoReq::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("msg:%s, len:%d\n", (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() ||
		!d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
            XINFO("field no exit or type error");
#endif

			return;
	}
	const char* reqid = it->value.GetString();

	if(((it = d.FindMember("mid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
            XINFO("field no exit or type error");
#endif

			return;
	}
	int mid = it->value.GetInt();

	if(((it = d.FindMember("timestamp")) == d.MemberEnd()) || !it->value.IsInt64()) {
#if defined(DEBUG)
            XINFO("field no exit or type error");
#endif

			return;
	}
	uint64_t timestamp = it->value.GetInt64();

	std::string moduleName = std::string("SG") + UVTcpServer::GetModuleIDStr();
	MonitorAns monitorAns(reqid, 3, moduleName.c_str(), getpid(), UVTcpServer::GetModuleID(), GetMicroSecond(), timestamp);
	DecoMonitorAns decoMonitorAns(monitorAns, UVTcpServer::GetSockIP(), UVTcpServer::GetPort(), g_solution_list.Length());
	const char* monitorAck = decoMonitorAns.ToJson();

#if defined(DEBUG)
	XINFO("publish ACK| SG->MT", monitorAck);
#endif
	TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_MONITOR_NAME, 
														std::to_string(mid).c_str(),
														monitorAck,
														strlen(monitorAck)+1,
														ETag::EMONTORANS);
	/* take care for this, because response isnot to TT,
	 * it is response to MT, so should release memory */
	//free(event);
}
