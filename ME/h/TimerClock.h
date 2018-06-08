#pragma once

#include "StockExcDef.h"
#include <muduo/base/Logging.h>

class CTimerClock
{
public:
	CTimerClock();
	virtual ~CTimerClock();

	void StartTimerByOtherLoop(MuduoEventLoop* pEventLoop);
	void StartTimerBySelf();
	void SetMatchCallBack(OnTimerCallBack func) { m_funcMatchCallBack = func; }
	void SetSnapShotCallBack(OnTimerCallBack func) { m_funcSnapShotCallback = func; }
	void SetServerStatusCallBacl(OnTimerCallBack func) { m_funcServerStatus = func; }


protected:
	void TimerMatch();
	void TimerMarketSnapShot();
	void TimerServerStatus();
	void DefaultCallBack(int iFlag, int iVal) { LOG_ERROR << "default timer callback function"; }



protected:
	MuduoEventLoop* m_pEventLoop;
	MuduoEventLoopThread m_eventLoopThread;
	OnTimerCallBack    m_funcMatchCallBack;
	OnTimerCallBack    m_funcSnapShotCallback;
	OnTimerCallBack    m_funcServerStatus;
};

