#ifndef _COMCLI_S_H_
#define _COMCLI_S_H_

#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <stdio.h>    
#include <errno.h>    
#include <unistd.h>    
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>    
#include <sys/epoll.h>    
#include <netinet/in.h>    

#include "log.h"
#include "tcp.h"
#include "const.h"
#include "kerner.h"
#include "msgopr.h"
#include "tyinhttp.h"
#include "./json/writer.h"
#include "./json/features.h"
#include "./json/forwards.h"
#include "./curl/curl.h"
#include "./openssl/ssl.h"
#include "./openssl/rand.h"
#include "./openssl/err.h"

using namespace std;


extern "C" void cmd_msg_cb(int fd, short events, void* arg);  
extern "C" void server_msg_cb(struct bufferevent* bev, void* arg);  
extern "C" void event_cb(struct bufferevent *bev, short event, void *arg);  


#endif
