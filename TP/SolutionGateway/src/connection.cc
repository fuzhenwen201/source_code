/*************************************************************************
  > File Name: connection.cc
  > Author: xyz
  > Mail: xiao13149920@foxmail.com 
  > Created Time: Fri 03 Feb 2017 12:00:04 PM CST
 ************************************************************************/
#include<unistd.h>
#include<string.h>
#include<string>
#include<stdlib.h>
#include<uuid/uuid.h> /* for uuid */
#include<arpa/inet.h> /*for ntohl*/
#include<netinet/in.h> /* for sockaddr */
#include<rapidjson/document.h>
#include<rapidjson/writer.h>
#include<rapidjson/rapidjson.h>
#include<rapidjson/stringbuffer.h>
#include"connection.h"
#include"http_request.h"
#include"tnode_adapter.h"
#include"ThreadPool.h"
#include"xdatetime.h"
//#include"mysqlop.h"
//#include"object_pool.h"
//#include"redisop.h"
#include"object_pool.h"
#include"safe_queue.h"
#include"order_list.h"
#include"solution_config.h"
//#include"safe_list.h"

//extern http_parser_settings g_settings;
//extern uv_loop_t* g_loop;
//extern snetwork_xservice_solutiongateway::SafeList<snetwork_xservice_solutiongateway::SendOrderListNode> g_list;
using namespace snetwork_xservice_xflagger;
//using RedisPool = ObjectPool<snetwork_xservice_db::Redis, 5>;
//extern RedisPool* g_redisPool;
//extern SafeList<SendOrderListNode> g_solution_list;
extern SafeList<SessionListNode> g_solution_list;
extern ThreadPool* g_threadPool;

using AsyncTPool = ObjectPool<AsyncT, ASYNC_T_SIZE>;
extern AsyncTPool* g_asyncTPool;

extern SafeQueue<AsyncT*> g_asyncTQueue;
uv_async_t g_uv_async;
uv_sem_t g_uv_sem;
uv_mutex_t g_uv_mutex;

namespace snetwork_xservice_solutiongateway {
/* Tcp Server begin */
uv_loop_t* UVTcpServer::m_loop = nullptr;
HttpRequest* UVTcpServer::m_settings = nullptr;
std::once_flag UVTcpServer::m_once;
uv_buf_t UVTcpServer::m_buf;
std::map<void*, struct UVTcpServer::UV_BUF_T> UVTcpServer::m_map;
uv_tcp_t UVTcpServer::m_server;
char UVTcpServer::m_strSGID[10];
unsigned short UVTcpServer::m_port;
char UVTcpServer::m_dbName[20];
char UVTcpServer::m_strAddr[INET_ADDRSTRLEN];
int UVTcpServer::m_iSGID;
CAES UVTcpServer::m_aes;

static void hex_print(const void* pv, size_t len)
{
	const unsigned char * p = (const unsigned char*)pv;
	if (NULL == pv)
		printf("NULL");
	else
	{
		size_t i = 0;
		for (; i<len;++i)
			printf("%02X ", *p++);
	}
	printf("\n");
}

#define BUF_SIZE  1024

char* byteToHexString(const char* str, int& len) {
	static char ch[BUF_SIZE];
	memset(ch, 0, BUF_SIZE);

	int L = 0;
	L = snprintf(ch + L, BUF_SIZE - L, "%c", '[');
	
	for (int i = 0; ((i < len) && (L < BUF_SIZE)); i++) {
		L += snprintf(ch + L, BUF_SIZE - L, "%02X ", *(unsigned char*)&str[i]);
	}

	if (L < BUF_SIZE) {
		L += snprintf(ch + L, BUF_SIZE - L, "%c", ']');
	}
	len = L;

	return ch;
}


UVTcpServer::UVTcpServer() {
	Init();
}

UVTcpServer::~UVTcpServer() {
	if (m_settings != nullptr) {
		delete m_settings;
		m_settings = nullptr;
	}

	uv_loop_close(m_loop);
	int r = uv_loop_close(m_loop);
	if (r == UV_EBUSY)
	{
		XERROR("uv_loop_close error %s\n", uv_strerror(r));	/* code */
	} else {
		free(m_loop);
	}
}

int UVTcpServer::Init(void) {
	m_settings = new HttpRequest();

	unsigned char iv_dec[AES_BLOCK_SIZE];
	memset(iv_dec, 0, AES_BLOCK_SIZE);
	strcpy((char*)iv_dec, "HELLOWORLD");
	m_aes.Init(iv_dec);	

	m_loop = uv_default_loop();
	uv_tcp_init(m_loop, &m_server);
	SXConfig* sxconfig = dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig());
	memset(m_strSGID, 0, sizeof(m_strSGID));
	strncpy(m_strSGID, sxconfig->SolutionID().c_str(), 10);
	m_port = sxconfig->SolutionPort();
	m_iSGID = atoi(m_strSGID);
	memset(m_dbName, 0, sizeof(m_dbName));
	strncpy(m_dbName, sxconfig->MySqlDBName().c_str(), 20);
	//uv_ip4_addr("0.0.0.0", m_port, &m_addr);
	char ipAddrTmp[INET_ADDRSTRLEN];
	memset(ipAddrTmp, 0, INET_ADDRSTRLEN);
	GetIPAddress4(ipAddrTmp);
	uv_ip4_addr(ipAddrTmp, m_port, &m_addr);
	uv_tcp_nodelay((uv_tcp_t*)&m_server, 1);
	uv_tcp_bind(&m_server, (const struct sockaddr*)&m_addr, 0);

	uv_tcp_keepalive((uv_tcp_t*)&m_server, 1, 3600);
	uv_tcp_simultaneous_accepts((uv_tcp_t*)&m_server, 1);

#if defined(SIGNALENGINE)
	int r = uv_listen((uv_stream_t*)&m_server, SOLUTIONGATEWAY_BACKLOG, OnNewConnection);
#else
	int r = uv_listen((uv_stream_t*)&m_server, SOLUTIONGATEWAY_BACKLOG, OnNewConnectionTcp);
#endif
	//int r = uv_listen((uv_stream_t*)&m_server, SOLUTIONGATEWAY_BACKLOG, on_new_connection);
	if (r) {
		XERROR("Listen error %s\n", uv_strerror(r));

		return 1;
	}

	struct sockaddr_in sockname;
	int sockLen = sizeof(sockname);
	memset(&sockname, 0, sockLen);
	uv_tcp_getsockname(&m_server, (struct sockaddr*)&sockname, &sockLen);
	inet_ntop(AF_INET, &sockname.sin_addr, m_strAddr, INET_ADDRSTRLEN);

	AsyncT** asyncT = new AsyncT*[ASYNC_T_SIZE];
	for (int i=0; i<ASYNC_T_SIZE; ++i) {
		asyncT[i] = g_asyncTPool->GetObject();
		uv_async_init(m_loop, &asyncT[i]->data, WriteReqCB);
	}

	for (int i=0; i<ASYNC_T_SIZE; ++i) {
		g_asyncTPool->GetInstance()->ReleaseObject(asyncT[i]);
	}
	delete[] asyncT;

	return 0;
}

