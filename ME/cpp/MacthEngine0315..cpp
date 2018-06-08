#include "MacthEngine.h"
#include <muduo/base/Logging.h>
#include <unistd.h>
#include <iostream>
#include <sched.h>
#include <muduo/base/Timestamp.h>
CMacthEngine::CMacthEngine()
{
	m_pOrderCache = new STOrderCache[MAX_ORDER_INDEX];
//	memset(m_pOrderCache, 0, sizeof(STOrderCache)*MAX_ORDER_INDEX);

// 	m_hashMapExAsk.clear();
// 	m_hashMapExBid.clear();
	m_mapMatchExch.clear();
	m_pInputRingBuf = new XRingBuffer(RING_BUF_SIZE);
	m_pOutputRingBuf = new XRingBuffer(RING_BUF_SIZE);
	//在这里先申请内存池中的一个快
	m_pFirstOrder = m_memPool.allocate(1);
//	m_pRemAskOrder = NULL;
//m_pRemBidOrder = NULL;
	m_listMarketAskOrder.clear();
	m_listMarketBidOrder.clear();
	m_iProcCnt = 0;
	m_iWriteWaitCnt = 0;
	m_iSetCpuID = 255;
	m_threadID = 0;
	m_lStartTime = 0;
}


CMacthEngine::~CMacthEngine()
{
	ExitWork();
	m_bIsRunning = false;
	m_listMarketAskOrder.clear();
	m_listMarketBidOrder.clear();
	if (m_pOrderCache)
	{
		delete []m_pOrderCache;
	}
	if (m_pInputRingBuf)
	{
		delete[]m_pInputRingBuf;
	}
	if (m_pOutputRingBuf)
	{
		delete[]m_pOutputRingBuf;
	}
	if (m_bIsRunning)
		ExitWork();
}

bool CMacthEngine::AddAskOrBidOrder(STSubmitOrderInfo* pOrderInfo, STOrderBookInfo* pOrderBookInfo)
{
	//如果是委托买/卖单的话，则首先要查看是否还有剩余的市场化订单，有的话，则优先处理
	std::list<STSubmitOrderInfo*>* pListOrder = (pOrderInfo->ucTransType == EN_ORDER_BID) ? &m_listMarketAskOrder : &m_listMarketBidOrder;
	while (pListOrder->size() > 0)
	{
		STExechOrderinfo stExecOrder;
		STSubmitOrderInfo* pMarketOrder = pListOrder->front();
		//如果委托单小于或等于市场化订单队列的第一个单
		if ((pOrderInfo->fNumber < pMarketOrder->fNumber) || IsFloatEqual(pOrderInfo->fNumber, pMarketOrder->fNumber))
		{
			//构造成交单，并写入成交明细中
			stExecOrder.lExchID = ++m_ulTransID;
			stExecOrder.lPrice = pOrderInfo->lPrice;
			stExecOrder.fNumber = pOrderInfo->fNumber;
			stExecOrder.lAskOrderID = (pOrderInfo->ucTransType == EN_ORDER_BID) ? pMarketOrder->lSubmitOrderID : pOrderInfo->lSubmitOrderID;
			stExecOrder.lBidOrderID = (pOrderInfo->ucTransType == EN_ORDER_BID) ? pOrderInfo->lSubmitOrderID : pMarketOrder->lSubmitOrderID;
			stExecOrder.ucExchType = 1;
			WriteTransferOrder(&stExecOrder);
			//如果刚好相等的话，则从市场订单列表中弹出次订单
			if (IsFloatEqual(pOrderInfo->fNumber, pMarketOrder->fNumber))
			{
				//弹出元素，回收内存
				pListOrder->pop_front();
				m_memPool.destroy(pMarketOrder);
				m_memPool.deallocate(pMarketOrder);
			}
			else
			{
				pMarketOrder->fNumber -= pOrderInfo->fNumber;
				pMarketOrder->ucTransStatus = EN_TRANS_STATUS_PART_TRANS;
			}
			//该委托单处理完则直接
			return true;
		}
		else
		{
			//构造成交单，并写入成交明细中
			stExecOrder.lExchID = ++m_ulTransID;
			stExecOrder.lPrice = pOrderInfo->lPrice;
			stExecOrder.fNumber = pMarketOrder->fNumber;
			stExecOrder.lAskOrderID = (pOrderInfo->ucTransType == EN_ORDER_BID) ? pMarketOrder->lSubmitOrderID : pOrderInfo->lSubmitOrderID;
			stExecOrder.lBidOrderID = (pOrderInfo->ucTransType == EN_ORDER_BID) ? pOrderInfo->lSubmitOrderID : pMarketOrder->lSubmitOrderID;
			stExecOrder.ucExchType = 1;
			WriteTransferOrder(&stExecOrder);

			//如果委托单大于市场订单，则需从列表中弹出该市场单
			pOrderInfo->fNumber -= pMarketOrder->fNumber;
			pOrderInfo->ucTransStatus = EN_TRANS_STATUS_PART_TRANS;
			pListOrder->pop_front();
			m_memPool.destroy(pMarketOrder);
			m_memPool.deallocate(pMarketOrder);
		}
	}

	//能进入此步说明之前的市场订单队列已经被处理完，此时则讲委托订单加入到OrderBook中去
	MapOrderBook* pMapOrderBook = (pOrderInfo->ucTransType == EN_ORDER_BID) ? &(pOrderBookInfo->mapBidOrderBook) : &(pOrderBookInfo->mapAskOrderBook);
	MapOrderBook::iterator iter = pMapOrderBook->find(pOrderInfo->lPrice);
	STSubmitOrderInfo* pTmpOrder = NULL;
	//获取内存池中的地址
	if (NULL != m_pFirstOrder)
	{
		pTmpOrder = m_pFirstOrder;
		m_pFirstOrder = NULL;//只使用1次，目的是为了使得内存池的第一个块内存的分配在构造函数阶段就进行分配
	}
	else
		pTmpOrder = m_memPool.allocate(1);
	memcpy(pTmpOrder, pOrderInfo, sizeof(STSubmitOrderInfo));
	//按照价格去查找并加入到OrderBool中去
	if (iter != pMapOrderBook->end())
	{
		//按照时间进行订单的加入
		iter->second.fTotalNumber += pTmpOrder->fNumber;
		iter->second.mulmapOrder.insert(std::make_pair(pOrderInfo->lSubmitTime, pTmpOrder));
	}
	else
	{
		STPriceOrderInfo stPriceOrder;
		stPriceOrder.fTotalNumber = pTmpOrder->fNumber;
		stPriceOrder.mulmapOrder.insert(std::make_pair(pOrderInfo->lSubmitTime, pTmpOrder));
		pMapOrderBook->insert(std::make_pair(pOrderInfo->lPrice, stPriceOrder));
	}
	//将索引表和订单信息进行关联
	//reinterpret_cast主要是将数据从一种类型的转换为另一种类型。所谓“通常为操作数的位模式提供较低层的重新解释”也就是说将数据以二进制存在形式的重新解释
	//这里也就是将pTmpOrder指针重新解释并转换为uintptr_t
	UINT uiIndex = EffectiveMod(pTmpOrder->uiOrderIndex, MAX_ORDER_INDEX);
	m_pOrderCache[uiIndex].pOrderAddr = reinterpret_cast<uintptr_t>(pTmpOrder);
	m_pOrderCache[uiIndex].lSubmitOrderID = pTmpOrder->lSubmitOrderID;
	return true;
}

