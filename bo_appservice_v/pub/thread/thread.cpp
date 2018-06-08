#include "../../include/thread.h"
#include "../../include/const.h"

/*******************************线程基本操作**********************************/
//线程处理函数
static void* ThreadFunction(void* )
{
	LOG_INFO("ThreadFunction");
	while(1)
	{
		LOG_INFO("业务处理开始");
		sleep(1);
		LOG_INFO("业务处理完成");
	}
}

//设置错误码
void CThread::SetErrCode(int errcode)
{
	m_ErrCode = errcode;
}

CThread::CThread()
{
	int ret;
	if((ret = pthread_create(&m_ThreadCre, NULL, ThreadFunction, NULL)))
	{
		m_ErrCode = ret;
		LOG_ERR("创建线程失败,ret[%d]",ret);
	}

}

CThread::CThread(bool createsuspended, bool detach)
{
}

CThread::~CThread()
{
	LOG_ERR("退出CThread");
}

void CThread::SetThreadState(int state)
{
	m_ThreadState = state;
}

//结束线程
bool CThread::Terminate(void)
{
	return true;
}

bool CThread::Start(void)
{
}

//线程退出
void CThread::Exit(void)
{
	pthread_exit(0);
}

//线程唤醒
bool CThread::Wakeup(void)
{
}

int CThread::GetThreadState(void)
{
	return m_ThreadState;
}

//获取错误码
int CThread::GetLastError(void)
{
	return m_ErrCode;
}

void CThread::SetThreadName(string thrname)
{
	m_ThreadName = thrname;
}

string CThread::GetThreadName(void)
{
	return m_ThreadName;
}

pthread_t CThread::GetThreadId(void)
{
	//return m_ThreadID;
	return m_ThreadCre;
}

bool CThread::SetProiority(int priority)
{
}

int CThread::GetProiority(int priority)
{
}

int CThread::GetConcurrency(void)
{
}

void CThread::SetConcurrency(int num)
{
}

int CThread::Detach(void)
{
	int ret;
	if(ret = pthread_detach(Self()))
	{
		m_ErrCode = ret;
		LOG_ERR("回收资源设置失败,ret[%d]",ret);
	}
	return ret;
}

//回收线程资源
bool CThread::Join(void)
{
	int ret;
	if(ret = pthread_join(Self(), NULL))
	{
		m_ErrCode = ret;
		LOG_ERR("回收资源设置失败,ret[%d]",ret);
	}
	return ret;
}

//放弃CPU时间片
bool CThread::Yield(void)
{
	int ret;
	if(ret = pthread_yield())
	{
		m_ErrCode = ret;
		LOG_ERR("放弃时间片失败,ret[%d]",ret);
	}
	return ret;
}

//获取线程ID
pthread_t CThread::Self(void)
{
	return pthread_self(); 
}
/*****************************************************end*************************************************************/

/*************************************************工作线程 start*******************************************************/
CWorkThread::CWorkThread()
{
	m_Job = NULL;
	m_JobData = NULL;
	m_ThreadPool = NULL;
	m_IsEnd = false;
}

CWorkThread:: ~CWorkThread()
{
	if(m_Job != NULL)
		delete m_Job;
	if(m_ThreadPool != NULL)
		delete m_ThreadPool;
}
void CWorkThread::Run()
{
	SetThreadState(THREAD_RUNNING);
	while(1)
	{
		while(m_Job == NULL)
			pthread_cond_wait(&m_JobCond, &m_WorkMutex);
		m_Job->Run(m_JobData);
		m_Job->SetWorkThread(NULL);
		m_Job = NULL;
		pthread_mutex_lock(&m_WorkMutex);
		m_ThreadPool->MoveToIdleList(this);
		if(m_ThreadPool->m_IdleList.size() > m_ThreadPool->GetAvaiHighNum())
		{
			m_ThreadPool->DeleteIdleThread(m_ThreadPool->m_IdleList.size()->m_ThreadPool-GetInitNum());
		}
		pthread_mutex_unlock(&m_WorkMutex);
	}
}

void CWorkThread::SetJob(CJob* job, void* jobdata)
{
	pthread_mutex_lock(&m_VarMutex);
	m_Job = job;
	m_JobData = jobdata;
	job->SetWorkThread(this);
	pthread_mutex_unlock(&m_VarMutex);

	pthread_cond_signal(&m_JobCond);
}

CJob* CWorkThread::GetJob(void)
{
	return m_Job;
}

