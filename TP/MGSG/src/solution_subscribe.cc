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
#include <syscall.h>

#include"session_list.h"
#include"connection.h"

#include"object_pool.h"
#include"safe_queue.h"
#include"solution_config.h"
// #include"xdatetime.h"

#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

using namespace snetwork_xservice_xflagger;

SafeList<CMTSessionListNode> g_solution_list;
extern ThreadPool* g_threadPool;
extern SXConfig* sxconfig;

const static size_t headerSize = sizeof(HeaderTag);
const static size_t writeReqSize = sizeof(WriteReq<CMTSessionListNode, 2>);

using AsyncTPool = ObjectPool<AsyncT, ASYNC_T_SIZE>;
extern AsyncTPool* g_asyncTPool;
SafeQueue<AsyncT*>  g_asyncTQueue;

void SendAckBySession(CMTSessionListNode& node, const char* event, unsigned int eventLen, ETag tag) {
	/*respone tcp request*/
	uv_stream_t* stream = node.m_stream;
	/*If stream is writeable */
	if (0 == uv_is_writable((uv_stream_t*)stream) ||
		0 == uv_is_active((uv_handle_t*)stream)) {
		XWARNING("Stream was disconnect");
		node.m_status = ELogin::ELOGINUNWRITE;

		return;
	}

	write_req_t* req = (write_req_t*)malloc(sizeof(write_req_t));
	HeaderTag headerTag;
	headerTag.tag = htons(tag);
	headerTag.length = htonl(eventLen);
	size_t size = sizeof(HeaderTag);
	char* response = (char*)calloc(1, eventLen + size);
	memcpy(response, reinterpret_cast<char*>(&headerTag), size);
	memcpy(&response[size], event, eventLen);
	req->buf = uv_buf_init(response, size + eventLen);

	if (0 == uv_is_writable((uv_stream_t*)stream) ||
		0 == uv_is_active((uv_handle_t*)stream)) {
		XWARNING("Stream was disconnect");
		node.m_status = ELogin::ELOGINUNWRITE;

		free(req->buf.base);
		free(req);

		return;
	}

	uv_write(&req->req, (uv_stream_t*)stream, &req->buf, 1, [](uv_write_t* req, int status) {
		write_req_t* wr = (write_req_t*)req;
		int tag = ntohs(*(unsigned short*)&wr->buf.base[2]);
		XINFO("MGSG->CMT\ttag=0x%02X, status=%d|msg=%s",tag, status, &wr->buf.base[8]);

		free(wr->buf.base);
		free(wr);
	});
}

void AsynSend(CMTSessionListNode* node, char* event, unsigned int eventLen, ETag tag) {
	WriteReq<CMTSessionListNode, 2>* writeReq = (WriteReq<CMTSessionListNode, 2>*)malloc(writeReqSize);
	HeaderTag* headerTag = (HeaderTag*)malloc(headerSize);

	headerTag->head = 0x8F;
	headerTag->tag = htons(tag);
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


#if defined(DEBUG)
	fprintf(stderr, "line:%d|fun:%s\n", __LINE__, __FUNCTION__);
#endif
}

void AsynSend(CMTSessionListNode* node, std::shared_ptr<void> event, unsigned int eventLen, ETag tag,
 std::shared_ptr<void> data = nullptr) {
	WriteReq<CMTSessionListNode, 2>* writeReq = new WriteReq<CMTSessionListNode, 2>;

	std::shared_ptr<void> header(malloc(headerSize), [](void* p) {
		free(p);
	});

	HeaderTag* headerTag = (HeaderTag*)header.get();
	headerTag->head = 0x8F;
	headerTag->tag = htons(tag);
	headerTag->length = htonl(eventLen);
	writeReq->buf[0].base = (char*)header.get();
	writeReq->buf[0].len = headerSize;
	writeReq->buf[1].base = (char*)event.get();
	writeReq->buf[1].len = eventLen;
	writeReq->node = node;
	writeReq->header = header;
	writeReq->event = event;
	writeReq->subscribe = data;

	AsyncT* asyncT = g_asyncTPool->GetObject();
	asyncT->data.data = (void*)writeReq;
	g_asyncTQueue.Push(asyncT);
	uv_async_send(&asyncT->data);
}

