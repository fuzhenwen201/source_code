#ifndef NEWEXCHANGE_CHAINOPERATOR_HEAD
#define NEWEXCHANGE_CHAINOPERATOR_HEAD

#include <string.h>
#include <list>
#include <stdio.h>

#include "xlogger.h"
#include "EthJsonRPC.h"
#include "BitCoinJsonRPC.h"
#include <set>
using namespace std;
using namespace snetwork_xservice_xflagger;



class CChainOperator
{
public:
	CChainOperator();
	~CChainOperator();
	int Init(std::list<BlockChainInfo>& info);
	int Clean();
	
	// 入金检测
	bool CheckDeposit(BlockChainInfo& info, std::map<std::string,STDepositAddr> *pDesportAddrMap,list<STDepositAddr>& desportInfo_out);
	
	// 转币
	bool TransferCoin(ChainType type , STTradeData *pTradeDataList);
	
	// 交易确认
	bool TradeConfirm(ChainType type,STConfirmData *pConfirmDataList);
	
	// 创建地址 
	bool CreateAddr(ChainType type, STExchangeAddrInfo *pExchangeAddrInfoList);
	std::string GetBlance(std::string& addr);
	
private:
	BitCoinAPI* bitcoinapi_ ;
	ETHOperJsonRPC* ethapi_;
	
};

#endif 






