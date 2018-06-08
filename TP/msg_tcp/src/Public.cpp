

#include "Public.h"
#include "WokerModule.h"
#include "xlogger.h"
//#include "tnodeconfig_v0.h"
#include "ConfigFileReader.h"
#include "marketdataagent.h"







STConfInfo stConfinfo ;



void SetMsgHeader(STMsgHeader &header,unsigned short len,unsigned char version,unsigned int msgid,char *pRequestId)
{
	assert(pRequestId!=NULL);

	header.starter = htonl(STARTER);
	header.msglen = htons(len);
	header.version =htons(version);
	header.msgid = htonl(msgid);
	memcpy(header.requestid,pRequestId,sizeof(header.requestid));

	//XINFO("SetMsgHeader:starter:0x%04x,msgid:0x%04x, msglen:%d,version:%d,requestid:%s \n",STARTER,msgid, len ,version,pRequestId);
}




int messageHeaderVersion = 0;



/*
@ return :
-1:  sokect error
 0:	 un read conplish
>0:  read a package, readlen
*/

ssize_t  recvforlen(int socket, void *vptr, size_t n)  
{  
	 ssize_t nleft;  
	 ssize_t nread;  
	 unsigned char *ptr;  
	  
	 ptr = (unsigned char *)vptr;  
	 nleft = n;  
	 while (nleft > 0) 
	 {  
		if ( (nread = recv(socket, ptr, nleft,0)) < 0) 
		{  
	  		if (errno == EINTR)  // 信号中断， 可以继续循环读
	    		nread = 0; 
			else if ( errno == EAGAIN)  // no data
			{
				XINFO("recvforlen: socket:%d, EAGAIN error,fail : %s\n",socket,strerror(errno));
				nread = 0; 
				break; 
			}
	   		else 
	   		{
	   			XERROR("recvforlen: socket:%d,  other error,  fail %s\n",socket,strerror(errno));
	    		return -1;
	   		}
	  	} else if (nread == 0)  // client closed
	   		return -1;    
	  
	  nleft -= nread;  
	  ptr   += nread;  
	 }  
	 return  (n - nleft); // alread readlen
}  




ssize_t   sendforlen(int socket, const void *vptr, size_t n,const char *pRequestid)  
{  
	 size_t  nleft;  
	 ssize_t  nwritten;  
	 const unsigned char *ptr;  
	  
	 ptr = (const unsigned char *)vptr;  
	 nleft = n;  
	 while (nleft > 0) 
	 {  

		nwritten = send(socket, ptr, nleft,0);
		if( nwritten < 0 && errno == EINTR) { nwritten = 0; continue; }
		if( nwritten < 0 && errno == EAGAIN) 
		{
			XINFO("sendforlen: socket:%d,requestid:%s, EAGAIN error, fail : %s\n",socket,pRequestid ,strerror(errno));
			return 0;
		}
		if( nwritten == 0 || nwritten < 0 )
		{
			XERROR("sendforlen: socket:%d,requestid:%s, other error,  fail :%s\n", socket,pRequestid,strerror(errno));
			return -1;
		}
	  	nleft -= nwritten;  
	  	ptr   += nwritten;  
	 }  
	 
	 return(n);  
}  






int SendToClient(STDataNode *pNode,void * pack, int len,const char *pRequestid)
{
	unsigned char  *pBuf = (unsigned char *)pack;
		
	if(NULL == pNode->pConn)return -1;
	int ret  = -1;
	if( (pNode->pConn->conntime.tv_sec == pNode->objConn.conntime.tv_sec) 
				&& ( pNode->pConn->conntime.tv_usec == pNode->objConn.conntime.tv_usec ) )
	
	{
		
		ret =	sendforlen(pNode->objConn.socket,pack, len,pRequestid) ;
		/*			
		XINFO("SendToClient  socket:%d,return len :%d,content len:%d, client ip:%s,client port:%d, seqid:%lu \n ",
				pNode->objConn.socket, ret,len,iptostrip(htonl(pNode->objConn.ip)), pNode->objConn.port,pNode->objConn.srv_seqid);	
		*/
	}
	else
	{
		ret = -1;
		XERROR("SendToClient connect is bad, don't send , socket:%d,client ip:%s, client port:%d,seqid:%lu,requestid:%s \n",
			pNode->objConn.socket,iptostrip(htonl(pNode->objConn.ip)), pNode->objConn.port,pNode->objConn.srv_seqid, pRequestid);
	}

	

	return ret;
	

}





