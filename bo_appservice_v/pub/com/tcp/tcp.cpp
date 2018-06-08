#include "../../../include/tcp.h"

int g_epollFd = 0;
#define MAXBUF 1024
#define MAX_EVENTS 10000  //定义最大连接数

#if 1
#define SACEnvCommTimeout           "TL_COMM_TIMEOUT"
#define LCMaxTimeout                20
#define LCTcpRcvFlg                 0
#define LCTcpSndFlg                 0
#define NCRcvFlgReturnAfterData     1
#define NBLOCK_FLAG                 0
#define BLOCK_FLAG                  1
#define LIS_NUM						1024
#endif
stEpollEvent g_Events[MAX_EVENTS+1];

void EventSet(stEpollEvent* ev, int fd, void (*call_back)(int, int, void*), void *arg)
{
	ev->fd = fd;
	ev->call_back = call_back;
	ev->events = 0;
	ev->arg = arg;
	ev->status = 0;
	ev->last_active = time(NULL);
}

int InitSocket(int epollFd, short port, char* ip)
{
	int nRet , flag;
	int listenFd = socket(AF_INET, SOCK_STREAM, 0);
	if( -1  == listenFd)
	{
		LOG_ERR("socket err[%d]",errno);
		return ERR_SOCKET;
	}
	if ((nRet = fcntl(listenFd, F_SETFL, O_NONBLOCK)) < 0)
	{
		LOG_ERR("fcntl err[%d]",errno);
		return ERR_FCNTL;
	}

	flag = 1;
	if( nRet = setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) )
	{
		LOG_ERR("sockopt err[%d]",errno);
		return ERR_SOCKOPT;
	}

	LOG_INFO("server listen fd=%d", listenFd);
	EventSet(&g_Events[MAX_EVENTS], listenFd, AcceptConn, &g_Events[MAX_EVENTS]);
	// add listen socket
	if(nRet = EventAdd(epollFd, EPOLLIN|EPOLLET, &g_Events[MAX_EVENTS]))
	{
		return ERR_EPOLL_CTL;
	}
	// bind & listen
	sockaddr_in sin;
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	//sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_addr.s_addr = inet_addr(ip);
	sin.sin_port = htons(port);
	if(nRet = bind(listenFd, (const sockaddr*)&sin, sizeof(sin)))
	{
		LOG_ERR("bind err[%d]", errno);
		return ERR_BIND;
	}

	if(nRet = listen(listenFd, LIS_NUM))
	{
		LOG_ERR("listen err[%d]", errno);
		return ERR_LISTEN;
	}

	return 0;
}

int EventAdd(int epollFd, int events, stEpollEvent *ev)
{
	struct epoll_event epv = {0, {0}};
	int op;
	epv.data.ptr = ev;
	epv.events = ev->events = events;
	if(ev->status == 1){
		op = EPOLL_CTL_MOD;
	}
	else{
		op = EPOLL_CTL_ADD;
		ev->status = 1;
	}
	if(epoll_ctl(epollFd, op, ev->fd, &epv) < 0)
	{
		LOG_ERR("Event Add failed[fd=%d]errno[%d]", ev->fd, errno);
		return ERR_EPOLL_CTL;
	}
	return 0;
}

int EventDel(int epollFd, stEpollEvent *ev)
{
	int nRet;
	struct epoll_event epv = {0, {0}};
	if(ev->status != 1) return SUCESS;
	epv.data.ptr = ev;
	ev->status = 0;
	if(epoll_ctl(epollFd, EPOLL_CTL_DEL, ev->fd, &epv))
	{
		LOG_ERR("Event del failed[fd=%d]errno[%d]", ev->fd, errno);
		return ERR_EPOLL_CTL;
	}
	return 0;
}

