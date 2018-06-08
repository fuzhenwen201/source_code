#include "../include/worknew.h"

#define  HTTPRNRN       "\r\n\r\n"
#define  CONTENT_LENGTH "Content-Length"

CTinyHttp cTinyWrok;
CRedis rds;

long gOnlineTimeOut;
long gOnlineDelayTimeOut;
long gCrmTokenTimeOut;

//char* g_ServerUrl;
//char* g_ServerPort;

char *g_RdsPort;
char *g_RdsIp;
char *g_RdsTimeout;
char *g_RdsPassword;
char *g_RdsIns;

int g_LoginKey_SurviTime ;
int g_HistoryKey_SurviTime;

char g_ContentType[120];
CDispatch *g_pcdispatch;


pMsgHead msghead;
int nHeadLen;

//char g_AppToken[APP_TOKEN_LEN+1];
CIpcMemBoost *g_IpcMemBoost;
//char g_CrmToken[CRM_TOKEN_LEN+1];

void signal_handler(int sig)
{
    switch (sig)
    {
    case SIGKILL:
        LOG_INFO("kill");
        exit(1);
        break;
    case SIGTERM:
        LOG_INFO("sigterm");
        break;
    case SIGHUP:
        LOG_INFO("sighup");
        break;
    case SIGQUIT:
        LOG_INFO("sigquit");
        break;
    case SIGINT:
        LOG_INFO("sigint");
        break;
defaul:
        LOG_INFO("sig[%d]", sig);
    }
}

//消息队列句柄
//CIpcMsg* g_pRcvMsg;
//CIpcMsg* g_pSndMsg;


int lisSock;
int gworkNUm;
int gEpollNum;
int gMaxConnFd;
int gWorkGroup;
int *gepfd;
connection_t pconn_table;


std::map<long, struct sockinfo> sockMap;

pthread_t *pworker;
pthread_t *prcvmsgtid;
pthread_mutex_t maplock;

pthread_cond_t mapcond;
pthread_mutex_t Randlock;

void closeConnection(connection_t conn);

static sig_atomic_t shut_server = 0;
unsigned long g_seq_num = 1;

void shut_server_handler(int signo)
{
    shut_server = 1;
    LOG_INFO("sig[%d],shut_server[%d]", signo, shut_server);
}

void GetRand( unsigned long &lType)
{
    //pthread_mutex_lock(&Randlock);
    if(g_seq_num >= 999999)
        g_seq_num = 1;
    else
        ++g_seq_num;

    srand((unsigned)time(0));
    lType = rand();// % 10000000;
    lType += g_seq_num * 1000000000;
    //pthread_mutex_unlock(&Randlock);
}


int Equal(char a, char b)
{
    if(a >= 'A' && a <= 'Z') a += 'a' - 'A';
    if(b >= 'A' && b <= 'Z') b += 'a' - 'A';
    return a == b;
}

char *FindSubstr(char *s, char *sub)
{
    char *p, *q;
    while(*s)
    {
        for(p = s, q = sub; *q && Equal(*p, *q); p++, q++);
        if(*q == '\0') return s + strlen(sub);
        s++;
    }
    return 0;
}


void GetTagVal(char *buf, char *tag, char *dest)
{
    char *sPos = NULL;
    if(sPos = FindSubstr(buf, tag))
    {
        while(*sPos && *sPos != '\r' && *sPos != '\n' && *sPos != '&' && *sPos != '|')
        {
            *dest = *sPos;
            dest++;
            sPos++;
        }
    }
}

int get_line(int sock, char *buf, int size, int &ret, int &errcode)
{
    int i = 0;
    char c = '\0';
    int n;

    /*把终止条件统一为 \n 换行符，标准化 buf 数组*/
    while ((i < size - 1) && (c != '\n'))
    {
        n = recv(sock, &c, 1, 0);
        if (n > 0)
        {
            if (c == '\r')
            {
                /*使用 MSG_PEEK 标志使下一次读取依然可以得到这次读取的内容，可认为接收窗口不滑动*/
                n = recv(sock, &c, 1, MSG_PEEK);
                /* DEBUG printf("%02X\n", c); */
                if ((n > 0) && (c == '\n'))
                    recv(sock, &c, 1, 0);
                else
                    c = '\n';
            }
            /*存到缓冲区*/
            buf[i] = c;
            i++;
        }
        else
        {
            ret = n;
            errcode = errno;
            c = '\n';
        }
    }
    buf[i] = '\0';

    return(i);
}

