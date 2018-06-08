/*************************************************************************
	> File Name: tnode_adapter.cc
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Wed 08 Feb 2017 11:17:29 AM CST
 ************************************************************************/
#include<mutex>
#include<new>
#include<string.h>
#include<type_traits>
#include"tnode_adapter.h"
//#include"define.h"
#include"ThreadPool.h"
#include<rapidjson/document.h>
#include<thread>
#include <syscall.h>
#include<rapidjson/writer.h>
#include<rapidjson/rapidjson.h>
#include<rapidjson/stringbuffer.h>


#if defined(DPOSITION)
#include"position_subscribe.h"
#include"solution_config.h"
#endif

#if defined(DSOLUTION)
#include"solution_config.h"
#include"solution_subscribe.h"
#endif
//#include"tnode.h"
//#include"tnodeconfig_v0.h"

//namespace snetwork_xservice_solutiongateway {
/* TNodeAministrator begin ****/
//using namespace snetwork_xservice_tnode;
//SafeList<SendOrderListNode> g_send_order_list;
//SafeList<PositionListNode> g_position_list;
TNodeAministrator* TNodeAministrator::m_instance = nullptr;
std::mutex TNodeAministrator::m_mutex;
extern ThreadPool* g_threadPool;

using namespace snetwork_xservice_xflagger;
TNodeAministrator* TNodeAministrator::GetInstance(void) {
	if (m_instance == nullptr) {
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_instance == nullptr) {
			m_instance = new TNodeAministrator();
		}
	}

	return m_instance;
}

TNodeAministrator::TNodeAministrator() { 
#if 0
	snetwork_xservice_tnode::TNodeConfigV0 config;
#if defined(LOCALHOSTIP)
	config.VHost("/demo").Exchange("t_node").ExchangeGroup("t_node_grp").Address("localhost").Port("5672").User("demo").Password("demo");
#elif defined(REMOTE154)
	config.VHost("/demo").Exchange("t_node").ExchangeGroup("t_node_grp").Address("35.154.147.154").Port("5672").User("demo").Password("demo");
#endif
	snetwork_xservice_tnode::TNode tnode;
	tnode.Config(config);
	tnode.Bind(1);
	tnode.Run();
#endif
	snetwork_xservice_tnode::TNodeConfigV0 config;
#if defined(DSOLUTION)
	SXConfig* sxconfig = dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig());
	config.VHost(sxconfig->VHost()).Exchange(sxconfig->Exchange()).ExchangeGroup(sxconfig->ExchangeGroup()).Address(sxconfig->Address()).Port(sxconfig->Port()).User(sxconfig->User()).Password(sxconfig->Password());
#elif defined(DPOSITION)
	SXConfig* sxconfig = dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig());
	config.VHost(sxconfig->VHost()).Exchange(sxconfig->Exchange()).ExchangeGroup(sxconfig->ExchangeGroup()).Address(sxconfig->Address()).Port(sxconfig->Port()).User(sxconfig->User()).Password(sxconfig->Password());
#else
#if defined(LOCALHOSTIP)
	config.VHost("/demo").Exchange("t_node").ExchangeGroup("t_node_grp").Address("localhost").Port("5672").User("demo").Password("demo");
#elif defined(REMOTE154)
	config.VHost("/demo").Exchange("t_node").ExchangeGroup("t_node_grp").Address("35.154.147.154").Port("5672").User("demo").Password("demo");
#endif
#endif
	snetwork_xservice_tnode::TNode tnode;
	tnode.Config(config);
	tnode.Bind(1);
	tnode.SetRecvBufSize(200*1024);
	tnode.Run();

// #if defined(SIGNALENGINE)
//     m_agent = new snetwork_signalengine_marketdata::MarketDataAgent;
// 	m_agent->Init(tnode, "test");
// #endif

	m_atomic = 0;
}

