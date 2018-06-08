#ifndef _HTTPCLIBUSINESS_H_
#define _HTTPCLIBUSINESS_H_

#include "multcurl.h"
#include "AsncHttpClient.h"


 struct HttpResult
 {
 	CIpcMem *m_IpcMem;
 	long m_IpcMem_Len;
 	curl_slist * m_headerlist;
 	CIpcMsg *m_IpcSend;
 	HttpResult();
 	~HttpResult();
 };


//extern int GetTagVal(char* buf, char* tag, char* dest, char end);
class HttpCliBusiness
{
	public:
		HttpCliBusiness();
		~HttpCliBusiness();
		void InitHttpClient();
		typedef int (HttpCliBusiness::*pFun)(char*,char*,AsncHttpClient*, void*);
		int RequestHttpCli(char* pParam,char* Url, void* Response ,MultHttpClient *mult);

	protected:
		int LoginCrm(char* pParam, char* Url,AsncHttpClient *pClient,void* Response);
		int AuthCrm(char* pParam, char* Url,AsncHttpClient *pClient,void* Response);
		int LogoutBO(char* pParam, char* Url, AsncHttpClient *pClient, void* Response);
		int RegisterCrm(char* pParam, char* Url,AsncHttpClient *pClient,void* Response);
		int ForPwdCrm(char* pParam, char* Url,AsncHttpClient *pClient,void* Response);
		int UserGetHisCoutCrm(char* pParam, char* Url,AsncHttpClient *pClient,void* Response);
		int UserGetHisTimeCrm(char* pParam, char* Url,AsncHttpClient *pClient,void* Response);
		int UserGetTrdhisCrm(char* pParam, char* Url,AsncHttpClient *pClient,void* Response);
		int UserGetArchtraCrm(char* pParam, char* Url,AsncHttpClient *pClient,void* Response);
		int UserGetWithdrawCrm(char* pParam, char* Url,AsncHttpClient *pClient,void* Response);
		int UserGetWithdrawCrmTmp(char* pParam, char* Url,AsncHttpClient *pClient,void* Response); 
		int UserWithdrawReq(char* pParam, char* Url,AsncHttpClient *pClient,void* Response);
		int UserGetDepostReqCrm(char* pParam, char* Url,AsncHttpClient *pClient,void* Response);
		int UserDepostStatusReqCrm(char* pParam, char* Url,AsncHttpClient *pClient,void* Response);
		int UserDepostUploadReqCrm(char* pParam, char* Url,AsncHttpClient *pClient,void* Response);

		int UserRegisterTmpCrm(char* pParam, char* Url,AsncHttpClient *pClient,void* Response);
		int UserNameCrm(char* pParam, char* Url,AsncHttpClient *pClient,void* Response);
		int UserSetNameCrm(char* pParam, char* Url,AsncHttpClient *pClient,void* Response);
		int UserProfileDataCrm(char* pParam, char* Url,AsncHttpClient *pClient,void* Response);
	private:
		std::map<std::string,pFun> m_FunMap;
		



};


#endif 