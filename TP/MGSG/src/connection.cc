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
#include<rapidjson/document.h>
#include<rapidjson/writer.h>
#include<rapidjson/rapidjson.h>
#include<rapidjson/stringbuffer.h>
#include"connection.h"
#include"tnode_adapter.h"
#include"ThreadPool.h"
// #include"xdatetime.h"
#include"object_pool.h"
#include"safe_queue.h"
#include"solution_config.h"
#include <syscall.h>
#include "simplebuf.h"

using namespace snetwork_xservice_xflagger;
extern SafeList<CMTSessionListNode> g_solution_list;

extern ThreadPool* g_threadPool;
using AsyncTPool = ObjectPool<AsyncT, ASYNC_T_SIZE>;
extern AsyncTPool* g_asyncTPool;

extern SafeQueue<AsyncT*> g_asyncTQueue;
std::map<uv_stream_t*, SimpleBuf*> TcpConnBuf;

namespace snetwork_xservice_solutiongateway {
constexpr unsigned char TAG = 0x8F;
static void Print(ssize_t nread, const uv_buf_t* buf) {
	printf("[TCP IN]\tflag=%d,nread=%zu,bufLen=%zu, message is:", nread - buf->len, nread, buf->len);
	for (size_t i=0; i<nread; ++i) {
		if (isprint(buf->base[i])) {
			printf("%c", buf->base[i]);
		} else {
			printf("%02X", buf->base[i]);
		}
	}
	printf("\n\n");
}

bool VerifySession(rapidjson::Document &d, CMTSessionListNode &node) {
	rapidjson::Value::ConstMemberIterator it;
	if (((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("field no exit or type error");
#endif

		return false;
	}
	const char *reqid = it->value.GetString();

	if (((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
		XINFO("field no exit or type error");
#endif

		return false;
	}
	int managerid = it->value.GetInt();

	if (((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
		XINFO("field no exit or type error");
#endif

		return false;
	}
	const char *session = it->value.GetString();

	node = CMTSessionListNode(session, UVTcpServer::GetSGID(), managerid);

	return true;
}

/* Tcp Server begin */
uv_loop_t *UVTcpServer::m_loop = nullptr;
uv_tcp_t UVTcpServer::m_server;
char UVTcpServer::m_strSGID[10];
char UVTcpServer::m_dbName[20];
int UVTcpServer::m_iSGID;
UVTcpServer::MapRequest UVTcpServer::m_mapReq;

std::once_flag UVTcpServer::m_once;
uv_buf_t UVTcpServer::m_buf;
std::map<void*, struct UVTcpServer::UV_BUF_T> UVTcpServer::m_map;

UVTcpServer::UVTcpServer() {
	Init();
	SetupMapRequest();
}

UVTcpServer::~UVTcpServer() {
	uv_loop_close(m_loop);
	int r = uv_loop_close(m_loop);
	if (r == UV_EBUSY) {
		XERROR("uv_loop_close error %s\n", uv_strerror(r));    /* code */
	} else {
		free(m_loop);
	}
}

void UVTcpServer::SetupMapRequest() {
	m_mapReq.insert(std::make_pair(ETag::ESUBSCRIBE, &UVTcpServer::Subscribtion));
	m_mapReq.insert(std::make_pair(ETag::ELOGIN, &UVTcpServer::Login));
	m_mapReq.insert(std::make_pair(ETag::ELOGOUT, &UVTcpServer::Logout));
	//m_mapReq.insert(std::make_pair(ETag::ECHANGEPASSWORD, &UVTcpServer::AdminReConnect));
	m_mapReq.insert(std::make_pair(ETag::ECHANGEPASSWORD, &UVTcpServer::ChangePassword));
	m_mapReq.insert(std::make_pair(ETag::ECHANGEBALANCE, &UVTcpServer::ChangeBalance));
	m_mapReq.insert(std::make_pair(ETag::EUPDATETRADER, &UVTcpServer::UpdateTrader));
	m_mapReq.insert(std::make_pair(ETag::EGETTRADERS, &UVTcpServer::GetTraders));
	m_mapReq.insert(std::make_pair(ETag::EINSERTUPDATEMANAGERRIGHTS, &UVTcpServer::InsertUpdateManagerRights));
	m_mapReq.insert(std::make_pair(ETag::EGETMANAGERACCESS, &UVTcpServer::GetManagerAccess));
	m_mapReq.insert(std::make_pair(ETag::EUSERDELETECHECKREQ, &UVTcpServer::UserDeleteCheckReq));
	m_mapReq.insert(std::make_pair(ETag::EUSERDELETEREQ, &UVTcpServer::UserDeleteReq));
	m_mapReq.insert(std::make_pair(ETag::ECREATETRADER, &UVTcpServer::CreateTrader));
	m_mapReq.insert(std::make_pair(ETag::EGETONLINETRADER, &UVTcpServer::GetOnlineTrader));

	m_mapReq.insert(std::make_pair(ETag::EDISABLETRADER, &UVTcpServer::DisableTrader));
	m_mapReq.insert(std::make_pair(ETag::EKICKOUTTRADER, &UVTcpServer::KickoutTrader));
	m_mapReq.insert(std::make_pair(ETag::EGETTRADERBYTRADERID, &UVTcpServer::GetTraderByTraderId));
	m_mapReq.insert(std::make_pair(ETag::EGETALLGROUPBYMANAGERID, &UVTcpServer::GetAllGroupByManagerId));
	m_mapReq.insert(std::make_pair(ETag::EENABLEDISABLEMANAGER, &UVTcpServer::EnabledIsableManager));

	m_mapReq.insert(std::make_pair(ETag::EGETPOSITIONSREQ, &UVTcpServer::GetPositionsReq));
	m_mapReq.insert(std::make_pair(ETag::EGETPOSITIONSREQX, &UVTcpServer::GetPositionsReqX));
	m_mapReq.insert(std::make_pair(ETag::EGETTOP5BOREQ, &UVTcpServer::GetTop5BoReq));
	m_mapReq.insert(std::make_pair(ETag::EGETCUSTMBOREQ, &UVTcpServer::GetCustomBoReq));
	m_mapReq.insert(std::make_pair(ETag::EGETSYMBOLBOREQ, &UVTcpServer::GetSymbolBoReq));
	m_mapReq.insert(std::make_pair(ETag::EGETTOP5BOREMOVEREQ, &UVTcpServer::GetTop5BoRemoveReq));
	m_mapReq.insert(std::make_pair(ETag::EGETCUSTMBOREMOVEREQ, &UVTcpServer::GetCustomBoRemoveReq));
	m_mapReq.insert(std::make_pair(ETag::EGETSYMBOLBOREMOVEREQ, &UVTcpServer::GetSymbolBoRemoveReq));
	m_mapReq.insert(std::make_pair(ETag::ECMTRECONNECT, &UVTcpServer::CmtReconnect));

	m_mapReq.insert(std::make_pair(ETag::EADMINRECONNECT, &UVTcpServer::AdminReConnect));
	m_mapReq.insert(std::make_pair(ETag::EADMINLOGINREQ, &UVTcpServer::AdminLoginReq));
	m_mapReq.insert(std::make_pair(ETag::EADMINLOGOUTREQ, &UVTcpServer::AdminLogoutReq));
	m_mapReq.insert(std::make_pair(ETag::EADMINGCHANGEPASSWORDREQ, &UVTcpServer::AdminChangePasswordReq));
	m_mapReq.insert(std::make_pair(ETag::ECREATEMANAGERREQ, &UVTcpServer::CreateManagerReq));
	m_mapReq.insert(std::make_pair(ETag::EUPDATEMANAGERREQ, &UVTcpServer::UpdateManagerReq));
	m_mapReq.insert(std::make_pair(ETag::EDELETEMANAGERREQ, &UVTcpServer::DeleteManagerReq));
	m_mapReq.insert(std::make_pair(ETag::EUPDATEMANAGERRIGHTREQ, &UVTcpServer::UpdateManagerRightReq));
	m_mapReq.insert(std::make_pair(ETag::EGETMANAGERRIGHTREQ, &UVTcpServer::GetManagerRightReq));
	m_mapReq.insert(std::make_pair(ETag::EGETADMINS, &UVTcpServer::GetAdmins));
	m_mapReq.insert(std::make_pair(ETag::EGETMANAGERSREQ, &UVTcpServer::GetManagersReq));
	m_mapReq.insert(std::make_pair(ETag::EDELETEADMIN, &UVTcpServer::DeleteAdmin));
	m_mapReq.insert(std::make_pair(ETag::ECREATEADMIN, &UVTcpServer::CreateAdmin));
	m_mapReq.insert(std::make_pair(ETag::EUPDATEADMIN, &UVTcpServer::UpdateAdmin));

	//for account
	m_mapReq.insert(std::make_pair(ETag::ECREATEACCOUNT, &UVTcpServer::CreateAccount));
	m_mapReq.insert(std::make_pair(ETag::EUPDATEACCOUNT, &UVTcpServer::UpdateAccount));
	m_mapReq.insert(std::make_pair(ETag::EGETALLACCOUNTS, &UVTcpServer::GetAllAccounts));
	m_mapReq.insert(std::make_pair(ETag::EGETACCOUNTSBYACCOUNTID, &UVTcpServer::GetAccountsByAccountId));
	//for group
	m_mapReq.insert(std::make_pair(ETag::ECREATEGROUP, &UVTcpServer::CreateGroup));
	m_mapReq.insert(std::make_pair(ETag::EUPDATEGROUP, &UVTcpServer::UpdateGroup));
	m_mapReq.insert(std::make_pair(ETag::EGETGROUPDETAILSBYID, &UVTcpServer::GetGroupDetailsById));
	m_mapReq.insert(std::make_pair(ETag::EGETALLGROUPFORGATEWAY, &UVTcpServer::GetAllGroupForGateWay));
	m_mapReq.insert(std::make_pair(ETag::EGETALLGROUP, &UVTcpServer::GetAllGroup));
	m_mapReq.insert(std::make_pair(ETag::EDELETEGROUP, &UVTcpServer::DeleteGroup));
	m_mapReq.insert(std::make_pair(ETag::EUPDATEGROUPSECURITY, &UVTcpServer::UpdateGroupSecurity));
	m_mapReq.insert(std::make_pair(ETag::EUPDATEGROUPREPORT, &UVTcpServer::UpdateGroupReport));
	m_mapReq.insert(std::make_pair(ETag::ESAVEMANAGERAGAINSTGROUPS, &UVTcpServer::SaveManagerAgainstGroups));
	m_mapReq.insert(std::make_pair(ETag::EGETALLGROUPSBYMANAGERID, &UVTcpServer::GetAllGroupsByManagerId));
	//for security and symbol
	m_mapReq.insert(std::make_pair(ETag::ECREATESECURITY, &UVTcpServer::CreateSecurity));
	m_mapReq.insert(std::make_pair(ETag::EUPDATESECURITY, &UVTcpServer::UpdateSecurity));
	m_mapReq.insert(std::make_pair(ETag::EGETALLSECURITY, &UVTcpServer::GetAllSecurity));
	m_mapReq.insert(std::make_pair(ETag::EDELETESECURITY, &UVTcpServer::DeleteSecurity));
	m_mapReq.insert(std::make_pair(ETag::ECREATESYMBOL, &UVTcpServer::CreateSymbol));
	m_mapReq.insert(std::make_pair(ETag::EUPDATESYMBOL, &UVTcpServer::UpdateSymbol));
	m_mapReq.insert(std::make_pair(ETag::EGETALLSYMBOL, &UVTcpServer::GetAllSymbol));
	m_mapReq.insert(std::make_pair(ETag::EGETSYMBOLBYID, &UVTcpServer::GetSymbolById));
	m_mapReq.insert(std::make_pair(ETag::EDELETESYMBOL, &UVTcpServer::DeleteSymbol));

	m_mapReq.insert(std::make_pair(ETag::ESYNCHDATA, &UVTcpServer::SynchData));
	
}

int UVTcpServer::Init(void) {
	//std::cout << uv_version_string() << std::endl;
	m_loop = uv_default_loop();
	uv_tcp_init(m_loop, &m_server);
	SXConfig *sxconfig = dynamic_cast<SXConfig *>(SXFlagger::GetInstance()->GetConfig());
	memset(m_strSGID, 0, sizeof(m_strSGID));
	strncpy(m_strSGID, sxconfig->SolutionID().c_str(), 10);
	m_iSGID = atoi(m_strSGID);
	memset(m_dbName, 0, sizeof(m_dbName));
	strncpy(m_dbName, sxconfig->MySqlDBName().c_str(), 20);
	uv_ip4_addr("0.0.0.0", sxconfig->SolutionPort(), &m_addr);
	uv_tcp_nodelay((uv_tcp_t *) &m_server, 1);
	uv_tcp_bind(&m_server, (const struct sockaddr *) &m_addr, 0);

	uv_tcp_keepalive((uv_tcp_t *) &m_server, 1, 60 * 5);
	uv_tcp_simultaneous_accepts((uv_tcp_t *) &m_server, 1);
	int r = uv_listen((uv_stream_t *) &m_server, SOLUTIONGATEWAY_BACKLOG, OnNewConnectionTcp);
	if (r) {
		XERROR("Listen error %s\n", uv_strerror(r));

		return 1;
	}

	AsyncT **asyncT = new AsyncT *[ASYNC_T_SIZE];
	for (int i = 0; i < ASYNC_T_SIZE; ++i) {
		asyncT[i] = g_asyncTPool->GetObject();
		uv_async_init(m_loop, &asyncT[i]->data, WriteReqCB);
	}

	for (int i = 0; i < ASYNC_T_SIZE; ++i) {
		g_asyncTPool->GetInstance()->ReleaseObject(asyncT[i]);
	}
	delete[] asyncT;

	return 0;
}


void UVTcpServer::AllocBuffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
	buf->base = (char *) malloc(suggested_size);
	memset(buf->base, 0, suggested_size);
	buf->len = suggested_size;
}

void UVTcpServer::OnClose(uv_handle_s *handle) {
	//FREE(&handle);
	//if node is not in g_solution_list, next step make no sence.
	CMTSessionListNode node;
	node.m_stream = (uv_stream_t*)handle;
	g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
		dst.m_stream = nullptr;
	});

	//free((uv_stream_t*)handle);

	std::map<void*, struct UV_BUF_T>::iterator it;
	it = m_map.find((void*)handle);

	if (it != m_map.end()) {
		free(it->second.base);
		m_map.erase(it);
	}

	if (handle != nullptr)
	{
		free(handle);
		//because of maybe close sync and reset
		handle = nullptr;
	}

#if defined(DEBUG)
	XINFO("disconnection\n");
#endif
}

int UVTcpServer::Run(void) {
	return uv_run(m_loop, UV_RUN_DEFAULT);
}

/* TCP connect begin */
void UVTcpServer::OnNewConnectionTcp(uv_stream_t *server, int status) {
	if (status < 0) {
		XERROR("New connection error %s\n", uv_strerror(status));

		return;
	}

#if defined(DEBUG)
	XINFO("on_new_connection\n");
#endif
	uv_stream_t *stream = (uv_stream_t *) malloc(sizeof(uv_stream_t));
	if (stream == nullptr) {
		XERROR("malloc fail");

		return;
	}
	memset(stream, 0, sizeof(uv_stream_t));

	uv_tcp_init(server->loop, (uv_tcp_t *) stream);
	uv_tcp_nodelay((uv_tcp_t *) stream, 1);
	uv_tcp_keepalive((uv_tcp_t *) &stream, 1, 60 * 5);
	if (uv_accept(server, (uv_stream_t *) stream) == 0) {
		uv_read_start((uv_stream_t *) stream, UVTcpServer::AllocBuffer, UVTcpServer::EchoReadTcp);
		//TcpConnBuf.insert(std::make_pair(stream, new SimpleBuf));
#if defined(DEBUG)
		//XINFO("uv_read_start end\n");
		struct sockaddr_in addr;
		int addrLen = sizeof(addr);
		uv_tcp_getpeername((uv_tcp_t*)stream, (struct sockaddr*)&addr, &addrLen);
		
		char ipAddrTmp[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &addr.sin_addr, ipAddrTmp, INET_ADDRSTRLEN);
		XINFO("tcp=%p, peername=%s, port=%d, uv_read_start end\n", stream, ipAddrTmp, ntohs(addr.sin_port));
#endif
	} else {
		uv_close((uv_handle_t *) stream, UVTcpServer::OnClose);//tcp won't be active???
#if defined(DEBUG)
		XINFO("tcp=%p, uv_close end\n", stream);
#endif
	}
#if defined(DEBUG)
	XINFO("tcp=%p, uv_*** end\n", stream);
#endif
}

void UVTcpServer::EchoReadTcp(uv_stream_t *handle, ssize_t nread, const uv_buf_t *buf) {
	if (nread < 0) {
		//fprintf(stderr, "ECHO  threadid:%ld, handle:%p\n",uv_thread_self(), handle);
		XINFO("tcp=%p, ECHO  threadid:%ld, handle:%p\n",handle, uv_thread_self(), handle);
		if (nread == UV_EOF) {
			XINFO("tcp=%p, Read error: client close|%s\n",handle, uv_err_name(nread));
		} else if (nread == UV_ECONNRESET) {
			XINFO("tcp=%p, Read error: connect reset|%s\n", handle, uv_err_name(nread));
		} else {
			XINFO("tcp=%p, Read error %s\n", uv_err_name(nread), handle);
		}
		free(buf->base);

		if (uv_is_active((uv_handle_t *) handle)) {
			uv_read_stop((uv_stream_t *) handle);
		}
		uv_close((uv_handle_t *) handle, UVTcpServer::OnClose);

		return;
	}

	//		std::map<uv_stream_t*, SimpleBuf*>::iterator it;
	//		if((it = TcpConnBuf.find(handle)) != TcpConnBuf.end()) {
	//			it->second->WriteBuf(buf->base, buf->len);
	//		}
	//printf("---buf->len=%d---nread=%d---\n", buf->len, nread);
	ParserJsonTcp((uv_stream_t *) handle, buf, nread);
	//ParserJsonTcp((uv_stream_t *)handle);
	free(buf->base);
}
#if 0
	void UVTcpServer::ParserJsonTcp(uv_stream_t *stream, const uv_buf_t *buf, ssize_t nread) {
		//printf("%lu, <%s>\n\n", nread, buf->base);
		if (nread <= sizeof(HeaderTag)) {
			return;
		}

		HeaderTag headerTag;
		headerTag.head = *(unsigned char *) buf->base;
		headerTag.tag = ntohs(*(unsigned short *) &buf->base[2]);
		headerTag.length = ntohl(*(unsigned int *) &buf->base[4]);
#if 0
        if (headerTag.head != 0x8F ||
            headerTag.length != (nread - sizeof(HeaderTag))) {
            return;
        }
#endif
		//char* body = NULL;
		//asprintf(&body, "%s", &buf->base[sizeof(HeaderTag)]);
		ssize_t hadRead = 0;
		while (true) {
			if (headerTag.head != 0x8F) {
				XERROR("Header invalid");

				return;
			}

			if (headerTag.length > nread) {
				XERROR("TCP error message");

				return;
			}

			//if nread-hadRead too large ,programm will exit!!!
			char *body = (char *) malloc(nread - hadRead);
			memcpy(body, &buf->base[8 + hadRead], headerTag.length);
			hadRead += headerTag.length + 8;

#if defined(DEBUG)
			XINFO("<Thread>:<%d>,tag=0x%02X,length=%d,body=%s", syscall(SYS_gettid), headerTag.tag, headerTag.length,
				  body);
#endif

			//to call the Request Function and deal with request
			MapRequest::iterator it;
			if ((it = m_mapReq.find(headerTag.tag)) != m_mapReq.end()) {
				// (this->*(it->second))(body, stream);
				//(UVTcpServer::*(it->second))(body, stream);
				(*(it->second))(body, stream);
			}

			std::this_thread::yield();

			// if (hadRead == nread) {
			// 	break;
			// }
			if (hadRead >= nread) {
				break;
			}

			headerTag.head = *(unsigned char *) &buf->base[hadRead];
			headerTag.tag = ntohs(*(unsigned short *) &buf->base[hadRead + 2]);
			headerTag.length = ntohl(*(unsigned int *) &buf->base[hadRead + 4]);

		}
	}
#endif

void UVTcpServer::ParserJsonTcp(uv_stream_t* stream, const uv_buf_t* buf, ssize_t nread) {
	static const size_t SIZE = 131070; /* 65535X2 */

	// typedef void (*SubParseJsonTcp) (stream_t* stream, char* body);
	// static SubParseJsonTcp subParseJsonTcp[ETag::ECHANGEPWD + 1];
	static std::map<void*, struct UV_BUF_T>::iterator it;
	static char* tmpBody = nullptr;
	static size_t tmpLen = 0; 
	static HeaderTag headerTag;

	std::call_once(m_once, []() {
		m_buf.base = (char*) malloc(SIZE);
		memset(m_buf.base, 0, SIZE);
		m_buf.len = 0;

		// subParseJsonTcp[ETag::ESENDORDER] = &UVTcpServer::SendOrder;
		// subParseJsonTcp[ETag::ELOGIN] = &UVTcpServer::Login;
		// subParseJsonTcp[ETag::ELOGOUT] = &UVTcpServer::Logout;
		// subParseJsonTcp[ETag::ECHANGEPWD] = &UVTcpServer::ChangePWD;
	});

	bool findFlag = false;

	if(buf->base[0] == (char)TAG) {
		/* normal parese */
		tmpBody = buf->base;
		tmpLen = nread;//(buf->len = suggestsize > nread + 8)?
	} else {
		/* maybe request sent multiple time */
		/* first find the map if exist */
		it = m_map.find((void*)stream);
		if (it != m_map.end() &&
			*it->second.base == (char)TAG) {
			/* exist */
			it->second.len += nread;
			if (it->second.len > it->second.capacity) {
				size_t cnt = it->second.len / m_msgLen;
				it->second.capacity = (m_msgLen * (1+cnt));
				it->second.base = (char*)realloc(it->second.base, it->second.capacity);
			}
			memcpy(&it->second.base[it->second.len - nread], buf->base, nread);
			
			tmpBody = it->second.base;
			tmpLen = it->second.len;
			findFlag = true;
		} else {
			return;
		}
	}

	if (tmpLen <= 8) {
		UV_BUF_T elem;
		elem.len = tmpLen;
		if (tmpLen < m_msgLen) {
			elem.capacity = m_msgLen;
			elem.base = (char*)calloc(m_msgLen, 1);
			memcpy(elem.base, tmpBody, elem.len);
		} else {
			elem.capacity = tmpLen;
			elem.base = (char*)calloc(tmpLen, 1);
			memcpy(elem.base, tmpBody, elem.len);
		}

		std::map<void*, struct UV_BUF_T>::iterator it = m_map.find((void*)stream);
		if (it != m_map.end()) {
			free(it->second.base);
			m_map.erase(it);
		}

		m_map.insert(std::pair<void*, struct UV_BUF_T>((void*)stream, elem));

		return;
	}

	headerTag.head = *(unsigned char*)tmpBody;
	headerTag.tag = ntohs(*(unsigned short*)&tmpBody[2]);
	headerTag.length = ntohl(*(unsigned int*)&tmpBody[4]);

	if (headerTag.length + 8 > tmpLen) {
#if 0
		/* maybe message was split many port */
		if (findFlag) {
			return;
		}
#endif

		UV_BUF_T elem;
		elem.len = tmpLen;
		if (tmpLen < m_msgLen) {
			elem.capacity = m_msgLen;
			elem.base = (char*)malloc(m_msgLen);
			memcpy(elem.base, tmpBody, elem.len);
		} else {
			elem.capacity = tmpLen;
			elem.base = (char*)malloc(tmpLen);
			memcpy(elem.base, tmpBody, elem.len);
		}

		std::map<void*, struct UV_BUF_T>::iterator it = m_map.find((void*)stream);
		if (it != m_map.end()) {
			free(it->second.base);
			m_map.erase(it);
		}

		m_map.insert(std::pair<void*, struct UV_BUF_T>((void*)stream, elem));

		return;
	}

	ssize_t hadRead = 0;
	while(true) {
		char* body = (char*)malloc(headerTag.length);
		memcpy(body, &tmpBody[8+hadRead], headerTag.length);

#if defined(DEBUG)
		XINFO("tag=0x%02X,length=%d,body=%s", headerTag.tag, headerTag.length, body);
#endif

		MapRequest::iterator itReq;
		if ((itReq = m_mapReq.find(headerTag.tag)) != m_mapReq.end()) {
			(*(itReq->second))(body, stream);
		}
	
		hadRead += headerTag.length + 8;

		if (hadRead < tmpLen) {
			headerTag.head = *(unsigned char*)&tmpBody[hadRead];
			if (headerTag.head == TAG) {
				if (hadRead + 8 <= tmpLen) {
					headerTag.tag = ntohs(*(unsigned short*)&tmpBody[hadRead+2]);
					headerTag.length = ntohl(*(unsigned int*)&tmpBody[hadRead+4]);

					if (hadRead + 8 + headerTag.length <= tmpLen) {
						continue;
					} else {
						/* maybe message was split many part */
						if (findFlag) {
							it->second.len = tmpLen - hadRead;
							memmove(it->second.base, &it->second.base[hadRead], it->second.len);

							return;
						} else {
							UV_BUF_T elem;
							elem.len = tmpLen - hadRead;
							if (elem.len < m_msgLen) {
								elem.capacity = m_msgLen;
								elem.base = (char*)malloc(m_msgLen);
								memcpy(elem.base, &tmpBody[hadRead], elem.len);
							} else {
								elem.capacity = elem.len;
								elem.base = (char*)malloc(elem.len);
								memcpy(elem.base, &tmpBody[hadRead], elem.len);
							}

							std::map<void*, struct UV_BUF_T>::iterator it = m_map.find((void*)stream);
							if (it != m_map.end()) {
								free(it->second.base);
								m_map.erase(it);
							}

							m_map.insert(std::pair<void*, struct UV_BUF_T>((void*)stream, elem));

							return;
						}
					}
				} else {
					if (findFlag) {
						it->second.len = tmpLen - hadRead;
						memmove(it->second.base, &it->second.base[hadRead], it->second.len);

						return;
					} else {
						UV_BUF_T elem;
						elem.len = tmpLen - hadRead;
						if (elem.len < m_msgLen) {
							elem.capacity = m_msgLen;
							elem.base = (char*)calloc(m_msgLen, 1);
							memcpy(elem.base, &tmpBody[hadRead], elem.len);
						} else {
							elem.capacity = elem.len;
							elem.base = (char*)calloc(elem.len, 1);
							memcpy(elem.base, &tmpBody[hadRead], elem.len);
						}

						std::map<void*, struct UV_BUF_T>::iterator it = m_map.find((void*)stream);
						if (it != m_map.end()) {
							free(it->second.base);
							m_map.erase(it);
						}

						m_map.insert(std::pair<void*, struct UV_BUF_T>((void*)stream, elem));

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

void UVTcpServer::ParserJsonTcp(uv_stream_t *stream) {
	std::map<uv_stream_t*, SimpleBuf*>::iterator it;
	if((it = TcpConnBuf.find(stream)) == TcpConnBuf.end()) {
		return;
	}

	int nread= it->second->GetBufLenght();
	if (nread <= sizeof(HeaderTag)) {
		return;
	}

	char* buf= it->second->GetBuf();
	HeaderTag headerTag;
	headerTag.head = *(unsigned char *) buf;
	headerTag.tag = ntohs(*(unsigned short *) &buf[2]);
	headerTag.length = ntohl(*(unsigned int *) &buf[4]);

	ssize_t hadRead = 0;
	while (true) {
		if (headerTag.head != 0x8F) {
			XERROR("Header invalid");

			return;
		}

		if ((headerTag.length + 8) > nread) {
			XERROR("TCP error message");

			return;
		}

		//if nread-hadRead too large ,programm will exit!!!
		char *body = (char *) malloc(nread - hadRead);
		memcpy(body, &buf[8 + hadRead], headerTag.length);
		hadRead += headerTag.length + 8;

#if defined(DEBUG)
		XINFO("<Thread>:<%d>,tag=0x%02X,length=%d,body=%s", syscall(SYS_gettid), headerTag.tag, headerTag.length,
			  body);
#endif

		//to call the Request Function and deal with request
		MapRequest::iterator it;
		if ((it = m_mapReq.find(headerTag.tag)) != m_mapReq.end()) {
			// (this->*(it->second))(body, stream);
			//(UVTcpServer::*(it->second))(body, stream);
			(*(it->second))(body, stream);
		}

		//std::this_thread::yield();

		if (hadRead == nread) {
			break;
		}

		headerTag.head = *(unsigned char *) &buf[hadRead];
		headerTag.tag = ntohs(*(unsigned short *) &buf[hadRead + 2]);
		headerTag.length = ntohl(*(unsigned int *) &buf[hadRead + 4]);

	}
}

void UVTcpServer::WriteCB(uv_write_t *req, int status) {
	write_req_t* wr;
	wr = (write_req_t*) req;

	//int written = wr->buf.len;
	if (status != 0) {
		XERROR("async write", uv_strerror(status));
	}

	free(wr->buf.base);
	free(wr);
}

void UVTcpServer::Subscribtion(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
	#if defined(DEBUG)	
			XINFO("field no exit or type error");
	#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
	#if defined(DEBUG)	
			XINFO("field no exit or type error");
	#endif

			return;
		}
		int managerid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
	#if defined(DEBUG)	
			XINFO("field no exit or type error");
	#endif

			return;
		}
		const char *session = it->value.GetString();

		//if(((it = d.FindMember("body")) == d.MemberEnd()) || !it->value.IsString()) {
		if(((it = d.FindMember("body")) == d.MemberEnd()) || !it->value.IsArray()) {
	#if defined(DEBUG)	
			XINFO("field no exit or type error");
	#endif

			return;
		}
		const rapidjson::Value& body = d["body"];
		//const rapidjson::Value& body = it->value.GetArray();

		CMTSessionListNode node(session, m_iSGID, managerid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			// for (rapidjson::SizeType i = 0; i < it->value.GetArray().Size(); i++) {
			// 	if (it->value.GetArray()[i].IsInt()) {
			// 		XINFO("a[%d] = %d\n", i, it->value.GetArray()[i].GetInt());
			// 	} else if (it->value.GetArray()[i].IsInt()) {
			// 		XINFO("a[%s] = %s\n", i, it->value.GetArray()[i].GetString());
			// 	} else {
			// 		return;
			// 	}
			// }

			dst.m_subscribtion.reset();
			for (rapidjson::SizeType i = 0; i < body.Size(); i++) {
				int index = 0;
				if(body[i].IsInt() && ((index = body[i].GetInt()) < 2500)) {
	#if defined(DEBUG)	
					XINFO("body[%d] = %x\n", i, index);
	#endif
					dst.m_subscribtion.set(index);
				}
			}
		});
	});
}

void UVTcpServer::Login(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
	#if defined(DEBUG)	
			XINFO("field no exit or type error");
	#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
	#if defined(DEBUG)	
			XINFO("field no exit or type error");
	#endif

			return;
		}
		int managerid = it->value.GetInt();

		/* add the peer name */
		struct sockaddr_in addr;
		int addrLen = sizeof(addr);
		uv_tcp_getpeername((uv_tcp_t*)stream, (struct sockaddr*)&addr, &addrLen);
		//uv_tcp_getsockname((uv_tcp_t*)stream, (struct sockaddr*)&addr, &addrLen);
		char ipAddrTmp[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &addr.sin_addr, ipAddrTmp, INET_ADDRSTRLEN);

		rapidjson::Value ipAddr(ipAddrTmp, d.GetAllocator());
		//d.AddMember("ipadress", ipAddr, d.GetAllocator());
		d.AddMember("addr", ipAddr, d.GetAllocator());

		/*add the management solution gateway id*/
		d.AddMember("sgid", m_iSGID, d.GetAllocator());
		/* create uuid for login */
		uuid_t uuid;
		char session[33];
		uuid_generate_time_safe(uuid);
		DecToHex(session, (char*)&uuid);
		session[32] = 0;
		rapidjson::Value sessionid(session, d.GetAllocator());
		d.AddMember("session", sessionid, d.GetAllocator());
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		d.Accept(writer);

		/*publish LOGIN.CM:  */
		TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_LOGIN_NAME, 
													   SOLUTIONGATEWAY_PUBLISH_CMT_EVENT,
													   buffer.GetString(),
													   buffer.GetLength()+1,
													   ETag::ELOGIN);

		CMTSessionListNode node(session, m_iSGID, managerid);
		node.m_stream = stream;
		node.m_clienttype = ECLIENTTYPE::ECMT;
		node.m_subscribtion.set();
		g_solution_list.PushFront(std::move(node));
	});
}

void UVTcpServer::Logout(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
	#if defined(DEBUG)	
			XINFO("field no exit or type error");
	#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
	#if defined(DEBUG)	
			XINFO("field no exit or type error");
	#endif

			return;
		}
		int managerid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
	#if defined(DEBUG)	
			XINFO("field no exit or type error");
	#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, managerid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
		
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			//d.AddMember("sgid", UVTcpServer::GetSGID(), d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_LOGOUT_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_CMT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::ELOGOUT);
			dst.m_status = ELogin::ELOGINEXIT;
		});
	});
}