TNodeAministrator::~TNodeAministrator() {
// #if defined(SIGNALENGINE)
// 	if (m_agent != nullptr) {
// 		delete m_agent;
// 		m_agent = nullptr;
// 	}
// #endif

	for (auto& it : m_channel) {
		delete it;
		it = nullptr;
	}
}

snetwork_xservice_tnode::TNodeChannel TNodeAministrator::GetChannel(void) {
	snetwork_xservice_tnode::TNode tnode;
	tnode.Bind(1);

	return tnode.GetChannel();
}

bool TNodeAministrator::PublishGroup(const char* group, const char* event, const char* msg, unsigned int len, unsigned short tag) {
	snetwork_xservice_tnode::TNode tnode;
	tnode.Bind(1);

	snetwork_xservice_tnode::TNodeChannel channel = tnode.GetChannel();
	char* publish_msg = channel.AllocDataBuff(len);
	memcpy(publish_msg, msg, len-1);
	publish_msg[len-1] = 0;
	bool ret = channel.PublishGroup(group, event, publish_msg, len, tag);
#if defined(DEBUG)
	if (ret) {
		XINFO("ret=%d|publish groupname:<%s>, event:<%s>, tag=<0x%04X or %d>,msg:<%s>, len=%d\n", ret, group, event,tag, tag, msg, len);
	} else {
		XINFO("ret=%d|publish groupname:<%s>, event:<%s>, tag=<0x%04X or %d>,msg:<%s>, len=%d\n", ret, group, event,tag, tag, msg, len);
	}
#endif
}


/* TNodeAministrator end ****/
/*Subscribe   begin ****/
Subscribe::Subscribe(Consumer* consumer, const char* eventName, const char* groupName) :
	m_consumer(consumer),
	m_eventName(eventName),
	//m_eventNameLen(eventNameLen),
	m_groupName(groupName) {
	//TNodeAministrator::GetInstance()->GetChannel()->CreateGroup(m_groupName);
	//TNodeAministrator::GetInstance()->GetChannel()->AddConsumerToGroup(m_groupName, m_consumer);
	snetwork_xservice_tnode::TNodeChannel channel = TNodeAministrator::GetInstance()->GetChannel();
	channel.CreateGroup(m_groupName);
	channel.AddConsumerToGroup(m_groupName, m_consumer);
}
/*Subscribe   end ****/


/* Consumer  begin **/
Consumer::Consumer(const char* eventName, ETag tag) :
	m_strategy(nullptr),
	m_eventName(eventName),
	m_tag(tag) {
}