/*SubscribeLoginAns begin*/
SubscribeLoginAns::SubscribeLoginAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeLoginAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int managerid = it->value.GetInt();

  	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();


	if(((it = d.FindMember("loginstatus")) == d.MemberEnd()) || !it->value.IsBool()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	bool loginstatus = it->value.GetBool();

	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, managerid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	if(!loginstatus) {
		g_solution_list.Update(*node, false, [&](CMTSessionListNode& dst) {

			dst.m_status = ELogin::ELOGININVALID;
		});
	} else {
		g_solution_list.Update(*node, false, [&](CMTSessionListNode& dst) {

			dst.m_status = ELogin::ELOGINSUCCESS;
		});
	}
	AsynSend(node, event, eventLen, ETag::ELOGINANS);
}
/*SubscribeLoginAns end*/

/*SubscribeLogoutAns begin*/
SubscribeLogoutAns::SubscribeLogoutAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeLogoutAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* session = it->value.GetString();

  	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int managerid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();

	if(((it = d.FindMember("status")) == d.MemberEnd()) || !it->value.IsBool()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	bool status = it->value.GetBool();

	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, managerid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	if(!status) {
		g_solution_list.Update(*node, false, [&](CMTSessionListNode& dst) {

			dst.m_status = ELogin::ELOGINCLOSE;
		});
	} else {
		/* logout success */
		g_solution_list.Update(*node, false, [&](CMTSessionListNode& dst) {

			dst.m_status = ELogin::ELOGINLOGOUT;
		});
	}

	AsynSend(node, event, eventLen, ETag::ELOGOUTANS);
}
/*SubscribeLogoutAns end*/

/*SubscribeForceLogoutAns begin ****/
SubscribeForceLogoutAns::SubscribeForceLogoutAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeForceLogoutAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* session = it->value.GetString();

  	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int managerid = it->value.GetInt();

	if(((it = d.FindMember("status")) == d.MemberEnd()) || !it->value.IsBool()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	bool status = it->value.GetBool();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();

	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, managerid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);
	// if(!status) {
	// 	/* invalid logout, should close tcp */
	// 	g_solution_list.Update(*node, false, [&](CMTSessionListNode& dst) {

	// 		dst.m_status = ELogin::ELOGINCLOSE;
	// 	});
	// } else {
	// 	g_solution_list.Update(*node, false, [&](CMTSessionListNode& dst) {

	// 		dst.m_status = ELogin::ELOGINLOGOUT;
	// 	});
	// }
	//though forcelogout maybe false:1.send by main.cc ,response is false, we won't send again 2.sending caused by backend, if false,
	//we won't send again. WHY??? because backbend have dealed with forcelogout, so we needn't repeat ask it to do. But logout false,
	//we send forcelogout again , this maybe is proper. Certainly, after main.cc send forcelogout, node will be deleted soon at next 
	//step, and forcelogout and forcelogoutans won't happend again.
	g_solution_list.Update(*node, false, [&](CMTSessionListNode& dst) {

		dst.m_status = ELogin::ELOGINLOGOUT;
	});

	AsynSend(node, event, eventLen, ECMFORCELOGOUTANS);
}
/*SubscribeForceLogoutAns end ****/

/*SubscribeEchoReq begin ****/
SubscribeEchoReq::SubscribeEchoReq(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeEchoReq::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
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


	SXConfig* sxconfig = dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig());
	int modelId = atoi(sxconfig->SolutionID().c_str());
	const std::string& modelName = std::string("MGSG") + std::to_string(sxconfig->SolutionPort());

	MonitorAns monitorAns(reqid, 3, modelName.c_str(), getpid(), modelId, GetMicroSecond(), timestamp);
	const char* monitorAck = monitorAns.ToJson();

#if defined(DEBUG)
	XINFO("publish ACK| MGSG->MT", monitorAck);
#endif
	TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_MONITORANS_NAME, 
														std::to_string(mid).c_str(),
														monitorAck,
														strlen(monitorAck)+1,
														ETag::EMONTORANS);
}
/*SubscribeEchoReq end ****/

//For mgsg
/*SubscribeChangePasswordAns*/
SubscribeChangePasswordAns::SubscribeChangePasswordAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeChangePasswordAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;
	}
	int managerid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* reqid = it->value.GetString();

	if(((it = d.FindMember("status")) == d.MemberEnd()) || !it->value.IsBool()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	bool status = it->value.GetBool();

	if(((it = d.FindMember("password")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* password = it->value.GetString();

	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, managerid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, ECHANGEPASSWORDANS);
}
/*SubscribeChangePasswordAns end*/

/*SubscribeChangeBalanceAns begin*/
SubscribeChangeBalanceAns::SubscribeChangeBalanceAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeChangeBalanceAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int managerid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();

	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, managerid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, ECHANGEBALANCEANS);

}
/*SubscribeChangeBalanceAns end*/