int setnonblock(int fd)
{
    int flags;

    flags = fcntl(fd, F_GETFL);
    if (flags < 0)
        return flags;
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) < 0)
        return -1;

    return 0;
}

const char * iptostrip(unsigned int ip)
{
	struct in_addr addr = {0};
	addr.s_addr = ip;	
	return inet_ntoa(addr);
}


unsigned int striptoip(const char *ip)
{
	struct in_addr addr = {0};
	inet_aton(ip,&addr);

	return addr.s_addr;

}





void PrintCurrTime(const char *pbuf)
{
    struct timeval  tv;
    gettimeofday(&tv,NULL);
    unsigned int t1 = tv.tv_sec * 1000000 + tv.tv_usec;
    printf("%s:: microseconds:%u\n",pbuf,t1);
}



int ReadConfig(const char * file, STConfInfo &stconfig)
{
	sigset_t signal_mask;
	sigemptyset (&signal_mask);
	sigaddset (&signal_mask, SIGPIPE);
	pthread_sigmask (SIG_BLOCK, &signal_mask, NULL);
	
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigaction( SIGPIPE, &sa, 0 );

	assert(file!=NULL);
	
	char *buf = NULL;
	CConfigFileReader config;	
	
	if( -1 == config.LoadFile(file)  ) 
	{
		printf("open config file: %s,fail:%s\n",file, strerror(errno));
		return -1;
	}


	if (NULL == (buf = config.GetConfigName("send_intertime")))
	{
		printf("read ServerSelf->send_intertime error \n");
		return -1;
	}
	stconfig.send_intertime = atoi(buf);

		
	
	if (NULL == (buf = config.GetConfigName("memnode_count")))
	{
		printf("read ServerSelf->memnode_count error \n");
		return -1;
	}
	stconfig.memnode_count = atoi(buf);

	
	if (NULL == (buf = config.GetConfigName("worker_count")))
	{
		printf("read ServerSelf->worker_count error \n");
		return -1;
	}
	stconfig.worker_count = atoi(buf);

	
	if (NULL == (buf = config.GetConfigName("listen_port")))
	{
		printf("read ServerSelf->listen_port error \n");
		return -1;
	}
	stconfig.listen_port = atoi(buf);

	
	if (NULL == (buf = config.GetConfigName("mq_vhost")))
	{
		printf("read RabbitMQInfo->mq_vhost error \n");
		return -1;
	}
	stconfig.mq_vhost = buf;


	
	if (NULL == (buf = config.GetConfigName("mq_exchange")))
	{
		printf("read RabbitMQInfo->mq_exchange error \n");
		return -1;
	}
	stconfig.mq_exchange = buf;


	
	if (NULL == (buf = config.GetConfigName("mq_exchangeGroup")))
	{
		printf("read RabbitMQInfo->mq_exchangeGroup error \n");
		return -1;
	}
	stconfig.mq_exchangeGroup = buf;


	
	if (NULL == (buf = config.GetConfigName("mq_address")))
	{
		printf("read RabbitMQInfo->mq_address error \n");
		return -1;
	}
	stconfig.mq_address = buf;

	
	
	if (NULL == (buf = config.GetConfigName("mq_port")))
	{
		printf("read RabbitMQInfo->mq_port error \n");
		return -1;
	}
	stconfig.mq_port = buf;

	
	
	if (NULL == (buf = config.GetConfigName("mq_user")))
	{
		printf("read RabbitMQInfo->mq_user error \n");
		return -1;
	}
	stconfig.mq_user = buf;


	
	if (NULL == (buf = config.GetConfigName("mq_password")))
	{
		printf("read RabbitMQInfo->mq_password error \n");
		return -1;
	}
	stconfig.mq_password = buf;


	
	if (NULL == (buf = config.GetConfigName("mq_queueid")))
	{
		printf("read RabbitMQInfo->mq_queueid error \n");
		return -1;
	}
	stconfig.mq_queueid = atoi(buf);


	if (NULL == (buf = config.GetConfigName("logdir")))
	{
		printf("read RabbitMQInfo->logdir error \n");
		return -1;
	}
	stconfig.logdir = buf;

	

	// logon mq info 
	if (NULL == (buf = config.GetConfigName("mq_vhost_lg")))
	{
		printf("read RabbitMQInfo->mq_vhost_lg error \n");
		return -1;
	}
	stconfig.mq_vhost_lg = buf;


	
	if (NULL == (buf = config.GetConfigName("mq_exchange_lg")))
	{
		printf("read RabbitMQInfo->mq_exchange_lg error \n");
		return -1;
	}
	stconfig.mq_exchange_lg = buf;


	
	if (NULL == (buf = config.GetConfigName("mq_exchangeGroup_lg")))
	{
		printf("read RabbitMQInfo->mq_exchangeGroup_lg error \n");
		return -1;
	}
	stconfig.mq_exchangeGroup_lg = buf;


	
	if (NULL == (buf = config.GetConfigName("mq_address_lg")))
	{
		printf("read RabbitMQInfo->mq_address_lg error \n");
		return -1;
	}
	stconfig.mq_address_lg = buf;

	
	
	if (NULL == (buf = config.GetConfigName("mq_port_lg")))
	{
		printf("read RabbitMQInfo->mq_port_lg error \n");
		return -1;
	}
	stconfig.mq_port_lg = buf;

	
	
	if (NULL == (buf = config.GetConfigName("mq_user_lg")))
	{
		printf("read RabbitMQInfo->mq_user_lg error \n");
		return -1;
	}
	stconfig.mq_user_lg = buf;


	
	if (NULL == (buf = config.GetConfigName("mq_password_lg")))
	{
		printf("read RabbitMQInfo->mq_password_lg error \n");
		return -1;
	}
	stconfig.mq_password_lg = buf;


	
	if (NULL == (buf = config.GetConfigName("routingkey_lg")))
	{
		printf("read RabbitMQInfo->routingkey_lg error \n");
		return -1;
	}
	stconfig.routingkey_lg = buf;

	if (NULL == (buf = config.GetConfigName("bindingkey_lg")))
	{
		printf("read RabbitMQInfo->bindingkey_lg error \n");
		return -1;
	}
	stconfig.bindingkey_lg = buf;

	if (NULL == (buf = config.GetConfigName("queueid_lg")))
	{
		printf("read RabbitMQInfo->queueid_lg error \n");
		return -1;
	}
	stconfig.queueid_lg = atoi(buf);
	
	


	// histdata mq info 
	if (NULL == (buf = config.GetConfigName("mq_vhost_hist")))
	{
		printf("read RabbitMQInfo->mq_vhost_hist error \n");
		return -1;
	}
	stconfig.mq_vhost_hist = buf;



	if (NULL == (buf = config.GetConfigName("mq_exchange_hist")))
	{
		printf("read RabbitMQInfo->mq_exchange_hist error \n");
		return -1;
	}
	stconfig.mq_exchange_hist = buf;



	if (NULL == (buf = config.GetConfigName("mq_exchangeGroup_hist")))
	{
		printf("read RabbitMQInfo->mq_exchangeGroup_hist error \n");
		return -1;
	}
	stconfig.mq_exchangeGroup_hist = buf;



	if (NULL == (buf = config.GetConfigName("mq_address_hist")))
	{
		printf("read RabbitMQInfo->mq_address_hist error \n");
		return -1;
	}
	stconfig.mq_address_hist = buf;



	if (NULL == (buf = config.GetConfigName("mq_port_hist")))
	{
		printf("read RabbitMQInfo->mq_port_hist error \n");
		return -1;
	}
	stconfig.mq_port_hist = buf;



	if (NULL == (buf = config.GetConfigName("mq_user_hist")))
	{
		printf("read RabbitMQInfo->mq_user_hist error \n");
		return -1;
	}
	stconfig.mq_user_hist = buf;



	if (NULL == (buf = config.GetConfigName("mq_password_hist")))
	{
		printf("read RabbitMQInfo->mq_password_hist error \n");
		return -1;
	}
	stconfig.mq_password_hist = buf;


	if (NULL == (buf = config.GetConfigName("routingkey_hist")))
	{
		printf("read RabbitMQInfo->routingkey_hist error \n");
		return -1;
	}
	stconfig.routingkey_hist = buf;


	if (NULL == (buf = config.GetConfigName("bindingkey_hist")))
	{
		printf("read RabbitMQInfo->bindingkey_hist error \n");
		return -1;
	}
	stconfig.bindingkey_hist = buf;

	if (NULL == (buf = config.GetConfigName("queueid_hist")))
	{
		printf("read RabbitMQInfo->queueid_hist error \n");
		return -1;
	}
	stconfig.queueid_hist = atoi(buf);
	

	// read change notice mq info

	if (NULL == (buf = config.GetConfigName("mq_vhost_chge")))
	{
		printf("read RabbitMQInfo->mq_vhost_chge error \n");
		return -1;
	}
	stconfig.mq_vhost_chge = buf;


	
	if (NULL == (buf = config.GetConfigName("mq_exchangeGroup_chge")))
	{
		printf("read RabbitMQInfo->mq_exchangeGroup_chge error \n");
		return -1;
	}
	stconfig.mq_exchangeGroup_chge = buf;




	
	if (NULL == (buf = config.GetConfigName("mq_address_chge")))
	{
		printf("read RabbitMQInfo->mq_address_chge error \n");
		return -1;
	}
	stconfig.mq_address_chge = buf;

	
	
	if (NULL == (buf = config.GetConfigName("mq_port_chge")))
	{
		printf("read RabbitMQInfo->mq_port_chge error \n");
		return -1;
	}
	stconfig.mq_port_chge = buf;

	
	
	if (NULL == (buf = config.GetConfigName("mq_user_chge")))
	{
		printf("read RabbitMQInfo->mq_user_chge error \n");
		return -1;
	}
	stconfig.mq_user_chge = buf;


	
	if (NULL == (buf = config.GetConfigName("mq_password_chge")))
	{
		printf("read RabbitMQInfo->mq_password_chge error \n");
		return -1;
	}
	stconfig.mq_password_chge = buf;


	
	if (NULL == (buf = config.GetConfigName("mq_bindingkey_chge")))
	{
		printf("read RabbitMQInfo->mq_bindingkey_chge error \n");
		return -1;
	}
	stconfig.mq_bindingkey_chge = buf;



	if (NULL == (buf = config.GetConfigName("mq_queueid_chge")))
	{
		printf("read RabbitMQInfo->mq_queueid_chge error \n");
		return -1;
	}
	stconfig.mq_queueid_chge = atoi(buf);


	
	


	
	// end 


	if (NULL == (buf = config.GetConfigName("cm_process_time")))
	{
		printf("read cm_process_time error \n");
		return -1;
	}
	stconfig.cm_process_time = atoi(buf);


	
	if (NULL == (buf = config.GetConfigName("onlyscreenshowlog")))
	{
		printf("read onlyscreenshowlog error \n");
		return -1;
	}
	if (1 == atoi(buf))
		stconfig.onlyscreenshowlog = true;
	else
		stconfig.onlyscreenshowlog = false;
	

	
}