void UVTcpServer::ChangePassword(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int managerid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, managerid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_CHANGEPASSWORD_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_CMT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::ECHANGEPASSWORD);
		});
	});
}

void UVTcpServer::ChangeBalance(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int managerid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, managerid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_CHANGEBALANCE_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_CMT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::ECHANGEBALANCE);
		});
	});
}

void UVTcpServer::UpdateTrader(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error\n");
		free(body);

		return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		int managerid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, managerid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
		if (dst.m_status != ELogin::ELOGINSUCCESS) {

			return;
		}
		d.AddMember("sgid", m_iSGID, d.GetAllocator());
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		d.Accept(writer);

		TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_UPDATETRADER_NAME, 
													   SOLUTIONGATEWAY_PUBLISH_CMT_EVENT,
													   buffer.GetString(), 
													   buffer.GetLength()+1, 
													   ETag::EUPDATETRADER);
		});
	});
}

void UVTcpServer::GetTraders(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error\n");
		free(body);

		return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		int managerid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, managerid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {

		if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
		}
		d.AddMember("sgid", m_iSGID, d.GetAllocator());
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		d.Accept(writer);

		TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_GETTRADERS_NAME, 
													   SOLUTIONGATEWAY_PUBLISH_CMT_EVENT,
													   buffer.GetString(), 
													   buffer.GetLength()+1, 
													   ETag::EGETTRADERS);
		});
	});
}