/*SubscribeUpdateTraderAns begin*/
SubscribeUpdateTraderAns::SubscribeUpdateTraderAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeUpdateTraderAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int managerid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, managerid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EUPDATETRADERANS);
}
/*SubscribeUpdateTraderAns end*/


/*SubscribeGetTradersAns begin*/
SubscribeGetTradersAns::SubscribeGetTradersAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeGetTradersAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int managerid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, managerid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EGETTRADERSANS);
}
/*SubscribeGetTradersAns end*/



/*SubscribeInsertUpdateManabegingerRightsAns */
SubscribeInsertUpdateManagerRightsAns::SubscribeInsertUpdateManagerRightsAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeInsertUpdateManagerRightsAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int managerid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, managerid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EINSERTUPDATEMANAGERRIGHTSANS);
}
/*SubscribeInsertUpdateManagerRights end*/

/*SubscribeGetManagerAccessAns begin*/
SubscribeGetManagerAccessAns::SubscribeGetManagerAccessAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeGetManagerAccessAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int managerid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, managerid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EGETMANAGERACCESSANS);
}
/*SubscribeGetManagerAccessAns end*/

/*SubscribeUserDeleteCheckAns begin*/
SubscribeUserDeleteCheckAns::SubscribeUserDeleteCheckAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeUserDeleteCheckAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}


	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int managerid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, managerid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EUSERDELETECHECKANS);
}
/*SubscribeUserDeleteCheckAns */

/*SubscribeUserDeleteAns begin*/
SubscribeUserDeleteAns::SubscribeUserDeleteAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeUserDeleteAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int managerid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, managerid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EUSERDELETEANS);
}
/*SubscribeUserDeleteAns end*/

/*SubscribeCreateTraderAns begin*/
SubscribeCreateTraderAns::SubscribeCreateTraderAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeCreateTraderAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int managerid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, managerid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, ECREATETRADERANS);
}
/*SubscribeCreateTraderAns end*/

/*SubscribeGetOnlineTraderAns begin*/
SubscribeGetOnlineTraderAns::SubscribeGetOnlineTraderAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeGetOnlineTraderAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int managerid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, managerid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EGETONLINETRADERANS);
}
/*SubscribeGetOnlineTraderAns end*/

/*SubscribeDisableTraderAns begin*/
SubscribeDisableTraderAns::SubscribeDisableTraderAns(Consumer* consumer, const char* eventName, const char* groupName):
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeDisableTraderAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
	#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int managerid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, managerid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EDISABLETRADERANS);
}
/*SubscribeDisableTraderAns end*/

/*SubscribeKickoutTraderAns begin*/
SubscribeKickoutTraderAns::SubscribeKickoutTraderAns(Consumer* consumer, const char* eventName, const char* groupName):
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeKickoutTraderAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
	#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int managerid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, managerid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EKICKOUTTRADERANS);
}
/*SubscribeKickoutTraderAns end*/

/*SubscribeGetTraderByTraderIdAns begin*/
SubscribeGetTraderByTraderIdAns::SubscribeGetTraderByTraderIdAns(Consumer* consumer, const char* eventName, const char* groupName):
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeGetTraderByTraderIdAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
	#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int managerid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, managerid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EGETTRADERBYTRADERIDANS);
}
/*SubscribeGetTraderByTraderIdAns end*/

/*SubscribeGetAllGroupByManagerIdAns begin*/
SubscribeGetAllGroupByManagerIdAns::SubscribeGetAllGroupByManagerIdAns(Consumer* consumer, const char* eventName, const char* groupName):
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeGetAllGroupByManagerIdAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
	#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int managerid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, managerid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EGETALLGROUPBYMANAGERIDANS);
}
/*SubscribeGetAllGroupByManagerIdAns end*/

//for RM
/*SubscribeGetPositionsAns begin*/
SubscribeGetPositionsAns::SubscribeGetPositionsAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeGetPositionsAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int managerid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, managerid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, GETPOSITIONSANS);
}
/*SubscribeGetPositionsAns end*/

