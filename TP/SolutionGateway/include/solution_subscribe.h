/*************************************************************************
	> File Name: solution_subscribe.h
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Tue 28 Feb 2017 07:36:02 PM CST
 ************************************************************************/
#ifndef SOLUTIONGATEWAY_SOLUTION_SUBSCRIBE_H_
#define SOLUTIONGATEWAY_SOLUTION_SUBSCRIBE_H_

#include<mutex>
#include<string.h>
#include<type_traits>
#include"tnode_adapter.h"
#include"ThreadPool.h"
#include<rapidjson/document.h>
#include<rapidjson/writer.h>
#include<rapidjson/rapidjson.h>
#include<rapidjson/stringbuffer.h>

//#include"solution_subscribe.h"

/*SubscribeUser begin ****/
class SubscribeUser : public Subscribe {
	public:
		SubscribeUser(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_USER);
		virtual ~SubscribeUser() = default;

		SubscribeUser(const SubscribeUser& r) =default ;
		SubscribeUser& operator=(const SubscribeUser& r) = delete;
		SubscribeUser(SubscribeUser&& r) =default ;
		SubscribeUser& operator=(SubscribeUser&& r) = delete;

		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeUser end ****/

/*SubscribeValid begin ****/
class SubscribeValid : public Subscribe {
	public:
		SubscribeValid(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_VALID);
		virtual ~SubscribeValid() = default;

		SubscribeValid(const SubscribeValid& r) =default ;
		SubscribeValid& operator=(const SubscribeValid& r) = delete;
		SubscribeValid(SubscribeValid&& r) =default ;
		SubscribeValid& operator=(SubscribeValid&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeValid end ****/

/*SubscribeAns begin */
class SubscribeAns : public Subscribe {
	public:
		SubscribeAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_ANS);
		virtual ~SubscribeAns() = default;

		SubscribeAns(const SubscribeAns& r) =default ;
		SubscribeAns& operator=(const SubscribeAns& r) = delete;
		SubscribeAns(SubscribeAns&& r) =default ;
		SubscribeAns& operator=(SubscribeAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeAns end */
/*SubscribeLogin begin */
class SubscribeLogin : public Subscribe {
	public:
		SubscribeLogin(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_LOGIN_NAME);
		virtual ~SubscribeLogin() = default;

		SubscribeLogin(const SubscribeLogin& r) =default ;
		SubscribeLogin& operator=(const SubscribeLogin& r) = delete;
		SubscribeLogin(SubscribeLogin&& r) =default ;
		SubscribeLogin& operator=(SubscribeLogin&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeLogin end */
/*SubscribeLogout begin */
class SubscribeLogout : public Subscribe {
	public:
		SubscribeLogout(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_LOGOUT_NAME);
		virtual ~SubscribeLogout() = default;

		SubscribeLogout(const SubscribeLogout& r) =default ;
		SubscribeLogout& operator=(const SubscribeLogout& r) = delete;
		SubscribeLogout(SubscribeLogout&& r) =default ;
		SubscribeLogout& operator=(SubscribeLogout&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeLogout end */
/*SubscribeUserInfo begin */
class SubscribeUserInfo : public Subscribe {
	public:
		SubscribeUserInfo(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_USERINFO_NAME);
		virtual ~SubscribeUserInfo() = default;

		SubscribeUserInfo(const SubscribeUserInfo& r) =default ;
		SubscribeUserInfo& operator=(const SubscribeUserInfo& r) = delete;
		SubscribeUserInfo(SubscribeUserInfo&& r) =default ;
		SubscribeUserInfo& operator=(SubscribeUserInfo&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeUserInfo end */

/*SubscribeOCPosition begin */
class SubscribeOCPosition : public Subscribe {
	public:
		SubscribeOCPosition(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_OCPOSITION_NAME);
		virtual ~SubscribeOCPosition() = default;

		SubscribeOCPosition(const SubscribeOCPosition& r) =default ;
		SubscribeOCPosition& operator=(const SubscribeOCPosition& r) = delete;
		SubscribeOCPosition(SubscribeOCPosition&& r) =default ;
		SubscribeOCPosition& operator=(SubscribeOCPosition&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeOCPosition end */

/*SubscribeForceLogout begin */
class SubscribeForceLogout : public Subscribe {
	public:
		SubscribeForceLogout(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_FORCELOGOUT_NAME);
		virtual ~SubscribeForceLogout() = default;

		SubscribeForceLogout(const SubscribeForceLogout& r) =default ;
		SubscribeForceLogout& operator=(const SubscribeForceLogout& r) = delete;
		SubscribeForceLogout(SubscribeForceLogout&& r) =default ;
		SubscribeForceLogout& operator=(SubscribeForceLogout&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeForceLogout end */

/*SubscribeChangePWD begin */
class SubscribeChangePWD : public Subscribe {
	public:
		SubscribeChangePWD(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_CHANGEPWD_NAME);
		virtual ~SubscribeChangePWD() = default;

		SubscribeChangePWD(const SubscribeChangePWD& r) =default ;
		SubscribeChangePWD& operator=(const SubscribeChangePWD& r) = delete;
		SubscribeChangePWD(SubscribeChangePWD&& r) =default ;
		SubscribeChangePWD& operator=(SubscribeChangePWD&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeChangePWD end */

/*SubscribeTradeConditions begin */
class SubscribeTradeConditions : public Subscribe {
	public:
		SubscribeTradeConditions(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_TRADE_CONDITIONS_NAME);
		virtual ~SubscribeTradeConditions() = default;

		SubscribeTradeConditions(const SubscribeTradeConditions& r) =default ;
		SubscribeTradeConditions& operator=(const SubscribeTradeConditions& r) = delete;
		SubscribeTradeConditions(SubscribeTradeConditions&& r) =default ;
		SubscribeTradeConditions& operator=(SubscribeTradeConditions&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeTradeConditions end */

/*SubscribeEchoReq begin*/
class SubscribeEchoReq : public Subscribe {
	public:
		SubscribeEchoReq(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_MONITOR_NAME);
		virtual ~SubscribeEchoReq() = default;

		SubscribeEchoReq(const SubscribeEchoReq& r) =default ;
		SubscribeEchoReq& operator=(const SubscribeEchoReq& r) = delete;
		SubscribeEchoReq(SubscribeEchoReq&& r) =default ;
		SubscribeEchoReq& operator=(SubscribeEchoReq&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeEchoReq end*/
#endif