bool CMacthEngine::WriteTransferOrder(const STExechOrderinfo* pTransOrder)
{
	m_iProcCnt++;
	//将数据写到输出缓冲区中去
	if (m_iProcCnt == 3400)
	{
		long lEndTime = muduo::Timestamp::now().microSecondsSinceEpoch();
		std::cout << "Process 3400 trans all cost time = " << (long)((lEndTime - m_lStartTime)*0.5)<< std::endl;
	}
	int iLen = sizeof(STExechOrderinfo);
	ST_MARKET_HEADER_INFO stMarketInfo;
	stMarketInfo.iLen = iLen;
	stMarketInfo.uiMagicWords = MARKET_MAGIC_WORDS;
	stMarketInfo.wMsgType = 0;
	while (1)
	{
		if (m_pOutputRingBuf->getFreeSize() >= (sizeof(stMarketInfo)+iLen) )
		{
			m_pOutputRingBuf->pushData(&stMarketInfo, sizeof(stMarketInfo));
			m_pOutputRingBuf->pushData(pTransOrder, iLen);
			return true;


		}
	
		
		usleep(1);
		m_iWriteWaitCnt++;
		

	}

	return true;

}

bool CMacthEngine::WriteOrderBook()
{
//	int iLen = sizeof(STExechOrderinfo);
	std::map<UINT, MapPlatformOrderBook>::iterator iter = m_mapMatchExch.begin();
	ST_MARKET_HEADER_INFO stMarketInfo;
	stMarketInfo.uiMagicWords = MARKET_MAGIC_WORDS;
	stMarketInfo.wMsgType = 1;
	for (; iter != m_mapMatchExch.end(); iter++)
	{
		MapPlatformOrderBook::iterator iterOrderBook = iter->second.begin();
		for (; iterOrderBook != iter->second.end(); iterOrderBook++)
		{
			LOG_INFO << "ExchObj = " << iter->first << " Exchange's ID = " << iterOrderBook->first << " 's OrderBook is : ";
			int iDataLen = iterOrderBook->second.mapAskOrderBook.size() * sizeof(STSigOrderBookItem);
			stMarketInfo.iLen = iDataLen;
			stMarketInfo.wTransType = 1;
			STSigOrderBookItem stSigOrder;
			//写Ask OrderBook
			while (1)
			{
				if (iDataLen < 1)
				{
					break;
				}
				if (m_pOutputRingBuf->getFreeSize() >= sizeof(ST_MARKET_HEADER_INFO) + iDataLen)
				{
					m_pOutputRingBuf->pushData(&stMarketInfo, sizeof(ST_MARKET_HEADER_INFO));
					MapOrderBook::reverse_iterator re_ask_iter = iterOrderBook->second.mapAskOrderBook.rbegin();
					for (; re_ask_iter != iterOrderBook->second.mapAskOrderBook.rend(); re_ask_iter++)
					{
						stSigOrder.lPrice = re_ask_iter->first;
						stSigOrder.fNumber = re_ask_iter->second.fTotalNumber;
						m_pOutputRingBuf->pushData(&stSigOrder, sizeof(STSigOrderBookItem));
					}
					break;
				}
				usleep(2);
				continue;
			}
			iDataLen = iterOrderBook->second.mapBidOrderBook.size() * sizeof(STSigOrderBookItem);
			stMarketInfo.iLen = iDataLen;
			stMarketInfo.wTransType = 0;
			while (1)
			{
				if (iDataLen < 1)
				{
					break;
				}
				if (m_pOutputRingBuf->getFreeSize() >= sizeof(ST_MARKET_HEADER_INFO) + iDataLen)
				{
					m_pOutputRingBuf->pushData(&stMarketInfo, sizeof(ST_MARKET_HEADER_INFO));
					MapOrderBook::reverse_iterator re_bid_iter = iterOrderBook->second.mapBidOrderBook.rbegin();
					for (; re_bid_iter != iterOrderBook->second.mapBidOrderBook.rend(); re_bid_iter++)
					{
						stSigOrder.lPrice = re_bid_iter->first;
						stSigOrder.fNumber = re_bid_iter->second.fTotalNumber;
						m_pOutputRingBuf->pushData(&stSigOrder, sizeof(STSigOrderBookItem));
					}
					break;
				}
				usleep(2);
				continue;
			}
		}
	}
	return true;

}

