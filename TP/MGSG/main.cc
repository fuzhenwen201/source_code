/*************************************************************************
	> File Name: main.cc
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Fri 03 Feb 2017 11:26:30 AM CST
 ************************************************************************/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "connection.h" 
#include "tnode_adapter.h"
#include "ThreadPool.h"
#include"object_pool.h"
#include"safe_queue.h"
#include "solution_config.h"

using namespace snetwork_xservice_solutiongateway;
using namespace snetwork_xservice_xflagger;

void test(uv_timer_t* t) {
	printf("hello world\n");
}


TNodeAministrator* g_tnode = nullptr; 
ThreadPool* g_threadPool = nullptr;

using AsyncTPool = ObjectPool<AsyncT, ASYNC_T_SIZE>;
AsyncTPool* g_asyncTPool = nullptr;

extern SafeList<CMTSessionListNode> g_solution_list;

void RunList(void);
void DeleteNode(void);
void UpdateNode(void);
//void (*sa_handler)(int);
void SignalHandler(int);

int main(int argc, char* argv[]) {
	struct sigaction sa;
	//sa.sa_handler = SignalHandler;//set the action as ignore
	sa.sa_handler = SIG_IGN;//set the action as ignore
	sa.sa_flags = 0;
	if (sigemptyset(&sa.sa_mask) == -1 || //init signal set as empty
		sigaction(SIGPIPE, &sa, 0) == -1) { //mask sigpipe
			perror("failed to ignore SIGPIPE, sigaction");
	}

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
	/* logger setting */

	std::string fileName (sxconfig->LoggerFile());
	if (!sxconfig->Screen()) {
		fileName = sxconfig->LoggerFile();
	}
	(*XLogger::GetObject(fileName.c_str())).StdErr(sxconfig->Screen()).Colour(sxconfig->Color()).Dir(sxconfig->LoggerDir());
	//signal(SIGPIPE, SIG_IGN);

	g_tnode = TNodeAministrator::GetInstance();
	g_asyncTPool = AsyncTPool::GetInstance();

	g_threadPool = new ThreadPool(sxconfig->ThreadSize());
	usleep(2000);

	UVTcpServer* uv = new UVTcpServer();

#if defined(DSOLUTION)
	//common/3
	Consumer* monitor = new Consumer(sxconfig->SolutionID(), ETag::EMONITOR);
	monitor->CreateSubscibe();

	Consumer* cmforcelogout = new Consumer(sxconfig->SolutionID(), ETag::ECMFORCELOGOUTANS);
	cmforcelogout->CreateSubscibe();

	Consumer* adforcelogout = new Consumer(sxconfig->SolutionID(), ETag::EADFORCELOGOUTANS);
	adforcelogout->CreateSubscibe();
	
	//for CM/11
	Consumer* loginans = new Consumer(sxconfig->SolutionID(), ETag::ELOGINANS);
	loginans->CreateSubscibe();   

	Consumer* logoutans = new Consumer(sxconfig->SolutionID(), ETag::ELOGOUTANS);
	logoutans->CreateSubscibe();   

	Consumer* changepasswordans = new Consumer(sxconfig->SolutionID(), ETag::ECHANGEPASSWORDANS);
	changepasswordans->CreateSubscibe();   

	Consumer* changebalanceans = new Consumer(sxconfig->SolutionID(), ETag::ECHANGEBALANCEANS);
	changebalanceans->CreateSubscibe();   
	Consumer* updatetraderans = new Consumer(sxconfig->SolutionID(), ETag::EUPDATETRADERANS);
	updatetraderans->CreateSubscibe();   

	Consumer* gettradersans = new Consumer(sxconfig->SolutionID(), ETag::EGETTRADERSANS);
	gettradersans->CreateSubscibe(); 

	Consumer* getmanageraccessans = new Consumer(sxconfig->SolutionID(), ETag::EGETMANAGERACCESSANS);
	getmanageraccessans->CreateSubscibe();   

	Consumer* userdeletecheckans = new Consumer(sxconfig->SolutionID(), ETag::EUSERDELETECHECKANS);
	userdeletecheckans->CreateSubscibe();   

	Consumer* userdeleteans = new Consumer(sxconfig->SolutionID(), ETag::EUSERDELETEANS);
	userdeleteans->CreateSubscibe();

	Consumer* createtraderans = new Consumer(sxconfig->SolutionID(), ETag::ECREATETRADERANS);
	createtraderans->CreateSubscibe();  
	
	Consumer* getonlinetraderans = new Consumer(sxconfig->SolutionID(), ETag::EGETONLINETRADERANS);
	getonlinetraderans->CreateSubscibe(); 

	Consumer* disabletraderans = new Consumer(sxconfig->SolutionID(), ETag::EDISABLETRADERANS);
	disabletraderans->CreateSubscibe();
	
	Consumer* kickouttraderans = new Consumer(sxconfig->SolutionID(), ETag::EKICKOUTTRADERANS);
	kickouttraderans->CreateSubscibe();

	Consumer* gettraderbytraderidans = new Consumer(sxconfig->SolutionID(), ETag::EGETTRADERBYTRADERIDANS);
	gettraderbytraderidans->CreateSubscibe();

	Consumer* getallgroupbymanageridans = new Consumer(sxconfig->SolutionID(), ETag::EGETALLGROUPBYMANAGERIDANS);
	getallgroupbymanageridans->CreateSubscibe();
	

	//for RM//13
	Consumer* getpositionsans = new Consumer(sxconfig->SolutionID(), ETag::GETPOSITIONSANS);
	getpositionsans->CreateSubscibe();

	Consumer* gettop5boans = new Consumer(sxconfig->SolutionID(), ETag::GETTOP5BOANS);
	gettop5boans->CreateSubscibe(); 

	Consumer* getcustomizeboans = new Consumer(sxconfig->SolutionID(), ETag::GETCUSTOMIZEBOANS);
	getcustomizeboans->CreateSubscibe();

	Consumer* getsymbolmonitorans = new Consumer(sxconfig->SolutionID(), ETag::GETSYMBOLMONITORANS);
	getsymbolmonitorans->CreateSubscibe(); 

	Consumer* riskmanagementtop5bo = new Consumer(sxconfig->SolutionID(), ETag::RISKMANAGEMENTTOP5BO);
	riskmanagementtop5bo->CreateSubscibe();

	Consumer* riskmanagementcustombo = new Consumer(sxconfig->SolutionID(), ETag::RISKMANAGEMENTCUSTOMBO);
	riskmanagementcustombo->CreateSubscibe();

	Consumer* riskmanagementsymbolbo = new Consumer(sxconfig->SolutionID(), ETag::RISKMANAGEMENTSYMBOLBO);
	riskmanagementsymbolbo->CreateSubscibe();

	Consumer* rmupdatetraderbalance = new Consumer(sxconfig->SolutionID(), ETag::RMUPDATETRADERBALANCE);
	rmupdatetraderbalance->CreateSubscibe();
	
	Consumer* onlinetraderonroot = new Consumer(sxconfig->SolutionID(), ETag::EONLINETRADERONROOT);
	onlinetraderonroot->CreateSubscibe();

	Consumer* logouttraderonroot = new Consumer(sxconfig->SolutionID(), ETag::ELOGOUTTRADERONROOT);
	logouttraderonroot->CreateSubscibe();

	Consumer* newtraderonroot = new Consumer(sxconfig->SolutionID(), ETag::ENEWTRADERONROOT);
	newtraderonroot->CreateSubscibe();

	Consumer* updatetraderonroot = new Consumer(sxconfig->SolutionID(), ETag::EUPDATETRADERONROOT);
	updatetraderonroot->CreateSubscibe();

	Consumer* deletetraderonroot = new Consumer(sxconfig->SolutionID(), ETag::EDELETETRADERONROOT);
	deletetraderonroot->CreateSubscibe();

	Consumer* updateopentradeonroot = new Consumer(sxconfig->SolutionID(), ETag::EUPDATEOPENTRADEONROOT);
	updateopentradeonroot->CreateSubscibe();

	Consumer* updateclosedtradeonroot = new Consumer(sxconfig->SolutionID(), ETag::EUPDATECLOSEDTRADEONROOT);
	updateclosedtradeonroot->CreateSubscibe();

	//for RMX
	Consumer* getpositionsansx = new Consumer(sxconfig->SolutionID(), ETag::EGETPOSITIONSANSX);
	getpositionsansx->CreateSubscibe();

	Consumer* rmupdatetraderbalancex = new Consumer(sxconfig->SolutionID(), ETag::ERMUPDATETRADERBALANCEX);
	rmupdatetraderbalancex->CreateSubscibe();

	Consumer* updatepositionsansx = new Consumer(sxconfig->SolutionID(), ETag::EUPDATEPOSITIONSANSX);
	updatepositionsansx->CreateSubscibe();

	Consumer* orderchangebalance = new Consumer(sxconfig->SolutionID(), ETag::EORDERCHANGEBALANCE);
	orderchangebalance->CreateSubscibe();


 	//for Admin//9
	Consumer* adminloginans = new Consumer(sxconfig->SolutionID(), ETag::EADMINLOGINANS);
	adminloginans->CreateSubscibe();   

	Consumer* adminlogoutans = new Consumer(sxconfig->SolutionID(), ETag::EADMINLOGOUTANS);
	adminlogoutans->CreateSubscibe();   

	Consumer* adminchangepasswordans = new Consumer(sxconfig->SolutionID(), ETag::EADMINCHANGEPASSWORDANS);
	adminchangepasswordans->CreateSubscibe();   

	Consumer* createmanagerans = new Consumer(sxconfig->SolutionID(), ETag::ECREATEMANAGERANS);
	createmanagerans->CreateSubscibe();   

	Consumer* updatemanagerans = new Consumer(sxconfig->SolutionID(), ETag::EUPDATEMANAGERANS);
	updatemanagerans->CreateSubscibe();   

	Consumer* deletemanagerans = new Consumer(sxconfig->SolutionID(), ETag::EDELETEMANAGERANS);
	deletemanagerans->CreateSubscibe();   

	Consumer* updatemanagerrightans = new Consumer(sxconfig->SolutionID(), ETag::EUPDATEMANAGERRIGHTANS);
	updatemanagerrightans->CreateSubscibe();   

	Consumer* getadminsans = new Consumer(sxconfig->SolutionID(), ETag::EGETADMINSANS);
	getadminsans->CreateSubscibe();   
	
	Consumer* getmanagerrightans = new Consumer(sxconfig->SolutionID(), ETag::EGETMANAGERRIGHTANS);
	getmanagerrightans->CreateSubscibe();   

	Consumer* getmanagersans = new Consumer(sxconfig->SolutionID(), ETag::EGETMANAGERSANS);
	getmanagersans->CreateSubscibe();   

	Consumer* enabledisablemanagerans = new Consumer(sxconfig->SolutionID(), ETag::EENABLEDISABLEMANAGERANS);
	enabledisablemanagerans->CreateSubscibe();   

	Consumer* deleteadminans = new Consumer(sxconfig->SolutionID(), ETag::EDELETEADMINANS);
	deleteadminans->CreateSubscibe(); 

	Consumer* createadminans = new Consumer(sxconfig->SolutionID(), ETag::ECREATEADMINANS);
	createadminans->CreateSubscibe(); 

	Consumer* updateadminans  = new Consumer(sxconfig->SolutionID(), ETag::EUPDATEADMINANS);
	updateadminans ->CreateSubscibe();  
	
	Consumer* savemanageragainstgroupsans = new Consumer(sxconfig->SolutionID(), ETag::ESAVEMANAGERAGAINSTGROUPSANS);
	savemanageragainstgroupsans->CreateSubscibe();  

	Consumer* getallgroupsbymanageridans = new Consumer(sxconfig->SolutionID(), ETag::EGETALLGROUPSBYMANAGERIDANS);
	getallgroupsbymanageridans->CreateSubscibe();  
	

	//for account
	Consumer* createaccountans = new Consumer(sxconfig->SolutionID(), ETag::ECREATEACCOUNTANS);
	createaccountans->CreateSubscibe();

	Consumer* updateaccountans = new Consumer(sxconfig->SolutionID(), ETag::EUPDATEACCOUNTANS);
	updateaccountans->CreateSubscibe();

	Consumer* getallaccountsans = new Consumer(sxconfig->SolutionID(), ETag::EGETALLACCOUNTSANS);
	getallaccountsans->CreateSubscibe();

	Consumer*  getaccountsbyaccountidans = new Consumer(sxconfig->SolutionID(), ETag::EGETACCOUNTSBYACCOUNTIDANS);
	getaccountsbyaccountidans->CreateSubscibe();

	//for group
	Consumer* creategroupans = new Consumer(sxconfig->SolutionID(), ETag::ECREATEGROUPANS); 
	creategroupans->CreateSubscibe();

	Consumer* updategroupans = new Consumer(sxconfig->SolutionID(), ETag::EUPDATEGROUPANS); 
	updategroupans->CreateSubscibe();

	Consumer* getgroupdetailsbyidans = new Consumer(sxconfig->SolutionID(), ETag::EGETGROUPDETAILSBYIDANS); 
	getgroupdetailsbyidans->CreateSubscibe();

	Consumer* getallgroupforgatewayans = new Consumer(sxconfig->SolutionID(), ETag::EGETALLGROUPFORGATEWAYANS); 
	getallgroupforgatewayans->CreateSubscibe();

	Consumer* getallgroupans = new Consumer(sxconfig->SolutionID(), ETag::EGETALLGROUPANS); 
	getallgroupans->CreateSubscibe();

	Consumer* deletegroupans = new Consumer(sxconfig->SolutionID(), ETag::EDELETEGROUPANS); 
	deletegroupans->CreateSubscibe();

	Consumer* updategroupsecurityans = new Consumer(sxconfig->SolutionID(), ETag::EUPDATEGROUPSECURITYANS); 
	updategroupsecurityans->CreateSubscibe();

	Consumer* updategroupreportans = new Consumer(sxconfig->SolutionID(), ETag::EUPDATEGROUPREPORTANS); 
	updategroupreportans->CreateSubscibe();

	//for security and symbol
	Consumer* createsecurityans = new Consumer(sxconfig->SolutionID(), ETag::ECREATESECURITYANS);
	createsecurityans->CreateSubscibe();

	Consumer* updatesecurityans = new Consumer(sxconfig->SolutionID(), ETag::EUPDATESECURITYANS);
	updatesecurityans->CreateSubscibe();
	
	Consumer* getallsecurityans = new Consumer(sxconfig->SolutionID(), ETag::EGETALLSECURITYANS);
	getallsecurityans->CreateSubscibe();

	Consumer* deletesecurityans = new Consumer(sxconfig->SolutionID(), ETag::EDELETESECURITYANS);
	deletesecurityans->CreateSubscibe();

	Consumer* createsymbolans = new Consumer(sxconfig->SolutionID(), ETag::ECREATESYMBOLANS);
	createsymbolans->CreateSubscibe();

	Consumer* updatesymbolans = new Consumer(sxconfig->SolutionID(), ETag::EUPDATESYMBOLANS);
	updatesymbolans->CreateSubscibe();

	Consumer* getallsymbolans = new Consumer(sxconfig->SolutionID(), ETag::EGETALLSYMBOLANS);
	getallsymbolans->CreateSubscibe();

	Consumer* getsymbolbyidans = new Consumer(sxconfig->SolutionID(), ETag::EGETSYMBOLBYIDANS);
	getsymbolbyidans->CreateSubscibe();

	Consumer* deletesymbolans = new Consumer(sxconfig->SolutionID(), ETag::EDELETESYMBOLANS);
	deletesymbolans->CreateSubscibe();

	Consumer* synchdataans = new Consumer(sxconfig->SolutionID(), ETag::ESYNCHDATAANS);
	synchdataans->CreateSubscibe();

	std::thread t(RunList);
#endif
	uv->Run();
#if defined(DSOLUTION)
	t.join();
#if 0
	delete sendorder;
	delete valid;
	delete ans;
	delete login;
	delete logout;
	delete forcelogout;
	delete userinfo;
	delete ocposition;
	delete monitor;
#endif
	//common//3
	delete monitor;
	delete cmforcelogout;
	delete adforcelogout;
	//for CM/11
	delete loginans;
	delete logoutans;
	delete changepasswordans;
	delete changebalanceans;
	delete updatetraderans;
	delete gettradersans;
	delete getmanageraccessans;
	delete userdeletecheckans;
	delete userdeleteans;
	delete createtraderans;
	delete getonlinetraderans;

	delete disabletraderans;
	delete kickouttraderans;
	delete gettraderbytraderidans;
	delete getallgroupbymanageridans;
	//for RM/9
	delete getpositionsans;
	delete gettop5boans;
	delete getcustomizeboans;
	delete getsymbolmonitorans;
	delete riskmanagementtop5bo;
	delete riskmanagementcustombo;
	delete riskmanagementsymbolbo;
	delete onlinetraderonroot;
	delete logouttraderonroot;
	//for RMX/3
	delete getpositionsansx;
	delete rmupdatetraderbalancex;
	delete updatepositionsansx;
	//for Admin/9
	delete adminloginans;
	delete adminlogoutans;
	delete adminchangepasswordans;
	delete createmanagerans;
	delete updatemanagerans;
	delete deletemanagerans;
	delete updatemanagerrightans;
	delete getmanagerrightans;
	delete getadminsans;
	delete getmanagersans;
	delete enabledisablemanagerans;
	delete savemanageragainstgroupsans;
	delete getallgroupsbymanageridans;
	//for account
	delete createaccountans;
	delete updateaccountans;
	delete getallaccountsans;
	delete getaccountsbyaccountidans;
	//for group
	delete creategroupans;
	delete updategroupans;
	delete getgroupdetailsbyidans;
	delete getallgroupforgatewayans;
	delete getallgroupans;
	delete deletegroupans;
	delete updategroupsecurityans;
	delete updategroupreportans;
	//for security and symbol 
	delete createsecurityans;
	delete updatesecurityans;
	delete getallsecurityans;
	delete deletesecurityans;
	delete createsymbolans;
	delete updatesymbolans;
	delete getallsymbolans;
	delete getsymbolbyidans;
	delete deletesymbolans;

	delete synchdataans;
#endif
	delete uv;
	delete g_tnode;
	delete g_threadPool;

	FREEXLOGGER;

	return 0;
}

