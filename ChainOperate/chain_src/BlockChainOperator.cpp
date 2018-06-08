#include "BlockChainOperator.h"
 


CChainOperator::CChainOperator(){

}

CChainOperator::~CChainOperator(){
	Clean();
}
int CChainOperator::Init(std::list<BlockChainInfo>& info){
     std::list<BlockChainInfo>::iterator it;
     for(it = info.begin(); it != info.end(); it++)
     {
         switch(it->type)
         {
            case ChainType::TYPE_BITCOIN :
                bitcoinapi_ = new BitCoinAPI(it->sChainURL,it->sChainUserName,it->sChainPWD);
                break;
            case ChainType::TYPE_ETHEREUM :
                ethapi_ = new ETHOperJsonRPC(it->sChainURL);
                break;
            default:
                XERROR("Blockchain's type is error.");
         }
     }
     return 0;
}

int CChainOperator::Clean(){
    return 0;
}

bool CChainOperator::CheckDeposit(BlockChainInfo& info, std::map<std::string,STDepositAddr> *pDesportAddrMap,list<STDepositAddr>& desportInfo_out){
    switch(info.type)
    {
        case ChainType::TYPE_BITCOIN:
            return bitcoinapi_->Listunspent(info, pDesportAddrMap, desportInfo_out);
        case ChainType::TYPE_ETHEREUM:
            return ethapi_->CheckTransaction(info, pDesportAddrMap, desportInfo_out);
        default:
            XERROR("Blockchain's type is error.");
    }
    return false;
}

bool CChainOperator::TransferCoin(ChainType type , STTradeData *pTradeDataList){
    std::set<STTradeData>::iterator it;
    switch(type) {
        case ChainType::TYPE_BITCOIN:
            return bitcoinapi_->SendToAddress(pTradeDataList);
        case ChainType::TYPE_ETHEREUM:
            return ethapi_->SendTransaction(*pTradeDataList);
        default:
            XERROR("Blockchain's type is error");
    }
    return false;
}

std::string CChainOperator::GetBlance(std::string& addr){
    return ethapi_->GetETHBalance(addr);
}

bool CChainOperator::TradeConfirm(ChainType type,STConfirmData *pConfirmDataList){
    switch(type) {
        case ChainType::TYPE_BITCOIN:
            return bitcoinapi_->GetTransaction(pConfirmDataList);
        case ChainType::TYPE_ETHEREUM:
            return ethapi_->CheckingTransactionState(*pConfirmDataList);
        default:
            XERROR("Blockchain's type is error");
    }
    return false;
}

bool CChainOperator::CreateAddr(ChainType type, STExchangeAddrInfo *pExchangeAddrInfoList){
    std::set<STExchangeAddrInfo>::iterator it;
    switch(type) {
       case ChainType::TYPE_BITCOIN:
           return bitcoinapi_->GetNewAddress(pExchangeAddrInfoList);
       case ChainType::TYPE_ETHEREUM:
           return ethapi_->NewAccount(*pExchangeAddrInfoList);
       default:
           XERROR("Blockchain's type is error");
    }

    return false;
}
