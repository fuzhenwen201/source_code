#ifndef CONST_H
#define CONST_H
//地址长度
#define ADDRESSLEN 128

//私钥长度
#define PRIVATEKEYLEN 128

//区块链名字的长度
#define CHAINNAMELEN 128

//TXID长度
#define	TXIDLEN	256

// blockchain url
#define BLOCKCHAINURLLEN 100

// blockchain username 
#define BLOCKCHAINUSERLEN 100

// blockchain pwd 
#define BLOCKCHAINPWDLEN 128 

// blockchain label or account
#define BLOCKLABELLEN 100 


#define BTCLABELLEN 128

//货币长度
#define SYMBOLLEN 64

#include <string.h>
#include <list>
#include <stdio.h>
#include <map>
#include "xlogger.h"
#include "uint128_t.h"
using namespace std;
// 入金检测 用的 数据
typedef struct __STDepositAddr
{
	uint32_t nId;
	uint32_t nUserId;
	uint32_t nCoinId;
	uint64_t nScanStartBlock;				//扫描的其实块,传入, 同时也要传出, 然后更新到Blockchian_info表中
	uint32_t nConfirmBlkQty;
	
	char	sChainName[CHAINNAMELEN]; 	
	char 	sDestAddr[ADDRESSLEN];
	char 	sSrcAddr[ADDRESSLEN];			//入金源地址,链库回填
	char	sTxID[TXIDLEN];					//入金交易id,链库回填
	char    sContractAddr[ADDRESSLEN];      //合约地址
	char    sTokenValue[ADDRESSLEN];        //代币数
	double	fAmount;						//入金数目,链库回填 
	uint32_t nChainType;						//区块链类型
	
	char 	sSymbol[SYMBOLLEN];				//由区块链库填写,传出来
	__STDepositAddr &  operator=(__STDepositAddr &thr){
		nId = thr.nId;
		nUserId = thr.nUserId;
		nCoinId = thr.nCoinId;
		nScanStartBlock = thr.nScanStartBlock;
		nConfirmBlkQty = thr.nConfirmBlkQty ;
		nChainType = thr.nChainType;

		strncpy(sChainName, thr.sChainName, CHAINNAMELEN - 1);
		strncpy(sSrcAddr, thr.sSrcAddr, ADDRESSLEN - 1);
		strncpy(sDestAddr, thr.sDestAddr, ADDRESSLEN - 1);
		strncpy(sContractAddr, thr.sContractAddr, ADDRESSLEN - 1);
		strncpy(sTxID, thr.sTxID, TXIDLEN - 1);
		strncpy(sSymbol, thr.sSymbol, SYMBOLLEN);
		
		return *this;
	}

}STDepositAddr; 

// 转账的交易数据
typedef struct __STTradeData
{
	uint32_t nId;
	uint32_t nUserId;
	double 	fAmount;
    double  fGas;
    uint64_t nGasPrice ;
	uint32_t nChainType;
	char	sChainName[CHAINNAMELEN];
	char 	sSrcAddr[ADDRESSLEN];
	char 	sDestAddr[ADDRESSLEN];
	char 	sPriKey[PRIVATEKEYLEN];
	char	sTxID[TXIDLEN];
	char    sContractAddr[ADDRESSLEN];      //合约地址
	uint32_t uDecimals;
    uint32_t uCoinId;
	int		isSucess;					//1 sucess, 0 failure
	uint32_t nDirect;
	__STTradeData &  operator=(__STTradeData &thr) {
		nId = thr.nId;
		nUserId = thr.nUserId;
		fAmount = thr.fAmount;
        fGas = thr.fGas;
        nGasPrice = thr.nGasPrice;
		strncpy(sChainName, thr.sChainName,CHAINNAMELEN-1);
		strncpy(sSrcAddr, thr.sSrcAddr,ADDRESSLEN-1);
		strncpy(sDestAddr,thr.sDestAddr,ADDRESSLEN-1);
		strncpy(sPriKey, thr.sPriKey,PRIVATEKEYLEN-1);
		strncpy(sTxID, thr.sTxID,TXIDLEN-1);
		strncpy(sContractAddr, thr.sContractAddr, ADDRESSLEN - 1);
		isSucess = thr.isSucess ;
		nChainType = thr.nChainType;
		nDirect = thr.nDirect ;
		
		return *this;
	}

}STTradeData; 

