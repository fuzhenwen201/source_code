/*******************************************************************************************

*Department:	Exchange team

*Decrible:  	Public module
				
*Auhor:			Savin

*Createdtime:	2018-03-01

*ModifyTime:	

********************************************************************************************/

#include <signal.h>
#include "Public.h"
#include "ConfigFileReader.h"

ConnectionPool* g_connPool = nullptr;

STConfig g_stConfig;

//只要一个模块出问题,其他的模块处理完当前事务也退出
int g_nExit = 1;

int g_nDespositProcess = 1;
int g_nTransferCoinProcess = 1 ;
int g_nBlockConfirmProcess = 1;
int g_nCreateUserExchangeAddr = 1;

TNode *pTnode = nullptr ;

void InitLogSys()
{
    (*XLogger::GetObject("OnezFixMarket")).StdErr(g_stConfig.onlyscreenshowlog).Colour(true).Dir(g_stConfig.logdir).BuffDelay(1);
    XINFO("InitLogSys ....");
}

int InitDatabase()
{
	g_connPool = ConnectionPool::GetInstance(g_stConfig.sIp.c_str(),g_stConfig.sUser.c_str(),g_stConfig.sPwd.c_str(),g_stConfig.nPort,g_stConfig.nMaxconn);
	
	if(nullptr == g_connPool)return -1;
	
	return 0;
}

int ReadPublicConfig(CConfigFileReader &config)
{
	char *buf = nullptr;

    if (nullptr == (buf = config.GetConfigName("CreateAddr_Chkitime")))
    {
        printf("read RabbitMQInfo->CreateAddr_Chkitime error \n");
        return -1;
    }
    g_stConfig.nCreateAddr_Chkitime = atoi(buf);
	
	if (nullptr == (buf = config.GetConfigName("Deposit_Chkitime")))
    {
        printf("read RabbitMQInfo->Deposit_Chkitime error \n");
        return -1;
    }
    g_stConfig.nDeposit_Chkitime = atoi(buf);
	
	if (nullptr == (buf = config.GetConfigName("Confirm_Chkitime")))
    {
        printf("read RabbitMQInfo->nConfirm_Chkitime error \n");
        return -1;
    }
    g_stConfig.nConfirm_Chkitime = atoi(buf);
	
	if (nullptr == (buf = config.GetConfigName("Trade_Chkitime")))
    {
        printf("read RabbitMQInfo->Trade_Chkitime error \n");
        return -1;
    }
    g_stConfig.nTrade_Chkitime = atoi(buf);
	
	if (nullptr == (buf = config.GetConfigName("logdir")))
    {
        printf("read RabbitMQInfo->logdir error \n");
        return -1;
    }
    g_stConfig.logdir = buf;

     if (nullptr == (buf = config.GetConfigName("onlyscreenshowlog")))
     {
         printf("read onlyscreenshowlog error \n");
         return -1;
     }
	g_stConfig.onlyscreenshowlog = 1 == atoi(buf);

	return 0;
}

int ReadMySQLConfig(CConfigFileReader &config)
{
	char *buf = nullptr;
	if (nullptr == (buf = config.GetConfigName("DB_Ip")))
    {
        printf("read RabbitMQInfo->DB_Ip error \n");
        return -1;
    }
    g_stConfig.sIp = buf;
	
	if (nullptr == (buf = config.GetConfigName("DB_User")))
    {
        printf("read RabbitMQInfo->DB_User error \n");
        return -1;
    }
    g_stConfig.sUser = buf;
	
	if (nullptr == (buf = config.GetConfigName("DB_Pwd")))
    {
        printf("read RabbitMQInfo->DB_Pwd error \n");
        return -1;
    }
    g_stConfig.sPwd = buf;
	
	if (nullptr == (buf = config.GetConfigName("DB_Port")))
    {
        printf("read RabbitMQInfo->DB_Port error \n");
        return -1;
    }
    g_stConfig.nPort = atoi(buf);
	
	if (nullptr == (buf = config.GetConfigName("DB_Maxconn")))
    {
        printf("read RabbitMQInfo->DB_Maxconn error \n");
        return -1;
    }
    g_stConfig.nMaxconn = atoi(buf);
	
	if (nullptr == (buf = config.GetConfigName("DB_Name")))
    {
        printf("read RabbitMQInfo->DB_Name error \n");
        return -1;
    }
    g_stConfig.sDBName = buf;
	
	return 0;
}

