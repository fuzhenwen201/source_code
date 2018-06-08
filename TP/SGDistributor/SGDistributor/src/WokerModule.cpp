


#include "WokerModule.h"
#include "WorkeDataListMgr.h"
#include "MemPoolMgr.h"


list<CWorkerProcee *> CWorkerProcee::m_WorkerList;



CWorkerProcee::CWorkerProcee()
{
	pthread_mutex_init(&m_INServiceGWMapLock,NULL);

}






CWorkerProcee::~CWorkerProcee()
{
	XINFO("worker exit ....\n");
}








// 在创建worker时,调用,  该list用于 Inservice 分发内部msg到每个worker用
void CWorkerProcee::AddWorkerToList()
{
	m_WorkerList.push_back(this);
}






int CWorkerProcee::DeleteGWMsg(STConnNode *&pConnNode)
{
	XINFO("DeleteGWMsg gwip:%s,gwport:%d\n",iptostrip(htonl(pConnNode->gwip)),pConnNode->gwport);

	
	map<unsigned long, STINServiceGWBody>::iterator itr;
	itr = m_INServiceGWMap.find(pConnNode->gwip + pConnNode->gwport);
	if (itr != m_INServiceGWMap.end()) 
	{
		m_INServiceGWMap.erase(itr);
		XERROR("LRModule Delete GWMsg  sucess: gwip:%s,gwport:%d\n",
				iptostrip(htonl(pConnNode->gwip)), pConnNode->gwport);
	}
	else
	{
		XERROR("LRModule Delete GWMsg don't find: gwip:%s,gwport:%d\n",
				iptostrip(htonl(pConnNode->gwip)), pConnNode->gwport);
	}

	return 0;
}





int CWorkerProcee::LRModuleDeleteGWMsg(STConnNode *&pConnNode)
{
	if( 0 == pConnNode->gwip || 0 ==  pConnNode->gwport )
	{
		XERROR("LRModuleDeleteGWMsg,socket close,don't delete gw, ip:%s, client port:%d,conn addr:%p, socket : %d\n",
			iptostrip(pConnNode->ip), pConnNode->port,pConnNode,pConnNode->socket
		);

		return 0;
	}

	list<CWorkerProcee *>::iterator itr = m_WorkerList.begin();
	for(itr;itr !=m_WorkerList.end(); itr ++ )
	{
		CWorkerProcee *p = (*itr);
		p->DeleteGWMsg(pConnNode);
	}

	return 0;

}




int CWorkerProcee::DistributeGWMsg(STINServiceGWBody &gwMsg)
{
	

	map<unsigned long, STINServiceGWBody>::iterator itr;
	int nFindFlag = -1;
	STINServiceGWBody *pGwMsg = NULL;
	STINServiceGWBody *msgNode = &gwMsg;
	
	pthread_mutex_lock(&m_INServiceGWMapLock);
	{
		
		itr = m_INServiceGWMap.find(msgNode->gwip + msgNode->gwport);
	  	if (itr != m_INServiceGWMap.end()) 
		{
			pGwMsg = (STINServiceGWBody *)&(itr->second);
			nFindFlag = 0;
	  	}
		
		if( nFindFlag == 0 && 1 == msgNode->op_type )    // update
		{
			pGwMsg->curusercount = msgNode->curusercount;
			pGwMsg->maxusercount = msgNode->maxusercount;	
			XINFO("DistributeGWMsg update gw sucess: gwip:%s,gwport:%d,curusers:%d,maxusers:%d, gwtotal:%ld \n",
				iptostrip(htonl(msgNode->gwip)), msgNode->gwport,msgNode->curusercount, msgNode->maxusercount,m_INServiceGWMap.size()
			);
		}
		else if( nFindFlag == -1 && 1 == msgNode->op_type )   // add
		{
			m_INServiceGWMap.insert(pair<unsigned long,STINServiceGWBody>(msgNode->gwip + msgNode->gwport,gwMsg));
			XINFO("DistributeGWMsg insert gw sucess: gwip:%s,gwport:%d,curusers:%d,maxusers:%d, gwtotal:%ld \n",
				iptostrip(htonl(msgNode->gwip)), msgNode->gwport,msgNode->curusercount, msgNode->maxusercount,m_INServiceGWMap.size()
			);
		}
		else if( nFindFlag == 0 && 0 == msgNode->op_type ) // delete
		{
			m_INServiceGWMap.erase(itr);
			XINFO("DistributeGWMsg delete gw sucess: gwip:%s,gwport:%d,curusers:%d,maxusers:%d, gwtotal:%ld \n",
				iptostrip(htonl(msgNode->gwip)), msgNode->gwport,msgNode->curusercount, msgNode->maxusercount,m_INServiceGWMap.size() 
				);
		}
		else if( nFindFlag == -1 && 0 == msgNode->op_type ) 
		{
			XINFO("DistributeGWMsg not find for delete: gwip:%s,gwport:%d,curusers:%d,maxusers:%d, gwtotal:%ld \n",
				iptostrip(htonl(msgNode->gwip)), msgNode->gwport,msgNode->curusercount, msgNode->maxusercount,m_INServiceGWMap.size()
				);
		}
		
		ShowGwMap();
	}
	pthread_mutex_unlock(&m_INServiceGWMapLock);

	return 0;

}







