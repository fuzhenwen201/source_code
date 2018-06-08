#include "../../include/HttpCliMultiBusiness.h"
#include <assert.h>
#include "../../include/openssl/crypto.h"
//#include <openssl/crypto.h> 
/* we have this global to let the callback get easy access to it */
//#ifdef USE_OPENSSL  
static pthread_mutex_t *lockarray;
static void lock_callback(int mode, int type, const char *file, int line)  
{  
  (void)file;  
  (void)line;  
  if (mode & CRYPTO_LOCK) {  
    pthread_mutex_lock(&(lockarray[type]));  
  }  
  else {  
    pthread_mutex_unlock(&(lockarray[type]));  
  }  
}  
   
static unsigned long thread_id(void)  
{  
  unsigned long ret;  
   
  ret=(unsigned long)pthread_self();  
  return(ret);  
}  
   
static void init_locks(void)  
{  
  int i;  
   
  lockarray=(pthread_mutex_t *)OPENSSL_malloc(CRYPTO_num_locks() *  
                                            sizeof(pthread_mutex_t));  
  for (i=0; i<CRYPTO_num_locks(); i++) {  
    pthread_mutex_init(&(lockarray[i]),NULL);  
  }  
   
  CRYPTO_set_id_callback((unsigned long (*)())thread_id);  
 // CRYPTO_set_locking_callback((void (*)())lock_callback);  
  CRYPTO_set_locking_callback(lock_callback);
}  
   
static void kill_locks(void)  
{  
  int i;  
   
  CRYPTO_set_locking_callback(NULL);  
  for (i=0; i<CRYPTO_num_locks(); i++)  
    pthread_mutex_destroy(&(lockarray[i]));  
   
  OPENSSL_free(lockarray);  
}  
//#endif  

CIpcMsg *g_IpcSndMsg = NULL;
CIpcMsg *g_IpcSynSndMsg = NULL;
void signal_handler(int sig)
{
    switch (sig)
    {
    case SIGTERM:
    case SIGHUP:
    case SIGQUIT:
    case SIGINT:
    case SIGPIPE:
    LOG_INFO("signal [%d]",sig);
        break;
    }
}
void timeout(int signo)
{
    LOG_INFO("time out");
}

void HttpResponse(void *param)
{
    HttpInfo *response = (HttpInfo *)param;
    HttpResult *result = (HttpResult *)response->m_ret;
    IpcMsgBuf buf;
    memset(buf.mtext, 0x00, sizeof(buf.mtext));
    memcpy(buf.mtext, response->m_data, strlen(response->m_data));
    char szMsgType[MSG_TYPE_LEN + 1] = {0};
    memcpy(szMsgType, buf.mtext, MSG_TYPE_LEN);
    buf.mtype = strtol(szMsgType, NULL, 10);
    LOG_INFO("buf.mtype[%ld]", buf.mtype);
    if(response->m_status_code == 2)
    {
        if(g_IpcSynSndMsg->IpcSndMsg(buf, strlen(buf.mtext)) < 0)
            LOG_ERR("Req buf err!");
    }
	else if (response->m_status_code == 3)
	{
		LOG_DEBUG("need not send!");
	}
    else
    {
        if(g_IpcSndMsg->IpcSndMsg(buf, strlen(buf.mtext)) < 0)
            LOG_ERR("Req buf err!");
    }
    //LOG_INFO("send successful...................................................... ");
    if(result)
        delete result;
    if(response)
        delete response;

}
void *workerThread(void *arg)
{
    MultHttpClient *Mult = (MultHttpClient *)arg;
    Mult->Dispatch(HttpResponse);
}

