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
#include"object_pool.h"

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

using CMallocSizeNode = CMallocSize<char, CMALLOC_SIZE_TNODE>;
using CMallocSizePool = ObjectPool<CMallocSizeNode, 20>;
extern CMallocSizePool* g_cmallocSizePool;

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
	config.VHost("/demo").Exchange("t_node").ExchangeGroup("t_node_grp").Address("localhost").Port("5672").User("demo").Password("demo");
#endif

	snetwork_xservice_tnode::TNode tnode;
	tnode.Config(config);
	tnode.Bind(1);
	//tnode.SetReadThreadNums(3);
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

void TNodeAministrator::PublishGroup(const char* group, const char* event, const char* msg, unsigned int len, unsigned short tag) {
	snetwork_xservice_tnode::TNode tnode;
	tnode.Bind(1);

	snetwork_xservice_tnode::TNodeChannel channel = tnode.GetChannel();
	char* publish_msg = channel.AllocDataBuff(len);
	memcpy(publish_msg, msg, len-1);
	publish_msg[len-1] = 0;
	
	channel.PublishGroup(group, event, publish_msg, len, tag);
#if defined(DEBUG)
	//XINFO("publish groupname:<%s>, event:<%s>, tag=<0x%04X or %d>,msg:<%s>, len=%d\n", group, event,tag, tag, msg, len);
#endif
}


/* TNodeAministrator end ****/
/*Subscribe   begin ****/
Subscribe::Subscribe(Consumer* consumer, const char* eventName, const char* groupName) :
	m_consumer(consumer),
	m_eventName(eventName),
	//m_eventNameLen(eventNameLen),
	m_groupName(groupName) {
	snetwork_xservice_tnode::TNodeChannel channel = TNodeAministrator::GetInstance()->GetChannel();

	channel.CreateGroup(m_groupName);
	channel.AddConsumerToGroup(m_groupName, m_consumer);
}

Subscribe::Subscribe(const Subscribe& r) {
	//m_consumer = r.m_consumer;
	//m_eventName = r.m_eventName;
	//m_groupName = r.m_groupName;
	//printf("------------------\n");
}

