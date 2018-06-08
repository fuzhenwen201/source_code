#ifndef _COMSVR_APP_H_
#define _COMSVR_APP_H_

#include <stdio.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <map>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <sys/epoll.h>
#include <semaphore.h>

#include "log.h"
#include "tcp.h"
#include "const.h"
#include "common.h"


using namespace std;


typedef struct connection_st {
	int sock;
	int index;
	int inuse;
	int roff;
	int woff;
	IpcMsgBuf ipcBuf;
}*connection_t;

typedef struct sockinfo{
	struct epoll_event event;
	time_t ltime;
	int shmid;
}*pconninfo;


#endif
