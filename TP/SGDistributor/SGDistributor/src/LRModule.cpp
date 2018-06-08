

#include <arpa/inet.h>
#include "WokerModule.h"
#include "LRModule.h"
#include "InService.h"
#include "MemPoolMgr.h"
#include "WorkeDataListMgr.h"



CBaseLRModule::CBaseLRModule(int nPort)
{
	m_nPort = nPort;
}


void CBaseLRModule::Run()
{
	Init();
}








/*
// @return 
   -1: 错误
    0: 未读完
    1: 读完一个包
*/
int CBaseLRModule::ReadData(STDataNode *pDataNode)
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

	STMsgHeader *header = (STMsgHeader *)pDataNode->szData;
	
	pDataNode->nMsgLen = ntohs(header->msglen);

	/*
	int msgid = ntohl(header->msgid);
	if(msgid == 0)
	{
		XERROR("ReadData msgid is 0 , error\n");
		return -1;
	}
	XINFO("ReadData readlen:%d,msglen:%d,msgid:0x%04x\n ", pDataNode->nDataLen,pDataNode->nMsgLen, msgid);
	*/

	
	readlen = recvforlen(pDataNode->objConn.socket,pDataNode->szData+pDataNode->nDataLen, pDataNode->nMsgLen-pDataNode->nDataLen);
	if (readlen == -1 ) return -1;
	
	pDataNode->nDataLen += readlen;
	if(pDataNode->nDataLen != pDataNode->nMsgLen)
		return 0;

	return 1;
	
}




// 细节后期再处理
void CBaseLRModule::ReadCallback(int sock,short event,void* arg)
{
		STConnNode *pConnNode  = (STConnNode * )arg;
	
			
		if (EV_TIMEOUT & event) 
		{
			XERROR("ReadCallback timeout: client ip:%s, client port:%d, socket : %d\n",
					iptostrip(htonl(pConnNode->ip)), pConnNode->port,sock);
			event_del(pConnNode->pClientEvent);
			close(sock); 
			if(1!=pConnNode->isOutService)CWorkerProcee::LRModuleDeleteGWMsg(pConnNode);
			if(pConnNode->pDataNode != NULL)CMemPoolMgr::GetInstance()->PushDataNode(pConnNode->pDataNode);
			CMemPoolMgr::GetInstance()->PushConnNode(pConnNode);
			return ;
		}
	
	
		// 短连接处理
		if(1 == pConnNode->sendflag)
		{
			XERROR("The outclient sends the request many times or closed ! \n");
			event_del(pConnNode->pClientEvent);
			close(sock); 
			if(pConnNode->pDataNode != NULL)CMemPoolMgr::GetInstance()->PushDataNode(pConnNode->pDataNode);
			CMemPoolMgr::GetInstance()->PushConnNode(pConnNode);
			return ;
		}
		
	
		if(NULL == pConnNode->pDataNode )
		{
			STDataNode *pDataNode = CMemPoolMgr::GetInstance()->GetDataNode();
			if( NULL == pDataNode )
			{
				XERROR("GetDataNode mem node is zero , close client socket:%d\n",sock); 
				event_del(pConnNode->pClientEvent);
				close(sock); 
				if(1!=pConnNode->isOutService)CWorkerProcee::LRModuleDeleteGWMsg(pConnNode);
				CMemPoolMgr::GetInstance()->PushConnNode(pConnNode);
				return ;
			}
			
			pConnNode->pDataNode = pDataNode;
			pDataNode->nReadtimes = 0;
			pDataNode->pConn = pConnNode;
			pDataNode->objConn = *pConnNode;
		}
	
		STDataNode *pDataNode =  pConnNode->pDataNode;
	
	
		/*
		XINFO("ReadCallback : client ip:%s, client port:%d,conn addr:%p, data addr:%p,socket : %d\n",
					iptostrip(htonl(pDataNode->objConn.ip)), pDataNode->objConn.port,
					pConnNode,pDataNode,sock);
		*/

		
		int ret = ReadData(pDataNode);
		if( ret == -1)
		{
			XERROR("ReadCallback read error : client ip:%s, client port:%d,socket : %d\n",
					iptostrip(htonl(pDataNode->objConn.ip)), pDataNode->objConn.port,sock);
			
			event_del(pConnNode->pClientEvent);
			close(sock); 
			if(1!=pConnNode->isOutService)CWorkerProcee::LRModuleDeleteGWMsg(pConnNode);
			CMemPoolMgr::GetInstance()->PushDataNode(pDataNode);
			CMemPoolMgr::GetInstance()->PushConnNode(pConnNode);
			return ;
		}


		
		STMsgHeader *pHeader = (STMsgHeader *)pConnNode->pDataNode->szData;
		unsigned int msgid = ntohl(pHeader->msgid) ;
		
		if(0 == ret)
		{
			pDataNode->nReadtimes ++;
			if(READTIMES > pDataNode->nReadtimes)return ;

			XINFO("ReadCallback more time read package error: client ip:%s, client port:%d, \
						starter:%d,msglen:%d,readlen:%d,version:%d,msgid:0x%x\n",
						iptostrip(htonl(pDataNode->objConn.ip)), pDataNode->objConn.port, 
						ntohl(pHeader->starter), ntohs(pHeader->msglen),
						pDataNode->nDataLen, ntohs(pHeader->version),msgid);
			
			event_del(pConnNode->pClientEvent);
			close(sock); 
			if(1!=pConnNode->isOutService)CWorkerProcee::LRModuleDeleteGWMsg(pConnNode);
			CMemPoolMgr::GetInstance()->PushDataNode(pDataNode);
			CMemPoolMgr::GetInstance()->PushConnNode(pConnNode);
				
			return ;
		}
		
		/*
		XINFO("ReadCallback read package:msglen:%d,readdatalen:%d, client ip:%s, client port:%d, retlen:%d\n",
			pDataNode->nMsgLen,pDataNode->nDataLen,iptostrip(htonl(pDataNode->objConn.ip)), pDataNode->objConn.port,ret);
		*/
		
		
		if( msgid == MSG_OUTCLIENT_GW_REQ )
		{
			pConnNode->isOutService = 1;
			pConnNode->sendflag = 1;
			CWorkDataListMgr::GetInstance()->PushDataNode(pDataNode);
		}
		else
		{
			CInserviceWork::GetInstance()->PushDataNode(pDataNode);
		}
		
		
		pConnNode->pDataNode = NULL;
		
		return ;
}





