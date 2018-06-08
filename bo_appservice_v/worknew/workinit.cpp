#include "../include/workinit.h"


//msg key
CIpcMsg* g_pMsgWorkIpcComRcv;
CIpcMsg* g_pMsgWorkIpcComSnd;
CIpcMsg* g_pMsgWorkIpcBusRcv;
CIpcMsg* g_pMsgWorkIpcBusSnd;
CIpcMsg* g_pMsgWorkIpcBusSyn;


stCrmApi CrmConfig;
stCrmApi g_CrmApi[MAX_CRM_API_COUNT];

interfaceErrorKeyWordsMap mInterfaceErrKWords;


CUserManger g_cusermanage;
char gSrcId[SORCE_ID_LEN];
int InitMsg()
{

	char* pMsgWorkIpcComRcvKey;
	key_t tMsgWorkIpcComRcvKey;

	char* pMsgWorkIpcComSndKey;
	key_t tMsgWorkIpcComSndKey;

	char* pMsgWorkIpcBusRcvKey;
	key_t tMsgWorkIpcBusRcvKey;

	char* pMsgWorkIpcBusRcVKey;
	key_t tMsgWorkIpcBusRcVKey;

	char* pMsgWorkIpcBusSynKey;
	key_t tMsgWorkIpcBusSynKey;


	M_XRETURN(((pMsgWorkIpcComRcvKey = getenv("MSG_WORK_IPC_COMRCV_KEY"))==NULL),
			false, LOG_ERR("get MSG_WORK_IPC_COMRCV_KEY err!"));
	sscanf (pMsgWorkIpcComRcvKey, "%x", &tMsgWorkIpcComRcvKey);
	CIpcMsg MsgWorkIpcComRcv;
	MsgWorkIpcComRcv.IpcCreMsg(tMsgWorkIpcComRcvKey);
	g_pMsgWorkIpcComRcv = &MsgWorkIpcComRcv;


	M_XRETURN(((pMsgWorkIpcComSndKey = getenv("MSG_WORK_IPC_COMSND_KEY"))==NULL),
			false, LOG_ERR("get MSG_WORK_IPC_COMSND_KEY err!"));
	sscanf (pMsgWorkIpcComSndKey, "%x", &tMsgWorkIpcComSndKey);
	CIpcMsg MsgWorkIpcComSndKey;
	MsgWorkIpcComSndKey.IpcCreMsg(tMsgWorkIpcComSndKey);
	g_pMsgWorkIpcComSnd = &MsgWorkIpcComSndKey;

	M_XRETURN(((pMsgWorkIpcBusRcvKey = getenv("MSG_WORK_IPC_BUSRCV_KEY"))==NULL),
			false, LOG_ERR("get MSG_WORK_IPC_BUSRCV_KEY err!"));
	sscanf (pMsgWorkIpcBusRcvKey, "%x", &tMsgWorkIpcBusRcvKey);
	CIpcMsg MsgWorkIpcBusRcv;
	MsgWorkIpcBusRcv.IpcCreMsg(tMsgWorkIpcBusRcvKey);
	g_pMsgWorkIpcBusRcv = &MsgWorkIpcBusRcv;

	M_XRETURN(((pMsgWorkIpcBusRcVKey = getenv("MSG_WORK_IPC_BUSSND_KEY"))==NULL),
			false, LOG_ERR("get MSG_WORK_IPC_BUSSND_KEY err!"));
	sscanf (pMsgWorkIpcBusRcVKey, "%x", &tMsgWorkIpcBusRcVKey);
	CIpcMsg MsgWorkIpcBusRcV;
	MsgWorkIpcBusRcV.IpcCreMsg(tMsgWorkIpcBusRcVKey);
	g_pMsgWorkIpcBusSnd = &MsgWorkIpcBusRcV;

	M_XRETURN(((pMsgWorkIpcBusSynKey = getenv("MSG_WORK_IPC_BUSSYN_KEY"))==NULL),
			false, LOG_ERR("get MSG_WORK_IPC_BUSSYN_KEY err!"));
	sscanf (pMsgWorkIpcBusSynKey, "%x", &tMsgWorkIpcBusSynKey);
	CIpcMsg MsgWorkIpcBusSyn;
	MsgWorkIpcBusSyn.IpcCreMsg(tMsgWorkIpcBusSynKey);
	g_pMsgWorkIpcBusSyn = &MsgWorkIpcBusSyn;
}