void UVTcpServer::InsertUpdateManagerRights(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error\n");
		free(body);

		return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		int managerid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, managerid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
		if (dst.m_status != ELogin::ELOGINSUCCESS) {

			return;
		}
		d.AddMember("sgid", m_iSGID, d.GetAllocator());
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		d.Accept(writer);

		TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_INSERTUPDATEMANAGERRIGHTS_NAME, 
													   SOLUTIONGATEWAY_PUBLISH_CMT_EVENT,
													   buffer.GetString(), 
													   buffer.GetLength()+1, 
													   ETag::EINSERTUPDATEMANAGERRIGHTS);
		});
	});
}

void UVTcpServer::GetManagerAccess(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error\n");
		free(body);

		return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		int managerid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, managerid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
		if (dst.m_status != ELogin::ELOGINSUCCESS) {

			return;
		}
		d.AddMember("sgid", m_iSGID, d.GetAllocator());
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		d.Accept(writer);

		TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_GETMANAGERACCESS_NAME, 
													   SOLUTIONGATEWAY_PUBLISH_CMT_EVENT,
													   buffer.GetString(), 
													   buffer.GetLength()+1, 
													   ETag::EGETMANAGERACCESS);
		});
	});
}

void UVTcpServer::UserDeleteCheckReq(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error\n");
		free(body);

		return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		int managerid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, managerid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
		if (dst.m_status != ELogin::ELOGINSUCCESS) {

			return;
		}
		d.AddMember("sgid", m_iSGID, d.GetAllocator());
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		d.Accept(writer);

		TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_USERDELETECHECKREQ_NAME, 
													   SOLUTIONGATEWAY_PUBLISH_CMT_EVENT,
													   buffer.GetString(), 
													   buffer.GetLength()+1, 
													   ETag::EUSERDELETECHECKREQ);
		});
	});
}