Subscribe::Subscribe(Subscribe&& r) {
	//printf("+++++++++++++++++++\n");
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
			if (m_eventName == POSITIONMANAGEMENT_SUBSCRIBE_POSITION_EVENT.c_str()) {
				m_strategy = new SubscribeSendOrder(this, m_eventName.c_str());
			}

			break;
#endif
		case EORDERANS: /* 0x0002 */
#if 0
			if (m_eventName == POSITIONMANAGEMENT_SUBSCRIBE_POSITION_EVENT) {
				m_strategy = new SubscribeOrderAns(this, m_eventName.c_str());
			} else {
				m_strategy = new SubscribeAns(this, m_eventName.c_str());
			}
			break;
#endif
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
#endif
		case EMONITOR: /* RM->SG MONITOR.SGID */
			m_strategy = new SubscribeEchoReq(this, m_eventName.c_str());
			break;	

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
#if defined(DEBUG)
	XINFO("time:%ld, msg:<%s>\n", GetMicroSecond(), event);
#endif
	//return UNIVERSAL_TNODE_SUCCESS;
	
#if defined(MULTIPLE_THREAD)
	char* msg = nullptr;
	CMallocSizeNode* cmallocSize = nullptr;
	if (eventLen < CMALLOC_SIZE_TNODE) {
		cmallocSize = g_cmallocSizePool->GetObject();
		msg = cmallocSize->GetData();
	} else {
		msg = (char*)malloc(eventLen);
		if (nullptr == msg) {
			free(msg);
			msg = nullptr;

			return UNIVERSAL_TNODE_UNKNOWN;
		}
	}

	memcpy(msg, event, eventLen);

	do {
#if defined(DPOSITION)
		if (nullptr != dynamic_cast<SubscribeSendOrder*>(m_strategy)) {
			SubscribeSendOrder* sendOrder = new SubscribeSendOrder(*(SubscribeSendOrder*)m_strategy);  
			g_threadPool->Enqueue([sendOrder, msg, eventLen, cmallocSize] {
#if 0
				long sendOrderBegin = GetMicroSecond();
				sendOrder->Done((const char*)msg, eventLen);
#endif
				sendOrder->Done(msg, eventLen);

				if (eventLen < CMALLOC_SIZE_TNODE) {
					g_cmallocSizePool->ReleaseObject(cmallocSize);
				} else {
					free(msg);
				}
				delete sendOrder;
#if 0
				long sendOrderEnd = GetMicroSecond();
				XINFO("sendOrder: %ld\t%ld\t%ld\n", 
					   sendOrderEnd, 
					   sendOrderBegin, 
					   sendOrderEnd - sendOrderBegin);
#endif
			});
			break;
		}
#endif

#if defined(DPOSITION)
		if (nullptr != dynamic_cast<SubscribeOrderAns*>(m_strategy)) {
			SubscribeOrderAns* orderAns = new SubscribeOrderAns(*(SubscribeOrderAns*)m_strategy);  
			g_threadPool->Enqueue([orderAns, msg, eventLen, cmallocSize] {
#if 0
				long orderAnsBegin = GetMicroSecond();
				orderAns->Done((const char*)msg, eventLen);
#endif
				orderAns->Done(msg, eventLen);

				if (eventLen < CMALLOC_SIZE_TNODE) {
					g_cmallocSizePool->ReleaseObject(cmallocSize);
				} else {
					free(msg);
				}
				delete orderAns;
#if 0
				long orderAnsEnd = GetMicroSecond();
				XINFO("orderAns: %ld\t%ld\t%ld\n", 
					   orderAnsEnd, 
					   orderAnsBegin, 
					   orderAnsEnd - orderAnsBegin);
#endif
			});
			break;
		}
#endif

#if defined(DSOLUTION)
		if (nullptr != dynamic_cast<SubscribeAns*>(m_strategy)) {
			SubscribeAns* ordAns = new SubscribeAns(*(SubscribeAns*)m_strategy);  
			g_threadPool->Enqueue([ordAns, msg, eventLen, cmallocSize] {
				ordAns->Done(msg, eventLen);

				if (eventLen < CMALLOC_SIZE_TNODE) {
					g_cmallocSizePool->ReleaseObject(cmallocSize);
				} else {
					free(msg);
				}
				delete ordAns;
			});
			break;
		}
#endif

#if defined(DSOLUTION)
		if (nullptr != dynamic_cast<SubscribeValid*>(m_strategy)) {
			SubscribeValid* valid = new SubscribeValid(*(SubscribeValid*)m_strategy);  
			g_threadPool->Enqueue([valid, msg, eventLen, cmallocSize] {
				valid->Done(msg, eventLen);

				if (eventLen < CMALLOC_SIZE_TNODE) {
					g_cmallocSizePool->ReleaseObject(cmallocSize);
				} else {
					free(msg);
				}
				delete valid;
			});
			break;
		}
#endif

#if defined(DPOSITION)
		if (nullptr != dynamic_cast<SubscribeSystem*>(m_strategy)) {
			SubscribeSystem* system = new SubscribeSystem(*(SubscribeSystem*)m_strategy);  
			g_threadPool->Enqueue([system, msg, eventLen, cmallocSize] {
				system->Done(msg, eventLen);

				//free(msg);
				if (eventLen < CMALLOC_SIZE_TNODE) {
					g_cmallocSizePool->ReleaseObject(cmallocSize);
				} else {
					free(msg);
				}
				delete system;
			});
			break;
		}
#endif

#if defined(DSOLUTION)
		if (nullptr != dynamic_cast<SubscribeUser*>(m_strategy)) {
			SubscribeUser* user = new SubscribeUser(*(SubscribeUser*)m_strategy);  
			g_threadPool->Enqueue([user, msg, eventLen, cmallocSize] {
				user->Done(msg, eventLen);

				//free(msg);
				if (eventLen < CMALLOC_SIZE_TNODE) {
					g_cmallocSizePool->ReleaseObject(cmallocSize);
				} else {
					free(msg);
				}
				delete user;
			});
			break;
		}
#endif

#if defined(DPOSITION)
		if (nullptr != dynamic_cast<SubscribePull*>(m_strategy)) {
			SubscribePull* pull = new SubscribePull(*(SubscribePull*)m_strategy);  
			g_threadPool->Enqueue([pull, msg, eventLen, cmallocSize] {
				pull->Done(msg, eventLen);

				//free(msg);
				if (eventLen < CMALLOC_SIZE_TNODE) {
					g_cmallocSizePool->ReleaseObject(cmallocSize);
				} else {
					free(msg);
				}
				delete pull;
			});
			break;
		}
#endif

#if defined(DSOLUTION)
		if (nullptr != dynamic_cast<SubscribeUserInfo*>(m_strategy)) {
			SubscribeUserInfo* userInfo = new SubscribeUserInfo(*(SubscibeUserInfo*)m_strategy);  
			g_threadPool->Enqueue([userInfo, msg, eventLen, cmallocSize] {
				userInfo->Done(msg, eventLen);

				//free(msg);
				if (eventLen < CMALLOC_SIZE_TNODE) {
					g_cmallocSizePool->ReleaseObject(cmallocSize);
				} else {
					free(msg);
				}
				delete userInfo;
			});
			break;
		}

		if (nullptr != dynamic_cast<SubscribeLogin*>(m_strategy)) {
			SubscribeLogin* login = new SubscribeLogin(*(SubscribeLogin*)m_strategy);  
			g_threadPool->Enqueue([login, msg, eventLen, cmallocSize] {
				login->Done(msg, eventLen);

				//free(msg);
				if (eventLen < CMALLOC_SIZE_TNODE) {
					g_cmallocSizePool->ReleaseObject(cmallocSize);
				} else {
					free(msg);
				}
				delete login;
			});
			break;
		}

		if (nullptr != dynamic_cast<SubscribeOCPosition*>(m_strategy)) {
			SubscribeOCPosition* ocPosition = new SubscribeOCPosition(*(SubscribeOCPosition*)m_strategy);  
			g_threadPool->Enqueue([ocPosition, msg, eventLen, cmallocSize] {
				ocPosition->Done(msg, eventLen);

				//free(msg);
				if (eventLen < CMALLOC_SIZE_TNODE) {
					g_cmallocSizePool->ReleaseObject(cmallocSize);
				} else {
					free(msg);
				}
				delete ocPosition;
			});
			break;
		}

		if (nullptr != dynamic_cast<SubscribeLogout*>(m_strategy)) {
			SubscribeLogout* logout = new SubscribeLogout(*(SubscribeLogout*)m_strategy);  
			g_threadPool->Enqueue([logout, msg, eventLen, cmallocSize] {
				logout->Done(msg, eventLen);

				//free(msg);
				if (eventLen < CMALLOC_SIZE_TNODE) {
					g_cmallocSizePool->ReleaseObject(cmallocSize);
				} else {
					free(msg);
				}
				delete logout;
			});
			break;
		}

		if (nullptr != dynamic_cast<SubscribeForceLogout*>(m_strategy)) {
			SubscribeForceLogout* forceLogout = new SubscribeForceLogout(*m_strategy);  
			g_threadPool->Enqueue([forceLogout, msg, eventLen, cmallocSize] {
				forceLogout->Done(msg, eventLen);

				//free(msg);
				if (eventLen < CMALLOC_SIZE_TNODE) {
					g_cmallocSizePool->ReleaseObject(cmallocSize);
				} else {
					free(msg);
				}
				delete forceLogout;
			});
			break;
		}
#endif

#if defined(DSOLUTION)
		if (nullptr != dynamic_cast<SubscribeEchoReq*>(m_strategy)) {
			SubscribeEchoReq* echoReq = new SubscribeEchoReq(*(SubscribeEchoReq*)m_strategy);  
			g_threadPool->Enqueue([echoReq, msg, eventLen] {
				echoReq->Done(msg, eventLen);

				delete echoReq;
			});
			break;
		}
#elif defined(DPOSITION)
		if (nullptr != dynamic_cast<SubscribeEchoReq*>(m_strategy)) {
			SubscribeEchoReq* echoReq = new SubscribeEchoReq(*(SubscribeEchoReq*)m_strategy);  
			g_threadPool->Enqueue([echoReq, msg, eventLen, cmallocSize] {
				echoReq->Done(msg, eventLen);

				if (eventLen < CMALLOC_SIZE_TNODE) {
					g_cmallocSizePool->ReleaseObject(cmallocSize);
				} else {
					free(msg);
				}

				delete echoReq;
			});
			break;
		}
#endif

	} while (false);
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



