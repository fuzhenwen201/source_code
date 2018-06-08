#ifndef _CBUSINESS_H_
#define _CBUSINESS_H_

#if 1
#include "cdispatch.h"
#include "const.h"
#include "redis.h"
#include "common.h"
#endif

#include "redis.h"
#include <unistd.h>
#include <string.h>
#include <stdexcept>
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/asio/read_until.hpp> 

using namespace std;

extern void *businessThread(void *arg); 

int DelMt4Token(char *pKey, CRedis& pRds);

class CBusiness
{
	public:
		typedef int (CBusiness::*busifun)(char* ,  char* , int&, std::string, CRedis& ,char*,stCrmApi&);
		busifun m_Busifun;
		CBusiness(class CDispatch* pDispatch);
		int Registercb(busifun pfun);
		int Registercb( char* src, char* dest, int& nRoute, std::string strToken);
		//int Registercb(char* pApi, busifun& pbusifun);


		//外部接口
		int UserRegisterReqCrm(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );
		int UserRegisterRspApp(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );

		int UserLoginReqCrm(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );
		int UserLoginRspApp(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );  

		int UserhistoryReqCrm(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );
		int UserhistoryRspApp(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );

		int UserAuthReqCrm(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );
		int UserAuthRspApp(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );

		int UserForPwdReqCrm(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );
		int UserForPwdRspApp(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );

		int UserTradehistoryReqCrm(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );
		int UserTradehistoryRspApp(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );

		int UserTmpregisterReqCrm(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );
		int UserTmpregisterRspApp(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );

		int UserArchiveTradeReqCrm(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );
		int UserArchiveTradeRspApp(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );

		int UserWithdrawTemplateReqCrm(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );
		int UserWithdrawReqCrm(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );
		int UserWithdrawRspApp(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );
		int UserWithdrawHistoryReqCrm(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );
		int UserWithdrawHistoryRspApp(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );
		int UserWithdrawDeleteReqCrm(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );

		int UserDepositTemplateReqCrm(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );
		int UserDepositReqCrm(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );
		int UserDepositStatusReqCrm(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );
		int UserDepositUploadReqCrm(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );

		//入金文件上传应答 入金应答 入金状态应答
		int UserDepositRspApp(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );

		int UserGetnameReqCrm(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );
		int UserSetnameReqCrm(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );

		//获取姓名应答 设置姓名应答
		int UserNameRspApp(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );

		int UserProfileDataReqCrm(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );
		int UserProfileDataRspApp(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );

		//内部接口
		int UserArchiveOnline(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );
		int UserUploadlogRspApp(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );
		int UserHeartRspApp(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );
		int UserAgreementRspApp(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );
		int UserawaretradingRspApp(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );
		int UserLogoutRspApp(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );
		int UserAuthenticateRspApp(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );
		int UserCrmTokenRspApp(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );
		int ReloadConfigParam(char* src, char* dest, int& nRoute, std::string strToken,CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig );

		int Do_work(std::string strApi, char* src, char* dest, int& nRoute, std::string strTOken, CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig);

	protected:
		void Init();
		class CDispatch* m_Dispatch;
		std::map<std::string, busifun> m_Apimap;
};

#endif