CUserManger::CUserManger()
{
}

CUserManger::~CUserManger()
{
	std::map<std::string, userListInfo*>::iterator  mapIt;
	for (mapIt = mapCompUserInfo.begin(); mapIt != mapCompUserInfo.end(); mapIt++)
	{
		userListInfo *sUserListInfo = mapIt->second;
		list<std::string*>::iterator it;
		for (it = sUserListInfo->m_Userlist.begin(); it != sUserListInfo->m_Userlist.end();)
		{
			delete(*it);
			*it = NULL;
			sUserListInfo->m_Userlist.erase(it++);
		}
		delete(mapIt->second);
		mapCompUserInfo.erase(mapIt);
	}
}

int CUserManger::InitUserList()
{
	char *  pFilePath = NULL;
	FILE *fp = NULL;
	char line[40960];

	memset(line, 0x00, sizeof(line));

	if ((pFilePath = getenv("UL_FILE_CFG")) == NULL)
	{
		LOG_INFO("get configure err");
		return FAILE;
	}

	//pFilePath = (char *)"/home/jxolsys/jxolsys/etc/userlist.inimax";
	if ((fp = fopen(pFilePath, "r")) != NULL)
	{
		while (fgets(line, sizeof(line), fp) != NULL)
		{
			if (line[0] == '#')
			{
				continue;
			}

			int len = strlen(line);
			//去掉尾部的 \n 
			if (line[0] == '[' && line[len - 2] == ']')
			{
				char company[128] = { 0 };
				line[len - 2] = '\0';
				memcpy(company, line + 1, len - 3); // 去掉 [ ] \n
				LOG_INFO("get a company: %s\n", company);
				userListInfo *sUserinfo = new userListInfo;
				memset(line, 0x00, sizeof(line));
				while ((fgets(line, sizeof(line), fp)) != NULL)
				{
					if (line[0] == '[')
					{
						break;
					}

					Json::Value value;
					Json::Reader reader;
					if (reader.parse(line, value))
					{
						M_XRETURN((value["use"].isNull()), false, LOG_ERR("get use err"));
						M_XRETURN((value["flag"].isNull()), false, LOG_ERR("get flag err"));

						std::string strUse = value["use"].asString();
						std::string strFlag = value["flag"].asString();
						LOG_INFO("company[%s],use[%s], falg[%d]", company, strUse.c_str(), strFlag.c_str());

						sUserinfo->m_use = atoi(strUse.c_str());
						sUserinfo->m_flag = atoi(strFlag.c_str());
						const Json::Value arrayUser = value["user"];
						LOG_INFO("arrsz[%d]", arrayUser.size());
						for (unsigned int i = 0; i < arrayUser.size(); i++)
						{
							std::string* pstr = new std::string;
							*pstr = arrayUser[i].asString();
							sUserinfo->m_Userlist.push_back(pstr);
							LOG_INFO("MT4[%s]", arrayUser[i].asString().c_str());
						}

						mapCompUserInfo.insert(make_pair(company, sUserinfo));
						LOG_INFO("insert a company userlist, company:%s,count:%d,use:%d,flag:%d", 
								company, mapCompUserInfo.size(),sUserinfo->m_use,sUserinfo->m_flag);
					}

					break;
				}
			}
		}
	}

	if (fp != NULL)
	{
		fclose(fp);
	}

	return SUCESS;
}
// SUCESS:用户可以登录，FAILE:用户不能登录
int CUserManger::FindUser(std::string szUser,char* szSrcId)
{
	std::map<std::string, userListInfo*>::iterator it = mapCompUserInfo.find(szSrcId);
	if (it == mapCompUserInfo.end())
	{
		LOG_ERR("comany is not exit,%s", szSrcId);
		return SUCESS;  // 公司没有配置文件，则可以直接登录
	}

	userListInfo *sUserInfo = mapCompUserInfo[szSrcId];

	if (sUserInfo->m_use == 0)
	{
		LOG_ERR("comany user config is valid,company:%s, useflag:%d,flag:%d", 
				szSrcId,sUserInfo->m_use,sUserInfo->m_flag);
		return SUCESS; // 配置文件不生效, 则可以登录
	}

	std::string realUser = szUser.substr(strlen(szSrcId));
	// 白名单
	if (sUserInfo->m_flag == 1)
	{
		list<std::string*>::iterator it;
		for (it = sUserInfo->m_Userlist.begin(); it != sUserInfo->m_Userlist.end(); it++)
		{
			if (*(*it) == realUser)
			{
				LOG_INFO("User[%s] is in list", realUser.c_str());
				return SUCESS; // 在白名单中找到用户，则可以登录
			}
		}
		return FAILE; // 在白名单中未找到用户，则不可以登录
	}
	else
	{
		list<std::string*>::iterator it;
		for (it = sUserInfo->m_Userlist.begin(); it != sUserInfo->m_Userlist.end(); it++)
		{
			if (*(*it) == realUser)
			{
				LOG_INFO("User[%s] is in list", realUser.c_str());
				return FAILE; // 在白名单中找到用户，则不可以登录
			}
		}

		return SUCESS; // 在黑名单中未找到用户，则可以登录
	}


}
int LoadCrmConfig(CRedis& rds)
{
	std::string sCrmApi = rds.get("CRMAPICONFIG");

	if (sCrmApi.length() == 0)
	{
		LOG_ERR("get crm api info from redis error");
		return -1;
	}
	LOG_INFO("get crm api redis string,%s", sCrmApi.c_str());

	Json::Reader reader;
	Json::Value crmApiJson;
	if (!reader.parse(sCrmApi, crmApiJson))
	{
		LOG_ERR("parse crm api info error");
		return -2;
	}
	if (crmApiJson.isNull())
	{
		LOG_ERR("crmApiJson is null ");
		return -3;
	}
	Json::Value crmApiArray = crmApiJson["crmapi"];
	if (!crmApiArray.isArray())
	{
		LOG_ERR("crmApiArray is not a array,type:%d", crmApiArray.type());
		return -3;
	}
	LOG_ERR("crmApiArray is a array,type:%d", crmApiArray.type());


	unsigned int i = 0;
	unsigned int crmApiArraySize = MAX_CRM_API_COUNT;
	unsigned int crmJsonSize = crmApiArray.size();

	LOG_INFO("get crm api,mem array size:%d, redis size:%d", crmApiArraySize, crmJsonSize);

	for (unsigned int i = 0; i < crmApiArraySize && i < crmJsonSize; i++)
	{
		pstCrmApi pStCrmApiTmp = &g_CrmApi[i];

		Json::Value crmApiItem = crmApiArray[i];

		LOG_INFO("crmApiItem type:%d", crmApiItem.type());
		if (crmApiItem.isObject())
		{
			pStCrmApiTmp->strCompany = crmApiItem["company"].asString();

			pStCrmApiTmp->strUrlCrmHy = crmApiItem["crmhy"].asString();
			pStCrmApiTmp->strUrlCrmMy = crmApiItem["crmmy"].asString();
			pStCrmApiTmp->strUrlBoHy = crmApiItem["bohy"].asString();
			pStCrmApiTmp->strUrlBoMy = crmApiItem["bomy"].asString();
			pStCrmApiTmp->strUrlSerProEn = crmApiItem["serproen"].asString();
			pStCrmApiTmp->strUrlSerProCn = crmApiItem["serprocn"].asString();
			pStCrmApiTmp->strServerUrl = crmApiItem["serverurl"].asString();
			pStCrmApiTmp->strServerPort = crmApiItem["serverport"].asString();

			pStCrmApiTmp->strAppIosVersion = crmApiItem["iosver"].asString();
			
			/*pStCrmApiTmp->strAppMidIosVersion = crmApiItem["iosmultver"].asString();
			pStCrmApiTmp->strAppMidAnrVersion = crmApiItem["anrmultver"].asString();

			LOG_INFO("mid version ios[%s] anr[%s]",pStCrmApiTmp->strAppMidIosVersion.c_str(),pStCrmApiTmp->strAppMidAnrVersion.c_str());*/
			
			pStCrmApiTmp->strIosVerUrl = crmApiItem["iosverurl"].asString();
			pStCrmApiTmp->strAppIosRevieweverUrl = crmApiItem["iosreviewever"].asString();
			pStCrmApiTmp->strAppAdrRevieweverUrl = crmApiItem["adrreviewever"].asString();
			pStCrmApiTmp->strMustUpdateIosVersion = crmApiItem["iosmustver"].asString();
			pStCrmApiTmp->strAppAdrVersion = crmApiItem["adrver"].asString();
			pStCrmApiTmp->strMustUpdateAdrVersion = crmApiItem["adrmustver"].asString();
			pStCrmApiTmp->strSupportWeb = crmApiItem["supportweb"].asString();

			pStCrmApiTmp->strAppIosSkin = crmApiItem["iosskin"].asString();
			pStCrmApiTmp->strMustUpdateIosSkin = crmApiItem["iosmustkin"].asString();
			pStCrmApiTmp->strAppAdrSkin = crmApiItem["adrkin"].asString();
			pStCrmApiTmp->strMustUpdateAdrSkin = crmApiItem["adrmustkin"].asString();

			pStCrmApiTmp->strAppIosSymbol = crmApiItem["iossmbl"].asString();
			pStCrmApiTmp->strMustUpdateIosSymbol = crmApiItem["iosmustsmbl"].asString();
			pStCrmApiTmp->strAppAdrSymbol = crmApiItem["adrsmbl"].asString();
			pStCrmApiTmp->strMustUpdateAdrSymbol = crmApiItem["adrmustsmbl"].asString();

			pStCrmApiTmp->strIosUpdateVerUrl = crmApiItem["iosverurl"].asString();
			pStCrmApiTmp->strIosUpdateSkinUrl = crmApiItem["iosskinurl"].asString();
			pStCrmApiTmp->strIosUpdateSymbolUrl = crmApiItem["iossmblurl"].asString();

			pStCrmApiTmp->strAdrUpdateVerUrl = crmApiItem["adrverurl"].asString();
			pStCrmApiTmp->strAdrUpdateSkinUrl = crmApiItem["adrskinurl"].asString();
			pStCrmApiTmp->strAdrUpdateSymbolUrl = crmApiItem["adrsmblurl"].asString();

			pStCrmApiTmp->strRegistTmpFile = crmApiItem["regtmpfile"].asString();
			pStCrmApiTmp->strVideoUrl = crmApiItem["viedourl"].asString();
			pStCrmApiTmp->strMerchantKey = crmApiItem["merchantkey"].asString();
			pStCrmApiTmp->strAppIp = crmApiItem["appip"].asString();
			pStCrmApiTmp->strDoMain = crmApiItem["domain"].asString();
			pStCrmApiTmp->strMinTransAmount_zh = crmApiItem["mintransamount_zh"].asString();
			pStCrmApiTmp->strMinTransAmount_en = crmApiItem["mintransamount_en"].asString();
			pStCrmApiTmp->strProUser= crmApiItem["proflieuser"].asString();
			pStCrmApiTmp->strFedBackUrl = crmApiItem["feedbackurl"].asString();

			pStCrmApiTmp->strMaintenanceflag = crmApiItem["maintenanceflag"].asString();
			pStCrmApiTmp->strMaintenancecode = crmApiItem["maintenancecode"].asString();
			pStCrmApiTmp->strMaintenancedes_cn = crmApiItem["maintenancedes_cn"].asString();
			pStCrmApiTmp->strMaintenancedes_en = crmApiItem["maintenancedes_en"].asString();

			pStCrmApiTmp->strAppMidIosVersion = rds.hget(pStCrmApiTmp->strCompany, std::string("iosmultver"));
			pStCrmApiTmp->strAppMidAnrVersion = rds.hget(pStCrmApiTmp->strCompany,std::string("anrmultver"));
			LOG_INFO("key[%s] mid version ios[%s] anr[%s]",pStCrmApiTmp->strCompany.c_str(),pStCrmApiTmp->strAppMidIosVersion.c_str(),pStCrmApiTmp->strAppMidAnrVersion.c_str());

			pStCrmApiTmp->strIOS_Merchantkey = rds.hget(pStCrmApiTmp->strCompany, std::string("ios_merchantkey"));
			pStCrmApiTmp->strIOS_AppIp = rds.hget(pStCrmApiTmp->strCompany,std::string("ios_appip"));
			pStCrmApiTmp->strIOS_DoMain = rds.hget(pStCrmApiTmp->strCompany,std::string("ios_domain"));

			pStCrmApiTmp->strIosMaintenanceflag = rds.hget(pStCrmApiTmp->strCompany, std::string("ios_maintenanceflag"));
			pStCrmApiTmp->strIosMaintenancecode = rds.hget(pStCrmApiTmp->strCompany, std::string("ios_maintenancecode"));
			pStCrmApiTmp->strIosMaintenancedes_cn = rds.hget(pStCrmApiTmp->strCompany, std::string("ios_maintenancedes_cn"));
			pStCrmApiTmp->strIosMaintenancedes_en = rds.hget(pStCrmApiTmp->strCompany, std::string("ios_maintenancedes_en"));
			LOG_INFO("key[%s] mid strMaintenanceflag ios[%s] anr[%s]", pStCrmApiTmp->strCompany.c_str(), pStCrmApiTmp->strIosMaintenanceflag.c_str(), pStCrmApiTmp->strMaintenanceflag.c_str());

		}

	}



	LOG_INFO("get crm api mem array size:%d, redis size:%d", crmApiArraySize, crmJsonSize);

	LOG_INFO("get info bo:%s,%s, app/domain:%s,%s",
			g_CrmApi[0].strCompany.c_str(), g_CrmApi[0].strUrlBoHy.c_str(), g_CrmApi[1].strAppIp.c_str(), g_CrmApi[1].strDoMain.c_str());
	return 0;
}