void InitLogSys()
{
	(*XLogger::GetObject("msg_tcp_svr")).StdErr(stConfinfo.onlyscreenshowlog).Colour(true).Dir(stConfinfo.logdir).BuffDelay(0);
	XINFO("InitLogSys ....");
}







void ShowHex(const char * pbuf, int len)
{
	int i = 0;
	for(i = 0;i< len; i++)
	{
		printf("0x%02x",pbuf[i]);
	}
	printf("\n");
}







std::size_t  DecodeHistoryHdr(historydata::MessageHeader &hdr, char *buffer, std::uint64_t offset, std::uint64_t bufferLength)
{
    hdr.wrap(buffer, offset, messageHeaderVersion, bufferLength);

    return hdr.encodedLength();
}



std::size_t DecodeRealtimeHdr(realtime::MessageHeader &hdr, char *buffer, std::uint64_t offset, std::uint64_t bufferLength)
{
    hdr.wrap(buffer, offset, messageHeaderVersion, bufferLength);

    return hdr.encodedLength();
}



std::size_t DecodeHistroyHdr(historydata::MessageHeader &hdr, char *buffer, std::uint64_t offset, std::uint64_t bufferLength)
{
    hdr.wrap(buffer, offset, messageHeaderVersion, bufferLength);

    return hdr.encodedLength();
}





