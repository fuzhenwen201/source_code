#ifndef _ETHJSONRPC_H_
#define _ETHJSONRPC_H_
#include <string>
//#include "uint128_t.h"
#include "const.h"
struct EThAccount{
    std::string Adress_;
    std::string Pwd_;
    std::string Balance_;
    int ID_;
};

struct TransactionInfo{
    std::string fromAddress_;
    std::string toAddress_;
    std::string txHash_;
    long blockHeight_ ;
    std::string txAge_;
    std::string txValue_;
    std::string txGas_;
    std::string txGasPrice_;
    std::string txGasUsed_;
};
class ETHOperJsonRPC {
public:
    ETHOperJsonRPC(const char* URL);
    ~ETHOperJsonRPC();
    void SetURL(const char* URL) {
        URL_ = URL;
    };
    bool NewAccount(STExchangeAddrInfo& account);
    bool UnlockAccount(STTradeData& tx);
    bool LockAccount(STTradeData& tx);
    bool SendTransaction(STTradeData& tx,bool isFlag = false);
    std::string GetETHBalance(std::string& addr);
    uint64_t GetETHBlockNumber();
    bool CheckTransaction(BlockChainInfo& info,std::map<std::string,STDepositAddr> *pDesportAddrMap,list<STDepositAddr>& desportInfo_out);
    bool GetBlockbyNumber(uint32_t block,std::map<std::string,STDepositAddr> *pDesportAddrMap,list<STDepositAddr>& desportInfo_out );
    bool CheckingTransactionState(STConfirmData& tx);

private:
    std::string URL_;  
};
#endif
