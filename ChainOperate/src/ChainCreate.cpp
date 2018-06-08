/*******************************************************************************************

*Department:	Exchange team

*Decrible:  	Create user exchange user address
				
				
*Auhor:			Savin

*Createdtime:	2018-02-28

*ModifyTime:	


********************************************************************************************/

#include "Public.h"

#define AVAILABLE_CHAR_COUNT 62
const char g_availablechar[AVAILABLE_CHAR_COUNT] ={ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',  
 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S',   
'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};  

void GeneratePrikey(char *pKey,int length)
{
	if(length < 0)
		return;
	srand(static_cast<unsigned int>(time(nullptr))) ;
	for(int i = 0 ; i < length; i++) {
		int randomNum = rand() % AVAILABLE_CHAR_COUNT; 
		pKey[i] = g_availablechar[randomNum]; 
	}
	pKey[length] = '\0';
}

void * CreateUserExchangeAddr(void *argv)
{
	char sSQL[SQLLEN]={};
	MySqlDB db;
	db.Open();
	db.SelectDB(g_stConfig.sDBName.c_str());
	
	snprintf(sSQL,SQLLEN-1,"select ChainType, ChainURL,ChainUserName,ChainPWD from BLOCKCHAIN_INFO ;");
	MySqlRecordSet rs;
	size_t rows  = GetDataListFromDB(db, rs, sSQL, "ChainCreate");
	if(rows <= 0)
	{
		XERROR("Get BlockChain info failed");
        g_nCreateUserExchangeAddr = 0;
		db.Close();
		return nullptr;
	}
	std::list<BlockChainInfo> infoList ;

	for(size_t i =0; i < rows; i++)
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
		// 1 找需要创建地址的用户
		memset(sSQL, 0, SQLLEN) ;

		snprintf(sSQL, SQLLEN - 1, "SELECT u.ID, b.ID,b.ChainType,b.ChainLabel FROM BLOCKCHAIN_INFO b ,USER u LEFT JOIN"
                " user_depositaddress d ON u.id = d.UserID WHERE u.IsHaveAddress = 0 AND d.UserID IS NULL;");

		rs = {};
        rows  = GetDataListFromDB(db,rs,sSQL,"ChainCreate");
		if(rows <= 0)
		{
			sleep(5);
			continue;
		}

		XINFO("CreateUserExchangeAddr: Need create %ld addresses!",rows);
		
		list<STExchangeAddrInfo> exchangeAddrList;
		for(size_t i=0; i < rows; i++)
		{
			STExchangeAddrInfo data = {};

			data.nUserId = static_cast<uint32_t>(atol(rs.GetFieldByID(i, 0).c_str()));
			data.nBlockChainID = static_cast<uint32_t>(atoi(rs.GetFieldByID(i, 1).c_str()));
			data.nChainType = static_cast<uint32_t>(atoi(rs.GetFieldByID(i, 2).c_str()));
			strncpy(data.sBTCLabel, rs.GetFieldByID(i, 3).c_str(), BTCLABELLEN - 1);
			
			if(data.nChainType == ChainType::TYPE_ETHEREUM)
			{
				GeneratePrikey(data.sPriKey, 32);
			}
			else
			{
				strncpy(data.sPriKey, "HELLOcTDmWuGFEt69rZhWvYtKevgT6qYBITCOIN" ,PRIVATEKEYLEN - 1);
			}

			//调用接口,创建区块链地址
			if(!(ChainOperator.CreateAddr((ChainType)data.nChainType, &data)))
			{
				XERROR("the UserID = %ld create type = %d blockchain failed",data.nUserId,data.nChainType) ;
				sleep(5);  // for test
				continue;
			}

			exchangeAddrList.push_back(data);
		}

		// 插入入金地址表

		list<STExchangeAddrInfo>::iterator itr;
		for (itr=exchangeAddrList.begin(); itr!=exchangeAddrList.end(); ++itr)
		{
			STExchangeAddrInfo &data = *itr;

			// 插入入金地址; // UserID,BlockChainID 有唯一约束,即使失败不用额外处理什么;
			memset(sSQL, 0, SQLLEN) ;
			snprintf(sSQL, SQLLEN - 1,"INSERT INTO user_depositaddress(UserID,BlockChainID,DeAdress,PrivateKey) Value (%u,%u,'%s','%s')",
				                     data.nUserId, data.nBlockChainID, data.sUserExchangeAddr, data.sPriKey);
			db.ExecSql(sSQL);
			if(db.IsError())
			{
				XERROR("user_depositaddress insert error:%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sSQL);
				continue;
			}
		}

        memset(sSQL, 0, SQLLEN);
        snprintf(sSQL, SQLLEN - 1, "UPDATE USER SET IsHaveAddress = 1 WHERE USER.id IN (SELECT d.userid FROM user_depositaddress d);");
        db.ExecSql(sSQL);
        if(db.IsError()){
            XERROR("user table update error:%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sSQL);
        }

		sleep(5);
	}	
	
	g_nCreateUserExchangeAddr = 0;
    db.Close();
	return nullptr;
}

int ChainCreateProcess()
{
	
	pthread_attr_t attr;
	pthread_attr_init (&attr);   
  	pthread_attr_setscope (&attr, PTHREAD_SCOPE_SYSTEM);   
  	pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);  
	
	pthread_t nThreadID;
	
	if (0 != pthread_create(&nThreadID, &attr, CreateUserExchangeAddr, nullptr) )
	{
		XERROR("pthread_create ChainCreateProcess error:%s",strerror(errno));
		return -1;
	}
	
	return 0;
}