int judgeRequest(connection_t conn, struct epoll_event ev)
{
    char buf[8] = { 0 };

    int ret = recv(conn->sock, buf, 4, MSG_PEEK);

    if (ret < 0)
    {
        //由于是非阻塞的模式,所以当errno为EAGAIN时,表示当前缓冲区已无数据可//读在这里就当作是该次事件已处理过。
        if (errno == EAGAIN)
        {
            LOG_INFO("this is normal");
            return 0;
        }
        else
        {
            LOG_ERR("recv error[%d]!", errno);
            return -1;
        }
    }
    else if (ret == 0)
    {
        // 这里表示对端的socket已正常关闭.
        LOG_INFO("client close errno[%d]", errno);
        return -1;
    }

    LOG_INFO("get %s request", buf);
    return strcmp(buf, "POST") == 0 ? 1 : 2;
}

int handleGetRequest(connection_t conn, struct epoll_event ev, CIpcMsg MsgWorkIpcComSndKey)
{
    IpcMsgBuf ipcBuf;
    memset(ipcBuf.mtext, 0x00, BUFSZ);


    int ret = 0;
    int total = 0;
    int headLen = 0;

    headLen = sizeof(MsgHead);
#if 1
    while (1)
    {
        ret = recv(conn->sock, ipcBuf.mtext + headLen + total, BUFSZ - 1, 0);// 接受客户端消息

        if (ret < 0)
        {
            //由于是非阻塞的模式,所以当errno为EAGAIN时,表示当前缓冲区已无数据可//读在这里就当作是该次事件已处理过。
            if (errno == EAGAIN)
            {
                LOG_INFO("ret[%d]errno[%d]size[%d]total[%d]", ret, errno, strlen(ipcBuf.mtext), total);
                break;
            }
            else
            {
                LOG_ERR("recv error[%d]!", errno);
                return FAILE;
            }
        }
        else if (ret == 0)
        {
            // 这里表示对端的socket已正常关闭.
            LOG_ERR("client close");
            return FAILE;
        }

        total += ret;
    }
	
    if (total > 0)
    {
        //-------------------------------------------------------------------------------
        unsigned long lType = 0;
        pthread_mutex_lock(&maplock);
        GetRand(lType);
        sockinfo SockInfo;
        ipcBuf.mtype = lType;
        SockInfo.shmid = 0;
        time(&SockInfo.ltime);
        memcpy(&SockInfo.event, &ev, sizeof(ev));
        sprintf(ipcBuf.mtext, "%016ld", ipcBuf.mtype);
        memset(ipcBuf.mtext + MSG_TYPE_LEN, ' ', headLen - MSG_TYPE_LEN);
        sockMap.insert(make_pair(lType, SockInfo));
        pthread_mutex_unlock(&maplock);
        ipcBuf.mtext[MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN] = '1';
        //g_pMsgWorkIpcComSnd->IpcSndMsg(ipcBuf, strlen(ipcBuf.mtext));
        MsgWorkIpcComSndKey.IpcSndMsg(ipcBuf, strlen(ipcBuf.mtext));
        LOG_INFO("mtype[%ld],MsgId[%d],sock[%d]",
                 ipcBuf.mtype, MsgWorkIpcComSndKey.m_MsgId, ev.data.fd);
        memset(ipcBuf.mtext, 0x00, SNDBUFSZ);
    }
#endif

    return 0;
}
 
int getHttpContentLength(const char *buf)
{
    //string head = buf;
    //size_t pos = head.find(CONTENT_LENGTH);
    //if (pos == string::npos)
    //  return FAILE;

    char *sPos = NULL;
    int length = 0;
    char *pContent_len = const_cast<char *>(CONTENT_LENGTH);
    char *sbuf = const_cast<char *>(buf);
    sPos = FindSubstr(sbuf, pContent_len);
    if(sPos == NULL)
        return FAILE;
    //char *cursor = (char *)buf + pos;
    char *cursor = sPos;
    while (*cursor != '\n')
    {
        if (isdigit(*cursor))
        {
            do
            {
                length *= 10;
                length += *cursor - '0';
                cursor++;
            }
            while (isdigit(*cursor) && *cursor != '\n');

            return length;
        }

        cursor++;
    }

    return FAILE;
}

int checkHttpHead(const char *buf)
{
    std::string head = buf;
    size_t pos = head.find(HTTPRNRN);

    return pos == std::string::npos ? -1 : pos;
}