std::size_t DecodeRealtimeBody(
    realtime::Realtimedatas &realtime, char *buffer, std::uint64_t offset, std::uint64_t actingBlockLength,
    std::uint64_t actingVersion, std::uint64_t bufferLength)
{
    realtime.wrapForDecode(buffer, offset, actingBlockLength, actingVersion, bufferLength);

    realtime::Realtimedatas::RealtimeDataList &st_realtime = realtime.realtimeDataList();
    while (st_realtime.hasNext())
    {
        st_realtime.next();
        std::cout << "status : " << st_realtime.status()   << endl;
        std::cout << "bid : " <<  st_realtime.bid()  << endl;
        std::cout << "ask : " <<  st_realtime.ask()  << endl;
        std::cout << "timstamp : " <<  st_realtime.timestamp()  << endl;
        std::cout << "instrument : " <<  st_realtime.instrument()  << endl;
        std::cout << "-----------------------------------------------------" << endl;
    }



    return realtime.encodedLength();
}



std::size_t DecodeRealtime(char *buffer,size_t bufferlen)
{
	
	realtime::MessageHeader hdr_dec;
	realtime::Realtimedatas realtime_dec ;	

	
	std::size_t decodeHdrLength = DecodeRealtimeHdr(hdr_dec, buffer, 0, sizeof(buffer));
	printf("encodedLength:%lu,blockLength:%lu,version:%lu\n",hdr_dec.encodedLength(),hdr_dec.blockLength(), hdr_dec.version());
	std::size_t decodeMsgLength = DecodeRealtimeBody(realtime_dec, buffer, hdr_dec.encodedLength(), hdr_dec.blockLength(), hdr_dec.version(), bufferlen);

	return 0;
}