void DeleteNode(void) {
	g_solution_list.RemoveAll([&](CMTSessionListNode& r) {
		// if (nullptr == r.m_stream ||
		// 	r.m_status == ELogin::ELOGINLOGOUT || 
		// 	r.m_status == ELogin::ELOGININVALID ||
		// 	0 == uv_is_writable((uv_stream_t*)r.m_stream) ||
		// 	0 == uv_is_active((uv_handle_t*)r.m_stream)) {
		if (nullptr == r.m_stream ||
			//r.m_status == ELogin::ELOGINLOGOUT || 
			r.m_status == ELogin::ELOGININVALID) {

			return true;
		}

		return false;
	});
}


void UpdateNode(void) {
	g_solution_list.UpdateAll([&](CMTSessionListNode& r) {
		// if ((nullptr == r.m_stream ||
		// 	0 == uv_is_writable((uv_stream_t*)r.m_stream) ||
		// 	0 == uv_is_active((uv_handle_t*)r.m_stream)) &&
		// 	(r.m_status != ELogin::ELOGINLOGOUT || 
		// 	 r.m_status != ELogin::ELOGINDEFAULT)) {

		if (nullptr == r.m_stream &&
			 r.m_status != ELogin::ELOGINLOGOUT &&
		     r.m_status != ELogin::ELOGINDEFAULT && 
		     r.m_status != ELogin::ELOGININVALID) {
#if defined(DEBUG)
			XINFO("publish msg:<%s>, len=%d\n", r.ToJson(), strlen(r.ToJson())+1);
#endif
			if (r.m_clienttype == ECLIENTTYPE::ECMT) {
				/*publish forcelogout request to CM */
				TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_FORCELOGOUT_NAME, 
															   SOLUTIONGATEWAY_PUBLISH_CMT_EVENT,
															   r.ToJson(), 
															   strlen(r.ToJson())+1, 
															   ETag::ECMTFORCELOGOUT);
			} else if (r.m_clienttype == ECLIENTTYPE::EADT) {
			 	/*publish forcelogout request to AD */
			 	TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_FORCELOGOUT_NAME,
			 												   SOLUTIONGATEWAY_PUBLISH_ADT_EVENT,
			 												   r.ToJson(),
			 												   strlen(r.ToJson())+1,
			 												   ETag::EADFORCELOGOUT);
			}
			
			r.m_status = ELogin::ELOGINLOGOUT;
		}
	});
}

void SignalHandler(int num) {
	printf("----------%d|%s(%d)\n---------", __LINE__, __FUNCTION__, num);
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
