
#include <signal.h>

#include "Public.h"
#include "WokerModule.h"
#include "ConfigFileReader.h"





int g_nDistributePolicy ;

int g_ExitFlag 	= 0 ;


STConfInfo stConfinfo ;



void SetMsgHeader(STMsgHeader &header,unsigned short len,unsigned char version,unsigned int msgid,char *pRequestId)
{
	header.starter = htonl(STARTER);
	header.msglen = htons(len);
	header.version =htons(version);
	header.msgid = htonl(msgid);

	memcpy(header.requestid,pRequestId,sizeof(header.requestid));

	XINFO("SetMsgHeader:starter:0x%04x,msgid:0x%04x, msglen:%d,version:%d,requestid:%s \n",STARTER,msgid, len ,version,pRequestId);

}





void CheckExit()
{
	// v0.1 version
	while(1)
	{
		sleep(10);
	}

}







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




ssize_t   sendforlen(int socket, const void *vptr, size_t n)  
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
			XINFO("sendforlen: socket:%d, EAGAIN error,fail : %s\n",socket,strerror(errno));
			return 0;
		}
		if( nwritten == 0 || nwritten < 0 )
		{
			XERROR("sendforlen: socket:%d,  other error,  fail :%s\n", socket,strerror(errno));
			return -1;
		}
	  	nleft -= nwritten;  
	  	ptr   += nwritten;  
	 }  
	 
	 return(n);  
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




void InitLogSys()
{
	(*XLogger::GetObject("sgdistributor")).StdErr(stConfinfo.onlyscreenshowlog).Colour(true).Dir(stConfinfo.logdir).BuffDelay(0);
	XINFO("InitLogSys ....");
}



int ReadConfig(const char * file, STConfInfo &stconfig)
{
	assert(file!=NULL);

	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigaction( SIGPIPE, &sa, 0 );

	
	char *buf = NULL;
	CConfigFileReader config;	
	
	if( -1 == config.LoadFile(file)  ) 
	{
		printf("open config file: %s,fail:%s\n",file, strerror(errno));
		return -1;
	}


	if (NULL == (buf = config.GetConfigName("OutServicePort")))
	{
		printf("read OutServicePort error \n");
		return -1;
	}
	stconfig.nOutServicePort = atoi(buf);


	if (NULL == (buf = config.GetConfigName("WorkerNumber")))
	{
		printf("read WorkerNumber error \n");
		return -1;
	}
	stconfig.nWorkerNumber = atoi(buf);


	if (NULL == (buf = config.GetConfigName("DistributePolicy")))
	{
		printf("read DistributePolicy error \n");
		return -1;
	}
	stconfig.nDistributePolicy = atoi(buf);

	if (NULL == (buf = config.GetConfigName("MemNodeCount")))
	{
		printf("read MemNodeCount error \n");
		return -1;
	}
	stconfig.nMemNodeCount = atoi(buf);



	// read mq change
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




	if (NULL == (buf = config.GetConfigName("logdir")))
	{
		printf("read logdir error \n");
		return -1;
	}
	stconfig.logdir = buf;
	
	
	if (NULL == (buf = config.GetConfigName("onlyscreenshowlog")))
	{
		printf("read onlyscreenshowlog error \n");
		return -1;
	}
	if( 1 == atoi(buf) )
		stconfig.onlyscreenshowlog = true;
	else
		stconfig.onlyscreenshowlog = false;
	

	return 0;
}