int CWorkerProcee::InserviceDistributeGWMsg(STINServiceGWBody &msg)
{
	

	list<CWorkerProcee *>::iterator itr ;
	for(itr = m_WorkerList.begin();itr !=m_WorkerList.end(); itr ++ )
	{
		CWorkerProcee *p = (*itr);
		p->DistributeGWMsg(msg);
	}

	return 0;

}






void CWorkerProcee::ShowGwMap()
{
	XINFO("Show Gw List:\n");
    map<unsigned long, STINServiceGWBody>::iterator itr;
    for (itr=m_INServiceGWMap.begin(); itr!=m_INServiceGWMap.end(); ++itr)
    {

		STINServiceGWBody *pGwMsg = (STINServiceGWBody *)&(itr->second);
        XINFO("gwip:%s,gwport:%d,curusers:%d,maxusers:%d, gwtotal:%ld \n",
				iptostrip(htonl(pGwMsg->gwip)), pGwMsg->gwport,pGwMsg->curusercount, pGwMsg->maxusercount,m_INServiceGWMap.size()
				);	
        
    }
}






// 解析外部Client 协议
int CWorkerProcee::ParseOutProtocol(STDataNode *&pNode,STOutAppMsgRequest &appMsg,STOutAppMsgAck &appAckMsg)
{
	appMsg = *((STOutAppMsgRequest *)pNode->szData);

	//appMsg.msgbody.gwip 	=   ntohl(appMsg.msgbody.gwip);
	appMsg.msgbody.gwport 	= 	ntohs(appMsg.msgbody.gwport);
	appMsg.msgbody.groupid  = 	ntohl(appMsg.msgbody.groupid);
	appMsg.msgbody.clientid = 	ntohl(appMsg.msgbody.clientid);
	
	appAckMsg.msgbody.groupid 	= 	appMsg.msgbody.groupid;
	appAckMsg.msgbody.clientid	= 	appMsg.msgbody.clientid;


	SetMsgHeader(appAckMsg.msgheader,sizeof(appAckMsg),SRVVERSION,MSG_OUTCLIENT_GW_ACK,appMsg.msgheader.requestid);
	

	if(SRVVERSION != ntohs(appMsg.msgheader.version))
	{
		XERROR("ParseOutProtocol fail, srv self version:%d,client send version :%d, requestid:%s, out client ip:%s,out client port:%d\n",
				SRVVERSION,appMsg.msgheader.version, appMsg.msgheader.requestid ,iptostrip(htonl(pNode->objConn.ip)), pNode->objConn.port);
		
		appAckMsg.errNo =  htonl(ERRCODE_PARSEERR) ;
		return -1;
	}


	
	
	XINFO("ParseOutProtocol parse sucess, requestid:%s,oldroute sgurl:%s,oldrouteport:%d, groupid:%d,clientid:%d\n]\n",
		appMsg.msgheader.requestid, appMsg.msgbody.gwurl, appMsg.msgbody.gwport, appMsg.msgbody.groupid,appMsg.msgbody.clientid);

	appAckMsg.errNo =  htonl(ERRCODE_OK);
	
	
}





