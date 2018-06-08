#ifndef BITCOINJSONRPC_H
#define BITCOINJSONRPC_H
#include <string>

#include "const.h"
 
#include <map>


class BitCoinAPI {
    public:
        BitCoinAPI(const char* URL,const char* Name,const char* Pwd);
        ~BitCoinAPI();
        bool GetNewAddress(STExchangeAddrInfo* info);

		std::string GetAddressInfo(const char *pAddr);

        bool GetAddressesbyAccount(STExchangeAddrInfo* info);

        double GetBalance(STExchangeAddrInfo* info);
        std::string ShowPrivatekey(STExchangeAddrInfo* info) ;

        bool UnLockWallet(const char* passphrase,size_t timout=100);
        bool LockWallet();
       
        bool EncryptWallet(const char* passphrase);
         
        bool SendToAddress(STTradeData* pData);

        bool GetTransaction(STConfirmData* pData);

        int32_t GetBlockNum();

        bool Listunspent(BlockChainInfo& info,std::map<std::string,STDepositAddr> *pDesportAddrMap,list<STDepositAddr>& desportInfo_out);

        bool GetBlockTemplate();

        bool GetBlockChainInfo();

        bool SetPayFee(double Amout);

        void SetURL(const char* URL) {
            URL_ = URL;
        };
        void SetUserName(const char* Name){
            UserName_ = Name;
        }
        void SetPwd(const char* Pwd){
            Pwd_ = Pwd;
        }
    private:
        std::string URL_; 
        std::string UserName_;
        std::string Pwd_;
    
   
};

#endif