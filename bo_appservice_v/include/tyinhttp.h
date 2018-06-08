#ifndef _TINYHTTP_H_
#define _TINYHTTP_H_

#include <unistd.h>  
#include <ctype.h>  
#include <strings.h>  
#include <string.h>  
#include <sys/stat.h>  
#include <pthread.h>  
#include <sys/wait.h>  
#include "log.h"
#include "const.h"
#include "redis.h"
#include "kerner.h"
#include "msgopr.h"
#include "common.h"
#include "tyinhttp.h"
#include "./json/writer.h"
#include "./json/features.h"
#include "./json/forwards.h"
#include "./json/json.h"
#include "./curl/curl.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sys/time.h>
#include <sstream>
#include <map>
#include <list>



#define ISspace(x) isspace((int)(x))
#define CONSTR(s1,s2) (s1)+=(s2);

#define CRMHOST "52.76.247.145"
#define USERAGENT "Jxolsys/1.0\r\n"
#define CONNECTION "Keep-Alive\r\n"

#define SERVER_STRING "Server: httpd/0.1.0\r\n"  

typedef struct stHttpMap
{
	//请求头
	char szMsgId[16];
	char szType[3];

	char szReqHost[64];
	char szRspHost[64];

	char szReqUserAgent[128];
	char szRspUserAgent[128];

	char szReqAccept[128];
	char szRspAccept[128];

	char szReqConnection[128];
	char szRspConnection[128];

	char szReqAcceptCharset[128];
	char szRspAcceptCharset[128];

	char szReqAcceptEncoding[25];
	char szRspAcceptEncoding[25];

	char szReqAcceptLanguage[30];
	char szRspAcceptLanguage[30];

	//请求体
	char szReqCountryId[10];
	char szRspCountryId[10];

	char szReqEmail[50];
	char szRspEmail[50];

	char szReqUserName[60];
	char szRspUserName[60];

	char szReqLeverage[10];
	char szRspLeverage[10];

	char szReqPassportId[20];
	char szRspPassportId[20];

	char szReqPassword[20];
	char szRspPassword[20];

	char szReqPhone[12];
	char szRspPhone[12];

	char szReqIpAddress[16];
	char szRspIpAddress[16];

	//IOS&&AND key
	char MerchantKeyIOS[28];
	char MerchantKeyAND[28];

}HttpMap,*pHttpMap;


class CTinyHttp
{
	private:
		string		Requst;
		int 		m_Type;

	public:
		HttpMap		m_HttpMap;
	

	public:
		CTinyHttp();
		~CTinyHttp();

		//解析行数据
		int GetLine(const char* szSrcBuf, char* szDestBuf, int size);  

		//解析错误
		void UnimpleMented(string& strUnimpleBuf);

		void NotFound(string& strBuf);  

		//解包
		void UnpackHttp(char* buf);

		//组包
		int PackHttp(pHttpMap buf, char* dest, int method);

		//组请求头
		void MakeReqHead(int flag);

		int GetType(char* buf);

		//组应答头
		void MakeRspHead(int flag);

		int Equal(char a,char b);

		char* FindSubstr(char* s, char* sub);

		void GetTagVal(char* buf, char* tag, char* dest);

		void GetTagVal(char* buf, char* tag, char* dest, char end);

		void GetTagVal(char* buf, char* tag, char* dest,char end,size_t count);

		int Str2Http(char* buf, char* dest, int flag = 0);

		int CvtApp2Out(char* buf, char* dest, int flag = 0);

		int Con2Str(char* buf, char * split, char* constr);

		void MakeService2AppRspErr(char* dest, string strCode, string strReason, string strAppVer="");

		void MakeService2AppRspErr(char* dest, string strCode);

		void MakeService2AppRspHead(char* szHeadRsp, 
				        int nFla,int nLen, char* szHttpCode, char* szReason);

		void MakeCrm2AppRspHead(char* src, char* szHeadRsp, int nFlag, int nLen);

		void MakeService2AppRspBody(char* szBodyRsp, int nFlag);

		 int GetMT4AuthRspVal(char* src, string& strToken, string& strTokenType);

		 int GetJsonOneTagVal(string strSrc, string strTag, string& strVal);

		 int MakeMT4AuthReq(string mt4acct, string mt4pwd, char* dest, string bourl);
};

#endif

