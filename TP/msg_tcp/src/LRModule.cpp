#include <arpa/inet.h>
#include "WokerModule.h"
#include "LRModule.h"
#include "MemPoolMgr.h"
#include "LogonModule.h"
#include "HistDataModule.h"

#include "Group_Spread.h"



/*
// @return 
   -1: 错误
    0: 未读完
    1: 读完一个包
*/
int ReadData(STDataNode *pDataNode)
{
	int readlen = 0 ;
	int headerlen = sizeof(STMsgHeader) ;

	if(pDataNode->nDataLen < headerlen)
	{
		readlen = recvforlen(pDataNode->objConn.socket,pDataNode->szData + pDataNode->nDataLen, headerlen-pDataNode->nDataLen );
		if (readlen == -1 )return -1;
		if( readlen == 0)return 0;
		pDataNode->nDataLen += readlen;

		if(pDataNode->nDataLen != headerlen ) return 0;
	}

	
	pDataNode->nMsgLen = ntohs(*((unsigned short *)(pDataNode->szData+4)));

	int msgid =  ntohl(*((unsigned int *)(pDataNode->szData+8)));
	if(msgid == 0)
	{
		printf("ReadData msgid is 0 , error\n");
		return -1;
	}
	
	XINFO("ReadData readlen:%d,msglen:%d\n ", pDataNode->nDataLen,pDataNode->nMsgLen );

	
	readlen = recvforlen(pDataNode->objConn.socket,pDataNode->szData+pDataNode->nDataLen, pDataNode->nMsgLen-pDataNode->nDataLen);
	if (readlen == -1 ) return -1;
	
	pDataNode->nDataLen += readlen;
	if(pDataNode->nDataLen != pDataNode->nMsgLen)
		return 0;

	return 1;
	
}






void ReadCallback(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
	STConnNode *pConnNode  = (STConnNode * )watcher->conn_node;

	XINFO("ReadCallback .................\n");

	if(EV_ERROR & revents)  
	{  
		XERROR("ReadCallback error : client ip:%s, client port:%d, socket : %d\n",
					iptostrip(htonl(pConnNode->ip)), pConnNode->port,watcher->fd);
		ev_io_stop(loop,watcher);  
		close(watcher->fd); 
		if(pConnNode->pDataNode!=NULL)CMemPoolMgr::GetInstance()->PushDataNode(pConnNode->pDataNode);
		CMemPoolMgr::GetInstance()->PushConnNode(pConnNode);
		return ; 
	}  


	if(NULL == pConnNode->pDataNode )
	{
		STDataNode *pDataNode = CMemPoolMgr::GetInstance()->GetDataNode();
		if( NULL == pDataNode )
		{
			XERROR("GetDataNode mem node is zero , close client socket:%d\n",watcher->fd); 
			ev_io_stop(loop,watcher);  
			close(watcher->fd); 
			CMemPoolMgr::GetInstance()->PushConnNode(pConnNode);
			return ;
		}

		pConnNode->loop = loop;
		pConnNode->watcher = watcher;
		
		pConnNode->pDataNode = pDataNode;		
		pDataNode->pConn = pConnNode;
		pDataNode->objConn = *pConnNode;
	}
	
	XINFO("ReadCallback : client ip:%s, client port:%d,socket : %d\n",
				iptostrip(htonl(pConnNode->pDataNode->objConn.ip)), pConnNode->pDataNode->objConn.port,watcher->fd);
		
	int ret = ReadData(pConnNode->pDataNode);
	if( ret == -1)
	{
			XERROR("ReadCallback read error : client ip:%s, client port:%d, socket : %d\n",
					iptostrip(htonl(pConnNode->pDataNode->objConn.ip)), pConnNode->pDataNode->objConn.port,watcher->fd);

			ev_io_stop(loop,watcher);
        	close(watcher->fd);
			CMemPoolMgr::GetInstance()->PushDataNode(pConnNode->pDataNode);
			CMemPoolMgr::GetInstance()->PushConnNode(pConnNode);
			return ;
	}

	if ( ret == 0) return ;
	
	STMsgHeader *pHeader = (STMsgHeader *)pConnNode->pDataNode->szData;

	unsigned int msgid = ntohl(pHeader->msgid) ;

	
	
	if( LOGIN_REQUEST == msgid)
	{
		
		XINFO("LRModule Recv logon request,requestid:%s, client ip:%s, client port:%d,socket : %d\n",pHeader->requestid,
					iptostrip(htonl(pConnNode->pDataNode->objConn.ip)), pConnNode->pDataNode->objConn.port,watcher->fd);
		
		CLogonProcee::LRModuleProcessLogonRequest(pConnNode->pDataNode);
	}
	else if( RTMSG_REQUEST == msgid )
	{
		//if(1 == pConnNode->logon)
		{
			
			XINFO("LRModule Recv Realtime request,requestid:%s, client ip:%s, client port:%d,socket : %d\n",pHeader->requestid,
					iptostrip(htonl(pConnNode->pDataNode->objConn.ip)), pConnNode->pDataNode->objConn.port,watcher->fd);
			
			CWorkerProcee::LRModuleProcessRequest(pConnNode->pDataNode);
		}/*
		else
		{
			XERROR("ReadCallback read:: unlogon , cann't request realtime data:%d : client ip:%s, client port:%d,socket : %d\n",
					iptostrip(htonl(pConnNode->pDataNode->objConn.ip)), pConnNode->pDataNode->objConn.port,watcher->fd);
		
			ev_io_stop(loop,watcher);
	        close(watcher->fd);
			CMemPoolMgr::GetInstance()->PushDataNode(pConnNode->pDataNode);
			CMemPoolMgr::GetInstance()->PushConnNode(pConnNode);
			return ;
		}*/
	}
	else if( HISTMSG_REQUEST == msgid || HISTMSG_REQUEST_SMALLPACK == msgid )
	{
		//if(1 == pConnNode->logon)
		{

			
			XINFO("LRModule Recv Historical request,requestid:%s, client ip:%s, client port:%d,socket : %d\n",pHeader->requestid,
					iptostrip(htonl(pConnNode->pDataNode->objConn.ip)), pConnNode->pDataNode->objConn.port,watcher->fd);
			
			CHistDataProcess::LRModuleProcessHistDataRequest(pConnNode->pDataNode);
		}/*
		else
		{
			XERROR("ReadCallback read:: unlogon , cann't request history data:: client ip:%s, client port:%d,socket : %d\n",
					iptostrip(htonl(pConnNode->pDataNode->objConn.ip)), pConnNode->pDataNode->objConn.port,watcher->fd);
		
			ev_io_stop(loop,watcher);
	        close(watcher->fd);
			CMemPoolMgr::GetInstance()->PushDataNode(pConnNode->pDataNode);
			CMemPoolMgr::GetInstance()->PushConnNode(pConnNode);
			return ;
		}*/
		
	}
	else
	{
		XERROR("ReadCallback read:: msgid error:%d : client ip:%s, client port:%d,socket : %d\n",
					msgid,iptostrip(htonl(pConnNode->pDataNode->objConn.ip)), pConnNode->pDataNode->objConn.port,watcher->fd);
		
		ev_io_stop(loop,watcher);
        close(watcher->fd);
		CMemPoolMgr::GetInstance()->PushDataNode(pConnNode->pDataNode);
		CMemPoolMgr::GetInstance()->PushConnNode(pConnNode);
		return ;
	}

	pConnNode->pDataNode = NULL;
	
}









