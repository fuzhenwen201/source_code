/*******************************************************************************************

*Department:	TP

*Decrible:  	线程基类
				
				


*Auhor:			Savin

*Createdtime:	2017-04-19

*ModifyTime:	


********************************************************************************************/


#ifndef BASETHREAD_HEAD_H
#define BASETHREAD_HEAD_H

#include <pthread.h>


class CBaseThread  
{  
public:  
   
    int 			Start()   ;
    static void* 	ThreadFunc(void* pThread)  ; 
    virtual void 	Run() = 0;  
  
private:  
    pthread_t 		m_nThreadID;  
};




#endif //