bool CMacthEngine::ProcMarketOrder(STSubmitOrderInfo* pOrderInfo, STOrderBookInfo* pOrderBookInfo)
{
	if (pOrderInfo->ucTransType == EN_ORDER_BID)
		return ProcMarketBidOrder(pOrderInfo, &(pOrderBookInfo->mapAskOrderBook));
	else
		return ProcMarketAskOrder(pOrderInfo, &(pOrderBookInfo->mapBidOrderBook));
}
//处理市场买单，则需从卖单列表中从小到大进行处理
bool CMacthEngine::ProcMarketBidOrder(STSubmitOrderInfo* pSubmitOrder, MapOrderBook* pOrderBook)
{
	//如果市场化订单列表中有存在数据，则讲该市场订单加入到列表中去等待排队处理
	if (m_listMarketBidOrder.size() > 0)
	{
		//从内存池当中申请内存空间
		STSubmitOrderInfo* pMarketOrder = m_memPool.allocate(1);
		memcpy(pMarketOrder, pSubmitOrder, sizeof(STSubmitOrderInfo));
		m_listMarketBidOrder.push_back(pMarketOrder);
		return true;
	}
	//开始进行和委托订单进行成交
	MapOrderBook::iterator iter = (*pOrderBook).begin();
	bool bIsRet = false;
	while (iter != (*pOrderBook).end())
	{
		std::multimap<long, STSubmitOrderInfo*>::iterator iterTimeOrder = (iter->second).mulmapOrder.begin();
		while (iterTimeOrder != (iter->second).mulmapOrder.end())
		{
			//能执行到这里，说明市场化订单已经完全成交完了，并且该价格仍然存在委托单
			if (bIsRet)
				return true;
			//如果该单已经被撤销了，则跳过该单，并且释放内存
			if (iterTimeOrder->second->ucTransStatus == EN_TRANS_STATUS_CPL_CANCEL || iterTimeOrder->second->ucTransStatus == EN_TRANS_STATUS_PART_CANCEL ||
				iterTimeOrder->second->ucTransStatus == EN_TRANS_STATUS_CPL_TRANS)
			{
				//回收该委托订单
				//				UINT uiIndex = EffectiveMod(iterTimeOrder->second->uiOrderIndex, MAX_ORDER_INDEX);
				// 				if (0 == m_pOrderCache[uiIndex].lSubmitOrderID || m_pOrderCache[uiIndex].lSubmitOrderID == iterTimeOrder->second->lSubmitOrderID)
				// 					m_pOrderCache[uiIndex].pOrderAddr = NULL;
				if (iterTimeOrder->second->ucTransStatus == EN_TRANS_STATUS_CPL_TRANS)
					LOG_ERROR << "exchange disorder, AskOrder(" << iterTimeOrder->second->lSubmitOrderID << ") should be erase before.";

				(iter->second).fTotalNumber -= iterTimeOrder->second->fNumber;
				m_memPool.destroy(iterTimeOrder->second);
				m_memPool.deallocate(iterTimeOrder->second);
				iterTimeOrder = (iter->second).mulmapOrder.erase(iterTimeOrder);
				continue;
			}
			STExechOrderinfo stExecOrder;
			stExecOrder.lExchID = ++m_ulTransID;
			stExecOrder.lPrice = iterTimeOrder->second->lPrice;
			stExecOrder.lAskOrderID = iterTimeOrder->second->lSubmitOrderID;
			stExecOrder.lBidOrderID = pSubmitOrder->lSubmitOrderID;

			stExecOrder.ucExchType = 1;
			//如果市场化订单小于委托单的数量，则在相应的委托单上修改剩余数量并且修改成交标记位
			//然后发送成交订单，发送完后则直接返回
			if (pSubmitOrder->fNumber < iterTimeOrder->second->fNumber)
			{
				stExecOrder.fNumber = pSubmitOrder->fNumber;
				(iter->second).fTotalNumber -= pSubmitOrder->fNumber;
				iterTimeOrder->second->fNumber -= pSubmitOrder->fNumber;
				iterTimeOrder->second->ucTransStatus = EN_TRANS_STATUS_PART_TRANS;
				WriteTransferOrder(&stExecOrder);
				return true;
			}
			else
			{
				(iter->second).fTotalNumber -= iterTimeOrder->second->fNumber;
				if (IsFloatEqual(pSubmitOrder->fNumber, iterTimeOrder->second->fNumber))
					bIsRet = true;
				else
				{
					pSubmitOrder->fNumber -= iterTimeOrder->second->fNumber;
					pSubmitOrder->ucTransStatus = EN_TRANS_STATUS_PART_TRANS;
				}
				stExecOrder.fNumber = iterTimeOrder->second->fNumber;
				WriteTransferOrder(&stExecOrder);
				//回收该委托订单
				UINT uiIndex = EffectiveMod(iterTimeOrder->second->uiOrderIndex, MAX_ORDER_INDEX);
				if (0 == m_pOrderCache[uiIndex].lSubmitOrderID || m_pOrderCache[uiIndex].lSubmitOrderID == iterTimeOrder->second->lSubmitOrderID)
					m_pOrderCache[uiIndex].pOrderAddr = 0;

				m_memPool.destroy(iterTimeOrder->second);
				m_memPool.deallocate(iterTimeOrder->second);
				iterTimeOrder = (iter->second).mulmapOrder.erase(iterTimeOrder);
			}
		}
		//在这里放bIsRet是考虑到当该市场化订单刚好把该价格里的最后一个委托订单成交了的话，
		//则需要将该mulmap删除后，再进行返回
		iter = (*pOrderBook).erase(iter);
		if (bIsRet)
			return true;

	}
	//能执行到这里，说明该市场买单已经全部吃掉了当前全部的市场委托卖单，并且还有剩余的买单，则需要将其加入到市场订单列表当中
	STSubmitOrderInfo* pTmpOrder = m_memPool.allocate(1);
	memcpy(pTmpOrder, pSubmitOrder, sizeof(STSubmitOrderInfo));
	m_listMarketBidOrder.push_back(pTmpOrder);
	return true;
}


