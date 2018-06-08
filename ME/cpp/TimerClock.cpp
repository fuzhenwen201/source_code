#include "TimerClock.h"
#include <assert.h>




CTimerClock::CTimerClock()
{
	//m_pEventLoop = pEventLoop;
	m_funcMatchCallBack = std::bind(&CTimerClock::DefaultCallBack, this, 0, 0);
	m_funcSnapShotCallback = std::bind(&CTimerClock::DefaultCallBack, this, 0, 0);
	m_funcServerStatus = std::bind(&CTimerClock::DefaultCallBack, this, 0, 0);

}


CTimerClock::~CTimerClock()
{
}


void CTimerClock::StartTimerByOtherLoop(MuduoEventLoop* pEventLoop)
{
	assert(pEventLoop != NULL);
	pEventLoop->runEvery(MATCH_TIME_INTERVAL, std::bind(&CTimerClock::TimerMatch, this));
	pEventLoop->runEvery(MARKET_SNAPSHOT_TIME_INTERVAL, std::bind(&CTimerClock::TimerMarketSnapShot, this));
	pEventLoop->runEvery(SERVER_STATUS_TIME_INTERVAL, std::bind(&CTimerClock::TimerServerStatus, this));
//	pEventLoop->loop();

}

void CTimerClock::StartTimerBySelf()
{
	MuduoEventLoop* pEventloop = m_eventLoopThread.startLoop();
	pEventloop->runEvery(MATCH_TIME_INTERVAL, std::bind(&CTimerClock::TimerMatch, this));
	pEventloop->runEvery(MARKET_SNAPSHOT_TIME_INTERVAL, std::bind(&CTimerClock::TimerMarketSnapShot, this));
	pEventloop->runEvery(SERVER_STATUS_TIME_INTERVAL, std::bind(&CTimerClock::TimerServerStatus, this));

}

void CTimerClock::TimerMatch()
{
	m_funcMatchCallBack(EN_TIMER_MATCH, 1);

}
void CTimerClock::TimerMarketSnapShot()
{
	m_funcSnapShotCallback(EN_TIMER_MARKET_SNAPSHOT, 1);

}

void CTimerClock::TimerServerStatus()
{
	m_funcSnapShotCallback(EN_TIMER_SERVER_STATUS, 1);

}