void UVTcpServer::UserDeleteReq(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error\n");
		free(body);

		return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		int managerid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, managerid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
		if (dst.m_status != ELogin::ELOGINSUCCESS) {

			return;
		}
		d.AddMember("sgid", m_iSGID, d.GetAllocator());
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		d.Accept(writer);

		TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_USERDELETEREQ_NAME, 
													   SOLUTIONGATEWAY_PUBLISH_CMT_EVENT,
													   buffer.GetString(), 
													   buffer.GetLength()+1, 
													   ETag::EUSERDELETEREQ);
		});
	});
}

void UVTcpServer::CreateTrader(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error\n");
		free(body);

		return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		int managerid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, managerid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
		if (dst.m_status != ELogin::ELOGINSUCCESS) {

			return;
		}
		d.AddMember("sgid", m_iSGID, d.GetAllocator());
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		d.Accept(writer);

		TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_CREATETRADER_NAME, 
													   SOLUTIONGATEWAY_PUBLISH_CMT_EVENT,
													   buffer.GetString(), 
													   buffer.GetLength()+1, 
													   ETag::ECREATETRADER);
		});
	});
}

void UVTcpServer::GetOnlineTrader(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error\n");
		free(body);

		return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		int managerid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, managerid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {	
		if (dst.m_status != ELogin::ELOGINSUCCESS) {

			return;
		}
		d.AddMember("sgid", m_iSGID, d.GetAllocator());
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		d.Accept(writer);

		TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_GETONLINETRADER_NAME, 
													   SOLUTIONGATEWAY_PUBLISH_CMT_EVENT,
													   buffer.GetString(), 
													   buffer.GetLength()+1, 
													   ETag::EGETONLINETRADER);
		});
	});
}

