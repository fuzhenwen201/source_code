#pragma once
#include "StockExcDef.h"
#include "XRingBuffer.h"
#include <muduo/base/Logging.h>
#include <assert.h>
class CWriteMarketInfo
{
public:
	CWriteMarketInfo();
	virtual ~CWriteMarketInfo();
//	void ExitWirteThread() { m_bIsRunning = false; }
	void RegisterBuffer(XRingBuffer* pBuffer) {/*static_assert(pBuffer != NULL, "Input ringbuffer is null");*/assert(pBuffer!=NULL);m_pRingBuf = pBuffer; }
	
	
	void SetRunCpu(int iCpuID) { m_iSetCpuID = iCpuID; }
	void ExitWork() { m_bIsRunning = false; }
	bool StartPublishData(long lParam);

protected:
    ///////////////////////////////old interface£¨no more use£©///////////////////////////////////////////
//	void ExitWork() { m_bIsRunning = false; pthread_join(m_threadID, NULL); }
	static void* WriteMarketTrans(void* pInfo);
	bool StartWork();
	//////////////////////////////////////////////////////////////////////////
protected:
	XRingBuffer* m_pRingBuf;
	bool m_bIsRunning;
	int m_iSetCpuID;
	pthread_t m_threadID;
};

