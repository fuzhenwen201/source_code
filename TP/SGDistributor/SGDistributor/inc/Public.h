
/*******************************************************************************************

*Department:	TP

*Decrible:  	多个模块共享的数据结构
				


*Auhor:			Savin Chen

*Createdtime:	2017-04-19

*ModifyTime:	2017-08-15 Savin 添加SG域名映射功能


********************************************************************************************/


#ifndef DISTRIBUTE_PUBLIC_HEAD_H
#define DISTRIBUTE_PUBLIC_HEAD_H


#include <algorithm>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <errno.h>
#include "event.h"
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <map>
#include <list>
#include <arpa/inet.h>

#include"xlogger.h"


using namespace snetwork_xservice_xflagger;

using namespace std;

// 分配节点的数量
#define MEMNODE_COUNT 100000

#define READTIMES     10


// 要么原路分发策略,  要么平均分配策略
extern int g_nDistributePolicy ;

// 外部客户端超时时间 30s
#define OUTCLIENTTIMEOUT 30+10


#define SRVVERSION 1

// 外部服务
#define OUTSERVICE 1

// 内部服务
#define INSERVICE  0


#define USERIDLEN 32


#define STARTER		0X008F


typedef unsigned int uint32;
typedef unsigned short uint16;



typedef struct _STConfInfo
{
	int 	nOutServicePort;
	int 	nInServicePort;
	int		nWorkerNumber;
	int 	nDistributePolicy;
	int 	nMemNodeCount;

	//change notice mqinfo
	string 	mq_vhost_chge;
	string 	mq_exchange_chge;
	string 	mq_exchangeGroup_chge;
	string 	mq_address_chge;
	string 	mq_port_chge;
	string 	mq_user_chge;
	string 	mq_password_chge;
	string  mq_bindingkey_chge;
	int 	mq_queueid_chge;
	

	// log info
	string logdir;
	bool 	onlyscreenshowlog;
	
}STConfInfo;

extern STConfInfo stConfinfo;




/*
****************  @ 消息编号  ****************
*/
enum MSGID
{
	MSG_GATEWAY_REQ					= 0x0010,				//  GW 消息
	MSG_GATEWAY_ACK					= 0x0011,				//  GW 消息
	
	MSG_DISTRIBUTE_OLDROUTE_REQ 	= 0x1000,				// 原路分发策略, 如果原路不存在 ，结合1001 处理
	MSG_DISTRIBUTE_OLDROUTE_ACK 	= 0x1001,				// 原路分发策略, 如果原路不存在 ，结合1001 处理, 不用
	MSG_DISTRIBUTE_AVERAGE_REQ  	= 0x1002,				// 平均分配策略 
	MSG_DISTRIBUTE_AVERAGE_ACK  	= 0x1003,				// 平均分配策略 
	 	
	MSG_URLMAP_REQ					= 0x2000,				// SG url 映射 REQQUEST
	MSG_URLMAP_REQ_ACK				= 0x2001,				// SG url 映射 ACK
	MSG_DEFEED_BLACKLIST_REQ		= 0x2002,				// 防御黑名单 REQQUEST
	MSG_DEFEED_BLACKLIST_ACK		= 0x2003,				// 防御黑名单 ACK

	MSG_OUTCLIENT_GW_REQ			= 0x3000,				// 外部客户端 路由请求
	MSG_OUTCLIENT_GW_ACK			= 0x3001				// 外部客户端 路由请求 回馈

};



/*
****************  @ 错误码  ****************
*/

enum ERRORCODE
{
	ERRCODE_OK						= 0,				// 正确
	ERRCODE_NOGW					= 1,				// 没有找到GW 的 ip,port
	ERRCODE_PARSEERR				= 2,				// 协议解析失败

	ERRCODE_NOURL					= 3	,				// 根据IP没有找到URL
	ERRCODE_NOSGIP					= 4	,				// 根据URL没有找到IP
	
	// 待扩

};




/*
****************  @ 消息头  ****************
*/

typedef struct __STMsgHeader
{
	unsigned int	starter;				//  0x008F
	unsigned short  msglen;					// 消息总长度
	unsigned short 	version;				// 版本号: 目前 为 1
	unsigned int  	msgid;		
	char 			requestid[36];			

} __attribute__((packed)) STMsgHeader;




/*
****************  @ 外部协议BODY, FOR TT  ****************
*/
typedef struct _STOutMsgBody
{
	//unsigned int 	gwip;			//客户端 存储的 上次登录的ip, 没有就为0
	char			gwurl[66];		
	unsigned short 	gwport;			//客户端 存储的 上次登录的port,没有就为0
	unsigned int 	groupid;
	unsigned int	clientid;

} __attribute__((packed)) STOutMsgBody;




// ------外部协议  GW请求协议 ------

typedef struct __STOutAppMsgRequest
{
	STMsgHeader 	msgheader;			// 头部
	STOutMsgBody	msgbody;			// 主体
	
	__STOutAppMsgRequest()
	{
		memset(&msgheader,0,sizeof(msgheader));
		memset(&msgbody,0,sizeof(msgbody));
	}

}__attribute__((packed))  STOutAppMsgRequest;