void ListenAccept(struct ev_loop *loop, struct ev_io *watcher, int revents)  
{  

	XINFO("\n\n\nListenAccept ................\n");
	if(EV_ERROR & revents)  
	{  
	  XERROR("error event in accept ");  
	  exit(-1);  
	}  
	
	
	struct sockaddr_in ClientAddr;
	int nClientSocket = -1;
	socklen_t ClientLen = sizeof(ClientAddr);
		
	nClientSocket = accept(watcher->fd,(struct sockaddr *)&ClientAddr, &ClientLen);
	if(-1==nClientSocket)
	{
		XERROR("accet nClientSocket:%s\n",strerror(errno));
		exit(-1);  
	}
		
	if ( 0 != setnonblock(nClientSocket) )
	{
		XERROR("failed to set client socket socket to non-blocking,socket:%d\n",nClientSocket); 
		close(nClientSocket);
		exit(-1);  
	}
	
	STConnNode *pConn= CMemPoolMgr::GetInstance()->GetConnNode();
	if (NULL == pConn)
	{
		XERROR("GetConnNode mem node is zero , close client socket:%d\n",nClientSocket); 
		close(nClientSocket);
		exit(-1);  
	}
	gettimeofday(&(pConn->conntime),NULL);
	pConn->ip = ntohl(ClientAddr.sin_addr.s_addr);
	pConn->port = ntohs(ClientAddr.sin_port);
	pConn->socket = nClientSocket;
	pConn->pClientEvent->conn_node = (void *)pConn;



	ev_io_init(pConn->pClientEvent, ReadCallback, nClientSocket, EV_READ);  
	ev_io_start(loop, pConn->pClientEvent);  


	XINFO("a client connect :socket:%d,ip:%s,port:%d\n",nClientSocket,inet_ntoa(ClientAddr.sin_addr),ntohs(ClientAddr.sin_port));	
		
	
}  





int StartListenModule(int nServicePort)
{
	int nSocket=-1;
	int nRet = -1;
	
	nSocket = socket(PF_INET,SOCK_STREAM,0);
	if(-1==nSocket) // 
	{
		XERROR("socket error:%s\n",strerror(errno));
		return -1;
	}
	
	int value = 1;
	setsockopt(nSocket,SOL_SOCKET,SO_REUSEADDR,&value,sizeof(value));
	
	
	struct sockaddr_in ServerAddr;
	ServerAddr.sin_family = PF_INET;
	ServerAddr.sin_port = htons(nServicePort);
	ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	nRet = bind(nSocket,(struct sockaddr *)&ServerAddr, (socklen_t)sizeof(ServerAddr));
	if(-1==nRet)
	{
		XERROR("bind error:%s\n",strerror(errno));
		return -1;
	}
	nRet = listen(nSocket,1000);
	if(-1==nRet)
	{
		XERROR("listen error:%s\n",strerror(errno));
		return -1;
	}
	
		
	if ( 0 != setnonblock(nSocket) )
	{
		XERROR("failed to set listen socket socket to non-blocking"); 
		return -1;
	}

	  
	struct ev_loop *loop = ev_default_loop(0);    
	struct ev_io socket_accept;   
	ev_io_init(&socket_accept, ListenAccept, nSocket, EV_READ);  
	ev_io_start(loop, &socket_accept);  
	    
	while (1)  
	{  
	  ev_loop(loop, 0);  
	}  
	  	

}






