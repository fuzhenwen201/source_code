/*************************************************************************
	> File Name: lockop.h
	> Author: xyz
	> Mail: xiao13149920@foxmail.com
	> Created Time: Tue 12 Sep 2017 05:46:29 PM CST
 ************************************************************************/
#ifndef _LOCKOP_H_
#define _LOCKOP_H_

#include<pthread.h>
#include<time.h>

class CCondition;

class CMutex {
	public:
		void Init(bool pshared = false);
		int Lock(void);
		int TryLock(void);
		int TimeLock(const struct timespec& abs_timeout);
		int UnLock(void);

		friend class CCondition;

	private:
		pthread_mutex_t m_mutex;
		pthread_mutexattr_t m_mutex_attr;
};


class CCondition {
	public:
		void Init(bool pshared = false);
		int Wait(CMutex& m);
		int TimeWait(const struct timespec& ts, CMutex& m);
		int Signal(void);
		int BroadCast(void);

	private:
		pthread_cond_t m_cond;
		pthread_condattr_t m_cond_attr;
};

class CReadWriteLock {
	public:
		void Init(bool pshared = false);
		void ReadLock(void);
		void ReadUnLock(void);
		void WriteLock(void);
		void WriteUnLock(void);

	private:
		CMutex m_mutex;
		CCondition m_rcond;
		CCondition m_wcond;
		size_t m_readers;
		size_t m_readers_wait;
		size_t m_writers;
		size_t m_writers_wait;
};
#endif