void UVTcpServer::WriteReqCB(uv_async_t* handle) {
	//WriteReq<SessionListNode, 2>* data = (WriteReq<SessionListNode, 2>*)handle->data;
	AsyncT* asyncT = nullptr;
	while(g_asyncTQueue.Pop(asyncT)) {
		WriteReq<SessionListNode, 2>* data = (WriteReq<SessionListNode, 2>*)asyncT->data.data;
		//g_asyncTPool->ReleaseObject(asyncT);
		int ret = g_solution_list.Update(*data->node, false, [&](SessionListNode& dst) {
			/*respone http request*/
			http_request_t* request = dst.m_http_request;
#if 0
			/*If stream is writeable */
			if (nullptr == dst.m_http_request ||
				0 == uv_is_writable((uv_stream_t*)&request->stream)) {
				XWARNING("Stream was disconnect");
				//dst.m_status = ELogin::ELOGINUNWRITE;
#if 0
				if (dst.m_status != ELogin::ELOGINLOGOUT) {
					dst.m_status = ELogin::ELOGINUNWRITE;
				}
#endif
				//free(data->buf[0].base);
				//free(data->buf[1].base);
				delete data->node;
				//free(data);
				delete data;

				return;
			}

			if (nullptr == dst.m_http_request || 
				0 == uv_is_active((uv_handle_t*)&request->stream)) {
				XWARNING("Stream was disconnect");
				//dst.m_status = ELogin::ELOGINCLOSE;
#if 0
				if (dst.m_status != ELogin::ELOGINLOGOUT) {
					dst.m_status = ELogin::ELOGINCLOSE;
				}
#endif
				//free(data->buf[0].base);
				//free(data->buf[1].base);
				delete data->node;
				//free(data);
				delete data;

				return;
			}

#else
			if (nullptr == dst.m_http_request) {
				XWARNING("Stream was disconnect|dst is nullptr");
				delete data->node;
				delete data;

				return;
			}

			if (dst.m_http_request->stream.type != UV_TCP ||
				0 == uv_is_writable((uv_stream_t*)&request->stream) ||
				0 == uv_is_active((uv_handle_t*)&request->stream)) {
				XWARNING("Stream was disconnect|type=%d", dst.m_http_request->stream.type);
				delete data->node;
				delete data;

				return;
			}
#endif
			data->node->m_http_request = request;
			data->node->m_status = dst.m_status;
			uv_write((uv_write_t*)&data->req, (uv_stream_t*)&request->stream, data->buf, 2, [](uv_write_t* req, int status) {
				WriteReq<SessionListNode, 2>* wr = (WriteReq<SessionListNode, 2>*)req;
				int tag = ntohs(*(unsigned short*)&wr->buf[0].base[2]);
				XINFO("SG->TT\ttag=0x%02X|msg=%s",tag, SimpleXorDecrypt(wr->buf[1].base, (unsigned int)wr->buf[1].len, XOR_BASE));
				if ((tag == ETag::ELOGOUTANS ||
					tag == ETag::EFORCELOGOUTANS ||
					wr->node->m_status == ELogin::ELOGININVALID)
					&& status == 0) {
					http_request_t* http_request = wr->node->m_http_request;
					uv_close((uv_handle_t*)http_request, OnClose);
				}

				//free(wr->buf[0].base);
				//free(wr->buf[1].base);
				delete wr->node;
				//free(wr);
				delete wr;
			});
		});

		/* if connect was close, release memory */
		if (ret == 0) {
			delete data->node;
			delete data;
		}

		g_asyncTPool->ReleaseObject(asyncT);
	}
#if 0 
	g_solution_list.Update(*data->node, false, [&](SessionListNode& dst) {
		/*respone http request*/
		http_request_t* request = dst.m_http_request;
		/*If stream is writeable */
		if (0 == uv_is_writable((uv_stream_t*)&request->stream)) {
			XWARNING("Stream was disconnect");
			dst.m_status = ELogin::ELOGINUNWRITE;
			free(data->buf[0].base);
			free(data->buf[1].base);
			delete data->node;
			free(data);
			uv_mutex_unlock(&g_uv_mutex);
			uv_sem_wait(&g_uv_sem);

			return;
		}

		if (0 == uv_is_active((uv_handle_t*)&request->stream)) {
			XWARNING("Stream was disconnect");
			dst.m_status = ELogin::ELOGINCLOSE;
			free(data->buf[0].base);
			free(data->buf[1].base);
			delete data->node;
			free(data);
			uv_mutex_unlock(&g_uv_mutex);
			uv_sem_wait(&g_uv_sem);

			return;
		}

		uv_write((uv_write_t*)&data->req, (uv_stream_t*)&request->stream, data->buf, 2, [](uv_write_t* req, int status) {
			WriteReq<SessionListNode, 2>* wr = (WriteReq<SessionListNode, 2>*)req;
			int tag = ntohs(*(unsigned short*)&wr->buf[0].base[2]);
			XINFO("SG->TT\ttag=0x%02X|msg=%s",tag, wr->buf[1].base);

			free(wr->buf[0].base);
			free(wr->buf[1].base);
			delete wr->node;
			free(wr);
		});
	});

	uv_mutex_unlock(&g_uv_mutex);
	uv_sem_wait(&g_uv_sem);
#endif
}

/*http connect begin */
#if defined(SIGNALENGINE)
void UVTcpServer::OnNewConnection(uv_stream_t* server, int status) {
	if (status < 0) {
		XERROR("New connection error %s\n", uv_strerror(status));

		return;
	}

#if defined(DEBUG)
	//std::string str1 = "on_new_connection";
	//XINFO(str1);
	XINFO("on_new_connection\n");
#endif

	http_request_t* http_request = (http_request_t*)malloc(sizeof(http_request_t));
	if (http_request == nullptr) {
		XERROR("malloc fail");

		return;
	}
	memset(http_request, 0, sizeof(http_request_t));

	uv_tcp_init(server->loop, (uv_tcp_t*)&http_request->stream);
	/* set references so we can use our http_request in http_parser and libuv */
	http_request->stream.data = http_request;
	http_request->parser.data = http_request;

	if (uv_accept(server, (uv_stream_t*)&http_request->stream) == 0) {
		clock_gettime(CLOCK_REALTIME, &http_request->accept);
		http_parser_init(&http_request->parser, HTTP_REQUEST);
		uv_read_start((uv_stream_t*)&http_request->stream, UVTcpServer::AllocBuffer, UVTcpServer::EchoRead);
#if defined(DEBUG)
	XINFO("uv_read_start end\n");
#endif
	} else {
		uv_close((uv_handle_t*)&http_request->stream, UVTcpServer::OnClose);
#if defined(DEBUG)
	XINFO("uv_close end\n");
#endif
	}
#if defined(DEBUG)
	XINFO("uv_*** end\n");
#endif
}