//处理市场卖单
bool CMacthEngine::ProcMarketAskOrder(STSubmitOrderInfo* pSubmitOrder, MapOrderBook* pOrderBook)
{
	//如果市场化订单列表中有存在数据，则讲该市场订单加入到列表中去等待排队处理
	if (m_listMarketAskOrder.size() > 0)
	{
		//从内存池当中申请内存空间
		STSubmitOrderInfo* pMarketOrder = m_memPool.allocate(1);
		memcpy(pMarketOrder, pSubmitOrder, sizeof(STSubmitOrderInfo));
		m_listMarketAskOrder.push_back(pMarketOrder);
		return true;
	}
	bool bIsRet = false;
	MapOrderBook::reverse_iterator iter = (*pOrderBook).rbegin();

	while (iter != (*pOrderBook).rend())
	{
		std::multimap<long, STSubmitOrderInfo*>::iterator iterTimeOrder = (iter->second).mulmapOrder.begin();
		while (iterTimeOrder != (iter->second).mulmapOrder.end())
		{
			//能执行到这里，说明市场化订单已经完全成交完了，并且该价格仍然存在委托单
			if (bIsRet)
				return true;
			//如果该单已经被撤销了，则跳过该单，并且释放内存
			if (iterTimeOrder->second->ucTransStatus == EN_TRANS_STATUS_CPL_CANCEL || iterTimeOrder->second->ucTransStatus == EN_TRANS_STATUS_PART_CANCEL ||
				iterTimeOrder->second->ucTransStatus == EN_TRANS_STATUS_CPL_TRANS)
			{
				//回收该委托订单
//				UINT uiIndex = EffectiveMod(iterTimeOrder->second->uiOrderIndex, MAX_ORDER_INDEX);
// 				if (0 == m_pOrderCache[uiIndex].lSubmitOrderID || m_pOrderCache[uiIndex].lSubmitOrderID == iterTimeOrder->second->lSubmitOrderID)
// 					m_pOrderCache[uiIndex].pOrderAddr = NULL;
				if (iterTimeOrder->second->ucTransStatus == EN_TRANS_STATUS_CPL_TRANS)
					LOG_ERROR << "exchange disorder, BidOrder(" << iterTimeOrder->second->lSubmitOrderID << ") should be erase before.";

				(iter->second).fTotalNumber -= iterTimeOrder->second->fNumber;
				m_memPool.destroy(iterTimeOrder->second);
				m_memPool.deallocate(iterTimeOrder->second);
				iterTimeOrder = (iter->second).mulmapOrder.erase(iterTimeOrder);
				continue;
			}
			STExechOrderinfo stExecOrder;
			stExecOrder.lExchID = ++m_ulTransID;
			stExecOrder.lPrice = iterTimeOrder->second->lPrice;
			stExecOrder.lAskOrderID = pSubmitOrder->lSubmitOrderID;
			stExecOrder.lBidOrderID = iterTimeOrder->second->lSubmitOrderID;
			stExecOrder.ucExchType = 1;
			//如果市场化订单小于委托单的数量，则在相应的委托单上修改剩余数量并且修改成交标记位
			//然后发送成交订单，发送完后则直接返回
			if (pSubmitOrder->fNumber < iterTimeOrder->second->fNumber)
			{
				(iter->second).fTotalNumber -= pSubmitOrder->fNumber;
				stExecOrder.fNumber = pSubmitOrder->fNumber;
				iterTimeOrder->second->fNumber -= pSubmitOrder->fNumber;
				iterTimeOrder->second->ucTransStatus = EN_TRANS_STATUS_PART_TRANS;
				WriteTransferOrder(&stExecOrder);
				return true;
			}
			else
			{
				(iter->second).fTotalNumber -= iterTimeOrder->second->fNumber;
				if (IsFloatEqual(pSubmitOrder->fNumber, iterTimeOrder->second->fNumber))
					bIsRet = true;
				else
				{
					pSubmitOrder->fNumber -= iterTimeOrder->second->fNumber;
					pSubmitOrder->ucTransStatus = EN_TRANS_STATUS_PART_TRANS;
				}

				stExecOrder.fNumber = iterTimeOrder->second->fNumber;
				WriteTransferOrder(&stExecOrder);
				//回收该委托订单
				UINT uiIndex = EffectiveMod(iterTimeOrder->second->uiOrderIndex, MAX_ORDER_INDEX);
				if (0 == m_pOrderCache[uiIndex].lSubmitOrderID || m_pOrderCache[uiIndex].lSubmitOrderID == iterTimeOrder->second->lSubmitOrderID)
					m_pOrderCache[uiIndex].pOrderAddr = 0;
				
				m_memPool.destroy(iterTimeOrder->second);
				m_memPool.deallocate(iterTimeOrder->second);
				iterTimeOrder = (iter->second).mulmapOrder.erase(iterTimeOrder);
			}
		}
		//在这里放bIsRet是考虑到当该市场化订单刚好把该价格里的最后一个委托订单成交了的话，
		//则需要将该mulmap删除后，再进行返回
		iter = MapOrderBook::reverse_iterator((*pOrderBook).erase((++iter).base()));
		if (bIsRet)
			return true;

	}
	//能执行到这里，说明该市场卖单已经全部吃掉了当前全部的市场委托买单，则需要将其加入到市场订单列表当中
	STSubmitOrderInfo* pTmpOrder = m_memPool.allocate(1);
	memcpy(pTmpOrder, pSubmitOrder, sizeof(STSubmitOrderInfo));
	m_listMarketAskOrder.push_back(pTmpOrder);
	return true;

}

