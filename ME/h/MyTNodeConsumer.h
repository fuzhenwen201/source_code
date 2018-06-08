#pragma once
#include "./RabbitMQ/tnode.h"
#include "./XRingBuffer.h"
#include "StockExcDef.h"
#include <unistd.h>
#include "MyProtobuf.h"
class CMyTNodeConsumer : public snetwork_xservice_tnode::TNodeConsumer
{
public:
	CMyTNodeConsumer(const char* pRoutingKey, const char* pQueneName)
	{
		m_strQuenename = pQueneName;
		m_strRoutingKey = pRoutingKey;
		m_pDataBuffer = NULL;
	//	m_pDataSndBuf = NULL;

	}
	virtual ~CMyTNodeConsumer() { return; }
	virtual std::string GetBindingkey() { return m_strRoutingKey; }
	virtual std::string GetQueueName() { return m_strQuenename; }
	void RegisterDataBuffer(XRingBuffer* pBuffer) { assert(pBuffer != NULL); m_pDataBuffer = pBuffer; }
//	void RegisterSendBuffer(XRingBuffer* pBuffer) { assert(pBuffer != NULL); m_pDataSndBuf = pBuffer; }
	unsigned int ConsumerData(char *pMsg, int nMsgLen)
	{
		if (nMsgLen < 1 || pMsg == NULL)
		{
			LOG_INFO << "recv data from mq error.MsgLen = " << nMsgLen << "or data address is empty";
			return 1;
		}
		assert(m_pDataBuffer != NULL);
		STSubmitOrderInfo stSubOrderInfo;
		if (false == CMyProtobuf::GetInstance()->ParserSubOrderToInnerData(pMsg, nMsgLen, stSubOrderInfo))
		{
			return 1;
		}
		 
// 		ST_INTERVAL_BUF_HEADER stDataHeader;
// 		stDataHeader.uiMagicWords = TAG_INTERVAL_BUF;
// 		stDataHeader.ilen = nMsgLen;
		int iRepIndex = 0;
		while (1)
		{
			if (m_pDataBuffer->getFreeSize() >=  sizeof(stSubOrderInfo) )
			{
				iRepIndex = 0;
				/*m_pDataBuffer->pushData(&stDataHeader, sizeof(stDataHeader));*/
				m_pDataBuffer->pushData(&stSubOrderInfo, sizeof(stSubOrderInfo));
				return 1;

			}
			iRepIndex++;
			if (iRepIndex % 5 == 0)
			{
				//在这里使用LOG_ERROR时，必须如果在判断语句中必须使用括号，因为实际上LOG_ERROR在输出时是宏定义的2个语句
				LOG_ERROR << "push data to ringbuffer failed 5 times . msglen = " << sizeof(stSubOrderInfo);
			}
			usleep(20);

		}

	}

protected:
	std::string	m_strRoutingKey;
	std::string	m_strQuenename;
	XRingBuffer* m_pDataBuffer;
//	XRingBuffer* m_pDataSndBuf;

};