void UVTcpServer::EchoRead(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf) {
	http_request_t* http_request = (http_request_t*)handle->data;
	if (nread < 0) {
		if (nread != UV_EOF) {
			XERROR("Read error %s\n", uv_err_name(nread));
		}
		free(buf->base);
		uv_close((uv_handle_t*)handle, UVTcpServer::OnClose);

		return;
	}

	clock_gettime(CLOCK_REALTIME, &http_request->read);
	size_t parsed = http_parser_execute(&http_request->parser, m_settings->GetSettings(), buf->base, nread);

	if (parsed < nread) {
		XERROR("parse error\n");
		uv_close((uv_handle_t*)handle, UVTcpServer::OnClose);
		//free(&http_request);
	} else {
		if (strncmp(http_request->method, "POST", 4) != 0) {
			uv_close((uv_handle_t*)handle, UVTcpServer::OnClose);
		} else {
		clock_gettime(CLOCK_REALTIME, &http_request->parse_end);
		ParserJson(http_request);
		//g_threadPool->enqueue(UVTcpServer::ParserJson, http_request);
		}
	}

	free(buf->base);
#if 0
	FREE(&http_request->url);
	FREE(&http_request->method);
	FREE(&http_request->body);
	for (int i=0; i<http_request->header_lines; ++i) {
		FREE((void**)&http_request->headers[i].field);
		FREE((void**)&http_request->headers[i].value);
	}
	FREE(&http_request);
#endif
}

void UVTcpServer::ParserJson(http_request_t* http_request) {
	/*parser http body */
	rapidjson::Document d;
	http_request_t* request = http_request;
	if (strncmp(request->url, URL_MARK_DATA, strlen(URL_MARK_DATA)) == 0) {
		/*mark data(real time or history data)*/
#if 0
		char* test = "{\"symbols\":[\"USD_HKD\", \"USD_HK\"]}";
		d.Parse(test);
#endif
		d.Parse(http_request->body);
		if (d.HasParseError() ||
			!d.IsObject()) {
			XERROR("parser json error\n");

			return;
		}

		const rapidjson::Value& a = d["symbols"];
		rapidjson::SizeType  size = a.Size();
		if (size <= 0) {
			return;
		}

		std::string content = "{\"symbols\":[";
		std::string symbols = "symbols";
		clock_gettime(CLOCK_REALTIME, &http_request->query_begin);
		/* getmark data for library */
		for (rapidjson::SizeType i=0; i<size; ++i) {
			std::string price;
			TNodeAministrator::GetInstance()->GetMarketDataAgent()->GetMarketData(a[i].GetString(), price);
			content += price;
			content += ",";
		}

		clock_gettime(CLOCK_REALTIME, &http_request->query_end);
		content[content.length()-1] = ']';
		content += "}";
		/*respone http request*/
		write_req_t* req = (write_req_t*)malloc(sizeof(write_req_t));
		std::string headers;
		for (int i=0; i<request->header_lines; ++i) {
			if (strncmp(request->headers[i].field, "Content-Type", request->headers[i].field_length) == 0) {
				headers += std::string(request->headers[i].field, request->headers[i].field_length);
				headers += ": ";
				headers += std::string(request->headers[i].value, request->headers[i].value_length);
				headers +="\r\n";
				break;
			}
		}
		char* response = NULL;
		asprintf(&response, "%s%s%s%zu%s%s", "HTTP/1.1 200 OK\r\n", headers.c_str(), RESPONSE, content.length(), "\r\n\r\n", content.c_str());
		//asprintf(&response, "%s %s", RESPONSE, "hello world\n");
		req->buf = uv_buf_init(response, strlen(response));
		//request->req->buf = uv_buf_init(response, strlen(response));
		uv_write((uv_write_t*)&http_request->req, (uv_stream_t*)&http_request->stream, &req->buf, 1, NULL);
		//uv_write((uv_write_t*)&http_request->req, (uv_stream_t*)&http_request->stream, &http_request->req.buf, 1, NULL);
		//uv_write((uv_write_t*)req, (uv_stream_t*)&http_request->stream, &req->buf, 1, NULL);
		//FREE(&http_request->req->buf.base);
		//free(req);
		//FREE(&response);

		/*free allocator */
		clock_gettime(CLOCK_REALTIME, &http_request->end);
		XINFO("%zu, %zu, %zu, %zu, %zu, %zu, %zu, %zu, %zu, %zu, %zu", 
			  request->accept.tv_sec*1000000000UL +request->accept.tv_nsec, 
			  request->read.tv_sec*1000000000UL +request->read.tv_nsec, 
			  request->parse_end.tv_sec*1000000000UL +request->parse_end.tv_nsec,
			  request->query_begin.tv_sec*1000000000UL +request->query_begin.tv_nsec,
			  request->query_end.tv_sec*1000000000UL +request->query_end.tv_nsec,
			  request->end.tv_sec*1000000000UL +request->end.tv_nsec,

			  request->read.tv_sec*1000000000UL +request->read.tv_nsec - request->accept.tv_sec*1000000000UL - request->accept.tv_nsec, 
			  request->parse_end.tv_sec*1000000000UL +request->parse_end.tv_nsec - request->read.tv_sec*1000000000UL - request->read.tv_nsec, 
			  request->query_end.tv_sec*1000000000UL +request->query_end.tv_nsec - request->query_begin.tv_sec*1000000000UL - request->query_begin.tv_nsec,
			  request->end.tv_sec*1000000000UL +request->end.tv_nsec - request->query_end.tv_sec*1000000000UL - request->query_end.tv_nsec,
			  request->end.tv_sec*1000000000UL +request->end.tv_nsec - request->accept.tv_sec*1000000000UL - request->accept.tv_nsec 
			  );
		FREE(&req->buf.base);
		FREE(&req);

		FREE(&request->url);
		FREE(&request->method);
		FREE(&request->body);
		for (int i=0; i<request->header_lines; ++i) {
			FREE((void**)&request->headers[i].field);
			FREE((void**)&request->headers[i].value);
		}
		//FREE(&request);
		//FREE(&request->req->buf.base);
	} else if (strncmp(request->url, URL_SEND_ORDER, strlen(URL_SEND_ORDER)) == 0) {
		/* send order */
		d.Parse(http_request->body);
		if (d.HasParseError() ||
			!d.IsObject()) {
			XERROR("parser json error\n");

			return;
		}

		SendOrderListNode node;
		rapidjson::Value::ConstMemberIterator it = d.FindMember("reqid");
		if (it != d.MemberEnd()) {
#if defined(DEBUG)
		XINFO("reqid: %s\n", it->value.GetString());
#endif
			//HexToBinary((unsigned char*)node.m_reqID, (unsigned char*)it->value.GetString(), strlen(it->value.GetString()));
			memcpy(node.m_reqID, (unsigned char*)it->value.GetString(), strlen(it->value.GetString()));
		}

		it = d.FindMember("expiryperiod");
		if (it != d.MemberEnd()) {
#if defined(DEBUG)
		XINFO("expiryperiod: %d\n", it->value.GetInt());
#endif
			node.m_expiryPeriod = it->value.GetInt();
		}
		/*add the solution gateway id*/
		//d.AddMember("sgid", 1, d.GetAllocator());
		d.AddMember("sgid",  m_iSGID, d.GetAllocator());
		/*add transfer: default 0 */
		d.AddMember("transfer", 0, d.GetAllocator());
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		d.Accept(writer);

		/*publish sendorder.$: broadcast */
		TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_SENDORDER_GROUP_NAME, 
				//SOLUTIONGATEWAY_PUBLISH_SENDORDER_ALL_EVENT,
				SOLUTIONGATEWAY_PUBLISH_SENDORDER_POSITION_ALLTE_EVENT,
				buffer.GetString(), 
				buffer.GetLength()+1, 
				ETag::ESENDORDER);
		//SendOrderListNode node;
		node.m_http_request = http_request;
		//memcpy(node.m_http_request, http_request, sizeof(http_request));
		//node.m_sgID = 1;
		node.m_sgID = m_iSGID;
		g_solution_list.PushFront(std::move(node));
	}
}
/*http connect end */
#endif