/*SubscribeGetTop5boAns begin*/
SubscribeGetTop5boAns::SubscribeGetTop5boAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeGetTop5boAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int managerid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, managerid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, GETTOP5BOANS);
}
/*SubscribeGetTop5boAns end*/

/*SubscribeGetCustomizeBoAns begin*/
SubscribeGetCustomizeBoAns::SubscribeGetCustomizeBoAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeGetCustomizeBoAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int managerid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();

	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, managerid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, GETCUSTOMIZEBOANS);
}
/*SubscribeGetCustomizeBoAns end*/

/*SubscribeGetSymbolMonitorAns begin*/
SubscribeGetSymbolMonitorAns::SubscribeGetSymbolMonitorAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeGetSymbolMonitorAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int managerid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, managerid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, GETSYMBOLMONITORANS);
}
/*SubscribeGetSymbolMonitorAns end*/

//broadcasting
/*SubscribeRiskManagementTop5Bo begin*/
SubscribeRiskManagementTop5Bo::SubscribeRiskManagementTop5Bo(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeRiskManagementTop5Bo::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}
	
	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("direction")) == d.MemberEnd()) || !it->value.IsInt()) {
	#if defined(DEBUG)	
	XINFO("field no exit or type error");
	#endif

	return;
	}
	int direction = it->value.GetInt();

	std::shared_ptr<CustomSubscribe> subscribe (new CustomSubscribe, [](CustomSubscribe* p) {
		delete p;
	});
	//subscribe.get()->direction = direction; //up or down is enough
	subscribe->direction = direction;

	// std::shared_ptr<void> subscribe (malloc(sizeof(CustomSubscribe)), [](void* p) {
	// 		free(p);
	// });
	// (CustomSubscribe*)subscribe.get()->direction = direction; //up or down is enough

	AsynSend(nullptr, event, eventLen, RISKMANAGEMENTTOP5BO, subscribe);
}
/*SubscribeRiskManagementTop5Bo end*/

/*SubscribeRiskManagementCustomBo begin*/
SubscribeRiskManagementCustomBo::SubscribeRiskManagementCustomBo(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeRiskManagementCustomBo::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("direction")) == d.MemberEnd()) || !it->value.IsInt()) {
	#if defined(DEBUG)	
	XINFO("field no exit or type error");
	#endif

	return;
	}
	int direction = it->value.GetInt();

	if(((it = d.FindMember("assetid")) == d.MemberEnd()) || !it->value.IsInt()) {
	#if defined(DEBUG)	
	XINFO("field no exit or type error");
	#endif

	return;
	}
	int assetid = it->value.GetInt();

	std::shared_ptr<CustomSubscribe> subscribe (new CustomSubscribe, [](CustomSubscribe* p) {
		delete p;
	});//up or down of one symbol 
	// subscribe.get()->direction = direction;
	// subscribe.get()->assetid = assetid;
	subscribe->direction = direction;
	subscribe->assetid = assetid;

	// std::shared_ptr<void> subscribe (malloc(sizeof(CustomSubscribe)), [](void* p) {
	// 	free(p);
	// });//up or down of one symbol 
	// (CustomSubscribe*)subscribe.get()->direction = direction;
	// (CustomSubscribe*)subscribe.get()->assetid = assetid;

	AsynSend(nullptr, event, eventLen, RISKMANAGEMENTCUSTOMBO, subscribe);
}
/*SubscribeRiskManagementCustomBo end*/

/*SubscribeRiskManagementSymbolBo begin*/
SubscribeRiskManagementSymbolBo::SubscribeRiskManagementSymbolBo(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeRiskManagementSymbolBo::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	// rapidjson::Value::ConstMemberIterator it;
	// if(((it = d.FindMember("direction")) == d.MemberEnd()) || !it->value.IsInt()) {
	// #if defined(DEBUG)	
	// XINFO("field no exit or type error");
	// #endif

	// return;
	// }
	// int direction = it->value.GetInt();

	// std::shared_ptr<void> subscribe (new CustomSubscribe, [](void* p) {
	// 	delete p;
	// });
	// (CustomSubscribe*)subscribe.get()->direction = direction; 

	AsynSend(nullptr, event, eventLen, RISKMANAGEMENTSYMBOLBO);
}
/*SubscribeRiskManagementSymbolBo end*/

/*SubscribeRMUpdateTraderBalance begin*/
SubscribeRMUpdateTraderBalance::SubscribeRMUpdateTraderBalance(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeRMUpdateTraderBalance::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}


	AsynSend(nullptr, event, eventLen, RMUPDATETRADERBALANCE);
}
/*SubscribeRMUpdateTraderBalance end*/