void *LaunchHttpClient(void *arg)
{

    string strResponse = "";
    char *szIp = NULL;
    char *szPort = NULL;
    int   nRet = 0;
    char *tmp = NULL;
    char *szSndkey = NULL;
    char *szSndMax = NULL;
    char *szSynSndkey = NULL;
    char *szSynSndMax = NULL;
    char *szRcvkey = NULL;
    char *szRcvMax = NULL;
    //char* g_Url_Crm_My;

    char *szSndSynkey = NULL;
    char *szSndSynMax = NULL;




    char szCmdReq[CMD_LEN + 1];

    key_t msgSndkey ;
    key_t msgSynSndkey ;
    key_t msgRcvkey ;

    int len = 0;
    IpcMsgBuf buf;
    memset(&buf, 0x00, sizeof(buf));

    memset(szCmdReq, 0x00, sizeof(szCmdReq));

    buf.mtype = 0;



#if 0
    M_XRETURN(((szSndMax = getenv("MAX_MSGC")) == NULL),
              false, LOG_ERR("get msq num err!"));
    M_XRETURN(((szSndkey = getenv("MSG_HTTPCLIENT_IPC_BUSSND_KEY")) == NULL),
              false, LOG_ERR("get msq id err!"));
    sscanf(szSndkey, "%x", &msgSndkey);
    CIpcMsg vcSndMsg(atoi(szSndMax), 10240);
    vcSndMsg.IpcCreMsg(msgSndkey);
    // g_IpcSndMsg = &vcSndMsg;
    LOG_INFO("消息队列ID[%s]", szSndkey);
    LOG_INFO("消息队列key[%s]", szSndMax);

    M_XRETURN(((szSynSndMax = getenv("MAX_MSGB")) == NULL),
              false, LOG_ERR("get msq num err!"));
    M_XRETURN(((szSynSndkey = getenv("MSG_HTTPCLIENT_IPC_BUSSYN_KEY")) == NULL),
              false, LOG_ERR("get msq id err!"));
    LOG_INFO("消息队列ID[%s]", szSynSndkey);
    LOG_INFO("消息队列key[%s]", szSynSndMax);
    sscanf (szSynSndkey, "%x", &msgSynSndkey);
    LOG_INFO("消息队列标识[%d]", msgSynSndkey);

    CIpcMsg vcSynSndMsg(atoi(szSynSndMax), 10240);
    vcSynSndMsg.IpcCreMsg(msgSynSndkey);
    //g_IpcSynSndMsg = &vcSynSndMsg;
#endif
    M_XRETURN(((szRcvMax = getenv("MAX_MSGE")) == NULL),
              NULL, LOG_ERR("get msq num err!"));
    M_XRETURN(((szRcvkey = getenv("MSG_HTTPCLIENT_IPC_BUSRCV_KEY")) == NULL),
              NULL, LOG_ERR("get msq id err!"));
    sscanf (szRcvkey, "%x", &msgRcvkey);
    CIpcMsg vcRcvMsg(atoi(szRcvMax), 10240);
    vcRcvMsg.IpcCreMsg(msgRcvkey);
    LOG_INFO("消息队列ID[%s]", szRcvkey);
    LOG_INFO("消息队列key[%s]", szRcvMax);



/*
    sigset(SIGALRM, timeout);

    LOG_INFO("start to send req");
    signal(SIGHUP, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGQUIT, signal_handler);
*/
    char Url[128] = {0};
    char szLang[80] = {0};
    char SndTag[2048] = { 0 };
    char szMsgType[MSG_TYPE_LEN + 1] = {0};
    //char* sPos = NULL;
    //char* ePos = NULL;
    int  nLen = 0;
    int  nFlag = 0;
    //int  nRoute = 0;
    MsgHead msghead;

    HttpCliBusiness client;
    client.InitHttpClient();
    MultHttpClient *pMult = (MultHttpClient *)arg;
    HttpInfo *response = NULL;
    while(1)
    {
        nFlag = 0;

        LOG_INFO("start rcv msg");
        memset(buf.mtext, 0x00, BUFSZ);
        buf.mtype = 0;
        //strResponse = "";
        memset(Url, 0x00, sizeof(Url));

        if ((nRet = vcRcvMsg.IpcRcvMsg(&buf, BUFSZ)) <= 0)
        {
            sleep(1);
            LOG_ERR("RcvMsgErr[%d]", nRet);
            continue;
        }


        memset(szMsgType, 0x00, sizeof(szMsgType));
        memcpy(szMsgType, buf.mtext, MSG_TYPE_LEN);
        memcpy((char *)&msghead, buf.mtext, sizeof(MsgHead));

        //buf.mtype = strtol(szMsgType, NULL, 10);
        //LOG_INFO("szMsgType[%s],type[%ld]", szMsgType, buf.mtype);

        response = new HttpInfo();
        memset(response->m_data, 0x00, sizeof(response->m_data));
        memcpy(response->m_data, (char * )&msghead, sizeof(MsgHead));
        response->m_data[MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN] = '9';
        HttpResult *result = new HttpResult();
        //result->m_IpcSend = &vcSynSndMsg;
        response->m_ret = result;

        LOG_INFO("szMsgType=[%s]", szMsgType);
        int nRoute = client.RequestHttpCli(buf.mtext, Url, response, pMult);
        // feedback 配置出错的情况
        if(nRoute == -1)
        {        
            buf.mtype = strtol(szMsgType, NULL, 10);
            memset(buf.mtext, 0x00, BUFSZ);
            memcpy(buf.mtext, (char * )&msghead, sizeof(MsgHead));
            buf.mtext[MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN] = '9';
            strcat(buf.mtext, "|");
            strcat(buf.mtext, Url);
            strcat(buf.mtext, "|");
            if(result)
                delete result;
            if(response)
                delete response;
            if(g_IpcSndMsg->IpcSndMsg(buf, strlen(buf.mtext)) < 0)
                    LOG_ERR("szMsgType[%s] Req buf err!",szMsgType);
             
            continue;
        }

        LOG_INFO("finish req");
    }
    LOG_ERR("LaunchHttpClient exit");
    assert(false);

}