std::size_t DecodeHistoryBody(
    historydata::CandlesData &candlesData, char *buffer, std::uint64_t offset, std::uint64_t actingBlockLength,
    std::uint64_t actingVersion, std::uint64_t bufferLength)
{
    candlesData.wrapForDecode(buffer, offset, actingBlockLength, actingVersion, bufferLength);

    historydata::CandlesData::Candles &candles = candlesData.candles();    


	// for test
	//return candlesData.encodedLength();

	printf("----adsfadsfa----------------\n");
	
	int i = 0;
    while (candles.hasNext())
    {
        candles.next();
        std::cout << "openBid : " << candles.openBid()   << endl;
        std::cout << "openAsk : " << candles.openAsk()   << endl;
        std::cout << "highBid : " << candles.highBid()   << endl;
        std::cout << "highAsk : " << candles.highAsk()   << endl;
        std::cout << "lowBid : " << candles.lowBid()   << endl;
        std::cout << "lowAsk : " << candles.lowAsk()   << endl;
        std::cout << "closeBid : " << candles.closeBid()   << endl;
        std::cout << "closeAsk : " << candles.closeAsk()   << endl;
        std::cout << "volumn : " << candles.volumn()   << endl;
        std::cout << "complete : " << candles.complete()   << endl;
        std::cout << "time : " << candles.time()   << endl;
        std::cout << ""<< ++i <<"-----------------------------------------------------" << endl;
    }



    return candlesData.encodedLength();
}




std::size_t DecodeHistory(char *buffer,size_t bufferlen)
{	
	historydata::MessageHeader hdr_dec;
	historydata::CandlesData   candles_dec ;

	
	std::size_t decodeHdrLength = DecodeHistoryHdr(hdr_dec, buffer, 0, sizeof(buffer));
	std::size_t decodeMsgLength = DecodeHistoryBody(candles_dec, buffer, hdr_dec.encodedLength(), hdr_dec.blockLength(), hdr_dec.version(),bufferlen);

	return 0;
}