void UVTcpServer::DisableTrader(char* body, uv_stream_t* stream){
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int managerid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, managerid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_DISABLETRADER_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_CMT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EDISABLETRADER);
		});
	});
}

void UVTcpServer::KickoutTrader(char* body, uv_stream_t* stream){
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int managerid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, managerid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_KICKOUTTRADER_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_CMT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EKICKOUTTRADER);
		});
	});
}

void UVTcpServer::GetTraderByTraderId(char* body, uv_stream_t* stream){
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int managerid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, managerid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_GETTRADERBYTRADERID_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_CMT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EGETTRADERBYTRADERID);
		});
	});
}

void UVTcpServer::GetAllGroupByManagerId(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int managerid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, managerid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_GETALLGROUPBYMANAGERID_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_CMT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EGETALLGROUPBYMANAGERID);
		});
	});
}

void UVTcpServer::EnabledIsableManager(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int managerid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, managerid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_ENABLEDISABLEMANAGER_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EENABLEDISABLEMANAGER);
		});
	});
}

//for RM/7
void UVTcpServer::GetPositionsReq(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error\n");
		free(body);

		return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		int managerid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, managerid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {

		if (dst.m_status != ELogin::ELOGINSUCCESS) {

			return;
		}
		d.AddMember("sgid", m_iSGID, d.GetAllocator());
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		d.Accept(writer);

		TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_GETPOSITIONSREQ_NAME, 
													   SOLUTIONGATEWAY_PUBLISH_RM_EVENT,
													   buffer.GetString(), 
													   buffer.GetLength()+1, 
													   ETag::EGETPOSITIONSREQ);
		});
	});
}

void UVTcpServer::GetPositionsReqX(char* body, uv_stream_t* stream) {
		g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error\n");
		free(body);

		return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		int managerid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, managerid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {

		if (dst.m_status != ELogin::ELOGINSUCCESS) {

			return;
		}
		d.AddMember("sgid", m_iSGID, d.GetAllocator());
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		d.Accept(writer);

		TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_GETPOSITIONSREQX_NAME, 
													   SOLUTIONGATEWAY_PUBLISH_RM_EVENT,
													   buffer.GetString(), 
													   buffer.GetLength()+1, 
													   ETag::EGETPOSITIONSREQX);
		});
	});
}


void UVTcpServer::GetTop5BoReq(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error\n");
		free(body);

		return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		int managerid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *session = it->value.GetString();

		if(((it = d.FindMember("direction")) == d.MemberEnd()) || !it->value.IsInt()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		int direction = it->value.GetInt();

		CMTSessionListNode node(session, m_iSGID, managerid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if(direction > -1 && direction < 2) {
				dst.m_subs.subtop5bo[direction] = true;
			}

			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_GETTOP5BOREQ_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_RM_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EGETTOP5BOREQ);
		});
	});
}

