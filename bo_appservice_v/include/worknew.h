#ifndef _WORK_H_
#define _WORK_H_

#if 1
#include "cdispatch.h"
#include "cmsghandle.h"
#include "cbusiness.h"
#include "workinit.h"
#endif

#include "log.h"
#include "const.h"
#include "redis.h"
#include "kerner.h"
#include "msgopr.h"
#include "memopr.h"
#include "common.h"
#include "memopr_boost.h"
#include "tyinhttp.h"
#include "./json/writer.h"
#include "./json/features.h"
#include "./json/forwards.h"
#include "./json/json.h"
#include "./curl/curl.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <stdlib.h>
#include <sys/time.h>
#include <sstream>
#include <map>
#include <list>
using namespace std;

typedef struct connection_st {
	int sock;
	int index;
	int inuse;
	int roff;
	int woff;
	IpcMsgBuf ipcBuf;
}*connection_t;

typedef struct sockinfo{
	struct epoll_event event;
	time_t ltime;
	int shmid;
}*pconninfo;

extern int UserLoginReqCrm(std::string , char* , char* , int&);
extern int UserLoginRspApp(std::string , char* , char* , int&);
extern long get_tm_sec();
extern long get_tm_usec();
//extern int InitUserList();

extern void strrpl(char* pDstOut, char* pSrcIn, const char* pSrcRpl, const char* pDstRpl);

extern void MakeService2AppRspHead(char* szHeadRsp,
		int nFla,int nLen, char* szHttpCode, char* szReason);



extern CTinyHttp cTinyWrok;
//extern CRedis rds;
extern void CreateToken(char* toke, char* user);
extern pMsgHead msghead;
extern int nHeadLen;

extern long gOnlineTimeOut;
extern long gOnlineDelayTimeOut;
extern long gCrmTokenTimeOut;





//extern stCrmApi CrmConfig;

//extern char gSrcId[SORCE_ID_LEN];

typedef struct _error_desc
{
	std::string keyWords;
	std::string enDesc;
	std::string cnDesc;
}errDesc;

typedef deque<errDesc> keyWordsDeque;
typedef std::map<std::string, keyWordsDeque> interfaceErrorKeyWordsMap;
typedef std::map<std::string, keyWordsDeque>::iterator interfaceErrorKeyWordsIt;

extern interfaceErrorKeyWordsMap mInterfaceErrKWords;

//extern char g_AppToken[APP_TOKEN_LEN+1];
extern char g_ContentType[120];
//extern CIpcMemBoost *g_IpcMemBoost;
//extern char g_CrmToken[CRM_TOKEN_LEN+1];
extern CDispatch* g_pcdispatch;
extern int LoadCrmConfig(CRedis& rds);
extern void interfaceErrKWordsInit(CRedis& rds);
extern void makeErrCnEnDeque(const char *key, const char *en, const char *cn,keyWordsDeque &keyWordsMap);
extern int GetCrmConfig(stCrmApi& Crmapi);
void cpystCrmApi(stCrmApi& srcCrmapi, stCrmApi& dstCrmapi);

//extern stCrmApi CrmConfig;
extern stCrmApi g_CrmApi[MAX_CRM_API_COUNT];

extern interfaceErrorKeyWordsMap mInterfaceErrKWords;

extern int g_LoginKey_SurviTime ;
extern int g_HistoryKey_SurviTime;

#endif
