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

#include "log.h"
#include "tcp.h"
#include "const.h"


using namespace std;


int listenfd;
int sock_op=1;
struct sockaddr_in address;
struct epoll_event event;
struct epoll_event events[1024];
int epfd;
int n;
int i;
char buf[512];
int off;
int result;
char *p;

typedef struct stsockStruct
{
	time_t time;
	int fd;
}sockStruct,*pSockStruct;

int init_thread_pool(int threadNum);
void *epoll_loop(void* para);
void *check_connect_timeout(void* para);


#define MAXRECVBUF 4096
#define MAXBUF MAXRECVBUF+10 

int fd_Setnonblocking(int fd)
{
	int op;

	op=fcntl(fd,F_GETFL,0);
	fcntl(fd,F_SETFL,op|O_NONBLOCK);

	return op;
}

void on_sigint(int signal)
{
	exit(0);
}


#endif
