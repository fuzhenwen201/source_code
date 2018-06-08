/*************************************************************************
	> File Name: main.cc
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Fri 03 Feb 2017 11:26:30 AM CST
 ************************************************************************/

#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"connection.h" 
#include"http_request.h" 
#include"tnode_adapter.h"
#include"ThreadPool.h"
#include"object_pool.h"
//#include"redisop.h"
#include"object_pool.h"
#include"safe_queue.h"
#include"solution_config.h"
#include "hexutil.h"
#include "Teaencrypt.h"
#include <math.h>

using namespace snetwork_xservice_solutiongateway;
using namespace snetwork_xservice_xflagger;

void test(uv_timer_t* t) {
	printf("hello world\n");
}

//uv_loop_t* g_loop = nullptr;
//http_parser_settings g_settings;
//snetwork_xservice_solutiongateway::TNodeAministrator* g_tnode = nullptr; 
//snetwork_xservice_solutiongateway::SafeList<snetwork_xservice_solutiongateway::SendOrderListNode> g_list; 

TNodeAministrator* g_tnode = nullptr; 
//SafeList<SendOrderListNode> g_list; 
ThreadPool* g_threadPool = nullptr;
//using RedisPool = ObjectPool<snetwork_xservice_db::Redis, 5>;
//RedisPool* g_redisPool = nullptr;
//extern SafeList<SendOrderListNode> g_solution_list;
using AsyncTPool = ObjectPool<AsyncT, ASYNC_T_SIZE>;
AsyncTPool* g_asyncTPool = nullptr;
extern SafeList<SessionListNode> g_solution_list;

void RunList(void);
void DeleteNode(void);
void UpdateNode(void);

char intTohexChar(int x) {
    static const char HEX[16] = {
		 '0', '1', '2', '3', '4', '5', '6', '7',
		 '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
    };
    return HEX[x];
}

int main(int argc, char* argv[]) {
	//InitDaemon();
#if 1
	XConfig* cfg = new SXConfig;
	XFlagger* flagger = SXFlagger::GetInstance();
	flagger->ParseCommandLine(&argc, &argv, false);
	flagger->FlagXConfig(cfg);
	SXConfig* sxconfig = dynamic_cast<SXConfig*>(flagger->GetConfig());
	if (sxconfig == nullptr) {
		fprintf(stderr, "config error");

		exit(1);
	}
	std::cout<<*sxconfig<<std::endl;

#if defined(SECURITY)

	HardwareUID uid;
	uid.collect();
	char result[1024] = {};
	string security = sxconfig->Cipher();
	char sour[1024] = {};
	strncpy(sour,security.c_str(),security.size());

	AES aes(key);
	aes.InvCipher(sour, result);
	char str[9] = {};
	for(int i = 0; i < 8; i++){
            int b = pow(16,7-i);
            str[i] = intTohexChar((int)(uid.hostid/b%16));
	}
	str[8] = '\0';
	if (0 != strcmp(str,result)) {
	    XERROR("The machines are not allowed to be deployed!\n");
	    exit(1);
        }
#endif

	/* logger setting */
	std::string fileName (sxconfig->LoggerFile());
	if (!sxconfig->Screen()) {
		fileName = sxconfig->LoggerFile();
	}
	(*XLogger::GetObject(fileName.c_str())).StdErr(sxconfig->Screen()).Colour(sxconfig->Color()).Dir(sxconfig->LoggerDir());
#endif

	g_tnode = TNodeAministrator::GetInstance();
	//g_redisPool = RedisPool::GetInstance();
	g_asyncTPool = AsyncTPool::GetInstance();

	g_threadPool = new ThreadPool(sxconfig->ThreadSize());
	usleep(2000);

	UVTcpServer* uv = new UVTcpServer();

#if defined(DSOLUTION)
	/* te/pm: USER.[SGID] RequestACK */
	Consumer* sendorder = new Consumer(sxconfig->SolutionID(), ETag::EREQUESTACK);
	sendorder->CreateSubscibe();

	/* pm->sg: VALID.[SGID] ValidateOrder */
	Consumer* valid = new Consumer(sxconfig->SolutionID(), ETag::EVALIDATEORDER);
	valid->CreateSubscibe();

	/* te->sg: ORDERANS.[SGID] OrderAns */
	Consumer* ans = new Consumer(sxconfig->SolutionID(), ETag::EORDERANS);
	ans->CreateSubscibe();

	Consumer* login = new Consumer(sxconfig->SolutionID(), ETag::ELOGINANS);
	login->CreateSubscibe();

	Consumer* logout = new Consumer(sxconfig->SolutionID(), ETag::ELOGOUTANS);
	logout->CreateSubscibe();

	Consumer* forcelogout = new Consumer(sxconfig->SolutionID(), ETag::EFORCELOGOUTANS);
	forcelogout->CreateSubscibe();

	Consumer* changepwd = new Consumer(sxconfig->SolutionID(), ETag::ECHANGEPWDANS);
	changepwd->CreateSubscibe();

	Consumer* tradeConditions = new Consumer(sxconfig->SolutionID(), ETag::ETRADECONDITIONS);
	tradeConditions->CreateSubscibe();

	Consumer* userinfo = new Consumer(sxconfig->SolutionID(), ETag::EUSERINFON);
	userinfo->CreateSubscibe();

	Consumer* ocposition = new Consumer(sxconfig->SolutionID(), ETag::EOCPOSITION);
	ocposition->CreateSubscibe();

	Consumer* monitor = new Consumer(sxconfig->SolutionID(), ETag::EMONITOR);
	monitor->CreateSubscibe();

	std::thread t(RunList);
#endif
	uv->Run();
#if defined(DSOLUTION)
	t.join();
	delete sendorder;
	delete valid;
	delete ans;
	delete login;
	delete logout;
	delete forcelogout;
	delete changepwd;
	delete tradeConditions;
	delete userinfo;
	delete ocposition;
	delete monitor;
#endif
	delete uv;
	delete g_tnode;
	delete g_threadPool;

	FREEXLOGGER;

	return 0;
}