/*SubscribeOnlineTraderOnRoot begin*/
SubscribeOnlineTraderOnRoot::SubscribeOnlineTraderOnRoot(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeOnlineTraderOnRoot::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}


	AsynSend(nullptr, event, eventLen, EONLINETRADERONROOT);
}
/*SubscribeOnlineTraderOnRoot end*/

/*SubscribeLogoutTraderOnRoot begin*/
SubscribeLogoutTraderOnRoot::SubscribeLogoutTraderOnRoot(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeLogoutTraderOnRoot::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	AsynSend(nullptr, event, eventLen, ELOGOUTTRADERONROOT);
}
/*SubscribeLogoutTraderOnRoot end*/

/*SubscribeNewTraderOnRoot end*/
SubscribeNewTraderOnRoot::SubscribeNewTraderOnRoot(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeNewTraderOnRoot::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}


	AsynSend(nullptr, event, eventLen, ENEWTRADERONROOT);
}
/*SubscribeNewTraderOnRoot end*/

/*SubscribeUpdateTraderOnRoot end*/
SubscribeUpdateTraderOnRoot::SubscribeUpdateTraderOnRoot(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeUpdateTraderOnRoot::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}


	AsynSend(nullptr, event, eventLen, EUPDATETRADERONROOT);
}
/*SubscribeUpdateTraderOnRoot begin*/


/*SubscribeDeleteTraderOnRoot end*/
SubscribeDeleteTraderOnRoot::SubscribeDeleteTraderOnRoot(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeDeleteTraderOnRoot::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}


	AsynSend(nullptr, event, eventLen, EDELETETRADERONROOT);
}
/*SubscribeDeleteTraderOnRoot begin*/

/*SubscribeUpdateOpenTradeOnRoot begin*/
SubscribeUpdateOpenTradeOnRoot::SubscribeUpdateOpenTradeOnRoot(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeUpdateOpenTradeOnRoot::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}


	AsynSend(nullptr, event, eventLen, EUPDATEOPENTRADEONROOT);
}
/*SubscribeUpdateOpenTradeOnRoot end*/

/*SubscribeUpdateClosedTradeOnRoot begin*/
SubscribeUpdateClosedTradeOnRoot::SubscribeUpdateClosedTradeOnRoot(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeUpdateClosedTradeOnRoot::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}


	AsynSend(nullptr, event, eventLen, EUPDATECLOSEDTRADEONROOT);
}
/*SubscribeUpdateClosedTradeOnRoot end*/

//for RMX
/*SubscribeGetPositionsAnsX begin*/
SubscribeGetPositionsAnsX::SubscribeGetPositionsAnsX(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeGetPositionsAnsX::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int managerid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();

	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, managerid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EGETPOSITIONSANSX);
}
/*SubscribeGetPositionsAnsX end*/

/*SubscribeRMUpdateTraderBalanceX begin*/
SubscribeRMUpdateTraderBalanceX::SubscribeRMUpdateTraderBalanceX(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeRMUpdateTraderBalanceX::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;		
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}


	AsynSend(nullptr, event, eventLen, ERMUPDATETRADERBALANCEX);
}
/*SubscribeRMUpdateTraderBalanceX end*/

/*SubscribeUpdatePositionsAnsX begin*/
SubscribeUpdatePositionsAnsX::SubscribeUpdatePositionsAnsX(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeUpdatePositionsAnsX::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}


	AsynSend(nullptr, event, eventLen, EUPDATEPOSITIONSANSX);
}
/*SubscribeUpdatePositionsAnsX end*/

/*SubscribeOrderChangeBalance begin*/
SubscribeOrderChangeBalance::SubscribeOrderChangeBalance(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeOrderChangeBalance::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}


	AsynSend(nullptr, event, eventLen, EORDERCHANGEBALANCE);
}
/*SubscribeOrderChangeBalance end*/


//For Admin
/*SubscribeAdminChangePasswordAns begin*/
SubscribeAdminChangePasswordAns::SubscribeAdminChangePasswordAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeAdminChangePasswordAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EADMINCHANGEPASSWORDANS);
}
/*SubscribeAdminChangePasswordAns end*/

/*SubscribeCreateManagerAns */
SubscribeCreateManagerAns::SubscribeCreateManagerAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeCreateManagerAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, ECREATEMANAGERANS);
}
/*SubscribeCreateManagerAns end*/

