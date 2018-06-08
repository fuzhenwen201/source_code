

#include "LRModule.h"
#include "WokerModule.h"
#include "InService.h"
#include "MemPoolMgr.h"

#include "URLMap.h" 


list<STDataNode *> 						CInserviceWork::m_InRequestList;
pthread_mutex_t							CInserviceWork::m_InRequestListLock = PTHREAD_MUTEX_INITIALIZER;



CInserviceWork*  CInserviceWork::m_pInstance = NULL;

CInserviceWork::CInserviceWork()
{

}



CInserviceWork * CInserviceWork::GetInstance()
{
	if(NULL == m_pInstance)
	{
		m_pInstance = new CInserviceWork;
	}
	
	return m_pInstance;
}


int CInserviceWork::PushDataNode(STDataNode * pNode)
{
	pthread_mutex_lock(&m_InRequestListLock) ;
	m_InRequestList.push_back(pNode);
	pthread_mutex_unlock(&m_InRequestListLock) ;
}

	
void CInserviceWork::Run()
{
	//m_SgUrlMap.ReadURLMap();
	//m_SgUrlMap.ShowUrlMapList();
	
	while(1)
	{
		//m_SgUrlMap.ReadURLMap();
	
		if (0 == m_InRequestList.size())
		{
			sleep(1);
			continue;
		}

		

		STDataNode * pNode ;
		pthread_mutex_lock(&m_InRequestListLock);
		pNode = m_InRequestList.front();
		m_InRequestList.pop_front();
		pthread_mutex_unlock(&m_InRequestListLock);

		ProcessInData(pNode);

	}
}






int  CInserviceWork::ProcessGw(STDataNode * &pNode, char *pData,STInServiceAck & ack,unsigned char inclient_type) 
{
	STINServiceGWBody *msgBody = (STINServiceGWBody *)pData;

	msgBody->op_type		= ntohs(msgBody->op_type);
	msgBody->gwip 			= ntohl(msgBody->gwip);
	msgBody->gwport 		= ntohl(msgBody->gwport);
	msgBody->curusercount	= ntohl(msgBody->curusercount);
	msgBody->maxusercount 	= ntohl(msgBody->maxusercount);

	if( msgBody->op_type!=1 && msgBody->op_type != 0 )
	{
		XERROR("In Service GW msg error:op_type:%d error,  client ip:%s , client port:%d  \n",
			msgBody->op_type, iptostrip(htonl(pNode->objConn.ip)), pNode->objConn.port );
		ack.errNo = htonl(ERRCODE_PARSEERR);
		return -1;	
	}


	if(msgBody->gwip == 0 || msgBody->gwport== 0 )
	{
		XERROR("In Service gwip:%d or gwport:%d error, client ip:%s , client port:%d  \n",
			msgBody->gwip,msgBody->gwport, iptostrip(htonl(pNode->objConn.ip)), pNode->objConn.port );
		ack.errNo = htonl(ERRCODE_PARSEERR);
		return -1;	
	}	


	if(1 == msgBody->op_type)
	{
		if( msgBody->curusercount < 0 ||  msgBody->curusercount > msgBody->maxusercount  ||  msgBody->curusercount > msgBody->maxusercount  )
		{
			XERROR("In Service curusercount:%d or maxusercount:%d error, client ip:%s , client port:%d  \n",
				msgBody->curusercount,msgBody->maxusercount, iptostrip(htonl(pNode->objConn.ip)), pNode->objConn.port );
			ack.errNo = htonl(ERRCODE_PARSEERR);
			return -1;	

		}
	}

	/*
	int ret =	m_SgUrlMap.GetURLFromSGIp(msgBody->gwip+msgBody->gwpport, msgBody->gwurl);
	if(-1 == ret)
	{
		XERROR("In Service, sgip:%d  don't find url , client ip:%s , client port:%d  \n",
				msgBody->gwip  , iptostrip(htonl(pNode->objConn.ip)), pNode->objConn.port );
		ack.errNo = htonl(ERRCODE_NOURL);
		return -1;	
	}
	*/

	if( 0 == inclient_type)
	{
		pNode->pConn->gwip = msgBody->gwip;
		pNode->pConn->gwport = msgBody->gwport;
	}

	
	XINFO("In Service GW msg parse sucess:\n[\n");
	XINFO("from client ip:%s, client:%d, operate type:%d\n", iptostrip(htonl(pNode->objConn.ip)), pNode->objConn.port,msgBody->op_type);
	XINFO("gw ip:%s, gw port:%d\n", iptostrip(htonl(msgBody->gwip)),msgBody->gwport );
	if( 0 != msgBody->op_type)
		XINFO("curusers:%d,maxusers:%d\n]\n",msgBody->curusercount,msgBody->maxusercount);
	else
		XINFO("]\n");
	
	CWorkerProcee::InserviceDistributeGWMsg(*msgBody);
	ack.msgheader.msgid= htonl(MSG_GATEWAY_ACK);
	ack.errNo = htonl(ERRCODE_OK);

	
	return 0;
}







