/*******************************************************************************************

*Department:	TP

*Decrible:  	外部客户端  测试程序 之外部客户端 , APP etc

				usage:
				
				./msg_rttcp_client srvip srvport groupid clientid coincount coinarray realtime:1/historydata:0 granularity
				
				

*Auhor:			Savin Chen

*Createdtime:	2017-04-27

*ModifyTime:	


********************************************************************************************/




#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <sys/un.h>  
#include <arpa/inet.h>  
#include <netinet/in.h>  
  
#include "../inc/Public.h"





char sRequestId_logon[37]	={"logon678901234567890123456789012ab12"};
char sRequestId_rt[37]		={"rt345678901234567890123456789012ab12"};
char sRequestId_hist[37]	={"hist5678901234567890123456789012ab12"};
char sRequestId_spread[37]	={"spread56789012345678901234567890ab12"};



typedef struct _STPriceBody
{
	unsigned long	srv_seqid;	 
	unsigned int 	groupid;
	unsigned int	clientid;
	unsigned short 	msgnumber;					// 币种个数
	char			coinresponse[COINNUMBER*COINCONNECTMAXLEN+COINNUMBER];

}__attribute__((packed)) STPriceBody;


typedef struct _STHistoryHeader_Test
{
	unsigned int	starter;				//  0x008F
	unsigned short  msglen;					// 消息总长度
	unsigned short 	version;				// 版本号: 目前 为 1
	unsigned int  	msgid;		
	char 			requestid[36];	

	unsigned int 	groupid;
	unsigned int	clientid;
	int		 		errNo;

	
	int 			CandleCounts ;
	char 			coinname[8];
	char 			granularity[4];
	int 			datalen;

}__attribute__((packed)) STHistoryHeader_Test;

typedef struct _STHistoryData_Test
{
	STHistoryHeader_Test header;
	char 			data[MAXHISTORYDATA];	

}STHistoryData_Test;





char coinarray[1024];
int connect_fd  ;

unsigned int groupid;
unsigned int clientid; 
unsigned short coincount;

char granularity[12];



int spreadtype = 0;




int SendRealtimeRequest(int optype,int _coincount,char *_coinarray)
{
		int ret = -1;
		STRTMsgRequest req;
		memset(&req,0,sizeof(req));
		req.body.optype = htons(optype);
		req.body.msgnumber = htons(_coincount);
		req.body.groupid = htonl(groupid);
		req.body.clientid = htonl(clientid);
		strcpy(req.body.coinarray,_coinarray);
		int contentlen = strlen(req.body.coinarray);
	
		int sendlen = sizeof(req.header)+12+contentlen;
		SetMsgHeader(req.header,sendlen,1,RTMSG_REQUEST,sRequestId_rt);
		{
			ret = sendforlen(connect_fd,(void *)&req ,sendlen,sRequestId_rt) ;
			if( sendlen != ret ) 
			{
				printf("send return fail:send len%d,send ret:%d ok\n",sendlen,ret) ;
				close(connect_fd);
				return -1;
			}
			
			printf("send realtime request OK:send len%d,send ret:%d ok\n",sendlen,ret) ;
		}


	/*
	STMsgRequestAck reqack;
	int recvlen = sizeof(STMsgRequestAck);
	memset(&reqack,0, recvlen);
	ret = recvforlen(connect_fd,(void *)&reqack,recvlen);
	if( recvlen != ret ) 
	{
		printf("recvforlen faill ...,recvlen:%d,ret:%ld\n",recvlen,ret);
		close(connect_fd);
		return -1;
	}
	
	printf("recv realtime request ack recvlen:%d,ret:%d ok\n",recvlen,ret) ;
	{
		printf("starter:0x%04x,msglen:%d,version:%d,msgid:0x%04x, requestid:%s , errno:%d, groupid:%d,clientid:%d\n", 
				ntohl(reqack.header.starter),ntohs(reqack.header.msglen) ,ntohs(reqack.header.version),ntohl(reqack.header.msgid),reqack.header.requestid,
				ntohl(reqack.errNo),ntohl(reqack.groupid), ntohl(reqack.clientid));
	}
	*/

	return 0;

}