//  获取到路由, 返回0
int CWorkerProcee::GetGWIPPort_OldRoute(STOutAppMsgRequest &appMsg,STOutAppMsgAck &outCliAckMsg)
{

	unsigned int gwip = m_SgUrlMap.GetSGipFromURL(appMsg.msgbody.gwurl);
	
	map<unsigned long, STINServiceGWBody>::iterator itr;
	pthread_mutex_lock(&m_INServiceGWMapLock);
	{
	 	itr = m_INServiceGWMap.find(gwip + appMsg.msgbody.gwport);
  		if (itr == m_INServiceGWMap.end())
  		{
  			XINFO("GetGWIPPort_OldRoute don't find gw:oldsgurl:%s,oldgwport:%d\n",
						appMsg.msgbody.gwurl, appMsg.msgbody.gwport);
			
			pthread_mutex_unlock(&m_INServiceGWMapLock);
			return -1;
		}

		STINServiceGWBody *pGwMsg = (STINServiceGWBody *)&(itr->second);
		if (pGwMsg->curusercount >= pGwMsg->maxusercount ) 
		{
			
			XINFO("GetGWIPPort_OldRoute find sg, but curusers [%u] >= maxusers[%u] :oldsgurl:%s,oldgwport:%d\n",
						pGwMsg->curusercount, pGwMsg->maxusercount ,appMsg.msgbody.gwurl, appMsg.msgbody.gwport);
			pthread_mutex_unlock(&m_INServiceGWMapLock);
			return -1;
		}

		//outCliAckMsg.msgbody.gwip = pGwMsg->gwip;
		strncpy(outCliAckMsg.msgbody.gwurl,appMsg.msgbody.gwurl,URLMAXLEN-1);
		outCliAckMsg.msgbody.gwport = pGwMsg->gwport ;
		
	}
	pthread_mutex_unlock(&m_INServiceGWMapLock);


	XINFO("GetGWIPPort_OldRoute  find gw:newswurl:%s,newgwport:%d,oldsgurl:%s,oldgwport:%d\n",
			outCliAckMsg.msgbody.gwurl, outCliAckMsg.msgbody.gwport,appMsg.msgbody.gwurl, appMsg.msgbody.gwport);

	return 0;
}







//  获取到路由, 返回0

int CWorkerProcee::GetGWIPPort_Aerage(STOutAppMsgRequest &appMsg,STOutAppMsgAck &outCliAckMsg)
{
	unsigned int emptyusers = -1;
	int nHasRoute = -1;
	int tmpusers = 0 ;
	map<unsigned long, STINServiceGWBody>::iterator itr;

	char sgurl[URLMAXLEN] = {0};
	unsigned int sgip = 0;
	
	pthread_mutex_lock(&m_INServiceGWMapLock);
	{
		
		for (itr=m_INServiceGWMap.begin(); itr!=m_INServiceGWMap.end(); ++itr)
		{
			STINServiceGWBody *pGwMsg = (STINServiceGWBody *)&(itr->second);
			if(pGwMsg == NULL) continue;
			tmpusers = pGwMsg->maxusercount - pGwMsg->curusercount;
			if(emptyusers == -1 || tmpusers > emptyusers ) 
			{
				//outCliAckMsg.msgbody.gwip= pGwMsg->gwip;
				//strncpy(outCliAckMsg.msgbody.gwurl,pGwMsg->gwurl,URLMAXLEN-1);

				memset(sgurl,0,URLMAXLEN);
				sgip = pGwMsg->gwip;
				m_SgUrlMap.GetURLFromSGIp(pGwMsg->gwip+pGwMsg->gwport, sgurl);
				
				outCliAckMsg.msgbody.gwport= pGwMsg->gwport ;
				emptyusers = tmpusers;
				nHasRoute = 0;
			}
			
			continue;
		}
	}
	pthread_mutex_unlock(&m_INServiceGWMapLock);

	if(nHasRoute == 0)
	{	
		if(0 != strlen(sgurl))
		{
			strncpy(outCliAckMsg.msgbody.gwurl,sgurl,URLMAXLEN-1);
			XINFO("GetGWIPPort_Aerage,requestid:%s,  find gw:newsgurl:%s,newsgport:%d,oldsgurl:%s,oldsgport:%d\n",
			appMsg.msgheader.requestid,outCliAckMsg.msgbody.gwurl, outCliAckMsg.msgbody.gwport,appMsg.msgbody.gwurl, appMsg.msgbody.gwport);
		}
		else
		{
			XERROR("GetGWIPPort_Aerage,requestid:%s,   local ip:%s corresponds to url don't find in ipurl map\n",
			appMsg.msgheader.requestid, iptostrip(htonl(sgip)) , appMsg.msgbody.gwport);
			m_SgUrlMap.ShowUrlMapList();
			nHasRoute = -1;
		}
	}
	else
	{
	 
		XERROR("GetGWIPPort_Aerage,requestid:%s, don't find sg ip port; oldurl:%s,oldport:%d \n",
			appMsg.msgheader.requestid, appMsg.msgbody.gwurl, appMsg.msgbody.gwport);
		m_SgUrlMap.ShowUrlMapList();
		
	}
	
	return nHasRoute;

}









