/*******************************************************************************************

*Department:	Exchange team

*Decrible:  	pubic
				
*Auhor:			Savin

*Createdtime:	2018-02-26

*ModifyTime:	


********************************************************************************************/
#ifndef NEWEXCHANGE_PUBLIC_HEAD
#define NEWEXCHANGE_PUBLIC_HEAD

#include <stdlib.h>
#include <list>
#include <set>
#include <pthread.h>
#include <unistd.h>

#include "mysqlop.h"

#include "tnode.h"
#include "BlockChainOperator.h"
#define SERVICECONFIG "./ChainOperateSrv.cfg"

#define SQLLEN	2048
#define JSONLEN 1024

#define POOLADDR_TO_USERWALLET  0x9001
#define USERWALLET_TO_EXCHANGEADDR 0x9002
#define EXCHANGEADDR_TO_POOLADDR 0x9003
#define POOLADDR_TO_EXCHANGEADDR 0x9004
using namespace std;
using namespace snetwork_xservice_tnode;

typedef struct __STConfig
{
	// 间隔时间
	int nCreateAddr_Chkitime;
	int nDeposit_Chkitime;
	int nConfirm_Chkitime;
	int nTrade_Chkitime;
	string  logdir;
    bool    onlyscreenshowlog;
	
	// MYSQL
	string 	sIp;
	string 	sUser;
	string 	sPwd;
	int  	nPort;
	int  	nMaxconn;
	string	sDBName;
	
	// TNODE
	string mq_vhost_token;
	string mq_exchange_token;
	string mq_address_token;
	int    mq_port_token;
	string mq_user_token;
	string mq_password_token;
	string mq_routingkey_token;

}STConfig;

typedef struct  __STTransferCoinFailData
{
	uint32_t nId;
	char sTxId[TXIDLEN];
	uint32_t nDirect;
	uint32_t nType;
				   
				   
	__STTransferCoinFailData &  operator=(__STTransferCoinFailData &thr)
	{
		nId = thr.nId;
		nDirect = thr.nDirect;
		nType = thr.nType;
		strncpy(sTxId,thr.sTxId,TXIDLEN-1);
		return *this;
	}
	
	
}TransferCoinFailData;

typedef struct  __STBlockConfirmFailData
{
	uint32_t nId;
	uint32_t nUserId;
	uint32_t nCoinId;
	uint32_t nDirect;
	uint32_t nType;
					
}STBlockConfirmFailData;

extern ConnectionPool*  g_connPool ;

extern STConfig g_stConfig;

extern int g_nExit;
extern int g_nDespositProcess ;
extern int g_nTransferCoinProcess ;
extern int g_nBlockConfirmProcess ;
extern int g_nCreateUserExchangeAddr ;

extern TNode *pTnode;

void InitLogSys();

int InitDatabase();

int ReadConfig();
	
int DespositProcess();
	
int TransferCoinProcess();
	
int BlockConfirmProcess();
	
int ChainCreateProcess();

int ProcessLastFailTask();

int ProcessLastFailTask();

size_t GetDataListFromDB(MySqlDB &db,MySqlRecordSet &rs,const char *pSQL, const char *pModuleName);

int CheckExit();

#endif