// ------外部协议  GW 应答协议 ------
typedef struct __STOutAppMsgAck
{
	STMsgHeader 	msgheader;			// 头部
	unsigned int	errNo;				// 错误码, 0 就是正确, 	
	STOutMsgBody	msgbody;			// 主体
	
	__STOutAppMsgAck()
	{
		memset(&msgheader,0,sizeof(msgheader));
		memset(&msgbody,0,sizeof(msgbody));
		errNo = ERRCODE_OK;
	}
	
}__attribute__((packed))  STOutAppMsgAck;







/*
****************  @ 内部协议BODY, FOR monitor  ****************
*/


// ------内部服务gw协议  Body------
typedef struct _STINServiceGWBody
{
	unsigned short	op_type;			//  1 : INSERT OR UPDATE 0 : DELETE
	unsigned int 	gwip ;        		//内部gw的 ip   	
	//char			gwurl[66];			//sg ip对应的域名
	unsigned int 	gwport; 			//内部gw的 port			
	unsigned int 	curusercount ;		//内部gw 的当前用户数	
	unsigned int 	maxusercount ;		//内部gw 的最大用户数	
} __attribute__((packed))  STINServiceGWBody;
	




// ------内部服务gw协议    ------
typedef struct _STINServiceGWMsg
{
	
	STMsgHeader 	msgheader;			// 头部
	unsigned short  monitor_id;			//
	unsigned short	inclient_type;		// 内部客户端类型澹 0: gw, other: monitor
	unsigned short  numbers	;			//协议数据的个数,  每次最多不超过20个
	// 每个GW,都有以下字段
	STINServiceGWBody body;

	_STINServiceGWMsg()
	{
		memset(&msgheader,0,sizeof(msgheader));
		inclient_type = 0;
		numbers = 0;
		memset(&body,0,sizeof(body));
	}

} __attribute__((packed))  STINServiceGWMsg;
	







// 内部服务分发策略协议
typedef struct _STINServiceDistributeMsg
{
	STMsgHeader 	msgheader;			// 头部
	
} __attribute__((packed))  STINServiceDistributeMsg;
	





// ------内部服务协议 ACK    ------
typedef struct _STInServiceAck
{
	STMsgHeader 	msgheader;			// 头部
	unsigned int	errNo;				// 错误码, 0 就是正确, 			

}STInServiceAck;





// -------- 内存池节点 -------------

typedef struct _STDataNode STDataNode;



typedef struct _STConnNode
{
	unsigned int 	ip;				// OUT/IN 连接到SERVER时的 ip,port; 不是协议里的ip,port
	unsigned short 	port;
	int 			socket;
	struct timeval  conntime;		// 连接的时间
	struct event *	pClientEvent;
	int 			isOutService;  	// 1: 外部服务;  其他 内部服务
	STDataNode 		*pDataNode;		// 指向_STDataNode 

	unsigned int 	gwip;			// gw ip, gwport
	unsigned short 	gwport;		
	unsigned char 	sendflag;		//		
	void Clean()
	{
		ip = 0;
		port = 0;
		socket = 0;
		conntime.tv_sec = 0;
		conntime.tv_usec = 0;
		isOutService = 0;
		pDataNode = NULL;
		gwip = 0;
		gwport = 0;
		sendflag = 0;
	}

	_STConnNode &  operator=(_STConnNode &thr)
	{
		ip = thr.ip;
		port = thr.port;
		socket = thr.socket;
		conntime.tv_sec = thr.conntime.tv_sec;
		conntime.tv_usec = thr.conntime.tv_usec;
		pClientEvent = thr.pClientEvent;
		isOutService = thr.isOutService;
		pDataNode = thr.pDataNode;
		gwip = thr.gwip;
		gwport = thr.gwport;
		sendflag = thr.sendflag;
		return *this;
	}

}STConnNode;


typedef struct _STDataNode
{	
	int 			nDataLen;		// 已读长度
	int				nMsgLen;		// 消息长度		
	char 			szData[360]; 	// 所有协议中最大的数据

	int 			nReadtimes;

	STConnNode *pConn;			
	STConnNode  objConn;

	void Clean()
	{
		nDataLen = 0 ;
		nMsgLen = 0;
		memset(szData,0,sizeof(szData));
		pConn = NULL;
		memset(&objConn,0,sizeof(objConn));

		nReadtimes = 0;
	}

	_STDataNode()
	{
		Clean();
	}


}STDataNode;





extern int g_ExitFlag ;


void SetMsgHeader(STMsgHeader &header,unsigned short len,unsigned char version,unsigned int msgid,char *pRequestId);


void CheckExit();



/*
@ return :
-1:  sokect error
 0:	 un read conplish
>0:  read a package, readlen
*/

ssize_t  recvforlen(int fd, void *vptr, size_t n)  ;



ssize_t   sendforlen(int fd, const void *vptr, size_t n)  ;


int setnonblock(int fd);

const char * iptostrip(unsigned int ip) ;

unsigned int striptoip(const char *ip);


int ProcessCMD(int argc,char *argv[]);


int ReadConfig(const char * file, STConfInfo &stconfig);


void InitLogSys();





#endif //DISTRIBUTE_PUBLIC_HEAD_H