void AcceptConn(int fd, int events, void *arg)
{
	struct sockaddr_in sin;
	socklen_t len = sizeof(struct sockaddr_in);
	int nfd, i;
	// accept
	if((nfd = accept(fd, (struct sockaddr*)&sin, &len)) == -1)
	{
		if(errno != EAGAIN && errno != EINTR)
		{
			LOG_ERR("bad accept,Errno[%d]", errno);
		}
		return ;
	}
	do
	{
		for(i = 0; i < MAX_EVENTS; i++)
		{
			if(g_Events[i].status == 0)
			{
				break;
			}
		}
		if(i == MAX_EVENTS)
		{
			LOG_ERR("max connection limit[%d].",MAX_EVENTS);
			break;
		}

		if(fcntl(nfd, F_SETFL, O_NONBLOCK) < 0) break;

		EventSet(&g_Events[i], nfd, RecvData, &g_Events[i]);
		if(EventAdd(g_epollFd, EPOLLIN|EPOLLET, &g_Events[i]))
		{
			return ;
		}
	}while(0);
}

// receive data
void RecvData(int fd, int events, void *arg)
{
	stEpollEvent *ev = (stEpollEvent* )arg;
	int len;
	char szBufLen[BUF_LEN_LEN+1];

	char* szKey = NULL;
	char* szMaxMsgA = NULL;
	char* szMaxLenA = NULL;
	long lKey;

	IpcMsgBuf buf;
	szMaxMsgA = getenv("MAX_MSGA");
	szMaxLenA = getenv("MAX_LEN_MSGA");
	szKey = getenv("MSGA_IPC_KEY");

	LOG_INFO("MAXMSG[%s],MAXLEN[%s] KEY[%s]", szMaxMsgA, szMaxLenA,szKey);
	if(szMaxMsgA == NULL || szMaxLenA == NULL || szKey == NULL)
	{
		LOG_ERR("请检查消息队列参数配置");
		return;
	}

	lKey = atol(szKey);
	key_t msgkey = lKey;

	CIpcMsg cMymsg(atoi(szMaxMsgA), atoi(szMaxLenA));
	cMymsg.IpcCreMsg(msgkey);

	LOG_INFO("最大长度[%d],最大数量[%d]", atol(szMaxLenA), atol(szMaxMsgA));

	int num;
	num = cMymsg.GetNumMsg();
	LOG_INFO("消息队列数量[%d]",num);

	// receive data
	memset(szBufLen, 0x00, sizeof(szBufLen));
	len = recv(fd, szBufLen, BUF_LEN_LEN, 0);  
	if(len <= 0)
	{
		close(ev->fd);
		LOG_ERR("[fd=%d] closed.", fd);
	}

	LOG_INFO("buflen[%s]",  szBufLen);
	len = recv(fd, ev->buff, atoi(szBufLen), 0);  
	EventDel(g_epollFd, ev);
	if(len > 0)
	{
		ev->len = len;
		ev->buff[len] = 0;
		LOG_INFO("fd[%d],recvdata[%s]", fd, ev->buff);
		// change to send event
		EventSet(ev, fd, SendData, ev);
		EventAdd(g_epollFd, EPOLLOUT|EPOLLET, ev);

		memset(&buf, 0x00, sizeof(buf));
		buf.mtype = 1;
		memcpy(buf.mtext, ev->buff, len); 
		cMymsg.IpcSndMsg(buf, strlen(buf.mtext));
	}
	else if(len == 0)
	{
		close(ev->fd);
		LOG_INFO("[fd=%d] closed gracefully.", fd);
	}
	else
	{
		close(ev->fd);
		LOG_ERR("recv[fd=%d] error[%d]:%s", fd, errno, strerror(errno));
	}
	close(fd);
	LOG_INFO("close[fd=%d]", fd);
}