void CBaseLRModule::ListenAccept(int listensock,short event,void* arg)
{
		struct event_base *pEventMgr = (struct event_base *)arg;
		assert(pEventMgr!=NULL);
		
		XINFO("\n\n\nListenAccept ................\n");
	
		struct sockaddr_in ClientAddr;
		int nClientSocket = -1;
		socklen_t ClientLen = sizeof(ClientAddr);
		
		nClientSocket = accept(listensock,(struct sockaddr *)&ClientAddr, &ClientLen);
		if(-1==nClientSocket)
		{
			XERROR("accet nClientSocket:%s\n",strerror(errno));
			return;
		}
		
		if ( 0 != setnonblock(nClientSocket) )
		{
			XERROR("failed to set client socket socket to non-blocking,socket:%d\n",nClientSocket); 
			close(nClientSocket);
			return ;
		}
	
		STConnNode *pConn= CMemPoolMgr::GetInstance()->GetConnNode();
		if (NULL == pConn)
		{
			XERROR("GetConnNode mem node is zero , close client socket:%d\n",nClientSocket); 
			close(nClientSocket);
			return ;
		}
		gettimeofday(&(pConn->conntime),NULL);
		pConn->ip = ntohl(ClientAddr.sin_addr.s_addr);
		pConn->port = ntohs(ClientAddr.sin_port);
		pConn->socket = nClientSocket;
		
		
		
		struct timeval tv;
		evutil_timerclear(&tv);
		tv.tv_sec = OUTCLIENTTIMEOUT;
			
		event_assign(pConn->pClientEvent,pEventMgr,nClientSocket,EV_TIMEOUT|EV_READ|EV_PERSIST,ReadCallback,pConn);
		event_add(pConn->pClientEvent,&tv);
		
		XINFO("a client connect :socket:%d,ip:%s,port:%d\n",nClientSocket,inet_ntoa(ClientAddr.sin_addr),ntohs(ClientAddr.sin_port));	
		
		return ;
}





int CBaseLRModule::Init()
{
	
		// 1. 初始化socket	
		int nSocket=-1;
		int nRet = -1;
	
		nSocket = socket(PF_INET,SOCK_STREAM,0);
		if(-1==nSocket) // 
		{
			XINFO("socket error:%s\n",strerror(errno));
			g_ExitFlag = 1;
			return -1;
		}
	
		int value = 1;
		setsockopt(nSocket,SOL_SOCKET,SO_REUSEADDR,&value,sizeof(value));
	
	
		struct sockaddr_in ServerAddr;
		ServerAddr.sin_family = PF_INET;
		ServerAddr.sin_port = htons(m_nPort);
		ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
		nRet = bind(nSocket,(struct sockaddr *)&ServerAddr, (socklen_t)sizeof(ServerAddr));
		if(-1==nRet)
		{
			XINFO("bind error:%s\n",strerror(errno));
			g_ExitFlag = 1;
			return -1;
		}
	
	
		//int listen(int sockfd, int backlog);
		nRet = listen(nSocket,1000);
		if(-1==nRet)
		{
			XINFO("listen error:%s\n",strerror(errno));
			g_ExitFlag = 1;
			return -1;
		}
	
		
		 if ( 0 != setnonblock(nSocket) )
		 {
			XINFO("failed to set listen socket socket to non-blocking"); 
			g_ExitFlag = 1;
			return -1;
		 }
			
	
		//2. 开始初始化libevent
		struct event_base *pEventMgr = NULL;
		pEventMgr = event_base_new();
		if(NULL==pEventMgr)
		{
			XINFO("event_base_new error:%s\n",strerror(errno));
			g_ExitFlag = 1;
			return -1;
		}
	
		
		struct event ListenEvent;
		if(-1==event_assign(&ListenEvent,pEventMgr,nSocket,EV_READ|EV_PERSIST,ListenAccept,pEventMgr))
		{
			XINFO("event_assign error:%s\n",strerror(errno));
			g_ExitFlag = 1;
			return -1;
		}
	
		
		if(-1==event_add(&ListenEvent,NULL))
		{
			XINFO("event_add error:%s\n",strerror(errno));
			g_ExitFlag = 1;
			return -1;
		}
	
		XINFO("server port:%d begin listen ...\n",m_nPort);
	
		
		if(-1==event_base_dispatch(pEventMgr))
		{
			XINFO("event_base_dispatch error:%s\n",strerror(errno));
			g_ExitFlag = 1;
			return -1;
		}
	
		
		
	
		return 0;
}




