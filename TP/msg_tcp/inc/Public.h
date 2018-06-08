

/*******************************************************************************************

*Department:	TP

*Decrible:  	多个模块共享的数据结构
				
				


*Auhor:			Savin Chen

*Createdtime:	2017-05-08


*ModifyTime:

savin 2017-05-26:   	增加Longo模块方面的协议数据
savin 2017-05-31:		增加历史行情查询方面的协议数据	
savin 2017-08-21:		更换全新的new_tnode 





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
#include "marketdataagent.h"
#include <ev.h>
#include"xlogger.h"



#include "realtime/MessageHeader.h"
#include "realtime/Realtimedatas.h"

#include "history/MessageHeader.h"
#include "history/CandlesData.h"




using namespace std;
using namespace snetwork_signalengine_marketdata;
using namespace snetwork_xservice_xflagger;



// 币种个数
#define COINNUMBER 200

// 每个币种的长度 
#define COINMAXLEN 12

//行情的最大长度 
#define	COINPRICEMAXLEN 24

// 每个币种行情内容的长度 
#define COINCONNECTMAXLEN 136

#define SRVVERSION 1

#define STARTER		0X008F

#define SENDFAILTIMES 60


#define MAXHISTORYDATA 350*2000


typedef unsigned int uint32;
typedef unsigned short uint16;



// #SBE
extern int messageHeaderVersion ;



typedef struct _STConfInfo
{
	
	// server 
	int 	send_intertime;
	int 	memnode_count;
	int 	worker_count;
	int 	listen_port;


	//logon mqinfo
	string 	mq_vhost_lg;
	string 	mq_exchange_lg;
	string 	mq_exchangeGroup_lg;
	string 	mq_address_lg;
	string 	mq_port_lg;
	string 	mq_user_lg;
	string 	mq_password_lg;
	
	string 	routingkey_lg;  	
	string 	bindingkey_lg;		
	int		queueid_lg	 ;   
	

	//realtimedata mqinfo
	string 	mq_vhost;
	string 	mq_exchange;
	string 	mq_exchangeGroup;
	string 	mq_address;
	string 	mq_port;
	string 	mq_user;
	string 	mq_password;
	int 	mq_queueid;


	//histdata mqinfo
	string 	mq_vhost_hist;
	string 	mq_exchange_hist;
	string 	mq_exchangeGroup_hist;
	string 	mq_address_hist;
	string 	mq_port_hist;
	string 	mq_user_hist;
	string 	mq_password_hist;

	string 	routingkey_hist;  	
	string 	bindingkey_hist;		
	int		queueid_hist	;		
	
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
	
	// cm
	int 	cm_process_time;

	// log info
	string 	logdir;
	bool 	onlyscreenshowlog;
	
}STConfInfo;

extern STConfInfo stConfinfo;





/*
****************  @ 错误码  ****************
*/

enum ERRORCODE
{
	ERRCODE_OK						= 0,				// 正确
	ERRCODE_PARSEERR				= 1,				// 协议解析失败
	ERRCODE_UNLOGON					= 2,				// 未登录
	ERRCODE_INERROR					= 3					//  系统内部错误
	// 待扩

};


/*
****************  @ 消息编号  ****************
*/
enum MSGID
{
	
	RTMSG_REQUEST					= 0x4001,				// 实时消息
	RTMSG_REQUEST_ACK				= 0x4002,				// 实时消息回馈

	RTMSG_PUSH						= 0x4003,				// 实时消息推送
	RTMSG_BO_PUSH					= 0x4004,				// BO实时消息推送


	CURRENCYSYMBOL_REQUEST			= 0x4100,				//货币对spread改变请求
	CURRENCYSYMBOL_REQUEST_ACK		= 0x4101,				//醣叶詓pread改变请求 的 ACK
	CURRENCYGROUP_REQUEST			= 0x4102,				// 组里货币对改变 请求
	CURRENCYGROUP_REQUEST_ACK		= 0x4103,				// 组里货币对改变 请求 的ACK
	
	
	