// send data 
void SendData(int fd, int events, void *arg)
{
	stEpollEvent* ev = (stEpollEvent* )arg;
	int len;
	// send data
	len = send(fd, ev->buff, ev->len, 0);
	ev->len = 0;
	EventDel(g_epollFd, ev);
	if(len > 0)
	{
		// change to receive event
		EventSet(ev, fd, RecvData, ev);
		if(EventAdd(g_epollFd, EPOLLIN|EPOLLET, ev))
		{
			return ;
		}
		LOG_INFO("----------fd--------");
	}
	else
	{
		close(ev->fd);
		LOG_ERR("recv[fd=%d] error[%d]", fd, errno);
		return ;
	}
}

/*******************************************************
 * **函数名称: nCSocketRcv                               **
 * **功能描述: 从SOCKET中读取指定长度的内容              **
 * **作    者:                                           **
 * **输入参数:                                           **
 *             vhDataSocket: SOCKET ID                   **
 *             vnRcvFlg: 接收控制标志                    **
 * **输出参数:                                           **
 *             vvpTcpData: 接收数据BUFFER                **
 *             vnpExpDataL: 已接收数据长度               **
 * **返回值:   0/-1                                      **
 * *******************************************************/
int nCSocketRcv(int vhDataSocket, char* vvpTcpData, int&  vnpExpDataL, int vnRcvFlg)
{
	int     len;
	int     lnDataL;
	int     lnTotalDataL;
	char*   lspTcpData;

	lspTcpData   = vvpTcpData;
	lnTotalDataL = 0;

#if 0
	while(vnpExpDataL > lnTotalDataL) {
		if ((lnDataL = read(vhDataSocket,
						lspTcpData,
						vnpExpDataL - lnTotalDataL
						)) <= 0) {
			vnpExpDataL = lnTotalDataL;
			if (lnDataL == 0 || errno == ECONNRESET) 
				return -1;
			else 
			{
				LOG_INFO("errno[%d]",errno);
				return -3;
			}
		}

		lnTotalDataL += lnDataL;
		lspTcpData   += lnDataL;

		if (vnRcvFlg & NCRcvFlgReturnAfterData) {
			vnpExpDataL = lnTotalDataL;
			return 0;
		}
	}
#endif
	len = recv(vhDataSocket, vvpTcpData, vnpExpDataL, vnRcvFlg);  
	if(len <= 0 && errno != EAGAIN)
	{
		close(vhDataSocket);
		LOG_ERR("[fd=%d]ret[%d] errno[%d]", vhDataSocket,len,errno);
		return len;
	}

	return 0;
}

/*******************************************************
 * **函数名称: nCSocketSnd                               **
 * **功能描述: 向SOCKET中发送指定长度的内容              **
 * **作    者:                                           **
 * **输入参数:                                           **
 *             vhDataSocket: SOCKET ID                   **
 *             vvpTcpData: 发送数据BUFFER                **
 *             vnpExpDataL: 希望发送数据的长度           **
 * **输出参数:                                           **
 * **返回值:   0/-1                                      **
 * *******************************************************/

int nCSocketSnd(int vhDataSocket, char*  vvpTcpData, int& vnpExpDataL) 
{
	int     lnDataL;
	int     lnTotalDataL;
	char*   lspTcpData;
	char*   lspTmp;
	int     llTmp;
	static unsigned int llTimeout = LCMaxTimeout + 1;

	if (llTimeout > LCMaxTimeout) {
		llTimeout = 0;
		if ((lspTmp = getenv(SACEnvCommTimeout)) != NULL) {
			if (((llTmp = atoi(lspTmp)) < 0) ||
					(llTmp > LCMaxTimeout)) {
				llTmp = 0;
			}
			llTimeout = llTmp;
		}
	}

	lspTcpData = vvpTcpData;
	lnTotalDataL = 0;

	alarm(llTimeout);
#if 1
	while (vnpExpDataL > lnTotalDataL) {
		if ((lnDataL = send(vhDataSocket,
						lspTcpData,
						vnpExpDataL - lnTotalDataL,
						LCTcpSndFlg)) < 0) {
			llTmp = errno; 
			alarm(0);
			vnpExpDataL = lnTotalDataL;
			return llTmp;
		}
		lnTotalDataL += lnDataL;
		lspTcpData += lnDataL;
	}
#endif
	//send(vhDataSocket,lspTcpData,vnpExpDataL,LCTcpSndFlg);
	alarm(0);
	return 0;
}