int  StartInService()
{
	/*
	// 1. 初始化socket	
	int nSocket=-1;
	int nRet = -1;
	
	nSocket = socket(PF_INET,SOCK_STREAM,0);
	if(-1==nSocket) // 
	{
		XINFO("socket error:%s\n",strerror(errno));
		g_ExitFlag = 1;
		return -1;
	}
	
	int value = 1;
	setsockopt(nSocket,SOL_SOCKET,SO_REUSEADDR,&value,sizeof(value));
	
	
	struct sockaddr_in ServerAddr;
	ServerAddr.sin_family = PF_INET;
	ServerAddr.sin_port = htons(stConfinfo.nInServicePort);
	ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	nRet = bind(nSocket,(struct sockaddr *)&ServerAddr, (socklen_t)sizeof(ServerAddr));
	if(-1==nRet)
	{
		XINFO("bind inservice error:%s\n",strerror(errno));
		g_ExitFlag = 1;
		return -1;
	}
	
	
	
	nRet = listen(nSocket,1000);
	if(-1==nRet)
	{
		XINFO("listen inservice error:%s\n",strerror(errno));
		g_ExitFlag = 1;
		return -1;
	}
	
	
	 if ( 0 != setnonblock(nSocket) )
	 {
		XINFO("inservice failed to set listen socket socket to non-blocking"); 
		g_ExitFlag = 1;
		return -1;
	 }

	 
	struct sockaddr_in ClientAddr;
	int nClientSocket = -1;
	socklen_t ClientLen = sizeof(ClientAddr);
	

	while(1)
	{
		memset(&ClientAddr,0,ClientLen);
		nClientSocket = accept(nSocket,(struct sockaddr *)&ClientAddr, &ClientLen);
		if(-1==nClientSocket)
		{
			XERROR("InService accet nClientSocket:%s\n",strerror(errno));
			return;
		}
				 
		if ( 0 != setnonblock(nClientSocket) )
		{
			XERROR("InService failed to set client socket socket to non-blocking,socket:%d\n",nClientSocket); 
			close(nClientSocket);
			continue;
		}
			 
		STConnNode *pConn= CMemPoolMgr::GetInstance()->GetConnNode();
		if (NULL == pConn)
		{
			close(nClientSocket);
			XERROR("InService GetConnNode mem node is zero , close client socket:%d\n",nClientSocket); 
			sleep(10);
			continue;
		}
		
		gettimeofday(&(pConn->conntime),NULL);
		pConn->ip = ntohl(ClientAddr.sin_addr.s_addr);
		pConn->port = ntohs(ClientAddr.sin_port);
		pConn->socket = nClientSocket;




		STDataNode *pDataNode = CMemPoolMgr::GetInstance()->GetDataNode();
		if( NULL == pDataNode )
		{
				close(nClientSocket);
				XERROR("InService GetDataNode mem node is zero , close client socket:%d\n",sock); 
				CMemPoolMgr::GetInstance()->PushConnNode(pConn);
				sleep(10);
				continue;
		}
	
		pConn->pDataNode = pDataNode;		
		pDataNode->pConn = pConn;
		pDataNode->objConn = *pConn;

		int ret = 0;
		do
		{
			ret = CBaseLRModule::ReadData(pDataNode) ;
			if( 1 == ret)break;

			if(-1 == ret)
			{
				close(nClientSocket);
				XERROR("InService ReadData error, close client socket:%d\n",sock); 
				CMemPoolMgr::GetInstance()->PushDataNode(pDataNode);
				CMemPoolMgr::GetInstance()->PushConnNode(pConn);
				break;
			}

			if(0 == ret)
			{
				sleep(1);
				continue;  // //针对内部的一个client，继续读
			}
			
		}while(1);
		
		if(1==ret)CInserviceWork::GetInstance()->PushDataNode(pDataNode);	


	}
	*/

	return 0;

}





