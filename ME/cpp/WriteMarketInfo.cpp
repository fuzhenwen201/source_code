#include "WriteMarketInfo.h"
#include "./RabbitMQ/tnode.h"
#include "MyProtobuf.h"

extern snetwork_xservice_tnode::TNode* g_pTNode;

CWriteMarketInfo::CWriteMarketInfo()
{
	m_pRingBuf = NULL;
	m_bIsRunning = false;
	m_iSetCpuID = 255;
	m_threadID = 0;
}


CWriteMarketInfo::~CWriteMarketInfo()
{
	if (m_bIsRunning)
        ExitWork();
	
}

bool CWriteMarketInfo::StartWork()
{
	m_bIsRunning = true;
	if (pthread_create(&m_threadID, NULL, WriteMarketTrans, this) != 0)
	{
		LOG_FATAL << "create CWriteMarketInfo thread  failed";
		return false;
	}
	return true;
}

bool CWriteMarketInfo::StartPublishData(long lParam)
{
	if (m_pRingBuf == NULL)
	{
		LOG_FATAL << "Input buffer can not be null in write market order";
		return NULL;
	}
	if (m_iSetCpuID < 255)
	{
		cpu_set_t cpuSet;
		CPU_ZERO(&cpuSet);
		CPU_SET(m_iSetCpuID, &cpuSet);
		int iRet = pthread_setaffinity_np(pthread_self(), sizeof(cpuSet), &cpuSet);
		if (iRet != 0)
		{
			LOG_ERROR << "set thread(WriteMarketTrans) affinity failed.error = " << iRet;
			return NULL;
		}
	}
	m_bIsRunning = true;
	STExechOrderinfo stExcOrderInfo;
	int iLen = 0;
	int iHeaderLen = sizeof(STMarketHeaderInfo);
	STMarketHeaderInfo stMarketHeader;
	char chKeyInfo[60] = { 0 };
	int iFlowNumber = 0;
	std::string strContent;
	while (m_bIsRunning)
	{
		if (!m_pRingBuf->popData(&stMarketHeader, iHeaderLen, iHeaderLen))
		{
			usleep(2);
			continue;
		}
		if (stMarketHeader.uiMagicWords != MARKET_MAGIC_WORDS)
		{
			LOG_ERROR << "receive market magic words error£¬recv value = " << stMarketHeader.uiMagicWords << " in write market trading module.";
			usleep(1000000);
			continue;
		}
		if (0 == stMarketHeader.wMsgType)
		{
			iLen = sizeof(STExechOrderinfo);
			if (stMarketHeader.usLen != iLen)
			{
				LOG_ERROR << "receive market data's length error.rece data's len = " << stMarketHeader.usLen << "should be " << iLen;
				usleep(1000000);
				continue;
			}
			while (false == m_pRingBuf->popData(&stExcOrderInfo, iLen, iLen))
				usleep(2);
			
			sprintf(chKeyInfo, "_MarketData.Trade.%ld", stExcOrderInfo.uiBidUsrID);
			strContent.clear();
			CMyProtobuf::GetInstance()->SearlizeInnerDealInfoToString(stExcOrderInfo, ++iFlowNumber, strContent);
			if (-1 == g_pTNode->PublishToMQ(chKeyInfo, (char*)(strContent.c_str()), strContent.size()))
				LOG_ERROR << "PublishToMQ failed.routting_key = " << chKeyInfo;
		}

	}
}


