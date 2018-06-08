#pragma once
#include <string.h>
#include "XRingBuffer.h"
#include <vector>
//#include <c++\4.8\ext\hash_map>
#include <map>
#include <functional>
#include <muduo/net/EventLoop.h>
#include <muduo/net/EventLoopThread.h>

#define MAX_ORDER_INDEX   8388608 //8*1024*1024
#define MATCH_TIME_INTERVAL 0.01  //匹配的时间间隔为10ms
#define MARKET_SNAPSHOT_TIME_INTERVAL 3//市场快照的行情间隔为3秒
#define SERVER_STATUS_TIME_INTERVAL  5
#define FLOAT_PRICE_TO_LONG   10000000 //将价格从float类型转化为long型的系数，七位足够保证精度
#define MARKET_MAGIC_WORDS    0xFF8899FF
#define MAX_USR_ORDERID_LEN   33
typedef unsigned char BYTE;
typedef unsigned int UINT;
typedef muduo::net::EventLoop   MuduoEventLoop;
typedef muduo::net::EventLoopThread  MuduoEventLoopThread;

enum EN_TRANS_STATUS
{
	EN_TRANS_STATUS_SUBMIT = 0,
	EN_TRANS_STATUS_CPL_TRANS,
	EN_TRANS_STATUS_PART_TRANS,
	EN_TRANS_STATUS_CPL_CANCEL = 4
//	EN_TRANS_STATUS_PART_CANCEL
};

enum EN_TIMER_EVENT
{
	EN_TIMER_MATCH = 0,
	EN_TIMER_MARKET_SNAPSHOT,
	EN_TIMER_SERVER_STATUS

};

enum  EN_ORDER_TYPE
{
	EN_ORDER_BID = 0,
	EN_ORDER_ASK
};

typedef struct ST_SUBMIT_ORDER_INFO
{
	long lSubmitOrderID;    //订单ID
	long lSubmitTime;       //下单时间，单位为微妙
	long  lPrice;           //交易价格,在这里会将交易单乘以FLOAT_PRICE_TO_LONG让其转化为
	UINT  uiExchObjID;         //交易品种代码
	UINT  uiPlatformID;      //客户端锁在的平台注册ID，用于标识是哪个交易所的数据
	UINT  uiOrderIndex;     //接收端分配的索引号，为顺序增长，从0开始
	long  lNumber;          //交易数量
	BYTE ucTransType;       //交易类型，0 表示买， 1表示卖
	BYTE ucOrderType;       //订单类型，0 表示限价订单，1表示市场订单
	BYTE ucTransStatus;     //成交状态， 0：已委托， 1:全部成交 2：部分成交 3：全部撤单 4：部分撤单
	BYTE ucIsCancelOrder;   //是否进行撤单请求， 0：不撤单， 1：撤单
	char chUsrOrderID[MAX_USR_ORDERID_LEN];
	BYTE chReservse[3];
	ST_SUBMIT_ORDER_INFO()
	{
		memset(this, 0, sizeof(ST_SUBMIT_ORDER_INFO));
	}
	~ST_SUBMIT_ORDER_INFO()
	{
		memset(this, 0, sizeof(ST_SUBMIT_ORDER_INFO));
	}
}STSubmitOrderInfo;


typedef struct ST_EXCHANGE_ORDER_INFO
{
	long lExchID;         //成交编号
	long lBidOrderID;     //买单编号
	long lAskOrderID;     //卖单编号
	long lPrice;         //价格
	long lNumber;         //数量
	UINT uiBidUsrID;
	UINT uiAskUsrID;
	UINT uiSymbolID;
	BYTE  ucExchType;     //交易类型  0:撤单 1:成交 
	char chBidUsrOrderID[MAX_USR_ORDERID_LEN]; //用户平台中的订单ID
	char chAskUsrOrderID[MAX_USR_ORDERID_LEN];
	char  chReservse[7];
	ST_EXCHANGE_ORDER_INFO()
	{
		memset(this, 0, sizeof(ST_EXCHANGE_ORDER_INFO));
	}
}STExechOrderinfo;



