#include "EthJsonRPC.h"
#include "ManageETHAccount.h"
#include <unistd.h>
CManagementETH::CManagementETH(){

} 

CManagementETH::~CManagementETH(){

}

bool CManagementETH::InitETHInfo(ETHOperJsonRPC* oper ,int Height){
     // sync Address from DB
     if(!InitETHAccount()){
         printf("ETH Account Init failed ");
         return false;
     }
        
     operETH_ = oper ;
     isStop_ = false;
     if(Height != 0){
         blockHeight_ = Height;
     }
     else {
         blockHeight_ = operETH_->GetETHBlockNumber();
     }

}

bool CManagementETH::NewETHAccount(std::string& address)
{
    EThAccount account;
    operETH_->NewAccount(account);
    //  insert account into DB 
    //
    std::unique_lock<std::mutex> lock(this->addressMutex_);
    addressList_.insert(account.Adress_);
    address =  account.Adress_;
     return true;
}

bool CManagementETH::AddETHAccount(std::string& address)
{
    
    std::unique_lock<std::mutex> lock(this->addressMutex_);
    addressList_.insert(address);
     
     return true;
}

void CManagementETH::RunETH()
{
    while (!isStop_)
    {
        CheckTransactionInfo();
        int currentHeight = operETH_->GetETHBlockNumber();
        for (int i = blockHeight_+1; i <= 2424499; i++)
        {
            SearchTransactionInfo(i);
        }
        blockHeight_ = currentHeight;
        sleep(10);
    }
}

 void CManagementETH::StopETH()
 {
     isStop_ = true;
 }
bool CManagementETH::InitETHAccount()
{
    // sync Address from DB
    return true;
 }

 void CManagementETH::SearchTransactionInfo(int block){
     std::string result;
     if (!operETH_->GetBlockbyNumber(block, result))
     {
         printf("Get Block by Number failed\n");
         return;
     }
     rapidjson::Document d;
     size_t pos = result.find('{');
     d.Parse(result.c_str() + pos);
     if (d.HasParseError() || !d.IsObject())
     {
         printf("parser json error|event=%s", result.c_str());
         return ;
     }
     /*
     rapidjson::Value::ConstMemberIterator iter;
  if ( ((iter = d.FindMember("transactions")) == d.MemberEnd()) || iter->value.IsString())
	{
		printf("field no exit or type error");
		return ;
	} */
     
    //rapidjson::Value& contents = iter->value ;
    rapidjson::Value &test = d["result"];

     rapidjson::Value &contents = test["transactions"];
     printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
     if (contents.IsArray())
     {
         for (size_t i = 0; i < contents.Size(); ++i)
         {
             rapidjson::Value &v = contents[i];
             assert(v.IsObject());
             if (!v.HasMember("to") || !v["to"].IsString())
             {
                 printf("field no exit or type error");
                 continue;
             }
             
             std::string to = v["to"].GetString();
             std::set<std::string>::iterator it = addressList_.find(to);
             if (it == addressList_.end())
             {
                 continue;
             }
             // Record User transaction
            TransactionInfo * info = new TransactionInfo; 
            info->toAddress_ = to ;
             if (!v.HasMember("from") || !v["from"].IsString())
             {
                 printf("field no exit or type error");
                 delete info;
                 continue;
             }
             info->fromAddress_ = v["from"].GetString();
             
             if (!v.HasMember("hash") || !v["hash"].IsString())
             {
                 printf("field no exit or type error");
                 delete info;
                 continue;
             }
             info->txHash_ = v["hash"].GetString();

            if (!v.HasMember("blockNumber") || !v["blockNumber"].IsString())
             {
                 printf("field no exit or type error");
                 delete info ;
                 continue;
             }
             info->blockHeight_ = strtol(v["blockNumber"].GetString(),NULL,16);

            if (!v.HasMember("value") || !v["value"].IsString())
             {
                 printf("field no exit or type error");
                 delete info;
                 continue;
             }
             info->txValue_ = v["value"].GetString();

            if (!v.HasMember("value") || !v["value"].IsString())
             {
                 printf("field no exit or type error");
                 delete info;
                 continue;
             }
             info->txValue_ = v["value"].GetString();
            
            if (!v.HasMember("gas") || !v["gas"].IsString())
             {
                 printf("field no exit or type error");
                 delete info;
                 continue;
             }
             info->txGas_ = v["gas"].GetString();

            if (!v.HasMember("gasPrice") || !v["gasPrice"].IsString())
             {
                 printf("field no exit or type error");
                 delete info ;
                 continue;
             }
             info->txGasPrice_ = v["gasPrice"].GetString() ;

             // insert map 
             txTmpMap_.insert(std::pair<std::string,TransactionInfo*>(info->txHash_,info));

         }
     }
 }

 bool CManagementETH::CheckTransactionInfo(){
     // get currentblockheight
    long Height =  operETH_->GetETHBlockNumber();
     std::map<std::string,TransactionInfo*>::iterator it = txTmpMap_.begin();
     for(;it != txTmpMap_.end();)
     {
         if( (it->second->blockHeight_ + BLANK) <= Height)
        {
            std::string hash = it->first ;
            if(operETH_->CheckingTransactionState(*(it->second)) == 1)
            {
                // record balance and tx history
                printf("trade success++++++++++++\n");
                delete (it->second);
                txTmpMap_.erase(it++);
                
                continue;
            }
        }
        it++;
     }
    return true;
 }

 bool CManagementETH::WithdrawalETH(std::string& srcAddress, std::string& destAddress, std::string &value)
 {
     TransactionInfo *info = new TransactionInfo;
     memset(info,0,sizeof(info));
     info->fromAddress_ = srcAddress;
     info->toAddress_ = destAddress;
     info->txGas_ = gas_;
     info->txGasPrice_ = gasPrice_;
     info->txValue_ = value;
     info->blockHeight_ = blockHeight_;
     if(operETH_->SendTransaction(*info,true))
     {
        txTmpMap_.insert(std::pair<std::string,TransactionInfo*>(info->txHash_,info));
     }
     return true;
     
 }

 bool CManagementETH::AggregateFunds(std::string& srcAddress, std::string& destAddress)
 {
 }

  