/*SubscribeUpdateManagerAns begin*/
SubscribeUpdateManagerAns::SubscribeUpdateManagerAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeUpdateManagerAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EUPDATEMANAGERANS);
}
/*SubscribeUpdateManagerAns end*/


/*SubscribeGetAdminsAns begin*/
SubscribeGetAdminsAns::SubscribeGetAdminsAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeGetAdminsAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EGETADMINSANS);
}
/*SubscribeGetAdminsAns end*/

/*SubscribeGetManagersAns begin*/
SubscribeGetManagersAns::SubscribeGetManagersAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeGetManagersAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}


	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();


	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EGETMANAGERSANS);
}
/*SubscribeGetManagersAns */

/*SubscribeDeleteManagerAns begin*/
SubscribeDeleteManagerAns::SubscribeDeleteManagerAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeDeleteManagerAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EDELETEMANAGERANS);
}
/*SubscribeDeleteManagerAns end*/

/*SubscribeUpdateManagerRightAns begin*/
SubscribeUpdateManagerRightAns::SubscribeUpdateManagerRightAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeUpdateManagerRightAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EUPDATEMANAGERRIGHTANS);
}
/*SubscribeUpdateManagerRightAns end*/

/*SubscribeGetManagerRightAns begin*/
SubscribeGetManagerRightAns::SubscribeGetManagerRightAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeGetManagerRightAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EGETMANAGERRIGHTANS);
}
/*SubscribeGetManagerRightAns end*/

/*SubscribeEnabledIsableManagerAns begin*/
SubscribeEnabledIsableManagerAns::SubscribeEnabledIsableManagerAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeEnabledIsableManagerAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EENABLEDISABLEMANAGERANS);
}
/*SubscribeEnabledIsableManagerAns end*/

/*SubscribeDeleteAdminAns begin*/
SubscribeDeleteAdminAns::SubscribeDeleteAdminAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeDeleteAdminAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EDELETEADMINANS);
}
/*SubscribeDeleteAdminAns end*/


/*SubscribeCreateAdminAns begin*/
SubscribeCreateAdminAns::SubscribeCreateAdminAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeCreateAdminAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("superadminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int superadminid = it->value.GetInt();


	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, superadminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, ECREATEADMINANS);
}
/*SubscribeCreateAdminAns end*/

/*SubscribeUpdateAdminAns begin*/
SubscribeUpdateAdminAns::SubscribeUpdateAdminAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeUpdateAdminAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();
	
	if(((it = d.FindMember("superadminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int superadminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, superadminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EUPDATEADMINANS);
}
/*SubscribeUpdateAdminAns end*/


//for account 
/*SubscribeCreateAccountAns begin*/
SubscribeCreateAccountAns::SubscribeCreateAccountAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeCreateAccountAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, ECREATEACCOUNTANS);
}
/*SubscribeCreateAccountAns end*/

/*SubscribeUpdateAccountAns begin*/
SubscribeUpdateAccountAns::SubscribeUpdateAccountAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeUpdateAccountAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EUPDATEACCOUNTANS);
}
/*SubscribeUpdateAccountAns end*/

/*SubscribeGetAllAccountsAns begin*/
SubscribeGetAllAccountsAns::SubscribeGetAllAccountsAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeGetAllAccountsAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EGETALLACCOUNTSANS);
}
/*SubscribeGetAllAccountsAns end*/

/*


SubscribeGetAccountsByAccountIdAns begin*/
SubscribeGetAccountsByAccountIdAns::SubscribeGetAccountsByAccountIdAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeGetAccountsByAccountIdAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EGETACCOUNTSBYACCOUNTIDANS);
}
/*SubscribeGetAccountsByAccountIdAns end*/

//for group
/*SubscribeCreateGroupAns begin*/
SubscribeCreateGroupAns::SubscribeCreateGroupAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeCreateGroupAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, ECREATEGROUPANS);
}
/*SubscribeCreateGroupAns end*/

/*SubscribeUpdateGroupAns begin*/
SubscribeUpdateGroupAns::SubscribeUpdateGroupAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeUpdateGroupAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EUPDATEGROUPANS);
}
/*SubscribeUpdateGroupAns end*/

