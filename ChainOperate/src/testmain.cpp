#include "EthJsonRPC.h"
#include "ManageETHAccount.h"
#include "BitCoinJsonRPC.h"
#include "unistd.h"
int main(int argc,char *argv[])
{
    /*
    ETHOperJsonRPC eth ;
    eth.SetURL("119.23.108.120:8545");
    CManagementETH *pManageETH = new CManagementETH();

    long height = 2424492;
    std::string mainAdress = "0xd31b83e218a1169e4ac88d409faf4bbca1b3490d" ;
    pManageETH->InitETHInfo(&eth,height);
    pManageETH->AddETHAccount(mainAdress);
    pManageETH->RunETH();*/
    /*EThAccount  src;
    src.ID_ = 67;
    src.Pwd_ = "123";
    EThAccount dest;
    dest.ID_ = 70;
    dest.Pwd_ = "456";
    //eth.NewAccount(src);
    //eth.NewAccount(dest);0xd31b83e218a1169e4ac88d409faf4bbca1b3490d
    src.Adress_ = "0xd31b83e218a1169e4ac88d409faf4bbca1b3490d";
    dest.Adress_ = "0x286c3ea25e92f77d116e237db8298099a37ac681";
   // eth.GetETHBalance(src);
   // eth.UnlockAccount(src);
    //eth.UnlockAccount(dest);
   // std::string value = "0x1";
    //eth.SendTransaction(src,dest,value);
    //printf("result=%s\n",result.c_str());
    //eth.GetBlockbyNumber(2424759);
    //eth.GetETHBlockNumber();
    //std::string hash = "0x0e3c5f74989c6779ca8ebd827efba82c105b5250db6e079ff016735145d967b2";
   // eth.CheckingTransactionState(hash) ; */
    STExchangeAddrInfo *info = new STExchangeAddrInfo();
    //info->nUserId = 123456;
    //info->nUserId = 
    BitCoinAPI *bitcoin = new BitCoinAPI("47.52.188.215:23114","chain123","chain123456789") ;
    //bitcoin->GetNewAddress(info);
    std::string address = "mnS8kqYZ53kjL2Zd2Cit5kLrqUd1Uw1jJc";
    //std::string pwd = "cTDmWuGFEt69rZhWvYtKevgT6qYsaoD72qV3qt6tWftF5JdS722q";
    //std::string passphrase = "HELLOcTDmWuGFEt69rZhWvYtKevgT6qYBITCOIN" ;
   // bitcoin->GetAddressesbyAccount(info);
    //strcpy(info->sUserExchangeAddr,address.c_str());
    //strcpy(info->sPriKey,pwd.c_str());
    //bitcoin->ShowPrivatekey(info);
    //
    //bitcoin->EncryptWallet(passphrase.c_str());
    //sleep(5);
    //bitcoin->UnLockWallet(passphrase.c_str(),100);
    long num = bitcoin->GetBlockNum();
    std::map<std::string,STDepositAddr> DespositMap ;
     
    bitcoin->ListTransactions(DespositMap,"123456",2);
    STConfirmData *pData = new STConfirmData;
    strcpy(pData->sTxId,"5e37b83e83d6354e656754fe44677beb323a4f935d12c90a91c182d703696ef7");
    //char testTxid = "9081b3bdbe92c7d214bd51551318103ae1bff51d3ef93d261aaca0da6fe40e9e";
    bitcoin->GetBlockChainInfo();
    //bitcoin->GetTransaction(pData);
    sleep(5);
    //bitcoin->LockWallet();
    //bitcoin->GetBlockTemplate();
    
    return 0;
}
 