void CWorkThread::SetThreadPool(CThreadPool* thrpool)
{
	pthread_mutex_lock(&m_VarMutex);
	m_ThreadPool = thrpool;
	pthread_mutex_unlock(&m_VarMutex);
}

CThreadPool* CWorkThread::GetThreadPool(void)
{
	return m_ThreadPool;
}

/*****************************************************end*************************************************************/


/********************************************************线程池操作start*********************************************/
CWorkThread* CThreadPool::GetIdleThread(void)
{
	//线程从空闲队列中取出可能丢失
	CWorkThread* thr = NULL;
	pthread_mutex_lock(&m_IdleMutex);
	//若是设置了lock则会先unlock mutex
	while(m_IdleList.size() == 0)
		pthread_cond_wait(&m_IdleCond, &m_IdleMutex);
	if( m_IdleList.size() > 0 )
	{
		thr = (CWorkThread*)m_IdleList.front();
	}
	pthread_mutex_unlock(&m_IdleMutex);
	return thr;
}

void CThreadPool::AppendToIdleList(CWorkThread* jobthread)
{
	pthread_mutex_lock(&m_IdleMutex);
	m_IdleList.push_back(jobthread);
	m_ThreadList.push_back(jobthread);
	pthread_mutex_unlock(&m_IdleMutex);
}

void CThreadPool::MoveToBusyList(CWorkThread* idlethread)
{
	pthread_mutex_lock(&m_BusyMutex);
	m_BusyList.push_back(idlethread);
	m_AvaiNum;
	pthread_mutex_unlock(&m_BusyMutex);

	pthread_mutex_lock(&m_IdleMutex);
	vector<CWorkThread*>::iterator pos;
	for(pos = m_IdleList.begin(); pos != m_IdleList.end(); pos++)
	{
		if((*pos) == idlethread)
			m_IdleList.erase(pos);
	}
	pthread_mutex_unlock(&m_IdleMutex);
}

void CThreadPool::MoveToIdleList(CWorkThread* busythread)
{
	pthread_mutex_lock(&m_IdleMutex);
	m_IdleList.push_back(busythread);
	m_AvaiNum++;
	pthread_mutex_unlock(&m_IdleMutex);

	pthread_mutex_lock(&m_BusyMutex);
	vector<CWorkThread* >::iterator pos;
	pthread_mutex_unlock(&m_BusyMutex);
}

void CThreadPool::CreateIdleThread(int num)
{
	for(int i=0; i<num; i++)
	{
		CWorkThread* thr = new CWorkThread();
		thr->SetThreadPool(this);
		AppendToIdleList(thr);
		pthread_mutex_lock(&m_VarMutex);
		m_AvaiNum++;
		pthread_mutex_unlock(&m_VarMutex);
		thr->Run();
	}
}

void CThreadPool::DeleteIdleThread(int num)
{
	CWorkThread* thr;
	pthread_mutex_lock(&m_IdleMutex);
	for(int i=0; i<num; i++)
	{
		if(m_IdleList.size() > 0)
		{
			thr = (CWorkThread*)m_IdleList.front();
		}
	}

	pthread_mutex_lock(&m_IdleMutex);
	vector<CWorkThread* >::iterator pos;
	for(pos = m_IdleList.begin(); pos != m_IdleList.end(); pos++)
	{
		if((*pos) = thr)
		{
			m_IdleList.erase(pos);
		}
	}
	pthread_mutex_unlock(&m_IdleMutex);
} 
CThreadPool::CThreadPool()
{
	m_MaxNum = THREAD_NUM_MAX;
	m_InitNum = THREAD_NUM_DEF;
	m_MinNum = THREAD_NUM_MIN;

	pthread_cond_init(&m_BusyCond, NULL);
	pthread_cond_init(&m_IdleCond, NULL);
	pthread_cond_init(&m_IdleJobCond, NULL);
	pthread_cond_init(&m_MaxNumCond, NULL);

	m_BusyList.clear();
	m_IdleList.clear();

	for(int i=0; i<m_InitNum; i++)
	{
		CWorkThread* thr = new CWorkThread();
		AppendToIdleList(thr);
		thr->Run();
	}
}