int handlePostRequest(connection_t conn, struct epoll_event ev, CIpcMsg MsgWorkIpcComSndKey)
{
    pIpcMsgBuf pIpcBuf = &conn->ipcBuf;
    conn->woff = 0;
    conn->roff = 0;
    memset(pIpcBuf->mtext, 0x00, BUFSZ);

    CIpcMem mIpcMem;
    unsigned long key = 0;
    //  IpcMsgBuf mBody;


    int ret = 0;
    int times = 0;
    int nHeadLen = sizeof(MsgHead);

    // 还未接收POST请求头
    while (1)
    {
        ret = recv(conn->sock, pIpcBuf->mtext, BUFSZ - 1, MSG_PEEK);// 窥探buf里面的数据

        if (ret < 0)
        {
            //mIpcMem.Detach();
            //mIpcMem.Delete();
            LOG_ERR("recv head err[%d]!", errno);
            return FAILE;
        }
        else if (ret == 0)
        {
            // 这里表示对端的socket已正常关闭.
            //mIpcMem.Detach();
            //mIpcMem.Delete();
            LOG_ERR("client close");
            return FAILE;
        }

        //LOG_INFO("get valid data");

        // 接收POST HTTP包头
        //计算body 长度,并接收head
        int headLen = 0;
        if ((headLen = checkHttpHead(pIpcBuf->mtext)) != -1)
        {
            LOG_INFO("HEAD LEN[%d]", headLen);
            // 获取HTTP包体长度
            if ((conn->roff = getHttpContentLength(pIpcBuf->mtext)) == -1)
            {
                LOG_ERR("get post head content-length failed,buf[%s]", pIpcBuf->mtext);
                return FAILE;
            }
            LOG_INFO("the body length [%d]",conn->roff);
            if(conn->roff > SHMBUFSZ)
            {
                LOG_ERR("the body length is too big [%d]",conn->roff);
                return FAILE;
            }
            memset(pIpcBuf->mtext, 0x00, BUFSZ);
            ret = recv(conn->sock, pIpcBuf->mtext + nHeadLen, headLen + strlen(HTTPRNRN), 0);// 接收HTTP头

            // 读取缓冲区中的HTTP请求包头
            if (ret > 0)
            {
                pthread_mutex_lock(&maplock);
                GetRand(key);
                pthread_mutex_unlock(&maplock);
                if (mIpcMem.Create(key % 1000000000) != 0)
                {
                    LOG_ERR("create share memary error,%d", errno);
                    return FAILE;
                }

                bzero(mIpcMem.GetBuf(), sizeof(IpcMemBuf));

                LOG_INFO("get post head success[%s],ret[%d],contentlength[%d]", pIpcBuf->mtext + nHeadLen, ret, conn->roff);

                //continue;
            }
            else
            {
                LOG_ERR("get http post head error,%d", errno);
                return FAILE;
            }
        }
        else
        {
            LOG_ERR("get http post head failed,repeat times:%d", conn->woff);
            return FAILE;
        }
#if 1
        //接收body数据
        while(1)
        {
            struct timeval timeout = {5, 0};
            if(setsockopt(conn->sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval)) == -1)
            {
                LOG_INFO("NO[%d]", errno);
            }
            // 获取HTTP包体长度
            ret = recv(conn->sock, mIpcMem.GetBuf()->mtext + mIpcMem.GetBuf()->msize, conn->roff, MSG_WAITALL);
            //ret = recv(conn->sock, mIpcMem.GetBuf()->mtext + mIpcMem.GetBuf()->msize, conn->roff-(int)mIpcMem.GetBuf()->msize, 0);
            LOG_INFO("XXXXXXXXXsize[%d]roff[%d]ret[%d]errno[%d]", (int)mIpcMem.GetBuf()->msize, conn->roff, ret, errno);
            //ret = recv(conn->sock, mIpcMem.GetBuf()->mtext + mIpcMem.GetBuf()->msize, BUFSZ - 1, MSG_PEEK);// 窥探buf里面的数据
            if (ret > 0)
            {
                mIpcMem.GetBuf()->msize += ret;
                if ((int)mIpcMem.GetBuf()->msize == conn->roff)
                {
                    goto label;
                }
                times = 0;
                continue;
            }
            else if (ret < 0)
            {
                //if (errno == EAGAIN && times < 3)
                if (errno == EAGAIN && (int)mIpcMem.GetBuf()->msize <= conn->roff)
                {
                    LOG_DEBUG("size[%d]roff[%d]ret[%d]", (int)mIpcMem.GetBuf()->msize, conn->roff, ret);
                    //LOG_INFO("size[%d]roff[%d]", (int)mIpcMem.GetBuf()->msize, conn->roff);
                    if ((int)mIpcMem.GetBuf()->msize == conn->roff)
                    {
                        goto label;
                    }
                    //times++;
                    usleep(1);
                    continue;
                }
                LOG_ERR("get http post body error[%d], times[%d]", errno, times);
                return FAILE;
            }
            else
            {
                LOG_ERR("get http post body failed,repeat times:%d", conn->woff);
            }
            mIpcMem.Detach();
            mIpcMem.Delete();
            return FAILE;
        }
#endif
    }