bool CMacthEngine::CancelSubmitOrder(const STSubmitOrderInfo* pOrderInfo, STOrderBookInfo* pOrderBookInfo)
{
	UINT uiIndex = EffectiveMod(pOrderInfo->uiOrderIndex, MAX_ORDER_INDEX);
	//如果等于0，则认为是初始化状态，如果程序刚开始启动的时候，肯定是现有订单再有撤单
	//等于0时候，分为2种情况，也就是来了订单就被等待的市场单所成交了或者是程序因为某种原因重启之后，要从持久化队列中恢复数据到订单簿
	//由于恢复订单簿时候，会写OrderCache，此时如果等于0，就代表订单簿中没有该订单，也就是表示已经成交了，所以可以直接返回无需处理
	if (m_pOrderCache[uiIndex].lSubmitOrderID == 0)
	{
		return true;
	}
	else if (m_pOrderCache[uiIndex].lSubmitOrderID == pOrderInfo->lSubmitOrderID)
	{
		if (m_pOrderCache[uiIndex].pOrderAddr > 0)
		{
			STSubmitOrderInfo* pPreOrder = reinterpret_cast<STSubmitOrderInfo*>(m_pOrderCache[uiIndex].pOrderAddr);
			STExechOrderinfo stExecOrder;
			stExecOrder.lExchID = ++m_ulTransID;
			stExecOrder.lPrice = pPreOrder->lPrice;
			stExecOrder.fNumber = pPreOrder->fNumber;

			BYTE ucTransStatus = pPreOrder->ucTransStatus;
			if (ucTransStatus == EN_TRANS_STATUS_SUBMIT)
				pPreOrder->ucTransStatus = EN_TRANS_STATUS_CPL_CANCEL;
			else if (ucTransStatus == EN_TRANS_STATUS_PART_TRANS)
				pPreOrder->ucTransStatus = EN_TRANS_STATUS_PART_CANCEL;
			//如果处于全部成交，全部撤单或者部分撤单状态的话，此时该订单所对应剩余的数量应该为0，在这里这么做是为了将之前的成交信息和该次的撤单信息区分开，以遍在后续撮合成交，处理该订撤销单的时候，
			//订单总数能够减去正确地数量
			else
				pPreOrder->fNumber = 0;
			m_pOrderCache[uiIndex].pOrderAddr = 0;
	
			if (pPreOrder->ucTransType == EN_ORDER_BID)
				stExecOrder.lBidOrderID = pPreOrder->lSubmitOrderID;
			else
				stExecOrder.lAskOrderID = pPreOrder->lSubmitOrderID;
			stExecOrder.ucExchType = 0;
			WriteTransferOrder(&stExecOrder);

		}

		return true;
	}
	//如果不相等，则说明被该序号被冲掉了，则需要到OrderBook中去寻找,找到后，如果不是全部成交或者已经撤单的情况
	//则直接撤销该单，在mulmap中删除该点，并且在该价格所对应的总数量中减去锁撤销的单中的数量
	else
	{
		MapOrderBook* pMapOrderBook = (pOrderInfo->ucTransType == EN_ORDER_BID) ? &(pOrderBookInfo->mapBidOrderBook) : &(pOrderBookInfo->mapAskOrderBook);
		MapOrderBook::iterator iter = (*pMapOrderBook).find(pOrderInfo->lPrice);
		if (iter != (*pMapOrderBook).end())
		{
			typedef std::multimap<long, STSubmitOrderInfo*>::iterator TimeOrderIter;
			//	TimeOrderIter iterTimeOrder = (iter->second).find(pOrderInfo->lSubmitTime);

			std::pair<TimeOrderIter, TimeOrderIter> range = (iter->second).mulmapOrder.equal_range(pOrderInfo->lSubmitTime);
			while (range.first != range.second)
			{
				//如果查找到，则进行撤单操作，并返回
				if (range.first->second->lSubmitOrderID == pOrderInfo->lSubmitOrderID)
				{
					//如果是处于委托状态，则将状态改为全部成交
					bool bIsNeedCancel = false;
					if (range.first->second->ucTransStatus == EN_TRANS_STATUS_SUBMIT)
					{
						(iter->second).fTotalNumber -= range.first->second->fNumber;
						range.first->second->ucTransStatus = EN_TRANS_STATUS_CPL_CANCEL;
						bIsNeedCancel = true;
					}
					//如果是已经部分成交，则状态改为部分撤单
					else if (range.first->second->ucTransStatus == EN_TRANS_STATUS_PART_TRANS)
					{
						(iter->second).fTotalNumber -= range.first->second->fNumber;
						range.first->second->ucTransStatus = EN_TRANS_STATUS_PART_CANCEL;
						bIsNeedCancel = true;
					}
					//如果是全部成交或者是撤单状态，则无需进行撤单
					else
						bIsNeedCancel = false;
					if (bIsNeedCancel)
					{
						STExechOrderinfo stExecOrder;
						stExecOrder.lExchID = ++m_ulTransID;
						stExecOrder.lPrice = range.first->second->lPrice;
						stExecOrder.fNumber = range.first->second->fNumber;
						if (range.first->second->ucTransType == EN_ORDER_BID)
							stExecOrder.lBidOrderID = range.first->second->lSubmitOrderID;
						else
							stExecOrder.lAskOrderID = range.first->second->lSubmitOrderID;
						stExecOrder.ucExchType = 0;
						WriteTransferOrder(&stExecOrder);
					}
					//在mulmap中进行删除
					range.first = (iter->second).mulmapOrder.erase(range.first);
					return true;

				}
				else
					range.first++;
			}

		}
		return true;
	}

}

