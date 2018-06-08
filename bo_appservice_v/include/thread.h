#ifndef _THREAD_H_
#define _THREAD_H_ 
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <sys/time.h>
#include <cstdarg>
#include <sys/stat.h>
#include <errno.h>
#include <semaphore.h> 
#include <vector>
#include <assert.h>

#include "log.h"
#include "const.h"

static void* ThreadFunction(void* );

using namespace std;

class CThreadPool;

//线程
class CThread
{
	private:
		int m_ErrCode;
		//Semaphore m_ThreadSemaphore;
		//unsigned long m_ThreadID;
		pthread_t m_ThreadID;
		pthread_t m_ThreadCre;
		bool   m_Detach;
		bool   m_CreateSuspended;
		string  m_ThreadName;
		//0:死亡 1:忙碌 2:空闲
		int    m_ThreadState;

	protected:
		//设置错误码
		void SetErrCode(int errcode);
		//线程处理函数

	public:
		CThread();
		CThread(bool createsuspended, bool detach);
		virtual ~CThread();
		//virtual void Run(void) = 0;

		void SetThreadState(int state);
//		//结束线程
		bool Terminate(void);
		bool Start(void);
		//线程退出
		void Exit(void);
		//线程唤醒
		bool Wakeup(void);

		int GetThreadState(void);
		//获取错误码
		int GetLastError(void);
		void SetThreadName(string thrname);
		string GetThreadName(void);
		pthread_t GetThreadId(void);
		bool SetProiority(int priority);
		int GetProiority(int priority);

		int GetConcurrency(void);
		void SetConcurrency(int num);

		int Detach(void);

		//回收线程资源
		bool Join(void);
		bool Yield(void);
		//获取线程ID
		pthread_t Self(void);
};

//任务线程
class CJob
{
	private:
		int m_JobNo;
		string m_JobName;
		CThread* m_pWorkThread;
	public:
		CJob(void);
		virtual ~CJob();

		int GetJobNo(void) const;
		void SetJobNo(int num);
		string GetJobName(void)const;
		void SetJobName(string jobname);
		CThread* GetWorkThread(void) const;
		void SetWorkThread(CThread *pWorkThread);

		virtual void Run(void *ptr) = 0;
};

//工作线程
class CWorkThread:public CThread
{
	private:
		CThreadPool* m_ThreadPool;
		CJob* m_Job;
		void* m_JobData;

		pthread_mutex_t m_VarMutex;
		bool m_IsEnd;

	protected:
	public:
		pthread_cond_t m_JobCond;
		pthread_mutex_t m_WorkMutex;
		CWorkThread();
		virtual ~CWorkThread();
		void Run();
		void SetJob(CJob* job, void* jobdata);
		CJob* GetJob(void);
		void SetThreadPool(CThreadPool* thrpool);
		CThreadPool* GetThreadPool(void);
};

//线程池
class CThreadPool
{
	friend class CWorkThread;
	private:
		//线程最大数量
		unsigned int m_MaxNum;

		//线程最小数量
		unsigned int m_MinNum;

		//当前线程池中所允许存在的空闲线程的最小数目
		unsigned int m_AvailLow;

		//当前线程池中所允许的空闲的线程的最大数目
		unsigned int m_AvaiHigh;

		//目前线程池中实际存在的线程的个数
		unsigned int m_AvaiNum;

		//处世创建时线程池中的线程的个数
		unsigned int m_InitNum;

	public:
		//线程链表
		vector<CWorkThread*> m_ThreadList;
		//忙碌链表
		vector<CWorkThread*> m_BusyList;
		//空闲链表
		vector<CWorkThread*> m_IdleList;

	protected:
		CWorkThread* GetIdleThread(void);
		void AppendToIdleList(CWorkThread* jobthread);
		void MoveToBusyList(CWorkThread* idlethread);
		void MoveToIdleList(CWorkThread* busythread);
		void DeleteIdleThread(int num);
		void CreateIdleThread(int num);

	public:

		pthread_mutex_t m_BusyMutex;
		pthread_mutex_t m_IdleMutex;
		pthread_mutex_t m_JobMutex;
		pthread_mutex_t m_VarMutex;

		pthread_cond_t m_BusyCond;
		pthread_cond_t m_IdleCond;
		pthread_cond_t m_IdleJobCond;
		pthread_cond_t m_MaxNumCond;

		CThreadPool();
		CThreadPool(int initnum, int maxnum, int minnum);
		virtual ~CThreadPool();
		void SetMaxNum(int maxnum);
		int  GetMaxNum(int maxnum);
		void SetAvailLowNum(int minnum);
		int  GetAvailLowNum(int minnum);
		void SetAvailHightNum(int minnum);
		int  GetAvailHightNum(int minnum);
		void SetInitNum(int minnum);
		int  GetInitNum(int minnum);
		int  GetActuaAvailNum(int minnum);
		int  GetAllNum();
		int  GetBusyNum();
		void TerminateAll(void);
		void Run(CJob* job, void* jobdata);
};


//管理线程
class CThreadManage
{
	private:
		CThreadPool* m_Pool;
		int  m_NumOfThread;
	protected:
		void SetParalleNum(int num);
		CThreadManage();
		CThreadManage(int num, int max, int min);
		virtual  ~CThreadManage();
		void Run(CJob* job, void* jobdata);
		void TerminateAll(void);
	
};
#endif