/*******************************************************
 * **函数名称: nCCreateSocket                            **
 * **功能描述: 创建TCP/IP,并设定相关SOCKET Options       **
 * **作    者:                                           **
 * **输入参数:                                           **
 * **输出参数:                                           **
 *             vhpSocketId: socket id                    **
 * **返回值:   0/-1                                      **
 * *******************************************************/
int nCCreateSocket(int& vhpSocketId)
{
	//int                    lhListenSocket;
	int                    llOpt;
	int                    rc;
	struct linger          lslngrOpt;

	if ((vhpSocketId = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		return errno;
	} 

	/* turn on socket keep alive option */
	llOpt = 1;
	if (setsockopt(vhpSocketId, SOL_SOCKET, SO_KEEPALIVE, &llOpt, sizeof(llOpt))) {
		rc = errno;
		close(vhpSocketId);
		return rc;
	} /* end of if */

	/* close socket, if socket buff have data, we will wait 1 */
	lslngrOpt.l_onoff = 1;
	lslngrOpt.l_linger = 0;
	if (setsockopt(vhpSocketId, SOL_SOCKET, SO_LINGER, &lslngrOpt, sizeof(lslngrOpt))) {
		rc = errno;
		close(vhpSocketId);
		return rc;
	} /* end of if */

	/* turn on socket reuseaddr option */
	llOpt = 1;
	if (setsockopt(vhpSocketId, SOL_SOCKET, SO_REUSEADDR, &llOpt, sizeof(llOpt))) {
		rc = errno;
		close(vhpSocketId);
		return rc;
	} /* end of if */

	return 0;
}

/*******************************************************
 * **函数名称: nCBind                           **
 * **功能描述: 创建TCP/IP SOCKET并绑定到指定IP和端口上   **
 * **作    者:                                           **
 * **输入参数:                                           **
 *             vnPortNum:端口号                          **
 *             vvpLocAddr: 绑定的本地地址                **
 * **输出参数:                                           **
 *             vhpSocketId: socket id                    **
 * **返回值:   0/-1                                      **
 * *******************************************************/
int nCBind(unsigned int vnPortNum, char* vvpLocAddr, int& vhpSocketId)
{
	int                    rc;
	//int                   *hListenSocket;
	struct sockaddr_in     lsaddrinBind;

	rc = nCCreateSocket(vhpSocketId);
	if (rc)
		return rc;

	lsaddrinBind.sin_family = AF_INET;
	lsaddrinBind.sin_port = htons(vnPortNum);
	lsaddrinBind.sin_addr.s_addr = inet_addr((char*)vvpLocAddr);

	if (bind(vhpSocketId, (struct sockaddr*)(&lsaddrinBind), sizeof(lsaddrinBind))) {
		rc = errno;
		close(vhpSocketId);
		return rc;
	} /* end of if */

	return 0;
} /* end of nCBind */

/*******************************************************
 * **函数名称: nCSetSocketFlag                           **
 * **功能描述: 设置SOCKET标志                            **
 * **作    者:                                           **
 * **输入参数:                                           **
 *             vnSocketId:   Socket ID                   **
 *             vnSocketFlag: 标志位                      **
 * **输出参数:                                           **
 * **返回值:   0/-1                                      **
 * *******************************************************/
int nCSetSocketFlag(int vnSocketId, int vnSocketFlag)
{
	int flags, rc;

	/* get fd flags */
	flags = fcntl(vnSocketId, F_GETFL, 0);
	if (flags == -1)
		return errno;

	/* set fd flags */
	rc = fcntl(vnSocketId, F_SETFL, flags|vnSocketFlag);
	if (rc == -1)
		return errno;

	return 0;
}

/*******************************************************
 * **函数名称: nCClearSocketFlag                         **
 * **功能描述: 清空SOCKET标志                            **
 * **作    者:                                           **
 * **输入参数:                                           **
 *             vnSocketId:   Socket ID                   **
 *             vnSocketFlag: 标志位                      **
 * **输出参数:                                           **
 * **返回值:   0/-1                                      **
 * *******************************************************/
int nCClearSocketFlag(int vnSocketId, int vnSocketFlag)
{
	int flags, rc;

	/* get fd flags */
	flags = fcntl(vnSocketId, F_GETFL, 0);
	if (flags == -1)
		return errno;

	/* set fd flags */
	rc = fcntl(vnSocketId, F_SETFL, flags & (~vnSocketFlag));
	if (rc == -1)
		return errno;

	return 0;
}

/*******************************************************
 * **函数名称: nCNBlockConnect                  **
 * **功能描述: 创建TCP/IP SOCKET并以非阻塞方式           **
 *             连接到指定IP和端口上                      **
 * **作    者:                                           **
 * **输入参数:                                           **
 *             vnPortNum:端口号                          **
 *             vvpRemAddr: 连接的远端IP地址              **
 * **输出参数:                                           **
 *             vhpSocketId: socket id                    **
 * **返回值:   0/-1                                      **
 * *******************************************************/
int nCNBlockConnect(unsigned int vnPortNum, char* vvpRemAddr, int& vhpSocketId)
{
	int                    rc;
	//int                    lhConnectSocket;
	struct sockaddr_in     lsaddrinRemote;

	rc = nCCreateSocket(vhpSocketId);
	if (rc)
		return rc;
	LOG_INFO("create socket sucess");

	/* set the socket to NBLOCK */
	rc = nCSetSocketFlag(vhpSocketId, O_NONBLOCK);
	if (rc) {
		close(vhpSocketId);
		return rc;
	}

	memset(&lsaddrinRemote, 0, sizeof(lsaddrinRemote));
	lsaddrinRemote.sin_port = htons(vnPortNum);
	lsaddrinRemote.sin_family = AF_INET;
	lsaddrinRemote.sin_addr.s_addr = inet_addr(vvpRemAddr);

	rc = connect(vhpSocketId, (struct sockaddr*)(&lsaddrinRemote),
			sizeof(lsaddrinRemote));
	if (rc < 0 && errno != EINPROGRESS) {
		close(vhpSocketId);
		return errno;
	}

	//vhpSocketId = lhConnectSocket;

	return 0;
}

/*******************************************************
 * **函数名称: nCConnect                                 **
 * **功能描述: 创建TCP/IP SOCKET并连接到指定IP和端口上   **
 * **作    者:                                           **
 * **输入参数:                                           **
 *             vnPortNum:端口号                          **
 *             vvpLocAddr: 连接的远端IP地址              **
 * **输出参数:                                           **
 *             vhpSocketId: socket id                    **
 * **返回值:   0/-1                                      **
 * *******************************************************/
int nCConnect(unsigned int vnPortNum, char* vvpRemAddr, int& vhpSocketId)
{
	int                    rc;
	//int                    lhConnectSocket;
	struct sockaddr_in     lsaddrinRemote;

	rc = nCCreateSocket(vhpSocketId);
	if (rc)
		return rc;

	memset(&lsaddrinRemote, 0, sizeof(lsaddrinRemote));
	lsaddrinRemote.sin_port = htons(vnPortNum);
	lsaddrinRemote.sin_family = AF_INET;
	lsaddrinRemote.sin_addr.s_addr = inet_addr(vvpRemAddr);

	if (connect(vhpSocketId, (struct sockaddr*)(&lsaddrinRemote), sizeof(lsaddrinRemote))) {
		rc = errno;
		close(vhpSocketId);
		return rc;
	} /* end of if */

	//*vhpSocketId = lhConnectSocket;
	return 0;
}

/*******************************************************
 * **函数名称: nCListenPort                              **
 * **功能描述: 在指定的SOCKET上监听                      **
 * **作    者:                                           **
 * **输入参数:                                           **
 *             nListenSocket:监听SOCKET ID               **
 *             nListenQueueLen:连接需要的队列长度        **
 *  *输出参数:                                           **
 *  *返回值:   0/-1                                      **
 * *******************************************************/
int nCListenPort(int nListenSocket, int nListenQueueLen)
{
	int rc;

	rc = listen(nListenSocket, nListenQueueLen);
	if (rc)
		return errno;

	return 0;
}

/*******************************************************
 * **函数名称: nCListenAccept                            **
 * **功能描述: accept 新的连接请求                       **
 * **作    者:                                           **
 * **输入参数:                                           **
 *             nListenSocket:监听SOCKET ID               **
 * **输出参数:                                           **
 *             pnAcceptSocket:新的连接SOCKET ID          **
 *             pRemoteSocket:新连接远端SOCKET信息        **
 * **返回值:   0/-1                                      **
 * *******************************************************/
int nCListenAccept(int nListenSocket, int& nAcceptSocket, struct sockaddr_in *pRemoteSocket)
{
	int rc;
	//int nAcceptSocket;
	struct sockaddr_in   Client;
	unsigned int Client_len = sizeof(Client);

	nAcceptSocket = accept(nListenSocket, (struct sockaddr *)&Client, &Client_len);
	if (nAcceptSocket < 0)
		return errno;

	//*pnAcceptSocket = nAcceptSocket;
	memcpy(pRemoteSocket, &Client, Client_len);
	return 0;
}


/*******************************************************libevent start **********************************************************************************/
Conn::Conn(int fd) : m_fd(fd)
{
	m_Prev = NULL;
	m_Next = NULL;
}

Conn::~Conn()
{

}

ConnQueue::ConnQueue()
{
	//建立头尾结点，并调整其指针
	m_head = new Conn(0);
	m_tail = new Conn(0);
	m_head->m_Prev = m_tail->m_Next = NULL;
	m_head->m_Next = m_tail;
	m_tail->m_Prev = m_head;
}

ConnQueue::~ConnQueue()
{
	Conn *tcur, *tnext;
	tcur = m_head;
	//循环删除链表中的各个结点
	while( tcur != NULL )
	{
		tnext = tcur->m_Next;
		delete tcur;
		tcur = tnext;
	}
}

//插入到第二的位置
Conn *ConnQueue::InsertConn(int fd, LibeventThread *t)
{
	Conn *c = new Conn(fd);
	c->m_Thread = t;
	Conn *next = m_head->m_Next;

	c->m_Prev = m_head;
	c->m_Next = m_head->m_Next;
	m_head->m_Next = c;
	next->m_Prev = c;
	return c;
}

void ConnQueue::DeleteConn(Conn *c)
{
	c->m_Prev->m_Next = c->m_Next;
	c->m_Next->m_Prev = c->m_Prev;
	delete c;
}

void ConnQueue::PrintQueue()
{
	Conn *cur = m_head->m_Next;
	while( cur->m_Next != NULL )
	{
		LOG_INFO("CurFd[%d]", cur->m_fd);
		cur = cur->m_Next;
	}
}

TcpEventServer::TcpEventServer(int count)
{
	//初始化各项数据
	m_ThreadCount = count;
	m_Port = -1;
	m_MainBase = new LibeventThread;
	m_Threads = new LibeventThread[m_ThreadCount];
	m_MainBase->tid = pthread_self();
	m_MainBase->base = event_base_new();

	//初始化各个子线程的结构体
	for(int i=0; i<m_ThreadCount; i++)
	{
		SetupThread(&m_Threads[i]);
	}

}

TcpEventServer::~TcpEventServer()
{
	//停止事件循环（如果事件循环没开始，则没效果）
	StopRun(NULL);

	//释放内存
	event_base_free(m_MainBase->base);
	for(int i=0; i<m_ThreadCount; i++)
		event_base_free(m_Threads[i].base);

	delete m_MainBase;
	delete [] m_Threads;
}

void TcpEventServer::ErrorQuit(const char *str)
{
	//输出错误信息，退出程序
	LOG_ERR("Err[%s]", str);
	if( errno != 0 )    
		LOG_ERR("Err[%s]",  strerror(errno));
	exit(1);    
}

void TcpEventServer::SetupThread(LibeventThread *me)
{
	//建立libevent事件处理机制
	me->tcpConnect = this;
	me->base = event_base_new();
	if( NULL == me->base )
		ErrorQuit("event base new error");

	//在主线程和子线程之间建立管道
	int fds[2];
	if( pipe(fds) )
		ErrorQuit("create pipe error");
	me->notifyReceiveFd = fds[0];
	me->notifySendFd = fds[1];

	//让子线程的状态机监听管道
	event_set( &me->notifyEvent, me->notifyReceiveFd,
			EV_READ | EV_PERSIST, ThreadProcess, me );
	event_base_set(me->base, &me->notifyEvent);
	if ( event_add(&me->notifyEvent, 0) == -1 )
		ErrorQuit("Can't monitor libevent notify pipe\n");
}

void *TcpEventServer::WorkerLibevent(void *arg)
{
	//开启libevent的事件循环，准备处理业务
	LibeventThread *me = (LibeventThread*)arg;
	LOG_INFO("thread %u started", (unsigned int)me->tid);
	event_base_dispatch(me->base);
	LOG_INFO("subthread done");
}

bool TcpEventServer::StartRun()
{
	evconnlistener *listener;

	//如果端口号不是EXIT_CODE，就监听该端口号
	if( m_Port != EXIT_CODE )
	{
		sockaddr_in sin;
		memset(&sin, 0, sizeof(sin));
		sin.sin_family = AF_INET;
		sin.sin_port = htons(m_Port);
		listener = evconnlistener_new_bind(m_MainBase->base, 
				ListenerEventCb, (void*)this,
				LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
				(sockaddr*)&sin, sizeof(sockaddr_in));
		if( NULL == listener )
			ErrorQuit("TCP listen error");
	}

	//开启各个子线程
	for(int i=0; i<m_ThreadCount; i++)
	{
		pthread_create(&m_Threads[i].tid, NULL,  
				WorkerLibevent, (void*)&m_Threads[i]);
	}

	//开启主线程的事件循环
	event_base_dispatch(m_MainBase->base);

	//事件循环结果，释放监听者的内存
	if( m_Port != EXIT_CODE )
	{
		LOG_INFO("free listen");
		evconnlistener_free(listener);
	}
}

void TcpEventServer::StopRun(timeval *tv)
{
	int contant = EXIT_CODE;
	//向各个子线程的管理中写入EXIT_CODE，通知它们退出
	for(int i=0; i<m_ThreadCount; i++)
	{
		write(m_Threads[i].notifySendFd, &contant, sizeof(int));
	}
	//结果主线程的事件循环
	event_base_loopexit(m_MainBase->base, tv);
}

void TcpEventServer::ListenerEventCb(struct evconnlistener *listener, 
		evutil_socket_t fd,
		struct sockaddr *sa, 
		int socklen, 
		void *user_data)
{
	TcpEventServer *server = (TcpEventServer*)user_data;

	//随机选择一个子线程，通过管道向其传递socket描述符
	int num = rand() % server->m_ThreadCount;
	int sendfd = server->m_Threads[num].notifySendFd;
	write(sendfd, &fd, sizeof(evutil_socket_t));
}

void TcpEventServer::ThreadProcess(int fd, short which, void *arg)
{
	LibeventThread *me = (LibeventThread*)arg;

	//从管道中读取数据（socket的描述符或操作码）
	int pipefd = me->notifyReceiveFd;
	evutil_socket_t confd;
	read(pipefd, &confd, sizeof(evutil_socket_t));

	//如果操作码是EXIT_CODE，则终于事件循环
	if( EXIT_CODE == confd )
	{
		event_base_loopbreak(me->base);
		return;
	}

	//新建连接
	struct bufferevent *bev;
	bev = bufferevent_socket_new(me->base, confd, BEV_OPT_CLOSE_ON_FREE);
	if (!bev)
	{
		LOG_ERR("Error constructing bufferevent!");
		event_base_loopbreak(me->base);
		return;
	}

	//将该链接放入队列
	Conn *conn = me->connectQueue.InsertConn(confd, me);

	//准备从socket中读写数据
	bufferevent_setcb(bev, ReadEventCb, WriteEventCb, CloseEventCb, conn);
	bufferevent_enable(bev, EV_WRITE);
	bufferevent_enable(bev, EV_READ);

	//调用用户自定义的连接事件处理函数
	me->tcpConnect->ConnectionEvent(conn);
}

void TcpEventServer::ReadEventCb(struct bufferevent *bev, void *data)
{
	Conn *conn = (Conn*)data;
	conn->m_ReadBuf = bufferevent_get_input(bev);
	conn->m_WriteBuf = bufferevent_get_output(bev);

	//调用用户自定义的读取事件处理函数
	conn->m_Thread->tcpConnect->ReadEvent(conn);
} 

void TcpEventServer::WriteEventCb(struct bufferevent *bev, void *data)
{
	Conn *conn = (Conn*)data;
	conn->m_ReadBuf = bufferevent_get_input(bev);
	conn->m_WriteBuf = bufferevent_get_output(bev);

	//调用用户自定义的写入事件处理函数
	conn->m_Thread->tcpConnect->WriteEvent(conn);

}

void TcpEventServer::CloseEventCb(struct bufferevent *bev, short events, void *data)
{
	Conn *conn = (Conn*)data;
	//调用用户自定义的断开事件处理函数
	conn->m_Thread->tcpConnect->CloseEvent(conn, events);
	conn->GetThread()->connectQueue.DeleteConn(conn);
	bufferevent_free(bev);
}

bool TcpEventServer::AddSignalEvent(int sig, void (*ptr)(int, short, void*))
{
	//新建一个信号事件
	event *ev = evsignal_new(m_MainBase->base, sig, ptr, (void*)this);
	if ( !ev || 
			event_add(ev, NULL) < 0 )
	{
		event_del(ev);
		return false;
	}

	//删除旧的信号事件（同一个信号只能有一个信号事件）
	DeleteSignalEvent(sig);
	m_SignalEvents[sig] = ev;

	return true;
}

bool TcpEventServer::DeleteSignalEvent(int sig)
{
	map<int, event*>::iterator iter = m_SignalEvents.find(sig);
	if( iter == m_SignalEvents.end() )
		return false;

	event_del(iter->second);
	m_SignalEvents.erase(iter);
	return true;
}

event *TcpEventServer::AddTimerEvent(void (*ptr)(int, short, void *), 
		timeval tv, bool once)
{
	int flag = 0;
	if( !once )
		flag = EV_PERSIST;

	//新建定时器信号事件
	event *ev = new event;
	event_assign(ev, m_MainBase->base, -1, flag, ptr, (void*)this);
	if( event_add(ev, &tv) < 0 )
	{
		event_del(ev);
		return NULL;
	}
	return ev;
}

bool TcpEventServer::DeleteTImerEvent(event *ev)
{
	int res = event_del(ev);
	return (0 == res);
} 

/*******************************************************libevent end ************************************************************************************/
