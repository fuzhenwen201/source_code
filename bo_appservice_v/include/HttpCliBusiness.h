#ifndef _HTTPCLIBUSINESS_H_
#define _HTTPCLIBUSINESS_H_

#include "HttpClient.h"


//extern int GetTagVal(char* buf, char* tag, char* dest, char end);
class HttpCliBusiness
{
	public:
		HttpCliBusiness();
		~HttpCliBusiness();
		void InitHttpClient();
		typedef int (HttpCliBusiness::*pFun)(char*,char*,HttpClient*, string&);
		int RequestHttpCli(char* pParam,char* Url, string& strResponse );

	protected:
		int LoginCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse);
		int AuthCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse);
		int RegisterCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse);
		int ForPwdCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse);
		int UserGetHisCoutCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse);
		int UserGetHisTimeCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse);
		int UserGetTrdhisCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse);
		int UserGetArchtraCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse);
		int UserGetWithdrawCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse);
		int UserGetWithdrawCrmTmp(char* pParam, char* Url,HttpClient *pClient,string& strResponse); 
		int UserWithdrawReq(char* pParam, char* Url,HttpClient *pClient,string& strResponse);
		int UserGetDepostReqCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse);
		int UserDepostStatusReqCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse);
		int UserDepostUploadReqCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse);

		int UserRegisterTmpCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse);
		int UserNameCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse);
		int UserSetNameCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse);
		int UserProfileDataCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse);
	private:
		std::map<std::string,pFun> m_FunMap;
		



};


#endif 