void makeErrCnEnDeque(const char *key, const char *en, const char *cn,keyWordsDeque &keyWordsMap)
{
	errDesc mErrDesc;
	mErrDesc.keyWords = key; 
	mErrDesc.enDesc = en;
	mErrDesc.cnDesc = cn;

	keyWordsMap.push_back(mErrDesc);
}




void interfaceErrKWordsInit(CRedis& rds)
{
	keyWordsDeque keyWordsMap;
	std::string sErrKWords = rds.get("SERRKWORDS");
	LOG_INFO("SERRKWORDS[%s]", sErrKWords.c_str());

	Json::Reader reader;
	Json::Value rootJson;
	if (!reader.parse(sErrKWords, rootJson))
	{   
		LOG_ERR("sErrKWords info error");
		return ;
	}   
	if (rootJson.isNull())
	{   
		LOG_ERR("rootJson sErrKWords  is null ");
		return;
	}   
	// 获取KEY值
	Json::Value arrayKeys = rootJson["keys"];
	LOG_INFO("get SERRKWORDS of keys[%d]", arrayKeys.size());
	std::string strEname;
	std::string strCname;
	std::string strKey;
	std::string strsubKey;
	for (unsigned int i = 0; i < arrayKeys.size(); i++)
	{   
		strKey = arrayKeys[i].asString();
		Json::Value subarrayKeys = rootJson[strKey.c_str()]["subkeys"];
		for (unsigned int j = 0; j < subarrayKeys.size(); j++)
		{   
			strsubKey = subarrayKeys[j].asString();
			strEname = rootJson[strKey.c_str()][strsubKey.c_str()]["ename"].asString();
			strCname = rootJson[strKey.c_str()][strsubKey.c_str()]["cname"].asString();
			makeErrCnEnDeque(strsubKey.c_str(), strEname.c_str(), strCname.c_str(), keyWordsMap);
		}   
		mInterfaceErrKWords.insert(make_pair((char *)(strKey.c_str()), keyWordsMap));
		keyWordsMap.clear();
	}   
}