void* CWriteMarketInfo::WriteMarketTrans(void* pInfo)
{
	CWriteMarketInfo* pWirteMarket = (CWriteMarketInfo*)pInfo;
	if (pWirteMarket->m_pRingBuf == NULL)
	{
		LOG_FATAL << "Input buffer can not be null in write market order";
		return NULL;
	}
	if (pWirteMarket->m_iSetCpuID < 255)
	{
		cpu_set_t cpuSet;
		CPU_ZERO(&cpuSet);
		CPU_SET(pWirteMarket->m_iSetCpuID, &cpuSet);
		int iRet = pthread_setaffinity_np(pthread_self(), sizeof(cpuSet), &cpuSet);
		if (iRet != 0)
		{
			LOG_ERROR << "set thread(WriteMarketTrans) affinity failed.error = " << iRet;
			return NULL;
		}
	}
	STExechOrderinfo stExcOrderInfo;
	int iLen = 0;
	int iHeaderLen = sizeof(STMarketHeaderInfo);
	STMarketHeaderInfo stMarketHeader;
	std::string str;
	while (pWirteMarket->m_bIsRunning)
	{
		//////////////////////////////////ÎªÁËÑÝÊ¾////////////////////////////////////////
		if (!pWirteMarket->m_pRingBuf->popData(&stMarketHeader, iHeaderLen, iHeaderLen))
		{
			usleep(2);
			continue;
		}
		if (stMarketHeader.uiMagicWords != MARKET_MAGIC_WORDS)
		{
			LOG_ERROR << "receive market magic words error£¬recv value = " << stMarketHeader.uiMagicWords << " in write market trading module.";
			usleep(3000000);
			continue;
		}
		if (0 == stMarketHeader.wMsgType)
		{
			iLen = sizeof(STExechOrderinfo);
			if (stMarketHeader.usLen != iLen)
			{
				LOG_ERROR << "receive market data's length error.rece data's len = " << stMarketHeader.usLen << "should be " << iLen;
				usleep(3000000);
				continue;
			}
			while (1)
			{
				if (!pWirteMarket->m_pRingBuf->popData(&stExcOrderInfo, iLen, iLen))
				{
					usleep(2);
					continue;
				}
				break;
			}
	
			//	str.clear();
			//   		sprintf(str, "ExchID=%ld, ExchType = %d, Price = %f, Number = %f, AskOrderID = %ld, BidOrderID = %ld", stExcOrderInfo.lExchID,
			//  			stExcOrderInfo.ucExchType, stExcOrderInfo.fPrice, stExcOrderInfo.fNumber, stExcOrderInfo.lAskOrderID, stExcOrderInfo.lBidOrderID);
			LOG_INFO << "ExchID=" << stExcOrderInfo.lExchID << " ExchType=" << stExcOrderInfo.ucExchType << " Price=" << (float)(1.0*stExcOrderInfo.lPrice / FLOAT_PRICE_TO_LONG) << " Number=" << (float)(1.0*stExcOrderInfo.lNumber / FLOAT_PRICE_TO_LONG)
			    	<< " AskOrderID=" << stExcOrderInfo.lAskOrderID << "  BidOrderID=" << stExcOrderInfo.lBidOrderID;

		}
		else if (1 == stMarketHeader.wMsgType)
		{
		//	std::string strOrderStatus = (stMarketHeader.wTransType == 0) ? "Bid :" : "Ask ";
			iLen = sizeof(STSigOrderBookItem);
			if (stMarketHeader.usLen % iLen != 0)
			{
				LOG_ERROR << "receive market data's length is not multiple of SIG_ORDER_ITEM.rece data's len = " << stMarketHeader.usLen;
				pWirteMarket->m_pRingBuf->popData(stMarketHeader.usLen);
				usleep(1000000);
				continue;
			}
			int iCnt = stMarketHeader.usLen / iLen;
			while (pWirteMarket->m_pRingBuf->getUsedSize()<stMarketHeader.usLen)
			{
				usleep(2);
				continue;
			}
			STSigOrderBookItem stSigOrder;
			for (int i = 0; i < iCnt; i++)
			{
				pWirteMarket->m_pRingBuf->popData(&stSigOrder, iLen, iLen);
				if (0 == stMarketHeader.wTransType)
				{
					LOG_INFO << "Bid " << i + 1 << ":        " << (float)(1.0*stSigOrder.lPrice / FLOAT_PRICE_TO_LONG) << "        " << (float)(1.0*stSigOrder.lNumber / FLOAT_PRICE_TO_LONG);
				}
				else
				{
					LOG_INFO << "Ask " << iCnt - i << ":        " << (float)(1.0*stSigOrder.lPrice / FLOAT_PRICE_TO_LONG) << "        " << (float)(1.0*stSigOrder.lNumber / FLOAT_PRICE_TO_LONG);
				}

			}

		}
		else
		{
			LOG_ERROR << "Unknow MsgType = " << stMarketHeader.wMsgType;
			usleep(1000000);
			continue;
		}

		//////////////////////////////////////////////////////////////////////////
		/*
		if (!pWirteMarket->m_pRingBuf->popData(&stExcOrderInfo, iLen, iLen))
		{
			usleep(2);
			continue;
		}
	//	str.clear();
//   		sprintf(str, "ExchID=%ld, ExchType = %d, Price = %f, Number = %f, AskOrderID = %ld, BidOrderID = %ld", stExcOrderInfo.lExchID,
//  			stExcOrderInfo.ucExchType, stExcOrderInfo.fPrice, stExcOrderInfo.fNumber, stExcOrderInfo.lAskOrderID, stExcOrderInfo.lBidOrderID);
 		LOG_INFO << "ExchID=" << stExcOrderInfo.lExchID << " ExchType=" << stExcOrderInfo.ucExchType << " Price=" << (float)(1.0*stExcOrderInfo.lPrice/FLOAT_PRICE_TO_LONG) << " Number=" << stExcOrderInfo.fNumber
 			<< " AskOrderID=" << stExcOrderInfo.lAskOrderID << "  BidOrderID=" << stExcOrderInfo.lBidOrderID;
//		LOG_INFO << str;
		//str.clear();*/
	}
	return NULL;

}