void UVTcpServer::GetCustomBoReq(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error\n");
		free(body);

		return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		int managerid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *session = it->value.GetString();

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

		CMTSessionListNode node(session, m_iSGID, managerid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			// if((direction > -1 && direction < 2) && (assetid > -1 && assetid < 72)) {
			// 	dst.m_subs.subcustomsymbol[direction].set(assetid);//subscribe the symbol
			// }
			if((direction > -1 && direction < 2) && (assetid > 10000 && assetid < 22000)) {
				if (dst.m_subs.subcustomsymbolmap[direction].find(assetid) != dst.m_subs.subcustomsymbolmap[direction].end()) {
					dst.m_subs.subcustomsymbolmap[direction][assetid] = true;
				} else {
					dst.m_subs.subcustomsymbolmap[direction].insert(std::make_pair(assetid, true));//subscribe the symbol					
				}
			}

			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_GETCUSTMBOREQ_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_RM_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EGETCUSTMBOREQ);
		});
	});
}

void UVTcpServer::GetSymbolBoReq(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error\n");
		free(body);

		return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		int managerid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, managerid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			dst.m_subs.subsystemsymbol = true;

			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_GETSYMBOLBOREQ_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_RM_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EGETSYMBOLBOREQ);
		});
	});
}

void UVTcpServer::GetTop5BoRemoveReq(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error\n");
		free(body);

		return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		int managerid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *session = it->value.GetString();

		if(((it = d.FindMember("direction")) == d.MemberEnd()) || !it->value.IsInt()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		int direction = it->value.GetInt();

		CMTSessionListNode node(session, m_iSGID, managerid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if(direction > -1 && direction < 2) {
				dst.m_subs.subtop5bo[direction] = false;
			}

			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_GETTOP5BOREMOVEREQ_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_RM_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EGETTOP5BOREMOVEREQ);
		});
	});
}		

void UVTcpServer::GetCustomBoRemoveReq(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error\n");
		free(body);

		return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		int managerid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *session = it->value.GetString();

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

		CMTSessionListNode node(session, m_iSGID, managerid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			// if((direction > -1 && direction < 2) && (assetid > -1 && assetid < 72)) {
			// 	dst.m_subs.subcustomsymbol[direction].reset(assetid);//unsubscribe the symbol
			// }

			if((direction > -1 && direction < 2) && (assetid > 10000 && assetid < 22000)) {
				if (dst.m_subs.subcustomsymbolmap[direction].find(assetid) != dst.m_subs.subcustomsymbolmap[direction].end()) {
					dst.m_subs.subcustomsymbolmap[direction][assetid] = false;
				}
			}

			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_GETCUSTMBOREMOVEREQ_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_RM_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EGETCUSTMBOREMOVEREQ);
		});
	});
}	

void UVTcpServer::GetSymbolBoRemoveReq(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error\n");
		free(body);

		return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		int managerid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, managerid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			dst.m_subs.subsystemsymbol = false;

			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_GETSYMBOLBOREMOVEREQ_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_RM_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EGETSYMBOLBOREMOVEREQ);
		});
	});
}

void UVTcpServer::CmtReconnect(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
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
		uv_tcp_getpeername((uv_tcp_t*)stream, (struct sockaddr*)&addr, &addrLen);
		char ipAddrTmp[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &addr.sin_addr, ipAddrTmp, INET_ADDRSTRLEN);
		rapidjson::Value ipAddr(ipAddrTmp, d.GetAllocator());

		d.AddMember("addr", ipAddr, d.GetAllocator());
		d.AddMember("sgid", m_iSGID, d.GetAllocator());

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		d.Accept(writer);

		rapidjson::Value::ConstMemberIterator it;

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
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
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("managerid")) == d.MemberEnd()) || !it->value.IsInt()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		int managerid = it->value.GetInt();

		CMTSessionListNode node(session, m_iSGID, managerid);
		bool oringinConn = false;
		bool routineExit = false;
		int ret = g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_stream == stream) {
				oringinConn = true;

				return;
			}
			//this condition is used for recognizing whether the related TCP source recyced.
			if (dst.m_stream != nullptr) {//relogin on the same tcp invalid
				uv_close((uv_handle_t*)dst.m_stream, OnClose);
				dst.m_stream = stream;

			} else {
				routineExit = true;
			}

		});

		if (0 == ret) {
			node.m_stream = stream;
			node.m_clienttype = ECLIENTTYPE::ECMT;
			g_solution_list.PushFront(std::move(node));
		}
		
		/*publish LOGIN.CLIE:  */
		if (!oringinConn && !routineExit) {//routine logout need to filte
			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_CMTRECONNECT_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_CMT_EVENT,
														   buffer.GetString(),
														   buffer.GetLength()+1,
														   ETag::ECMTRECONNECT);
		}
	});
}

//for admin
void UVTcpServer::AdminReConnect(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
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
		uv_tcp_getpeername((uv_tcp_t*)stream, (struct sockaddr*)&addr, &addrLen);
		char ipAddrTmp[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &addr.sin_addr, ipAddrTmp, INET_ADDRSTRLEN);
		rapidjson::Value ipAddr(ipAddrTmp, d.GetAllocator());

		d.AddMember("addr", ipAddr, d.GetAllocator());
		d.AddMember("sgid", m_iSGID, d.GetAllocator());

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		d.Accept(writer);

		rapidjson::Value::ConstMemberIterator it;

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
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
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		int adminid = it->value.GetInt();

		CMTSessionListNode node(session, m_iSGID, adminid);
		bool oringinConn = false;
		bool routineExit = false;
		int ret = g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_stream == stream) {
				oringinConn = true;

				return;
			}
			//this condition is used for recognizing whether the related TCP source recyced.
			if (dst.m_stream != nullptr) {//relogin on the same tcp invalid
				uv_close((uv_handle_t*)dst.m_stream, OnClose);
				dst.m_stream = stream;

			} else {
				routineExit = true;
			}

		});

		if (0 == ret) {
			node.m_stream = stream;
			node.m_clienttype = ECLIENTTYPE::EADT;
			g_solution_list.PushFront(std::move(node));
		}
		
		/*publish LOGIN.CLIE:  */
		if (!oringinConn && !routineExit) {//routine logout need to filte
			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_ADMINRECONNECT_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(),
														   buffer.GetLength()+1,
														   ETag::EADMINRECONNECT);
		}
	});	
}

void UVTcpServer::AdminLoginReq(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error\n");
		free(body);

		return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		int adminid = it->value.GetInt();

		/*add the management solution gateway id*/
		d.AddMember("sgid", m_iSGID, d.GetAllocator());
		/* create uuid for login */
		uuid_t uuid;
		char session[33];
		uuid_generate_time_safe(uuid);
		DecToHex(session, (char*)&uuid);
		session[32] = 0;
		rapidjson::Value sessionid(session, d.GetAllocator());
		d.AddMember("session", sessionid, d.GetAllocator());
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		d.Accept(writer);

		/*publish LOGIN.CMT:  */
		TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_ADMINLOGINREQ_NAME, 
												   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
												   buffer.GetString(),
												   buffer.GetLength()+1,
												   ETag::EADMINLOGINREQ);

		CMTSessionListNode node(session, m_iSGID, adminid);
		node.m_stream = stream;
		node.m_clienttype = ECLIENTTYPE::EADT;
		g_solution_list.PushFront(std::move(node));
	});
		
}

void UVTcpServer::AdminLogoutReq(char* body, uv_stream_t* stream) {
/*logout */
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error\n");
		free(body);

		return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
		XINFO("field no exit or type error");
		#endif

		return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {

		if (dst.m_status != ELogin::ELOGINSUCCESS) {

			return;
		}

		d.AddMember("sgid", m_iSGID, d.GetAllocator());
		//d.AddMember("sgid", UVTcpServer::GetSGID(), d.GetAllocator());
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		d.Accept(writer);

		TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_ADMINLOGOUTREQ_NAME, 
													   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
													   buffer.GetString(), 
													   buffer.GetLength()+1, 
													   ETag::EADMINLOGOUTREQ);
		dst.m_status = ELogin::ELOGINEXIT;
		});
	});
}

void UVTcpServer::AdminChangePasswordReq(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
			XINFO("field no exit or type error");
		#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
		#if defined(DEBUG)	
			XINFO("field no exit or type error");
		#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
		#if defined(DEBUG)	
			XINFO("field no exit or type error");
		#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_ADMINGCHANGEPASSWORDREQ_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EADMINGCHANGEPASSWORDREQ);
		});
	});
}