int GetCrmConfig(stCrmApi& Crmapi)
{
	unsigned int i = 0;
	unsigned int len =MAX_CRM_API_COUNT;

	for (i = 0; i < len; i++)
	{
		if (g_CrmApi[i].strCompany == Crmapi.strCompany)
		{
			cpystCrmApi(g_CrmApi[i], Crmapi);
			//Crmapi = g_CrmApi[i];
			return SUCESS;
		}
	}
	LOG_INFO("can not find the sorceid");
	return FAILE;
}

void cpystCrmApi(stCrmApi& srcCrmapi, stCrmApi& dstCrmapi)
{

	dstCrmapi.strCompany=srcCrmapi.strCompany;
	dstCrmapi.strUrlCrmHy=srcCrmapi.strUrlCrmHy;
	dstCrmapi.strUrlCrmMy=srcCrmapi.strUrlCrmMy;
	dstCrmapi.strUrlBoHy=srcCrmapi.strUrlBoHy;
	dstCrmapi.strUrlBoMy=srcCrmapi.strUrlBoMy;
	dstCrmapi.strUrlSerProEn=srcCrmapi.strUrlSerProEn;
	dstCrmapi.strUrlSerProCn=srcCrmapi.strUrlSerProCn;
	dstCrmapi.strServerUrl=srcCrmapi.strServerUrl;
	dstCrmapi.strServerPort=srcCrmapi.strServerPort;

	dstCrmapi.strAppIosVersion=srcCrmapi.strAppIosVersion;
	dstCrmapi.strAppIosRevieweverUrl = srcCrmapi.strAppIosRevieweverUrl;
	dstCrmapi.strMustUpdateIosVersion=srcCrmapi.strMustUpdateIosVersion;
	dstCrmapi.strAppAdrVersion=srcCrmapi.strAppAdrVersion;
	dstCrmapi.strAppAdrRevieweverUrl = srcCrmapi.strAppAdrRevieweverUrl;
	dstCrmapi.strMustUpdateAdrVersion=srcCrmapi.strMustUpdateAdrVersion;
	dstCrmapi.strSupportWeb=srcCrmapi.strSupportWeb;
	dstCrmapi.strIosVerUrl = srcCrmapi.strIosVerUrl;

	dstCrmapi.strAppIosSkin=srcCrmapi.strAppIosSkin;
	dstCrmapi.strMustUpdateIosSkin=srcCrmapi.strMustUpdateIosSkin;
	dstCrmapi.strAppAdrSkin=srcCrmapi.strAppAdrSkin;
	dstCrmapi.strMustUpdateAdrSkin=srcCrmapi.strMustUpdateAdrSkin;

	dstCrmapi.strAppIosSymbol=srcCrmapi.strAppIosSymbol;
	dstCrmapi.strMustUpdateIosSymbol=srcCrmapi.strMustUpdateIosSymbol;
	dstCrmapi.strAppAdrSymbol=srcCrmapi.strAppAdrSymbol;
	dstCrmapi.strMustUpdateAdrSymbol=srcCrmapi.strMustUpdateAdrSymbol;

	dstCrmapi.strIosUpdateVerUrl=srcCrmapi.strIosUpdateVerUrl;
	dstCrmapi.strIosUpdateSkinUrl=srcCrmapi.strIosUpdateSkinUrl;
	dstCrmapi.strIosUpdateSymbolUrl=srcCrmapi.strIosUpdateSymbolUrl;

	dstCrmapi.strAdrUpdateVerUrl=srcCrmapi.strAdrUpdateVerUrl;
	dstCrmapi.strAdrUpdateSkinUrl=srcCrmapi.strAdrUpdateSkinUrl;
	dstCrmapi.strAdrUpdateSymbolUrl=srcCrmapi.strAdrUpdateSymbolUrl;

	dstCrmapi.strRegistTmpFile=srcCrmapi.strRegistTmpFile;
	dstCrmapi.strVideoUrl=srcCrmapi.strVideoUrl;
	dstCrmapi.strMerchantKey=srcCrmapi.strMerchantKey;
	dstCrmapi.strAppIp=srcCrmapi.strAppIp;
	dstCrmapi.strDoMain=srcCrmapi.strDoMain;
	dstCrmapi.strIOS_Merchantkey = srcCrmapi.strIOS_Merchantkey;
	dstCrmapi.strIOS_AppIp = srcCrmapi.strIOS_AppIp;
	dstCrmapi.strIOS_DoMain = srcCrmapi.strIOS_DoMain;
	dstCrmapi.strMinTransAmount_zh=srcCrmapi.strMinTransAmount_zh;
	dstCrmapi.strMinTransAmount_en=srcCrmapi.strMinTransAmount_en;
	dstCrmapi.strProUser = srcCrmapi.strProUser;
	dstCrmapi.strFedBackUrl = srcCrmapi.strFedBackUrl;

	dstCrmapi.strMaintenanceflag = srcCrmapi.strMaintenanceflag;
	dstCrmapi.strMaintenancecode = srcCrmapi.strMaintenancecode;
	dstCrmapi.strMaintenancedes_cn = srcCrmapi.strMaintenancedes_cn;
	dstCrmapi.strMaintenancedes_en = srcCrmapi.strMaintenancedes_en;

	dstCrmapi.strIosMaintenanceflag = srcCrmapi.strIosMaintenanceflag;
	dstCrmapi.strIosMaintenancecode = srcCrmapi.strIosMaintenancecode;
	dstCrmapi.strIosMaintenancedes_cn = srcCrmapi.strIosMaintenancedes_cn;
	dstCrmapi.strIosMaintenancedes_en = srcCrmapi.strIosMaintenancedes_en;

	dstCrmapi.strAppMidIosVersion = srcCrmapi.strAppMidIosVersion;
	dstCrmapi.strAppMidAnrVersion = srcCrmapi.strAppMidAnrVersion;
}