bool CMacthEngine::AddSubmitOrder(STSubmitOrderInfo* pSubmitOrder)
{
	STOrderBookInfo* pOrderBook = NULL;
	std::map<UINT, MapPlatformOrderBook>::iterator iter = m_mapMatchExch.find(pSubmitOrder->uiExchObjID);
	MapPlatformOrderBook::iterator iterPlatOrder;
	if (iter == m_mapMatchExch.end())
	{
		if (pSubmitOrder->ucIsCancelOrder == 1)
			return false;
		STOrderBookInfo stOrderBook;
		MapPlatformOrderBook mapPlatOrderBook;
		mapPlatOrderBook.clear();
		mapPlatOrderBook[pSubmitOrder->uiPlatformID] = stOrderBook;
		m_mapMatchExch[pSubmitOrder->uiExchObjID] = mapPlatOrderBook;
		iter = m_mapMatchExch.find(pSubmitOrder->uiExchObjID);
		iterPlatOrder = iter->second.find(pSubmitOrder->uiPlatformID);
	}
	else
	{
		iterPlatOrder = iter->second.find(pSubmitOrder->uiPlatformID);
		if (iterPlatOrder == iter->second.end())
		{
			if (pSubmitOrder->ucIsCancelOrder == 1)
				return false;
			STOrderBookInfo stOrderBook;
			iter->second.insert(std::make_pair(pSubmitOrder->uiPlatformID, stOrderBook));
			iterPlatOrder = iter->second.find(pSubmitOrder->uiPlatformID);
		}
	}
	pOrderBook = &(iterPlatOrder->second);

	if (pSubmitOrder->ucIsCancelOrder == 1)
	 return	CancelSubmitOrder(pSubmitOrder, pOrderBook);
	else
	{
		if (1 == pSubmitOrder->ucOrderType)
		   return ProcMarketOrder(pSubmitOrder, pOrderBook);
		else
		   return AddAskOrBidOrder(pSubmitOrder, pOrderBook);
	}

}
bool CMacthEngine::MatchExchangeObj(STOrderBookInfo& stOrderBook)
{
	//////////////////////////////////////////////////////////////////////////
// 	MapOrderBook::reverse_iterator  rev_iter_bid_test = stOrderBook.mapAskOrderBook.rbegin();
// 	for (; rev_iter_bid_test != stOrderBook.mapAskOrderBook.rend(); rev_iter_bid_test++)
// 		LOG_INFO << "test Ask Order Book:" << (1.0*rev_iter_bid_test->first) / FLOAT_PRICE_TO_LONG << " " << rev_iter_bid_test->second.fTotalNumber;
// 
// 	for (MapOrderBook::iterator iter_test = stOrderBook.mapBidOrderBook.begin(); iter_test != stOrderBook.mapBidOrderBook.end(); iter_test++)
// 	{
// 		LOG_INFO << "test Bid Order Book:" << (1.0*iter_test->first) / FLOAT_PRICE_TO_LONG << " " << iter_test->second.fTotalNumber;
// 	}

	
	//////////////////////////////////////////////////////////////////////////
	MapOrderBook::reverse_iterator  rev_iter_bid = stOrderBook.mapBidOrderBook.rbegin();
	typedef std::multimap<long, STSubmitOrderInfo*>::iterator timeorder_iter;
	if (stOrderBook.mapAskOrderBook.size() < 1 || stOrderBook.mapBidOrderBook.size() < 1)
	{
		LOG_DEBUG << "there is no order in ask_order_book or bid_order_book";
		return true;
	}	

	MapOrderBook::iterator iter_ask = stOrderBook.mapAskOrderBook.begin();
	timeorder_iter iter_time_ask = (iter_ask->second).mulmapOrder.begin();
	STExechOrderinfo stExecOrder;
	//反序遍历买单
	while (rev_iter_bid != stOrderBook.mapBidOrderBook.rend())
	{
		if (iter_ask->first > rev_iter_bid->first)
		{
			LOG_DEBUG << "ask price is greatter than bid price";
			return true;
		}
		//获取该价格下的所有买单
        timeorder_iter iter_time_bid = (rev_iter_bid->second).mulmapOrder.begin();

		while (iter_time_bid != (rev_iter_bid->second).mulmapOrder.end())
		{
			if (rev_iter_bid->first < iter_ask->first)
				return true;
			//遍历该价格下的所有卖单
			while (iter_time_ask != (iter_ask->second).mulmapOrder.end())
			{     
				memset(&stExecOrder, 0, sizeof(STExechOrderinfo));
				stExecOrder.lExchID = ++m_ulTransID;
				//成交价以买单的最高价为主
				stExecOrder.lPrice = iter_time_bid ->second->lPrice;
				stExecOrder.lAskOrderID = iter_time_ask->second->lSubmitOrderID;
				stExecOrder.lBidOrderID = iter_time_bid->second->lSubmitOrderID;
			 
				stExecOrder.ucExchType = 1;
				//如果买单数量小于等于卖单，则将该买单全部成交
 				if ((iter_time_bid->second->fNumber < iter_time_ask->second->fNumber) || IsFloatEqual(iter_time_bid->second->fNumber, iter_time_ask->second->fNumber))
				{
					stExecOrder.fNumber = iter_time_bid->second->fNumber;
					WriteTransferOrder(&stExecOrder);
					//修改买卖单的总数信息
					(rev_iter_bid->second).fTotalNumber -= iter_time_bid->second->fNumber;
	     			(iter_ask->second).fTotalNumber -= iter_time_bid->second->fNumber;

					iter_time_bid->second->ucTransStatus = EN_TRANS_STATUS_CPL_TRANS;
//					iter_time_ask->second->fNumber -= iter_time_bid->second->fNumber;
					iter_time_ask->second->ucTransStatus = EN_TRANS_STATUS_PART_TRANS;
					UINT uiIndex = EffectiveMod(iter_time_bid->second->uiOrderIndex, MAX_ORDER_INDEX);
					if (0 == m_pOrderCache[uiIndex].lSubmitOrderID)
						LOG_ERROR << "Order Cache's submit order id is null. Org SubmitOrdrID =" << iter_time_bid->second->lSubmitOrderID << "OrderIndex = " << iter_time_bid->second->uiOrderIndex;
					//设置OrderCache中的标记为已成交
					else if (m_pOrderCache[uiIndex].lSubmitOrderID == iter_time_bid->second->lSubmitOrderID)
						m_pOrderCache[uiIndex].pOrderAddr = 0;
			
                    //如果相等的话，则同时也要清除卖单的相关信息
					if (IsFloatEqual(iter_time_bid->second->fNumber, iter_time_ask->second->fNumber))
					{
						iter_time_ask->second->fNumber -= iter_time_bid->second->fNumber;
						iter_time_ask->second->ucTransStatus = EN_TRANS_STATUS_CPL_TRANS;
						uiIndex = EffectiveMod(iter_time_ask->second->uiOrderIndex, MAX_ORDER_INDEX);
						if (0 == m_pOrderCache[uiIndex].lSubmitOrderID)
							LOG_ERROR << "Order Cache's submit order id is null. Org AskSubmitOrdrID =" << iter_time_ask->second->lSubmitOrderID << "OrderIndex = " << iter_time_ask->second->uiOrderIndex;
						//设置OrderCache中的标记为已成交
						else if (m_pOrderCache[uiIndex].lSubmitOrderID == iter_time_ask->second->lSubmitOrderID)
							m_pOrderCache[uiIndex].pOrderAddr = 0;
						//释放内存
						m_memPool.destroy(iter_time_ask->second);
						m_memPool.deallocate(iter_time_ask->second);
						iter_time_ask = (iter_ask->second).mulmapOrder.erase(iter_time_ask);
					}
					else
					{
						iter_time_ask->second->ucTransStatus = EN_TRANS_STATUS_PART_TRANS;
						iter_time_ask->second->fNumber -= iter_time_bid->second->fNumber;
					}
			
					//释放买单的内存和相关信息
					m_memPool.destroy(iter_time_bid->second);
					m_memPool.deallocate(iter_time_bid->second);
					iter_time_bid = (rev_iter_bid->second).mulmapOrder.erase(iter_time_bid);
					//如果在卖单的队列里，该买单被完全交易，则跳出卖单队列循环，重新找下一个买单
					break;
				}
				else//如果买单的数量大于卖单
				{
					stExecOrder.fNumber = iter_time_ask->second->fNumber;
					WriteTransferOrder(&stExecOrder);
					//修改买卖单的总数信息
					(rev_iter_bid->second).fTotalNumber -= iter_time_ask->second->fNumber;
					(iter_ask->second).fTotalNumber -= iter_time_ask->second->fNumber;

					iter_time_bid->second->ucTransStatus = EN_TRANS_STATUS_PART_TRANS;
					iter_time_bid->second->fNumber -= iter_time_ask->second->fNumber;
					//清除卖单的相关信息
					iter_time_ask->second->ucTransStatus = EN_TRANS_STATUS_CPL_TRANS;
					UINT uiIndex = EffectiveMod(iter_time_ask->second->uiOrderIndex, MAX_ORDER_INDEX);
					if (0 == m_pOrderCache[uiIndex].lSubmitOrderID)
						LOG_ERROR << "Order Cache's submit order id is null. Org AskSubmitOrdrID =" << iter_time_ask->second->lSubmitOrderID << "OrderIndex = " << iter_time_ask->second->uiOrderIndex;
					//设置OrderCache中的标记为已成交
					else if (m_pOrderCache[uiIndex].lSubmitOrderID == iter_time_ask->second->lSubmitOrderID)
						m_pOrderCache[uiIndex].pOrderAddr = 0;
					//释放内存
					m_memPool.destroy(iter_time_ask->second);
					m_memPool.deallocate(iter_time_ask->second);
					iter_time_ask = (iter_ask->second).mulmapOrder.erase(iter_time_ask);
				}
			
			}
			//如果卖单中该价格的所有买单都已成交，则删除当前价格，获取下一个价格的卖单信息
			if (iter_time_ask == (iter_ask->second).mulmapOrder.end())
			{
				iter_ask = stOrderBook.mapAskOrderBook.erase(iter_ask);
				//如果卖单全部被成交了，则推出
				if (iter_ask == stOrderBook.mapAskOrderBook.end())
				{
					return true;
				}
				iter_time_ask = (iter_ask->second).mulmapOrder.begin();
			}
		}
		//能运行到这步，说明该价格的买单已经全部被交易，需要获取下一个价格的买单
		rev_iter_bid = MapOrderBook::reverse_iterator(stOrderBook.mapBidOrderBook.erase((++rev_iter_bid).base()));

	}
	return true;


}
bool CMacthEngine::StartMatchExchange()
{
	std::map<UINT, MapPlatformOrderBook>::iterator iter = m_mapMatchExch.begin();
	MapPlatformOrderBook::iterator iterPlatorder;
	for (; iter != m_mapMatchExch.end(); iter++)
	{
		for (iterPlatorder = iter->second.begin(); iterPlatorder != iter->second.end(); iterPlatorder++)
		{
			MatchExchangeObj(iterPlatorder->second);
		}
		
	}
	return true;
}

