#ifndef _COMMON_H_
#define _COMMON_H_ 
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <sys/time.h>
#include <cstdarg>
#include <sys/stat.h>
#include <errno.h>
#include <sstream>


#include "log.h"
#include "const.h"
#include "redis.h"
#include "kerner.h"
#include "msgopr.h"
#include "memopr.h"
#include "tyinhttp.h"
#include "./json/writer.h"
#include "./json/features.h"
#include "./json/forwards.h"
#include "./json/json.h"
#include "./curl/curl.h"


using namespace std;

typedef struct stMsgHead
{
	char szMsgType[MSG_TYPE_LEN];
	char szMAC[MAC_LEN];
	char szCrmUser[CRM_USER_LEN];
	char szCmPwd[CRM_PWD_LEN];
	char szMt4User[MT4_USER_LEN];
	char szMt4Pwd[MT4_PWD_LEN];
	char szRoute[ROUTE_LEN];
	char szSrcId[SORCE_ID_LEN];
	char szShmKey[SHM_KEY_LEN];
}MsgHead,*pMsgHead;

typedef struct _crm_api
{
	std::string strCompany;
	std::string strUrlCrmHy;
	std::string strUrlCrmMy;
	std::string strUrlBoHy;
	std::string strUrlBoMy;
	std::string strUrlSerProEn;
	std::string strUrlSerProCn;
	std::string strServerUrl;
	std::string strServerPort;

	std::string strIosVerUrl;
	std::string strAppIosVersion;
	std::string strAppMidIosVersion;
	std::string strAppIosRevieweverUrl;
	std::string strAppMidAnrVersion;
	std::string strMustUpdateIosVersion;
	std::string strAppAdrVersion;
	std::string strAppAdrRevieweverUrl;
	std::string strMustUpdateAdrVersion;

	std::string strSupportWeb;

	std::string strAppIosSkin;
	std::string strMustUpdateIosSkin;
	std::string strAppAdrSkin;
	std::string strMustUpdateAdrSkin;

	std::string strAppIosSymbol;
	std::string strMustUpdateIosSymbol;
	std::string strAppAdrSymbol;
	std::string strMustUpdateAdrSymbol;

	std::string strIosUpdateVerUrl;
	std::string strIosUpdateSkinUrl;
	std::string strIosUpdateSymbolUrl;

	std::string strAdrUpdateVerUrl;
	std::string strAdrUpdateSkinUrl;
	std::string strAdrUpdateSymbolUrl;

	std::string strRegistTmpFile;	// ×¢²áÄ£°åÎÄ¼þ
	std::string strVideoUrl;			// ÊÓÆµ
	std::string strMerchantKey;
	std::string strAppIp;
	std::string strDoMain;
	std::string strIOS_Merchantkey;
	std::string strIOS_AppIp;
	std::string strIOS_DoMain;

	std::string strProUser;
	std::string strMinTransAmount_zh;
	std::string strMinTransAmount_en;

	std::string strFedBackUrl;
	std::string strMaintenanceflag;
	std::string strMaintenancecode;
	std::string strMaintenancedes_cn;
	std::string strMaintenancedes_en;

	std::string strIosMaintenanceflag;
	std::string strIosMaintenancecode;
	std::string strIosMaintenancedes_cn;
	std::string strIosMaintenancedes_en;

}stCrmApi,*pstCrmApi;

typedef struct 
{
	time_t sTime;
	time_t eTime;
	int    count;
	int    unit;
}stHistory, *pstHistory;


extern time_t convert_str_to_tm(char * str_time);

extern int gethis(std::string sTime, std::string eTime,std::string timeframe, stHistory& his);

extern void addtime(time_t srctm, long unit, char* destime, char* szTimeZone);

extern long get_tm_usec();
extern long get_tm_msec();

extern long get_tm_sec();

extern time_t NowTime();
extern time_t str_to_time_t(const string& ATime, const string& AFormat);

extern std::string int2str(int n);

extern std::string ltos(long l);

extern void rand_str(std::string& str,const int len);

extern void CreateToken(char* token, char* szUser);

extern int RTrim(char* str);

extern int LTrim(char* str);

extern int LRTrim(char* str);

extern std::string& strLtrim(std::string &s);

extern std::string& strRtrim(std::string &s);

void strrpl(char* pDstOut, char* pSrcIn, const char* pSrcRpl, const char* pDstRpl);

int Base64_Decode(char *pDest, const char *pSrc, int srclen);
int Base64_Encode(char *pDest, const char *pSrc, int srclen);
int Base64Dec(char *buf,char*text,int size);

BYTE Decode_GetByte(char c); 
char Encode_GetChar(BYTE num);


extern int GetCrmConfig(stCrmApi& Crmapi);

extern int CvtBigToSml(char* str, int len);

extern char * eatSpace(char *str);


extern int getApiByReq(string strSrcIn, string& strDstOut, const char* pEnd, int nModulId);

extern void formatUrl(string &s);
extern void formatConfig(stCrmApi &conf);
extern void DelSrcId(char* s,char* szSrcId);

#endif