	HISTMSG_REQUEST					= 0xF003,				// 历史消息
	HISTMSG_REQUEST_ACK				= 0xF004,				// 历史消息回馈
	HISTMSG_REQUEST_SMALLPACK		= 0xF005,				// 历史消息 小包请求

	LOGIN_REQUEST					= 0xF001,				// 登录消息
	LOGIN_REQUEST_ACK				= 0xF002				// 登录反馈消息

};





enum MSGOPTYPE
{
	RT_MSG_DELETE						= 0,				// 实时行情删除
	RT_MSG_ADD							= 1,				// 实时行情新增
	RT_MSG_UPDATE						= 2					// 实时行情更新
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



// -- ---- 登录包 体 ----
typedef struct __STLogonBody
{
unsigned int 	groupid;
unsigned int	clientid;
char			password[32];
} __attribute__((packed))STLogonBody;



// -- ---- 登录包  ----
typedef struct __STLogonNode
{
	STMsgHeader 		header;
	STLogonBody			body;

	__STLogonNode()
	{
		Clean();
	}

	void Clean()
	{
		memset(&header,0,sizeof(header));
		memset(&body,0,sizeof(body));
	}

} __attribute__((packed))STLogonNode;



// -- ---- 登录反馈包 体 ------
typedef struct __STLogonBodyAck
{
	unsigned int 	groupid;
	unsigned int	clientid;
	char			symbollist[COINNUMBER*COINMAXLEN];
	int		 		errNo;
} __attribute__((packed))STLogonBodyAck;


// -- ---- 登录反馈包 ------
typedef struct __STLogonNodeAck
{
	STMsgHeader 		header;
	STLogonBodyAck		body;
	
	__STLogonNodeAck()
	{
		Clean();
	}

	void Clean()
	{
		memset(&header,0,sizeof(header));
		memset(&body,0,sizeof(body));
	}

} __attribute__((packed))STLogonNodeAck;





// for log
typedef struct _STMsgToDC
{
	char			requestid[36];
	unsigned int	groupid;
	unsigned int	clientid;
	unsigned int	candlecounts;			
	char 			coinname[8];			//EUR_USD
	char 			granularity[4];			// 粒度:  分钟线, 时钟线等
	int 			bodylen; // or msg index  一值两用
}__attribute__((packed))STMsgToDC;




// -- ---- 历史数据 请求包 体 ------
typedef struct __STHistDataRequestBody
{
	unsigned int 	groupid;
	unsigned int 	clientid;
	unsigned int	candlecounts;			
	char 			coinname[8];			//EUR_USD
	char 			granularity[4];			// 粒度:  分钟线, 时钟线等
} __attribute__((packed))STHistDataRequestBody;





// -- ---- 历史数据 请求包  ------
typedef struct __STHistDataRequest
{
	STMsgHeader 			header;
	STHistDataRequestBody	body;
	
	__STHistDataRequest()
	{
		Clean();
	}

	void Clean()
	{
		memset(&header,0,sizeof(header));
		memset(&body,0,sizeof(body));
	}

} __attribute__((packed))STHistDataRequest;




// -- ---- 历史数据 返回包 体 ------
typedef struct __STHistDataResponseBody
{
	unsigned int 	groupid;
	unsigned int	clientid;
	int		 		errNo;

	
	int 			CandleCounts ;
	char 			coinname[8];
	char 			granularity[4];
	int 			datalen;
	char 			data[MAXHISTORYDATA];	

	
			
} __attribute__((packed))STHistDataResponseBody;



// -- ---- 历史数据 返回包  ------
typedef struct __STHistDataResponse
{
	STMsgHeader 			header;
	STHistDataResponseBody	body;
	
	__STHistDataResponse()
	{
		Clean();
	}

	void Clean()
	{
		memset(&header,0,sizeof(header));
		memset(&body,0,sizeof(body));
	}

} __attribute__((packed))STHistDataResponse;





// -- ---- 实时数据包 体 ------
typedef struct __STRTMsgBody
{
	
	unsigned short 	optype;		 // 参考MSGOPTYPE
	unsigned short 	msgnumber;	 // 币种个数
	unsigned int 	groupid;
	unsigned int	clientid;
	char 			coinarray[COINNUMBER*COINMAXLEN];  // 最多多 80个币种, 每个币种最大字符 不超过15个字段	, 币种之间 分号 :

} __attribute__((packed)) STMsgBody;


// -- ---- 实时数据包  ------
typedef struct __STRTMsgRequest
{
	STMsgHeader 		header;
	STMsgBody			body;

	__STRTMsgRequest()
	{
		Clean();
	}

	void Clean()
	{
		memset(&header,0,sizeof(header));
		memset(&body,0,sizeof(body));
	}

} __attribute__((packed)) STRTMsgRequest;



// -- ---- 实时数据反馈包 ------
typedef struct __STMsgRequestAck
{
	STMsgHeader 	header;
	unsigned int 	groupid;
	unsigned int	clientid;
	unsigned int	errNo;				// 错误码, 0 就是正确, 	

	
	__STMsgRequestAck()
	{
		Clean();
	}

	void Clean()
	{
		memset(&header,0,sizeof(header));
		groupid = 0;
		clientid = 0;
		errNo = 0;
	}

} __attribute__((packed)) STMsgRequestAck;



// -- ---- 实时数据包 推送包 ------
typedef struct __STRTMsgToClient
{
	STMsgHeader 	header;
	unsigned long	srv_seqid;	 
	unsigned int 	groupid;
	unsigned int	clientid;
	unsigned short 	msgnumber;					// 币种个数
	char			coinresponse[COINNUMBER*COINCONNECTMAXLEN+COINNUMBER];  //{"instrument":"EUR_USD","status":1,"bid":1086960,"ask":1087080,"timestamp":1494556542338992}
	

} __attribute__((packed)) STRTMsgToClient;





typedef struct _STDataNode STDataNode;




typedef struct _STConnNode
{
	unsigned int 	ip;				// OUT/IN 连接到SERVER时的 ip,port; 不是协议里的ip,port
	unsigned short 	port;
	int 			socket;
	struct timeval  conntime;		// 连接的时间
	struct ev_io	*pClientEvent;
	STDataNode 		*pDataNode;		// 用于一次没有读完用
	int				logon;			// 0: 没有登录, 1 登录成功

	unsigned long	srv_seqid;	 

	int 			sendfailtimes ;

	struct ev_loop *loop ;
	struct ev_io *watcher ;
	void Clean()
	{
		ip = 0;
		srv_seqid = 0;
		port = 0;
		socket = 0;
		conntime.tv_sec = 0;
		conntime.tv_usec = 0;
		pDataNode = NULL;
		logon = 0 ;
		loop = NULL;
		watcher = NULL;
		sendfailtimes = 0;
	}

	_STConnNode()
	{
		Clean();
	}


	// need portion field  only
	_STConnNode &  operator=(_STConnNode &thr)
	{
		ip = thr.ip;
		srv_seqid = thr.srv_seqid;
		port = thr.port;
		socket = thr.socket;
		conntime.tv_sec = thr.conntime.tv_sec;
		conntime.tv_usec = thr.conntime.tv_usec;
		pDataNode = thr.pDataNode;
		pClientEvent = thr.pClientEvent;
		logon = thr.logon;
		loop = thr.loop;
		watcher = thr.watcher;
		
		return *this;
	}
	

} STConnNode;




typedef struct _STCoinPairInfo
{
	int status;
	int bid;
	int ask;
	unsigned long int timestamp;

	void Clean()
	{
		bid = 0;
		ask = 0;
		timestamp = 0;
	}
	_STCoinPairInfo()
	{
		Clean();
	}
}STCoinPairInfo;






typedef struct _STDataNode
{
	STRTMsgRequest		*pRequest;
	STLogonNode			*pLogon;
	STHistDataRequest	*pHistData;

	unsigned int 		nDataLen;							// 已读长度
	unsigned int		nMsgLen;							// 消息长度	
	char 				szData[sizeof(STRTMsgRequest)]; 	// 所有协议中最大的数据	
	
	map<string,STCoinPairInfo> 	coinlist;					// coinname price ,price 初始化为0 
	STConnNode 			*pConn;
	STConnNode 		 	objConn;
	int 				errNo;

	void Clean()
	{
		pLogon = NULL;
		pRequest = NULL;
		nDataLen = 0;
		nMsgLen = 0;
		memset(szData,0,sizeof(szData));
		coinlist.clear();
		pConn = NULL;
		memset(&objConn,0,sizeof(objConn));
		errno = 0;
	}
	
	_STDataNode()
	{
		Clean();
	}


	// need portion field  only
	_STDataNode &  operator=(_STDataNode &thr)
	{
		nDataLen = thr.nDataLen;
		nMsgLen = thr.nMsgLen;
		memcpy(szData,thr.szData,thr.nMsgLen);
		pConn = thr.pConn;
		objConn = thr.objConn;
		errNo = thr.errNo;
		return *this;
	}
	

} STDataNode;



typedef struct __STGroupSpread
{
	STMsgHeader 	header;
	int				othervalue;  // groupid or errno

	void Clean()
	{
		memset(&header,0,sizeof(header));
		othervalue = 0;
	}

	__STGroupSpread()
	{
		Clean();
	}
	
} __attribute__((packed)) STGroupSpread ;


void 		SetMsgHeader(STMsgHeader &header,unsigned short len,unsigned char version,unsigned int msgid,char *pRequestId);


void 		CheckExit();



/*
@ return :
-1:  sokect error
 0:	 un read conplish
>0:  read a package, readlen
*/

ssize_t  	recvforlen(int fd, void *vptr, size_t n)  ;



ssize_t   	sendforlen(int fd, const void *vptr, size_t n,const char *pRequestid)  ;


int SendToClient(STDataNode *pNode,void * pack, int len,const char *pRequestid);


int 		setnonblock(int fd);

const char * iptostrip(unsigned int ip) ;

unsigned int striptoip(const char *ip);


int 	StartListenModule(int nServicePort);

void PrintCurrTime(const char *pbuf);


int 	ReadConfig(const char * file, STConfInfo &stconfig);

void 	InitLogSys();



void ShowHex(const char * pbuf, int len);



std::size_t  DecodeHistoryHdr(historydata::MessageHeader &hdr, char *buffer, std::uint64_t offset, std::uint64_t bufferLength) ;

std::size_t DecodeRealtimeHdr(realtime::MessageHeader &hdr, char *buffer, std::uint64_t offset, std::uint64_t bufferLength) ;

std::size_t DecodeHistroyHdr(historydata::MessageHeader &hdr, char *buffer, std::uint64_t offset, std::uint64_t bufferLength);

std::size_t DecodeRealtimeBody(
    realtime::Realtimedatas &realtime, char *buffer, std::uint64_t offset, std::uint64_t actingBlockLength,
    std::uint64_t actingVersion, std::uint64_t bufferLength) ;


std::size_t DecodeRealtime(char *buffer,size_t bufferlen) ;
std::size_t DecodeHistoryBody(
    historydata::CandlesData &candlesData, char *buffer, std::uint64_t offset, std::uint64_t actingBlockLength,
    std::uint64_t actingVersion, std::uint64_t bufferLength);


std::size_t DecodeHistory(char *buffer,size_t bufferlen);




#endif //DISTRIBUTE_PUBLIC_HEAD_H

