#ifndef _LOG_H_
#define _LOG_H_ 
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <sys/time.h>
#include <cstdarg>
#include <sys/stat.h>
#include <errno.h>
#include "slog.h"
using namespace std;

//日志信息
typedef struct T_Logger_tag
{
	int        nLogLevel;                      //<日志级别, 大于该级别的日志不输出
	char       szFilePath[256];                //<日志文件路径
	//char       szFileName[256];
	FILE       *fpLogger;                      //<日志文件指针
	size_t     nChangeLogSize;                 //<日志文件切换的文件大小(单位MB)
	bool       bIsDailyChange;                 //<是否要日切换
	char       szLastChangeDate[8 + 1];        //<上次切换日志的日期
	unsigned long       filenode;
} T_Logger;

extern "C" int InitLog(const char *szMoudleName, char* seq);
//extern "C" int WriteLog(const char *szLevel, const char *szFileName, \
		int nLine, const char *szFunctionName, const char *szFormat, ...);
#endif