/*SubscribeGetGroupdeTailsByIdAns begin*/
SubscribeGetGroupdeTailsByIdAns::SubscribeGetGroupdeTailsByIdAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeGetGroupdeTailsByIdAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EGETGROUPDETAILSBYIDANS);
}
/*SubscribeGetGroupdeTailsByIdAns end*/

/*SubscribeGetAllGroupForGateWayAns begin*/
SubscribeGetAllGroupForGateWayAns::SubscribeGetAllGroupForGateWayAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeGetAllGroupForGateWayAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EGETALLGROUPFORGATEWAYANS);
}
/*SubscribeGetAllGroupForGateWayAns end*/

/*SubscribeGetAllGroupAns begin*/
SubscribeGetAllGroupAns::SubscribeGetAllGroupAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeGetAllGroupAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EGETALLGROUPANS);
}
/*SubscribeGetAllGroupAns end*/

/*SubscribeDeleteGroupAns begin*/
SubscribeDeleteGroupAns::SubscribeDeleteGroupAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeDeleteGroupAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EDELETEGROUPANS);
}
/*SubscribeDeleteGroupAns end*/


/*SubscribeUpdateGroupSecurityAns begin*/
SubscribeUpdateGroupSecurityAns::SubscribeUpdateGroupSecurityAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeUpdateGroupSecurityAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EUPDATEGROUPSECURITYANS);
}
/*SubscribeUpdateGroupSecurityAns end*/

/*SubscribeUpdateGroupReportAns begin*/
SubscribeUpdateGroupReportAns::SubscribeUpdateGroupReportAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeUpdateGroupReportAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EUPDATEGROUPREPORTANS);
}
/*SubscribeUpdateGroupReportAns end*/

/*SubscribeSaveManagerAgainstGroupsAns begin*/
SubscribeSaveManagerAgainstGroupsAns::SubscribeSaveManagerAgainstGroupsAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {

}

void SubscribeSaveManagerAgainstGroupsAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, ESAVEMANAGERAGAINSTGROUPSANS);
}
/*SubscribeSaveManagerAgainstGroupsAns end*/

/*SubscribeGetAllGroupsByManagerIdAns begin*/
SubscribeGetAllGroupsByManagerIdAns::SubscribeGetAllGroupsByManagerIdAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {

}

void SubscribeGetAllGroupsByManagerIdAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EGETALLGROUPSBYMANAGERIDANS);
}
/*SubscribeGetAllGroupsByManagerIdAns end*/


/*SubscribeCreateSecurityAns begin*/
SubscribeCreateSecurityAns::SubscribeCreateSecurityAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {

}

void SubscribeCreateSecurityAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, ECREATESECURITYANS);
}
/*SubscribeCreateSecurityAns end*/

/*SubscribeUpdateSecurityAns begin*/
SubscribeUpdateSecurityAns::SubscribeUpdateSecurityAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {

}

void SubscribeUpdateSecurityAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EUPDATESECURITYANS);
}
/*SubscribeUpdateSecurityAns end*/

/*SubscribeGetAllSecurityAns begin*/
SubscribeGetAllSecurityAns::SubscribeGetAllSecurityAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {

}

void SubscribeGetAllSecurityAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EGETALLSECURITYANS);
}
/*SubscribeGetAllSecurityAns end*/

/*SubscribeDeleteSecurityAns begin*/
SubscribeDeleteSecurityAns::SubscribeDeleteSecurityAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {

}

void SubscribeDeleteSecurityAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EDELETESECURITYANS);
}
/*SubscribeDeleteSecurityAns end*/

/*SubscribeCreateSymbolAns begin*/
SubscribeCreateSymbolAns::SubscribeCreateSymbolAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {

}

void SubscribeCreateSymbolAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, ECREATESYMBOLANS);
}
/*SubscribeCreateSymbolAns end*/

/*SubscribeUpdateSymbolAns begin*/
SubscribeUpdateSymbolAns::SubscribeUpdateSymbolAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {

}

void SubscribeUpdateSymbolAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();
	
	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();

	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EUPDATESYMBOLANS);
}
/*SubscribeUpdateSymbolAns end*/

/*SubscribeGetAllSymbolAns begin*/
SubscribeGetAllSymbolAns::SubscribeGetAllSymbolAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {

}

void SubscribeGetAllSymbolAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EGETALLSYMBOLANS);
}
/*SubscribeGetAllSymbolAns end*/

/*SubscribeGetSymbolByIdAns begin*/
SubscribeGetSymbolByIdAns::SubscribeGetSymbolByIdAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {

}

void SubscribeGetSymbolByIdAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();
	
	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	AsynSend(node, event, eventLen, EGETSYMBOLBYIDANS);
}
/*SubscribeGetSymbolByIdAns end*/

/*SubscribeDeleteSymbolAns begin*/
SubscribeDeleteSymbolAns::SubscribeDeleteSymbolAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {

}

void SubscribeDeleteSymbolAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();
	

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();

	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);
	
	AsynSend(node, event, eventLen, EDELETESYMBOLANS);
}
/*SubscribeDeleteSymbolAns end*/

/*SubscribeSynchDataAns begin*/
SubscribeSynchDataAns::SubscribeSynchDataAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {

}

void SubscribeSynchDataAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("filed no exit or type error\n");
#endif
		return;		
	}
	int adminid = it->value.GetInt();
	

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();

	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);
	
	AsynSend(node, event, eventLen, ESYNCHDATAANS);
}
/*SubscribeSynchDataAns end*/

/*SubscribeAdminLoginAns begin*/
SubscribeAdminLoginAns::SubscribeAdminLoginAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeAdminLoginAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* session = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int adminid = it->value.GetInt();

  	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();


	if(((it = d.FindMember("loginstatus")) == d.MemberEnd()) || !it->value.IsBool()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	bool loginstatus = it->value.GetBool();

	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);
	
	if(!loginstatus) {
		/* invalid login, should close tcp */
		g_solution_list.Update(*node, false, [&](CMTSessionListNode& dst) {

			dst.m_status = ELogin::ELOGININVALID;
		});
	} else {
		/* login success */
		g_solution_list.Update(*node, false, [&](CMTSessionListNode& dst) {
			
			dst.m_status = ELogin::ELOGINSUCCESS;
		});
	}

	AsynSend(node, event, eventLen, EADMINLOGINANS);
}
/*SubscribeAdminLoginAns end*/

/*SubscribeAdminLogoutAns begin*/
SubscribeAdminLogoutAns::SubscribeAdminLogoutAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeAdminLogoutAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* session = it->value.GetString();

  	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();

	if(((it = d.FindMember("status")) == d.MemberEnd()) || !it->value.IsBool()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	bool status = it->value.GetBool();

	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);
	
	if(!status) {
		/* invalid logout, should close tcp */
		g_solution_list.Update(*node, false, [&](CMTSessionListNode& dst) {

			dst.m_status = ELogin::ELOGINCLOSE;
		});
	} else {
		/* logout success */
		g_solution_list.Update(*node, false, [&](CMTSessionListNode& dst) {

			dst.m_status = ELogin::ELOGINLOGOUT;
		});
	}

	AsynSend(node, event, eventLen, EADMINLOGOUTANS);
}
/*SubscribeAdminLogoutAns end*/


/*SubscribeAdminForceLogoutAns begin ****/
SubscribeAdminForceLogoutAns::SubscribeAdminForceLogoutAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeAdminForceLogoutAns::Done(std::shared_ptr<void> event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("<Thread>:<%d>, msg:%s, len:%d\n", syscall(SYS_gettid), (char*)event.get(), eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse((char*)event.get(), eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", (char*)event.get());

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* session = it->value.GetString();

  	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int sgid = it->value.GetInt();

	if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int adminid = it->value.GetInt();

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();


	if(((it = d.FindMember("status")) == d.MemberEnd()) || !it->value.IsBool()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	bool status = it->value.GetBool();

	CMTSessionListNode *node = new CMTSessionListNode(session, sgid, adminid);
	memcpy(node->m_reqID, reqid, REQUEST_ID_LEN);

	// if(!status) {
	// 	/* invalid logout, should close tcp */
	// 	g_solution_list.Update(*node, false, [&](CMTSessionListNode& dst) {

	// 		dst.m_status = ELogin::ELOGINCLOSE;
	// 	});
	// } else {
	// 	/* logout success */
	// 	g_solution_list.Update(*node, false, [&](CMTSessionListNode& dst) {

	// 		dst.m_status = ELogin::ELOGINLOGOUT;
	// 	});
	// }
	g_solution_list.Update(*node, false, [&](CMTSessionListNode& dst) {

		dst.m_status = ELogin::ELOGINLOGOUT;
	});

	AsynSend(node, event, eventLen, EADFORCELOGOUTANS);
}
/*SubscribeAdminForceLogoutAns end ****/
