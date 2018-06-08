/*************************************************************************
	> File Name: position_subscribe.h > Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Tue 28 Feb 2017 07:26:28 PM CST
 ************************************************************************/
#ifndef POSITION_SUBSCRIBE_H_
#define POSITION_SUBSCRIBE_H_
#include"tnode_adapter.h"
#include<regex.h> /* for regex* */
#include<error.h>

/*SubscribeSendOrder begin */
class SubscribeSendOrder : public Subscribe {
	public:
		SubscribeSendOrder(Consumer* consumer, const char* eventName, const char* groupName=POSITIONMANAGEMENT_SUBSCRIBE_SENDORDER);
		virtual ~SubscribeSendOrder() = default;

		SubscribeSendOrder(const SubscribeSendOrder& r) =default ;
		SubscribeSendOrder& operator=(const SubscribeSendOrder& r) = delete;
		SubscribeSendOrder(SubscribeSendOrder&& r) =default ;
		SubscribeSendOrder& operator=(SubscribeSendOrder&& r) = delete;

	public:
		virtual void Done(const char* event, unsigned int eventLen) override;
		virtual void Done(char* event, unsigned int eventLen) override;

	public:
		static int StaticInit(void);
	protected:
		static regex_t re;
		/* orderans field + 1: because rm[0] store the all match string*/
		//regmatch_t m_rm[SENDORDER_FIELDS]; 
};
/*SubscribeSendOrder end */

/*SubscribeSystem begin */
class SubscribeSystem : public Subscribe {
	public:
		SubscribeSystem(Consumer* consumer, const char* eventName, const char* groupName=POSITIONMANAGEMENT_SUBSCRIBE_SYSTEM);
		virtual ~SubscribeSystem() = default;

		SubscribeSystem(const SubscribeSystem& r) =default ;
		SubscribeSystem& operator=(const SubscribeSystem& r) = delete;
		SubscribeSystem(SubscribeSystem&& r) =default ;
		SubscribeSystem& operator=(SubscribeSystem&& r) = delete;

	public:
		virtual void Done(const char* event, unsigned int eventLen) override;
		virtual void Done(char* event, unsigned int eventLen) override;

};
/*SubscribeSystem end */

/*SubscribeOrderAns begin */
class SubscribeOrderAns : public Subscribe {
	public:
		SubscribeOrderAns(Consumer* consumer, const char* eventName, const char* groupName=POSITIONMANAGEMENT_SUBSCRIBE_ORDERANS);
		virtual ~SubscribeOrderAns() = default;

		SubscribeOrderAns(const SubscribeOrderAns& r) =default ;
		SubscribeOrderAns& operator=(const SubscribeOrderAns& r) = delete;
		SubscribeOrderAns(SubscribeOrderAns&& r) =default ;
		SubscribeOrderAns& operator=(SubscribeOrderAns&& r) = delete;

	public:
		virtual void Done(const char* event, unsigned int eventLen) override;
		virtual void Done(char* event, unsigned int eventLen) override;

	public:
		static int StaticInit(void);
	protected:
		static regex_t re;
		/* orderans field + 1: because rm[0] store the all match string*/
		//regmatch_t m_rm[ORDERANS_FIELDS]; 
};
/*SubscribeOrderAns end */

/*SubscribePush begin */
class SubscribePush : public Subscribe {
	public:
		SubscribePush(Consumer* consumer, const char* eventName, const char* groupName=POSITIONMANAGEMENT_SUBSCRIBE_PUSH);
		virtual ~SubscribePush() = default;

		SubscribePush(const SubscribePush& r) =default ;
		SubscribePush& operator=(const SubscribePush& r) = delete;
		SubscribePush(SubscribePush&& r) =default ;
		SubscribePush& operator=(SubscribePush&& r) = delete;

	public:
		virtual void Done(const char* event, unsigned int eventLen) override;
		virtual void Done(char* event, unsigned int eventLen) override;

};
/*SubscribePush end */

/*SubscribePull begin */
class SubscribePull : public Subscribe {
	public:
		SubscribePull(Consumer* consumer, const char* eventName, const char* groupName=POSITIONMANAGEMENT_SUBSCRIBE_PULL);
		virtual ~SubscribePull() = default;

		SubscribePull(const SubscribePull& r) =default ;
		SubscribePull& operator=(const SubscribePull& r) = delete;
		SubscribePull(SubscribePull&& r) =default ;
		SubscribePull& operator=(SubscribePull&& r) = delete;

	public:
		virtual void Done(const char* event, unsigned int eventLen) override;
		virtual void Done(char* event, unsigned int eventLen) override;

};
/*SubscribePull end */

/*SubscribeModify begin */
class SubscribeModify : public Subscribe {
	public:
		SubscribeModify(Consumer* consumer, const char* eventName, const char* groupName=POSITIONMANAGEMENT_SUBSCRIBE_MODIFY);
		virtual ~SubscribeModify() = default;

		SubscribeModify(const SubscribeModify& r) =default ;
		SubscribeModify& operator=(const SubscribeModify& r) = delete;
		SubscribeModify(SubscribeModify&& r) =default ;
		SubscribeModify& operator=(SubscribeModify&& r) = delete;

	public:
		virtual void Done(const char* event, unsigned int eventLen) override;
		virtual void Done(char* event, unsigned int eventLen) override;

};
/*SubscribeModify end */

/*SubscribeEchoReq begin*/
class SubscribeEchoReq : public Subscribe {
	public:
		SubscribeEchoReq(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_MONITOR_NAME);
		virtual ~SubscribeEchoReq() = default;

		SubscribeEchoReq(const SubscribeEchoReq& r) = default ;
		SubscribeEchoReq& operator=(const SubscribeEchoReq& r) = delete;
		SubscribeEchoReq(SubscribeEchoReq&& r) = default ;
		SubscribeEchoReq& operator=(SubscribeEchoReq&& r) = delete;

	public:
		virtual void Done(const char* event, unsigned int eventLen) override;
		virtual void Done(char* event, unsigned int eventLen) override;

};
/*SubscribeEchoReq end*/
#endif