bool CMacthEngine::WriteMarketSnapShot()
{
	//temp delete
	//LOG_INFO << "Current proc transmition number:" << m_iProcCnt << " and write exchange order wait times:"<<m_iWriteWaitCnt;
	m_iProcCnt = 0;
	m_iWriteWaitCnt = 0;
	WriteOrderBook();
	return true;
}

bool CMacthEngine::StartWork()
{
	m_bIsRunning = true;
	if (pthread_create(&m_threadID, NULL, MatchExchangeFunc, this) != 0)
	{
		LOG_FATAL<<"create CMacthEngine thread  failed";
		return false;
	}
	return true;
}

void* CMacthEngine::MatchExchangeFunc(void* pInfo)
{
	CMacthEngine* pMatchExch = (CMacthEngine*)pInfo;
	STSubmitOrderInfo stSubmitOrder;
	int iSubmitOrderLen = sizeof(STSubmitOrderInfo);
	//如果小于255,则认为设置亲和性
	if (pMatchExch->m_iSetCpuID < 255)
	{
		cpu_set_t cpuSet;
		CPU_ZERO(&cpuSet);
		CPU_SET(pMatchExch->m_iSetCpuID, &cpuSet);
		int iRet = pthread_setaffinity_np(pthread_self(), sizeof(cpuSet), &cpuSet);
		if (iRet != 0)
		{
			LOG_ERROR << "set thread(MacthEngine) affinity failed.error = " << iRet;
			return ((void*)0);
		}
	}

	while (pMatchExch->m_bIsRunning)
	{
		//查看是否到了撮合成交的时间
		if (1 == pMatchExch->GetMatchTimeFlag())
		{
			pMatchExch->StartMatchExchange();
			pMatchExch->SetTimerEvent(EN_TIMER_MATCH, 0);
		}
		if (1 == pMatchExch->GetSnapShotFlag())
		{
			pMatchExch->WriteMarketSnapShot();
			pMatchExch->SetTimerEvent(EN_TIMER_MARKET_SNAPSHOT, 0);
		}
		DWORD dwReadBytes = pMatchExch->m_pInputRingBuf->getUsedSize();
		if (dwReadBytes == 0)//如果数据取完了，则休眠5微妙
		{
			usleep(5);
			continue;
		}
		//如果此时出现读取的数据不是订单的整数倍的情况，报错，并丢掉该部分的数据
		else if (dwReadBytes < iSubmitOrderLen)
		{
			LOG_ERROR << "ReadBytes is not integer multiples of submit order's length , readed bytes =  " << dwReadBytes;
			pMatchExch->m_pInputRingBuf->popData(dwReadBytes);
			continue;
		}
		else
		{
			if (!pMatchExch->m_pInputRingBuf->popData(&stSubmitOrder, iSubmitOrderLen, iSubmitOrderLen))
			{
				continue;
			}
			if (pMatchExch->m_lStartTime == 0)
			{
				pMatchExch->m_lStartTime = muduo::Timestamp::now().microSecondsSinceEpoch();
			}
			pMatchExch->AddSubmitOrder(&stSubmitOrder);
		}

	}
	return ((void*)0);
}
//定时的回调函数
void CMacthEngine::SetTimerEvent(int iEvent, int iVal)
{
	muduo::MutexLockGuard mutexLockGuard(m_muteLock);
	switch (iEvent)
	{
	case EN_TIMER_MATCH:
		m_iMatchTimerFlag = iVal;
		break;
	case EN_TIMER_MARKET_SNAPSHOT:
		m_iSnapShotFlag = iVal;
		break;
	default:
		LOG_ERROR << "Unknow timer event val : " << iEvent;
		break;
	}
}