// 区块确认的数据
typedef struct __STConfirmData
{
	uint32_t nId;
	uint32_t nUserId;
	uint32_t nCoinId;
	double 	 fAmount;
	double   nGas;
	uint64_t nGasPrice;
	double   nGasUsed;
	uint32_t nType;
	uint32_t nChainType;			//1:BTC CHAIN, 2:ETH CHAIN
	uint64_t nChainConfirmBlkQty;		//本区块链的确认数
	uint64_t nConfirmBlkQty;				//当前确认数
	char	 sTxId[TXIDLEN];
	uint64_t nBlockNumber;   // blocknumber of tx
	uint32_t status;
	/*
	0x9001: PoolAddr  	UserWallet 
	0x9002: UserWallet  	ExchangeAddr
	0x9003: ExchangeAddr  PoolAddr
	*/
	// new add on 2018/03/05
	char sSrcAddr[ADDRESSLEN];
	char sDestAddr[ADDRESSLEN];
	
	__STConfirmData &  operator=(__STConfirmData &thr){
		nId = thr.nId;
		nUserId = thr.nUserId;
		nCoinId = thr.nCoinId;
		fAmount = thr.fAmount;
		nGasUsed = thr.nGasUsed;
		nType = thr.nType;
		nChainType = thr.nChainType ;
		nChainConfirmBlkQty = thr.nChainConfirmBlkQty;
		nConfirmBlkQty = thr.nConfirmBlkQty;
		strncpy(sTxId, thr.sTxId,TXIDLEN-1);
		strncpy(sSrcAddr,thr.sSrcAddr,ADDRESSLEN-1);
		strncpy(sDestAddr,thr.sDestAddr,ADDRESSLEN-1);
		return *this;
	}
}STConfirmData; 

// 创建用户exchange地址
typedef struct __STExchangeAddrInfo
{
	uint32_t nUserId;
	char	 sUserExchangeAddr[ADDRESSLEN];	//  创建成功时的区块链地址
	char 	 sPriKey[PRIVATEKEYLEN];
	uint32_t nChainType;			//1:BTC CHAIN, 2:ETH CHAIN
	//uint32_t nType;							//0, 创建成功,其他失败
	uint32_t nBlockChainID;
	char 	 sBTCLabel[BTCLABELLEN];
	__STExchangeAddrInfo &  operator=(__STExchangeAddrInfo &thr){
		nUserId = thr.nUserId;
		nChainType = thr.nChainType;
		nBlockChainID = thr.nBlockChainID;
		strncpy(sUserExchangeAddr, thr.sUserExchangeAddr,ADDRESSLEN-1);
		strncpy(sPriKey, thr.sPriKey,PRIVATEKEYLEN-1);
		strncpy(sBTCLabel, thr.sBTCLabel,BTCLABELLEN-1);
		
		return *this;
	}

}STExchangeAddrInfo;

enum ChainType{
	TYPE_BITCOIN = 1,
	TYPE_ETHEREUM = 2
};

typedef struct _CoinInfo{
	std::string sContractAddr;
	uint32_t Decimals;
}CoinInfo;
// BlockChain Init information 
typedef struct __BlockChainInfo
{
	 char sChainURL[BLOCKCHAINURLLEN];
	 char sChainUserName[BLOCKCHAINUSERLEN];
	 char sChainPWD[BLOCKCHAINPWDLEN];
	 ChainType type;
	 uint64_t sPosNum;
	 char sChainLabel[BLOCKLABELLEN];
	
	__BlockChainInfo &  operator=(__BlockChainInfo &thr){
		strncpy(sChainURL, thr.sChainURL,BLOCKCHAINURLLEN-1);
		strncpy(sChainUserName, thr.sChainUserName,BLOCKCHAINUSERLEN-1);
		strncpy(sChainPWD, thr.sChainPWD,BLOCKCHAINPWDLEN-1);
		strncpy(sChainLabel,thr.sChainLabel,BLOCKLABELLEN-1) ;
		type = thr.type;
		sPosNum = thr.sPosNum;
		return *this;
	}

}BlockChainInfo;

int hex_char_value(char c);
uint128_t hex2decimal(std::string& szHex);
#endif