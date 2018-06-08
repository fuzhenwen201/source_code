#ifndef _WORK_H_
#define _WORK_H_

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
#include <list> 
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sstream>

using namespace std;

typedef struct
{
	pid_t	pid;
	int		nSrvSeqId;
	int		nCntArgv;
	string	strArgv[20];
	char	*argv[20];
	string  strPath;
	int		nStartCnt;
	int		nCutTimes;
	long	lnLastTime;
	long	nunitTime;
	int		ntimes;
	int		nmaxtimes;
	int		nprocnum;
}Pci, *pPci;

//启动进程
pid_t lStartProcess(string svrName, Pci& pci);
void vStartAllProcess();
void vResetStartCnt();
void vStopAllProcess();
void vHandleExit(int n);
//pPci vFindPci(int pid);
void vHandleChildDeath(int n);
int GetOneTagVal(string strSrc, string strTag, string& strVal);


int m_maxrstnum; //重启次数
//long m_interval;
int m_procnum; //进程数量
int m_argc;

list<Pci> lisproc;

#endif