void UVTcpServer::CreateManagerReq(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_CREATEMANAGERREQ_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::ECREATEMANAGERREQ);
		});
	});
}

void UVTcpServer::UpdateManagerReq(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS || adminid != dst.m_managerID) {
				//filter directly and no respone back to ADT
				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_UPDATEMANAGERREQ_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EUPDATEMANAGERREQ);
		});
	});
}

void UVTcpServer::DeleteManagerReq(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_DELETEMANAGERREQ_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EDELETEMANAGERREQ);
		});
	});
}

void UVTcpServer::UpdateManagerRightReq(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_UPDATEMANAGERRIGHTREQ_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EUPDATEMANAGERRIGHTREQ);
		});
	});
}

void UVTcpServer::GetManagerRightReq(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_GETMANAGERRIGHTREQ_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EGETMANAGERRIGHTREQ);
		});
	});
}

void UVTcpServer::GetAdmins(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_GETADMINS_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EGETADMINS);
		});
	});
}

void UVTcpServer::DeleteAdmin(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_DELETEADMIN_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EDELETEADMIN);
		});
	});
}

void UVTcpServer::CreateAdmin(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

// 		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
// #if defined(DEBUG)	
// 			XINFO("field no exit or type error");
// #endif

// 			return;
// 		}
// 		int adminid = it->value.GetInt();
		if(((it = d.FindMember("superadminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
			XINFO("filed no exit or type error\n");
#endif
			return;		
		}
		int superadminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, superadminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_CREATEADMIN_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::ECREATEADMIN);
		});
	});
}

void UVTcpServer::UpdateAdmin(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
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

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, superadminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_UPDATEADMIN_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EUPDATEADMIN);
		});
	});
}


void UVTcpServer::GetManagersReq(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_GETMANAGERSREQ_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EGETMANAGERSREQ);
		});
	});
}

//for admin account
void UVTcpServer::CreateAccount(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_CREATEACCOUNT_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::ECREATEACCOUNT);
		});
	});
}

void UVTcpServer::UpdateAccount(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_UPDATEACCOUNT_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EUPDATEACCOUNT);
		});
	});
}

void UVTcpServer::GetAllAccounts(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_GETALLACCOUNTS_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EGETALLACCOUNTS);
		});
	});
}

void UVTcpServer::GetAccountsByAccountId(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_GETACCOUNTSBYACCOUNTID_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EGETACCOUNTSBYACCOUNTID);
		});
	});
}

//for admin group
 void UVTcpServer::CreateGroup(char* body, uv_stream_t* stream) {
 		g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_CREATEGROUP_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::ECREATEGROUP);
		});
	});
 }

 void UVTcpServer::UpdateGroup(char* body, uv_stream_t* stream) {
 		g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_UPDATEGROUP_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EUPDATEGROUP);
		});
	});
 }

 void UVTcpServer::GetGroupDetailsById(char* body, uv_stream_t* stream) {
 		g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_GETGROUPDETAILSBYID_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EGETGROUPDETAILSBYID);
		});
	});
 }

 void UVTcpServer::GetAllGroupForGateWay(char* body, uv_stream_t* stream) {
 		g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_GETALLGROUPFORGATEWAY_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EGETALLGROUPFORGATEWAY);
		});
	});
 }

 void UVTcpServer::GetAllGroup(char* body, uv_stream_t* stream) {
 		g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_GETALLGROUP_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EGETALLGROUP);
		});
	});
 }

 void UVTcpServer::DeleteGroup(char* body, uv_stream_t* stream) {
 		g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_DELETEGROUP_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EDELETEGROUP);
		});
	});
 }

 void UVTcpServer::UpdateGroupSecurity(char* body, uv_stream_t* stream) {
 		g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_UPDATEGROUPSECURITY_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EUPDATEGROUPSECURITY);
		});
	});
 }

 void UVTcpServer::UpdateGroupReport(char* body, uv_stream_t* stream) {
 		g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_UPDATEGROUPREPORT_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EUPDATEGROUPREPORT);
		});
	});
 }

void UVTcpServer::SaveManagerAgainstGroups(char* body, uv_stream_t* stream) {
	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_SAVEMANAGERAGAINSTGROUPS_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::ESAVEMANAGERAGAINSTGROUPS);
		});
	});
}
void UVTcpServer::GetAllGroupsByManagerId(char* body, uv_stream_t* stream) {
 	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_GETALLGROUPSBYMANAGERID_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EGETALLGROUPSBYMANAGERID);
		});
	});
}

//for security
 void UVTcpServer::CreateSecurity(char* body, uv_stream_t* stream) {
 	 	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_CREATESECURITY_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::ECREATESECURITY);
		});
	});
 }

 void UVTcpServer::UpdateSecurity(char* body, uv_stream_t* stream) {
 	 	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_UPDATESECURITY_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EUPDATESECURITY);
		});
	});
 }

 void UVTcpServer::GetAllSecurity(char* body, uv_stream_t* stream) {
 	 	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_GETALLSECURITY_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EGETALLSECURITY);
		});
	});
 }
 void UVTcpServer::DeleteSecurity(char* body, uv_stream_t* stream) {
 	 	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_DELETESECURITY_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EDELETESECURITY);
		});
	});
 }
 void UVTcpServer::CreateSymbol(char* body, uv_stream_t* stream) {
 	 	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_CREATESYMBOL_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::ECREATESYMBOL);
		});
	});
 }
 //for symbol
 void UVTcpServer::UpdateSymbol(char* body, uv_stream_t* stream) {
 	 	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		// d.MemberEnd();
		// printf("%s\n", "d.MemberEnd() is no problem!");
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_UPDATESYMBOL_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EUPDATESYMBOL);
		});
	});
 }
 void UVTcpServer::GetAllSymbol(char* body, uv_stream_t* stream) {
 	 	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_GETALLSYMBOL_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EGETALLSYMBOL);
		});
	});
 }
 void UVTcpServer::GetSymbolById(char* body, uv_stream_t* stream) {
 	 	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_GETSYMBOLBYID_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EGETSYMBOLBYID);
		});
	});
 }
 void UVTcpServer::DeleteSymbol(char* body, uv_stream_t* stream) {
 	 	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_DELETESYMBOL_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::EDELETESYMBOL);
		});
	});
 }