int LogonProcess()
{
	int ret = 0;
	STLogonNode logon;
	int sendlen = sizeof(logon);
	SetMsgHeader(logon.header,sendlen,1,LOGIN_REQUEST,sRequestId_logon);
	logon.body.groupid = htonl(groupid);
	logon.body.clientid = htonl(clientid);
	strcpy(logon.body.password,"Helldsd");

	ret = sendforlen(connect_fd,(void *)&logon ,sendlen,sRequestId_logon) ;
	if( sendlen != ret ) 
	{
		printf("send logon fail:send len%d,send ret:%d ok\n",sendlen,ret) ;
		close(connect_fd);
		return -1;
	}
	printf("send logon ok, groupid:%d,clientid:%d,sendlen:%d,ret:%d ok\n",groupid,clientid,sendlen,ret) ;
	

	STLogonNodeAck ack;
	memset(&ack,0,sizeof(ack));
	int headlen = sizeof(ack.header);
	ret = recvforlen(connect_fd,(void *)(&(ack.header)),headlen);
	if( ret != headlen )  //不做精确的读
	{
		printf("logon recvforlen read head faill ...,recvlen:%d,ret:%ld\n",headlen,ret);
		close(connect_fd);
		return -1;
	}

	int bodylen = ntohs(ack.header.msglen) - headlen;
	printf("recv logon response:msglen:%d, bodylen:%d\n", ntohl(ack.header.msglen),bodylen);


	ret = recvforlen(connect_fd,(void *)&(ack.body),bodylen);
	if( bodylen != ret ) 
	{
		printf("recv logon response body faill ...,recvlen:%d,ret:%ld\n",bodylen,ret);
		close(connect_fd);
		return -1;
	}
	printf("send recv ok, bodylen:%d,ret:%ld\n",bodylen,ret);

	printf("starter:0x%04x,msglen:%d,version:%d,msgid:0x%04x,requestid:%s, groupid:%d,clientid:%d, data:%s\n", 
				ntohl(ack.header.starter),ntohs(ack.header.msglen) ,ntohs(ack.header.version),ntohl(ack.header.msgid),ack.header.requestid,
				ntohl(ack.body.groupid), ntohl(ack.body.clientid),ack.body.symbollist);


	if(0 == strcmp(ack.body.symbollist,"-1"))
	{
		printf("logon fail\n");
		return -1;
	}
	else
	{
		printf("logon sucess\n");
		return 0;
	}

	return 0;
	
	
}