//某一个价格下所对应的订单数据
typedef struct ST_PRICE_ORDER_INFO
{
	long lTotalNumber;
	std::multimap<long, STSubmitOrderInfo*> mulmapOrder;  //对应的以时间为排序的订单
	ST_PRICE_ORDER_INFO()
	{
		lTotalNumber = 0;
		mulmapOrder.clear();
	}
}STPriceOrderInfo;

typedef std::map<long, STPriceOrderInfo> MapOrderBook;

typedef struct ST_ORDER_BOOK_INFO
{
	MapOrderBook mapAskOrderBook;
	MapOrderBook mapBidOrderBook;
	ST_ORDER_BOOK_INFO()
	{
		mapAskOrderBook.clear();
		mapBidOrderBook.clear();
	}
}STOrderBookInfo;
//用于关联交易平台和订单簿
typedef std::map<UINT, STOrderBookInfo> MapPlatformOrderBook;

typedef std::function<void(int, int)> OnTimerCallBack;

typedef struct ST_SIG_ORDER_ITEM_INFO
{
	long lPrice;
	long lNumber; 
	ST_SIG_ORDER_ITEM_INFO()
	{
		lPrice = 0;
		lNumber = 0;
	}


}STSigOrderBookItem;
#pragma pack(4)
typedef struct ST_MARKET_HEADER_INFO
{
	unsigned int uiMagicWords;
	unsigned short usLen;
	unsigned char wMsgType; //0表示成交数据，1表示订单簿
	unsigned char wTransType;//表示订单薄的交易类型，0 表示买， 1表示卖
	ST_MARKET_HEADER_INFO()
	{
		memset(this, 0, sizeof(ST_MARKET_HEADER_INFO));
	}


}STMarketHeaderInfo;
#pragma pack()

#define MIN_FLOAT_PRECISE 0.000001
// inline bool IsFloatEqual(float fVal1, float fVal2)
// {
// 	return (fabs(fVal1 - fVal2) < MIN_FLOAT_PRECISE) ? true : false;
// }
// typedef struct ST_PRICE_ORDER_INFO
// {
// 	float fPrice;
// 	//std::vector<XRingBuffer> vtRingBuf;
// 	//mulmapSubmitOrder是一个关于时间和订单的map,时间的精度为微妙
// 	std::multimap<long, STSubmitOrderInfo> mulmapSubmitOrder;
// 	ST_PRICE_ORDER_INFO()
// 	{
// 		fPrice = 0;
// 		mulmapSubmitOrder.clear();
// 	}
// }STPriceOrderInfo;
#define TAG_INTERVAL_BUF 0xFF8899FF
//#pragma pack(4)
typedef struct ST_INTERVAL_BUF_HEADER
{
	unsigned int uiMagicWords;
	int  ilen;
	unsigned int  uiMsgType;
	unsigned int  uiReserve;
	ST_INTERVAL_BUF_HEADER()
	{
		uiMagicWords = 0;
		uiMsgType = ilen = 0;
		uiReserve = 0;
	}
}STIntervalBufHeader;
//#pragma pack()

enum  EN_INTERVAL_CMD_INFO
{
	EN_INTERVAL_AUTH = 0,
	EN_INTERVAL_SUBMIT_ORDER,
	EN_INTERVAL_CANCEL_ORDER,
	EN_INTERVAL_QUERY_ORDER,
	EN_INTERVAL_MARKET_TRANS_DATA,
	EN_INTERVAL_MARKET_QUOTA_DATA,
	EN_INTERVAL_MARKET_ORDER_BOOK,
	EN_INTERVAL_QUERY_HISTORY_ORDER,

	EN_INTERVAL_AUTH_RESP = 20,
	EN_INTERVAL_SUBMIT_ORDER_RESP,
	EN_INTERVAL_CANCEL_ORDER_RESP,
	EN_INTERVAL_QUERY_ORDER_RESP

};