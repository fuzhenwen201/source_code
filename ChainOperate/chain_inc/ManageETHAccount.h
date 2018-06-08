#ifndef _ManageETHAccount_H_
#define _ManageETHAccount_H_

#include "EthJsonRPC.h"
#include "ThreadPool.h"
#include <map>
#include <set>

extern ThreadPool* g_threadpool; 
#define BLANK 20
class CManagementETH{
public:
    CManagementETH();
    ~CManagementETH();
    bool InitETHInfo(ETHOperJsonRPC* oper ,int Height = 0);
    bool NewETHAccount(std::string& address);
    bool AddETHAccount(std::string& address);
    void RunETH();
    void StopETH();
    bool WithdrawalETH(std::string& srcAddress,std::string& destAddress,std::string& value);
    bool AggregateFunds(std::string& srcAddress,std::string& destAddress);
    void SettingGas(std::string& gas,std::string gasPrice){
        gas_ = gas;
        gasPrice_ = gasPrice;
    }
//protected:
    bool InitETHAccount();
    void SearchTransactionInfo(int block);
    bool CheckTransactionInfo();
private:
    long blockHeight_;
    ETHOperJsonRPC *operETH_;
    std::map<std::string,TransactionInfo*> txTmpMap_;
    std::mutex TmpMutex_;
    std::set<std::string> addressList_;
    std::mutex addressMutex_;
    bool isStop_;
    std::string gas_ ;
    std::string gasPrice_;

};

#endif