label:
    pthread_mutex_lock(&maplock);
    pIpcBuf->mtype = key;
    sockinfo SockInfo;
    SockInfo.shmid = mIpcMem.GetShmid();
    time(&SockInfo.ltime);
    memcpy(&SockInfo.event, &ev, sizeof(ev));
    sprintf(pIpcBuf->mtext, "%016ld", pIpcBuf->mtype);
    memset(pIpcBuf->mtext + MSG_TYPE_LEN, ' ', nHeadLen - MSG_TYPE_LEN);
    pIpcBuf->mtext[MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN] = '2';
    sockMap.insert(make_pair(pIpcBuf->mtype, SockInfo));
    pthread_mutex_unlock(&maplock);
    //g_pMsgWorkIpcComSnd->IpcSndMsg(*pIpcBuf, strlen(pIpcBuf->mtext));
    MsgWorkIpcComSndKey.IpcSndMsg(*pIpcBuf, strlen(pIpcBuf->mtext));
    LOG_DEBUG("post mtype[%ld],MsgId[%d],sock[%d]recvdata[%s]",
             pIpcBuf->mtype, g_pMsgWorkIpcComRcv->m_MsgId, ev.data.fd, pIpcBuf->mtext);

    LOG_INFO("size[%d]roff[%d]ret[%d]", (int)mIpcMem.GetBuf()->msize, conn->roff, ret);

    return SUCESS;
}

int handleReadEvent(connection_t conn, struct epoll_event ev, CIpcMsg MsgWorkIpcComSndKey)
{
    int nRet = judgeRequest(conn, ev);
    switch (nRet)
    {
    case 1:
    {
        return handlePostRequest(conn, ev, MsgWorkIpcComSndKey);
        //return -1;
        break;
    }
    case 2:
    {
        return handleGetRequest(conn, ev, MsgWorkIpcComSndKey);
        break;
    }
    default:
        return nRet;
        break;
    }
}

int handleWriteEvent(int sock, char *buf, int len)
{
    int vnpExpDataL = len;
    int lnTotalDataL = 0;
    int lnDataL = 0;
    int llTmp = 0;
    char *lspTcpData = NULL;
    lspTcpData = buf;
    while (vnpExpDataL > lnTotalDataL)
    {
        if ((lnDataL = send(sock,
                            lspTcpData,
                            vnpExpDataL - lnTotalDataL,
                            0)) < 0)
        {

			if (errno == EAGAIN)
			{
				continue;
			}
            llTmp = errno;
            vnpExpDataL = lnTotalDataL;
			LOG_ERR("socket close, len = %d", vnpExpDataL);
            break;
        }
        lnTotalDataL += lnDataL;
        lspTcpData += lnDataL;
		if (lnTotalDataL == 0)
		{
			LOG_ERR("get 0,len = %d", vnpExpDataL);
			break;
		}
    }
    LOG_DEBUG("lnTotalDataL[%d]", lnTotalDataL);
}

void closeConnection(connection_t conn)
{
    struct epoll_event evReg;

    conn->inuse = 0;
    conn->woff = conn->roff = 0;
    epoll_ctl(*(gepfd + conn->index), EPOLL_CTL_DEL, conn->sock, &evReg);
    close(conn->sock);
}