void UVTcpServer::AllocBuffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
	buf->base = (char*) malloc(suggested_size);
	memset(buf->base, 0, suggested_size);
	buf->len = suggested_size;
}

void UVTcpServer::OnClose(uv_handle_s* handle) {
#if defined(SIGNALENGIN)
	http_request_t* http_request = (http_request_t*)handle->data;
	FREE(&http_request->url);
	FREE(&http_request->method);
	FREE(&http_request->body);
	for (int i=0; i<http_request->header_lines; ++i) {
		FREE((void**)&http_request->headers[i].field);
		FREE((void**)&http_request->headers[i].value);
	}
	FREE(&http_request);
#endif

#if defined(DSOLUTION)
	SessionListNode node;
	node.m_http_request = (http_request_t*)handle;
	g_solution_list.Update(node, false, [&](SessionListNode& dst) {
		//dst.m_status = ELogin::ELOGINLOGOUT;
		dst.m_http_request = nullptr;
	});

	//fprintf(stderr, "++OnClose threadid:%ld, handle:%p\n",uv_thread_self(), handle);
	
	std::map<void*, struct UV_BUF_T>::iterator it;
	it = m_map.find((void*)handle);
	if (it != m_map.end()) {
		free(it->second.base);
		m_map.erase(it);
	}
	free(handle);
#endif

#if defined(DEBUG)
	XINFO("tcp=%p disconnection\n", handle);
#endif
}

int UVTcpServer::Run(void) {
	//Consumer* new consumer("1", ETag::ESENDORDER);

	return uv_run(m_loop, UV_RUN_DEFAULT);
}

/* TCP connect begin */
void UVTcpServer::OnNewConnectionTcp(uv_stream_t* server, int status) {
	if (status < 0) {
		XERROR("New connection error %s\n", uv_strerror(status));

		return;
	}

#if defined(DEBUG)
	XINFO("on_new_connection\n");
#endif
	http_request_t* http_request = (http_request_t*)malloc(sizeof(http_request_t));
	if (http_request == nullptr) {
		XERROR("malloc fail");

		return;
	}
	memset(http_request, 0, sizeof(http_request_t));
	//http_request->stream.data = http_request;

	uv_tcp_init(server->loop, (uv_tcp_t*)&http_request->stream);
	uv_tcp_nodelay((uv_tcp_t*)&http_request->stream, 1);
	uv_tcp_keepalive((uv_tcp_t*)&http_request->stream, 1, 3600);
	if (uv_accept(server, (uv_stream_t*)&http_request->stream) == 0) {
		uv_read_start((uv_stream_t*)&http_request->stream, UVTcpServer::AllocBuffer, UVTcpServer::EchoReadTcp);
#if defined(DEBUG)
	struct sockaddr_in addr;
	int addrLen = sizeof(addr);
	uv_tcp_getpeername((uv_tcp_t*)&http_request->stream, (struct sockaddr*)&addr, &addrLen);
	char ipAddrTmp[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &addr.sin_addr, ipAddrTmp, INET_ADDRSTRLEN);
	XINFO("tcp=%p, peername=%s, port=%d, uv_read_start end\n", http_request, ipAddrTmp, ntohs(addr.sin_port));
#endif
	} else {
		uv_close((uv_handle_t*)&http_request->stream, UVTcpServer::OnClose);
#if defined(DEBUG)
	XINFO("tcp=%p uv_close end\n", http_request);
#endif
	}
#if defined(DEBUG)
	XINFO("tcp=%p uv_*** end\n", http_request);
#endif
}

void UVTcpServer::EchoReadTcp(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf) {
	if (nread < 0) {
		//fprintf(stderr, "ECHO  threadid:%ld, handle:%p\n",uv_thread_self(), handle);
#if 1
		if (nread == UV_EOF) {
			XINFO("tcp=%p Read error: client close|%s\n", handle, uv_err_name(nread));
		} else if (nread == UV_ECONNRESET) {
			XINFO("tcp=%p Read error: connect reset|%s\n", handle, uv_err_name(nread));
		} else {
			XINFO("tcp=%p Read error %s\n", handle, uv_err_name(nread));
		}
		free(buf->base);
		
#if 0
		if (uv_is_active((uv_handle_t*)handle)) {
			uv_read_stop((uv_stream_t*)handle);
		}
#endif
#if 1
		uv_close((uv_handle_t*)handle, UVTcpServer::OnClose);
#else
		uv_close((uv_handle_t*)handle, NULL);
		SessionListNode node;
		node.m_http_request = (http_request_t*)handle;
		g_solution_list.Update(node, false, [&](SessionListNode& dst) {
			dst.m_status = ELogin::ELOGINCLOSE;
			dst.m_http_request = nullptr;
		});

		std::map<void*, struct UV_BUF_T>::iterator it;
		it = m_map.find((void*)handle);
		if (it != m_map.end()) {
			free(it->second.base);
			m_map.erase(it);
		}
		free(handle);
#endif
#else
		if (nread != UV_EOF) {
			XERROR("Read error %s\n", uv_err_name(nread));
		}
		free(buf->base);
		uv_close((uv_handle_t*)handle, UVTcpServer::OnClose);
#endif
        return;
	}

	ParserJsonTcp((http_request_t*)handle, buf, nread);
	free(buf->base);
}

