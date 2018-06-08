/*******************************************************************************************

*Department:	Exchange team

*Decrible:  	withdraw module
				1.withdraw  from assertpool address to user app address	
				2.withdraw  from exchange address to assertpool address
				
*Auhor:			Savin

*Createdtime:	2018-02-26

*ModifyTime:	


********************************************************************************************/

#include "Public.h" 
#include "mysqlop.h"

int ProcessTradeResult(MySqlDB &db,char *sSQL,FILE *fp,STTradeData &data)
{
	uint32_t isError = 0;
	
	// 1. 插入出入金确认表中
	memset(sSQL, 0, SQLLEN) ;
    snprintf(sSQL, SQLLEN - 1, "INSERT INTO WITHDRAWDEPOSIT_CONFIRMRECORD(UserID,CoinID,TYPE,Amount,SrcAddress,DestAddress,ApplyTime,CSConfirmTime,"
            "NetFee,GasPrice,ConfirmBlkQty,BlkConfirmTime,Txid) (SELECT UserID,CoinID,TYPE,Amount,SrcAddress,DestAddress,"
            "ApplyTime,CSConfirmTime,NetFee,GasPrice,0,0,\"%s\" FROM  WITHDRAW_APPLY WHERE ID = %u)",data.sTxID, data.nId);

	db.ExecSql(sSQL);
	if (db.IsError()) 
	{
		XERROR("ChainWithdraw insert error:%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sSQL);
		g_nExit = 0;
		isError = 2;
	}

    if(isError == 0) {
        // 2. 删除出金申请表中 已经出金的记录
        memset(sSQL, 0, SQLLEN);
        snprintf(sSQL, SQLLEN - 1, "DELETE FROM WITHDRAW_APPLY WHERE ID = %u", data.nId);

        db.ExecSql(sSQL);
        if (db.IsError()) {
            XERROR("ChainWithdraw DELETE error:%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sSQL);
            g_nExit = 0;
            isError = 1;
        }
    }

	if(0 != isError)
	{
		TransferCoinFailData writeData = {};
		writeData.nId = data.nId;
		writeData.nDirect = data.nDirect;
		writeData.nType = isError;
		strncpy(writeData.sTxId, data.sTxID, TXIDLEN - 1);
		fwrite(&writeData, sizeof(writeData), 1, fp);
	}
			
	return 0;
}

void * TransferCoin(void *argv)
{
	char sSQL[SQLLEN]={};

	snprintf(sSQL, SQLLEN - 1,"select ChainType, ChainURL, ChainUserName, ChainPWD from BLOCKCHAIN_INFO;");
	MySqlDB db;
	db.Open();
	db.SelectDB(g_stConfig.sDBName.c_str());
	
	MySqlRecordSet rs = {};
	size_t rows = GetDataListFromDB(db, rs, sSQL, "BlockConfirm");
	if(rows <= 0)
	{
		XERROR("Get BlockChain info failed");
		g_nTransferCoinProcess = 0;
		db.Close();
		return nullptr;
	}
	std::list<BlockChainInfo> infoList = {};

	for(size_t i =0 ; i < rows; i++)
	{
		BlockChainInfo info = {};
		info.type = (ChainType)atoi(rs.GetFieldByID(i, 0).c_str());
		strncpy(info.sChainURL, rs.GetFieldByID(i, 1).c_str(), BLOCKCHAINURLLEN - 1);
		strncpy(info.sChainUserName, rs.GetFieldByID(i, 2).c_str(), BLOCKCHAINUSERLEN - 1);
		strncpy(info.sChainPWD, rs.GetFieldByID(i, 3).c_str(), BLOCKCHAINPWDLEN - 1);
		infoList.push_back(info);
	}
	
	CChainOperator ChainOperator;
	ChainOperator.Init(infoList);
	while(1 == g_nExit)
	{
		FILE *fp = fopen("transfercoin_err.txt","w+");
		if(nullptr == fp)
		{
			XERROR("Create/open transfercoin_err.txt is error:%s",strerror(errno));
			sleep(5);
			continue;
		}

		// 出金
		/*
		条件说明:
		(w.amount < c.CSCheckMaxQty )  出金金额 不需审批的
		(w.IsCSConfirm = 1)   审批过的, exchange --> user 
		(w.Type = 0x9003)   userexchangeaddr --> pool的 出金
		 (w.Type = 0x9004)  pool --> userexchangeaddr 充值eth以便可以转出Token
		*/
		memset(sSQL, 0, SQLLEN );
		snprintf(sSQL, SQLLEN - 1, "SELECT w.ID,w.UserID,b.ChainType,w.Amount,w.SrcAddress,w.DestAddress,w.PrivateKey,w.Type,w.NetFee,w.GasPrice,c.ContractAddr,c.Decimals,w.CoinID "
				"FROM WITHDRAW_APPLY w ,COIN c, BLOCKCHAIN_INFO b  WHERE w.CoinID = c.ID AND c.BlockChainID = b.ID "
				"AND ( (w.Amount < c.CSCheckMaxQty ) OR (w.IsCSConfirm = 1) OR (w.Type = 0x9003) or (w.Type = 0x9004));");
		
		rs = {};
		rows  = GetDataListFromDB(db, rs, sSQL, "BlockConfirm");
		if(rows <= 0)
		{
			sleep(5);
            fclose(fp);
			continue;
		}

		list<STTradeData> tradeDataList = {};
		
		for(size_t i=0; i < rows; i++)
		{
			STTradeData data = {};
			data.nId = static_cast<uint32_t>(atol(rs.GetFieldByID(i, 0).c_str()));
			data.nUserId = static_cast<uint32_t>(atol(rs.GetFieldByID(i, 1).c_str()));
			data.nChainType = static_cast<uint32_t>(atoi(rs.GetFieldByID(i, 2).c_str()));
			data.fAmount = strtod(rs.GetFieldByID(i, 3).c_str(), nullptr);
			snprintf(data.sSrcAddr, ADDRESSLEN - 1, "%s", rs.GetFieldByID(i, 4).c_str());
			snprintf(data.sDestAddr, ADDRESSLEN - 1, "%s", rs.GetFieldByID(i, 5).c_str());

			if(TYPE_BITCOIN == (ChainType)data.nChainType )
			{
				strncpy(data.sPriKey,"HELLOcTDmWuGFEt69rZhWvYtKevgT6qYBITCOIN", PRIVATEKEYLEN-1 );
			}
			else
			{
				snprintf(data.sPriKey, PRIVATEKEYLEN - 1, "%s", rs.GetFieldByID(i, 6).c_str() );
			}
			data.nDirect = static_cast<uint32_t>(atol(rs.GetFieldByID(i, 7).c_str()));
            data.fGas = strtod(rs.GetFieldByID(i, 8).c_str(), nullptr);
            data.nGasPrice = strtoul(rs.GetFieldByID(i, 9).c_str(), nullptr, 10);
            const auto &contractAddr = rs.GetFieldByID(i, 10);
            if(!contractAddr.empty()) {
                snprintf(data.sContractAddr, ADDRESSLEN - 1, "%s", contractAddr.c_str());
            }
            data.uDecimals = static_cast<uint32_t>(atol(rs.GetFieldByID(i, 11).c_str()));
            data.uCoinId = static_cast<uint32_t>(atol(rs.GetFieldByID(i, 12).c_str()));
			//3. 调用区块链库的交易函数,进行转账
            if(data.uCoinId != 2 && data.uCoinId != 1) {
                std::string srcAddr = data.sSrcAddr;
                std::string strBalance = ChainOperator.GetBlance(srcAddr);
                uint128_t balance = hex2decimal(strBalance);
                if(balance < 630000000000000){
                    XINFO("Article %u application fee is insufficient.",data.nId);
                    continue;
                }
            }

			if(ChainOperator.TransferCoin((ChainType)data.nChainType, &data))
			{
				tradeDataList.push_back(data);
			}
			else
			{
				XERROR("The Transaction is failed and the userid = %u ",data.nUserId);
                continue;
			}
		}
		
	 	// for test
		//sleep(5);

		list<STTradeData>::iterator itr;
		
		for (itr = tradeDataList.begin(); itr != tradeDataList.end(); ++itr)
		{	
			STTradeData &data = *itr;
			if(0 == data.isSucess)
			{
				XERROR("ChainWithdraw trade not sucess: id:%u, userid:%u, ChainType:%u",data.nId, data.nUserId ,data.nChainType);
				continue;
			}

			ProcessTradeResult(db, sSQL, fp, data);
		}
		fclose(fp);
        sleep(5);
		//sleep(g_stConfig.nTrade_Chkitime);
	}
	g_nTransferCoinProcess = 0;
    db.Close();
	return nullptr;
}

int TransferCoinProcess()
{
	pthread_attr_t attr;
	pthread_attr_init (&attr);   
  	pthread_attr_setscope (&attr, PTHREAD_SCOPE_SYSTEM);   
  	pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);  
	
	pthread_t nThreadID;
	
	if (0 != pthread_create(&nThreadID, &attr, TransferCoin, nullptr))
	{
		XERROR("pthread_create TransferCoinProcess error:%s",strerror(errno));
		return -1;
	}
	
	return 0;
}