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
	tnode.SetRecvBufSize(1024*10);
	tnode.Bind(1);
	tnode.Run();

#if defined(SIGNALENGINE)
    m_agent = new snetwork_signalengine_marketdata::MarketDataAgent;
	m_agent->Init(tnode, "test");
#endif

	m_atomic = 0;
}

TNodeAministrator::~TNodeAministrator() {
#if defined(SIGNALENGINE)
	if (m_agent != nullptr) {
		delete m_agent;
		m_agent = nullptr;
	}
#endif

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
	m_groupName(groupName) {
	snetwork_xservice_tnode::TNodeChannel channel = TNodeAministrator::GetInstance()->GetChannel();
	channel.CreateGroup(m_groupName);
	channel.AddConsumerToGroup(m_groupName, m_consumer);
}

Subscribe::Subscribe(const Subscribe& r) {
}

Subscribe::Subscribe(Subscribe&& r) {
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
#if defined(DPOSITION)
		case ESENDORDER: /* 0x0001 */
			if (m_eventName == POSITIONMANAGEMENT_SUBSCRIBE_POSITION_EVENT) {
				m_strategy = new SubscribeSendOrder(this, m_eventName.c_str());
			}

			break;
#endif
		case EORDERANS: /* 0x0002 */
#if defined(DPOSITION)
			m_strategy = new SubscribeOrderAns(this, m_eventName.c_str());
#endif
#if defined(DSOLUTION)
			m_strategy = new SubscribeAns(this, m_eventName.c_str());
#endif
			break;


		case EVALIDATEORDER: /* 0x0003 */
#if defined(DSOLUTION)
			m_strategy = new SubscribeValid(this, m_eventName.c_str());
#endif

			break;

		case EREQUESTACK: /* 0x0004 */
#if defined(DPOSITION)
			m_strategy = new SubscribeSystem(this, m_eventName.c_str());
#endif

#if defined(DSOLUTION)
			m_strategy = new SubscribeUser(this, m_eventName.c_str());
#endif
			break;

#if defined(DPOSITION)
		case EPULL: /* 0x0006 CM->PM */
			m_strategy = new SubscribePull(this, m_eventName.c_str());
			break;
#endif

#if defined(DSOLUTION)
		case EUSERINFON: /*  CM->SG: USERINFO.SGID */
			m_strategy = new SubscribeUserInfo(this, m_eventName.c_str());
			break;

		case ELOGINANS: /*  CM->PM LOGIN.POSITION */
			m_strategy = new SubscribeLogin(this, m_eventName.c_str());
			break;

		case EOCPOSITION:
			m_strategy = new SubscribeOCPosition(this, m_eventName.c_str());
			break;

		case ELOGOUTANS: /* CM->SG LOGOUT.SGID*/
			m_strategy = new SubscribeLogout(this, m_eventName.c_str());
			break;

		case EFORCELOGOUTANS: /* CM->SG FORCELOGOUT.SGID */
			m_strategy = new SubscribeForceLogout(this, m_eventName.c_str());
			break;

		case ECHANGEPWDANS: /* CM->SG FORCELOGOUT.SGID */
			m_strategy = new SubscribeChangePWD(this, m_eventName.c_str());
			break;

		case ETRADECONDITIONS: /* CM->SG TRADECONDITIONS.SGID */
			m_strategy = new SubscribeTradeConditions(this, m_eventName.c_str());
			break;

		case EMONITOR: /* RM->SG MONITOR.SGID */
			m_strategy = new SubscribeEchoReq(this, m_eventName.c_str());
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

	do {
		if (nullptr != dynamic_cast<SubscribeAns*>(m_strategy)) {
			SubscribeAns* ordAns = new SubscribeAns(*(SubscribeAns*)m_strategy);  
			g_threadPool->Enqueue([ordAns, msg, eventLen] {
				ordAns->Done(msg, eventLen);

				delete ordAns;
			});
			break;
		}

		if (nullptr != dynamic_cast<SubscribeValid*>(m_strategy)) {
			SubscribeValid* valid = new SubscribeValid(*(SubscribeValid*)m_strategy);  
			g_threadPool->Enqueue([valid, msg, eventLen] {
				valid->Done(msg, eventLen);

				delete valid;
			});
			break;
		}

		if (nullptr != dynamic_cast<SubscribeUser*>(m_strategy)) {
			SubscribeUser* user = new SubscribeUser(*(SubscribeUser*)m_strategy);  
			g_threadPool->Enqueue([user, msg, eventLen] {
				user->Done(msg, eventLen);

				delete user;
			});
			break;
		}

		if (nullptr != dynamic_cast<SubscribeUserInfo*>(m_strategy)) {
			SubscribeUserInfo* userInfo = new SubscribeUserInfo(*(SubscribeUserInfo*)m_strategy);  
			g_threadPool->Enqueue([userInfo, msg, eventLen] {
				userInfo->Done(msg, eventLen);

				delete userInfo;
			});
			break;
		}

		if (nullptr != dynamic_cast<SubscribeLogin*>(m_strategy)) {
			SubscribeLogin* login = new SubscribeLogin(*(SubscribeLogin*)m_strategy);  
			g_threadPool->Enqueue([login, msg, eventLen] {
				login->Done(msg, eventLen);

				delete login;
			});
			break;
		}

		if (nullptr != dynamic_cast<SubscribeOCPosition*>(m_strategy)) {
			SubscribeOCPosition* ocPosition = new SubscribeOCPosition(*(SubscribeOCPosition*)m_strategy);  
			g_threadPool->Enqueue([ocPosition, msg, eventLen] {
				ocPosition->Done(msg, eventLen);

				delete ocPosition;
			});
			break;
		}

		if (nullptr != dynamic_cast<SubscribeLogout*>(m_strategy)) {
			SubscribeLogout* logout = new SubscribeLogout(*(SubscribeLogout*)m_strategy);  
			g_threadPool->Enqueue([logout, msg, eventLen] {
				logout->Done(msg, eventLen);

				delete logout;
			});
			break;
		}

		if (nullptr != dynamic_cast<SubscribeForceLogout*>(m_strategy)) {
			SubscribeForceLogout* forceLogout = new SubscribeForceLogout(*(SubscribeForceLogout*)m_strategy);  
			g_threadPool->Enqueue([forceLogout, msg, eventLen] {
				forceLogout->Done(msg, eventLen);

				delete forceLogout;
			});
			break;
		}

		if (nullptr != dynamic_cast<SubscribeEchoReq*>(m_strategy)) {
			SubscribeEchoReq* echoReq = new SubscribeEchoReq(*(SubscribeEchoReq*)m_strategy);  
			g_threadPool->Enqueue([echoReq, msg, eventLen] {
				echoReq->Done(msg, eventLen);

				delete echoReq;
			});
			break;
		}
	} while (false);
#else
	std::shared_ptr<void> msg (malloc(eventLen), [](void* p) {
		free(p);
	});
	memcpy(msg.get(), event, eventLen);

	do {
		if (nullptr != dynamic_cast<SubscribeAns*>(m_strategy)) {
			SubscribeAns* ordAns = new SubscribeAns(*(SubscribeAns*)m_strategy);  
			g_threadPool->Enqueue([ordAns, msg, eventLen] {
				ordAns->Done(msg, eventLen);

				delete ordAns;
			});
			break;
		}

		if (nullptr != dynamic_cast<SubscribeValid*>(m_strategy)) {
			SubscribeValid* valid = new SubscribeValid(*(SubscribeValid*)m_strategy);  
			g_threadPool->Enqueue([valid, msg, eventLen] {
				valid->Done(msg, eventLen);

				delete valid;
			});
			break;
		}

		if (nullptr != dynamic_cast<SubscribeUser*>(m_strategy)) {
			SubscribeUser* user = new SubscribeUser(*(SubscribeUser*)m_strategy);  
			g_threadPool->Enqueue([user, msg, eventLen] {
				user->Done(msg, eventLen);

				delete user;
			});
			break;
		}

		if (nullptr != dynamic_cast<SubscribeUserInfo*>(m_strategy)) {
			SubscribeUserInfo* userInfo = new SubscribeUserInfo(*(SubscribeUserInfo*)m_strategy);  
			g_threadPool->Enqueue([userInfo, msg, eventLen] {
				userInfo->Done(msg, eventLen);

				delete userInfo;
			});
			break;
		}

		if (nullptr != dynamic_cast<SubscribeLogin*>(m_strategy)) {
			SubscribeLogin* login = new SubscribeLogin(*(SubscribeLogin*)m_strategy);  
			g_threadPool->Enqueue([login, msg, eventLen] {
				login->Done(msg, eventLen);

				delete login;
			});
			break;
		}

		if (nullptr != dynamic_cast<SubscribeOCPosition*>(m_strategy)) {
			SubscribeOCPosition* ocPosition = new SubscribeOCPosition(*(SubscribeOCPosition*)m_strategy);  
			g_threadPool->Enqueue([ocPosition, msg, eventLen] {
				ocPosition->Done(msg, eventLen);

				delete ocPosition;
			});
			break;
		}

		if (nullptr != dynamic_cast<SubscribeLogout*>(m_strategy)) {
			SubscribeLogout* logout = new SubscribeLogout(*(SubscribeLogout*)m_strategy);  
			g_threadPool->Enqueue([logout, msg, eventLen] {
				logout->Done(msg, eventLen);

				delete logout;
			});
			break;
		}

		if (nullptr != dynamic_cast<SubscribeForceLogout*>(m_strategy)) {
			SubscribeForceLogout* forceLogout = new SubscribeForceLogout(*(SubscribeForceLogout*)m_strategy);  
			g_threadPool->Enqueue([forceLogout, msg, eventLen] {
				forceLogout->Done(msg, eventLen);

				delete forceLogout;
			});
			break;
		}

		if (nullptr != dynamic_cast<SubscribeChangePWD*>(m_strategy)) {
			SubscribeChangePWD* changePWD = new SubscribeChangePWD(*(SubscribeChangePWD*)m_strategy);  
			g_threadPool->Enqueue([changePWD, msg, eventLen] {
				changePWD->Done(msg, eventLen);

				delete changePWD;
			});
			break;
		}

		if (nullptr != dynamic_cast<SubscribeTradeConditions*>(m_strategy)) {
			SubscribeTradeConditions* tradeConditions = new SubscribeTradeConditions(*(SubscribeTradeConditions*)m_strategy);  
			g_threadPool->Enqueue([tradeConditions, msg, eventLen] {
				tradeConditions->Done(msg, eventLen);

				delete tradeConditions;
			});
			break;
		}

		if (nullptr != dynamic_cast<SubscribeEchoReq*>(m_strategy)) {
			SubscribeEchoReq* echoReq = new SubscribeEchoReq(*(SubscribeEchoReq*)m_strategy);  
			g_threadPool->Enqueue([echoReq, msg, eventLen] {
				echoReq->Done(msg, eventLen);

				delete echoReq;
			});
			break;
		}
	} while (false);
#endif
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



