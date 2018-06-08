/*************************************************************************
	> File Name: tnode_adapter.h
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Mon 06 Feb 2017 06:04:58 PM CST
 ************************************************************************/
#ifndef SOLUTIONGATEWAY_TNODE_ADAPTER_H_
#define SOLUTIONGATEWAY_TNODE_ADAPTER_H_
#include<atomic>
#include<vector>
#include<mutex>

#include"tnode.h"
#include"tnodeconfig_v0.h"

#if defined(DSOLUTION)
#include"marketdata.h"
#include"marketdataagent.h"
#endif

#include"safe_list.h"
#include"define.h"

//using namespace snetwork_xservice_tnode;
//namespace snetwork_xservice_solutiongateway {

/*TNodeAministrator begin ****/
//class snetwork_signalengine_marketdata::MarketDataAgent;
//extern SafeList<SendOrderListNode> g_send_order_list;
//extern SafeList<PositionListNode> g_position_list;
class TNodeAministrator {
	public:
		~TNodeAministrator();
	private:
		TNodeAministrator();

		TNodeAministrator(const TNodeAministrator& r) = delete;
		TNodeAministrator& operator=(const TNodeAministrator& r) = delete;
		TNodeAministrator(TNodeAministrator&& r) = delete;
		TNodeAministrator& operator=(TNodeAministrator&& r) = delete;

	public:
		static TNodeAministrator* GetInstance(void);

	public:
// #if defined(SIGNALENGINE)
// 		snetwork_signalengine_marketdata::MarketDataAgent* GetMarketDataAgent(void) const {
// 			return m_agent;
// 		}
// #endif
		//snetwork_xservice_tnode::TNodeChannel* GetChannel(void);
		snetwork_xservice_tnode::TNodeChannel GetChannel(void);
		bool PublishGroup(const char* group, const char* event, const char* msg, unsigned int len, unsigned short tag);

	private:
		//const std::string m_sendOrderGroupName;
		//const std::string m_sendOrderGroupEvent;
		static TNodeAministrator* m_instance;
		static std::mutex m_mutex;
		std::vector<snetwork_xservice_tnode::TNodeChannel*> m_channel;
		std::atomic<int> m_atomic;
// #if defined(SIGNALENGINE)
// 		snetwork_signalengine_marketdata::MarketDataAgent* m_agent;
// #endif
};
/*TNodeAministrator end ****/

/*Subscribe   begin ****/
class Consumer;
class Subscribe {
	public:
		Subscribe(Consumer* consumer, const char* eventName, const char* m_groupName);
		virtual ~Subscribe() = default;

		Subscribe(const Subscribe& r) = default;
		Subscribe& operator=(const Subscribe& r) = delete;
		Subscribe(Subscribe&& r) = delete;
		Subscribe& operator=(Subscribe&& r) = delete;

		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) = 0;

	protected:
		Consumer* m_consumer;
		const char*  m_eventName;
		//unsigned int m_eventNameLen;
		//const char* m_groupName;
		std::string m_groupName;
		//std::mutex m_mutex;
};
/*Subscribe end ****/

/*Consumer begin ****/
class Consumer : public snetwork_xservice_tnode::TNodeConsumer {
	public:
		explicit Consumer(const char* eventName, ETag tag);
		explicit Consumer(const std::string& eventName, ETag tag);
		virtual ~Consumer();

		Consumer(const Consumer& r) = delete;
		Consumer& operator=(const Consumer& r) = delete;
		Consumer(Consumer&& r) = delete;
		Consumer& operator=(Consumer&& r) = delete;

	public:
		std::string GetEventName() {
			return m_eventName;
		}

		int GetMessageTag() {
			return m_tag;
		}

		unsigned int EventHandle(unsigned short tag, char* event, unsigned int eventlen);
		unsigned int ServiceHandle(unsigned short tag, char* reqbody, unsigned int reqlen, snetwork_xservice_tnode::TNodeDataSend send); /* resever, empty implement*/

		void CreateSubscibe(void);

	private:
		Subscribe* m_strategy;
		std::string m_eventName;
		std::string m_groupName;
		ETag m_tag;
};
/*Consumer end ****/


//}

#endif // end of SOLUTIONGATEWAY_TNODE_ADAPTER_H_