void *RcvMsgThread(void *arg)
{
    int epfd = *(int *)arg;
    IpcMsgBuf ipcBuf;
    char szMsgType[MSG_TYPE_LEN + 1] = {0};
    int nHeadLen = sizeof(MsgHead);


    char *pMsgWorkIpcComRcvKey;
    key_t tMsgWorkIpcComRcvKey;


    M_VRETURNP(((pMsgWorkIpcComRcvKey = getenv("MSG_WORK_IPC_COMRCV_KEY")) == NULL),
               LOG_ERR("get MSG_WORK_IPC_COMRCV_KEY err"));

    sscanf (pMsgWorkIpcComRcvKey, "%x", &tMsgWorkIpcComRcvKey);
    CIpcMsg MsgWorkIpcComRcv;
    MsgWorkIpcComRcv.IpcCreMsg(tMsgWorkIpcComRcvKey);


    while(1)
    {
        ipcBuf.mtype = 0;
        LOG_INFO("rcvnsg start");
        memset(ipcBuf.mtext, 0x00, BUFSZ);

        //if (g_pMsgWorkIpcComRcv->IpcRcvMsg(&ipcBuf, BUFSZ)  == FAILE)
        if (MsgWorkIpcComRcv.IpcRcvMsg(&ipcBuf, BUFSZ)  == FAILE)
        {
            sleep(1);
            LOG_ERR("msgid[%x]errno[%d]", MsgWorkIpcComRcv.m_MsgId, errno);
            continue;
        }
        memcpy(szMsgType, ipcBuf.mtext, MSG_TYPE_LEN);

        LOG_DEBUG("msgtype[%s]", szMsgType);


        std::map<long, sockinfo>::iterator it;
        it = sockMap.find(atol(szMsgType));
        if(it != sockMap.end())
        {
            LOG_DEBUG("key[%ld]value[%d]", it->first, it->second.event.data.fd);
            connection_t conn = pconn_table + it->second.event.data.fd;
            memcpy(conn->ipcBuf.mtext, ipcBuf.mtext + nHeadLen , strlen(ipcBuf.mtext) + nHeadLen);
            struct epoll_event evReg;
            evReg.events |= EPOLLOUT | EPOLLONESHOT;
            evReg.data.fd = it->second.event.data.fd;

            if(epoll_ctl(epfd, EPOLL_CTL_MOD, it->second.event.data.fd, &evReg) < 0)
            {
                LOG_ERR("socket fd failed error[%d]", it->second.event.data.fd, errno);
                continue;
            }

            if (it->second.shmid > 0)
            {
                CIpcMem::Delete(it->second.shmid); // 删除共享内存
                LOG_INFO("delete share memary,shmid:%d", it->second.shmid);
            }

            //移除
            pthread_mutex_lock(&maplock);
            sockMap.erase(it);
            pthread_mutex_unlock(&maplock);
            LOG_DEBUG("erase sucess");
        }
        else
        {
            LOG_ERR("not found msgtype[%s]", szMsgType);
        }
    }
    LOG_ERR("RCVMSGTHREAD EXIT epfd[%d]", epfd);

}
void *workerThread(void *arg)
{
    int epfd = *(int *)arg;

    struct epoll_event event;
    struct epoll_event evReg;

#if 0
    char *pMsgWorkIpcComSndKey;
    key_t tMsgWorkIpcComSndKey;

    M_VRETURNP(((pMsgWorkIpcComSndKey = getenv("MSG_WORK_IPC_COMSND_KEY")) == NULL),
               LOG_ERR("get MSG_WORK_IPC_COMSND_KEY err!"));
    sscanf (pMsgWorkIpcComSndKey, "%x", &tMsgWorkIpcComSndKey);
    CIpcMsg MsgWorkIpcComSndKey;
    MsgWorkIpcComSndKey.IpcCreMsg(tMsgWorkIpcComSndKey);
#endif


    char *pMsgWorkIpcBusRcvKey;
    key_t tMsgWorkIpcBusRcvKey;

    M_XRETURN(((pMsgWorkIpcBusRcvKey = getenv("MSG_WORK_IPC_BUSRCV_KEY")) == NULL),
              NULL, LOG_ERR("get MSG_WORK_IPC_BUSRCV_KEY err!"));
    sscanf (pMsgWorkIpcBusRcvKey, "%x", &tMsgWorkIpcBusRcvKey);
    CIpcMsg MsgWorkIpcBusRcv;
    MsgWorkIpcBusRcv.IpcCreMsg(tMsgWorkIpcBusRcvKey);


    while (!shut_server)
    {
        int numEvents = epoll_wait(epfd, &event, 1, 1000);

        if (numEvents > 0)
        {
            int sock = event.data.fd;
            connection_t conn = pconn_table + sock;

            if (event.events & EPOLLOUT)
            {
                if (handleWriteEvent(sock, conn->ipcBuf.mtext, strlen(conn->ipcBuf.mtext)) == -1)
                {
                    LOG_ERR("Write err, errno[%d]", errno);
                }
                LOG_INFO("find write[%ld]", conn->ipcBuf.mtype);
                epoll_ctl(epfd, EPOLL_CTL_DEL, sock, &evReg);
                close(sock);
                memset(conn->ipcBuf.mtext, 0x00, strlen(conn->ipcBuf.mtext));
                continue;
            }

            if (event.events & EPOLLIN)
            {
                //if (handleReadEvent(conn, event, MsgWorkIpcComSndKey) == -1) {
                if (handleReadEvent(conn, event, MsgWorkIpcBusRcv) == -1)
                {
                    closeConnection(conn);
                    continue;
                }
            }
        }
    }
    LOG_INFO("thead exit");
    return NULL;
}

