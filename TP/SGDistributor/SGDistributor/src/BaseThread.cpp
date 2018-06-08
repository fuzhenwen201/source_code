

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "BaseThread.h"
#include "Public.h"

void* CBaseThread::ThreadFunc(void* pThread)  
{  
	CBaseThread* p = (CBaseThread *)pThread;  
    p->Run();  
	XINFO("worker thread exit ...\n");

	return NULL;
}  
  

int CBaseThread::Start()  
{
	pthread_attr_t attr;
	pthread_attr_init (&attr);   
  	pthread_attr_setscope (&attr, PTHREAD_SCOPE_SYSTEM);   
  	pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);  
	
	if (0 != pthread_create(&m_nThreadID, &attr, ThreadFunc, this) )
	{
		XERROR("pthread_create error:%s\n",strerror(errno));
		return -1;
	}

	return 0;
}