int  CInserviceWork::ProcessGwMsg(STDataNode * &pNode,STInServiceAck &ack)
{
	unsigned short msg_id 			=  	ntohs(*((unsigned short *)(  pNode->szData+sizeof(STMsgHeader)))  );
	unsigned short inclient_type 	= 	ntohs(*((unsigned short *)(  pNode->szData+sizeof(STMsgHeader)+2))  );
	unsigned short bodynumbers 		= 	ntohs(*((unsigned short *)(  pNode->szData+sizeof(STMsgHeader)+4))  );
	
	if( 0 > bodynumbers ) 
	 {
		XERROR("In Service GW msg error: numbers = %d, server don't support,  client ip:%s , client port:%d  \n",
			bodynumbers, iptostrip(htonl(pNode->objConn.ip)), pNode->objConn.port );
		ack.errNo = htonl(ERRCODE_PARSEERR);
		return -1;
	 }


	if(0 == bodynumbers )
	{
		XERROR("In Service recv beat:: request:%s,msgid:%d  ,client ip:%s , client port:%d  \n",
			 ack.msgheader.requestid,ack.msgheader.msgid,iptostrip(htonl(pNode->objConn.ip)), pNode->objConn.port );
		ack.errNo = htonl(ERRCODE_OK);
		return 0;
	
	}

	XINFO("ProcessGwMsg::monitor_id:%d, inclient type:%d, body number:%d\n",msg_id,inclient_type,bodynumbers);

	int i = 0;
	char  *pData = pNode->szData+sizeof(STMsgHeader)+6;	
	
	for( i=0; i< bodynumbers ; i++ )
	{
		if(-1 == ProcessGw(pNode, pData,ack,inclient_type) )break;
		pData = pData + 18;
	}

	
	return 0;

}










int  CInserviceWork::ProcessDistributeMsg(STDataNode * &pNode,STInServiceAck &ack)
{
	g_nDistributePolicy = ntohl(*((unsigned int *)(pNode->szData+3))) ;
	switch(g_nDistributePolicy)
	{
		case MSG_DISTRIBUTE_OLDROUTE_REQ:
			XINFO("The original routing gw, client ip:%s , client port:%d ...\n",
						iptostrip(htonl(pNode->objConn.ip)), pNode->objConn.port);
			ack.msgheader.msgid =  htonl(MSG_DISTRIBUTE_OLDROUTE_ACK);
			break;
		case MSG_DISTRIBUTE_AVERAGE_REQ:
			XINFO("LRU (least recently used) routing gw, client ip:%s , client port:%d ... ...\n",
					iptostrip(htonl(pNode->objConn.ip)), pNode->objConn.port);
			ack.msgheader.msgid =  htonl(MSG_DISTRIBUTE_AVERAGE_ACK);
			break;
		default:
			ack.msgheader.msgid =  htonl(g_nDistributePolicy);
			ack.errNo = htonl(ERRCODE_PARSEERR);
			XINFO("routing policy error,client send route policy id:%d, client ip:%s , client port:%d ...\n",
				g_nDistributePolicy,iptostrip(htonl(pNode->objConn.ip)), pNode->objConn.port);
			return -1;
	}
	
	ack.errNo = htonl(ERRCODE_OK);
	return 0;
}








int CInserviceWork::ProcessInData(STDataNode * &pNode)
{

	assert(pNode != NULL);
	STInServiceAck ack;		 


	STMsgHeader *pheader = (STMsgHeader *)pNode->szData;

	unsigned int starter = ntohl(pheader->starter);
	unsigned short version = ntohs(pheader->version); 
	unsigned int msgid = ntohl(pheader->msgid);


	if( STARTER != starter )
	{
		XERROR("InService unvalid starter:%d,client ip:%s , client port:%d\n",
			starter,iptostrip(htonl(pNode->objConn.ip)), pNode->objConn.port);
		
		ack.msgheader.msgid= htonl(msgid);
		ack.errNo = htonl(ERRCODE_PARSEERR);
	}
	else if(SRVVERSION != version)
	{
		XERROR("InService unvalid version:%d,client ip:%s , client port:%d\n",
			version,iptostrip(htonl(pNode->objConn.ip)), pNode->objConn.port);
		
		ack.msgheader.msgid= htonl(msgid);
		ack.errNo = htonl(ERRCODE_PARSEERR);
	}
	else
	{
		
		
		if(MSG_GATEWAY_REQ == msgid )
		{
			ProcessGwMsg(pNode,ack);
		}
		else if( (MSG_DISTRIBUTE_OLDROUTE_REQ ==  msgid) || (MSG_DISTRIBUTE_AVERAGE_REQ == msgid) )
		{
			ProcessDistributeMsg(pNode,ack);
		}
		else 
		{
			XERROR("InService unvalid msgid:%d,client ip:%s , client port:%d\n",
				msgid,iptostrip(htonl(pNode->objConn.ip)), pNode->objConn.port);
			
			ack.msgheader.msgid= htonl(msgid);
			ack.errNo = htonl(ERRCODE_PARSEERR);
		}
	}

	int len = sizeof(ack);
	memcpy(ack.msgheader.requestid,pheader->requestid,36);
	SetMsgHeader(ack.msgheader,sizeof(ack),1, msgid,pheader->requestid);

	if ( len != sendforlen(pNode->objConn.socket,(void *)&ack,len ))
	{
		XERROR("InService send ack fail , socket:%d,client ip:%s , client port:%d\n",
			pNode->objConn.socket,iptostrip(htonl(pNode->objConn.ip)), pNode->objConn.port);
	}
	else
	{
		XINFO("InService send ack success , socket:%d,client ip:%s , client port:%d\n",
			pNode->objConn.socket,iptostrip(htonl(pNode->objConn.ip)), pNode->objConn.port);
	}

	CMemPoolMgr::GetInstance()->PushDataNode(pNode);
	
	return 0;
	
	
}





