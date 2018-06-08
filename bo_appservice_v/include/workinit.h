#ifndef _WORK_INIT_H
#define _WORK_INIT_H

#if 1
#include "cdispatch.h"
#include "cmsghandle.h"
#include "cbusiness.h"
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

//队列
//通讯接收队列
extern CIpcMsg* g_pMsgWorkIpcComRcv;
//通讯发送队列
extern CIpcMsg* g_pMsgWorkIpcComSnd;
//业务处理接收队列
extern CIpcMsg* g_pMsgWorkIpcBusRcv;
//业务处理发送队列
extern CIpcMsg* g_pMsgWorkIpcBusSnd;
//同步队列
extern CIpcMsg* g_pMsgWorkIpcBusSyn;



extern int InitMsg();


//INIT while list
#if 1
typedef struct _user_list_info
{
	int m_use;
	int m_flag;
	std::list<std::string*> m_Userlist;
}userListInfo;

typedef std::map<std::string, userListInfo*>  companyUserListinfo;


class CUserManger
{
	public:
		CUserManger();
		~CUserManger();
		int InitUserList();
		int FindUser(std::string strUser,char* szSrcId);
		companyUserListinfo mapCompUserInfo;
};

extern class CUserManger g_cusermanage;
//extern char gSrcId[SORCE_ID_LEN];
#endif

#endif