int CWorkerProcee::GetGwInfo(STOutAppMsgRequest &appMsg,STOutAppMsgAck &outCliAckMsg)
{
	int nPolicyId = stConfinfo.nDistributePolicy;
	int nHasGw = -1; 	// -1 没有gw, 0 获取到gw

	
	if(strlen(appMsg.msgbody.gwurl)== 0 || appMsg.msgbody.gwport == 0)
		nPolicyId = MSG_DISTRIBUTE_AVERAGE_REQ;
	
	if ( nPolicyId == MSG_DISTRIBUTE_OLDROUTE_REQ )
	{
		if ( -1 == GetGWIPPort_OldRoute(appMsg,outCliAckMsg) )
		{
			nPolicyId = MSG_DISTRIBUTE_AVERAGE_REQ;
			outCliAckMsg.errNo = htonl(ERRCODE_NOGW);
		}
		else
			nHasGw = 0;
	}
	

	if (nHasGw == -1 && nPolicyId == MSG_DISTRIBUTE_AVERAGE_REQ )
	{
		if ( 0 == GetGWIPPort_Aerage(appMsg,outCliAckMsg))
			nHasGw = 0;
		else
			outCliAckMsg.errNo =  htonl(ERRCODE_NOGW);
	}


	if(0 == nHasGw)outCliAckMsg.errNo =  htonl(ERRCODE_OK);
	
	
	

	return nHasGw;
}


	
void CWorkerProcee::Run()
{
	m_SgUrlMap.ReadURLMap();
	
	int number = 0;
	while(1)
	{
		
		int nPolicyId = g_nDistributePolicy;
		int ret = -1;
		int nHasGw = -1;  // 0 表示有路由信息,-1表示没有

		m_SgUrlMap.ReadURLMap();

		// 1, 获取数据
		CWorkDataListMgr *pDataMgr  = CWorkDataListMgr::GetInstance();

		STDataNode *pNode = pDataMgr->GetDataNode();
		if(NULL == pNode)
		{
			//XINFO("worker data list no out data\n");
			usleep(5000);
			continue;
		}


		

		
		STOutAppMsgAck outCliAckMsg; 
		STOutAppMsgRequest outCliReqMsg;
		
		
		// 2. 解析协议
		ret = ParseOutProtocol(pNode,outCliReqMsg,outCliAckMsg) ;

		
		 // 3. 获取GW路由信息	
		if( ret == 0 && 0 == GetGwInfo(outCliReqMsg,outCliAckMsg))
		{
			XINFO("Worker GetGWIPPort to client , gw url:%s, gw port:%d\n",
					outCliAckMsg.msgbody.gwurl, outCliAckMsg.msgbody.gwport);
		}

		
		// 4. 发送 
		
		if( (pNode->pConn->conntime.tv_sec == pNode->objConn.conntime.tv_sec) 
				&& ( pNode->pConn->conntime.tv_usec == pNode->objConn.conntime.tv_usec ) )
		{
			
			//outCliAckMsg.msgbody.gwip     = htonl(outCliAckMsg.msgbody.gwip);
			outCliAckMsg.msgbody.gwport   = htons(outCliAckMsg.msgbody.gwport);
			outCliAckMsg.msgbody.groupid  = htonl(outCliAckMsg.msgbody.groupid);
			outCliAckMsg.msgbody.clientid = htonl(outCliAckMsg.msgbody.clientid);
						
			// 连接正常, 可以发送数据
			int len = sizeof(outCliAckMsg);
			ret =	sendforlen(pNode->objConn.socket,(void *)&outCliAckMsg, len) ;
			
						
			XINFO("outService send ack sucess [\n,ack errno:%d , socket:%d,return len :%d,content len:%d,\n client ip:%s,client port:%d \n ",
								ntohl(outCliAckMsg.errNo),pNode->objConn.socket, ret,len,
								iptostrip(htonl(pNode->objConn.ip)), pNode->objConn.port );
						
			XINFO("gw sgurl:%s, gw port:%d,groupid:%d,clientid:%d \n] \n",
								outCliAckMsg.msgbody.gwurl, ntohs(outCliAckMsg.msgbody.gwport), 
								ntohl(outCliAckMsg.msgbody.groupid) ,ntohl(outCliAckMsg.msgbody.clientid));
			
		}
		else
		{
			// 连接无效, 不用发数据
			XERROR("connect is bad, don't send ack, socket:%d,client ip:%s, client port:%d\n",
				pNode->objConn.socket,iptostrip(htonl(pNode->objConn.ip)), pNode->objConn.port);
		}

				
		CMemPoolMgr::GetInstance()->PushDataNode(pNode);
		

	}
	
	

	

}


void CWorkerProcee::WorkerBatchStart()
{
    int i = 0;
    for(i = 0; i < stConfinfo.nWorkerNumber; i++ )
    {
        CWorkerProcee *pWorker = new CWorkerProcee();
        pWorker->AddWorkerToList();
        pWorker->Start();
    }

	XINFO("worker %d module run ...\n",stConfinfo.nWorkerNumber);
}