int ReadMQInfoConfig(CConfigFileReader &config)
{
	char *buf = nullptr;
	if (nullptr == (buf = config.GetConfigName("mq_vhost_token")))
    {
        printf("read RabbitMQInfo->mq_vhost_token error \n");
        return -1;
    }
    g_stConfig.mq_vhost_token = buf;

	if (nullptr == (buf = config.GetConfigName("mq_exchange_token")))
    {
        printf("read RabbitMQInfo->mq_exchange_token error \n");
        return -1;
    }
    g_stConfig.mq_exchange_token = buf;

	if (nullptr == (buf = config.GetConfigName("mq_address_token")))
    {
        printf("read RabbitMQInfo->mq_address_token error \n");
        return -1;
    }
    g_stConfig.mq_address_token = buf;

	if (nullptr == (buf = config.GetConfigName("mq_port_token")))
    {
        printf("read RabbitMQInfo->mq_port_token error \n");
        return -1;
    }
    g_stConfig.mq_port_token = atoi(buf);

	if (nullptr == (buf = config.GetConfigName("mq_user_token")))
    {
        printf("read RabbitMQInfo->mq_user_token error \n");
        return -1;
    }
    g_stConfig.mq_user_token = buf;

	if (nullptr == (buf = config.GetConfigName("mq_password_token")))
    {
        printf("read RabbitMQInfo->mq_password_token error \n");
        return -1;
    }
    g_stConfig.mq_password_token = buf;

	if (nullptr == (buf = config.GetConfigName("mq_routingkey_token")))
    {
        printf("read RabbitMQInfo->mq_routingkey_token error \n");
        return -1;
    }
    g_stConfig.mq_routingkey_token = buf;	
	
	return 0;
}

int ReadConfig()
{
	struct sigaction sa = {};
    sa.sa_handler = SIG_IGN;
    sigaction( SIGPIPE, &sa, nullptr );

    CConfigFileReader config;

    if( -1 == config.LoadFile(SERVICECONFIG)  )
    {
        XERROR("open config file: %s,fail:%s",SERVICECONFIG, strerror(errno));
        return -1;
    }

    if(-1==ReadPublicConfig(config))return -1;

    if(-1==ReadMySQLConfig(config))return -1;
	
	if(-1==ReadMQInfoConfig(config))return -1;

    return 0;
}