void Consumer::CreateSubscibe(void) {
	switch (m_tag) {
#if defined(DSOLUTION)
		//For common/2
		case EMONITOR: /* RM->MGSG MONITOR.SGID */
			m_strategy = new SubscribeEchoReq(this, m_eventName.c_str());
			break;		
		case ECMFORCELOGOUTANS: /* CM->MGSG FORCELOGOUT.SGID */
			m_strategy = new SubscribeForceLogoutAns(this, m_eventName.c_str());
			break;
		case EADFORCELOGOUTANS: /* AD->MGSG FORCELOGOUT.SGID */
			m_strategy = new SubscribeAdminForceLogoutAns(this, m_eventName.c_str());
			break;
		//For CM/12			
		case ELOGINANS: 
			m_strategy = new SubscribeLoginAns(this, m_eventName.c_str());
			break;
		case ELOGOUTANS: 
			m_strategy = new SubscribeLogoutAns(this, m_eventName.c_str());
			break;
		case ECHANGEPASSWORDANS:	
			m_strategy = new SubscribeChangePasswordAns(this, m_eventName.c_str());
			break;	
		case ECHANGEBALANCEANS:
			m_strategy = new SubscribeChangeBalanceAns(this, m_eventName.c_str());
			break;	
		case EUPDATETRADERANS:
			m_strategy = new SubscribeUpdateTraderAns(this, m_eventName.c_str());
			break;	
		case EGETTRADERSANS:
			m_strategy = new SubscribeGetTradersAns(this, m_eventName.c_str());
			break;	
		case EINSERTUPDATEMANAGERRIGHTSANS:
			m_strategy = new SubscribeInsertUpdateManagerRightsAns(this, m_eventName.c_str());
			break;	
		case EGETMANAGERACCESSANS:
			m_strategy = new SubscribeGetManagerAccessAns(this, m_eventName.c_str());
			break;
		case EUSERDELETECHECKANS:
			m_strategy = new SubscribeUserDeleteCheckAns(this, m_eventName.c_str());
			break;
		case EUSERDELETEANS:
			m_strategy = new SubscribeUserDeleteAns(this, m_eventName.c_str());
			break;	
		case ECREATETRADERANS:
			m_strategy = new SubscribeCreateTraderAns(this, m_eventName.c_str());
			break;
		case EGETONLINETRADERANS:
			m_strategy = new SubscribeGetOnlineTraderAns(this, m_eventName.c_str());
			break;

		case EDISABLETRADERANS:
			m_strategy = new SubscribeDisableTraderAns(this, m_eventName.c_str());
			break;
		case EKICKOUTTRADERANS:
			m_strategy = new SubscribeKickoutTraderAns(this, m_eventName.c_str());
			break;
		case EGETTRADERBYTRADERIDANS:
			m_strategy = new SubscribeGetTraderByTraderIdAns(this, m_eventName.c_str());
			break;
		case EGETALLGROUPBYMANAGERIDANS:
			m_strategy = new SubscribeGetAllGroupByManagerIdAns(this, m_eventName.c_str());
			break;

			
		//for RM/7
		case GETPOSITIONSANS: 
			m_strategy = new SubscribeGetPositionsAns(this, m_eventName.c_str());
			break;
		case GETTOP5BOANS:
			m_strategy = new SubscribeGetTop5boAns(this, m_eventName.c_str());
			break;
		case GETCUSTOMIZEBOANS: 
			m_strategy = new SubscribeGetCustomizeBoAns(this, m_eventName.c_str());
			break;
		case GETSYMBOLMONITORANS:
			m_strategy = new SubscribeGetSymbolMonitorAns(this, m_eventName.c_str());
			break;
		case RISKMANAGEMENTTOP5BO: 
			m_strategy = new SubscribeRiskManagementTop5Bo(this, m_eventName.c_str());
			break;
		case RISKMANAGEMENTCUSTOMBO: 
			m_strategy = new SubscribeRiskManagementCustomBo(this, m_eventName.c_str());
			break;
		case RISKMANAGEMENTSYMBOLBO: 
			m_strategy = new SubscribeRiskManagementSymbolBo(this, m_eventName.c_str());
			break;
		case RMUPDATETRADERBALANCE:
			m_strategy = new SubscribeRMUpdateTraderBalance(this, m_eventName.c_str());
			break;
		case EONLINETRADERONROOT: 
			m_strategy = new SubscribeOnlineTraderOnRoot(this, m_eventName.c_str());
			break;
		case ELOGOUTTRADERONROOT:
			m_strategy = new SubscribeLogoutTraderOnRoot(this, m_eventName.c_str());
			break;
		case ENEWTRADERONROOT:
			m_strategy = new SubscribeNewTraderOnRoot(this, m_eventName.c_str());
			break;
		case EUPDATETRADERONROOT:
			m_strategy = new SubscribeUpdateTraderOnRoot(this, m_eventName.c_str());
			break;
		case EDELETETRADERONROOT:
			m_strategy = new SubscribeDeleteTraderOnRoot(this, m_eventName.c_str());
			break;
		case EUPDATEOPENTRADEONROOT:
			m_strategy = new SubscribeUpdateOpenTradeOnRoot(this, m_eventName.c_str());
			break;
		case EUPDATECLOSEDTRADEONROOT:
			m_strategy = new SubscribeUpdateClosedTradeOnRoot(this, m_eventName.c_str());
			break;
	//for RMX/3
		case EGETPOSITIONSANSX: 
			m_strategy = new SubscribeGetPositionsAnsX(this, m_eventName.c_str());
			break;
		case ERMUPDATETRADERBALANCEX: 
			m_strategy = new SubscribeRMUpdateTraderBalanceX(this, m_eventName.c_str());
			break;
		case EUPDATEPOSITIONSANSX: 
			m_strategy = new SubscribeUpdatePositionsAnsX(this, m_eventName.c_str());
			break;			
		case EORDERCHANGEBALANCE: 
			m_strategy = new SubscribeOrderChangeBalance(this, m_eventName.c_str());
			break;	

	//for Admin/9
		case EADMINLOGINANS: 
			m_strategy = new SubscribeAdminLoginAns(this, m_eventName.c_str());
			break;
		case EADMINLOGOUTANS: 
			m_strategy = new SubscribeAdminLogoutAns(this, m_eventName.c_str());
			break;
		case EADMINCHANGEPASSWORDANS:	
			m_strategy = new SubscribeAdminChangePasswordAns(this, m_eventName.c_str());
			break;	
		case ECREATEMANAGERANS:
			m_strategy = new SubscribeCreateManagerAns(this, m_eventName.c_str());
			break;	
		case EUPDATEMANAGERANS:
			m_strategy = new SubscribeUpdateManagerAns(this, m_eventName.c_str());
			break;	
		case EDELETEMANAGERANS:
			m_strategy = new SubscribeDeleteManagerAns(this, m_eventName.c_str());
			break;	
		case EUPDATEMANAGERRIGHTANS:
			m_strategy = new SubscribeUpdateManagerRightAns(this, m_eventName.c_str());
			break;	
		case EGETADMINSANS:
			m_strategy = new SubscribeGetAdminsAns(this, m_eventName.c_str());
			break;	
		case EGETMANAGERRIGHTANS:
			m_strategy = new SubscribeGetManagerRightAns(this, m_eventName.c_str());
			break;	
		case EGETMANAGERSANS:
			m_strategy = new SubscribeGetManagersAns(this, m_eventName.c_str());
			break;
		case EENABLEDISABLEMANAGERANS:
			m_strategy = new SubscribeEnabledIsableManagerAns(this, m_eventName.c_str());
			break;
		case EDELETEADMINANS:
			m_strategy = new SubscribeDeleteAdminAns(this, m_eventName.c_str());
			break;
		case ECREATEADMINANS:
			m_strategy = new SubscribeCreateAdminAns(this, m_eventName.c_str());
			break;
		case EUPDATEADMINANS:
			m_strategy = new SubscribeUpdateAdminAns(this, m_eventName.c_str());
			break;

		case ESAVEMANAGERAGAINSTGROUPSANS:
			m_strategy = new SubscribeSaveManagerAgainstGroupsAns(this, m_eventName.c_str());
			break;
		case EGETALLGROUPSBYMANAGERIDANS:
			m_strategy = new SubscribeGetAllGroupsByManagerIdAns(this, m_eventName.c_str());
			break;

		//for account
		case ECREATEACCOUNTANS:
			m_strategy = new SubscribeCreateAccountAns(this, m_eventName.c_str());
			break;
		case EUPDATEACCOUNTANS:
			m_strategy = new SubscribeUpdateAccountAns(this, m_eventName.c_str());
			break;
		case EGETALLACCOUNTSANS:
			m_strategy = new SubscribeGetAllAccountsAns(this, m_eventName.c_str());
			break;
		case EGETACCOUNTSBYACCOUNTIDANS:
			m_strategy = new SubscribeGetAccountsByAccountIdAns(this, m_eventName.c_str());
			break;
		//for group
		case ECREATEGROUPANS:
			m_strategy =  new SubscribeCreateGroupAns(this, m_eventName.c_str());
			break;
		case EUPDATEGROUPANS:
			m_strategy =  new SubscribeUpdateGroupAns(this, m_eventName.c_str());
			break;
		case EGETGROUPDETAILSBYIDANS:
			m_strategy =  new SubscribeGetGroupdeTailsByIdAns(this, m_eventName.c_str());
			break;
		case EGETALLGROUPFORGATEWAYANS:
			m_strategy =  new SubscribeGetAllGroupForGateWayAns(this, m_eventName.c_str());
			break;
		case EGETALLGROUPANS:
			m_strategy =  new SubscribeGetAllGroupAns(this, m_eventName.c_str());
			break;
		case EDELETEGROUPANS:
			m_strategy =  new SubscribeDeleteGroupAns(this, m_eventName.c_str());
			break;
		case EUPDATEGROUPSECURITYANS:
			m_strategy =  new SubscribeUpdateGroupSecurityAns(this, m_eventName.c_str());
			break;
		case EUPDATEGROUPREPORTANS:
			m_strategy =  new SubscribeUpdateGroupReportAns(this, m_eventName.c_str());
			break;
		//for security and symbol 
		case ECREATESECURITYANS:
			m_strategy = new SubscribeCreateSecurityAns(this, m_eventName.c_str());
			break;
		case EUPDATESECURITYANS:
			m_strategy = new SubscribeUpdateSecurityAns(this, m_eventName.c_str());
			break;
		case EGETALLSECURITYANS:
			m_strategy = new SubscribeGetAllSecurityAns(this, m_eventName.c_str());
			break;
		case EDELETESECURITYANS:
			m_strategy = new SubscribeDeleteSecurityAns(this, m_eventName.c_str());
			break;
		case ECREATESYMBOLANS:
			m_strategy = new SubscribeCreateSymbolAns(this, m_eventName.c_str());
			break;
		case EUPDATESYMBOLANS:
			m_strategy = new SubscribeUpdateSymbolAns(this, m_eventName.c_str());
			break;
		case EGETALLSYMBOLANS:
			m_strategy = new SubscribeGetAllSymbolAns(this, m_eventName.c_str());
			break;
		case EGETSYMBOLBYIDANS:
			m_strategy = new SubscribeGetSymbolByIdAns(this, m_eventName.c_str());
			break;
		case EDELETESYMBOLANS:	
			m_strategy = new SubscribeDeleteSymbolAns(this, m_eventName.c_str());
			break;
		case ESYNCHDATAANS:
			m_strategy = new SubscribeSynchDataAns(this, m_eventName.c_str());
			break;
#endif
	}
}

