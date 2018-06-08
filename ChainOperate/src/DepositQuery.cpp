/*******************************************************************************************

*Department:	Exchange team

*Decrible:  	deposit module
				
				
*Auhor:			Savin/Tony

*Createdtime:	2018-02-26

*ModifyTime:	


********************************************************************************************/

#include "Public.h"
#include <math.h>

/*int hex_char_value(char c){
    if(c >= '0' && c <= '9')
        return c - '0';
    else if(c >= 'a' && c <= 'f')
        return (c - 'a' + 10);
    else if(c >= 'A' && c <= 'F')
        return (c - 'A' + 10);

    return 0;
}

uint128_t hex2decimal(std::string& szHex){
    uint128_t number = 0;
    if(szHex[0] == '0' && (szHex[1] == 'x' || szHex[1] == 'X')){
        szHex= szHex.substr(2);
    }
    size_t size = szHex.size();
    for(uint32_t i = 0; i < size; i++) {
        uint128_t tmp = 1;
        for(int j = size - i - 1; j > 0; j--){
            tmp *= 16;
        }
        uint128_t tmp1 = tmp * hex_char_value(szHex[i]);
        number += tmp1;
    }
    return number;
}*/

void* DepositQuery(void *argv)
{
	char sSQL[SQLLEN] = {};

	MySqlDB db;
	db.Open();
	db.SelectDB(g_stConfig.sDBName.c_str());

	snprintf(sSQL, SQLLEN - 1, "select ChainType, ChainURL, ChainUserName, ChainPWD, ScanStartBlock, ChainLabel from BLOCKCHAIN_INFO;");
	MySqlRecordSet rs = {} ;
	size_t rows  = GetDataListFromDB(db, rs, sSQL, "DepositQuery");
	if(rows <= 0)
	{
		XERROR("Get BlockChain info failed.");
		g_nDespositProcess = 0;
        db.Close();
		return nullptr;
	}

	std::list<BlockChainInfo> infoList = {};
	for(size_t i = 0 ; i < rows; i++)
	{
		BlockChainInfo info = {};
		info.type = (ChainType)atoi(rs.GetFieldByID(i, 0).c_str());
		strncpy(info.sChainURL, rs.GetFieldByID(i, 1).c_str(), BLOCKCHAINURLLEN - 1);
		strncpy(info.sChainUserName, rs.GetFieldByID(i, 2).c_str(), BLOCKCHAINUSERLEN - 1);
		strncpy(info.sChainPWD, rs.GetFieldByID(i, 3).c_str(), BLOCKCHAINPWDLEN - 1);
        info.sPosNum = strtoul(rs.GetFieldByID(i, 4).c_str(), nullptr, 10);
        strncpy(info.sChainLabel, rs.GetFieldByID(i, 5).c_str(), BLOCKLABELLEN - 1);
		infoList.push_back(info);
	}

	CChainOperator ChainOperator ;
	ChainOperator.Init(infoList);

	while(1 == g_nExit)
    {

		// 用户入金地址查询
		snprintf(sSQL, SQLLEN-1, "select u.UserID,u.DeAdress, b.ChainName,b.ChainType, b.ConfirmBlkQty,b.ScanStartBlock,b.ID "
                "from user_depositaddress u, BLOCKCHAIN_INFO b where u.BlockChainID = b.ID;");
		
		rs = {};
		rows = GetDataListFromDB(db, rs, sSQL, "DepositQuery");
		if(rows <= 0)
		{
			sleep(5);
			continue;
		}
		
		std::map<std::string, STDepositAddr> bitCoinMap = {};
		std::map<std::string, STDepositAddr> ethMap = {};

		for(size_t i=0; i < rows; i++)
		{
			STDepositAddr data = {};

			data.nUserId = static_cast<uint32_t>(atol(rs.GetFieldByID(i, 0).c_str()));
			snprintf(data.sDestAddr, ADDRESSLEN - 1, "%s", rs.GetFieldByID(i, 1).c_str());
			snprintf(data.sChainName, CHAINNAMELEN - 1, "%s", rs.GetFieldByID(i,2).c_str() );
			data.nChainType = static_cast<uint32_t>(atoi(rs.GetFieldByID(i, 3).c_str()));
			data.nConfirmBlkQty = static_cast<uint32_t>(atoi(rs.GetFieldByID(i, 4).c_str()));
			data.nScanStartBlock = strtoul(rs.GetFieldByID(i, 5).c_str(), nullptr, 10);
			data.nId = static_cast<uint32_t>(atol(rs.GetFieldByID(i, 6).c_str()));
			if(data.nChainType  == 1)
			{
				bitCoinMap.insert(std::pair<std::string,STDepositAddr>(string(data.sDestAddr), data));
			}
			else if(data.nChainType == 2)
			{
				ethMap.insert(std::pair<std::string,STDepositAddr>(string(data.sDestAddr), data)) ;
			}
			else
			{
				XERROR("the User[%u] Chain Type is not match nChainType[%u]",data.nUserId,data.nChainType);
				continue;
			}
		}

        memset(sSQL, 0, SQLLEN);
        snprintf(sSQL, SQLLEN - 1, "SELECT c.ID, c.ContractAddr,c.Decimals FROM COIN c;");
        rs = {};
        rows = GetDataListFromDB(db, rs, sSQL, "DepositQuery");
        if(rows <= 0){
            sleep(5);
            continue;
        }
        std::map<uint32_t, CoinInfo> CoinInfos = {};
        for(size_t i = 0; i < rows; i++){
            CoinInfo tmp_coinfo = {};
            auto coinID = static_cast<uint32_t>(atol(rs.GetFieldByID(i, 0).c_str()));
            tmp_coinfo.sContractAddr = rs.GetFieldByID(i, 1);
            tmp_coinfo.Decimals = static_cast<uint32_t>(atol(rs.GetFieldByID(i, 2).c_str()));
            CoinInfos.insert(std::pair<uint32_t, CoinInfo>(coinID, tmp_coinfo));
        }

		memset(sSQL, 0, SQLLEN);
		snprintf(sSQL, SQLLEN - 1, "SELECT Address,PrivateKey FROM exchange_assetspool WHERE CoinID = 2;");
		rs = {};
		rows = GetDataListFromDB(db, rs, sSQL, "DepositQuery");
		if(rows <= 0){
			sleep(5);
			continue;
		}

		const std::string &Ethpooladdr = rs.GetFieldByID(0, 0);
		const std::string &EthpoolPrikey = rs.GetFieldByID(0, 1);

		list<STDepositAddr> desportInfo_out = {};

        for(auto itli = infoList.begin(); itli != infoList.end(); itli++){
            BlockChainInfo info = *itli;
            if(info.type == ChainType::TYPE_BITCOIN)
            {
                ChainOperator.CheckDeposit(info, &bitCoinMap, desportInfo_out);
            }
            else if(info.type == ChainType::TYPE_ETHEREUM)
            {
                ChainOperator.CheckDeposit(info, &ethMap,desportInfo_out);
            }
            else
            {
                XERROR("Chain Type is can't recognited[%d]",info.type);
                continue;
            }
            *itli = info;
        }

		XINFO("===========================");

		// 调用库操作接口
		/*为什么专门需要一个返回参数?因为防止本次扫描中,一个地址有多笔入金的情况*/
		/* bitcoin */

		db.StartTransaction();
		std::list<BlockChainInfo>::iterator iter;
		bool isFlag = true;
		for(iter = infoList.begin(); iter != infoList.end(); ++iter)
		{
			// 更新 区块链信息表中的 ScanStartBlock 字段
			memset(sSQL, 0, SQLLEN) ;
			snprintf(sSQL, SQLLEN - 1, "UPDATE BLOCKCHAIN_INFO SET ScanStartBlock = %lu where ChainType = %u;",iter->sPosNum,iter->type);
			db.ExecSql(sSQL);
			if(db.IsError())
			{
				XERROR("UPDATE BLOCKCHAIN_INFO error:%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sSQL);
				isFlag = false;
				break;
			}
			XINFO("SQL:%s",sSQL);
		}

		// 发现有入金情况, 就把该笔入金记录插入到 出入金确认记录表; 里面的txid是唯一约束
		// 没有考虑插入失败的情况

		list<STDepositAddr>::iterator itr;
		for (itr = desportInfo_out.begin(); itr != desportInfo_out.end(); ++itr )
		{
			bool IsToken = false;
			STDepositAddr &data = *itr;
			memset(sSQL, 0, SQLLEN);
			if(data.sContractAddr[0] == '\0') {

				uint32_t Type = 0;
				if(Ethpooladdr == data.sSrcAddr){
					Type = POOLADDR_TO_EXCHANGEADDR;
				}else{
					Type = USERWALLET_TO_EXCHANGEADDR;
				}


                // 注意从底层取出来的货币名称 是否是 类似这样的BTC/ETH等, 不是的话,到时在coin表中增加一个字段来标识底层取出来的虚拟货币名称
                snprintf(sSQL, SQLLEN - 1, "INSERT INTO WITHDRAWDEPOSIT_CONFIRMRECORD "
                                 "(UserID,Amount,Type,SrcAddress,DestAddress,ApplyTime,CSConfirmTime,BlkConfirmTime,Txid,ConfirmBlkQty,NetFee,GasPrice,CoinID) "
                                 "VALUES (%u,%f,%d,\"%s\",\"%s\",%ld,0,0,\"%s\",0,0.0,0, (select COIN.ID from COIN where COIN.ID = %u));",
                         data.nUserId, data.fAmount, Type, data.sSrcAddr, data.sDestAddr,
                         time(nullptr), data.sTxID, data.nId);
            }else{
                uint64_t denominator = 0;
                double   fdenominator = 0.0;
                uint32_t coinID = 0;
                for(auto itra = CoinInfos.begin(); itra != CoinInfos.end(); itra++){
                    if(itra->second.sContractAddr == data.sContractAddr){
                        fdenominator = pow(10, itra->second.Decimals);
                        denominator = static_cast<uint64_t>(fdenominator);
                        coinID = itra->first;
                        break;
                    }else{
                        continue;
                    }
                }

                if(denominator == 0){
                    continue;
                }
                std::string strToken = data.sTokenValue;
                uint128_t tokenvalue = hex2decimal(strToken);
                uint64_t ftoken = tokenvalue / denominator;
                uint64_t token_model = tokenvalue % denominator;
                double ftoken_value = ftoken + token_model / fdenominator;
                snprintf(sSQL, SQLLEN - 1, "INSERT INTO WITHDRAWDEPOSIT_CONFIRMRECORD "
                                 "(UserID,Amount,Type,SrcAddress,DestAddress,ApplyTime,CSConfirmTime,BlkConfirmTime,Txid,ConfirmBlkQty,NetFee,GasPrice,CoinID) "
                                 "VALUES (%u,%f,%d,\"%s\",\"%s\",%ld,0,0,\"%s\",0,0.0,0,%u);",
                         data.nUserId, ftoken_value, USERWALLET_TO_EXCHANGEADDR, data.sSrcAddr, data.sDestAddr, time(nullptr), data.sTxID, coinID);
				IsToken = true;
            }
			
			db.ExecSql(sSQL);
			if (db.IsError())
			{
                if(data.nChainType == ChainType::TYPE_ETHEREUM) {
                    XERROR("ChainWithdraw insert error:%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sSQL);
                    isFlag = false;
                    break;
                }else{
                    XINFO("Duplicate bitcoin data.");
                }
			}

			if(IsToken) {
				std::string addr = data.sDestAddr;
				std::string strbalance = ChainOperator.GetBlance(addr);
				uint128_t balance = hex2decimal(strbalance);
				if (balance < 630000000000000) {
					memset(sSQL, 0, SQLLEN);
					snprintf(sSQL, SQLLEN - 1, "INSERT INTO WITHDRAW_APPLY (NetFee,GasPrice,TYPE,ApplyTime,IsCSConfirm,UserID,CoinID,Amount,SrcAddress,"
							"DestAddress,PrivateKey) VALUES (0.001,1000000000,%u,%ld,0,%u,2,0.001,\"%s\",\"%s\",\"%s\");",POOLADDR_TO_EXCHANGEADDR,
							 time(nullptr),data.nUserId,Ethpooladdr.c_str(),data.sDestAddr,EthpoolPrikey.c_str());
					db.ExecSql(sSQL);
				}
			}

		}

		if(isFlag){
			db.Commint();
		}else{
			db.RollBack();
		}
        sleep(5);
	}
	
	g_nDespositProcess = 0;
    db.Close();
	return nullptr;
}

int DespositProcess()
{
	pthread_attr_t attr = {};
	pthread_attr_init (&attr);   
  	pthread_attr_setscope (&attr, PTHREAD_SCOPE_SYSTEM);   
  	pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);  
	
	pthread_t nThreadID;
	
	if (0 != pthread_create(&nThreadID, &attr, DepositQuery, nullptr))
	{
		XERROR("pthread_create DespositProcess error:%s",strerror(errno));
		return -1;
	}
	
	return 0;
}








