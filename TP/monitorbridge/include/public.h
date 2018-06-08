//
// Created by kevin on 7/6/17.
//

#ifndef TCPMODEL_PUBLIC_H
#define TCPMODEL_PUBLIC_H

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <arpa/inet.h>
#include <cerrno>
#include <ev.h>
#include "xlogger.h"
/*
****************  @ 消息头  ****************
*/
typedef struct __STMsgHeader{
    unsigned int	starter;				//  0x008F
    unsigned short  msglen;					// 消息总长度
    unsigned short 	version;				// 版本号: 目前 为 1
    unsigned int  	msgid;
    char 			requestid[36];

} __attribute__((packed)) STMsgHeader;

// ------内部服务gw协议  Body------
typedef struct _STINServiceGWBody{
    unsigned short	op_type;			//  1 : INSERT OR UPDATE 0 : DELETE
    unsigned int 	gwip ;        		//内部gw的 ip
    unsigned int 	gwport; 				//内部gw的 port
    unsigned int 	curusercount ;		//内部gw 的当前用户数
    unsigned int 	maxusercount ;		//内部gw 的最大用户数
} __attribute__((packed))  STINServiceGWBody;

// ------内部服务gw协议    ------
typedef struct _STINServiceGWMsg{
    STMsgHeader 	msgheader;			// 头部
    unsigned short  monitor_id;			//
    unsigned short	inclient_type;		// 内部客户端类型澹 0: gw, other: monitor
    unsigned short  numbers	;			//协议数据的个数,  每次最多不超过20个

    _STINServiceGWMsg()
    {
        memset(&msgheader,0,sizeof(msgheader));
        monitor_id = 0;
        inclient_type = 0;
        numbers = 0;
    }
} __attribute__((packed))  STINServiceGWMsg;

// ------内部服务协议 ACK    ------
typedef struct _STInServiceAck{
    STMsgHeader 	msgheader;			// 头部
    unsigned int	errNo;				// 错误码, 0 就是正确,

}STInServiceAck;

typedef struct __STOutAppMsgAck{
    STMsgHeader 	msgheader;			// 头部
    unsigned int	errNo;				// 错误码, 0 就是正确,

    __STOutAppMsgAck()
    {
        memset(&msgheader,0,sizeof(msgheader));
        errNo = 0;
    }

}__attribute__((packed))  STOutAppMsgAck;
#endif //TCPMODEL_PUBLIC_H