Consumer::Consumer(const std::string& eventName, ETag tag) :
	m_strategy(nullptr),
	m_eventName(eventName),
	m_tag(tag) {
}

Consumer::~Consumer() {
	if (m_strategy != nullptr) {
		delete m_strategy;
		m_strategy = nullptr;
	}
}

unsigned int Consumer::EventHandle(unsigned short tag, char *event, unsigned int eventLen) {
	if (tag != m_tag) {
		XERROR("expected tag is %d, but received is %d", m_tag, tag);

		return UNIVERSAL_TNODE_UNKNOWN;
	}
#if defined(MULTIPLE_THREAD)
#if 0
	char* msg = (char*)malloc(eventLen);
	if (nullptr == msg) {
		return -1;
	}

	memcpy(msg, event, eventLen);
#endif
	std::shared_ptr<void> msg (malloc(eventLen), [](void* p) {
		free(p);
	});
	memcpy(msg.get(), event, eventLen);

	//g_threadPool->enqueue(lamba_func, m_strategy, msg, eventLen);
	//g_threadPool->enqueue([this]() {
	g_threadPool->Enqueue([this, msg, eventLen]() {
			std::this_thread::yield();
			m_strategy->Done(msg, eventLen);

			//free(msg);
			});


#else
	m_strategy->Done(event, eventLen);
#endif
	return UNIVERSAL_TNODE_SUCCESS;
}

unsigned int Consumer::ServiceHandle(unsigned short tag, char *reqbody, unsigned int reqlen, snetwork_xservice_tnode::TNodeDataSend send) {/* resever, empty implement*/

	return UNIVERSAL_TNODE_SUCCESS;
}
/* Consumer  end **/
//}