void *listenThread(void *arg)
{
    int lisEpfd = epoll_create(10240);

    struct epoll_event evReg;
    evReg.events  = EPOLLIN;
    evReg.data.fd = lisSock;

    epoll_ctl(lisEpfd, EPOLL_CTL_ADD, lisSock, &evReg);

    struct epoll_event event;

    int rrIndex = 0; /* round robin index */

    /* only handle listen socekt */
    while (!shut_server)
    {
        int numEvent = epoll_wait(lisEpfd, &event, 1, 1000);
        if (numEvent > 0)
        {
            int sock = accept(lisSock, NULL, NULL);
            if (sock > 0)
            {
                if(gMaxConnFd < sock)
                {
                    LOG_INFO("maxfd[%d], sock[%d]", gMaxConnFd, sock);
                    close(sock);
                }
                else
                {
                    (pconn_table + sock)->inuse = 1;

                    int flag;
                    flag = fcntl(sock, F_GETFL);
                    fcntl(sock, F_SETFL, flag | O_NONBLOCK);

                    evReg.data.fd = sock;
                    evReg.events = EPOLLIN | EPOLLONESHOT;

                    (pconn_table + sock)->index = rrIndex;
                    epoll_ctl(*(gepfd + rrIndex), EPOLL_CTL_ADD, sock, &evReg);
                    rrIndex = (rrIndex + 1) % gEpollNum;
                    LOG_DEBUG("new conn fd[%d]", sock);
                }
            }
        }
    }

    close(lisEpfd);
    return NULL;
}

void subscribeconfig(redisAsyncContext *c, void *r, void *priv)
{
    redisReply *reply = (redisReply*)r;
    if (reply == NULL) return;
    if ( reply->type == REDIS_REPLY_ARRAY && reply->elements == 3 )
    {
        if ( strcmp( reply->element[0]->str, "subscribe" ) != 0 )
        {
            LOG_INFO("channel %s: %s\n", reply->element[1]->str, reply->element[2]->str);
            CRedis* rds = (CRedis*)priv;
            LoadCrmConfig(*rds);
        }
    }
}

void subscribeToken(redisAsyncContext *c, void *r, void *priv)
{	
	redisReply *reply = (redisReply*)r;
	if (reply == NULL) return;
	LOG_DEBUG("type=%d,ele=%d", reply->type, reply->elements);
	if (reply->type == REDIS_REPLY_ARRAY && reply->elements == 4)
	{
		if (strcmp(reply->element[0]->str, "psubscribe") != 0)
		{
			LOG_INFO("channel %s: %s\n", reply->element[1]->str, reply->element[2]->str);
			
			char *pTmp = strstr(reply->element[2]->str, (char*)"expired");
			if (pTmp != NULL)
			{
				LOG_INFO("is expired!");
				CRedis* rds = (CRedis*)priv;
				LOG_DEBUG("key is : %s", reply->element[3]->str);
				if(strstr(reply->element[3]->str,"commemdb"))
				{
					LOG_DEBUG("del met4 token");
					DelMt4Token(reply->element[3]->str, *rds);
				}
			}
		}
	}
}

// subscribe redis
void* subscribethread(void *param)
{
    CRedis *rds = (CRedis*)param;
    struct event_base *base = event_base_new();
    //初始化redis的IP和端口
    char *pRdsIp = NULL;
    char *pRdsPort = NULL;
    char *pRdsTimeout = NULL;
    char *pRdsPassword = NULL;
    char *pRdsIns = NULL;
    M_XRETURN(((pRdsIp = getenv("RDS_IP")) == NULL),
              NULL, LOG_ERR("get env RDS_IP err!"));

    M_XRETURN(((pRdsPort = getenv("RDS_PORT")) == NULL),
              NULL, LOG_ERR("get env RDS_PORT err!"));

    M_XRETURN(((pRdsTimeout = getenv("RDS_CON_TIMEOUT")) == NULL),
              NULL, LOG_ERR("get env RDS_CON_TIMEOUT err!"));

    //M_XRETURN(((pRdsPassword = getenv("RDS_PWD")) == NULL),
              //NULL, LOG_ERR("get env RDS_PWD err!"));
	pRdsPassword = getenv("RDS_PWD");
    pRdsIns = getenv("RDS_INS");

    LOG_INFO("init redis ip[%s],por[%s],time[%s],pwd[%s],ins[%s]", pRdsIp, pRdsPort, pRdsTimeout, pRdsPassword, pRdsIns);

    CRedis cRds;
	if (int nRet = cRds.asynconnect(pRdsIp, atoi(pRdsPort), atoi(pRdsTimeout), pRdsPassword == NULL ? "" : pRdsPassword ,
                               pRdsIns == NULL ? "" : pRdsIns, base))
    {
        LOG_INFO("connect reids err[%d]", nRet);
        return NULL;
    }

    cRds.asyncsubscribe("configure", subscribeconfig,rds);
	cRds.asyncpsubscribe("__keyevent@0__:expired", subscribeToken, rds);
    
	event_base_dispatch(base);

    return NULL;
}