int SendHistMarketQuery()
{
	int ret = 0;
	STHistDataRequest hist;
	int sendlen = sizeof(hist);
	SetMsgHeader(hist.header,sendlen,1,HISTMSG_REQUEST_SMALLPACK,sRequestId_hist);
	//SetMsgHeader(hist.header,sendlen,1,HISTMSG_REQUEST,sRequestId_hist);
	hist.body.candlecounts = htonl(1);
	hist.body.groupid = htonl(groupid);
	hist.body.clientid = htonl(clientid);

	//coinarray
	//strcpy(hist.body.coinname,"CAD_CHF");
	//strcpy(hist.body.granularity,"M10");

	strcpy(hist.body.coinname,coinarray);
	strcpy(hist.body.granularity,granularity);
	
	ret = sendforlen(connect_fd,(void *)&hist ,sendlen,sRequestId_hist) ;
	if( sendlen != ret ) 
	{
		printf("send histdata request fail:send len%d,send ret:%d ok\n",sendlen,ret) ;
		close(connect_fd);
		return -1;
	}
	printf("send  histdata request ok, sendlen:%d,ret:%d ok\n",sendlen,ret) ;


	STHistoryData_Test resp;
	memset(&resp,0,sizeof(resp));
	//while(1)
	{
		int headlen = sizeof(resp.header);
		printf("history data headlen:%d\n",headlen);
		
		
		ret = recvforlen(connect_fd,(void *)(&(resp.header)),headlen);
		if( ret != headlen )  //不做精确的读
		{
			printf("recvforlen read head faill ...,recvlen:%d,ret:%ld\n",headlen,ret);
			close(connect_fd);
			return -1;
		}

		int bodylen = ntohl(resp.header.datalen);
		
		printf("read hist header:head len:%d, body len:%d\n",headlen,bodylen);
		printf("starter:0x%04x,msglen:%d,version:%d,msgid:0x%04x\n",
				ntohl(resp.header.starter),ntohs(resp.header.msglen), ntohs(resp.header.version),ntohl(resp.header.msgid));
		printf("groupid:%d,clientid:%d,errno:%d, CandleCounts:%d,coinname:%s,granularity:%s,datalen:%d,requestid:%s\n", 
				ntohl(resp.header.groupid), ntohl(resp.header.clientid), resp.header.errNo, ntohl(resp.header.CandleCounts),
				resp.header.coinname,resp.header.granularity,ntohl(resp.header.datalen),resp.header.requestid
			  );
		

		
		ret = recvforlen(connect_fd,(void *)(&(resp.data)),bodylen);
		if( ret != bodylen )  //不做精确的读
		{
			printf("recvforlen read head faill ...,recvlen:%d,ret:%ld\n",bodylen,ret);
			close(connect_fd);
			return -1;
		}
		printf("recv read hist body: recvlen:%d,ret:%d ok\n",bodylen,ret ) ;
	
		
		DecodeHistory(resp.data,sizeof(resp.data));


		/*
		// for test
		char buffer[100]={0};
		ret = recvforlen(connect_fd,(void *)buffer,99);
		printf("test data:%d\n",ret);
		for(i=0;i<ret;i++)
		{
			printf("0x%02x",pBuf[i]);
		}
		printf("\n");
		*/


	}
	
	

}





