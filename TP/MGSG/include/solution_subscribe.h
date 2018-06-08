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
#include"solution_subscribe.h"

/*SubscribeForceLogoutAns begin */
class SubscribeForceLogoutAns : public Subscribe {
	public:
		SubscribeForceLogoutAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_FORCELOGOUT_NAME);
		virtual ~SubscribeForceLogoutAns() = default;

		SubscribeForceLogoutAns(const SubscribeForceLogoutAns& r) =delete ;
		SubscribeForceLogoutAns& operator=(const SubscribeForceLogoutAns& r) = delete;
		SubscribeForceLogoutAns(SubscribeForceLogoutAns&& r) =delete ;
		SubscribeForceLogoutAns& operator=(SubscribeForceLogoutAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeForceLogoutAns end */

/*SubscribeEchoReq begin*/
class SubscribeEchoReq : public Subscribe {
	public:
		SubscribeEchoReq(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_MONITOR_NAME);
		virtual ~SubscribeEchoReq() = default;

		SubscribeEchoReq(const SubscribeEchoReq& r) =delete ;
		SubscribeEchoReq& operator=(const SubscribeEchoReq& r) = delete;
		SubscribeEchoReq(SubscribeEchoReq&& r) =delete ;
		SubscribeEchoReq& operator=(SubscribeEchoReq&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeEchoReq end*/

 /*SubscribeLogoutAns begin*/
class SubscribeLoginAns : public Subscribe {
	public:
		SubscribeLoginAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_LOGINANS_NAME);
		virtual ~SubscribeLoginAns() = default;

		SubscribeLoginAns(const SubscribeLoginAns& r) =delete ;
		SubscribeLoginAns& operator=(const SubscribeLoginAns& r) = delete;
		SubscribeLoginAns(SubscribeLoginAns&& r) =delete ;
		SubscribeLoginAns& operator=(SubscribeLoginAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeLogoutAns end*/

/*SubscribeLogoutAns begin*/
class SubscribeLogoutAns : public Subscribe {
	public:
		SubscribeLogoutAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_LOGOUTANS_NAME);
		virtual ~SubscribeLogoutAns() = default;

		SubscribeLogoutAns(const SubscribeLogoutAns& r) =delete ;
		SubscribeLogoutAns& operator=(const SubscribeLogoutAns& r) = delete;
		SubscribeLogoutAns(SubscribeLogoutAns&& r) =delete ;
		SubscribeLogoutAns& operator=(SubscribeLogoutAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeLogoutAns end*/

/*SubscribeChangePasswordAns begin*/
class SubscribeChangePasswordAns : public Subscribe {
	public:
		SubscribeChangePasswordAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_CHANGEPASSWORDANS_NAME);
		virtual ~SubscribeChangePasswordAns() = default;

		SubscribeChangePasswordAns(const SubscribeChangePasswordAns& r) =delete ;
		SubscribeChangePasswordAns& operator=(const SubscribeChangePasswordAns& r) = delete;
		SubscribeChangePasswordAns(SubscribeChangePasswordAns&& r) =delete ;
		SubscribeChangePasswordAns& operator=(SubscribeChangePasswordAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeChangePasswordAns end*/

/*SubscribeChangeBalanceAns begin*/
class SubscribeChangeBalanceAns : public Subscribe {
	public:
		SubscribeChangeBalanceAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_CHANGEBALANCEANS_NAME);
		virtual ~SubscribeChangeBalanceAns() = default;

		SubscribeChangeBalanceAns(const SubscribeChangeBalanceAns& r) =delete ;
		SubscribeChangeBalanceAns& operator=(const SubscribeChangeBalanceAns& r) = delete;
		SubscribeChangeBalanceAns(SubscribeChangeBalanceAns&& r) =delete ;
		SubscribeChangeBalanceAns& operator=(SubscribeChangeBalanceAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeChangeBalanceAns end*/

/*SubscribeUpdateTraderAns begin*/
class SubscribeUpdateTraderAns : public Subscribe {
	public:
		SubscribeUpdateTraderAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_UPDATETRADERANS_NAME);
		virtual ~SubscribeUpdateTraderAns() = default;

		SubscribeUpdateTraderAns(const SubscribeUpdateTraderAns& r) =delete ;
		SubscribeUpdateTraderAns& operator=(const SubscribeUpdateTraderAns& r) = delete;
		SubscribeUpdateTraderAns(SubscribeUpdateTraderAns&& r) =delete ;
		SubscribeUpdateTraderAns& operator=(SubscribeUpdateTraderAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeUpdateTraderAns end*/

/*SubscribeGetTradersAns begin*/
class SubscribeGetTradersAns : public Subscribe {
	public:
		SubscribeGetTradersAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_GETTRADERSANS_NAME);
		virtual ~SubscribeGetTradersAns() = default;

		SubscribeGetTradersAns(const SubscribeGetTradersAns& r) =delete ;
		SubscribeGetTradersAns& operator=(const SubscribeGetTradersAns& r) = delete;
		SubscribeGetTradersAns(SubscribeGetTradersAns&& r) =delete ;
		SubscribeGetTradersAns& operator=(SubscribeGetTradersAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeGetTradersAns end*/

/*SubscribeInsertUpdateManagerRightsAns begin*/
class SubscribeInsertUpdateManagerRightsAns : public Subscribe {
	public:
		SubscribeInsertUpdateManagerRightsAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_INSERTUPDATEMANAGERRIGHTSANS_NAME);
		virtual ~SubscribeInsertUpdateManagerRightsAns() = default;

		SubscribeInsertUpdateManagerRightsAns(const SubscribeInsertUpdateManagerRightsAns& r) =delete ;
		SubscribeInsertUpdateManagerRightsAns& operator=(const SubscribeInsertUpdateManagerRightsAns& r) = delete;
		SubscribeInsertUpdateManagerRightsAns(SubscribeInsertUpdateManagerRightsAns&& r) =delete ;
		SubscribeInsertUpdateManagerRightsAns& operator=(SubscribeInsertUpdateManagerRightsAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeInsertUpdateManagerRightsAns end*/

/*SubscribeGetManagerAccessAns begin*/
class SubscribeGetManagerAccessAns : public Subscribe {
	public:
		SubscribeGetManagerAccessAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_GETMANAGERACCESSANS_NAME);
		virtual ~SubscribeGetManagerAccessAns() = default;

		SubscribeGetManagerAccessAns(const SubscribeGetManagerAccessAns& r) =delete ;
		SubscribeGetManagerAccessAns& operator=(const SubscribeGetManagerAccessAns& r) = delete;
		SubscribeGetManagerAccessAns(SubscribeGetManagerAccessAns&& r) =delete ;
		SubscribeGetManagerAccessAns& operator=(SubscribeGetManagerAccessAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeGetManagerAccessAns end*/

/*SubscribeUserDeleteCheckAns begin*/
class SubscribeUserDeleteCheckAns : public Subscribe {
	public:
		SubscribeUserDeleteCheckAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_USERDELETECHECKANS_NAME);
		virtual ~SubscribeUserDeleteCheckAns() = default;

		SubscribeUserDeleteCheckAns(const SubscribeUserDeleteCheckAns& r) =delete ;
		SubscribeUserDeleteCheckAns& operator=(const SubscribeUserDeleteCheckAns& r) = delete;
		SubscribeUserDeleteCheckAns(SubscribeUserDeleteCheckAns&& r) =delete ;
		SubscribeUserDeleteCheckAns& operator=(SubscribeUserDeleteCheckAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeUserDeleteCheckAns end*/

/*SubscribeUserDeleteAns begin*/
class SubscribeUserDeleteAns : public Subscribe {
	public:
		SubscribeUserDeleteAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_USERDELETEANS_NAME);
		virtual ~SubscribeUserDeleteAns() = default;

		SubscribeUserDeleteAns(const SubscribeUserDeleteAns& r) =delete ;
		SubscribeUserDeleteAns& operator=(const SubscribeUserDeleteAns& r) = delete;
		SubscribeUserDeleteAns(SubscribeUserDeleteAns&& r) =delete ;
		SubscribeUserDeleteAns& operator=(SubscribeUserDeleteAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeUserDeleteAns end*/

/*SubscribeCreateTraderAns begin*/
class SubscribeCreateTraderAns : public Subscribe {
	public:
		SubscribeCreateTraderAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_CREATETRADERANS_NAME);
		virtual ~SubscribeCreateTraderAns() = default;

		SubscribeCreateTraderAns(const SubscribeCreateTraderAns& r) =delete ;
		SubscribeCreateTraderAns& operator=(const SubscribeCreateTraderAns& r) = delete;
		SubscribeCreateTraderAns(SubscribeCreateTraderAns&& r) =delete ;
		SubscribeCreateTraderAns& operator=(SubscribeCreateTraderAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeCreateTraderAns end*/

/*SubscribeGetOnlineTraderAns begin*/
class SubscribeGetOnlineTraderAns : public Subscribe {
	public:
		SubscribeGetOnlineTraderAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_GETONLINETRADERANS_NAME);
		virtual ~SubscribeGetOnlineTraderAns() = default;

		SubscribeGetOnlineTraderAns(const SubscribeGetOnlineTraderAns& r) =delete ;
		SubscribeGetOnlineTraderAns& operator=(const SubscribeGetOnlineTraderAns& r) = delete;
		SubscribeGetOnlineTraderAns(SubscribeGetOnlineTraderAns&& r) =delete ;
		SubscribeGetOnlineTraderAns& operator=(SubscribeGetOnlineTraderAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeGetOnlineTraderAns end*/

/*SubscribeDisableTraderAns begin*/
class SubscribeDisableTraderAns : public Subscribe {
	public:
		SubscribeDisableTraderAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_DISABLETRADERANS_NAME);
		virtual ~SubscribeDisableTraderAns() = default;

		SubscribeDisableTraderAns(const SubscribeDisableTraderAns& r) =delete ;
		SubscribeDisableTraderAns& operator=(const SubscribeDisableTraderAns& r) = delete;
		SubscribeDisableTraderAns(SubscribeDisableTraderAns&& r) =delete ;
		SubscribeDisableTraderAns& operator=(SubscribeDisableTraderAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;
};
/*SubscribeDisableTraderAns end*/

/*SubscribeKickoutTraderAns begin*/
class SubscribeKickoutTraderAns : public Subscribe {
	public:
		SubscribeKickoutTraderAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_KICKOUTTRADERANS_NAME);
		virtual ~SubscribeKickoutTraderAns() = default;

		SubscribeKickoutTraderAns(const SubscribeKickoutTraderAns& r) =delete ;
		SubscribeKickoutTraderAns& operator=(const SubscribeKickoutTraderAns& r) = delete;
		SubscribeKickoutTraderAns(SubscribeKickoutTraderAns&& r) =delete ;
		SubscribeKickoutTraderAns& operator=(SubscribeKickoutTraderAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeKickoutTraderAns end*/

/*SubscribeGetTraderByTraderIdAns begin*/
class SubscribeGetTraderByTraderIdAns : public Subscribe {
	public:
		SubscribeGetTraderByTraderIdAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_GETTRADERBYTRADERIDANS_NAME);
		virtual ~SubscribeGetTraderByTraderIdAns() = default;

		SubscribeGetTraderByTraderIdAns(const SubscribeGetTraderByTraderIdAns& r) =delete ;
		SubscribeGetTraderByTraderIdAns& operator=(const SubscribeGetTraderByTraderIdAns& r) = delete;
		SubscribeGetTraderByTraderIdAns(SubscribeGetTraderByTraderIdAns&& r) =delete ;
		SubscribeGetTraderByTraderIdAns& operator=(SubscribeGetTraderByTraderIdAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeGetTraderByTraderIdAns end*/

/* SubscribeGetAllGroupByManagerIdAns begin*/
class SubscribeGetAllGroupByManagerIdAns : public Subscribe {
	public:
		SubscribeGetAllGroupByManagerIdAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_GETALLGROUPBYMANAGERIDANS_NAME);
		virtual ~SubscribeGetAllGroupByManagerIdAns() = default;

		SubscribeGetAllGroupByManagerIdAns(const SubscribeGetAllGroupByManagerIdAns& r) =delete ;
		SubscribeGetAllGroupByManagerIdAns& operator=(const SubscribeGetAllGroupByManagerIdAns& r) = delete;
		SubscribeGetAllGroupByManagerIdAns(SubscribeGetAllGroupByManagerIdAns&& r) =delete ;
		SubscribeGetAllGroupByManagerIdAns& operator=(SubscribeGetAllGroupByManagerIdAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeGetAllGroupByManagerIdAns end*/

//for RM 
/*SubscribeGetPositionsAns begin*/
class SubscribeGetPositionsAns : public Subscribe {
	public:
		SubscribeGetPositionsAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_GETPOSITIONSANS_NAME);
		virtual ~SubscribeGetPositionsAns() = default;

		SubscribeGetPositionsAns(const SubscribeGetPositionsAns& r) =delete ;
		SubscribeGetPositionsAns& operator=(const SubscribeGetPositionsAns& r) = delete;
		SubscribeGetPositionsAns(SubscribeGetPositionsAns&& r) =delete ;
		SubscribeGetPositionsAns& operator=(SubscribeGetPositionsAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeGetPositionsAns end*/

/*SubscribeGetTop5boAns begin*/
class SubscribeGetTop5boAns : public Subscribe {
	public:
		SubscribeGetTop5boAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_GETTOP5BOANS_NAME);
		virtual ~SubscribeGetTop5boAns() = default;

		SubscribeGetTop5boAns(const SubscribeGetTop5boAns& r) =delete ;
		SubscribeGetTop5boAns& operator=(const SubscribeGetTop5boAns& r) = delete;
		SubscribeGetTop5boAns(SubscribeGetTop5boAns&& r) =delete ;
		SubscribeGetTop5boAns& operator=(SubscribeGetTop5boAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeGetTop5boAns end*/

/*SubscribeGetCustomizeBoAns begin*/
class SubscribeGetCustomizeBoAns : public Subscribe {
	public:
		SubscribeGetCustomizeBoAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_GETCUSTOMIZEBOANS_NAME);
		virtual ~SubscribeGetCustomizeBoAns() = default;

		SubscribeGetCustomizeBoAns(const SubscribeGetCustomizeBoAns& r) =delete ;
		SubscribeGetCustomizeBoAns& operator=(const SubscribeGetCustomizeBoAns& r) = delete;
		SubscribeGetCustomizeBoAns(SubscribeGetCustomizeBoAns&& r) =delete ;
		SubscribeGetCustomizeBoAns& operator=(SubscribeGetCustomizeBoAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeGetCustomizeBoAns end*/

/*SubscribeGetSymbolMonitorAns begin*/
class SubscribeGetSymbolMonitorAns : public Subscribe {
	public:
		SubscribeGetSymbolMonitorAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_GETSYMBOLMONITORANS_NAME);
		virtual ~SubscribeGetSymbolMonitorAns() = default;

		SubscribeGetSymbolMonitorAns(const SubscribeGetSymbolMonitorAns& r) =delete ;
		SubscribeGetSymbolMonitorAns& operator=(const SubscribeGetSymbolMonitorAns& r) = delete;
		SubscribeGetSymbolMonitorAns(SubscribeGetSymbolMonitorAns&& r) =delete ;
		SubscribeGetSymbolMonitorAns& operator=(SubscribeGetSymbolMonitorAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeGetSymbolMonitorAns end*/

/*SubscribeRiskManagementTop5Bo begin*/
class SubscribeRiskManagementTop5Bo : public Subscribe {
	public:
		SubscribeRiskManagementTop5Bo(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_RISKMANAGEMENTTOP5BO_NAME);
		virtual ~SubscribeRiskManagementTop5Bo() = default;

		SubscribeRiskManagementTop5Bo(const SubscribeRiskManagementTop5Bo& r) =delete ;
		SubscribeRiskManagementTop5Bo& operator=(const SubscribeRiskManagementTop5Bo& r) = delete;
		SubscribeRiskManagementTop5Bo(SubscribeRiskManagementTop5Bo&& r) =delete ;
		SubscribeRiskManagementTop5Bo& operator=(SubscribeRiskManagementTop5Bo&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeRiskManagementTop5Bo end*/

/*SubscribeRiskManagementCustomBo begin*/
class SubscribeRiskManagementCustomBo : public Subscribe {
	public:
		SubscribeRiskManagementCustomBo(Consumer* consumer, const char*eventName, const char* groupName = SOLUTIONGATEWAY_SUBSCRIBE_RISKMANAGEMENTCUSTOMBO_NAME);
		virtual ~SubscribeRiskManagementCustomBo() = default;

		SubscribeRiskManagementCustomBo(const SubscribeRiskManagementCustomBo& r) =delete ;
		SubscribeRiskManagementCustomBo& operator=(const SubscribeRiskManagementCustomBo& r) = delete;
		SubscribeRiskManagementCustomBo(SubscribeRiskManagementCustomBo&& r) =delete ;
		SubscribeRiskManagementCustomBo& operator=(SubscribeRiskManagementCustomBo&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeRiskManagementCustomBo end*/

/*SubscribeRiskManagementSymbolBo begin*/
class SubscribeRiskManagementSymbolBo : public Subscribe {
	public:
		SubscribeRiskManagementSymbolBo(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_RISKMANAGEMENTSYMBOLBO_NAME);
		virtual ~SubscribeRiskManagementSymbolBo() = default;

		SubscribeRiskManagementSymbolBo(const SubscribeRiskManagementSymbolBo& r) =delete ;
		SubscribeRiskManagementSymbolBo& operator=(const SubscribeRiskManagementSymbolBo& r) = delete;
		SubscribeRiskManagementSymbolBo(SubscribeRiskManagementSymbolBo&& r) =delete ;
		SubscribeRiskManagementSymbolBo& operator=(SubscribeRiskManagementSymbolBo&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeRiskManagementSymbolBo end*/

/*SubscribeRMUpdateTraderBalance begin*/
class SubscribeRMUpdateTraderBalance : public Subscribe {
	public:
		SubscribeRMUpdateTraderBalance(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_RMUPDATETRADERBALANCE_NAME);
		virtual ~SubscribeRMUpdateTraderBalance() = default;

		SubscribeRMUpdateTraderBalance(const SubscribeRMUpdateTraderBalance& r) =delete ;
		SubscribeRMUpdateTraderBalance& operator=(const SubscribeRMUpdateTraderBalance& r) = delete;
		SubscribeRMUpdateTraderBalance(SubscribeRMUpdateTraderBalance&& r) =delete ;
		SubscribeRMUpdateTraderBalance& operator=(SubscribeRMUpdateTraderBalance&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeRMUpdateTraderBalance end*/


/*SubscribeOnlineTraderOnRoot begin*/
class SubscribeOnlineTraderOnRoot : public Subscribe {
	public:
		SubscribeOnlineTraderOnRoot(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_ONLINETRADERONROOT_NAME);
		virtual ~SubscribeOnlineTraderOnRoot() = default;

		SubscribeOnlineTraderOnRoot(const SubscribeOnlineTraderOnRoot& r) =delete ;
		SubscribeOnlineTraderOnRoot& operator=(const SubscribeOnlineTraderOnRoot& r) = delete;
		SubscribeOnlineTraderOnRoot(SubscribeOnlineTraderOnRoot&& r) =delete ;
		SubscribeOnlineTraderOnRoot& operator=(SubscribeOnlineTraderOnRoot&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeOnlineTraderOnRoot end*/

/*SubscribeLogoutTraderOnRoot begin*/
class SubscribeLogoutTraderOnRoot : public Subscribe {
	public:
		SubscribeLogoutTraderOnRoot(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_LOGOUTTRADERONROOT_NAME);
		virtual ~SubscribeLogoutTraderOnRoot() = default;

		SubscribeLogoutTraderOnRoot(const SubscribeLogoutTraderOnRoot& r) =delete ;
		SubscribeLogoutTraderOnRoot& operator=(const SubscribeLogoutTraderOnRoot& r) = delete;
		SubscribeLogoutTraderOnRoot(SubscribeLogoutTraderOnRoot&& r) =delete ;
		SubscribeLogoutTraderOnRoot& operator=(SubscribeLogoutTraderOnRoot&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeLogoutTraderOnRoot end*/

/*SubscribeNewTraderOnRoot begin*/
class SubscribeNewTraderOnRoot : public Subscribe {
	public:
		SubscribeNewTraderOnRoot(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_NEWTRADERONROOT_NAME);
		virtual ~SubscribeNewTraderOnRoot() = default;

		SubscribeNewTraderOnRoot(const SubscribeNewTraderOnRoot& r) =delete ;
		SubscribeNewTraderOnRoot& operator=(const SubscribeNewTraderOnRoot& r) = delete;
		SubscribeNewTraderOnRoot(SubscribeNewTraderOnRoot&& r) =delete ;
		SubscribeNewTraderOnRoot& operator=(SubscribeNewTraderOnRoot&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeNewTraderOnRoot end*/

/*SubscribeUpdateTraderOnRoot begin*/
class SubscribeUpdateTraderOnRoot : public Subscribe {
	public:
		SubscribeUpdateTraderOnRoot(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_UPDATETRADERONROOT_NAME);
		virtual ~SubscribeUpdateTraderOnRoot() = default;

		SubscribeUpdateTraderOnRoot(const SubscribeUpdateTraderOnRoot& r) =delete ;
		SubscribeUpdateTraderOnRoot& operator=(const SubscribeUpdateTraderOnRoot& r) = delete;
		SubscribeUpdateTraderOnRoot(SubscribeUpdateTraderOnRoot&& r) =delete ;
		SubscribeUpdateTraderOnRoot& operator=(SubscribeUpdateTraderOnRoot&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeUpdateTraderOnRoot end*/

/*SubscribeDeleteTraderOnRoot begin*/
class SubscribeDeleteTraderOnRoot : public Subscribe {
	public:
		SubscribeDeleteTraderOnRoot(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_DELETETRADERONROOT_NAME);
		virtual ~SubscribeDeleteTraderOnRoot() = default;

		SubscribeDeleteTraderOnRoot(const SubscribeDeleteTraderOnRoot& r) =delete ;
		SubscribeDeleteTraderOnRoot& operator=(const SubscribeDeleteTraderOnRoot& r) = delete;
		SubscribeDeleteTraderOnRoot(SubscribeDeleteTraderOnRoot&& r) =delete ;
		SubscribeDeleteTraderOnRoot& operator=(SubscribeDeleteTraderOnRoot&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;
};
/*SubscribeDeleteTraderOnRoot end*/

/*SubscribeUpdateOpenTradeOnRoot begin*/
class SubscribeUpdateOpenTradeOnRoot : public Subscribe {
	public:
		SubscribeUpdateOpenTradeOnRoot(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_UPDATEOPENTRADEONROOT_NAME);
		virtual ~SubscribeUpdateOpenTradeOnRoot() = default;

		SubscribeUpdateOpenTradeOnRoot(const SubscribeUpdateOpenTradeOnRoot& r) =delete ;
		SubscribeUpdateOpenTradeOnRoot& operator=(const SubscribeUpdateOpenTradeOnRoot& r) = delete;
		SubscribeUpdateOpenTradeOnRoot(SubscribeUpdateOpenTradeOnRoot&& r) =delete ;
		SubscribeUpdateOpenTradeOnRoot& operator=(SubscribeUpdateTraderOnRoot&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeUpdateOpenTradeOnRoot end*/

/*SubscribeUpdateClosedTradeOnRoot begin*/
class SubscribeUpdateClosedTradeOnRoot : public Subscribe {
	public:
		SubscribeUpdateClosedTradeOnRoot(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_UPDATECLOSEDTRADEONROOT_NAME);
		virtual ~SubscribeUpdateClosedTradeOnRoot() = default;

		SubscribeUpdateClosedTradeOnRoot(const SubscribeUpdateClosedTradeOnRoot& r) =delete ;
		SubscribeUpdateClosedTradeOnRoot& operator=(const SubscribeUpdateClosedTradeOnRoot& r) = delete;
		SubscribeUpdateClosedTradeOnRoot(SubscribeUpdateClosedTradeOnRoot&& r) =delete ;
		SubscribeUpdateClosedTradeOnRoot& operator=(SubscribeUpdateClosedTradeOnRoot&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeUpdateClosedTradeOnRoot end*/

//for RMX
/*SubscribeGetPositionsAnsX begin*/
class	SubscribeGetPositionsAnsX : public Subscribe{
		public:
		SubscribeGetPositionsAnsX(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_GETPOSITIONSANSX_NAME);
		virtual ~SubscribeGetPositionsAnsX() = default;

		SubscribeGetPositionsAnsX(const SubscribeGetPositionsAnsX& r) =delete ;
		SubscribeGetPositionsAnsX& operator=(const SubscribeGetPositionsAnsX& r) = delete;
		SubscribeGetPositionsAnsX(SubscribeGetPositionsAnsX&& r) =delete ;
		SubscribeGetPositionsAnsX& operator=(SubscribeGetPositionsAnsX&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;
};
/*SubscribeGetPositionsAnsX end*/

/*SubscribeRMUpdateTraderBalanceX begin*/
class	SubscribeRMUpdateTraderBalanceX : public Subscribe{
		public:
		SubscribeRMUpdateTraderBalanceX(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_RMUPDATETRADERBALANCEX_NAME);
		virtual ~SubscribeRMUpdateTraderBalanceX() = default;

		SubscribeRMUpdateTraderBalanceX(const SubscribeRMUpdateTraderBalanceX& r) =delete ;
		SubscribeRMUpdateTraderBalanceX& operator=(const SubscribeRMUpdateTraderBalanceX& r) = delete;
		SubscribeRMUpdateTraderBalanceX(SubscribeRMUpdateTraderBalanceX&& r) =delete ;
		SubscribeRMUpdateTraderBalanceX& operator=(SubscribeRMUpdateTraderBalanceX&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;
};

/*SubscribeRMUpdateTraderBalanceX end*/

/*SubscribeUpdatePositionsAnsX begin*/
class	SubscribeUpdatePositionsAnsX : public Subscribe{
		public:
		SubscribeUpdatePositionsAnsX(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_UPDATEPOSITIONSANSX_NAME);
		virtual ~SubscribeUpdatePositionsAnsX() = default;

		SubscribeUpdatePositionsAnsX(const SubscribeUpdatePositionsAnsX& r) =delete ;
		SubscribeUpdatePositionsAnsX& operator=(const SubscribeUpdatePositionsAnsX& r) = delete;
		SubscribeUpdatePositionsAnsX(SubscribeUpdatePositionsAnsX&& r) =delete ;
		SubscribeUpdatePositionsAnsX& operator=(SubscribeUpdatePositionsAnsX&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;
};
/*SubscribeUpdatePositionsAnsX end*/


/*SubscribeOrderChangeBalance begin*/
class	SubscribeOrderChangeBalance : public Subscribe{
		public:
		SubscribeOrderChangeBalance(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_ORDERCHANGEBALANCE_NAME);
		virtual ~SubscribeOrderChangeBalance() = default;

		SubscribeOrderChangeBalance(const SubscribeOrderChangeBalance& r) =delete ;
		SubscribeOrderChangeBalance& operator=(const SubscribeOrderChangeBalance& r) = delete;
		SubscribeOrderChangeBalance(SubscribeOrderChangeBalance&& r) =delete ;
		SubscribeOrderChangeBalance& operator=(SubscribeOrderChangeBalance&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;
};
/*SubscribeOrderChangeBalance end*/


//For Admin
 /*SubscribeAdminLoginAns begin*/
class SubscribeAdminLoginAns : public Subscribe {
	public:
		SubscribeAdminLoginAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_ADMINLOGINANS_NAME);
		virtual ~SubscribeAdminLoginAns() = default;

		SubscribeAdminLoginAns(const SubscribeAdminLoginAns& r) =delete ;
		SubscribeAdminLoginAns& operator=(const SubscribeAdminLoginAns& r) = delete;
		SubscribeAdminLoginAns(SubscribeAdminLoginAns&& r) =delete ;
		SubscribeAdminLoginAns& operator=(SubscribeAdminLoginAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeAdminLoginAns end*/

/*SubscribeAdminLogoutAns begin*/
class SubscribeAdminLogoutAns : public Subscribe {
	public:
		SubscribeAdminLogoutAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_ADMINLOGOUTANS_NAME);
		virtual ~SubscribeAdminLogoutAns() = default;

		SubscribeAdminLogoutAns(const SubscribeAdminLogoutAns& r) =delete ;
		SubscribeAdminLogoutAns& operator=(const SubscribeAdminLogoutAns& r) = delete;
		SubscribeAdminLogoutAns(SubscribeAdminLogoutAns&& r) =delete ;
		SubscribeAdminLogoutAns& operator=(SubscribeAdminLogoutAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeAdminLogoutAns end*/

/*SubscribeAdminForceLogoutAns begin*/
class SubscribeAdminForceLogoutAns : public Subscribe {
	public:
		SubscribeAdminForceLogoutAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_ADMINFORCELOGOUT_NAME);
		virtual ~SubscribeAdminForceLogoutAns() = default;

		SubscribeAdminForceLogoutAns(const SubscribeAdminForceLogoutAns& r) =delete ;
		SubscribeAdminForceLogoutAns& operator=(const SubscribeAdminForceLogoutAns& r) = delete;
		SubscribeAdminForceLogoutAns(SubscribeAdminForceLogoutAns&& r) =delete ;
		SubscribeAdminForceLogoutAns& operator=(SubscribeAdminForceLogoutAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeAdminForceLogoutAns end*/

/*SubscribeAdminChangePasswordAns begin*/
class SubscribeAdminChangePasswordAns : public Subscribe {
	public:
		SubscribeAdminChangePasswordAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_ADMINCHANGEPASSWORDANS_NAME);
		virtual ~SubscribeAdminChangePasswordAns() = default;

		SubscribeAdminChangePasswordAns(const SubscribeAdminChangePasswordAns& r) =delete ;
		SubscribeAdminChangePasswordAns& operator=(const SubscribeAdminChangePasswordAns& r) = delete;
		SubscribeAdminChangePasswordAns(SubscribeAdminChangePasswordAns&& r) =delete ;
		SubscribeAdminChangePasswordAns& operator=(SubscribeAdminChangePasswordAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeAdminChangePasswordAns end*/

/*SubscribeCreateManagerAns begin*/
class SubscribeCreateManagerAns : public Subscribe {
	public:
		SubscribeCreateManagerAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_CREATEMANAGERANS_NAME);
		virtual ~SubscribeCreateManagerAns() = default;

		SubscribeCreateManagerAns(const SubscribeCreateManagerAns& r) =delete ;
		SubscribeCreateManagerAns& operator=(const SubscribeCreateManagerAns& r) = delete;
		SubscribeCreateManagerAns(SubscribeCreateManagerAns&& r) =delete ;
		SubscribeCreateManagerAns& operator=(SubscribeCreateManagerAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeCreateManagerAns end*/

/*SubscribeUpdateManagerAns begin*/
class SubscribeUpdateManagerAns : public Subscribe {
	public:
		SubscribeUpdateManagerAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_UPDATEMANAGERANS_NAME);
		virtual ~SubscribeUpdateManagerAns() = default;

		SubscribeUpdateManagerAns(const SubscribeUpdateManagerAns& r) =delete ;
		SubscribeUpdateManagerAns& operator=(const SubscribeUpdateManagerAns& r) = delete;
		SubscribeUpdateManagerAns(SubscribeUpdateManagerAns&& r) =delete ;
		SubscribeUpdateManagerAns& operator=(SubscribeUpdateManagerAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeUpdateManagerAns end*/


/*SubscribeGetAdminsAns begin*/
class SubscribeGetAdminsAns : public Subscribe {
	public:
		SubscribeGetAdminsAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_GETADMINSANS_NAME);
		virtual ~SubscribeGetAdminsAns() = default;

		SubscribeGetAdminsAns(const SubscribeGetAdminsAns& r) =delete ;
		SubscribeGetAdminsAns& operator=(const SubscribeGetAdminsAns& r) = delete;
		SubscribeGetAdminsAns(SubscribeGetAdminsAns&& r) =delete ;
		SubscribeGetAdminsAns& operator=(SubscribeGetAdminsAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeGetAdminsAns end*/

/*SubscribeGetManagersAns begin*/
class SubscribeGetManagersAns : public Subscribe {
	public:
		SubscribeGetManagersAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_GETMANAGERSANS_NAME);
		virtual ~SubscribeGetManagersAns() = default;

		SubscribeGetManagersAns(const SubscribeGetManagersAns& r) =delete ;
		SubscribeGetManagersAns& operator=(const SubscribeGetManagersAns& r) = delete;
		SubscribeGetManagersAns(SubscribeGetManagersAns&& r) =delete ;
		SubscribeGetManagersAns& operator=(SubscribeGetManagersAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeGetManagersAns end*/

/*SubscribeDeleteManagerAns begin*/
class SubscribeDeleteManagerAns : public Subscribe {
	public:
		SubscribeDeleteManagerAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_DELETEMANAGERANS_NAME);
		virtual ~SubscribeDeleteManagerAns() = default;

		SubscribeDeleteManagerAns(const SubscribeDeleteManagerAns& r) =delete ;
		SubscribeDeleteManagerAns& operator=(const SubscribeDeleteManagerAns& r) = delete;
		SubscribeDeleteManagerAns(SubscribeDeleteManagerAns&& r) =delete ;
		SubscribeDeleteManagerAns& operator=(SubscribeDeleteManagerAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeDeleteManagerAns end*/

/*SubscribeUpdateManagerRightAns begin*/
class SubscribeUpdateManagerRightAns : public Subscribe {
	public:
		SubscribeUpdateManagerRightAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_UPDATEMANAGERRIGHTANS_NAME);
		virtual ~SubscribeUpdateManagerRightAns() = default;

		SubscribeUpdateManagerRightAns(const SubscribeUpdateManagerRightAns& r) =delete ;
		SubscribeUpdateManagerRightAns& operator=(const SubscribeUpdateManagerRightAns& r) = delete;
		SubscribeUpdateManagerRightAns(SubscribeUpdateManagerRightAns&& r) =delete ;
		SubscribeUpdateManagerRightAns& operator=(SubscribeUpdateManagerRightAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeUpdateManagerRightAns end*/

/*SubscribeGetManagerRightAns begin*/
class SubscribeGetManagerRightAns : public Subscribe {
	public:
		SubscribeGetManagerRightAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_GETMANAGERRIGHTANS_NAME);
		virtual ~SubscribeGetManagerRightAns() = default;

		SubscribeGetManagerRightAns(const SubscribeGetManagerRightAns& r) =delete ;
		SubscribeGetManagerRightAns& operator=(const SubscribeGetManagerRightAns& r) = delete;
		SubscribeGetManagerRightAns(SubscribeGetManagerRightAns&& r) =delete ;
		SubscribeGetManagerRightAns& operator=(SubscribeGetManagerRightAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeGetManagerRightAns end*/

/*SubscribeEnabledIsableManagerAns begin*/

class SubscribeEnabledIsableManagerAns : public Subscribe {
	public:
		SubscribeEnabledIsableManagerAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_ENABLEDISABLEMANAGERANS_NAME);
		virtual ~SubscribeEnabledIsableManagerAns() = default;

		SubscribeEnabledIsableManagerAns(const SubscribeEnabledIsableManagerAns& r) =delete ;
		SubscribeEnabledIsableManagerAns& operator=(const SubscribeEnabledIsableManagerAns& r) = delete;
		SubscribeEnabledIsableManagerAns(SubscribeEnabledIsableManagerAns&& r) =delete ;
		SubscribeEnabledIsableManagerAns& operator=(SubscribeEnabledIsableManagerAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeEnabledIsableManagerAns end*/

/*SubscribeDeleteAdminAns begin*/

class SubscribeDeleteAdminAns : public Subscribe {
	public:
		SubscribeDeleteAdminAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_DELETEADMINANS_NAME);
		virtual ~SubscribeDeleteAdminAns() = default;

		SubscribeDeleteAdminAns(const SubscribeDeleteAdminAns& r) =delete ;
		SubscribeDeleteAdminAns& operator=(const SubscribeDeleteAdminAns& r) = delete;
		SubscribeDeleteAdminAns(SubscribeDeleteAdminAns&& r) =delete ;
		SubscribeDeleteAdminAns& operator=(SubscribeDeleteAdminAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};

/*SubscribeDeleteAdminAns end*/

/*SubscribeCreateAdminAns begin*/
class SubscribeCreateAdminAns : public Subscribe {
	public:
		SubscribeCreateAdminAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_CREATEADMINANS_NAME);
		virtual ~SubscribeCreateAdminAns() = default;

		SubscribeCreateAdminAns(const SubscribeCreateAdminAns& r) =delete ;
		SubscribeCreateAdminAns& operator=(const SubscribeCreateAdminAns& r) = delete;
		SubscribeCreateAdminAns(SubscribeCreateAdminAns&& r) =delete ;
		SubscribeCreateAdminAns& operator=(SubscribeCreateAdminAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeCreateAdminAns end*/

/*SubscribeUpdateAdminAns begin*/
class SubscribeUpdateAdminAns : public Subscribe {
	public:
		SubscribeUpdateAdminAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_UPDATEADMINANS_NAME);
		virtual ~SubscribeUpdateAdminAns() = default;

		SubscribeUpdateAdminAns(const SubscribeUpdateAdminAns& r) =delete ;
		SubscribeUpdateAdminAns& operator=(const SubscribeUpdateAdminAns& r) = delete;
		SubscribeUpdateAdminAns(SubscribeUpdateAdminAns&& r) =delete ;
		SubscribeUpdateAdminAns& operator=(SubscribeUpdateAdminAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeUpdateAdminAns end*/


//for account
class SubscribeCreateAccountAns : public Subscribe {
	public:
		SubscribeCreateAccountAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_CREATEACCOUNTANS_NAME);
		virtual ~SubscribeCreateAccountAns() = default;

		SubscribeCreateAccountAns(const SubscribeCreateAccountAns& r) = delete;
		SubscribeCreateAccountAns& operator=(const SubscribeCreateAccountAns& r) = delete;
		SubscribeCreateAccountAns(const SubscribeCreateAccountAns&& r) = delete;
		SubscribeCreateAccountAns& operator=(const SubscribeCreateAccountAns&& r) = delete; 

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;
};

class SubscribeUpdateAccountAns : public Subscribe {
	public:
		SubscribeUpdateAccountAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_UPDATEACCOUNTANS_NAME);
		virtual ~SubscribeUpdateAccountAns() = default;

		SubscribeUpdateAccountAns(const SubscribeUpdateAccountAns& r) = delete;
		SubscribeUpdateAccountAns& operator=(const SubscribeUpdateAccountAns& r) = delete;
		SubscribeUpdateAccountAns(const SubscribeUpdateAccountAns&& r) = delete;
		SubscribeUpdateAccountAns& operator=(const SubscribeUpdateAccountAns&& r) = delete; 

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;
};

class SubscribeGetAllAccountsAns : public Subscribe {
	public:
		SubscribeGetAllAccountsAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_GETALLACCOUNTSANS_NAME);
		virtual ~SubscribeGetAllAccountsAns() = default;

		SubscribeGetAllAccountsAns(const SubscribeGetAllAccountsAns& r) = delete;
		SubscribeGetAllAccountsAns& operator=(const SubscribeGetAllAccountsAns& r) = delete;
		SubscribeGetAllAccountsAns(const SubscribeGetAllAccountsAns&& r) = delete;
		SubscribeGetAllAccountsAns& operator=(const SubscribeGetAllAccountsAns&& r) = delete; 

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;
};

class SubscribeGetAccountsByAccountIdAns : public Subscribe {
	public:
		SubscribeGetAccountsByAccountIdAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_GETACCOUNTSBYACCOUNTIDANS_NAME);
		virtual ~SubscribeGetAccountsByAccountIdAns() = default;

		SubscribeGetAccountsByAccountIdAns(const SubscribeGetAccountsByAccountIdAns& r) = delete;
		SubscribeGetAccountsByAccountIdAns& operator=(const SubscribeGetAccountsByAccountIdAns& r) = delete;
		SubscribeGetAccountsByAccountIdAns(const SubscribeGetAccountsByAccountIdAns&& r) = delete;
		SubscribeGetAccountsByAccountIdAns& operator=(const SubscribeGetAccountsByAccountIdAns&& r) = delete; 

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;
};

//for group
/*SubscribeCreateGroupAns begin*/
class SubscribeCreateGroupAns : public Subscribe{
	public: 
		SubscribeCreateGroupAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_CREATEGROUPANS_NAME);
		virtual ~SubscribeCreateGroupAns() = default;

		SubscribeCreateGroupAns(const SubscribeCreateGroupAns& r) = delete;
		SubscribeCreateGroupAns& operator=(const SubscribeCreateGroupAns& r) = delete;
		SubscribeCreateGroupAns(const SubscribeCreateGroupAns&& r) = delete;
		SubscribeCreateGroupAns& operator=(const SubscribeCreateGroupAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeCreateGroupAns end*/

/*SubscribeUpdateGroupAns begin*/	
class SubscribeUpdateGroupAns : public Subscribe{
	public: 
		SubscribeUpdateGroupAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_UPDATEGROUPANS_NAME);
		virtual ~SubscribeUpdateGroupAns() = default;

		SubscribeUpdateGroupAns(const SubscribeUpdateGroupAns& r) = delete;
		SubscribeUpdateGroupAns& operator=(const SubscribeUpdateGroupAns& r) = delete;
		SubscribeUpdateGroupAns(const SubscribeUpdateGroupAns&& r) = delete;
		SubscribeUpdateGroupAns& operator=(const SubscribeUpdateGroupAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeUpdateGroupAns end*/

/*SubscribeGetGroupdeTailsByIdAns begin*/	
class SubscribeGetGroupdeTailsByIdAns : public Subscribe{
	public: 
		SubscribeGetGroupdeTailsByIdAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_GETGROUPDETAILSBYIDANS_NAME);
		virtual ~SubscribeGetGroupdeTailsByIdAns() = default;

		SubscribeGetGroupdeTailsByIdAns(const SubscribeGetGroupdeTailsByIdAns& r) = delete;
		SubscribeGetGroupdeTailsByIdAns& operator=(const SubscribeGetGroupdeTailsByIdAns& r) = delete;
		SubscribeGetGroupdeTailsByIdAns(const SubscribeGetGroupdeTailsByIdAns&& r) = delete;
		SubscribeGetGroupdeTailsByIdAns& operator=(const SubscribeGetGroupdeTailsByIdAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeGetGroupdeTailsByIdAns end*/

/*SubscribeGetAllGroupForGateWayAns begin*/	
class SubscribeGetAllGroupForGateWayAns : public Subscribe{
	public: 
		SubscribeGetAllGroupForGateWayAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_GETALLGROUPFORGATEWAYANS_NAME);
		virtual ~SubscribeGetAllGroupForGateWayAns() = default;

		SubscribeGetAllGroupForGateWayAns(const SubscribeGetAllGroupForGateWayAns& r) = delete;
		SubscribeGetAllGroupForGateWayAns& operator=(const SubscribeGetAllGroupForGateWayAns& r) = delete;
		SubscribeGetAllGroupForGateWayAns(const SubscribeGetAllGroupForGateWayAns&& r) = delete;
		SubscribeGetAllGroupForGateWayAns& operator=(const SubscribeGetAllGroupForGateWayAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};

/*SubscribeGetAllGroupForGateWayAns end*/

/*SubscribeGetAllGroupAns begin*/	
class SubscribeGetAllGroupAns : public Subscribe{
	public: 
		SubscribeGetAllGroupAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_GETALLGROUPANS_NAME);
		virtual ~SubscribeGetAllGroupAns() = default;

		SubscribeGetAllGroupAns(const SubscribeGetAllGroupAns& r) = delete;
		SubscribeGetAllGroupAns& operator=(const SubscribeGetAllGroupAns& r) = delete;
		SubscribeGetAllGroupAns(const SubscribeGetAllGroupAns&& r) = delete;
		SubscribeGetAllGroupAns& operator=(const SubscribeGetAllGroupAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};

/*SubscribeGetAllGroupAns end*/

/*SubscribeDeleteGroupAns begin*/	
class SubscribeDeleteGroupAns : public Subscribe{
	public: 
		SubscribeDeleteGroupAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_DELETEGROUPANS_NAME);
		virtual ~SubscribeDeleteGroupAns() = default;

		SubscribeDeleteGroupAns(const SubscribeDeleteGroupAns& r) = delete;
		SubscribeDeleteGroupAns& operator=(const SubscribeDeleteGroupAns& r) = delete;
		SubscribeDeleteGroupAns(const SubscribeDeleteGroupAns&& r) = delete;
		SubscribeDeleteGroupAns& operator=(const SubscribeDeleteGroupAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeDeleteGroupAns end*/

/*SubscribeUpdateGroupSecurityAns begin*/	
class SubscribeUpdateGroupSecurityAns : public Subscribe{
	public: 
		SubscribeUpdateGroupSecurityAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_UPDATEGROUPSECURITYANS_NAME);
		virtual ~SubscribeUpdateGroupSecurityAns() = default;

		SubscribeUpdateGroupSecurityAns(const SubscribeUpdateGroupSecurityAns& r) = delete;
		SubscribeUpdateGroupSecurityAns& operator=(const SubscribeUpdateGroupSecurityAns& r) = delete;
		SubscribeUpdateGroupSecurityAns(const SubscribeUpdateGroupSecurityAns&& r) = delete;
		SubscribeUpdateGroupSecurityAns& operator=(const SubscribeUpdateGroupSecurityAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeUpdateGroupSecurityAns end*/

/*SubscribeUpdateGroupReportAns begin*/
class SubscribeUpdateGroupReportAns : public Subscribe{
	public: 
		SubscribeUpdateGroupReportAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_UPDATEGROUPREPORTANS_NAME);
		virtual ~SubscribeUpdateGroupReportAns() = default;

		SubscribeUpdateGroupReportAns(const SubscribeUpdateGroupReportAns& r) = delete;
		SubscribeUpdateGroupReportAns& operator=(const SubscribeUpdateGroupReportAns& r) = delete;
		SubscribeUpdateGroupReportAns(const SubscribeUpdateGroupReportAns&& r) = delete;
		SubscribeUpdateGroupReportAns& operator=(const SubscribeUpdateGroupReportAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;

};
/*SubscribeUpdateGroupReportAns end*/

/*SubscribeSaveManagerAgainstGroupsAns begin*/
class SubscribeSaveManagerAgainstGroupsAns : public Subscribe {
	public:
		SubscribeSaveManagerAgainstGroupsAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_SAVEMANAGERAGAINSTGROUPS_NAME);
		virtual ~SubscribeSaveManagerAgainstGroupsAns() = default;

		SubscribeSaveManagerAgainstGroupsAns(const SubscribeSaveManagerAgainstGroupsAns& r) = delete;
		SubscribeSaveManagerAgainstGroupsAns& operator=(const SubscribeSaveManagerAgainstGroupsAns& r) = delete;
		SubscribeSaveManagerAgainstGroupsAns(const SubscribeSaveManagerAgainstGroupsAns&& r) = delete;
		SubscribeSaveManagerAgainstGroupsAns& operator=(const SubscribeSaveManagerAgainstGroupsAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;	
};

/*SubscribeSaveManagerAgainstGroupsAns end*/

/*SubscribeGetAllGroupsByManagerIdAns begin*/
class SubscribeGetAllGroupsByManagerIdAns : public Subscribe {
	public:
		SubscribeGetAllGroupsByManagerIdAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_GETALLGROUPSBYMANAGERID_NAME);
		virtual ~SubscribeGetAllGroupsByManagerIdAns() = default;

		SubscribeGetAllGroupsByManagerIdAns(const SubscribeGetAllGroupsByManagerIdAns& r) = delete;
		SubscribeGetAllGroupsByManagerIdAns& operator=(const SubscribeGetAllGroupsByManagerIdAns& r) = delete;
		SubscribeGetAllGroupsByManagerIdAns(const SubscribeGetAllGroupsByManagerIdAns&& r) = delete;
		SubscribeGetAllGroupsByManagerIdAns& operator=(const SubscribeGetAllGroupsByManagerIdAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;	
};

/*SubscribeGetAllGroupsByManagerIdAns end*/


//for security and symbol
/*SubscribeCreateSecurityAns begin*/
class SubscribeCreateSecurityAns : public Subscribe {
	public:
		SubscribeCreateSecurityAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_CREATESECURITYANS_NAME);
		virtual ~SubscribeCreateSecurityAns() = default;

		SubscribeCreateSecurityAns(const SubscribeCreateSecurityAns& r) = delete;
		SubscribeCreateSecurityAns& operator=(const SubscribeCreateSecurityAns& r) = delete;
		SubscribeCreateSecurityAns(const SubscribeCreateSecurityAns&& r) = delete;
		SubscribeCreateSecurityAns& operator=(const SubscribeCreateSecurityAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;	
};
/*SubscribeCreateSecurityAns end*/


/*SubscribeUpdateSecurityAns begin*/
class SubscribeUpdateSecurityAns : public Subscribe {
	public:
		SubscribeUpdateSecurityAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_UPDATESECURITY_NAME);
		virtual ~SubscribeUpdateSecurityAns() = default;

		SubscribeUpdateSecurityAns(const SubscribeUpdateSecurityAns& r) = delete;
		SubscribeUpdateSecurityAns& operator=(const SubscribeUpdateSecurityAns& r) = delete;
		SubscribeUpdateSecurityAns(const SubscribeUpdateSecurityAns&& r) = delete;
		SubscribeUpdateSecurityAns& operator=(const SubscribeUpdateSecurityAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;	
};
/*SubscribeUpdateSecurityAns end*/


/*SubscribeGetAllSecurityAns begin*/
class SubscribeGetAllSecurityAns : public Subscribe {
	public:
		SubscribeGetAllSecurityAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_GETALLSECURITYANS_NAME);
		virtual ~SubscribeGetAllSecurityAns() = default;

		SubscribeGetAllSecurityAns(const SubscribeGetAllSecurityAns& r) = delete;
		SubscribeGetAllSecurityAns& operator=(const SubscribeGetAllSecurityAns& r) = delete;
		SubscribeGetAllSecurityAns(const SubscribeGetAllSecurityAns&& r) = delete;
		SubscribeGetAllSecurityAns& operator=(const SubscribeGetAllSecurityAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;	
};
/*SubscribeGetAllSecurityAns end*/

/*SubscribeDeleteSecurityAns begin*/
class SubscribeDeleteSecurityAns : public Subscribe {
	public:
		SubscribeDeleteSecurityAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_DELETESECURITYANS_NAME);
		virtual ~SubscribeDeleteSecurityAns() = default;

		SubscribeDeleteSecurityAns(const SubscribeDeleteSecurityAns& r) = delete;
		SubscribeDeleteSecurityAns& operator=(const SubscribeDeleteSecurityAns& r) = delete;
		SubscribeDeleteSecurityAns(const SubscribeDeleteSecurityAns&& r) = delete;
		SubscribeDeleteSecurityAns& operator=(const SubscribeDeleteSecurityAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;	
};
/*SubscribeDeleteSecurityAns end*/

/*SubscribeCreateSymbolAns begin*/
class SubscribeCreateSymbolAns : public Subscribe {
	public:
		SubscribeCreateSymbolAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_CREATESYMBOLANS_NAME);
		virtual ~SubscribeCreateSymbolAns() = default;

		SubscribeCreateSymbolAns(const SubscribeCreateSymbolAns& r) = delete;
		SubscribeCreateSymbolAns& operator=(const SubscribeCreateSymbolAns& r) = delete;
		SubscribeCreateSymbolAns(const SubscribeCreateSymbolAns&& r) = delete;
		SubscribeCreateSymbolAns& operator=(const SubscribeCreateSymbolAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;	
};
/*SubscribeCreateSymbolAns end*/

/*SubscribeUpdateSymbolAns begin*/
class SubscribeUpdateSymbolAns : public Subscribe {
	public:
		SubscribeUpdateSymbolAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_UPDATESYMBOLANS_NAME);
		virtual ~SubscribeUpdateSymbolAns() = default;

		SubscribeUpdateSymbolAns(const SubscribeUpdateSymbolAns& r) = delete;
		SubscribeUpdateSymbolAns& operator=(const SubscribeUpdateSymbolAns& r) = delete;
		SubscribeUpdateSymbolAns(const SubscribeUpdateSymbolAns&& r) = delete;
		SubscribeUpdateSymbolAns& operator=(const SubscribeUpdateSymbolAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;	
};
/*SubscribeUpdateSymbolAns end*/

/*SubscribeGetAllSymbolAns begin*/
class SubscribeGetAllSymbolAns : public Subscribe {
	public:
		SubscribeGetAllSymbolAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_GETALLSYMBOLANS_NAME);
		virtual ~SubscribeGetAllSymbolAns() = default;

		SubscribeGetAllSymbolAns(const SubscribeGetAllSymbolAns& r) = delete;
		SubscribeGetAllSymbolAns& operator=(const SubscribeGetAllSymbolAns& r) = delete;
		SubscribeGetAllSymbolAns(const SubscribeGetAllSymbolAns&& r) = delete;
		SubscribeGetAllSymbolAns& operator=(const SubscribeGetAllSymbolAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;	
};
/*SubscribeGetAllSymbolAns end*/

/*SubscribeGetSymbolByIdAns begin*/
class SubscribeGetSymbolByIdAns : public Subscribe {
	public:
		SubscribeGetSymbolByIdAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_GETSYMBOLBYIDANS_NAME);
		virtual ~SubscribeGetSymbolByIdAns() = default;

		SubscribeGetSymbolByIdAns(const SubscribeGetSymbolByIdAns& r) = delete;
		SubscribeGetSymbolByIdAns& operator=(const SubscribeGetSymbolByIdAns& r) = delete;
		SubscribeGetSymbolByIdAns(const SubscribeGetSymbolByIdAns&& r) = delete;
		SubscribeGetSymbolByIdAns& operator=(const SubscribeGetSymbolByIdAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;	
};
/*SubscribeGetSymbolByIdAns end*/

/*SubscribeDeleteSymbolAns begin*/
class SubscribeDeleteSymbolAns : public Subscribe {
	public:
		SubscribeDeleteSymbolAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_DELETESYMBOLANS_NAME);
		virtual ~SubscribeDeleteSymbolAns() = default;

		SubscribeDeleteSymbolAns(const SubscribeDeleteSymbolAns& r) = delete;
		SubscribeDeleteSymbolAns& operator=(const SubscribeDeleteSymbolAns& r) = delete;
		SubscribeDeleteSymbolAns(const SubscribeDeleteSymbolAns&& r) = delete;
		SubscribeDeleteSymbolAns& operator=(const SubscribeDeleteSymbolAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;	
};
/*SubscribeDeleteSymbolAns end*/


/*SubscribeSynchDataAns begin*/
class SubscribeSynchDataAns : public Subscribe {
	public:
		SubscribeSynchDataAns(Consumer* consumer, const char* eventName, const char* groupName=SOLUTIONGATEWAY_SUBSCRIBE_SYNCHDATAANS_NAME);
		virtual ~SubscribeSynchDataAns() = default;

		SubscribeSynchDataAns(const SubscribeSynchDataAns& r) = delete;
		SubscribeSynchDataAns& operator=(const SubscribeSynchDataAns& r) = delete;
		SubscribeSynchDataAns(const SubscribeSynchDataAns&& r) = delete;
		SubscribeSynchDataAns& operator=(const SubscribeSynchDataAns&& r) = delete;

	public:
		virtual void Done(std::shared_ptr<void> event, unsigned int eventLen) override;	
};
/*SubscribeSynchDataAns end*/

#endif