int ProcessLastFail_ConfirmTask()
{

	FILE *fp = fopen("./blockconfirm_err.txt","r");
	if(nullptr == fp)
	{
		XERROR("ProcessLastFailTask open blockconfirm_err.txt is error:%s",strerror(errno));
		return -1;
	}
	
	list<STBlockConfirmFailData>  confirmData = {};

	while (true) {
		STBlockConfirmFailData tmpconfirmdata = {};
		if (1 != fread(&tmpconfirmdata, sizeof(STBlockConfirmFailData), 1, fp))
			break;
		confirmData.push_back(tmpconfirmdata);
	}
    fclose(fp);

	if(confirmData.empty()){
		return 0;
	}

	fp = fopen("./blockconfirm_err.txt","w+");
	if(nullptr == fp)
	{
		XERROR("Create/open blockconfirm_err.txt is error:%s",strerror(errno));
		return -1;
	}

	char sSQL[SQLLEN] = {};

	MySqlDB db;
	db.Open();
	db.SelectDB(g_stConfig.sDBName.c_str());

	int isFail = 0;
	list<STBlockConfirmFailData>::iterator itr;
	for (itr = confirmData.begin(); itr != confirmData.end(); ++itr){
		STBlockConfirmFailData &data = *itr;
		if( 1 == isFail) // 把余下的写入到文件
		{
			fwrite(&data, sizeof(data), 1, fp);
			continue;
		}

		switch(data.nType)
		{
			case 3:
			{
				memset(sSQL, 0, SQLLEN);
				snprintf(sSQL, SQLLEN - 1, "INSERT INTO WITHDRAWDEPOSIT_ECORD_HISTROY(UserID,CoinID,Amount,TYPE,SrcAddress,DestAddress,"
						"ApplyTime,CSConfirmTime,ConfirmBlkQty,BlkConfirmTime,Txid,NetFee,GasPrice) (SELECT UserID,CoinID,Amount,TYPE,SrcAddress,"
						"DestAddress,ApplyTime,CSConfirmTime,ConfirmBlkQty,BlkConfirmTime,Txid,NetFee,GasPrice FROM WITHDRAWDEPOSIT_CONFIRMRECORD WHERE ID = %u);", data.nId);
				db.ExecSql(sSQL);
				if(db.IsError()){
					XERROR("BlockConfirm insert error:%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sSQL);
					isFail = 1;
					fwrite(&data, sizeof(data), 1, fp);
					break;
				}
				data.nType = data.nType - 1;
			}
			case 2:
			{
				if(EXCHANGEADDR_TO_POOLADDR == data.nDirect) {
					memset(sSQL, 0, SQLLEN);
					snprintf(sSQL, SQLLEN - 1,
							 "insert into WITHDRAW_APPLY (NetFee,GasPrice,Type,ApplyTime,UserID,CoinID,Amount,SrcAddress,DestAddress,PrivateKey) values "
									 "(0.000315,15000000000,%d,%ld, select w.UserID,w.CoinID,w.Amount,w.DestAddress,e.Address, e.PrivateKey from WITHDRAWDEPOSIT_CONFIRMRECORD w "
									 "join EXCHANGE_ASSETSPOOL e on w.CoinID = e.CoinID where w.ID = %u);",EXCHANGEADDR_TO_POOLADDR, time(nullptr), data.nId);
					db.ExecSql(sSQL);
					if (db.IsError()) {
						XERROR("BlockConfirm insert error:%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sSQL);
						isFail = 1;
						fwrite(&data, sizeof(data), 1, fp);
						break;
					}
					data.nType = data.nType - 1;
				}
			}
			case 1:
			{
				memset(sSQL, 0, SQLLEN);
				snprintf(sSQL, SQLLEN - 1, "DELETE FROM WITHDRAWDEPOSIT_CONFIRMRECORD WHERE ID = %u;", data.nId);
				db.ExecSql(sSQL);
				if (db.IsError()) {
					XERROR("BlockConfirm DELETE error:%s,con=%d|sql=", db.GetMySqlErrMsg(), db.IsConnect(), sSQL);
					isFail = 1;
					fwrite(&data, sizeof(data), 1, fp);
				}
				break;
			}
			default:
			{
				XERROR("Unsupported error type.");
				break;
			}
		}
	}
	db.Close();
	fclose(fp);

	return 0;
}

int ProcessLastFail_TransferTask()
{
	char sSQL[SQLLEN] = {0};
	
	list<TransferCoinFailData> transferCoinlist;

	FILE *fp = fopen("transfercoin_err.txt", "r");
	if(nullptr == fp)
	{
		XERROR("Create/open transfercoin_err.txt is error:%s", strerror(errno));
		return -1;
	}

	while(true)
	{
		TransferCoinFailData transferData = {};

		if(1 != fread(&transferData, sizeof(transferData), 1, fp))
			break;

		transferCoinlist.push_back(transferData);
	}
	fclose(fp);	
	
	if(transferCoinlist.empty()) {
        return 0;
    }
	
	fp = fopen("transfercoin_err.txt","w+");
	if(nullptr == fp)
	{
		XERROR("Create/open transfercoin_err.txt is error:%s",strerror(errno));
		return -1;
	}
	
	MySqlDB db;
	db.Open();
	db.SelectDB(g_stConfig.sDBName.c_str());
	
	int isFail = 0;
	list<TransferCoinFailData>::iterator itr;
	for (itr = transferCoinlist.begin(); itr != transferCoinlist.end(); ++itr)
	{
		TransferCoinFailData &data = *itr;
		if( 1 == isFail) // 把余下的写入到文件
		{
			fwrite(&data,sizeof(data),1,fp);
			continue;
		}
		switch(data.nType)
		{
			case 2:
			{
				memset(sSQL, 0, SQLLEN);
				snprintf(sSQL, SQLLEN - 1, "INSERT INTO WITHDRAWDEPOSIT_CONFIRMRECORD(UserID,CoinID,Type,Amount,SrcAddress,DestAddress,"
						"ApplyTime,CSConfirmTime,NetFee,GasPrice,ConfirmBlkQty,BlkConfirmTime,Txid) values "
						"(SELECT UserID,CoinID,Type,Amount,SrcAddress,DestAddress,ApplyTime,CSConfirmTime,NetFee,GasPrice FROM "
						"WITHDRAW_APPLY where ID = %u,0,0,%s)",data.nId, data.sTxId);

				db.ExecSql(sSQL);
				if (db.IsError()) 
				{
					XERROR("ChainWithdraw insert error:%s,con=%d|sql=", db.GetMySqlErrMsg(), db.IsConnect(), sSQL);
					isFail = 1;
					fwrite(&data,sizeof(data),1,fp);
					break;
				}
				data.nType = data.nType - 1;
			}
			case 1:
			{
				memset(sSQL,0,SQLLEN) ;
				snprintf(sSQL,SQLLEN-1,"DELETE FROM WITHDRAW_APPLY WHERE ID = %u",data.nId);
				db.ExecSql(sSQL);
				if (db.IsError()) 
				{
					XERROR("ChainWithdraw DELETE error:%s,con=%d|sql=", db.GetMySqlErrMsg(), db.IsConnect(), sSQL);
					isFail = 1;
					fwrite(&data,sizeof(data),1,fp);
				}
				break;
			}
			default:
			{
				XERROR("Unsupported error type.");
				break;
			}
		}
	}
	db.Close();
	fclose(fp);

	return 0;
}

int ProcessLastFailTask()
{
	if(-1 == ProcessLastFail_ConfirmTask())return -1;
	
	if(-1 == ProcessLastFail_TransferTask())return -1;
	
	return 0;
}

size_t GetDataListFromDB(MySqlDB &db,MySqlRecordSet &rs,const char *pSQL, const char *pModuleName)
{
	rs = db.QuerySql(pSQL);
	if (db.IsError()) 
	{
		XERROR("%s: QuerySql:%s,con=%d|sql=%s",pModuleName ,db.GetMySqlErrMsg(), db.IsConnect(), pSQL);
		db.Close();
		g_nExit = 0;
		return 0;
	}
	
	size_t rows = rs.GetRows();

	return rows;
}

int CheckExit()
{
	while(1)
	{
		if(0 == g_nExit)
		{
			if( 0 == g_nDespositProcess && 0 == g_nTransferCoinProcess &&
				0 == g_nBlockConfirmProcess && 0 == g_nCreateUserExchangeAddr )
			{
				XERROR("All process module exit , pls check !!!\n");
				sleep(10);
				break;
			}
		}
		
		sleep(10);
	}
	
	return 0;
}