int main(int argc, char *argv[])
{
    if(argc < 2)
        return FAILE;

    InitLog("httpclient", argv[1]);
    LOG_INFO("httpclient[%s][%s]", argv[0], argv[1]);

    sigset(SIGALRM, timeout);
    LOG_INFO("start to send req");
    signal(SIGHUP, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGQUIT, signal_handler);
    signal(SIGPIPE, signal_handler);
 

    char *szSndkey = NULL;
    char *szSndMax = NULL;
    char *szSynSndkey = NULL;
    char *szSynSndMax = NULL;
    char *szSndSynkey = NULL;
    char *szSndSynMax = NULL;
    key_t msgSndkey ;
    key_t msgSynSndkey ;
    key_t msgRcvkey ;
    M_XRETURN(((szSndMax = getenv("MAX_MSGC")) == NULL),
              false, LOG_ERR("get msq num err!"));
    M_XRETURN(((szSndkey = getenv("MSG_HTTPCLIENT_IPC_BUSSND_KEY")) == NULL),
              false, LOG_ERR("get msq id err!"));
    sscanf(szSndkey, "%x", &msgSndkey);
    CIpcMsg vcSndMsg(atoi(szSndMax), 10240);
    vcSndMsg.IpcCreMsg(msgSndkey);
    g_IpcSndMsg = &vcSndMsg;
    LOG_INFO("消息队列ID[%s]", szSndkey);
    LOG_INFO("消息队列key[%s]", szSndMax);

    M_XRETURN(((szSynSndMax = getenv("MAX_MSGB")) == NULL),
              false, LOG_ERR("get msq num err!"));
    M_XRETURN(((szSynSndkey = getenv("MSG_HTTPCLIENT_IPC_BUSSYN_KEY")) == NULL),
              false, LOG_ERR("get msq id err!"));
    LOG_INFO("消息队列ID[%s]", szSynSndkey);
    LOG_INFO("消息队列key[%s]", szSynSndMax);
    sscanf (szSynSndkey, "%x", &msgSynSndkey);
    LOG_INFO("消息队列标识[%d]", msgSynSndkey);

    CIpcMsg vcSynSndMsg(atoi(szSynSndMax), 10240);
    vcSynSndMsg.IpcCreMsg(msgSynSndkey);
    g_IpcSynSndMsg = &vcSynSndMsg;
    curl_global_init(CURL_GLOBAL_ALL);
    init_locks();
    char *szThreadNum = NULL;
    M_XRETURN(((szThreadNum = getenv("HTTPCLIENT_THREADNUM")) == NULL),
              false, LOG_ERR("get msq num err!"));
    size_t threadNum = atoi(szThreadNum) ;
    for(size_t i = 0 ; i < threadNum; i++)
    {
        MultHttpClient *pMult = new MultHttpClient();
        pMult->InitMultCurl();
        pthread_t lanuchtid ;
        pthread_create(&lanuchtid, NULL, LaunchHttpClient, pMult);
        pthread_t disTid;
        pthread_create(&disTid, NULL, workerThread, pMult);

    }
    MultHttpClient *pMult = new MultHttpClient();
    pMult->InitMultCurl();
    pthread_t disTid;
    pthread_create(&disTid, NULL, workerThread, pMult);
    LaunchHttpClient(pMult);

    kill_locks();
    return 0;
}