void UVTcpServer::SendOrder(http_request_t* http_request, std::shared_ptr<char> dec_body){
	g_threadPool->Enqueue([dec_body, http_request] {
		rapidjson::Document d;
		/* send order */
		d.Parse(dec_body.get());
		if (d.HasParseError() ||
			!d.IsObject()) {
			XERROR("parser json error\n");

			return;
		}

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("sessionid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
			XINFO("filed no exit or type error");
#endif

			return;
		}
		const char* sessionid = it->value.GetString();

		if(((it = d.FindMember("groupid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
			XINFO("filed no exit or type error");
#endif

			return;
		}
		int groupid = it->value.GetInt();

		if(((it = d.FindMember("clientid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
			XINFO("filed no exit or type error");
#endif

			return;
		}
		int clientid = it->value.GetInt();

		if(((it = d.FindMember("assetid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
			XINFO("filed no exit or type error");
#endif

			return;
		}
		int assetid = it->value.GetInt();
#if 0
		if(assetid < 0 || assetid > 70) {
#if defined(DEBUG)
			XINFO("assetid should be [0, 70]");
#endif

			return;
		}
#endif

		if(((it = d.FindMember("direction")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
			XINFO("filed no exit or type error");
#endif

			return;
		}
		int direction = it->value.GetInt();
		if(direction != 0 && direction != 1) {
#if defined(DEBUG)
			XINFO("direction should be 0 or 1");
#endif

			return;
		}

		if(((it = d.FindMember("amount")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
			XINFO("filed no exit or type error");
#endif

			return;
		}
		int amount= it->value.GetInt();
		if(amount < 1) {
#if defined(DEBUG)
			XINFO("amount should > 0");
#endif

			return;
		}

		if(((it = d.FindMember("expiryperiod")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
			XINFO("filed no exit or type error");
#endif

			return;
		}

		int expiryperiod = it->value.GetInt();
		if((expiryperiod <= 0) || ((expiryperiod % 5) != 0)) {
#if defined(DEBUG)
			XINFO("expiryperiod time error");
#endif

			return;
		}

		/*verify login by sessionid */
		SessionListNode node(sessionid, groupid, clientid);
		g_solution_list.Update(node, false, [&](SessionListNode& dst) {
			if (dst.m_http_request != http_request) {
				if (dst.m_http_request != nullptr &&
					dst.m_groupID == groupid &&
					dst.m_clientID == clientid) {
					XERROR("Have two connect, the older status|type=%d", dst.m_http_request->stream.type);
					memset(dst.m_sessionID, 0, SESSION_ID_LEN);
					uv_close((uv_handle_t*)&dst.m_http_request->stream, OnClose);
				} else {
					/* invalidate user */
					memset(dst.m_sessionID, 0, SESSION_ID_LEN);
					if(uv_is_active((uv_handle_t*)http_request)) {
						uv_close((uv_handle_t*)http_request, OnClose);
					}
				}

				return;
			} else {
				memset(dst.m_sessionID, 0, SESSION_ID_LEN);
				if ((dst.m_groupID != groupid ||
					dst.m_clientID != clientid) &&
					uv_is_active((uv_handle_t*)&dst.m_http_request->stream)) {

					uv_close((uv_handle_t*)&dst.m_http_request->stream, OnClose);

					return;
				}
			}

			if (dst.m_status != ELogin::ELOGINSUCCESS &&
				uv_is_active((uv_handle_t*)&dst.m_http_request->stream)) {
				XERROR("invalid login user, disconnect");
				uv_close((uv_handle_t*)&dst.m_http_request->stream, OnClose);

				return;
			}

			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			/*publish sendorder.$: broadcast */
			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_SENDORDER_GROUP_NAME, 
														   //SOLUTIONGATEWAY_PUBLISH_SENDORDER_ALL_EVENT,
														   SOLUTIONGATEWAY_PUBLISH_SENDORDER_POSITION_ALLTE_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::ESENDORDER);
		});
	});
}

void UVTcpServer::Login(http_request_t* http_request, std::shared_ptr<char> dec_body){
	/*login */
	g_threadPool->Enqueue([dec_body, http_request] {
		rapidjson::Document d;
		d.Parse(dec_body.get());
		if (d.HasParseError() ||
			!d.IsObject()) {
			XERROR("parser json error\n");
			
			return;
		}

		/* add the peer name */
		struct sockaddr_in addr;
		int addrLen = sizeof(addr);
		uv_tcp_getpeername((uv_tcp_t*)&http_request->stream, (struct sockaddr*)&addr, &addrLen);
		char ipAddrTmp[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &addr.sin_addr, ipAddrTmp, INET_ADDRSTRLEN);
		rapidjson::Value ipAddr(ipAddrTmp, d.GetAllocator());
		//d.AddMember("ipaddress", ipAddr, d.GetAllocator());
		d.AddMember("addr", ipAddr, d.GetAllocator());
		/*add the solution gateway id*/
		d.AddMember("sgid", m_iSGID, d.GetAllocator());
		/* create uuid for login */
		uuid_t uuid;
		char session[33];
		uuid_generate_time_safe(uuid);
		DecToHex(session, (char*)&uuid);
		session[32] = 0;
		rapidjson::Value sessionid(session, d.GetAllocator());
		d.AddMember("sessionid", sessionid, d.GetAllocator());
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		d.Accept(writer);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		//const char *reqid = it->value.GetString();

		/*publish LOGIN.CLIE:  */
		TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_LOGIN_NAME, 
													   SOLUTIONGATEWAY_PUBLISH_LOGIN_EVENT,
													   buffer.GetString(),
													   buffer.GetLength()+1,
													   ETag::ELOGIN);

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

		SessionListNode node(session, groupid, clientid, m_iSGID);
		node.m_http_request = http_request;
		g_solution_list.PushFront(std::move(node));
	});
}

void UVTcpServer::ReLogin(http_request_t* http_request, std::shared_ptr<char> dec_body){
		g_threadPool->Enqueue([dec_body, http_request] {
		rapidjson::Document d;
		d.Parse(dec_body.get());
		if (d.HasParseError() ||
			!d.IsObject()) {
			XERROR("parser json error\n");

			return;
		}
		
		/* add the peer name */
		struct sockaddr_in addr;
		int addrLen = sizeof(addr);
		uv_tcp_getpeername((uv_tcp_t*)&http_request->stream, (struct sockaddr*)&addr, &addrLen);
		char ipAddrTmp[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &addr.sin_addr, ipAddrTmp, INET_ADDRSTRLEN);
		rapidjson::Value ipAddr(ipAddrTmp, d.GetAllocator());
		//d.AddMember("ipaddress", ipAddr, d.GetAllocator());
		d.AddMember("addr", ipAddr, d.GetAllocator());
		/*add the solution gateway id*/
		d.AddMember("sgid", m_iSGID, d.GetAllocator());
		/* create uuid for login */
		// uuid_t uuid;
		// char session[33];
		// uuid_generate_time_safe(uuid);
		// DecToHex(session, (char*)&uuid);
		// session[32] = 0;
		// rapidjson::Value sessionid(session, d.GetAllocator());
		// d.AddMember("sessionid", sessionid, d.GetAllocator());
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		d.Accept(writer);

		rapidjson::Value::ConstMemberIterator it;

		if(((it = d.FindMember("sessionid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}


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

		SessionListNode node(session, groupid, clientid, m_iSGID);
		bool oringinConn = false;
		bool routineExit = false;
		int ret = g_solution_list.Update(node, false, [&](SessionListNode& dst) {
			if (dst.m_http_request == http_request) {
				oringinConn = true;

				return;
			}
			//this condition is used for recognizing whether the related TCP source recyced.
			if (dst.m_http_request != nullptr) {//relogin on the same tcp invalid
				uv_close((uv_handle_t*)&dst.m_http_request->stream, OnClose);
				dst.m_http_request = http_request;
				// if (uv_is_active((uv_handle_t*)&dst.m_http_request->stream)) {
				// 	uv_read_stop((uv_stream_t*)&dst.m_http_request->stream);
				// }
				// uv_close((uv_handle_t*)&dst.m_http_request->stream, NULL);

				// std::map<void*, struct UV_BUF_T>::iterator it;
				// it = m_map.find((void*)&dst.m_http_request->stream);

				// if (it != m_map.end()) {
				// 	free(it->second.base);
				// 	m_map.erase(it);
				// }

				// free(dst.m_http_request);
			} else {
				routineExit = true;
			}
			//if (dst.m_http_request == nullptr) {}
			//dst.m_http_request = http_request;
		});

		if (0 == ret) {
			node.m_http_request = http_request;
			g_solution_list.PushFront(std::move(node));
		}
		
		/*publish LOGIN.CLIE:  */
		if (!oringinConn && !routineExit) {//routine logout need to filte
			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_LOGIN_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_LOGIN_EVENT,
														   buffer.GetString(),
														   buffer.GetLength()+1,
														   ETag::ERELOGIN);
		}
	});
}

void UVTcpServer::Logout(http_request_t* http_request, std::shared_ptr<char> dec_body){
	/*logout */
	g_threadPool->Enqueue([dec_body, http_request] {
		rapidjson::Document d;
		/* send order */
		d.Parse(dec_body.get());
		if (d.HasParseError() ||
			!d.IsObject()) {
			XERROR("parser json error\n");

			return;
		}

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("sessionid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *sessionid = it->value.GetString();

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

		SessionListNode node(sessionid, groupid, clientid, m_iSGID);
		g_solution_list.Update(node, false, [&](SessionListNode& dst) {
			/*add the solution gateway id*/
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			/*publish LOGOUT.CLIENTMANAGE */
			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_LOGOUT_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_LOGOUT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::ELOGOUT);
		});
	});
}

void UVTcpServer::ChangePWD(http_request_t* http_request, std::shared_ptr<char> dec_body){
	/*login */
	g_threadPool->Enqueue([dec_body, http_request] {
		rapidjson::Document d;
		d.Parse(dec_body.get());
		if (d.HasParseError() ||
			!d.IsObject()) {
			XERROR("parser json error\n");

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

		if(((it = d.FindMember("password")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char* password = it->value.GetString();

		if(((it = d.FindMember("oldpassword")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char* oldpassword = it->value.GetString();

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

		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}

		/*add the solution gateway id*/
		d.AddMember("sgid", m_iSGID, d.GetAllocator());
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		d.Accept(writer);

		/*publish CHANGEPWD.CLIENTMANGE:  */
		TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_CHANGEPWD_NAME, 
													   SOLUTIONGATEWAY_PUBLISH_CHANGEPWD_EVENT,
													   buffer.GetString(),
													   buffer.GetLength()+1,
													   ETag::ECHANGEPWD);

	});
}

void UVTcpServer::ParserJsonTcp(http_request_t* http_request, const uv_buf_t* buf, ssize_t nread) {
	static const size_t SIZE = 131070; /* 65535X2 */
	//static const unsigned char TAG = 0x8F;

	typedef void (*SubParseJsonTcp) (http_request_t* http_request, std::shared_ptr<char> body);
	static SubParseJsonTcp subParseJsonTcp[ETag::ECHANGEPWD + 1];
	static std::map<void*, struct UV_BUF_T>::iterator it;
	static char* tmpBody = nullptr;
	static size_t tmpLen = 0; 
	static HeaderTag headerTag;

	std::call_once(m_once, []() {
		m_buf.base = (char*) malloc(SIZE);
		memset(m_buf.base, 0, SIZE);
		m_buf.len = 0;

		subParseJsonTcp[ETag::ESENDORDER] = &UVTcpServer::SendOrder;
		subParseJsonTcp[ETag::ELOGIN] = &UVTcpServer::Login;
		subParseJsonTcp[ETag::ERELOGIN] = &UVTcpServer::ReLogin;
		subParseJsonTcp[ETag::ELOGOUT] = &UVTcpServer::Logout;
		subParseJsonTcp[ETag::ECHANGEPWD] = &UVTcpServer::ChangePWD;
	});

	bool findFlag = false;

	if(buf->base[0] == (char)TAG) {////////////
		/* normal parese */
		tmpBody = buf->base;
		tmpLen = nread;
	} else {
		/* maybe request sent multiple time */
		/* first find the map if exist */
		it = m_map.find((void*)http_request);
		if (it != m_map.end() &&
			*it->second.base == (char)TAG) {
			/* exist */
			it->second.len += nread;
			if (it->second.len > it->second.cap) {
				size_t cnt = it->second.len / m_msgLen;
				it->second.cap = (it->second.cap << (1+cnt));
				it->second.base = (char*)realloc(it->second.base, it->second.cap);
			}
			memcpy(&it->second.base[it->second.len - nread], buf->base, nread);
			
			tmpBody = it->second.base;
			tmpLen = it->second.len;
			findFlag = true;
		} else {
			return;
		}
	}

	if (tmpLen <= 8) {/////////////
		UV_BUF_T elem;
		elem.len = tmpLen;
		if (tmpLen < m_msgLen) {
			elem.cap = m_msgLen;
			elem.base = (char*)malloc(m_msgLen);
			memcpy(elem.base, tmpBody, elem.len);
		} else {
			elem.cap = tmpLen;
			elem.base = (char*)malloc(tmpLen);
			memcpy(elem.base, tmpBody, elem.len);
		}

		std::map<void*, struct UV_BUF_T>::iterator it = m_map.find((void*)http_request);
		if (it != m_map.end()) {
			free(it->second.base);
			m_map.erase(it);
		}

		m_map.insert(std::pair<void*, struct UV_BUF_T>((void*)http_request, elem));

		return;
	}

	headerTag.head = *(unsigned char*)tmpBody;
	headerTag.tag = ntohs(*(unsigned short*)&tmpBody[2]);
	headerTag.length = ntohl(*(unsigned int*)&tmpBody[4]);

	if (headerTag.length + 8 > tmpLen) {///////////////
#if 0
		/* maybe message was split many port */
		if (findFlag) {
			return;
		}
#endif

		UV_BUF_T elem;
		elem.len = tmpLen;
		if (tmpLen < m_msgLen) {
			elem.cap = m_msgLen;
			elem.base = (char*)calloc(m_msgLen, 1);
			memcpy(elem.base, tmpBody, elem.len);
		} else {
			elem.cap = tmpLen;
			elem.base = (char*)calloc(tmpLen, 1);
			memcpy(elem.base, tmpBody, elem.len);
		}

		std::map<void*, struct UV_BUF_T>::iterator it = m_map.find((void*)http_request);
		if (it != m_map.end()) {
			free(it->second.base);
			m_map.erase(it);
		}

		m_map.insert(std::pair<void*, struct UV_BUF_T>((void*)http_request, elem));

		return;
	}

	ssize_t hadRead = 0;
	while(true) {
		char* body = (char*)calloc(headerTag.length, 1);
		memcpy(body, &tmpBody[hadRead+8], headerTag.length);

#if defined(DEBUG)
		int len = headerTag.length;
		XINFO("tcp=%p,tag=0x%02X,enc_lenght=%d,enc_body=%s", http_request, headerTag.tag, headerTag.length, byteToHexString(body, len));
#endif

		unsigned char aes_key[AES_BLOCK_SIZE];
		memset(aes_key, 0, AES_BLOCK_SIZE);
		strcpy((char*)aes_key, "helloworld");
		std::shared_ptr<char> dec_out = m_aes.Decrypt(aes_key, (unsigned char*)body, headerTag.length);

#if defined(DEBUG)
	    len = strlen(dec_out.get()) + 1;
	    XINFO("%s", byteToHexString(dec_out.get(), len));
		XINFO("tcp=%p,tag=0x%02X,dec_length=%d,dec_body=%s", http_request, headerTag.tag, strlen(dec_out.get()) + 1, dec_out.get());
#endif
		
		free(body);

		switch (headerTag.tag) {
			case ETag::ESENDORDER: 
				/* sendorder */
				subParseJsonTcp[ESENDORDER](http_request, dec_out);

				break;

			case ETag::ELOGIN: 
				/* login */
				subParseJsonTcp[ELOGIN](http_request, dec_out);

				break;

			case ETag::ERELOGIN: 
				/* login */
				subParseJsonTcp[ERELOGIN](http_request, dec_out);

				break;

			case ETag::ELOGOUT: 
				/*logout */
				subParseJsonTcp[ELOGOUT](http_request, dec_out);
				
				break;

			case ETag::ECHANGEPWD: 
				/*change password */
				subParseJsonTcp[ECHANGEPWD](http_request, dec_out);

				break;
		}
	
		hadRead += headerTag.length + 8;

		if (hadRead < tmpLen) {
			headerTag.head = *(unsigned char*)&tmpBody[hadRead];
			if (headerTag.head == TAG) {
				if (hadRead + 8 < tmpLen) {
					headerTag.tag = ntohs(*(unsigned short*)&tmpBody[hadRead+2]);
					headerTag.length = ntohl(*(unsigned int*)&tmpBody[hadRead+4]);

					if (hadRead + 8 + headerTag.length <= tmpLen) {
						continue;
					} else {
						/* maybe message was split many part */
						if (findFlag) {/////////////////
							it->second.len = tmpLen - hadRead;
							memmove(it->second.base, &it->second.base[hadRead], it->second.len);

							return;
						} else {
							UV_BUF_T elem;
							elem.len = tmpLen - hadRead;
							if (elem.len < m_msgLen) {
								elem.cap = m_msgLen;
								elem.base = (char*)malloc(m_msgLen);
								memcpy(elem.base, &tmpBody[hadRead], elem.len);
							} else {
								elem.cap = elem.len;
								elem.base = (char*)malloc(elem.len);
								memcpy(elem.base, &tmpBody[hadRead], elem.len);
							}
							std::map<void*, struct UV_BUF_T>::iterator it = m_map.find((void*)http_request);
							if (it != m_map.end()) {
								free(it->second.base);
								m_map.erase(it);
							}

							m_map.insert(std::pair<void*, struct UV_BUF_T>((void*)http_request, elem));

							return;
						}
					}
				} else {
					if (findFlag) {//////////////
						// free(it->second.base);
						// m_map.erase(it);
						it->second.len = tmpLen - hadRead;
						memmove(it->second.base, &it->second.base[hadRead], it->second.len);

						return;
					} else {
						UV_BUF_T elem;
						elem.len = tmpLen - hadRead;
						if (elem.len < m_msgLen) {
							elem.cap = m_msgLen;
							elem.base = (char*)calloc(m_msgLen, 1);
							memcpy(elem.base, &tmpBody[hadRead], elem.len);
						} else {
							elem.cap = elem.len;
							elem.base = (char*)calloc(elem.len, 1);
							memcpy(elem.base, &tmpBody[hadRead], elem.len);
						}
						std::map<void*, struct UV_BUF_T>::iterator it = m_map.find((void*)http_request);
						if (it != m_map.end()) {
							free(it->second.base);
							m_map.erase(it);
						}

						m_map.insert(std::pair<void*, struct UV_BUF_T>((void*)http_request, elem));

						return;
					}
				}
			} else {
				if (findFlag) {
					free(it->second.base);
					m_map.erase(it);
				}

				return;
			}
		} else if (hadRead == tmpLen) {
			if (findFlag) {
				free(it->second.base);
				m_map.erase(it);
			}

			return;
		}
	}
}

#if 0
void UVTcpServer::ParserJsonTcp(http_request_t* http_request, const uv_buf_t* buf, ssize_t nread) {
	if (nread <= sizeof(HeaderTag)) {
		return;
	}

	HeaderTag headerTag;
	headerTag.head = *(unsigned char*)buf->base;
	headerTag.tag = ntohs(*(unsigned short*)&buf->base[2]);
	headerTag.length = ntohl(*(unsigned int*)&buf->base[4]);

	ssize_t hadRead = 0;
	while(true) {
		if (headerTag.head != 0x8F) {
			XERROR("header invalide");

			return;
		}

		if (headerTag.length > nread) {
			XERROR("TCP error message");

			return;
		}

		char* body = (char*)malloc(headerTag.length);
		memcpy(body, &buf->base[8+hadRead], headerTag.length);
		hadRead += headerTag.length + 8;

#if defined(DEBUG)
XINFO("tag=0x%02X,length=%d,body=%s", headerTag.tag, headerTag.length, body);
#endif
		switch (headerTag.tag) {
			case ETag::ESENDORDER: {
				g_threadPool->Enqueue([body, http_request] {
					rapidjson::Document d;
					/* send order */
					d.Parse(body);
					if (d.HasParseError() ||
						!d.IsObject()) {
						XERROR("parser json error\n");
						free(body);

						return;
					}
					free(body);

					rapidjson::Value::ConstMemberIterator it;
					if(((it = d.FindMember("sessionid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
						XINFO("filed no exit or type error");
#endif
						
						return;
					}
					const char* sessionid = it->value.GetString();

					if(((it = d.FindMember("assetid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
						XINFO("filed no exit or type error");
#endif

						return;
					}
					int assetid = it->value.GetInt();
					if(assetid < 0 || assetid > 70) {
#if defined(DEBUG)
						XINFO("assetid should be [0, 70]");
#endif

						return;
					}

					if(((it = d.FindMember("direction")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
						XINFO("filed no exit or type error");
#endif

						return;
					}
					int direction = it->value.GetInt();
					if(direction != 0 && direction != 1) {
#if defined(DEBUG)
						XINFO("direction should be 0 or 1");
#endif

						return;
					}

					if(((it = d.FindMember("amount")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
						XINFO("filed no exit or type error");
#endif

						return;
					}
					int amount= it->value.GetInt();
					if(amount < 1) {
#if defined(DEBUG)
						XINFO("amount should > 0");
#endif

						return;
					}

					if(((it = d.FindMember("expiryperiod")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
						XINFO("filed no exit or type error");
#endif

						return;
					}
					int expiryperiod = it->value.GetInt();
					if((expiryperiod == 0) || ((expiryperiod % 5) != 0)) {
#if defined(DEBUG)
						XINFO("expiryperiod time error");
#endif

						return;
					}
					
					/*verify login by sessionid */
					SessionListNode node(sessionid);
					g_solution_list.Update(node, false, [&](SessionListNode& dst) {
						if (dst.m_status != ELogin::ELOGINSUCCESS) {
							XERROR("invalid login user, disconnect");

							return;
						}

						d.AddMember("sgid", m_iSGID, d.GetAllocator());
						rapidjson::StringBuffer buffer;
						rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
						d.Accept(writer);

						/*publish sendorder.$: broadcast */
						TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_SENDORDER_GROUP_NAME, 
																	   //SOLUTIONGATEWAY_PUBLISH_SENDORDER_ALL_EVENT,
																	   SOLUTIONGATEWAY_PUBLISH_SENDORDER_POSITION_ALLTE_EVENT,
																	   buffer.GetString(), 
																	   buffer.GetLength()+1, 
																	   ETag::ESENDORDER);
					});
				});
				break;
			}

			case ETag::ELOGIN: {
				/*login */
				g_threadPool->Enqueue([body, http_request] {
					rapidjson::Document d;
					d.Parse(body);
					if (d.HasParseError() ||
						!d.IsObject()) {
						XERROR("parser json error\n");
						free(body);

						return;
					}
					free(body);

					/* add the peer name */
					struct sockaddr_in addr;
					int addrLen = sizeof(addr);
					uv_tcp_getpeername((uv_tcp_t*)&http_request->stream, (struct sockaddr*)&addr, &addrLen);
					char ipAddrTmp[INET_ADDRSTRLEN];
					inet_ntop(AF_INET, &addr.sin_addr, ipAddrTmp, INET_ADDRSTRLEN);
					rapidjson::Value ipAddr(ipAddrTmp, d.GetAllocator());
					//d.AddMember("ipaddress", ipAddr, d.GetAllocator());
					d.AddMember("addr", ipAddr, d.GetAllocator());
					/*add the solution gateway id*/
					d.AddMember("sgid", m_iSGID, d.GetAllocator());
					/* create uuid for login */
					uuid_t uuid;
					char session[33];
					uuid_generate_time_safe(uuid);
					DecToHex(session, (char*)&uuid);
					session[32] = 0;
					rapidjson::Value sessionid(session, d.GetAllocator());
					d.AddMember("sessionid", sessionid, d.GetAllocator());
					rapidjson::StringBuffer buffer;
					rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
					d.Accept(writer);

					rapidjson::Value::ConstMemberIterator it;
					if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
						XINFO("field no exit or type error");
#endif

						return;
					}
					//const char *reqid = it->value.GetString();

					/*publish LOGIN.CLIE:  */
					TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_LOGIN_NAME, 
																   SOLUTIONGATEWAY_PUBLISH_LOGIN_EVENT,
																   buffer.GetString(),
																   buffer.GetLength()+1,
																   ETag::ELOGIN);

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

					SessionListNode node(session, groupid, clientid, m_iSGID);
					node.m_http_request = http_request;
					g_solution_list.PushFront(std::move(node));
				});
				break;
			}

			case ETag::ELOGOUT: {
				/*logout */
				g_threadPool->Enqueue([body, http_request] {
					rapidjson::Document d;
					/* send order */
					d.Parse(body);
					if (d.HasParseError() ||
						!d.IsObject()) {
						XERROR("parser json error\n");
						free(body);

						return;
					}
					free(body);

					rapidjson::Value::ConstMemberIterator it;
					if(((it = d.FindMember("sessionid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
						XINFO("field no exit or type error");
#endif

						return;
					}
					const char *sessionid = it->value.GetString();

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

					SessionListNode node(sessionid, groupid, clientid, m_iSGID);
					g_solution_list.Update(node, false, [&](SessionListNode& dst) {
						/*add the solution gateway id*/
						d.AddMember("sgid", m_iSGID, d.GetAllocator());
						rapidjson::StringBuffer buffer;
						rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
						d.Accept(writer);

						/*publish LOGOUT.CLIENTMANAGE */
						TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_LOGOUT_NAME, 
																	   SOLUTIONGATEWAY_PUBLISH_LOGOUT_EVENT,
																	   buffer.GetString(), 
																	   buffer.GetLength()+1, 
																	   ETag::ELOGOUT);
					});
				});
				break;
			}

			case ETag::ECHANGEPWD: {
				/*login */
				g_threadPool->Enqueue([body, http_request] {
					rapidjson::Document d;
					d.Parse(body);
					if (d.HasParseError() ||
						!d.IsObject()) {
						XERROR("parser json error\n");
						free(body);

						return;
					}
					free(body);

					rapidjson::Value::ConstMemberIterator it;
					if(((it = d.FindMember("sessionid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
						XINFO("field no exit or type error");
#endif

						return;
					}
					const char* sessionid = it->value.GetString();

					if(((it = d.FindMember("password")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
						XINFO("field no exit or type error");
#endif

						return;
					}
					const char* password = it->value.GetString();

					if(((it = d.FindMember("oldpassword")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
						XINFO("field no exit or type error");
#endif

						return;
					}
					const char* oldpassword = it->value.GetString();

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

					if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
						XINFO("field no exit or type error");
#endif

						return;
					}

					/*add the solution gateway id*/
					d.AddMember("sgid", m_iSGID, d.GetAllocator());
					rapidjson::StringBuffer buffer;
					rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
					d.Accept(writer);

					/*publish CHANGEPWD.CLIENTMANGE:  */
					TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_CHANGEPWD_NAME, 
																   SOLUTIONGATEWAY_PUBLISH_CHANGEPWD_EVENT,
																   buffer.GetString(),
																   buffer.GetLength()+1,
																   ETag::ECHANGEPWD);

				});
				break;
			}
		}
	
		//std::this_thread::yield();

		//if (hadRead == nread) {
		if (hadRead >= nread) {
			break;
		}

		headerTag.head = *(unsigned char*)&buf->base[hadRead];
		headerTag.tag = ntohs(*(unsigned short*)&buf->base[hadRead+2]);
		headerTag.length = ntohl(*(unsigned int*)&buf->base[hadRead+4]);
	}

}
#endif

void UVTcpServer::WriteCB(uv_write_t *req, int status) {
	write_req_t* wr;
	wr = (write_req_t*) req;

	//int written = wr->buf.len;
	if (status != 0) {
		XERROR("async write", uv_strerror(status));
	}

#if 0
	/* Parse Json */
	int tag = ntohs(*(uint16_t*)&wr->buf.base[2]);
	XINFO("SG->TT\ttag=0x%02X, status=%d|msg=%s", tag, status, &wr->buf.base[8]);
	char* json = nullptr;
	asprintf(&json, "%s", &wr->buf.base[8]);
#endif
	free(wr->buf.base);
	free(wr);

#if 0
	g_threadPool->Enqueue([json] {
		rapidjson::Document d;
		d.Parse(json);
		if (d.HasParseError() ||
			!d.IsObject()) {
			XERROR("parser json error|event=%s", json);
			free(json);

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
  		if(((it = d.FindMember("orderstatus")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int orderstatus = it->value.GetInt();
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
		//int orderid = d["orderid"].GetInt();
		int orderid = 999;
  		if(((it = d.FindMember("assetid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int assetid = it->value.GetInt();
  		if(((it = d.FindMember("direction")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int direction = it->value.GetInt();
  		if(((it = d.FindMember("amount")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int amount = it->value.GetInt();
  		if(((it = d.FindMember("payout")) == d.MemberEnd()) || !it->value.IsInt64()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		long payout = it->value.GetInt64();
  		if(((it = d.FindMember("opentime")) == d.MemberEnd()) || !it->value.IsInt64()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		long opentime = it->value.GetInt64();
  		if(((it = d.FindMember("openprice")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int openprice = it->value.GetInt();
  		if(((it = d.FindMember("closeprice")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int closeprice = it->value.GetInt();
		//int expiryperiod = d["expiryperiod"].GetInt();
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

		/*asyn write order ans to database*/
		char* sqlbuf = nullptr;
		CDateTime dateTime;
		char* tmpDay = dateTime.AddDay(0);
		int day = atoi(tmpDay);

		asprintf(&sqlbuf, SG_ORDER_ANS_FORMAT,
				 day,
				 reqid,
				 groupid,
				 clientid,
				 orderid,
				 orderstatus,
				 assetid,
				 direction,
				 amount,
				 payout,
				 opentime,
				 openprice,
				 closeprice,
				 ORDER_ANS_PASSIVE_TESG,
				 sgid,
				 teid,
				 GetMicroSecond());
		snetwork_xservice_db::MySqlDB db;
		db.Open();
		db.SelectDB(POSITION_DB_NAME);

		ssize_t rows = db.ExecSql(sqlbuf);
		db.Close();

		free(sqlbuf);
		free(json);
	});
#endif
}
/* TCP connect end */


}
