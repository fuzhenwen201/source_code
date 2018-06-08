#pragma once
#include "StockExcDef.h"
//#include <c++\4.8\ext\hash_map>
#include "MemoryPool.h"
#include "XRingBuffer.h"
#include <muduo/base/Mutex.h>
#include <list>
#include <pthread.h>
#define RING_BUF_SIZE 8388608 //8M的
typedef struct ST_ORDER_CACHE_INFO
{
	long  lSubmitOrderID;
	uintptr_t pOrderAddr;
	ST_ORDER_CACHE_INFO()
	{
		lSubmitOrderID = 0;
		pOrderAddr = 0;
	}
	~ST_ORDER_CACHE_INFO()
	{
		lSubmitOrderID = 0;
		pOrderAddr = 0;
	}


}STOrderCache,*LPOrderCache;

class CMacthEngine
{
public:
	CMacthEngine();
	virtual ~CMacthEngine();
	XRingBuffer* GetInputRingBuffer() { return m_pInputRingBuf; }
	XRingBuffer* GetOutputRingBuffer() { return m_pOutputRingBuf; }
	void SetTimerEvent(int iEvent, int iVal);
	static void* MatchExchangeFunc(void* pInfo);
	void ExitWork() { m_bIsRunning = false; pthread_join(m_threadID, NULL); }
	//按照与运算来实现的高效求与。不过uiMod必须是2的整数幂
	inline UINT EffectiveMod(UINT uiParam, UINT uiMod) { return uiParam&(uiMod - 1); }
	inline int GetMatchTimeFlag() { return m_iMatchTimerFlag; }
	inline int GetSnapShotFlag() { return m_iSnapShotFlag; }
	void SetRunCpu(int iCpuID) { m_iSetCpuID = iCpuID; }
	bool StartWork();

protected:
	bool AddSubmitOrder(STSubmitOrderInfo* pSubmitOrder);
	bool AddAskOrBidOrder(STSubmitOrderInfo* pOrderInfo, STOrderBookInfo* pOrderBookInfo);
	bool CancelSubmitOrder(const STSubmitOrderInfo* pOrderInfo, STOrderBookInfo* pOrderBookInfo);
	bool ProcMarketOrder(STSubmitOrderInfo* pOrderInfo, STOrderBookInfo* pOrderBookInfo);
	bool ProcMarketBidOrder(STSubmitOrderInfo* pSubmitOrder, MapOrderBook* pOrderBook);
	bool ProcMarketAskOrder(STSubmitOrderInfo* pSubmitOrder, MapOrderBook* pOrderBook);
	bool WriteTransferOrder(const STExechOrderinfo* pTransOrder);
	bool StartMatchExchange(); //开始进行撮合成交
	bool WriteMarketSnapShot();
	bool MatchExchangeObj(STOrderBookInfo& stOrderBook);
	bool WriteOrderBook();
protected:
	//由于虚拟货币对（即交易品种 ）也就最多几十种，在这里也就没有必要用hash_map
//	std::map<UINT, STOrderBookInfo> m_mapMatchExch; //交易买卖单
	std::map<UINT, MapPlatformOrderBook> m_mapMatchExch; //交易买卖单
	LPOrderCache m_pOrderCache;
	bool m_bIsRunning;
	XRingBuffer* m_pInputRingBuf;//XRingBuffer是用来一对一写入的，是非线程安全的
	XRingBuffer* m_pOutputRingBuf;
	int m_iMatchTimerFlag;  //1代表时间到了触发， 其他值都代表没有触发
	int m_iSnapShotFlag;
	mutable muduo::MutexLock m_muteLock;
	MemoryPool<STSubmitOrderInfo> m_memPool; //订单对象的内存池
	STSubmitOrderInfo*   m_pFirstOrder;
//	STSubmitOrderInfo*   m_pRemAskOrder;     //用来保存没有处理完的卖单，也就是说一笔卖单把所有的买单都清掉，还剩余
//	STSubmitOrderInfo*   m_pRemBidOrder;     //用来保存没有处理完的买单，也就是说一笔买单把所有的卖单都清掉，还剩余
	unsigned long   m_ulTransID;            //交易ID，顺序增长
	std::list<STSubmitOrderInfo*> m_listMarketBidOrder;
	std::list<STSubmitOrderInfo*> m_listMarketAskOrder;
	int m_iProcCnt;   //统计每秒处理的交易数
	int m_iWriteWaitCnt;//统计在输出时，由于数据堵塞而进行的等待尝试次数
	int m_iSetCpuID;
	pthread_t m_threadID;
	long m_lStartTime;
	


};