int main(int argc, char *const argv[])
{
#if 0
    if(argc < 6)
        return FAILE;
#endif

    InitLog("worknew", argv[1]);
    LOG_INFO("args[%s][%s][%s][%s][%s][%s][%s][%s]",
             argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);

    !getenv("OL_TIME_OUT") ?
    gOnlineTimeOut = 15 :
                     gOnlineTimeOut = atoi(getenv("OL_TIME_OUT"));
    LOG_INFO("user online timeout[%d]", gOnlineTimeOut);

    !getenv("OL_DEL_TIME_OUT") ?
    gOnlineDelayTimeOut = 15 :
                          gOnlineDelayTimeOut = atoi(getenv("OL_DEL_TIME_OUT"));
    LOG_INFO("user online timeout[%d]", gOnlineDelayTimeOut);

    !getenv("CRM_TOKEN_TIME_OUT") ?
    gCrmTokenTimeOut = 850 :
                       gCrmTokenTimeOut = atoi(getenv("CRM_TOKEN_TIME_OUT"));
    LOG_INFO("crm token timeout[%d]", gCrmTokenTimeOut);

    char *szSndkey = NULL;
    char *szSndMax = NULL;
    char *szRcvkey = NULL;
    char *szRcvMax = NULL;

    key_t msgSndkey ;
    key_t msgRcvkey ;
    IpcMsgBuf ipcBuf;

    //初始化redis的IP和端口
    M_XRETURN(((g_RdsIp = getenv("RDS_IP")) == NULL),
              false, LOG_ERR("get env RDS_IP err!"));

    M_XRETURN(((g_RdsPort = getenv("RDS_PORT")) == NULL),
              false, LOG_ERR("get env RDS_PORT err!"));

    M_XRETURN(((g_RdsTimeout = getenv("RDS_CON_TIMEOUT")) == NULL),
              false, LOG_ERR("get env RDS_CON_TIMEOUT err!"));

    //M_XRETURN(((g_RdsPassword = getenv("RDS_PWD")) == NULL),
              //false, LOG_ERR("get env RDS_PWD err!"));
	g_RdsPassword = getenv("RDS_PWD");
    g_RdsIns = getenv("RDS_INS");

    LOG_INFO("init redis ip[%s],por[%s],time[%s],pwd[%s],ins[%s]", g_RdsIp, g_RdsPort, g_RdsTimeout, g_RdsPassword, g_RdsIns);


    //初始化白名单
    g_cusermanage.InitUserList();

    signal(SIGHUP, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGQUIT, signal_handler);
    signal(SIGKILL, signal_handler);

    int nRet = 0;
    CRedis rds;
    if(nRet = rds.connect(g_RdsIp, atoi(g_RdsPort), atoi(g_RdsTimeout), g_RdsPassword == NULL ? "" : g_RdsPassword, g_RdsIns == NULL ? "" : g_RdsIns))
    {
        LOG_INFO("connect reids err[%d]", nRet);
        return FAILE;
    }

    //初始化错误应答信息
    interfaceErrKWordsInit(rds);

    if (LoadCrmConfig(rds) != 0)
    {
        LOG_ERR("get crm confi failed %d", errno);
        return FAILE;
    }

    //int nHeadLen = MSG_TYPE_LEN+SPLIT_LEN;
    nHeadLen = sizeof(MsgHead);

    LOG_INFO("headlen[%d]", nHeadLen);


    g_pcdispatch = new CMsghandle();


    int nIndex = 0;
    int nRoute = 0;
    char szDest[BUFSZ];
    char szMsgType[MSG_TYPE_LEN + 1] = {0};
    char szTokenTmp[APP_TOKEN_LEN + 1];

    InitMsg();

    char *szMaxfd = getenv("CONN_MAXFD");

    szMaxfd == NULL ?
    gMaxConnFd = 20480 :
                 gMaxConnFd = atoi(szMaxfd);

    char nbusythrnum;
    char *szbusythrnum = getenv("BUSY_THR_NUM");
    szbusythrnum == NULL ?
    nbusythrnum = 15 :
                  nbusythrnum = atoi(szbusythrnum);

    char *szWorkGroup = getenv("WORKER_PER_GROUP");
    szWorkGroup == NULL ?
    gWorkGroup = 10 :
                 gWorkGroup = atoi(szWorkGroup);

    char *szEpollNum = getenv("EPOLL_NUM");
    szEpollNum == NULL ?
    gEpollNum = 1 :
                gEpollNum = atoi(szEpollNum);

    gworkNUm = gEpollNum * gWorkGroup;

    int epfd[gEpollNum] ;
    memset(epfd, 0x00, sizeof(epfd));
    gepfd = epfd;

    char *LoginKey_SurviTime = getenv("LOGKEY_SURVITIME");
    g_LoginKey_SurviTime = atoi(LoginKey_SurviTime);
    char *HistoryKey_SurviTime = getenv("HISKEY_SURVITIME");
    g_HistoryKey_SurviTime = atoi(HistoryKey_SurviTime);

    pconn_table = new connection_st[gMaxConnFd];
    M_XRETURN(pconn_table == NULL,
              false, LOG_ERR("alloc mem err!"));

    memset(pconn_table, 0x00, sizeof(connection_st));

    pthread_t worker[gworkNUm];
    memset(worker, 0x00, sizeof(pthread_t)*gworkNUm);

    pthread_t rcvmsgtid[gEpollNum];
    memset(rcvmsgtid, 0x00, sizeof(pthread_t)*gEpollNum);

    LOG_INFO("MaxConnFd[%d],WorkGroup[%d],EpollNum[%d],workNUm[%d]busytrdnum[%d]",
             gMaxConnFd, gWorkGroup, gEpollNum, gworkNUm, nbusythrnum);

    int c;
    for (c = 0; c < gMaxConnFd; ++c)
    {
        (pconn_table + c)->sock = c;
    }

    // subscribe redis
    pthread_t sub;
    pthread_create(&sub, NULL, subscribethread, &rds);

    pthread_mutex_init(&maplock, NULL);
    pthread_mutex_init(&Randlock, NULL);
    pthread_cond_init(&mapcond, NULL);

    struct sigaction act;
    memset(&act, 0, sizeof(act));

    act.sa_handler = shut_server_handler;
    sigaction(SIGINT, &act, NULL);
    //sigaction(SIGTERM, &act, NULL);

    /* create 2 different epoll fd */
    int epi;
    for (epi = 0; epi < gEpollNum; ++ epi)
    {
        epfd[epi] = epoll_create(gMaxConnFd);
    }

    lisSock = socket(AF_INET, SOCK_STREAM, 0);

    int reuse = 1;
    setsockopt(lisSock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    int flag;
    flag = fcntl(lisSock, F_GETFL);
    fcntl(lisSock, F_SETFL, flag | O_NONBLOCK);


    char *szLisPort = getenv("LISPORT");
    int nPort;

    argv[2] == NULL ?
    nPort = 9090 :
            nPort = atoi(argv[2]);

    LOG_INFO("PORT[%d][%s]", nPort, argv[2]);


    struct sockaddr_in lisAddr;
    lisAddr.sin_family = AF_INET;
    lisAddr.sin_port = htons(nPort);
    lisAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(lisSock, (struct sockaddr *)&lisAddr, sizeof(lisAddr)) == -1)
    {
        perror("bind");
        return -1;
    }

    listen(lisSock, 4096);

    pthread_t lisTid;
    pthread_create(&lisTid, NULL, listenThread, NULL);


    for (int n = 0; n < gEpollNum; ++ n)
    {
        pthread_create(rcvmsgtid + n, NULL, RcvMsgThread, epfd + n);
    }

    int i;

    for (i = 0; i < gEpollNum; ++i)
    {
        int j;
        for (j = 0; j < gWorkGroup; ++j)
        {
            pthread_create(worker + (i * gWorkGroup + j), NULL, workerThread, epfd + i);
        }
    }

    pthread_t busyTid[nbusythrnum];

    for(int j = 0; j < nbusythrnum; ++j)
    {
        pthread_create(busyTid, NULL, businessThread, NULL);
    }

    for (i = 0; i < gworkNUm; ++i)
    {
        pthread_join(worker[i], NULL);
        LOG_INFO("回收工作线程资源[%ld]", worker[i]);
    }

    for (i = 0; i < nbusythrnum; ++i)
    {
        pthread_join(busyTid[i], NULL);
        LOG_INFO("回收工作线程资源[%ld]", busyTid[i]);
    }

    pthread_join(lisTid, NULL);

    for (int n = 0; n < gEpollNum; ++ n)
    {
        pthread_join(rcvmsgtid[n], NULL);
    }

    struct epoll_event evReg;

    for (c = 0; c < gMaxConnFd; ++c)
    {
        connection_t conn = pconn_table + c;
        if (conn->inuse)
        {
            epoll_ctl(epfd[conn->index], EPOLL_CTL_DEL, conn->sock, &evReg);
            close(conn->sock);
            LOG_INFO("close sock[%d]", conn->sock);
        }
    }

    for (epi = 0; epi < gEpollNum; ++epi)
    {
        close(epfd[epi]);
        LOG_INFO("close epfd[%d]", epfd[epi]);
    }
    close(lisSock);

    pthread_mutex_destroy(&maplock);
    pthread_mutex_destroy(&Randlock);
    pthread_cond_destroy(&mapcond);

    delete []pconn_table;
    delete g_pcdispatch;
    return 0;
}