#if 0
void RunList(void) {
	std::chrono::system_clock::time_point nw;
	std::chrono::system_clock::time_point end;
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds;
	int sleepTime = 0;

	sleep(SAFE_LIST_ELAPSED_SECONDS);
	while (1) {
		g_solution_list.Remove([&](SendOrderListNode& r) {
#if 0
			if ((1 == r.m_pm_user) && 
				(1 == r.m_te_user) &&
				(1 == r.m_pm_valid)) {
				/* time out need to remove */
				nw = std::chrono::system_clock::now();
#if 0
				if (r.m_stamp > r.m_openTime) {
					elapsed_seconds = nw - r.m_stamp;
				} else {
					elapsed_seconds = nw - r.m_openTime;
				}
#endif
				elapsed_seconds = nw - r.m_openTime;
				if (2 * r.m_expiryPeriod < elapsed_seconds.count()) {
					/* time out, remove list, and async write to database */
					XINFO("sendorder=%s timeout", r.m_reqID);
#if 0
					/* need to complete */
					SendOrderListNode tmp = std::move(r);
#endif
					return true;
				}
			} else if ((1 == r.m_te_orderans_openprice) &&
				   (1 == r.m_te_orderans_closeprice)) {
				return true;
			}
#endif
			if ((1 == r.m_te_orderans_openprice) &&
				   (1 == r.m_te_orderans_closeprice) &&
				   3 == r.m_pm_valid) {
				return true;
			}

			if (uv_is_writable((uv_stream_t*)&r.m_http_request->stream) == 0) {
				/* lost connect or logout*/
				return true;
			}

			/**/
			if (uv_is_active((uv_handle_t*)&r.m_http_request->stream) ==0) {
				/*disconnect or logout */
				return true;
			}

			return false;
		});

		end = std::chrono::system_clock::now();
		elapsed_seconds = end - start;
		sleepTime = elapsed_seconds.count() - SAFE_LIST_ELAPSED_SECONDS;
		start = std::move(end);

		if (sleepTime > 0) {
			sleep(elapsed_seconds.count());
		}
	}
}
#endif

void DeleteNode(void) {
	g_solution_list.RemoveAll([&](SessionListNode& r) {
		if (nullptr == r.m_http_request ||
			r.m_status == ELogin::ELOGINLOGOUT || 
			r.m_status == ELogin::ELOGININVALID) {
			/* if m_status is not login success, remove node*/
			return true;
		}

		return false;
	});
}


void UpdateNode(void) {
	g_solution_list.UpdateAll([&](SessionListNode& r) {
#if 0
		if (nullptr == r.m_http_request ||
			r.m_status == ELogin::ELOGINUNWRITE ||
			r.m_status == ELogin::ELOGINCLOSE ||
			(r.m_status != ELogin::ELOGINLOGOUT &&
			 (0 == uv_is_writable((uv_stream_t*)&r.m_http_request->stream) ||
			 0 == uv_is_active((uv_handle_t*)&r.m_http_request->stream)))) {
#endif
		if (nullptr == r.m_http_request &&
			r.m_status != ELogin::ELOGINLOGOUT &&
			r.m_status != ELogin::ELOGINDEFAULT &&
			r.m_status != ELogin::ELOGININVALID) {
#if defined(DEBUG)
			XINFO("publish msg:<%s>, len=%d\n", r.ToJson(), strlen(r.ToJson())+1);
#endif
			/*publish forcelogout request to CM */
			TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_FORCELOGOUT_NAME, 
														   SOLUTIONGATEWAY_PUBLISH_FORCELOGOUT_EVENT,
														   r.ToJson(), 
														   strlen(r.ToJson())+1, 
														   ETag::EFORCELOGOUT);
			r.m_status = ELogin::ELOGINLOGOUT;
		}
	});
}


void RunList(void) {
	std::chrono::system_clock::time_point nw;
	std::chrono::system_clock::time_point end;
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds;
	int sleepTime = 0;

	sleep(SAFE_LIST_ELAPSED_SECONDS);
	while (1) {
		end = std::chrono::system_clock::now();
		UpdateNode();
		DeleteNode();

		elapsed_seconds = end - start;
		sleepTime = SAFE_LIST_ELAPSED_SECONDS - elapsed_seconds.count();
		start = std::move(end);

		if (sleepTime > 0) {
			sleep(sleepTime);
		}
	}
}