void UVTcpServer::SynchData(char* body, uv_stream_t* stream) {
 	 	g_threadPool->Enqueue([body, stream] {
		rapidjson::Document d;
		d.Parse(body);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error\n");
			free(body);

			return;
		}
		free(body);

		rapidjson::Value::ConstMemberIterator it;
		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *reqid = it->value.GetString();

		if(((it = d.FindMember("adminid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		int adminid = it->value.GetInt();

		if(((it = d.FindMember("session")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			return;
		}
		const char *session = it->value.GetString();

		CMTSessionListNode node(session, m_iSGID, adminid);
		g_solution_list.Update(node, false, [&](CMTSessionListNode& dst) {
			if (dst.m_status != ELogin::ELOGINSUCCESS) {

				return;
			}
			d.AddMember("sgid", m_iSGID, d.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			d.Accept(writer);

			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_SYNCHDATA_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
														   buffer.GetString(), 
														   buffer.GetLength()+1, 
														   ETag::ESYNCHDATA);
		});
	});
}

void UVTcpServer::WriteReqCB(uv_async_t* handle) {
#if defined(DEBUG)
	//fprintf(stderr, "<Thread>:<%d>++++++++++line:%d|%s\n", syscall(SYS_gettid),__LINE__, __FUNCTION__);
	XINFO("<Thread>:<%d>++++++++++line:%d|%s\n",syscall(SYS_gettid), __LINE__, __FUNCTION__);
#endif
	
	AsyncT* asyncT = nullptr;
	while(g_asyncTQueue.Pop(asyncT)) {
		WriteReq<CMTSessionListNode, 2> *data = (WriteReq<CMTSessionListNode, 2> *) asyncT->data.data;
		uv_handle_s *handle = (uv_handle_s *) &asyncT->data;
		g_asyncTPool->ReleaseObject((AsyncT*)handle);

		if (data->node != nullptr) {
			int ret = g_solution_list.Update(*data->node, false, [&](CMTSessionListNode &dst) {
				uv_stream_t *stream = (uv_stream_t *) dst.m_stream;

				if (nullptr == dst.m_stream) {

					delete data->node;
					delete data;

					return;
				}

				if (ELogin::ELOGINREJECT == dst.m_status ||
					0 == uv_is_active((uv_handle_t*)stream) ||
					0 == uv_is_writable((uv_stream_t*)stream) ||
					dst.m_stream->type != UV_TCP) {

					XWARNING("Stream writing was rejected");

					delete data->node;
					delete data;

					//free(stream); //whether free tcp stream here maybe properest??? not in OnClose()
					return;
				}

#if defined(DEBUG)
				// fprintf(stderr, "%d|unicast|%s: dst.m_managerID: %d, dst.m_status: %d, dst.m_sessionID: %s, dst.m_stream->type: %d\n", __LINE__, 
				// 	__FUNCTION__, dst.m_managerID, dst.m_status, dst.m_sessionID, dst.m_stream->type);
				XINFO("%d|unicast|%s: dst.m_managerID: %d, dst.m_status: %d, dst. dst.m_sessionID: %s, data->node->m_reqID: %s, dst.m_stream->type: %d\n", __LINE__, 
					__FUNCTION__, dst.m_managerID, dst.m_status, dst.m_sessionID, data->node->m_reqID, dst.m_stream->type);
#endif
				data->node->m_stream = stream;
				data->node->m_status = dst.m_status;
				uv_write((uv_write_t *) &data->req, (uv_stream_t *)stream, data->buf, 2, [](uv_write_t *req, int status) {
					WriteReq<CMTSessionListNode, 2> *wr = (WriteReq<CMTSessionListNode, 2> *) req;
					int tag = ntohs(*(unsigned short *) &wr->buf[0].base[2]);
#if defined(DEBUG)
					// fprintf(stderr, "%d|unicast|%s+++: dst.m_managerID: %d, dst.m_status: %d, dst.m_sessionID: %s, dst.m_stream->type: %d\n\n", __LINE__, 
					// __FUNCTION__, wr->node->m_managerID, wr->node->m_status, wr->node->m_sessionID, wr->node->m_stream->type);
					XINFO("%d|unicast|%s+++: dst.m_managerID: %d, dst.m_status: %d, dst.m_sessionID: %s, wr->node->m_reqID: %s, dst.m_stream->type: %d\n\n", __LINE__, 
						  __FUNCTION__, wr->node->m_managerID, wr->node->m_status, wr->node->m_sessionID, wr->node->m_reqID, wr->node->m_stream->type);
#endif
					uv_stream_t* stream = (uv_stream_t*)wr->node->m_stream;
					if (status != 0) {
						fprintf(stderr, "uv_writeCB|%s\n\n", uv_strerror(status));
						g_solution_list.Update(*wr->node, false, [&](CMTSessionListNode &dst) {
							dst.m_status = ELogin::ELOGINREJECT;
						});
					} else if ((tag == ETag::ELOGOUTANS ||
								tag == ETag::EADMINLOGOUTANS ||
								tag == ETag::ECMFORCELOGOUTANS ||
								tag == ETag::EADFORCELOGOUTANS || 
								wr->node->m_status == ELogin::ELOGININVALID) && status == 0) {
						usleep(50000);
						if (uv_is_active((uv_handle_t*)stream)) {
							uv_close((uv_handle_t *) stream, UVTcpServer::OnClose);
						}
					}

					delete wr->node;
					delete wr;
				});
			});

			if(ret == 0) {
				delete data->node;
				delete data;
			}
		} else {
			int ret = g_solution_list.UpdateAll([&](CMTSessionListNode &dst) {
				uv_stream_t *stream = (uv_stream_t *) dst.m_stream;

				WriteReq<CMTSessionListNode, 2> *writeReq =  new WriteReq<CMTSessionListNode, 2>;
				CMTSessionListNode *node = new CMTSessionListNode(dst.m_sessionID, dst.m_sgID, dst.m_managerID);
				writeReq->header = data->header;
				writeReq->event = data->event;
				writeReq->subscribe= data->subscribe;
				writeReq->buf[0].base = (char*)writeReq->header.get();
				writeReq->buf[0].len = data->buf[0].len;
				writeReq->buf[1].base = (char*)writeReq->event.get();
				writeReq->buf[1].len = data->buf[1].len;
				node->m_stream = dst.m_stream;
				node->m_status = dst.m_status;
				writeReq->node = node;
	
				int tag = ntohs(*(unsigned short*)&writeReq->buf[0].base[2]);
				//if(nullptr == dst.m_stream || !dst.m_subscribtion[tag]) {
				if(nullptr == dst.m_stream) {
					delete node;
					delete writeReq;

					return;
				}
				
				if (ELogin::ELOGINSUCCESS != dst.m_status || 
					0 == uv_is_active((uv_handle_t*)stream) ||
					0 == uv_is_writable((uv_stream_t*)stream) ||
					dst.m_stream->type != UV_TCP) {

					XWARNING("Stream was invalid");
					delete node;
					delete writeReq;

					//free(stream);
					return;
				}

				//for symbol realtime subscribing
				CustomSubscribe *subscribe = (CustomSubscribe*)data->subscribe.get();
				switch (tag) {
					case RISKMANAGEMENTTOP5BO: //0x030A
						// CustomSubscribe *subscribe = (CustomSubscribe*)data->subscribe.get();
						if (dst.m_subs.subtop5bo[subscribe->direction]) {

							break;
						} else {
							delete node;
							delete writeReq;

							return;
						}
					case RISKMANAGEMENTCUSTOMBO: //0x030C
						//CustomSubscribe *subscribe = (CustomSubscribe*)data->subscribe.get();
						// if (dst.m_subs.subcustomsymbol[subscribe->direction][subscribe->assetid]) {
						if (dst.m_subs.subcustomsymbolmap[subscribe->direction][subscribe->assetid]) {
					
							break;	
						} else {
							delete node;
							delete writeReq;

							return;
						}
					case RISKMANAGEMENTSYMBOLBO: //0x030E
						//CustomSubscribe *subscribe = (CustomSubscribe*)data->subscribe.get();
						//subscribe = data->subscribe.get();
						if (dst.m_subs.subsystemsymbol) {

							break;
						} else {
							delete node;
							delete writeReq;

							return;
						}
				}

#if defined(DEBUG)
				// fprintf(stderr, "%d|broadcast|%s: dst.m_managerID: %d, dst.m_status: %d, dst.m_sessionID: %s, dst.m_stream->type: %d\n", __LINE__, 
				// 	__FUNCTION__, dst.m_managerID, dst.m_status, dst.m_sessionID, dst.m_stream->type);
				XINFO("%d|broadcast|%s: dst.m_managerID: %d, dst.m_status: %d, dst.m_sessionID: %s, dst.m_stream->type: %d\n", __LINE__, 
									__FUNCTION__, dst.m_managerID, dst.m_status, dst.m_sessionID, dst.m_stream->type);
#endif		
				//why and when the callback would lost???
				uv_write((uv_write_t *) &writeReq->req, (uv_stream_t *) stream, writeReq->buf, 2, [](uv_write_t *req, int status) {
					WriteReq<CMTSessionListNode, 2> *wr = (WriteReq<CMTSessionListNode, 2> *) req;
					int tag = ntohs(*(unsigned short *) &wr->buf[0].base[2]);
#if defined(DEBUG)
					// fprintf(stderr, "%d|broadcast|%s+++: dst.m_managerID: %d, dst.m_status: %d, dst.m_sessionID: %s, dst.m_stream->type: %d\n\n", __LINE__, 
					// __FUNCTION__, wr->node->m_managerID, wr->node->m_status, wr->node->m_sessionID, wr->node->m_stream->type);
					XINFO("%d|broadcast|%s+++: dst.m_managerID: %d, dst.m_status: %d, dst.m_sessionID: %s, dst.m_stream->type: %d\n\n", __LINE__, 
						  __FUNCTION__, wr->node->m_managerID, wr->node->m_status, wr->node->m_sessionID, wr->node->m_stream->type);
#endif
					if (status != 0) {
						fprintf(stderr, "uv_writeCB|%s\n\n", uv_strerror(status));
						g_solution_list.Update(*wr->node, false, [&](CMTSessionListNode &dst) {
							dst.m_status = ELogin::ELOGINREJECT;
						});
					}

					delete wr->node;
					delete wr;
				});
		});

		delete data;
		}
	}
}
/* TCP connect end */
}