CThreadPool::CThreadPool(int initnum, int maxnum, int minnum)
{
	maxnum >  0 && maxnum < THREAD_NUM_MAX  ? 
		m_MaxNum  = maxnum  : m_MaxNum  = THREAD_NUM_MAX;
	minnum >  0 && minnum < THREAD_NUM_MAX  ? 
		m_MinNum  = minnum  : m_MinNum  = THREAD_NUM_MIN;
	initnum > 0 && initnum < THREAD_NUM_MAX ? 
		m_InitNum = initnum : m_InitNum = THREAD_NUM_DEF;

	m_BusyList.clear();
	m_IdleList.clear();

	for(int i=0; i<m_InitNum; i++)
	{
		CWorkThread* thr = new CWorkThread();
		AppendToIdleList(thr);
		thr->Run();
	}
	
}

CThreadPool::~CThreadPool()
{
	TerminateAll();
}

void CThreadPool::SetMaxNum(int maxnum)
{
	m_MaxNum = maxnum;
}

int CThreadPool::GetMaxNum(int maxnum)
{
	return maxnum;
}

void CThreadPool::SetAvailLowNum(int minnum)
{
}

int CThreadPool::GetAvailLowNum(int minnum)
{
}

void CThreadPool::SetAvailHightNum(int minnum)
{
}

int CThreadPool::GetAvailHightNum(int minnum)
{
}

void CThreadPool::SetInitNum(int minnum)
{
	m_InitNum = minnum;
}

int CThreadPool::GetInitNum(int minnum)
{
	return m_InitNum;
}

int CThreadPool::GetActuaAvailNum(int minnum)
{
}

int CThreadPool::GetAllNum()
{
}

int CThreadPool::GetBusyNum()
{
	return m_BusyList.size();
}

void CThreadPool::TerminateAll(void)
{
}

void CThreadPool::Run(CJob* job, void* jobdata)
{
	if(GetBusyNum() == m_MaxNum)
		pthread_cond_wait(&m_IdleCond, &m_IdleMutex);
	if(m_IdleList.size() < m_AvailLow)
	{
		if(GetAllNum()+m_InitNum-m_IdleList.size() < m_MaxNum)
		{
			CreateIdleThread(m_InitNum-m_IdleList.size());
		}
		else
		{
			CreateIdleThread(m_MaxNum-GetAllNum());
		}

		CWorkThread* idlethr = GetIdleThread();
		if(idlethr != NULL)
		{
			pthread_mutex_lock(&(idlethr->m_WorkMutex));
			MoveToBusyList(idlethr);
			idlethr->SetThreadPool(this);
			//job->SetWorkThread(m_pWorkThread);
			idlethr->SetJob(job,jobdata);
			pthread_mutex_unlock(&(idlethr->m_WorkMutex));
		}
	}
		
}
/********************************************************线程池操作end**********************************************/

/***********************管理线程start*************************/
void CThreadManage::SetParalleNum(int num)
{
	m_NumOfThread = num;
}

CThreadManage::CThreadManage()
{
	m_NumOfThread = THREAD_NUM_DEF;
	m_Pool = new CThreadPool(m_NumOfThread, THREAD_NUM_MAX, THREAD_NUM_MIN);
	LOG_INFO("thread num [%d]", m_NumOfThread);
}

CThreadManage::CThreadManage(int num, int max, int min)
{
	m_NumOfThread = num;
	m_Pool = new CThreadPool(m_NumOfThread, max, min);
	LOG_INFO("thread num [%d]", m_NumOfThread);
}

CThreadManage::~CThreadManage()
{
	if(NULL  != m_Pool)
		delete m_Pool;
}

void CThreadManage::Run(CJob* job, void* jobdata)
{
	m_Pool->Run(job, jobdata);
}

void CThreadManage::TerminateAll(void)
{
	m_Pool->TerminateAll();
}

/***********************管理线程end*************************/



/***********************工作线程start*************************/
CJob::CJob(void)
:m_pWorkThread(NULL)
,m_JobNo(0)
,m_JobName(NULL)
{
}

CJob::~CJob()
{
	//if(m_JobName.length())
		//free(m_JobName);
}

int CJob::GetJobNo() const
{
	return m_JobNo;
}

void CJob::SetJobNo(int num)
{
	m_JobNo = num;
}

string CJob::GetJobName(void)const
{
	return m_JobName;
}

void CJob::SetJobName(string jobname)
{
	m_JobName = jobname;
}

CThread* CJob::GetWorkThread(void)const
{
	return m_pWorkThread;
}

void CJob::SetWorkThread(CThread *pWorkThread)
{
	m_pWorkThread = pWorkThread;
}

/***********************工作线程end*************************/