int SendGroupSpread()
{
	STGroupSpread spread, spread_ack;
	int sendlen = sizeof(STGroupSpread);

	SetMsgHeader(spread.header,sendlen,1,spreadtype,sRequestId_spread);
	spread.othervalue = htonl(groupid);
	
	if (spreadtype == CURRENCYGROUP_REQUEST)
	{
		spread.othervalue = htonl(groupid) ;
	}

	
	int ret = sendforlen(connect_fd,(void *)&spread ,sendlen,sRequestId_spread) ;
	if( sendlen != ret ) 
	{
		printf("Send GroupSpread request fail, sendlen:%d,ret:%d ok, spreadtype:0x%04x\n",sendlen,ret,spreadtype) ;
		close(connect_fd);
		return -1;
	}
	printf("Send GroupSpread request ok, sendlen:%d,ret:%d ok, spreadtype:0x%04x\n",sendlen,ret,spreadtype) ;


	ret = recvforlen(connect_fd,(void *)(&(spread_ack)),sendlen);
	if( ret != sendlen )  //不做精确的读
	{
		printf("recv GroupSpread request ack faill ...,recvlen:%d,ret:%ld\n",sendlen,ret);
		close(connect_fd);
		return -1;
	}
	printf("recv GroupSpread request ack ok: recvlen:%d,ret:%d, msgid:%d,value:%d ok\n",
		sendlen,ret, ntohl(spread_ack.header.msgid), ntohl(spread_ack.othervalue) ) ;

	close(connect_fd);
	

	return 0;
}

  
int main(int argc, char *argv[])  
{  

	if(argc != 9)
	{
		printf("parameter error,Usage:\n./msg_rttcp_client srvip srvport groupid clientid coincount coinarray realtime:1/historydata:0 granularity\n\n");
		return -1;
	}

	printf("srvip:%s, srvport:%s, groupid:%s,clientid:%s,coincount:%s, coinarray:%s, datatype:%s granularity:%s\n",
		argv[1],argv[2],argv[3],argv[4],argv[5],argv[6], argv[7],argv[8]
	);

	int datatype ;
	char srvip[20] ;
	memset(srvip,0,sizeof(srvip));
	memset(coinarray,0,sizeof(coinarray));
	memset(granularity,0,sizeof(granularity));
	

	
	strncpy(srvip,argv[1],sizeof(srvip));
	unsigned short srvport = atoi(argv[2]);
	//groupid =  striptoip(argv[3]);
	groupid = atol(argv[3]);
	clientid =  atol(argv[4]);
	spreadtype = clientid;
	coincount = atoi(argv[5]);
	strncpy(coinarray,argv[6],sizeof(coinarray)-1);
	datatype = atoi(argv[7]);
	strncpy(granularity,argv[8],sizeof(granularity)-1);
	  
    connect_fd = socket(AF_INET, SOCK_STREAM, 0);   
    if(connect_fd < 0)  
    {  
        perror("socket");  
        return -1;  
    }  
  
     
    struct sockaddr_in addr;   
    addr.sin_family = AF_INET;   
    addr.sin_port = htons(srvport);   
    addr.sin_addr.s_addr = inet_addr(srvip);   
  
    socklen_t addr_len = sizeof(addr);  
    int ret = connect(connect_fd, (struct sockaddr*)&addr, addr_len);  
    if(ret < 0)  
    {  
        perror("connect");  
        return -1;  
    }  

	printf("socket:%d, connect server , datatype:%d ...\n",connect_fd,datatype);

	printf("spreadtype:%d, clientid:%d,argv[4]:%s\n",spreadtype,clientid,argv[4]);
	if (spreadtype == CURRENCYGROUP_REQUEST || spreadtype==CURRENCYSYMBOL_REQUEST)
	{
		SendGroupSpread();
		sleep(1);
		exit(0);
	}

	if( 0 != LogonProcess())return -1;
	
	
	// hist market
	if(datatype == 0)
	{ 
		int ii ;
		//for(ii = 0 ; ii < 10; ii ++)
		{
			if(datatype == 0 )SendHistMarketQuery();
			//sleep(3);
		}
	}
	
	
	if(datatype)
	{
		if(-1 == SendRealtimeRequest(RT_MSG_ADD,coincount,coinarray))return -1;
	

		STMsgHeader 	header;
		STPriceBody 	body;
		int count = 0;
		while(1)
		{
			count ++;
			printf("\n\n\n");
			
			int headlen = sizeof(header);
			memset(&header,0,headlen );	
			ret = recvforlen(connect_fd,(void *)(&(header)),headlen);
			if( ret != headlen )  //不做精确的读
			{
				printf("recvforlen read head faill ...,recvlen:%d,ret:%ld\n",headlen,ret);
				close(connect_fd);
				return -1;
			}

			int msglen = ntohs(header.msglen);
			int contentlen = msglen-sizeof(header);
			printf("read header: msg total len:%d,head len:%d, body len:%d\n",msglen,headlen,contentlen);
			printf("starter:0x%04x,msglen:%d,version:%d,msgid:0x%04x,requestid:%s\n",
					ntohl(header.starter),ntohs(header.msglen), ntohs(header.version),ntohl(header.msgid),header.requestid);


			
			memset(&body,0,sizeof(body));
			ret = recvforlen(connect_fd,(void *)(&body),contentlen);
			if( ret != contentlen )  //不做精确的读
			{
				printf("recvforlen read head faill ...,recvlen:%d,ret:%ld\n",headlen,ret);
				close(connect_fd);
				return -1;
			}
			printf("recv read body: recvlen:%d,ret:%d ok\n",contentlen,ret) ;
			printf("seqid:%lu,groupid:%d,clientid:%d,number:%d,content:%s \n", 
					ntohl(body.srv_seqid),ntohl(body.groupid), ntohl(body.clientid),ntohs(body.msgnumber),body.coinresponse
				  );


			DecodeRealtime(body.coinresponse,sizeof(body.coinresponse));
			
			/*
			if(5==count)
			{
				//if(-1 == SendRequest(RT_MSG_DELETE,coincount,coinarray))return 0;
				//char _coinarray[]="EUR_DKK";
				//if(-1 == SendRequest(RT_MSG_UPDATE,1,_coinarray))return 0;
		
			}
			*/

		}

	}	
	

    close(connect_fd);  
    return 0;  
}  
