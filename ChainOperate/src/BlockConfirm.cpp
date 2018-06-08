/*******************************************************************************************

*Department:	Exchange team

*Decrible:  	Trade block confirm
				
				
*Auhor:			Savin

*Createdtime:	2018-02-27

*ModifyTime:	


********************************************************************************************/

#include "Public.h"

int ProcessConfirmData(MySqlDB &db,STConfirmData &data,char *sJson, char *sSQL, FILE *fp)
{
	// 1. 发消息给manager
	memset(sJson, 0, JSONLEN);
    if(data.nType == 0x9002) {//入金
        snprintf(sJson, JSONLEN - 1,
                 "{\"Tag\":%u,\"UserID\":%u,\"CoinID\":%u,\"SrcAddr\":\"%s\",\"DestAddr\":\"%s\",\"Amount\":%f,\"Netfee\":%f,\"Usedfee\":%f}",
                 data.nType, data.nUserId, data.nCoinId, data.sSrcAddr, data.sDestAddr, data.fAmount, 0.0, 0.0);
    }else{
        snprintf(sJson, JSONLEN - 1,
                 "{\"Tag\":%u,\"UserID\":%u,\"CoinID\":%u,\"SrcAddr\":\"%s\",\"DestAddr\":\"%s\",\"Amount\":%f,\"Netfee\":%f,\"Usedfee\":%f}",
                 data.nType, data.nUserId, data.nCoinId, data.sSrcAddr, data.sDestAddr, data.fAmount, data.nGas, data.nGasUsed);
    }

	if( -1 == pTnode->PublishToMQ(g_stConfig.mq_routingkey_token.c_str(), sJson, strlen(sJson)))
	{
		XERROR("tnode PublishToMQ fail, pls check! [%s]", sJson);
		return -1;
	}

	db.SelectDB(g_stConfig.sDBName.c_str());

	uint32_t isError = 0;
	// 2. 插入到出入确认历史表
	memset(sSQL, 0, SQLLEN) ;
	snprintf(sSQL, SQLLEN - 1, "INSERT INTO WITHDRAWDEPOSIT_ECORD_HISTROY(UserID,CoinID,Amount,TYPE,SrcAddress,DestAddress,"
			"ApplyTime,CSConfirmTime,ConfirmBlkQty,BlkConfirmTime,Txid,NetFee,GasPrice) (SELECT UserID,CoinID,Amount,TYPE,SrcAddress,"
			"DestAddress,ApplyTime,CSConfirmTime,ConfirmBlkQty,BlkConfirmTime,Txid,NetFee,GasPrice FROM WITHDRAWDEPOSIT_CONFIRMRECORD WHERE ID = %u);", data.nId);

	db.ExecSql(sSQL);
	if (db.IsError()) 
	{
		XERROR("BlockConfirm insert error:%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sSQL);
		g_nExit = 0;
		isError = 3;
	}

    if(0 == isError) {
        // 3. 如果这条确认是USERWALLET_TO_EXCHANGEADDR bingq, 还要把这条记录插入出金申请表中, 把这笔钱由UserExchangeAddr转到资金池
        if (USERWALLET_TO_EXCHANGEADDR == data.nType && TYPE_ETHEREUM == (ChainType)data.nChainType) {
            memset(sSQL, 0, SQLLEN);
            //每种代币一个地址
            /*snprintf(sSQL, SQLLEN - 1,
                     "INSERT INTO WITHDRAW_APPLY (NetFee,GasPrice,TYPE,ApplyTime,IsCSConfirm,UserID,CoinID,Amount,SrcAddress,DestAddress,PrivateKey)"
            "(SELECT 0.000315,15000000000,%d,%ld,0, w.UserID,w.CoinID,w.Amount,w.DestAddress,e.Address, d.PrivateKey FROM user_depositaddress d,WITHDRAWDEPOSIT_CONFIRMRECORD w"
            " JOIN EXCHANGE_ASSETSPOOL e ON w.CoinID = e.CoinID WHERE w.ID = %u AND w.UserID = d.UserID AND w.CoinID = d.BlockChainID);",EXCHANGEADDR_TO_POOLADDR, time(nullptr), data.nId);*/

            //基于以太坊的代币共用一个地址
            snprintf(sSQL, SQLLEN - 1, "INSERT INTO WITHDRAW_APPLY (NetFee,GasPrice,TYPE,ApplyTime,IsCSConfirm,UserID,CoinID,Amount,SrcAddress,DestAddress,PrivateKey)"
                    " (SELECT 0.00063,1000000000,%d,%ld,0, w.UserID,w.CoinID,w.Amount,w.DestAddress,e.Address, d.PrivateKey FROM user_depositaddress d,WITHDRAWDEPOSIT_CONFIRMRECORD w,"
                    "EXCHANGE_ASSETSPOOL e WHERE w.ID=%u AND w.UserID=d.UserID AND e.CoinID=d.BlockChainID AND e.CoinID=%u);",EXCHANGEADDR_TO_POOLADDR,time(nullptr),data.nId,TYPE_ETHEREUM);
            db.ExecSql(sSQL);
            if (db.IsError()) {
                XERROR("ChainWithdraw insert error:%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sSQL);
                g_nExit = 0;
                isError = 2;
            }
        }else if(USERWALLET_TO_EXCHANGEADDR == data.nType && TYPE_BITCOIN == (ChainType)data.nChainType){
            memset(sSQL, 0, SQLLEN);
            snprintf(sSQL, SQLLEN - 1,
                     "INSERT INTO WITHDRAW_APPLY (NetFee,GasPrice,TYPE,ApplyTime,IsCSConfirm,UserID,CoinID,Amount,SrcAddress,DestAddress,PrivateKey)"
                             "(SELECT 0.0,0,%d,%ld,0, w.UserID,w.CoinID,w.Amount,w.DestAddress,e.Address, d.PrivateKey FROM user_depositaddress d,WITHDRAWDEPOSIT_CONFIRMRECORD w"
                             " JOIN EXCHANGE_ASSETSPOOL e ON w.CoinID = e.CoinID WHERE w.ID = %u AND w.UserID = d.UserID AND w.CoinID = d.BlockChainID);",EXCHANGEADDR_TO_POOLADDR, time(nullptr), data.nId);

            db.ExecSql(sSQL);
            if (db.IsError()) {
                XERROR("ChainWithdraw insert error:%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sSQL);
                g_nExit = 0;
                isError = 2;
            }
        }
    }

    if(0 == isError) {
        // 4. 删除出入金确认表中的数据
        memset(sSQL, 0, SQLLEN);
        snprintf(sSQL, SQLLEN - 1, "DELETE FROM WITHDRAWDEPOSIT_CONFIRMRECORD WHERE ID = %u;", data.nId);
        db.ExecSql(sSQL);
        if (db.IsError()) {
            XERROR("BlockConfirm insert error:%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sSQL);
            g_nExit = 0;
            isError = 1;
        }
    }

    if(0 != isError)
    {
        STBlockConfirmFailData writeData = {};
        writeData.nId = data.nId;
        writeData.nUserId = data.nUserId;
        writeData.nCoinId = data.nCoinId;
        writeData.nDirect = data.nType;
        writeData.nType = isError;
        fwrite(&writeData, sizeof(writeData), 1, fp);
    }

	return 0;
}

void *BlockConfirm(void *argv)
{
	char sSQL[SQLLEN] = {};

	MySqlDB db;
	db.Open();
	db.SelectDB(g_stConfig.sDBName.c_str());

    snprintf(sSQL, SQLLEN - 1, "select ChainType, ChainURL, ChainUserName, ChainPWD from BLOCKCHAIN_INFO;");
	MySqlRecordSet rs = {};
	size_t rows  = GetDataListFromDB(db, rs, sSQL, "BlockConfirm");
	if(rows <= 0)
	{
		XERROR("Get BlockChain info failed");
		g_nBlockConfirmProcess = 0;
		db.Close();
		return nullptr;
	}
	std::list<BlockChainInfo> infoList = {};

	for(size_t i =0 ;i< rows ; i++)
	{
		BlockChainInfo info = {};
		info.type = (ChainType)atoi(rs.GetFieldByID(i, 0).c_str());
		strncpy(info.sChainURL,rs.GetFieldByID(i, 1).c_str(),BLOCKCHAINURLLEN-1) ;
		strncpy(info.sChainUserName,rs.GetFieldByID(i, 2).c_str(),BLOCKCHAINUSERLEN-1) ;
		strncpy(info.sChainPWD,rs.GetFieldByID(i, 3).c_str(),BLOCKCHAINPWDLEN-1) ;
		infoList.push_back(info);
	}

	CChainOperator ChainOperator ;
	ChainOperator.Init(infoList);
	char sJson[JSONLEN] = {};
    STTnodeConfig tnodeconfig = {};
    tnodeconfig.mq_exchange_group = g_stConfig.mq_exchange_token;
    tnodeconfig.mq_user = g_stConfig.mq_user_token;
    tnodeconfig.mq_port = g_stConfig.mq_port_token;
    tnodeconfig.mq_passwd = g_stConfig.mq_password_token;
    tnodeconfig.mq_host = g_stConfig.mq_address_token;
    tnodeconfig.mq_vhost = g_stConfig.mq_vhost_token;
    pTnode = new snetwork_xservice_tnode::TNode(tnodeconfig);
    pTnode->Init();
    pTnode->RunConsumer();

    while(1 == g_nExit)
	{
		FILE *fp = fopen("blockconfirm_err.txt","w+");
		if(nullptr == fp)
		{
			XERROR("Create/open blockconfirm_err.txt is error:%s",strerror(errno));
			sleep(5);
			continue;
		}
		
		//扫描确认数据
		memset(sSQL,0,SQLLEN) ;
		snprintf(sSQL,SQLLEN-1, "SELECT  w.ID, w.Txid, w.Type, w.UserID, w.CoinID , w.Amount , b.ConfirmBlkQty,"
				"b.ChainType,w.SrcAddress ,w.DestAddress,w.NetFee,w.GasPrice FROM WITHDRAWDEPOSIT_CONFIRMRECORD w, COIN c, BLOCKCHAIN_INFO b "
				"WHERE w.CoinID = c.ID AND c.BlockChainID = b.ChainType;");

		rs = {};
		rows  = GetDataListFromDB(db, rs, sSQL, "BlockConfirm");
		if(rows <= 0)
		{
			sleep(5);
			fclose(fp);
			continue;
		}

		list<STConfirmData> confirmDataList = {};
		
		for(size_t i=0; i < rows; i++)
		{
			STConfirmData data = {};

			data.nId = static_cast<uint32_t>(atol(rs.GetFieldByID(i, 0).c_str()));
			snprintf(data.sTxId, TXIDLEN - 1, "%s", rs.GetFieldByID(i, 1).c_str());
			data.nType = static_cast<uint32_t>(atoi(rs.GetFieldByID(i, 2).c_str()));
			data.nUserId = static_cast<uint32_t>(atol(rs.GetFieldByID(i, 3).c_str()));
			data.nCoinId = static_cast<uint32_t>(atol(rs.GetFieldByID(i, 4).c_str()));
			data.fAmount = strtod(rs.GetFieldByID(i, 5).c_str(), nullptr);
			data.nChainConfirmBlkQty = strtoul(rs.GetFieldByID(i, 6).c_str(), nullptr, 10);
			data.nChainType = static_cast<uint32_t>(atoi(rs.GetFieldByID(i, 7).c_str()));
			snprintf(data.sSrcAddr, ADDRESSLEN - 1, "%s", rs.GetFieldByID(i, 8).c_str());
			snprintf(data.sDestAddr, ADDRESSLEN - 1, "%s", rs.GetFieldByID(i, 9).c_str());
            data.nGas = strtod(rs.GetFieldByID(i, 10).c_str(), nullptr);
            data.nGasPrice = strtoul(rs.GetFieldByID(i, 11).c_str(), nullptr, 10);
			// 调用区块链库 
			if(ChainOperator.TradeConfirm((ChainType)data.nChainType,&data))
			{
				confirmDataList.push_back(data);
			}
			else
			{
				XERROR("the Tx = %s is not created or exists ,the userid = %d",data.sTxId,data.nUserId);
			}
		}
		 
		// 调用区块链库
		for (auto itr = confirmDataList.begin(); itr != confirmDataList.end(); ++itr)
		{
			STConfirmData &data = *itr;
			
			if((data.nChainType == ChainType::TYPE_ETHEREUM && data.status == 1 && data.nConfirmBlkQty >= data.nChainConfirmBlkQty)
                || (data.nChainType == ChainType::TYPE_BITCOIN && data.nConfirmBlkQty >= data.nChainConfirmBlkQty))//满足确认数时的业务操作
			{
				ProcessConfirmData(db,data,sJson,sSQL,fp);
			}
			else //更新出入金确认表 
			{
				memset(sSQL, 0, SQLLEN);
				snprintf(sSQL,SQLLEN-1,"UPDATE WITHDRAWDEPOSIT_CONFIRMRECORD SET ConfirmBlkQty=%lu,BlkConfirmTime=%ld WHERE ID=%u", data.nConfirmBlkQty, time(nullptr), data.nId);
				
				db.ExecSql(sSQL);
				if (db.IsError()) 
				{
					XERROR("BlockConfirm insert error:%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sSQL);
				}
			}
		} // end for

		fclose(fp);
        sleep(5);
		//sleep(g_stConfig.nConfirm_Chkitime);
	} // end while
	
	pTnode->Close();
	delete pTnode;
	pTnode = nullptr;
	
	g_nBlockConfirmProcess = 0;
    db.Close();
	XERROR("BlockConfirmProcess exit !!!!");
	
	return nullptr;
}

int BlockConfirmProcess()
{
	
	pthread_attr_t attr;
	pthread_attr_init (&attr);   
  	pthread_attr_setscope (&attr, PTHREAD_SCOPE_SYSTEM);   
  	pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);  
	
	pthread_t nThreadID;
	
	if (0 != pthread_create(&nThreadID, &attr, BlockConfirm, nullptr) )
	{
		XERROR("pthread_create TransferCoinProcess error:%s",strerror(errno));
		return -1;
	}
	
	return 0;
}









