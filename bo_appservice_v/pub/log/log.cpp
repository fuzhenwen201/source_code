#include "../../include/log.h"
#include "../../include/const.h"
 

#if 1
extern "C" int InitLog(const char *szMoudleName, char *seq)
{
    char   *pFilePath = NULL;
    char line[1024] = { 0 };
    if((pFilePath = getenv("LOG_PATH")) == NULL)
    {
        cout << "获取文件配置信息出错!" << endl;
        //printf("获取文件配置信息出错!\n");
        return FAILE;
    }
    snprintf(line,sizeof(line),"%s%s%s.config",pFilePath,szMoudleName,seq);
    SLOG_INIT(line);
}
#endif 

#if 0
static T_Logger tLogger;
pthread_mutex_t loglock;


extern "C" int InitLog(const char *szMoudleName, char *seq)
{
    char   *pFilePath = NULL;
    FILE *fp = NULL;
    char line[1024];

    memset(line, 0x00, sizeof(line));

    if((pFilePath = getenv("LOG_FILE_CFG")) == NULL)
    {
        cout << "获取文件配置信息出错!" << endl;
        //printf("获取文件配置信息出错!\n");
        return FAILE;
    }
    if((fp = fopen(pFilePath, "r")) != NULL )
    {
        while(fgets(line, sizeof(line), fp) != NULL)
        {
            if(line[0] == '#')
            {
                continue;
            }
            //去掉[]
            if( memcmp(line + 1, szMoudleName, strlen(szMoudleName) ) == 0)
            {
                memset(line, 0x00, sizeof(line));
                while((fgets(line, sizeof(line), fp)) != NULL )
                {
                    if(line[0] == '[')
                    {
                        break;
                    }
                    else if(memcmp(line, "PATH", 4) == 0)
                    {
                        //memcpy(tLogger.szFilePath, line+5, strlen(line) - 5);
                        line[strlen(line) - 1 ] = '\0';
                        //sprintf(tLogger.szFileName,"%s%s%s",line + 5, szMoudleName, seq);
                        sprintf(tLogger.szFilePath, "%s%s%s.log", line + 5, szMoudleName, seq);
                    }
                    else if(memcmp(line, "LEVEL", 5) == 0)
                    {
                        tLogger.nLogLevel = atoi(line + 6);
                    }
                    else if(memcmp(line, "SIZE", 4) == 0)
                    {
                        tLogger.nChangeLogSize = atol(line + 5);
                    }
                    memset(line, 0x00, sizeof(line));
                }
            }
        }
    }
    else
    {
        cout << "打开配置文件错误:" << pFilePath << endl;
        return FAILE;
    }

    return SUCESS;
}

extern "C" int WriteLog(const char *szLevel, const char *szFileName, \
                        int nLine, const char *szFunctionName, const char *szFormat, ...)
{
    char szTime[256];
    char szBuf[256];
    va_list varList;
    struct stat tFileStat;
    struct timeval tTimeVal;

    //取时间, 精确到毫秒
    memset(szTime, 0x00, sizeof (szTime));
    memset(szBuf, 0x00, sizeof (szBuf));
    gettimeofday(&tTimeVal, NULL);
    strftime(szBuf, sizeof (szBuf), "TIME:%Y-%m-%d_%H:%M:%S",
             localtime(&tTimeVal.tv_sec));
    snprintf(szTime, sizeof (szTime), "%s.%03ld",
             szBuf, tTimeVal.tv_usec / 1000);

    //检查文件是否存在
    if(NULL == tLogger.fpLogger)
    {
        if(tLogger.szFilePath[0] != '\0')
        {
            if(NULL == (tLogger.fpLogger = fopen(tLogger.szFilePath, "a+")))
            {
                cout << "打开文件出错1" << tLogger.szFilePath << endl;
                return FAILE;
            }
            stat(tLogger.szFilePath, &tFileStat);
            tLogger.filenode = tFileStat.st_ino;
        }
        else
        {
            cout << "打开文件出错2" << tLogger.szFilePath << endl;
            return FAILE;
        }
    }
    else if(stat(tLogger.szFilePath, &tFileStat) != 0)
    {
        if(ENOENT == errno || tLogger.filenode != tFileStat.st_ino)
        {
            pthread_mutex_lock(&loglock);
            if(tLogger.fpLogger != NULL)
                fclose(tLogger.fpLogger);

            if(NULL == (tLogger.fpLogger = fopen(tLogger.szFilePath, "a+")))
            {
                cout << "打开文件出错3" << tLogger.szFilePath << endl;
                return FAILE;
            }
            tLogger.filenode = tFileStat.st_ino;

            pthread_mutex_unlock(&loglock);
        }
    }
    // backup log
    if(tFileStat.st_size  >= tLogger.nChangeLogSize*1024*1024)
    {
        fclose(tLogger.fpLogger);
        tLogger.fpLogger = NULL;
        char format[256] = { 0 };
        char filename[256] = { 0 };
        strftime(format, sizeof (format), "%Y%m%d%H%M",
                 localtime(&tTimeVal.tv_sec));
        sscanf(tLogger.szFilePath, "%[^_.]", filename);
        //printf("%s \t %s\n", filename,format);
        memset(tLogger.szFilePath, 0x00, sizeof(tLogger.szFilePath));
        snprintf(tLogger.szFilePath, sizeof(tLogger.szFilePath), "%s_%s.log", filename, format);
        if(NULL == (tLogger.fpLogger = fopen(tLogger.szFilePath, "a+")))
        {
            cout << "打开文件出错4" << tLogger.szFilePath << endl;
            return FAILE;
        }
    }
 
    //sprintf(szTmp, "[%s][%s][FILE:%s][LINE:%d]\n", szLevel, szTime, szFileName, nLine);
    //日志时间, 文件名, 行号
    fprintf(tLogger.fpLogger, "[%s][%s][FILE:%s][LINE:%d]",
            szLevel, szTime, szFileName, nLine);

    //函数名
    fprintf(tLogger.fpLogger, "%s: ", szFunctionName);

    va_start(varList, szFormat);
    vfprintf(tLogger.fpLogger, szFormat, varList);
    fprintf(tLogger.fpLogger, "\n");
    va_end(varList);
    fflush(tLogger.fpLogger);

    return SUCESS;
}

#endif 
