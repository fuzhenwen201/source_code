#include "../include/worknew.h"

void CreateToken(char *toke, char *user);
#if 0
int UserhistoryReqCrm(char *src, char *dest, int &nRoute, std::string strToken);
int UserTradehistoryReqCrm(char *src, char *dest, int &nRoute, std::string strToken);
#endif

int getNginxRealIp(char *src, char *ip)
{
	char *pBpos = NULL, *pEpos = NULL, *pTmp = NULL;
	char szIP[] = { "X-Forwarded-For" };
	char szEnd[] = { "\r\n" };
	char cEnd1 = '\0';
	char cEnd2 = '\0';
	char *pIp = ip;

	pBpos = strstr(src, szIP);
	if (NULL == pBpos)
	{
		LOG_ERR("not find ip, src = %s", src);
		return FAILE;
	}
	pEpos = strstr(pBpos, szEnd);
	if (NULL == pEpos)
	{
		LOG_ERR("not find end, src = %s", src);
		return FAILE;
	}
	//暂时截断。
	cEnd1 = *pEpos;
	*pEpos = '\0';
	//跳过 X-Forwarded-For:
	pBpos += (strlen(szIP) + 1);

	pTmp = strstr(pBpos, ",");
	if (NULL == pTmp)
	{
		//直接拷贝到结尾
		char *pSrc = pBpos;
		while (*pSrc)
		{
			//跳过空格
			if (*pSrc == ' ')
			{
				pSrc++;
				continue;
			}
			(*ip++) = (*pSrc++);
		}
		//恢复截断
		*pEpos = cEnd1;
	}
	else
	{
		//截断这个位置，直接拷贝之前的数据
		cEnd2 = *pTmp;
		*pTmp = '\0';
		char *pSrc = pBpos;
		while (*pSrc)
		{
			//跳过空格
			if (*pSrc == ' ')
			{
				pSrc++;
				continue;
			}
			(*ip++) = (*pSrc++);
		}
		//恢复截断
		*pTmp = cEnd2;
		*pEpos = cEnd1;
	}

	LOG_INFO("ip = %s|||", pIp);
	return 0;
}

void makeMsgHead(char *pHead,char *pMsgType)
{
	memcpy(pHead, pMsgType, MSG_TYPE_LEN);
}

int sendToDelMT4(std::string &strType, std::string &strToken, std::string &bourl)
{
	char szMsgHead[nHeadLen + 1];
	char szMsgType[] = { "9999999999999999" };
	char dest[2048] = { 0 };
	IpcMsgBuf ipcSynBuf;

	std::string strBoToken = "Authorization:";
	strBoToken += strType;
	strBoToken += " ";
	strBoToken += strToken;
	strcat(dest, "URL:");
	strcat(dest, bourl.c_str());
	strcat(dest, USER_LOGOUT_BO);
	strcat(dest, "|");

	sprintf(dest + strlen(dest), "%s%s",
			TAG_CRM_TOKEN, strBoToken.c_str());

	memset(szMsgHead, 0, nHeadLen);
	makeMsgHead(szMsgHead,szMsgType);

	memset(ipcSynBuf.mtext, 0x00, BUFSZ);
	memcpy(ipcSynBuf.mtext, szMsgHead, nHeadLen);
	memset(ipcSynBuf.mtext + MSG_TYPE_LEN, ' ', nHeadLen - MSG_TYPE_LEN);
	ipcSynBuf.mtext[MSG_TYPE_LEN + MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN] = '3';
	memcpy(ipcSynBuf.mtext + nHeadLen, dest, strlen(dest));

	LOG_INFO("buf[%s]", ipcSynBuf.mtext);
	ipcSynBuf.mtype = atol(szMsgType);

	char *pMsgWorkIpcBusSndKey;
	key_t tMsgWorkIpcBusRcVKey;
	M_XRETURN(((pMsgWorkIpcBusSndKey = getenv("MSG_WORK_IPC_BUSSND_KEY")) == NULL),
			false, LOG_ERR("get MSG_WORK_IPC_BUSSND_KEY err!"));
	sscanf(pMsgWorkIpcBusSndKey, "%x", &tMsgWorkIpcBusRcVKey);
	CIpcMsg MsgWorkIpcBusSnd;
	MsgWorkIpcBusSnd.IpcCreMsg(tMsgWorkIpcBusRcVKey);
	MsgWorkIpcBusSnd.IpcSndMsg(ipcSynBuf, strlen(ipcSynBuf.mtext));

}

int DelMt4Token(char *pKey, CRedis& pRds)
{
	if (pKey == NULL)
	{
		return -1;
	}

	char szAccountLen[3] = { 0 }, szWlId[10] = {0};
	int nAccountLen = -1;
	char cTmp = '\0', *pTmp = NULL;
	memcpy(szAccountLen, pKey, 2);
	nAccountLen = atoi(szAccountLen);
	if (nAccountLen < 0)
	{
		LOG_ERR("account len = %d", nAccountLen);
		return -1;
	}
	LOG_DEBUG("account len = %d", nAccountLen);

	pTmp = pKey;
	pTmp += 2;

	cTmp = pTmp[9];
	pTmp[9] = '\0';
	memcpy(szWlId, pTmp, 9);
	pTmp[9] = cTmp;

	cTmp = pTmp[nAccountLen];
	pTmp[nAccountLen] = '\0';

	LOG_DEBUG("account = %s, wlid = %s", pTmp, szWlId);

	stCrmApi CrmConfig;
	CrmConfig.strCompany = szWlId;
	if (FAILE == GetCrmConfig(CrmConfig))
	{
		LOG_ERR("get cmr config error, id:%s", szWlId);
	}

	std::string strAcc = pRds.get(pTmp);
	if (strAcc.empty())
	{
		return -1;
	}

	Json::Reader reader;
	Json::Value value;

	if (reader.parse(strAcc.c_str(), value))
	{
		Json::Value jMt4Acc(Json::arrayValue);
		jMt4Acc = value["mt4accounts"];
		for (unsigned int i = 0;i < jMt4Acc.size(); i++)
		{
			std::string strMt4acc = jMt4Acc[i].asString();
			std::string strMt4accInfo = pRds.get(strMt4acc);
			LOG_DEBUG("mt4 acc=%s,info=%s",strMt4acc.c_str(),strMt4accInfo.c_str());
			Json::Reader readerMt4;
			Json::Value valueMt4;
			if (readerMt4.parse(strMt4accInfo.c_str(), valueMt4))
			{
				std::string strType = valueMt4["mt4tokentype"].asString();
				std::string strToken = valueMt4["mt4token"].asString();
				LOG_DEBUG("mt4 type=%s,token=%s", strType.c_str(), strToken.c_str());
				sendToDelMT4(strType, strToken, CrmConfig.strUrlBoMy);
			}
			else
			{
				LOG_ERR("bad mt4,info:%s", strMt4accInfo.c_str());
			}
		}
	}
	else
	{
		LOG_ERR("bad acc , is not json, stracc:%s", strAcc.c_str());
		return -1;
	}
	return  0;	
}

int nCheckOnline(std::string strToken, CRedis &pRds, CBusiness *pBusiness, char *szSrcId, stCrmApi &CrmConfig)
{
	if(strToken == "") return FAILE;

	std::string strRdsToken = pRds.get(strToken);

	Json::Reader reader;
	Json::Value crm;
	Json::Value token;
	std::string strCrmUser;
	std::string strCrmUserLen;

	strCrmUserLen = strToken.substr(0, CRM_USER_LEN_LEN);
	strCrmUser = strToken.substr(CRM_USER_LEN_LEN, atoi(strCrmUserLen.c_str()));
	//get crmuser
	std::string strRdsCrm = pRds.get(strCrmUser);
	LOG_DEBUG("crm user[%s]", strCrmUser.c_str());

	if(strRdsToken != "")
	{

		long lUpdateTime;
		std::string strUpdateTime;
		std::string strCrmLastTime;
		Json::Reader reader;
		Json::Value value;
		Json::Value jsonappToken;
		Json::FastWriter fast;

		if(reader.parse(strRdsToken, value))
		{
			M_XRETURN((value["updatetime"].isNull()), false, LOG_ERR("get updatetime err"));


			strUpdateTime = value["updatetime"].asString();
			if(strUpdateTime != "")
			{
				lUpdateTime = get_tm_sec();

				LOG_DEBUG("TIME[%s]CurTime[%ld],Oltime[%ld]delayonlitime[%ld]",
						strUpdateTime.c_str(), lUpdateTime, gOnlineTimeOut, gOnlineDelayTimeOut);

				value["updatetime"] = Json::Value(ltos(lUpdateTime));


				//if(gOnlineTimeOut >= lUpdateTime - atol(strUpdateTime.c_str()))
				if(gOnlineDelayTimeOut >= lUpdateTime - atol(strUpdateTime.c_str()))
				{
					LOG_DEBUG("user online token[%s] oldtime[%s], newtime[%ld], setbuf[%s]",
							strToken.c_str(), strUpdateTime.c_str(), lUpdateTime,  fast.write(value).c_str());

					LOG_DEBUG("CurTime[%ld],crmtokentimeout[%ld]", lUpdateTime, gCrmTokenTimeOut);
					if(reader.parse(strRdsCrm, crm))
					{
						//在线
						crm["userstatus"] = Json::Value("1");
						pRds.set(strCrmUser, fast.write(crm));
						//pRds.set(strCrmUser,fast.write(crm),gOnlineDelayTimeOut);
					}

					//延长CRM token有效性
					if(!value["crmtokenlasttime"].isNull())
					{
						strCrmLastTime = value["crmtokenlasttime"].asString();

						LOG_DEBUG("crmtokenlasttime[%s]", strCrmLastTime.c_str());

						//if(gCrmTokenTimeOut < lUpdateTime - atol(strCrmLastTime.c_str()))
						//未超时并且大于一个最大的心跳包
						if(gCrmTokenTimeOut > lUpdateTime - atol(strCrmLastTime.c_str())
								&& gCrmTokenTimeOut - (lUpdateTime - atol(strCrmLastTime.c_str())) <= gOnlineDelayTimeOut)
						{
							//重新登陆
							std::string str;
							char src[2048] = {0};
							char dest[4096] = {0};
							//char szCrmPwd[CRM_PWD_LEN+1] = {0};
							int nRoute = 0;
							IpcMsgBuf buf;
							memset(buf.mtext, 0x00, sizeof(BUFSZ));

							char *pMsgWorkIpcBusSndKey;
							key_t tMsgWorkIpcBusRcVKey;
							M_XRETURN(((pMsgWorkIpcBusSndKey = getenv("MSG_WORK_IPC_BUSSND_KEY")) == NULL),
									false, LOG_ERR("get MSG_WORK_IPC_BUSSND_KEY err!"));
							sscanf (pMsgWorkIpcBusSndKey, "%x", &tMsgWorkIpcBusRcVKey);
							CIpcMsg MsgWorkIpcBusSnd;
							MsgWorkIpcBusSnd.IpcCreMsg(tMsgWorkIpcBusRcVKey);



							//buf.mtype = 999999999999999;
							buf.mtype = atol(MSG_CRM_TOKEN_EXTENDED);
							memcpy(src, MSG_CRM_TOKEN_EXTENDED, MSG_TYPE_LEN);
							memset(src + MSG_TYPE_LEN, ' ', nHeadLen - MSG_TYPE_LEN);

							//memcpy(buf.mtext, MSG_CRM_TOKEN_EXTENDED, MSG_TYPE_LEN);
							//memset(buf.mtext+MSG_TYPE_LEN, ' ', nHeadLen - MSG_TYPE_LEN);
							if(!value["crmtokenlasttime"].isNull() && !value["crmtokenlasttime"].isNull())
							{
								std::string crmuser = value["crmuser"].asString();
								std::string crmpwd = value["crmpwd"].asString();
								std::string host = value["issuer"].asString();
								//std::string macaddress = value["macadress"].asString();
								//macaddress.erase(std::find_if(macaddress.rbegin(), macaddress.rend(),
								//std::not1(std::ptr_fun(::isspace))).base(),macaddress.end());
								//char szMac[MAC_LEN + 1] = {0};
								sprintf(src + nHeadLen, "GET /App/login?email=%s&password=%s&sourceid=%s&macaddress=00-00-00-00-00-00 HTTP/1.1",
										crmuser.c_str(), crmpwd.c_str(), szSrcId);
								//char szPwd[CRM_PWD_LEN+1] = {0};
								//char *szTmp = const_cast<char*>(crmpwd.c_str());
								//char szTmpPwd[CRM_PWD_LEN+3]={0};
								//memcpy(szTmpPwd,crmpwd.c_str(), crmpwd.size());
								//strrpl(szPwd, szTmpPwd, "%3D", "=");
								//sprintf(buf.mtext+nHeadLen,"URL:%s/api/v1/login|SNDTAG:email=%s&password=%s",
								//host.c_str(),crmuser.c_str()+strlen(szSrcId),szPwd);
								//LOG_INFO("src[%s] ,buf.mtext[%s]",src,buf.mtext);

								pBusiness->UserLoginReqCrm(src, buf.mtext, nRoute, str, pRds, szSrcId, CrmConfig);
								DelSrcId(buf.mtext, szSrcId);
#if 0
								char *nPos = NULL;
								int nLen;
								if((nPos = strstr(buf.mtext, TAG_EMAIL)) )
								{
									nLen = strlen(TAG_EMAIL);
								}
								if(nPos != NULL)
								{
									//del user sorceid
									//LOG_INFO("len[%d]", nLen);
									nLen += (nPos - buf.mtext);
									//LOG_INFO("len[%d]", nLen);
									if(buf.mtext[nLen] == '&')
									{
										++nLen;
										//LOG_INFO("c[%c]nLen[%d]", buf.mtext[nLen - 1], nLen);
									}

									memcpy(buf.mtext + nLen, buf.mtext + nLen + strlen(szSrcId),
											strlen(buf.mtext) - nLen - strlen(szSrcId));
									buf.mtext[strlen(buf.mtext) - strlen(szSrcId)] = 0 ;
								}
#endif
								LOG_INFO("buf:[%s]", buf.mtext);
								MsgWorkIpcBusSnd.IpcSndMsg(buf, strlen(buf.mtext));
							}
						}
					}

					//更新原因时间
					//pRds.set(strToken, fast.write(value));
					pRds.set(strToken, fast.write(value), gOnlineDelayTimeOut);
					return SUCESS;
				}
				else
				{
					//离线
					if(reader.parse(strRdsCrm, crm))
					{
						crm["userstatus"] = Json::Value("2");
						//pRds.set(strCrmUser, fast.write(crm));
						pRds.set(strToken, fast.write(value), gOnlineDelayTimeOut);
						LOG_INFO("token[%s]user offline", strToken.c_str());
					}
				}
			}
			else
			{
				LOG_ERR("token[%s]get updata time err", strToken.c_str());
			}
		}
	}
	else
	{
		if(reader.parse(strRdsCrm, crm))
		{
			if(!crm["userstatus"].isNull())
			{
				std::string strUserStatus = crm["userstatus"].asString();
				std::string strAppToken = crm["app_token"].asString();
				LOG_INFO("user status[%s]", strUserStatus.c_str());

				if(strUserStatus == "2" )
				{
					LOG_INFO("token[%s]user offline", strToken.c_str());
					return -1;
				}
				else if(strUserStatus == "1" || strUserStatus == "3")
				{
					if(strAppToken == strToken)
					{
						LOG_INFO("token[%s]user offline", strToken.c_str());
						return -1;
					}
					else
					{
						LOG_INFO("token[%s]user login other machine", strToken.c_str());
						return -2;
					}

				}
			}
		}
	}
	return FAILE;
}

Json::Value  makeErrJson(const char *interfaceName, const char *errCode, const char *errResult)
{
	Json::Value jsonRsp;
	if (errCode == NULL || errResult == NULL)
		return jsonRsp;

	int matchFlag = false;
	Json::FastWriter fast;
	jsonRsp[TAG_RESULT] = Json::Value(errCode);
	interfaceErrorKeyWordsIt it = mInterfaceErrKWords.find((char *)interfaceName);

	LOG_DEBUG("map key1:%s,size:%d", interfaceName, mInterfaceErrKWords.size());

	std::string enDesc, cnDesc;

	if (it != mInterfaceErrKWords.end())
	{
		keyWordsDeque mKeyWordDeque = it->second;
		for (unsigned int i = 0; i < mKeyWordDeque.size(); i++)
		{
			if (strstr(errResult, mKeyWordDeque[i].keyWords.c_str()) != NULL)
			{
				if (matchFlag)
				{
					enDesc.append(", ");
					cnDesc.append(", ");
				}
				enDesc += mKeyWordDeque[i].enDesc;
				cnDesc += mKeyWordDeque[i].cnDesc;

				matchFlag = true;
			}
		}
	}

	if(!matchFlag)
	{
		enDesc = errResult;
		cnDesc = errResult;
	}

	jsonRsp[TAG_DESCEN] = Json::Value(enDesc);
	jsonRsp[TAG_DESCCN] = Json::Value(cnDesc);

	return jsonRsp;
}

int GetHttpErrorCode(char *src, std::string &errcode)
{
	char szRspCode[5] = { 0 };
	char szTmp[1024] = { 0 };
	char szRspMsg[128] = { 0 };

	cTinyWrok.GetTagVal(src, (char *)TAG_HTTP11, szTmp);
	LOG_DEBUG("tmp[%s]", szTmp);
	if (szTmp[0] != '\0')
	{
		char *nPos = strstr(szTmp, " ");
		if (nPos != NULL)
		{
			// get error code
			memcpy(szRspCode, szTmp, nPos - szTmp);
			memcpy(szRspMsg, nPos + 1, strlen(szTmp) + szTmp - nPos - 1);
			LOG_DEBUG("szRspCode[%s],szRspMsg[%s]nPos[%s]", szRspCode, szRspMsg, nPos);
		}
		else
		{
			LOG_ERR("Get retcode err");
			return FAILE;
		}
	}
	else
	{
		LOG_ERR("Get Tag[%s] err", TAG_HTTP11);
		return FAILE;
	}
	errcode.clear();
	errcode.append(szRspCode);

	return SUCESS;
}

int GetUserTypeAndCrmToken(char *appToken, std::string &urlPrefix, std::string &crmToken, CRedis &pRds , stCrmApi &CrmConfig)
{
	if (appToken == NULL)
		return FAILE;

	std::string strRdsAppToken;
	urlPrefix.clear();
	crmToken.clear();
	//first find crmtoken from share memory
	//char crm_Token[CRM_TOKEN_LEN] = { 0 };
	//g_IpcMemBoost->FindCrmtoken(appToken,crm_Token);
	//if(crmToken[0] != '\0')
	//{
	//  crmToken = crm_Token;
	//  return SUCESS;
	//}
	//then find crmtoken from rds
	if ((strRdsAppToken = pRds.get(appToken)) != "")
	{
		std::string strIsUser;
		cTinyWrok.GetJsonOneTagVal(strRdsAppToken, "issuer", strIsUser);
		if (strstr(strIsUser.c_str(), TAG_MY))
			//urlPrefix.append(g_Url_Crm_My);
			urlPrefix.append(CrmConfig.strUrlCrmMy.c_str());
		else
			//urlPrefix.append(g_Url_Crm_Hy);
			urlPrefix.append(CrmConfig.strUrlCrmHy.c_str());

		// get crm token from redis
		std::string strCrmUser;
		std::string rdsCrmUser;
		cTinyWrok.GetJsonOneTagVal(strRdsAppToken, "crmuser", strCrmUser);
		if ((rdsCrmUser = pRds.get(strCrmUser)) != "")
		{
			std::string strTmp;
			crmToken += "Authorization:";
			strTmp.clear();
			cTinyWrok.GetJsonOneTagVal(rdsCrmUser, "crm_token_type", strTmp);
			crmToken += strTmp;
			crmToken += " ";
			strTmp.clear();
			cTinyWrok.GetJsonOneTagVal(rdsCrmUser, "crm_token", strTmp);
			crmToken += strTmp;
			LOG_DEBUG("get crm token[%s],apptoken[%s],crmuser[%s],usertype[%s]",
					crmToken.c_str(), appToken, strCrmUser.c_str(), strIsUser.c_str());
		}
		else
		{
			LOG_ERR("get crm token failed,apptoken[%s]", appToken);
			return atoi(ERR_SERVER);
		}
	}
	else
	{
		std::string strRdsCrm;
		std::string strCrmUser;
		std::string strCrmUserLen;
		std::string strToken = appToken;

		Json::Reader reader;
		Json::Value crm;
		strCrmUserLen = strToken.substr(0, CRM_USER_LEN_LEN);
		strCrmUser = strToken.substr(CRM_USER_LEN_LEN, atoi(strCrmUserLen.c_str()));
		LOG_DEBUG("token[%s]", strToken.c_str());

		strRdsCrm = pRds.get(strCrmUser);

		if(reader.parse(strRdsCrm, crm))
		{
			if(!crm["userstatus"].isNull())
			{
				std::string strUserStatus = crm["userstatus"].asString();
				std::string strAppToken = crm["app_token"].asString();
				LOG_INFO("user status[%s]", strUserStatus.c_str());

				if(strUserStatus == "2")
				{
					LOG_INFO("token[%s]user offline", strToken.c_str());
					return atoi(ERR_OFFLINE);
				}
				else if(strUserStatus == "3" || strUserStatus == "1")
				{
					if(strAppToken == strToken )
					{
						LOG_INFO("token[%s]user offline", strToken.c_str());
						return atoi(ERR_OFFLINE);
					}
					LOG_INFO("token[%s]user login other machine", strToken.c_str());
					return atoi(ERR_LOGIN_OTHMACHINE);
				}
			}
			return atoi(ERR_SERVER);
		}
	}

	return SUCESS;
}
#if 0
void testauth(CRedis &rds)
{
	std::string str_src  = "0000007995752719                                                                                                                                                                                                                                                                                                                                                                                            2000100013275232                                                                                                                            GVzVDdyEwM==                                                                                                                90 200010001                                     |https://mqsfxbo.chpd8.com:18999/auth|HTTP/1.1 200 OK\r\nCache-Control: no-cache\r\nContent-Type: application/json;charset=UTF-8\r\nDate: Sat, 26 Aug 2017 21:59:05 GMT\r\nExpires: -1\r\nPragma: no-cache\r\nServer: Microsoft-HTTPAPI/2.0\r\nContent-Length: 726\r\nConnection: keep-alive\r\n\r\n{\"access_token\":\"AQAAANCMnd8BFdERjHoAwE_Cl-sBAAAA_eN4erMpJUKj-V8eAkg6cAAAAAACAAAAAAAQZgAAAAEAACAAAADUKOJnxCl9ugE6I0QEIMelPEhKZ6niilQCAt6ZeF4ZHAAAAAAOgAAAAAIAACAAAADIQ3dD3lEp90P4nGva5NRA3XTlGKHD965s_YITBVj2EtAAAACMs3ApwXQFMD_O7zuwXjQzcfJVoZPEcSvKIVjgTjOdT3MKN7OB7Pp_6ADUHGCku28-fuU4PBnO1W2HAr_zrb7O26wr_Ny4j5CHkQB95RjmwhlXUgRxZsTNvdZzC3SGkS9NVQsWjFw5InZyc2io4issZOrYdFDnd8mKip_W60yz2P891Pw4-EjH_Kc29zgOfzhYUvp-Uy4BU-TBlRGG8gxM1d6gOQiNBDCz-OvlguuddQ4H66gu5LoW6dFJOxS0rbk6BVJgofIz9Wse04fr7zXnQAAAAOXm1OnG3IKLqddDEbf0xNkD7J6spLlnxRJxXgyKNHcbmTuiJk2SQyf8RLTM8DYhHYhHpgMyY6kjvq6B9D8TBm8\",\"token_type\":\"bearer\",\"expires_in\":431999,\"login\":\"686718\",\".issued\":\"Sat, 26 Aug 2017 21:59:05 GMT\",\".expires\":\"Thu, 31 Aug 2017 21:59:05 GMT\"}"; 
	char src[10240] = { 0 };
	memcpy(src,str_src.c_str(),str_src.size());
	//snprintf(src,sizeof(str_src),"%s",str_src.c_str());
	LOG_INFO("src[%s]\n", str_src.c_str());
	char dest[10240] = { 0 };
	stCrmApi CrmConfig;
	CBusiness *pBusiness = new CBusiness(NULL);
	std::string strToken;
	int nRout = 2;
	pBusiness->UserAuthRspApp(src,dest,nRout,strToken,rds,NULL,CrmConfig);
	LOG_INFO("run success%s\n", dest);
}
#endif 

//业务处理线程
void *businessThread(void *arg)
{
	int nRet = 0;
	int nIndex = 0;
	int nRoute = 0;
	char szDest[BUFSZ];
	int len = RCVBUFSZ;
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	char szTokenTmp[APP_TOKEN_LEN + 1];
	int nModulId;
	std::string strReqApi;
	char szEndstr[28] = {0};

	CRedis cRds;
	char szAppToken[APP_TOKEN_LEN + 1];

	char *pRdsIp = NULL;
	char *pRdsPort = NULL;
	char *pRdsTimeout = NULL;
	char *pRdsPassword = NULL;
	char *pRdsIns = NULL;

	//初始化redis的IP和端口
	M_XRETURN(((pRdsIp = getenv("RDS_IP")) == NULL),
			NULL, LOG_ERR("get env RDS_IP err!"));

	M_XRETURN(((pRdsPort = getenv("RDS_PORT")) == NULL),
			NULL, LOG_ERR("get env RDS_PORT err!"));

	M_XRETURN(((pRdsTimeout = getenv("RDS_CON_TIMEOUT")) == NULL),
			NULL, LOG_ERR("get env RDS_CON_TIMEOUT err!"));

	//M_XRETURN(((pRdsPassword = getenv("RDS_PWD")) == NULL),
	//NULL, LOG_ERR("get env RDS_PWD err!"));
	pRdsPassword = getenv("RDS_PWD");
	pRdsIns = getenv("RDS_INS");

	LOG_INFO("init redis ip[%s],por[%s],time[%s],pwd[%s],ins[%s]", pRdsIp, pRdsPort, pRdsTimeout, pRdsPassword, pRdsIns);


	if(nRet = cRds.connect(pRdsIp, atoi(pRdsPort), atoi(pRdsTimeout), pRdsPassword == NULL ? "" : pRdsPassword, pRdsIns == NULL ? "" : pRdsIns))
	{
		LOG_INFO("connect reids err[%d]", nRet);
		return NULL;
	}


	char *pMsgWorkIpcBusRcvKey;
	key_t tMsgWorkIpcBusRcvKey;

	M_XRETURN(((pMsgWorkIpcBusRcvKey = getenv("MSG_WORK_IPC_BUSRCV_KEY")) == NULL),
			NULL, LOG_ERR("get MSG_WORK_IPC_BUSRCV_KEY err!"));
	sscanf (pMsgWorkIpcBusRcvKey, "%x", &tMsgWorkIpcBusRcvKey);
	CIpcMsg MsgWorkIpcBusRcv;
	MsgWorkIpcBusRcv.IpcCreMsg(tMsgWorkIpcBusRcvKey);

	char *pMsgWorkIpcComRcvKey;
	key_t tMsgWorkIpcComRcvKey;
	M_VRETURNP(((pMsgWorkIpcComRcvKey = getenv("MSG_WORK_IPC_COMRCV_KEY")) == NULL),
			LOG_ERR("get MSG_WORK_IPC_COMRCV_KEY err"));
	sscanf (pMsgWorkIpcComRcvKey, "%x", &tMsgWorkIpcComRcvKey);
	CIpcMsg MsgWorkIpcComRcv;
	MsgWorkIpcComRcv.IpcCreMsg(tMsgWorkIpcComRcvKey);


	char *pMsgWorkIpcBusSndKey;
	key_t tMsgWorkIpcBusRcVKey;
	M_XRETURN(((pMsgWorkIpcBusSndKey = getenv("MSG_WORK_IPC_BUSSND_KEY")) == NULL),
			NULL, LOG_ERR("get MSG_WORK_IPC_BUSSND_KEY err!"));
	sscanf (pMsgWorkIpcBusSndKey, "%x", &tMsgWorkIpcBusRcVKey);
	CIpcMsg MsgWorkIpcBusSnd;
	MsgWorkIpcBusSnd.IpcCreMsg(tMsgWorkIpcBusRcVKey);

	IpcMsgBuf buf;
	while(1)
	{
		LOG_INFO("start rcv msg");
		memset(szEndstr, 0, sizeof(szEndstr));
		memset(buf.mtext, 0x00, BUFSZ);
		//memset(&CrmConfig,0x00,sizeof(stCrmApi));
		buf.mtype = 0;
		stCrmApi CrmConfig;

		if ((nRet = MsgWorkIpcBusRcv.IpcRcvMsg(&buf, BUFSZ)) <= 0)
		{
			sleep(1);
			LOG_ERR("RcvMsgErr[%d]", nRet);
			continue;
		}

		memset(szMsgType, 0x00, sizeof(szMsgType));

		memcpy(szMsgType, buf.mtext, MSG_TYPE_LEN);
		LOG_INFO("消息类型[%s]", szMsgType);

		Json::FastWriter fast;
		Json::Value jsonRsp;
		char szFunName[128] = {0};
		char *sPos = NULL;
		char *ePos = NULL;
		sPos = strstr(buf.mtext + nHeadLen, TAG_APP);
		ePos = strstr(buf.mtext + nHeadLen, "?");

		char szSrcId[SORCE_ID_LEN] = { 0 };
		int nLen = 0;
		// memset(szSrcId, 0x00, sizeof(szSrcId));


		//char langue[80] = { 0 };
		char szTmp[1024] = {0};
		char *nPos = NULL;
		char szRspCode[100] = {0};
		char ostype[BUFSZ] = { 0 };

		bool bIsIos = false;

		cTinyWrok.GetTagVal(buf.mtext, (char * )TAG_HTTP11, szTmp);

		if(szTmp[0] != '\0')
		{
			//LOG_INFO("tmp[%s]", szTmp);
			nPos = strstr(szTmp, " ");
			if(nPos != NULL)
			{
				//取错误吗和错误原因
				memcpy(szRspCode, szTmp, nPos - szTmp);
				LOG_INFO("szMsgType[%s] szRspCode[%s]", szMsgType,szRspCode);
				if(szRspCode[0] == '5')
				{
					nRoute = 2;
					memset(szDest, 0x00, BUFSZ);
					memcpy(szDest, buf.mtext, nHeadLen);
					jsonRsp[TAG_RESULT] = Json::Value(szRspCode);
					cTinyWrok.MakeService2AppRspHead(szDest + strlen(szDest), 1, fast.write(jsonRsp).size(), (char *)"200", (char *)"OK");
					strcat(szDest, fast.write(jsonRsp).c_str());
					LOG_ERR("szMsgType[%s],server err!",szMsgType);
					goto lable;
				}

			}
		}

		cTinyWrok.GetTagVal(buf.mtext, (char*)TAG_USER_AGENT_HEAD, ostype);
		(cTinyWrok.FindSubstr(ostype, (char * )TAG_IPHONE_HEAD) != NULL )
			? (buf.mtext[MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN+1] = '1')
			: (buf.mtext[MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN+1] = '0');
		LOG_INFO("msgtype=[%s]ostype[%s][%c]", szMsgType,ostype, buf.mtext[MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN+1]);

		if (buf.mtext[MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN + 1] == '1')
		{
			bIsIos = true;
		}

		cTinyWrok.GetTagVal(buf.mtext, (char * )TAG_SORCE_ID_GET, szSrcId, ' ');
		LOG_DEBUG("szMsgType[%s] SrcId[%s]", szMsgType,szSrcId);
		if(szSrcId[0] == '\0')
		{
			nPos = strstr(buf.mtext, TAG_SORCE_ID_POST);
			if( nPos != NULL)   ePos = strstr(nPos, "\r\n");
			if(nPos != NULL &  ePos != NULL & ePos > nPos + strlen(TAG_SORCE_ID_POST) + strlen("\r\n"))
				memcpy(szSrcId, nPos + strlen(TAG_SORCE_ID_POST), ePos - nPos - strlen(TAG_SORCE_ID_POST));
		}
		LRTrim(szSrcId);
		//LOG_INFO("srcid[%s]", szSrcId);

		CrmConfig.strCompany = szSrcId;
		//get requst
		if(buf.mtext[MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN] == '1')
		{
			nModulId = 1;
			memcpy(szEndstr, "?", strlen("?"));
			memcpy(buf.mtext + MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN + ROUTE_LEN, szSrcId, strlen(szSrcId));
			int Ret = GetCrmConfig(CrmConfig);
			//req data
			nPos = strstr(buf.mtext, TAG_SORCE_ID_GET);
			if( nPos != NULL && szSrcId[0] != '\0' && SUCESS == Ret)
			{
				//nLen = nPos - buf.mtext;
				//去掉sorceid
				//memcpy(buf.mtext + nLen, buf.mtext + nLen + strlen(TAG_SORCE_ID_GET) + strlen(szSrcId),
				//strlen(buf.mtext) - nLen - strlen(szSrcId) - strlen(TAG_SORCE_ID_GET));
				//buf.mtext[strlen(buf.mtext) - strlen(TAG_SORCE_ID_GET) - strlen(szSrcId)] = 0 ;
			}
			else
			{
				memset(szDest, 0x00, BUFSZ);
				goto lable;
			}

			//用户名转小写
			if((sPos = cTinyWrok.FindSubstr(buf.mtext, (char *)TAG_EMAIL)))
			{
				ePos = strstr(sPos, (char *)"&");

				if(ePos > sPos)
				{
					LOG_DEBUG("szMsgType[%s] len[%d]",szMsgType, ePos - sPos);
					CvtBigToSml(sPos, ePos - sPos);
				}
			}

			memset(szTokenTmp, 0x00, sizeof(szTokenTmp));
			//memset(g_AppToken, 0x00, APP_TOKEN_LEN+1);
			memset(szAppToken, 0x00, APP_TOKEN_LEN + 1);
			cTinyWrok.GetTagVal(buf.mtext, (char * )TAG_APP_TOKEN, szTokenTmp, ' ');

			if(szTokenTmp[0] != '\0')
			{
				RTrim(szAppToken);
				strrpl(szAppToken, szTokenTmp, "%40", "@");
				LOG_INFO("szMsgType[%s] Apptoken[%s]",szMsgType, szAppToken);
			}

		}
		else if(buf.mtext[MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN] == '2')
		{
			//post requst
			GetCrmConfig(CrmConfig);
			nModulId = 2;
			memcpy(szEndstr, " HTTP", strlen(" HTTP"));
			memset(szTokenTmp, 0x00, sizeof(szTokenTmp));
			memset(szAppToken, 0x00, APP_TOKEN_LEN + 1);

			nPos = strstr(buf.mtext, TAG_APP_TOKEN_HEAD);
			if( nPos != NULL)   ePos = strstr(nPos, "\r\n");
			if(nPos != NULL &  ePos != NULL & ePos > nPos + strlen(TAG_APP_TOKEN_HEAD) + strlen("\r\n"))
				memcpy(szTokenTmp, nPos + strlen(TAG_APP_TOKEN_HEAD), ePos - nPos - strlen(TAG_APP_TOKEN_HEAD));
			//LOG_INFO("token[%s]", szTokenTmp);
			LRTrim(szTokenTmp);
			LOG_DEBUG("szMsgType[%s] pos[%s]ePos[%s]", szMsgType,nPos, ePos);
			//cTinyWrok.GetTagVal(nPos,(char* )TAG_SORCE_ID_POST, szSrcId, ' ');
			cTinyWrok.GetTagVal(buf.mtext, (char * )"content-type:", g_ContentType);
			//LOG_INFO("content-type:%s", g_ContentType);
			if(szTokenTmp[0] != '\0')
			{
				strrpl(szAppToken, szTokenTmp, "%40", "@");
				LOG_INFO("szMsgType[%s] Apptoken[%s]", szMsgType,szAppToken);
			}
			//LOG_INFO("token[%s]", szAppToken);
		}
		else
		{
			nModulId = 3;
			memcpy(szEndstr, "|HTTP", strlen("|HTTP"));
			memset(szSrcId, 0x00, sizeof(szSrcId));
			memcpy(szSrcId, buf.mtext + MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN + ROUTE_LEN, SORCE_ID_LEN);
			RTrim(szSrcId);
			LOG_INFO("szMsgType[%s] SRCID[%s]",szMsgType, szSrcId);
			CrmConfig.strCompany = szSrcId;
			GetCrmConfig(CrmConfig);
		}

		//get requst method
		if((nRet = getApiByReq(buf.mtext, strReqApi, szEndstr, nModulId)) == -1)
		{
			memset(szDest, 0x00, BUFSZ);
			LOG_ERR("szMsgType[%s] fun is null, index[%d]", szMsgType,nIndex);
			goto lable;
		}

		LOG_DEBUG("strIosMaintenanceflag[%s] strMaintenanceflag[%s]", CrmConfig.strIosMaintenanceflag.c_str(), CrmConfig.strMaintenanceflag.c_str());
		//处理维护模式
		if (bIsIos)
		{
			LOG_DEBUG("is ios");
			if (CrmConfig.strIosMaintenanceflag == "1")
			{
				nRoute = 2;
				memset(szDest, 0x00, BUFSZ);
				memcpy(szDest, buf.mtext, nHeadLen);
				jsonRsp[TAG_RESULT] = Json::Value(CrmConfig.strIosMaintenancecode);
				jsonRsp[TAG_DESCCN] = Json::Value(CrmConfig.strIosMaintenancedes_cn);
				jsonRsp[TAG_DESCEN] = Json::Value(CrmConfig.strIosMaintenancedes_en);
				cTinyWrok.MakeService2AppRspHead(szDest + strlen(szDest), 1, fast.write(jsonRsp).size(), (char *)"200", (char *)"OK");
				strcat(szDest, fast.write(jsonRsp).c_str());
				goto lable;
			}
		}
		else
		{
			LOG_DEBUG("is anr");
			if (CrmConfig.strMaintenanceflag == "1")
			{
				nRoute = 2;
				memset(szDest, 0x00, BUFSZ);
				memcpy(szDest, buf.mtext, nHeadLen);
				jsonRsp[TAG_RESULT] = Json::Value(CrmConfig.strMaintenancecode);
				jsonRsp[TAG_DESCCN] = Json::Value(CrmConfig.strMaintenancedes_cn);
				jsonRsp[TAG_DESCEN] = Json::Value(CrmConfig.strMaintenancedes_en);
				cTinyWrok.MakeService2AppRspHead(szDest + strlen(szDest), 1, fast.write(jsonRsp).size(), (char *)"200", (char *)"OK");
				strcat(szDest, fast.write(jsonRsp).c_str());
				goto lable;
			}
		}


		memset(szDest, 0x00, BUFSZ);
		//去掉URL中末尾的/
		formatConfig(CrmConfig);
		if(g_pcdispatch->do_work(strReqApi, buf.mtext, szDest, nRoute, szAppToken, cRds, szSrcId, CrmConfig) == FAILE)
		{
			memset(szDest, 0x00, BUFSZ);
			memcpy(szDest, buf.mtext, nHeadLen);
			LOG_ERR("szMsgType[%s] api [%s]",szMsgType, strReqApi.c_str());
			jsonRsp[TAG_RESULT] = Json::Value("500");
			cTinyWrok.MakeService2AppRspHead(szDest + strlen(szDest), 1, fast.write(jsonRsp).size(), (char *)"200", (char *)"OK");
			strcat(szDest, fast.write(jsonRsp).c_str());

			nRoute = 2;
		}


lable:
		if(szDest[0] != '\0')
		{
			memset(buf.mtext, 0x00, BUFSZ);
			buf.mtype = (unsigned long )atol(szMsgType);
			//IpcMsgBuf sharebuf;
			//sharebuf.mtype = 25;
			//memset(sharebuf.mtext,0x00,BUFSZ);
			LOG_DEBUG("msgtype[%ld]Route[%d]", buf.mtype, nRoute);
			time_t t = time(NULL);
			if(nRoute  == 1)
			{
#if 0

				// srcid
				if ((nPos = strstr(szDest, TAG_EMAIL)) != NULL || (nPos = strstr(szDest, TAG_MT4ACC_HIS)) != NULL 
					|| (nPos = strstr(szDest, TAG_MT4ACC)) != NULL || (nPos = strstr(szDest, TAG_USERNAME)) != NULL )
				{
					while(*nPos && *nPos != '=' && *nPos != ':')
						nPos++;

					if(*nPos == '=' || *nPos == ':')
						nPos++;

					int nLen = strlen(szDest);
					int nLeft = (nLen - strlen(szSrcId)) - (nPos - szDest);
					char ltmp[4096] = {0};
					memcpy(ltmp, nPos + strlen(szSrcId), nLeft);
					LOG_DEBUG("nPos[%s] idlen[%d] left len[%d]", nPos, strlen(szSrcId), nLeft);
					memcpy(nPos , ltmp, nLeft);
					szDest[nLen - strlen(szSrcId)] = 0 ;
				}
#endif
				DelSrcId(szDest, szSrcId);


				memcpy(buf.mtext, szDest, strlen(szDest));

				M_XCONTINUE((MsgWorkIpcBusSnd.IpcSndMsg(buf, strlen(buf.mtext)) < 0),
						LOG_ERR("szMsgType[%s] Req buf err!",szMsgType));
				LOG_DEBUG("sendbuf[%s]", buf.mtext);


			}
			else if(nRoute == 2)
			{
				//rsp to app
				std::string strRsp;
				std::string strReq;
				//strrpl(szEmail, szTmp, "%40", "@");

				memcpy(buf.mtext, szDest, strlen(szDest));

				M_XCONTINUE((MsgWorkIpcComRcv.IpcSndMsg(buf, strlen(buf.mtext)) < 0),
						LOG_ERR("szMsgType[%s] Rsp buf err!",szMsgType));
			}
			else if(nRoute == 3)
			{
				LOG_ERR("szMsgType[%s] not rsp",szMsgType);
			}
		}
		else
		{
			//特殊处理
			memset(buf.mtext + nHeadLen, 0x00, BUFSZ - nHeadLen);
			jsonRsp[TAG_RESULT] = Json::Value("500");
			cTinyWrok.MakeService2AppRspHead(buf.mtext + nHeadLen, 1, fast.write(jsonRsp).size(), (char *)"200", (char *)"OK");
			strcat(buf.mtext + nHeadLen, fast.write(jsonRsp).c_str());

			M_XCONTINUE((MsgWorkIpcComRcv.IpcSndMsg(buf, strlen(buf.mtext)) < 0),
					LOG_ERR("szMsgType[%s] Rsp buf err!",szMsgType));
			LOG_ERR("szMsgType[%s] unpack err!",szMsgType);
		}

	}
}

CBusiness::CBusiness(class CDispatch *pDispatch) : m_Dispatch(pDispatch)
{
	Init();
}

#if 0
int CBusiness::Registercb(busifun cbfun)
{
	return 0;
}
#endif

int CBusiness::Do_work(std::string strApi, char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	std::map<std::string, busifun>::iterator iter;
	//it = m_Apimap.find(strApi);
	//if(it != m_Apimap.end())
	for(iter = m_Apimap.begin(); iter != m_Apimap.end(); ++iter)
	{
		if(strApi.find(iter->first) != strApi.npos)
		{
			LOG_INFO("szMsgType[%s] API[%s]", szMsgType,iter->first.c_str());
			//pbusifun = it->second;
			(this->*(iter->second))(src, dest, nRoute, strToken, pRds, szSrcId, CrmConfig);
			return SUCESS;
		}
	}

	LOG_ERR("szMsgType[%s] api[%s], exec faile",szMsgType, strApi.c_str());
	return FAILE;

}



int CBusiness::UserLoginReqCrm(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	//LOG_INFO("szMsgType=%s",szMsgType);
	nRoute = 1;
	char szEmail[CRM_USER_LEN + 1] = {0};
	char szTmpPwd[CRM_PWD_LEN + 3] = {0};
	char szPwd[CRM_PWD_LEN + 1] = {0};
	char szMac[MAC_LEN + 1] = {0};
	char szTmp[CRM_USER_LEN + 3] = {0};
	char szLang[36] = { 0 };
	char szIp[64] = { 0 };
	char *ePos;
	bool bHasIp = false;
	ePos = strstr(src + 5 + nHeadLen, " ");
	if(ePos != NULL) *ePos = '\0';

	cTinyWrok.GetTagVal(src, (char * )TAG_EMAIL, szTmp, '\0', CRM_USER_LEN);
	cTinyWrok.GetTagVal(src, (char * )TAG_PWD, szTmpPwd, '\0', CRM_PWD_LEN + 3);
	cTinyWrok.GetTagVal(src, (char * )TAG_MAC, szMac, '\0', MAC_LEN);
	cTinyWrok.GetTagVal(src, (char * )TAG_LANG, szLang, '\0', 36);

	if (szIp[0] != '\0')
	{
		LOG_DEBUG("ip = %s", szIp);
		bHasIp = true;
	}

	if(szTmp[0] == '\0' || szTmpPwd[0] == '\0' || szMac[0] == '\0' )
	{
		LOG_ERR("szMsgType[%s] bad requst Tmp[%s], tmppwd[%s], mac[%s]",szMsgType, szTmp, szTmpPwd, szMac);
		return FAILE;
	}
	//LOG_INFO("headlen[%d]src[%s]", nHeadLen, src);

	strrpl(szEmail, szTmp, "%40", "@");
	strrpl(szPwd, szTmpPwd, "%3D", "=");
	memset(dest, 0x00, BUFSZ);
	memcpy(dest, src, nHeadLen);

	memcpy(dest + MSG_TYPE_LEN, szMac, strlen(szMac));

	memcpy(dest + MSG_TYPE_LEN + MAC_LEN, szEmail, strlen(szEmail));

	memcpy(dest + MSG_TYPE_LEN + MAC_LEN + CRM_USER_LEN, szTmpPwd, strlen(szTmpPwd));

	memcpy(dest + MSG_TYPE_LEN + MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN + ROUTE_LEN, szSrcId, strlen(szSrcId));
	LOG_INFO("szMsgType[%s] UserLoginReqCrm user[%s],mac[%s], srcid[%s]",szMsgType, szEmail, szMac, szSrcId);

	if(strlen(dest) < sizeof(MsgHead))
		memset(dest + strlen(dest), ' ', sizeof(MsgHead) - strlen(dest));

	//LOG_DEBUG("dst[%s]", dest);
	if (g_cusermanage.FindUser(szEmail, szSrcId) == FAILE)
	{
		nRoute = 2;
		Json::FastWriter fast;
		Json::Value jsonRsp;
		jsonRsp[TAG_RESULT] = Json::Value(ERR_LIMIT_USER);
		cTinyWrok.MakeService2AppRspHead(dest + strlen(dest), 1, fast.write(jsonRsp).size(), (char *)"200", (char *)"OK");
		strcat(dest, fast.write(jsonRsp).c_str());
		LOG_ERR("szMsgType[%s] limit[%s]  can not login",szMsgType, szEmail);
		return SUCESS;
	}


	char TmpMerchantkey[120] = { 0 } ;
	std::string Merchantkey;
	std::string AppIp;
	std::string Domain ;
	if(src[MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN+1] == '1')
	{
		AppIp = CrmConfig.strIOS_AppIp;
		Domain = CrmConfig.strIOS_DoMain;
		Merchantkey = CrmConfig.strIOS_Merchantkey;

	}
	else
	{
		AppIp = CrmConfig.strAppIp;
		Domain = CrmConfig.strDoMain;
		Merchantkey = CrmConfig.strMerchantKey;
	}
	Base64_Encode(TmpMerchantkey, Merchantkey.c_str(),Merchantkey.size());
	//LOG_INFO("Merchantkey[%s]",Merchantkey);
	strcat(dest, "URL:");
	strcat(dest, CrmConfig.strUrlCrmMy.c_str());
	strcat(dest, USER_LOGIN_CRM);
	strcat(dest, "|");
	memcpy(dest + strlen(dest), "SNDTAG:", strlen("SNDTAG:"));
	sprintf(dest + strlen(dest), "%s%s&%s%s&%slang:%s&%s%s&%s%s%s&%s%s", TAG_EMAIL, szEmail, TAG_PWD, szPwd, TAG_LANG, szLang ,\
			TAG_MERCHANTKEY_GET,TmpMerchantkey,TAG_DOMAIN_GET,TAG_DOMAIN_POST, \
			Domain.c_str(),TAG_APPIP_GET,AppIp.c_str());

	LOG_DEBUG("dest[%s]", dest);

	return SUCESS;
}

int CBusiness::UserLoginRspApp(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	nRoute = 2;
	char szTmp[4096] = {0};
	char szTmpDest[4096] = {0};
	char szRspCode[10] = {0};
	char szRspMsg[128] = {0};
	char szMac[MAC_LEN + 1] = {0};
	char szCrmUser[CRM_USER_LEN + 1] = {0};
	//char szSrcId[SORCE_ID_LEN + 1] = {0};
	char szCrmPwd[CRM_PWD_LEN + 1] = {0};
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	char szMsgHead[nHeadLen + 1];
	char *nPos = NULL;
	int  nLen = 0 ;
	long lupTime = 0; //保存最后更新时间
	char szCrmToken[CRM_TOKEN_LEN + 1];

	memset(dest, 0x00, strlen(dest));
	memset(szMsgHead, 0x00, sizeof(szMsgHead));
	memset(szCrmToken, 0x00, strlen(szCrmToken));
	//memcpy(dest, src, MSG_TYPE_LEN+SPLIT_LEN);
	memcpy(dest, src, nHeadLen);
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	memcpy(szMsgHead, src, nHeadLen);
	LOG_INFO("szMsgType=%s",szMsgType);
	if(memcmp(szMsgType, MSG_CRM_TOKEN_EXTENDED, strlen(MSG_CRM_TOKEN_EXTENDED)) == 0)  nRoute = 3;
	cTinyWrok.GetTagVal(src, (char * )TAG_HTTP11, szTmp);
	if(szTmp[0] != '\0')
	{
		nPos = strstr(szTmp, " ");
		if(nPos != NULL)
		{
			//取错误吗和错误原因
			memcpy(szRspCode, szTmp, nPos - szTmp);
			memcpy(szRspMsg, nPos + 1, strlen(szTmp) + szTmp - nPos - 1);
			LOG_ERR("szMsgType[%s] szRspCode[%s],szRspMsg[%s]nPos[%s]",szMsgType, szRspCode, szRspMsg, nPos);
		}
		else
		{
			Json::FastWriter rsperrfast;
			Json::Value jsonErrRsp;
			jsonErrRsp[TAG_RESULT] = Json::Value(500);
			cTinyWrok.MakeService2AppRspHead(dest + strlen(dest), 1, rsperrfast.write(jsonErrRsp).size(), (char *)"200", (char *)"OK");
			strcat(dest, rsperrfast.write(jsonErrRsp).c_str());
			LOG_ERR("szMsgType[%s] unpack err",szMsgType);
			return FAILE;
		}

	}
	else
	{
		Json::FastWriter rsperrfast;
		Json::Value jsonErrRsp;
		jsonErrRsp[TAG_RESULT] = Json::Value(500);
		cTinyWrok.MakeService2AppRspHead(dest + strlen(dest), 1, rsperrfast.write(jsonErrRsp).size(), (char *)"200", (char *)"OK");
		strcat(dest, rsperrfast.write(jsonErrRsp).c_str());
		LOG_ERR("szMsgType[%s] unpack err",szMsgType);
		return FAILE;
	}

	Json::FastWriter fast;
	Json::Value root;
	Json::Value jsonappToken;
	Json::Value loginRsp;
	memcpy(szMac, src + MSG_TYPE_LEN, MAC_LEN);

	//memcpy(szCrmUser,src+MSG_TYPE_LEN+MAC_LEN+CRM_USER_LEN+CRM_PWD_LEN+MT4_USER_LEN+MT4_PWD_LEN+ROUTE_LEN, SORCE_ID_LEN);
	//RTrim(szCrmUser);
	memcpy(szCrmUser, src + MSG_TYPE_LEN + MAC_LEN, CRM_USER_LEN);
	RTrim(szCrmUser);
	char szId[SORCE_ID_LEN + CRM_USER_LEN] = {0};
	memcpy(szId, src + MSG_TYPE_LEN + MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN + ROUTE_LEN, SORCE_ID_LEN);
	RTrim(szId);
	memcpy(szId + strlen(szId), szCrmUser, strlen(szCrmUser));
	memcpy(szCrmPwd, src + MSG_TYPE_LEN + MAC_LEN + CRM_USER_LEN, CRM_PWD_LEN);
	RTrim(szCrmPwd);
	nPos = cTinyWrok.FindSubstr(src, (char * )TAG_HTTP11);
	if (nPos != NULL)
	{
		if((nPos = cTinyWrok.FindSubstr(src, (char * )"\r\n\r\n") ) != NULL)
		{
			//strrpl(szTmp, nPos, "\\u63d0\\u6b3e", "withdraw");
			//strrpl(szTmpDest, szTmp, "\\u5165\\u91d1", "deposit");
			//LOG_DEBUG("tmp[%s]tmdest[%s]", szTmp, szTmpDest);
			if(memcmp(szRspCode, "200", strlen("200")) == 0)
			{
				//应答成功,取MT4账号,取login token,生成token,
				std::string strCrmtoken;
				std::string strCrmtokenType;
				std::string strAppToken;
				std::string strIsUser;
				//char  szMt4Acc[20][30];
				std::string strMt4Acc[20];
				std::string strWith;
				std::string strDepos;

				LOG_DEBUG("nPos[%s]", nPos);

				Json::Reader reader;
				Json::Value value;

				if(reader.parse(nPos, value))
				{
					M_XRETURN((value["token_type"].isNull()), false, LOG_ERR("get token_type err"));
					M_XRETURN((value["token"].isNull()), false, LOG_ERR("get token err"));
					M_XRETURN((value["issuer"].isNull()), false, LOG_ERR("get issuser err"));

					strCrmtokenType = value["token_type"].asString();
					strCrmtoken = value["token"].asString();
					memcpy(szCrmToken, strCrmtoken.c_str(), strCrmtoken.size());
					strIsUser = value["issuer"].asString();
					LOG_DEBUG("szMsgType[%s] token_type[%s], token[%s], issuer[%s]",szMsgType,
							strCrmtokenType.c_str(), strCrmtoken.c_str(), strIsUser.c_str());
				}

				if(nRoute == 3)
				{

					//std::string strRdsCrm = pRds.get(szId);
					LOG_DEBUG("szMsgType[%s] ID[%s]",szMsgType, szCrmUser);
					std::string strRdsCrm = pRds.get(szCrmUser);
					Json::Reader crmreader;
					Json::Value crmvalue;
					//Json::FastWriter fast;

					if(strRdsCrm != "")
					{
						if(crmreader.parse(strRdsCrm, crmvalue))
						{
							crmvalue.removeMember("crm_token");
							crmvalue["crm_token"] = Json::Value(strCrmtoken);
							pRds.set(szCrmUser, fast.write(crmvalue));
							//pRds.set(szId, fast.write(crmvalue));
							//pRds.set(szId, fast.write(crmvalue),gOnlineDelayTimeOut);
							LOG_DEBUG("szMsgType[%s] crmvalue[%s]",szMsgType, fast.write(crmvalue).c_str());

							if(!crmvalue["app_token"].isNull())
							{
								std::string strapptoken;
								Json::Reader apptokenreader;
								Json::Value apptokenvalue;
								strapptoken = crmvalue["app_token"].asString();
								LOG_DEBUG("szMsgType[%s] APPTOKEN[%s]",szMsgType, strapptoken.c_str());
								std::string strRdsApptoken = pRds.get(strapptoken);
								if(apptokenreader.parse(strRdsApptoken, apptokenvalue))
								{
									apptokenvalue.removeMember("crmtokenlasttime");
									apptokenvalue["crmtokenlasttime"] = Json::Value(ltos(get_tm_sec()));
									//LOG_INFO("update apptoken");
									//pRds.set(strapptoken, fast.write(apptokenvalue));
									pRds.set(strapptoken, fast.write(apptokenvalue), gOnlineDelayTimeOut);
									LOG_DEBUG("szMsgType[%s] apptokenvalue[%s]",szMsgType, fast.write(apptokenvalue).c_str());
								}
							}

							LOG_INFO("szMsgType[%s] modify token",szMsgType);
						}
					}
					LOG_INFO("szMsgType[%s] crm token extend",szMsgType);
					return SUCESS;
				}

				const Json::Value arrayMt4Acc = value["mt4accounts"];
				//LOG_INFO("arrsz[%d]", arrayMt4Acc.size());
				for (unsigned int i = 0; i < arrayMt4Acc.size(); i++)
				{
					strMt4Acc[i] = arrayMt4Acc[i].asString();
					root["mt4accounts"].append(strMt4Acc[i]);
					loginRsp["mt4accounts"].append(strMt4Acc[i]);
					jsonappToken["mt4accounts"].append(strMt4Acc[i]);
					//LOG_INFO("MT4[%s]", strMt4Acc[i].c_str());
				}

				std::string strDepositUrl;
				std::string strDepositmethod;
				const Json::Value arrayNumDeposit = value["webview"]["deposit"];
				//LOG_INFO("arrsz[%d]\n", arrayNumDeposit.size());
				strDepositUrl = arrayNumDeposit["url"].asString();
				strDepositmethod = arrayNumDeposit["method"].asString();

				std::string strWithdrawUrl;
				std::string strWithdrawmethod;
				const Json::Value arrayNumWithdraw = value["webview"]["withdraw"];
				//LOG_INFO("arrsz[%d]\n", arrayNumWithdraw.size());
				strWithdrawUrl = arrayNumWithdraw["url"].asString();
				strWithdrawmethod = arrayNumWithdraw["method"].asString();

				LOG_DEBUG("szMsgType[%s] Mac[%s], strDepositUrl[%s], strDepositmethod[%s], strWithdrawUrl[%s], strWithdrawmethod[%s]",
						szMsgType,szMac, strDepositUrl.c_str(), strDepositmethod.c_str(), strWithdrawUrl.c_str(), strWithdrawmethod.c_str());


				char szAppToken[APP_TOKEN_LEN + 1] = {0};
				CreateToken(szAppToken, szCrmUser);

				root["crm_token_type"] = Json::Value(strCrmtokenType);
				root["crm_token"] = Json::Value(strCrmtoken);
				root["app_token"] = Json::Value(szAppToken);
				root["issuer"] = Json::Value(strIsUser);
				root["deposit"] = Json::Value(strDepositUrl);
				root["DepostMethod"] = Json::Value(strDepositmethod);
				root["withdraw"] = Json::Value(strWithdrawUrl);
				root["Withdrawmethod"] = Json::Value(strWithdrawmethod);
				root["macadress"] = Json::Value(szMac);
				//
				loginRsp["app_token"] = Json::Value(szAppToken);
				//登陆完成
				root["userstatus"] = Json::Value("3");

				(strstr(strIsUser.c_str(), TAG_MY)) != NULL ?
					root["bouser"] = Json::Value(CrmConfig.strUrlBoMy + "/") :
					root["bouser"] = Json::Value(CrmConfig.strUrlBoHy + "/");
				loginRsp["bouser"] = root["bouser"];
				//root["bouser"]= Json::Value(CrmConfig.strUrlBoMy.c_str()):
				//root["bouser"]= Json::Value(CrmConfig.strUrlBoHy.c_str());

				//root["updatetime"]= Json::Value(ltos(lupTime));
				LOG_INFO("szMsgType[%s]%s",szMsgType, fast.write(root).c_str());

				int nMt4Ver = 0;
				std::string strRdsMac = pRds.get(szCrmUser);
				if (strRdsMac != "")    LOG_INFO("szMsgType[%s] crmuser[%s]value[%s]",szMsgType, szCrmUser, strRdsMac.c_str());
				//保存至redis
				pRds.set(szCrmUser, fast.write(root));
				//pRds.set(szCrmUser,fast.write(root),gOnlineDelayTimeOut);

				//root.removeMember("mt4accounts");MSG_CRM_TOKEN_EXTENDED
				if(strRdsMac != "" )
				{
					//不是第一次登陆
					//取MAC判断是否之前的手机登陆
					if(reader.parse(strRdsMac, value))
					{
						if(!value["macadress"].isNull())
						{
							//删除旧token
							std::string strRdsMac = value["macadress"].asString();
							std::string strOldToken = value["app_token"].asString();
							if(strOldToken != "") pRds.del(strOldToken);
							if(strRdsMac == szMac)
							{
								//取MA4账号去验证密码
								//"username=2542688&password=china12345&grant_type=password"
								//成功返回MT4账号
								if(!value["mt4accounts"].isNull())
								{
									for (unsigned int n = 0; n < arrayMt4Acc.size(); n++)
									{
										//root["mt4accounts"].append(strMt4Acc[i]);
										std::string strMt4Pwd;
										std::string strMt4Token;
										std::string strType;
										std::string strRdsMt4Val = pRds.get(strMt4Acc[n]);
										cTinyWrok.GetJsonOneTagVal(strRdsMt4Val, "mt4passwork", strMt4Pwd);

										LOG_INFO("szMsgType[%s] MT4[%s]",szMsgType, strMt4Acc[n].c_str());
										if(strMt4Pwd != "")
										{
											char *pos = NULL;
											int flag;
											if (strIsUser != "")
												pos = strstr((char *)strIsUser.c_str(), (char *)TAG_MY);
											flag = (pos != NULL);
											std::string bourl = flag ? CrmConfig.strUrlBoMy : CrmConfig.strUrlBoHy;
											bourl += "/";
											//先删除老bug
											cTinyWrok.GetJsonOneTagVal(strRdsMt4Val, "mt4token", strMt4Token);
											cTinyWrok.GetJsonOneTagVal(strRdsMt4Val, "mt4tokentype", strType);
											//sendToDelMT4(strType, strMt4Token, bourl);
											char szTmp[512] = {0};
											IpcMsgBuf ipcSynBuf;

											cTinyWrok.MakeMT4AuthReq((char *)strMt4Acc[n].c_str(), strMt4Pwd, szTmp, bourl);

											char *pMsgWorkIpcBusSynKey;
											key_t tMsgWorkIpcBusSynKey;
											M_XRETURN(((pMsgWorkIpcBusSynKey = getenv("MSG_WORK_IPC_BUSSYN_KEY")) == NULL),
													false, LOG_ERR("get MSG_WORK_IPC_BUSSYN_KEY err!"));
											sscanf (pMsgWorkIpcBusSynKey, "%x", &tMsgWorkIpcBusSynKey);
											CIpcMsg MsgWorkIpcBusSyn;
											MsgWorkIpcBusSyn.IpcCreMsg(tMsgWorkIpcBusSynKey);


											memset(ipcSynBuf.mtext, 0x00, BUFSZ);
											sprintf(ipcSynBuf.mtext, "%s%s", szMsgHead, szTmp);
											ipcSynBuf.mtext[MSG_TYPE_LEN + MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN] = '2';
											//LOG_INFO("buf[%s]len[%d]", ipcSynBuf.mtext, strlen(ipcSynBuf.mtext));

											ipcSynBuf.mtype = atol(szMsgType);
											//gRspMsg->IpcSndMsg(ipcSynBuf, strlen(ipcSynBuf.mtext));

											char *pMsgWorkIpcBusSndKey;
											key_t tMsgWorkIpcBusRcVKey;
											M_XRETURN(((pMsgWorkIpcBusSndKey = getenv("MSG_WORK_IPC_BUSSND_KEY")) == NULL),
													false, LOG_ERR("get MSG_WORK_IPC_BUSSND_KEY err!"));
											sscanf (pMsgWorkIpcBusSndKey, "%x", &tMsgWorkIpcBusRcVKey);
											CIpcMsg MsgWorkIpcBusSnd;
											MsgWorkIpcBusSnd.IpcCreMsg(tMsgWorkIpcBusRcVKey);

											//g_pMsgWorkIpcBusSnd->IpcSndMsg(ipcSynBuf, strlen(ipcSynBuf.mtext));
											MsgWorkIpcBusSnd.IpcSndMsg(ipcSynBuf, strlen(ipcSynBuf.mtext));

											//LOG_INFO("msgType[%ld]",  ipcSynBuf.mtype );
											memset(ipcSynBuf.mtext, 0x00, BUFSZ);
											ipcSynBuf.mtype = atol(szMsgType);
											//g_pSynRcvMsg->IpcRcvMsg(&ipcSynBuf, BUFSZ);
											//g_pMsgWorkIpcBusSyn->IpcRcvMsg(&ipcSynBuf, BUFSZ);
											// set timer

											MsgWorkIpcBusSyn.IpcRcvMsg(&ipcSynBuf, BUFSZ);
											//LOG_INFO("rcv[%s]", ipcSynBuf.mtext);


											std::string strFirstLine;
											char *ePos = strstr(ipcSynBuf.mtext, "\r\n\r\n");
											char *sPos = strstr(ipcSynBuf.mtext, TAG_HTTPOK);
											if(ePos > sPos && ePos != NULL && sPos != NULL)
											{
												std::string token;
												std::string tokentype;
												//验证成功
												nMt4Ver = 1;
												//返回MT4账号，token
												root["mt4"]["mt4veracct"].append(strMt4Acc[n]);
												cTinyWrok.GetMT4AuthRspVal(strstr(ipcSynBuf.mtext, "\r\n\r\n"), token, tokentype);
												root["mt4"]["mt4token"].append(token);
												root["mt4"]["mt4tokentype"].append(tokentype);


												loginRsp["mt4"]["mt4veracct"].append(strMt4Acc[n]);
												loginRsp["mt4"]["mt4token"].append(token);
												loginRsp["mt4"]["mt4tokentype"].append(tokentype);
												Json::Value mt4Val;
												mt4Val["mt4passwork"] = Json::Value(strMt4Pwd);
												mt4Val["mt4tokentype"] = Json::Value(tokentype);
												mt4Val["mt4token"] = Json::Value(token);
												//LOG_DEBUG("%s", fast.write(mt4Val).c_str());
												strRtrim(strMt4Acc[n]);
												strLtrim(strMt4Acc[n]);
												pRds.set(strMt4Acc[n], fast.write(mt4Val));
												//pRds.set(strMt4Acc[n], fast.write(mt4Val),gOnlineDelayTimeOut);

											}
										}
									}
								}

							}
							else
							{
								LOG_INFO("szMsgType[%s] old mac[%s]",szMsgType, strRdsMac.c_str());
								for (unsigned int n = 0; n < arrayMt4Acc.size(); n++)
								{
									pRds.del(strMt4Acc[n]);
								}
							}

						}

					}
				}

				//token保存了最后更新时间,账号归属等信息

				lupTime = get_tm_sec();
				//LOG_INFO("AppToken[%s]uptime[%ld]", szAppToken, lupTime);

				jsonappToken["updatetime"] = Json::Value(ltos(lupTime));
				jsonappToken["crmtokenlasttime"] = Json::Value(ltos(lupTime));
				jsonappToken["crmuser"] = Json::Value(szCrmUser);
				jsonappToken["crmpwd"] = Json::Value(szCrmPwd);
				jsonappToken["issuer"] = Json::Value(strIsUser);

				RTrim(szAppToken);
				//pRds.set(szAppToken, fast.write(jsonappToken));
				pRds.set(szAppToken, fast.write(jsonappToken), gOnlineDelayTimeOut);
				//LOG_INFO("rdsapptoken[%s]", fast.write(jsonappToken).c_str());
				//LOG_INFO("CNA1[%s]", rds.ExecCmd(szCrmUser, "GET").c_str());

				//判断CRM用户是否存在
				//pRds.get(szCrmUser);
				//应答报文
				nMt4Ver ?
					root["mt4verification"] = Json::Value("1") :
					root["mt4verification"] = Json::Value("0");
				//root["mt4verification"]= Json::Value("1");

				loginRsp["mt4verification"] = root["mt4verification"];
				root.removeMember("macadress");
				root.removeMember("updatetime");
				LOG_DEBUG("szMsgType[%s] %s",szMsgType, fast.write(root).c_str());


			}
			else
			{
				Json::FastWriter rsperrfast;
				Json::Reader read;
				Json::Value jsonErrRsp;
				read.parse(nPos, jsonErrRsp);
				jsonErrRsp[TAG_RESULT] = Json::Value(szRspCode);
				cTinyWrok.MakeService2AppRspHead(dest + strlen(dest), 1, fast.write(jsonErrRsp).size(), (char *)"200", (char *)"OK");
				strcat(dest, fast.write(jsonErrRsp).c_str());
				LOG_INFO("szMsgType[%s] dest[%s]",szMsgType, dest);
				return SUCESS;
			}
		}
		else
		{
			Json::FastWriter rsperrfast;
			Json::Value jsonErrRsp;
			jsonErrRsp[TAG_RESULT] = Json::Value(500);
			cTinyWrok.MakeService2AppRspHead(dest + strlen(dest), 1, fast.write(jsonErrRsp).size(), (char *)"200", (char *)"OK");
			strcat(dest, fast.write(jsonErrRsp).c_str());
			LOG_ERR("szMsgType[%s] unpack err",szMsgType);
			return FAILE;
		}
	}
	else
	{
		Json::FastWriter rsperrfast;
		Json::Value jsonErrRsp;
		jsonErrRsp[TAG_RESULT] = Json::Value(500);
		cTinyWrok.MakeService2AppRspHead(dest + strlen(dest), 1, fast.write(jsonErrRsp).size(), (char *)"200", (char *)"OK");
		strcat(dest, fast.write(jsonErrRsp).c_str());
		LOG_ERR("szMsgType[%s] unpack err",szMsgType);
		return FAILE;
	}

	//LOG_INFO("UserLoginRspApp user[%s]login sucess", szCrmUser);

	//cTinyWrok.MakeCrm2AppRspHead(src, dest + strlen(dest), 1, strlen(fast.write(root).c_str()));
	//strcat(dest + strlen(dest), fast.write(root).c_str());
	cTinyWrok.MakeCrm2AppRspHead(src, dest + strlen(dest), 1, strlen(fast.write(loginRsp).c_str()));
	strcat(dest + strlen(dest), fast.write(loginRsp).c_str());
	LOG_INFO("szMsgType[%s] buf[%s]",szMsgType, fast.write(loginRsp).c_str());
	return SUCESS;
}

int CBusiness::UserRegisterReqCrm(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	//LOG_INFO("szMsgType=%s",szMsgType);
	LOG_INFO("src[%s]",src);
	nRoute = 1;
	int nIpType = -1;
	char *pIpPos = NULL;

	char szIp[64] = { 0 };
	char szTmpIp[64] = { 0 };

	cTinyWrok.GetTagVal(src, (char *)TAG_HIS_IP, szIp, ' ', 64);
	if (szIp[0] == '\0')
	{
		pIpPos = strstr(src, (char *)TAG_HIS_IP);
		if (NULL == pIpPos)
		{
			//没有这个字段 直接添加
			nIpType = 0;
		}
		else
		{
			nIpType = 1;
			//有ip字段，但是空值。直接替换一个。
		}
	}
	else
	{
		LOG_DEBUG("ip = %s", szIp);
		nIpType = 2;
		//有字段，并且有值，替换
	}

	getNginxRealIp(src, szTmpIp);

	if (szTmpIp[0] == '\0')
	{
		LOG_DEBUG("not get ip from nignx head! defult ip = %s", (char*)DEAULT_APP_IP);
		strcpy(szTmpIp, (char*)DEAULT_APP_IP);
	}

	LOG_DEBUG("client ip = %s", szTmpIp);

	memcpy(dest, src, nHeadLen);
	strcat(dest, "URL:");
	strcat(dest, CrmConfig.strUrlCrmMy.c_str());
	strcat(dest, USER_REGISTER_CRM);
	strcat(dest, "|");
	//memcpy(dest + strlen(dest), "SNDTAG:", strlen("SNDTAG:"));
	std::string Merchantkey ;
	char TmpMerchantkey[120] = { 0 };
	std::string AppIp;
	std::string Domain ;
	if(src[MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN+1] == '1')
	{
		AppIp = CrmConfig.strIOS_AppIp;
		Domain = CrmConfig.strIOS_DoMain;
		Merchantkey = CrmConfig.strIOS_Merchantkey;
	}
	else
	{
		AppIp = CrmConfig.strAppIp;
		Domain = CrmConfig.strDoMain;
		Merchantkey = CrmConfig.strMerchantKey;

	}
	Base64_Encode(TmpMerchantkey,Merchantkey.c_str(), Merchantkey.size());
	snprintf(dest+strlen(dest),sizeof(dest+strlen(dest)),"SNDTAG:%s%s&%s%s%s&%s%s",TAG_MERCHANTKEY_GET,\
			TmpMerchantkey,TAG_DOMAIN_GET,TAG_DOMAIN_POST, \
			Domain.c_str(),TAG_APPIP_GET,AppIp.c_str());

	char *sPos = NULL;
	char *ePos = NULL;
	sPos = strstr(src, "?");
	ePos = strstr(src, TAG_HTTP);
	if(sPos == NULL || ePos == NULL  || ePos - sPos - 2 <= 0)
	{
		LOG_ERR("szMsgType[%s] bad requst",szMsgType);
		return FAILE;
	}

	char szTmp[1024] = {0};
	char szTmp2[1024] = {0};
	char szTmp3[1024] = { 0 };
	memcpy(szTmp, sPos + 1, ePos - sPos - 2);

	if (nIpType == 0)
	{
		snprintf(szTmp3, sizeof(szTmp3), "%s&%s%s", szTmp, (char *)TAG_HIS_IP, szTmpIp);
		strrpl(szTmp2, szTmp3, "%3D", "=");
		LOG_DEBUG("Tmp[%s]", szTmp2);
	}
	else if (nIpType == 2)
	{
		//算了  自己解算了
		pIpPos = NULL;
		pIpPos = strstr(szTmp, (char*)TAG_HIS_IP);
		char *pE = pIpPos + strlen((char*)TAG_HIS_IP) + strlen(szIp);
		if (*pE == '\0')
		{
			LOG_DEBUG("ip is last! just copy");
			memcpy(pIpPos + strlen((char*)TAG_HIS_IP), szTmpIp, strlen(szTmpIp));
		}
		else
		{
			char szLast[1024] = { 0 };
			//保存尾部数据
			strcpy(szLast, pE);
			pIpPos = pIpPos + strlen((char*)TAG_HIS_IP);
			//拷贝到当前位置后面。
			memcpy(pIpPos, szTmpIp, strlen(szTmpIp));
			//指针后移
			pIpPos += strlen(szTmpIp);
			//保存的数据拼接回来
			memcpy(pIpPos, szLast, strlen(szLast));
		}
		strrpl(szTmp2, szTmp, "%3D", "=");
		LOG_DEBUG("Tmp[%s]", szTmp2);
	}
	else
	{
		pIpPos = NULL;
		pIpPos = strstr(szTmp, (char *)TAG_HIS_IP);
		pIpPos += strlen((char *)TAG_HIS_IP);
		if (*pIpPos == '\0')
		{
			//最后一个 直接拷贝到后面就好了。
			LOG_DEBUG("it is last params,just copy");
			memcpy(pIpPos, szTmpIp, sizeof(szTmpIp));
		}
		else
		{
			//后面还有参数，拷贝然后拼接
			char szLast[1024] = {0};//保存后面的参数
			strcpy(szLast,pIpPos);
			LOG_DEBUG("last=%s||", szLast);
			//拷贝到当前位置后面。
			memcpy(pIpPos, szTmpIp, strlen(szTmpIp));
			//指针后移
			pIpPos += strlen(szTmpIp);
			//保存的数据拼接回来
			memcpy(pIpPos, szLast, strlen(szLast));	
		}
		strrpl(szTmp2, szTmp, "%3D", "=");
		LOG_DEBUG("Tmp[%s]", szTmp2);
	}

	strrpl(dest + strlen(dest), szTmp2, "%40", "@");

	LOG_INFO("dest[%s]", dest);

	//LOG_INFO("UserRegisterReqCrm srcid[%s]", szSrcId);
	return SUCESS;
}

int CBusiness::UserRegisterRspApp(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	nRoute = 2;
	char szTmp[1024] = {0};
	char *nPos = NULL;

	memset(szTmp, 0x00, sizeof(szTmp));
	memset(dest, 0x00, strlen(dest));
	char szRspCode[5] = {0};
	char szRspMsg[128] = {0};

	//memcpy(dest, src, MSG_TYPE_LEN+SPLIT_LEN);
	memcpy(dest, src, nHeadLen);

	cTinyWrok.GetTagVal(src, (char * )TAG_HTTP11, szTmp);
	//LOG_INFO("tmp[%s]", szTmp);
	if(szTmp[0] != '\0')
	{
		nPos = strstr(szTmp, " ");
		if(nPos != NULL)
		{
			//取错误码和错误原因
			memcpy(szRspCode, szTmp, nPos - szTmp);
			memcpy(szRspMsg, nPos + 1, strlen(szTmp) + szTmp - nPos - 1);
			LOG_ERR("szMsgType[%s] szRspCode[%s],szRspMsg[%s]nPos[%s]",szMsgType, szRspCode, szRspMsg, nPos);
		}
		else
		{
			LOG_ERR("szMsgType[%s] Get retcode err",szMsgType);
			return FAILE;
		}
	}
	else
	{
		LOG_ERR("szMsgType[%s] Get Tag[%s] err",szMsgType, TAG_HTTP11);
		return FAILE;
	}
	Json::Value crmRsp;
	if(strstr(szRspCode, "200"))
	{
		nPos = cTinyWrok.FindSubstr(src, (char * )TAG_HTTP11);
		if (nPos != NULL)
		{
			if((nPos = cTinyWrok.FindSubstr(src, (char * )"\r\n\r\n") ) != NULL)
			{
				cTinyWrok.MakeCrm2AppRspHead(src, dest + strlen(dest), 1, strlen(nPos));
				strcat(dest + strlen(dest), nPos);
			}
			else
			{
				LOG_ERR("szMsgType[%s] 解包错误",szMsgType);
				return FAILE;
			}
		}
		else
		{
			LOG_ERR("szMsgType[%s] 报文解析错误!",szMsgType);
			return FAILE;
		}
	}
	else
	{
		std::string errCode;
		if((nPos = cTinyWrok.FindSubstr(src, (char * )"\r\n\r\n") ) != NULL)
		{
			LOG_DEBUG("szMsgType[%s] nPos[%s]",szMsgType, nPos);
			Json::Reader reader;

			if(reader.parse(nPos, crmRsp))
			{
				//提取错误信息
				const Json::Value arrayMsg = crmRsp["msg"];
#if 0
				for (unsigned int i = 0; i < arrayMsg.size(); i++)
				{
					Json::Value msg = arrayMsg[i];
					Json::Value body = msg["body"];

					for (unsigned int j = 0; j < body.size(); j++)
					{
						errCode += body[i].asString();
						break;
					}
				}
#endif
			}

		}
		crmRsp[TAG_RESULT] = szRspCode;
		LOG_ERR("szMsgType[%s] ERRCODE[%s]",szMsgType, errCode.c_str());
		//Json::Value jsonRsp = makeErrJson(__FUNCTION__, szRspCode, errCode.c_str());
		Json::FastWriter fast;
		//cTinyWrok.MakeService2AppRspHead(dest+strlen(dest), 1, fast.write(jsonRsp).size(), (char*)"200", (char*)"OK");
		//strcat(dest, fast.write(jsonRsp).c_str());
		cTinyWrok.MakeService2AppRspHead(dest + strlen(dest), 1, fast.write(crmRsp).size(), (char *)"200", (char *)"OK");
		strcat(dest, fast.write(crmRsp).c_str());
	}
	LOG_INFO("szMsgType[%s] UserRegisterRspApp srcid[%s] retcode[%s] retRsp[%s] sucess dest[%s]",szMsgType, szSrcId, szRspCode, szRspMsg, dest);

}

int CBusiness::UserForPwdReqCrm(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	LOG_INFO("src[%s]",src);
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	//LOG_INFO("szMsgType=%s",szMsgType);
	nRoute = 1;
	//memcpy(dest, src, MSG_TYPE_LEN+SPLIT_LEN);
	memcpy(dest, src, nHeadLen);
	strcat(dest, "URL:");
	char szSrc[2049] = { 0 };
	char *bPos = strstr(src, TAG_HTTP);
	memcpy(szSrc, src, bPos - src - 1);
	strrpl(szSrc, szSrc, "%26", "&");
	char szTmp[256] = {0};
	char szEmail[256] = {0};
	char szIp[64] = { 0 };
	cTinyWrok.GetTagVal(szSrc, (char * )TAG_EMAIL, szEmail);
	char szLang[32] = { 0 };
	cTinyWrok.GetTagVal(szSrc, (char *)TAG_LANG, szLang);
	char *nPos = NULL;
	if((nPos = strstr(szEmail, " ")) != NULL) * nPos = '\0';

	if(szEmail[0] == '\0')
	{
		//LOG_INFO("bad req");
		LOG_ERR("szMsgType[%s] bad req %s[%s]",szMsgType, TAG_EMAIL, szEmail);
		return FAILE;
	}

	getNginxRealIp(szSrc, szIp);

	if (szIp[0] == '\0')
	{
		LOG_DEBUG("not get ip from nignx head! defult ip = %s", (char*)DEAULT_APP_IP);
		strcpy(szIp, (char*)DEAULT_APP_IP);
	}
	//检查访问频率：
	struct timeval tTimeVal;
	gettimeofday(&tTimeVal, NULL);

	std::string strIpInfo = pRds.get(szIp);
	LOG_DEBUG("ip:%s,info:%s", szIp, strIpInfo.c_str());


	char * szTime = getenv("FOR_PWD_TIME");;
	char * szTimes = getenv("FOR_PWD_TIMES");
	char * szDisableTime = getenv("FOR_PWD_DISABLE_TIME");
	if(szTime == NULL || szTimes == NULL || szDisableTime == NULL)
	{
		LOG_ERR("err parameter");
		return FAILE;
	}
	LOG_INFO("time[%s],times[%s],disabletime[%s]", szTime, szTimes,szDisableTime);

	if (!strIpInfo.empty())
	{
		Json::Reader reader;
		Json::Value  value;
		if (reader.parse(strIpInfo.c_str(), value))
		{
			int nLimit = value["limit"].asInt();
			//被限制啦
			if (nLimit == 1)
			{
				LOG_ERR("ip limit!ip:%s,info:%s", szIp, strIpInfo.c_str());
				return FAILE;
			}
			//还没被限制就算算是不是该限制了。
			std::string strTime = value["time"].asString();
			std::string strLimitTime = value["limittime"].asString();
			int nCount = value["count"].asInt();

			long nTime = strtol(strTime.c_str(), NULL, 10);
			long nLimitTime = strtol(strLimitTime.c_str(), NULL, 10);
			//开始算，一分钟内访问超过五次，五分钟内不让访问
			long nTmp = tTimeVal.tv_sec - nTime; //距离上次访问的时间
			LOG_DEBUG(" time! old time:%ld, now time : %ld,tmp:%d", nTime, tTimeVal.tv_sec, nTmp);
			if (nTmp <= atoi(szTime))
			{
				LOG_DEBUG("less %s,count:%d", szTime, nCount);
				//没有超过一分钟
				if (nCount + 1 > atoi(szTimes))
				{
					Json::FastWriter wt;
					//访问次数超过五次，限制，设置限制时间
					char szBuf[64] = { 0 };
					sprintf(szBuf, "%ld", tTimeVal.tv_sec);
					value["limittime"] = szBuf; //跟新限制时间
					value["limit"] = 1;
					value["count"] = ++nCount;
					std::string tmp = wt.write(value);
					pRds.set(szIp, tmp, atoi(szDisableTime));
					return FAILE;
				}
				else
				{
					Json::FastWriter wt;
					//访问次数超过五次，限制，设置限制时间
					value["limittime"] = "0"; //跟新限制时间
					value["limit"] = 0;
					value["count"] = ++nCount;
					std::string tmp = wt.write(value);
					pRds.set(szIp, tmp, atoi(szDisableTime));
				}
			}
			//超过60s 次数清零 重新搞
			else if(nTmp > atoi(szTime))
			{
				LOG_DEBUG("more 60,count:%d", nCount);
				Json::FastWriter wt;
				char szBuf[64] = { 0 };
				sprintf(szBuf, "%ld", tTimeVal.tv_sec);
				value["limittime"] = "0";
				value["limit"] = 0;
				value["count"] = 1;
				value["time"] = szBuf; //跟新计算开始时间
				std::string tmp = wt.write(value);
				pRds.set(szIp, tmp, atoi(szDisableTime));
			}
		}
		else
		{
			pRds.del(szIp);
			LOG_ERR("bad ip limit info. info:%s", strIpInfo.c_str());
		}
	}
	else
	{
		Json::Value value;
		Json::FastWriter wt;
		char szBuf[64] = { 0 };
		sprintf(szBuf, "%ld", tTimeVal.tv_sec);
		value["limittime"] = "0";
		value["limit"] = 0;
		value["count"] = 1;
		value["time"] = szBuf; //跟新计算开始时间
		std::string tmp = wt.write(value);
		pRds.set(szIp, tmp, 200);
	}

	//LOG_INFO("UserForPwdReqCrm %s[%s] srcid[%s]", TAG_EMAIL, szEmail, szSrcId);
	std::string strIsUser;
	strrpl(szTmp, szEmail, "%40", "@");
	std::string strRdsCrm = pRds.get(szTmp);
	cTinyWrok.GetJsonOneTagVal(strRdsCrm, "issuer", strIsUser);
	if (strIsUser != "")
	{
		strcat(dest, strIsUser.c_str());
		LOG_DEBUG("szMsgType[%s] isuser[%s]",szMsgType, strIsUser.c_str());
	}
	else
	{
		//strcat(dest, g_Url_Crm_My);
		strcat(dest, CrmConfig.strUrlCrmMy.c_str());
	}
	char TmpMerchantkey[120] = { 0 };
	std::string Merchantkey ;
	std::string AppIp;
	std::string Domain ;
	if(src[MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN+1] == '1')
	{
		AppIp = CrmConfig.strIOS_AppIp;
		Domain = CrmConfig.strIOS_DoMain;
		Merchantkey = CrmConfig.strIOS_Merchantkey;
	}
	else
	{
		AppIp = CrmConfig.strAppIp;
		Domain = CrmConfig.strDoMain;
		Merchantkey = CrmConfig.strMerchantKey;
	}
	Base64_Encode(TmpMerchantkey,Merchantkey.c_str(), Merchantkey.size());
	strcat(dest, USER_FORPWD_CRM);
	strcat(dest, "|");
	memcpy(dest + strlen(dest), "SNDTAG:", strlen("SNDTAG:"));
	memcpy(dest + strlen(dest), TAG_EMAIL, strlen(TAG_EMAIL));
	strrpl(dest + strlen(dest), szEmail, "%40", "@");
	sprintf(dest+strlen(dest),"&%s%s%s&%s%s&%s%s%s&%s%s",TAG_LANG,TAG_LANGUAGE_HEAD_CRM,szLang,TAG_MERCHANTKEY_GET,TmpMerchantkey ,TAG_DOMAIN_GET,TAG_DOMAIN_POST, \
			Domain.c_str(),TAG_APPIP_GET,AppIp.c_str());
	LOG_INFO("dest[%s]", dest);
	return SUCESS;
}

int CBusiness::UserForPwdRspApp(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	nRoute = 2;
	char szTmp[1024] = {0};
	char *nPos = NULL;

	memset(szTmp, 0x00, sizeof(szTmp));
	memset(dest, 0x00, strlen(dest));
	char szRspCode[5] = {0};
	char szRspMsg[128] = {0};

	//memcpy(dest, src, MSG_TYPE_LEN+SPLIT_LEN);
	memcpy(dest, src, nHeadLen);

	cTinyWrok.GetTagVal(src, (char * )TAG_HTTP11, szTmp);
	if(szTmp[0] != '\0')
	{
		LOG_DEBUG("szMsgType[%s] tmp[%s]",szMsgType, szTmp);
		nPos = strstr(szTmp, " ");
		if(nPos != NULL)
		{
			//取错误吗和错误原因
			memcpy(szRspCode, szTmp, nPos - szTmp);
			memcpy(szRspMsg, nPos + 1, strlen(szTmp) + szTmp - nPos - 1);
			LOG_DEBUG("szMsgType[%s] szRspCode[%s],szRspMsg[%s]nPos[%s]",szMsgType, szRspCode, szRspMsg, nPos);
		}
		else
		{
			LOG_ERR("szMsgType[%s] Get retcode err",szMsgType);
			return FAILE;
		}

	}
	else
	{
		LOG_ERR("szMsgType[%s] Get Tag[%s] err",szMsgType, TAG_HTTP11);
		return FAILE;
	}

	Json::FastWriter fast;
	Json::Value jsonRsp;
	jsonRsp[TAG_RESULT] = Json::Value(szRspCode);
	cTinyWrok.MakeService2AppRspHead(dest + strlen(dest), 1, fast.write(jsonRsp).size(), (char *)"200", (char *)"OK");
	strcat(dest, fast.write(jsonRsp).c_str());

	LOG_DEBUG("szMsgType[%s] UserForPwdRspApp srcid[%s] sucess",szMsgType, szSrcId);

	LOG_INFO("buf[%s]", dest);
	return SUCESS;
}

int CBusiness::UserTradehistoryRspApp(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	LOG_DEBUG("szMsgType[%s] UserTradehistoryRspApp srcid[%s]",szMsgType, szSrcId);
	nRoute = 2;
	char szRspCode[10] = {0};
	char szRspMsg[128] = {0};
	char *nPos = NULL;
	int  nLen = 0;

	memset(dest, 0x00, strlen(dest));

	nPos = strstr(src, "\r\n\r\n");
	memcpy(dest, src, nHeadLen);
	if (nPos != NULL)
	{
		cTinyWrok.MakeService2AppRspHead(dest + strlen(dest), 1, strlen(nPos) - 4, (char *)"200", (char *)"OK");
		strcat(dest, nPos + 4);
	}
	else
	{
		LOG_ERR("szMsgType[%s] 报文解析错误!",szMsgType);
		return FAILE;
	}

	LOG_INFO("buf[%s]", dest);
	return SUCESS;
}

int CBusiness::UserTradehistoryReqCrm(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	//LOG_INFO("szMsgType=%s",szMsgType);
	LOG_INFO("src[%s]",src);
	nRoute = 1;
	memcpy(dest, src, nHeadLen);

	char szTmp[2048] = {0};
	char szFirstLine[2048] = {0};
	char szMt4Acc[56] = {0};
	char szPageLen[10] = {0};
	char szPage[10] = {0};
	char szSort[10] = {0};
	long lTime;
	std::string strCrmUser;
	std::string strlupTime;
	std::string strIsUser;
	std::string strMt4User;
	std::string strMt4TokenType;
	std::string strMt4TokenTmp;
	std::string strMt4Token;
	std::string strRdsAppToken;
	Json::Reader reader;
	Json::Value value;

	std::string rdsCrmUser;
	std::string strCrmTokenType;
	std::string strCrmTokenTmp;
	std::string strCrmToken;
	std::string Merchantkey ;
	std::string AppIp;
	std::string Domain ;
	char TmpMerchantkey[120] = { 0 };
	char *nPos = strstr(src, TAG_HTTP);
	memcpy(szFirstLine, src, nPos - src - 1);
	//memcpy(szTmp, src, nPos - src - 1);
	LOG_DEBUG("szMsgType[%s] szFirstLine[%s]",szMsgType, szTmp);
	strrpl(szTmp, szFirstLine, "%26", "&");

	cTinyWrok.GetTagVal(szTmp, (char * )TAG_PAGE_LEN, szPageLen);
	cTinyWrok.GetTagVal(szTmp, (char * )TAG_PAGE, szPage);
	cTinyWrok.GetTagVal(szTmp, (char * )TAG_SORT, szSort);
	cTinyWrok.GetTagVal(szTmp, (char * )TAG_MT4ACC, szMt4Acc);

	//if((nPos = strstr(g_AppToken, " ")) != NULL) *nPos = '\0';

	//RTrim(g_AppToken);
	LOG_DEBUG("szMsgType[%s] UserTradehistoryReqCrm token[%s], pagelen[%s], page[%s], sort[%s] srcid[%s]",szMsgType, strToken.c_str(), szPageLen, szPage, szSort, szSrcId);

	//if(g_AppToken[0] == '\0' || szPage[0] == '\0' ||
	if(strToken == "" || szPage[0] == '\0' ||
			szPageLen[0] == '\0' || szSort[0] == '\0' || szMt4Acc[0] == '\0')
	{
		LOG_ERR("szMsgType[%s] 请求错误无法解析",szMsgType);

		return FAILE;
	}

	strRdsAppToken = pRds.get(strToken);
	if(strRdsAppToken != "")
	{
		cTinyWrok.GetJsonOneTagVal(strRdsAppToken, "crmuser", strCrmUser);
		cTinyWrok.GetJsonOneTagVal(strRdsAppToken, "updatetime", strlupTime);
		cTinyWrok.GetJsonOneTagVal(strRdsAppToken, "issuer", strIsUser);
		strcat(dest, "URL:");
		if(strstr(strIsUser.c_str(), TAG_MY))
		{
			//strcat(dest, g_Url_Crm_My);
			strcat(dest, CrmConfig.strUrlCrmMy.c_str());
			LOG_DEBUG("szMsgType[%s] 用户归属[%s]",szMsgType, TAG_MY);
		}
		else
		{
			//strcat(dest, g_Url_Crm_Hy);
			strcat(dest, CrmConfig.strUrlCrmHy.c_str());
			LOG_DEBUG("szMsgType[%s] 用户归属[%s]",szMsgType, TAG_MY);
		}
	}
	else
	{
		nRoute = 2 ;
		Json::Value crm;
		std::string strCrmUserLen = strToken.substr(0, CRM_USER_LEN_LEN);
		strCrmUser = strToken.substr(CRM_USER_LEN_LEN, atoi(strCrmUserLen.c_str()));
		std::string strRdsCrm = pRds.get(strCrmUser);
		if(reader.parse(strRdsCrm, crm))
		{
			if(!crm["userstatus"].isNull())
			{
				std::string strUserStatus = crm["userstatus"].asString();
				std::string strAppToken = crm["app_token"].asString();
				LOG_DEBUG("szMsgType[%s], user status[%s]",szMsgType, strUserStatus.c_str());

				if(strUserStatus == "2" )
				{
					cTinyWrok.MakeService2AppRspErr(dest, ERR_OFFLINE, "user offline");
					goto LABLE;
				}
				else if(strUserStatus == "1" || strUserStatus == "3")
				{
					if(strAppToken == strToken)
					{
						cTinyWrok.MakeService2AppRspErr(dest, ERR_OFFLINE, "user offline");
						goto LABLE;
					}
					else
					{
						cTinyWrok.MakeService2AppRspErr(dest, ERR_LOGIN_OTHMACHINE, "user login on other phone");
						goto LABLE;
					}

				}

			}
		}
		cTinyWrok.MakeService2AppRspErr(dest, ERR_OFFLINE, "user login on other phone");
		goto LABLE;
	}


	lTime = get_tm_sec();

	LOG_DEBUG("取出来的时间[%s], 现在的时间[%ld]", strlupTime.c_str(), lTime);

	if( strRdsAppToken == "" && lTime - atol(strlupTime.c_str()) > gOnlineDelayTimeOut )
	{
		nRoute = 2;
		cTinyWrok.MakeService2AppRspErr(dest, ERR_LOGIN_OTHMACHINE, "user login on other phone");
		LOG_ERR("szMsgType[%s] user offline [%s]",szMsgType, strToken.c_str());
		goto LABLE;
	}

	rdsCrmUser = pRds.get(strCrmUser);
	strCrmTokenType;
	strCrmTokenTmp;
	strCrmToken;

	if(rdsCrmUser != "")
	{
		cTinyWrok.GetJsonOneTagVal(rdsCrmUser, "crm_token", strCrmTokenTmp);
		cTinyWrok.GetJsonOneTagVal(rdsCrmUser, "crm_token_type", strCrmTokenType);
		strCrmToken += "Authorization:";
		strCrmToken += strCrmTokenType;
		strCrmToken += " ";
		strCrmToken += strCrmTokenTmp;
		LOG_DEBUG("mt4token[%s]", strCrmToken.c_str());
	}
	else
	{
		LOG_ERR("szMsgType[%s] 获取token失败",szMsgType);
		return FAILE;
	}
	LOG_DEBUG("szMsgType[%s] CRMUSER[%s], luptime[%ld], isuser[%s]",szMsgType, strCrmUser.c_str(), strlupTime.c_str(), strIsUser.c_str());

	strcat(dest, USER_GETTRDHIS__CRM);
	strcat(dest, "?");
	LOG_DEBUG("szMsgType[%s], buf[%s]",szMsgType, dest);
	sprintf(dest + strlen(dest), "%s%s&%s%s&%s%s|%s%smt4account:%s&%s%s",
			TAG_SORT, szSort, TAG_PAGE, szPage, TAG_PAGE_LEN, szPageLen, TAG_SND, TAG_MT4ACC, szMt4Acc, TAG_CRM_TOKEN, strCrmToken.c_str());

	if(src[MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN+1] == '1')
	{
		AppIp = CrmConfig.strIOS_AppIp;
		Domain = CrmConfig.strIOS_DoMain;
		Merchantkey = CrmConfig.strIOS_Merchantkey;
	}
	else
	{
		AppIp = CrmConfig.strAppIp;
		Domain = CrmConfig.strDoMain;
		Merchantkey = CrmConfig.strMerchantKey;
	}
	Base64_Encode(TmpMerchantkey,Merchantkey.c_str(), Merchantkey.size());
	sprintf(dest+strlen(dest),"&%s%s&%s%s%s&%s%s",TAG_MERCHANTKEY_GET,TmpMerchantkey ,TAG_DOMAIN_GET,TAG_DOMAIN_POST, \
			Domain.c_str(),TAG_APPIP_GET,AppIp.c_str());
LABLE:
	LOG_INFO("buf[%s]", dest);

	return SUCESS;
}

int CBusiness::UserHeartRspApp(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	nRoute = 2;
	LOG_INFO("src[%s]",src);
	char *nPos = NULL;
	int  nRet = 0 ;
	long lTime = 0;
	std::string strAppVer;

	memset(dest, 0x00, strlen(dest));
	cTinyWrok.FindSubstr(src, (char * )TAG_IPHONE) != NULL ?
		strAppVer = CrmConfig.strAppIosVersion :
		strAppVer = CrmConfig.strAppAdrVersion ;
	//if(g_AppToken[0] == '\0')
	if(strToken == "")
	{
		LOG_ERR("szMsgType[%s] bad req",szMsgType);
		return FAILE;
	}


	memcpy(dest, src, nHeadLen);
	nRet = nCheckOnline(strToken, pRds, this, szSrcId, CrmConfig);
	switch(nRet)
	{
		case 0:
			cTinyWrok.MakeService2AppRspErr(dest, "0", "sucess", strAppVer.c_str());
			break;
		case -1:
			cTinyWrok.MakeService2AppRspErr(dest, ERR_OFFLINE, "user offline");
			break;
		case -2:
			cTinyWrok.MakeService2AppRspErr(dest, ERR_LOGIN_OTHMACHINE, "user login on other phone");
			break;

	}

	if(nRet != 0)
	{
		pRds.del(strToken);
	}

	LOG_INFO("heart Rsp [%s]",dest);

	return SUCESS;
}

int CBusiness::UserhistoryReqCrm(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	//LOG_INFO("szMsgType=%s",szMsgType);
	LOG_INFO("src[%s]",src);
	memcpy(dest, src, nHeadLen);

	long lTime = 0;
	char szCount[256] = {0};
	char szStart[256] = {0};
	char szEnd[256] = {0};
	char szTmpStart[256] = {0};
	char szTmpEnd[256] = {0};
	char szTimeFrame[64] = {0};
	char szCurrExchange[64] = {0};
	char szDest[BUFSZ] = {0} ;

	//char szAppToken[APP_TOKEN_LEN+1];

	//memset(szAppToken, 0x00, sizeof(szAppToken));

	nRoute = 1;
	lTime = get_tm_usec();

	Json::Value value;
	Json::Reader reader;
	std::string strCrmUser;
	std::string strlupTime;
	std::string strIsUser;
	std::string strMt4User;
	std::string strMt4TokenType;
	std::string strMt4TokenTmp;
	std::string strMt4Token;
	char *timeout;
	char *nPos = NULL;

	//cTinyWrok.GetTagVal(src, (char* )TAG_APP_TOKEN, szAppToken, ' ');
	if(strToken.empty() || szSrcId == NULL)
	{
		nRoute = 2;
		LOG_ERR("szMsgType[%s] 请求错误无法解析",szMsgType);
		return FAILE;
	}
	//RTrim(g_AppToken);
	std::string strRdsAppToken = pRds.get(strToken);
	lTime = get_tm_sec();

	if(strRdsAppToken != "")
	{
		cTinyWrok.GetJsonOneTagVal(strRdsAppToken, "crmuser", strCrmUser);
		cTinyWrok.GetJsonOneTagVal(strRdsAppToken, "updatetime", strlupTime);
		cTinyWrok.GetJsonOneTagVal(strRdsAppToken, "issuer", strIsUser);
	}
	else
	{
		nRoute = 2;
		Json::Value crm;
		std::string strCrmUserLen = strToken.substr(0, CRM_USER_LEN_LEN);
		strCrmUser = strToken.substr(CRM_USER_LEN_LEN, atoi(strCrmUserLen.c_str()));
		std::string strRdsCrm = pRds.get(strCrmUser);
		if(reader.parse(strRdsCrm, crm))
		{
			if(!crm["userstatus"].isNull())
			{
				std::string strUserStatus = crm["userstatus"].asString();
				std::string strAppToken = crm["app_token"].asString();
				LOG_INFO("szMsgType[%s] user status[%s]",szMsgType, strUserStatus.c_str());

				if(strUserStatus == "2" )
				{
					cTinyWrok.MakeService2AppRspErr(dest, ERR_OFFLINE, "user offline");
				}
				else if(strUserStatus == "1" || strUserStatus == "3")
				{
					if(strAppToken == strToken)
					{
						cTinyWrok.MakeService2AppRspErr(dest, ERR_OFFLINE, "user offline");
					}
					else
					{
						cTinyWrok.MakeService2AppRspErr(dest, ERR_LOGIN_OTHMACHINE, "user login on other phone");
					}

				}

			}
		}
		//cTinyWrok.MakeService2AppRspErr(dest, ERR_OFFLINE, "user offline");
		LOG_INFO("szMsgType[%s] srcid[%s], token[%s] user offline",szMsgType, szSrcId, strToken.c_str());
		goto LABLE;
		//return FAILE;
	}
	LOG_DEBUG("szMsgType[%s] CRMUSER[%s] luptime[%s] timeout[%ld], isuser[%s] currtime[%ld] srcId[%s] token[%s]",
			szMsgType,strCrmUser.c_str(), strlupTime.c_str(), gOnlineTimeOut, strIsUser.c_str(), lTime, szSrcId, strToken.c_str());

	//LOG_INFO("buf[%s]", dest);

	if(reader.parse(strRdsAppToken, value))
	{
		const Json::Value arrayMt4Acc = value["mt4accounts"];
		LOG_DEBUG("arrsz[%d]", arrayMt4Acc.size());
		for (unsigned int i = 0; i < arrayMt4Acc.size(); i++)
		{
			LOG_DEBUG("szMsgType[%s], strMt4[%s]",szMsgType, arrayMt4Acc[i].asString().c_str());
			std::string rdsMt4 = pRds.get(arrayMt4Acc[i].asString());
			if(rdsMt4 != "")
			{

				cTinyWrok.GetJsonOneTagVal(rdsMt4, "mt4tokentype", strMt4TokenType);
				cTinyWrok.GetJsonOneTagVal(rdsMt4, "mt4token", strMt4TokenTmp);
				strMt4Token += "Authorization:";
				strMt4Token += strMt4TokenType;
				strMt4Token += " ";
				strMt4Token += strMt4TokenTmp;
				LOG_DEBUG("szMsgType[%s] mt4[%s]mt4token[%s]",szMsgType, arrayMt4Acc[i].asString().c_str(), strMt4Token.c_str());
				break;

			}
			else
			{
				if(i >= arrayMt4Acc.size() - 1)
				{
					LOG_ERR("szMsgType[%s] cant not find mt4 user",szMsgType);
					return FAILE;
				}
			}
		}
	}

	//getReq("https://mtemp.starfishfx.com/history/last/EURUSDbo?count=128&timeframe=M1", strResponse, (char*)CMD_HISDATA_BO);  //sucess
	//判断那种类型
	if(strstr(src, TAG_COUNT) != NULL)
	{
		//蜡烛条方式获取历史行情
		LOG_DEBUG("蜡烛条方式获取历史行情");
		cTinyWrok.GetTagVal(src, (char * )TAG_COUNT, szCount);
		cTinyWrok.GetTagVal(src, (char * )TAG_TIME_FRAME, szTimeFrame);
		cTinyWrok.GetTagVal(src, (char * )TAG_CURR_FRAME, szCurrExchange);

		if((nPos = strstr(szCount, " ")) != NULL) * nPos = '\0';
		if((nPos = strstr(szTimeFrame, " ")) != NULL) * nPos = '\0';
		if((nPos = strstr(szCurrExchange, " ")) != NULL) * nPos = '\0';

		LOG_DEBUG("szMsgType[%s] count[%s]timeframe[%s]currexchange[%s]apptoken[%s]srcid[%s]",szMsgType, szCount, szTimeFrame, szCurrExchange, strToken.c_str(), szSrcId);
		if(szCount[0] == '\0' || szTimeFrame[0] == '\0' || szCurrExchange[0] == '\0' || strToken.empty())
		{
			LOG_ERR("szMsgType[%s] bad req srcid[%s], token[%s]",szMsgType, szSrcId, strToken.c_str());
			return FAILE;
		}

		strcat(dest, "URL:");
		if(strstr(strIsUser.c_str(), TAG_MY))
			strcat(dest, CrmConfig.strUrlBoMy.c_str());
		else
			strcat(dest, CrmConfig.strUrlBoHy.c_str());

		strcat(dest, USER_GETHIS_COUNT_CRM);

		std::string Merchantkey ;
		std::string AppIp;
		std::string Domain ;
		char TmpMerchantkey[120] = { 0 };
		if(src[MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN+1] == '1')
		{
			AppIp = CrmConfig.strIOS_AppIp;
			Domain = CrmConfig.strIOS_DoMain;
			Merchantkey = CrmConfig.strIOS_Merchantkey;
		}
		else
		{
			AppIp = CrmConfig.strAppIp;
			Domain = CrmConfig.strDoMain;
			Merchantkey = CrmConfig.strMerchantKey;
		}
		Base64_Encode(TmpMerchantkey,Merchantkey.c_str(), Merchantkey.size());
		//sprintf(dest+strlen(dest), "%s?%s%s&%s%s",szCurrExchange,TAG_COUNT, szCount, TAG_TIME_FRAME, szTimeFrame);
		sprintf(dest + strlen(dest), "|%s%s?%s%s&%s%s|%s%s&%s%s%s&%s%s&", TAG_CURR_FRAME, szCurrExchange, TAG_COUNT, szCount, TAG_TIME_FRAME, szTimeFrame, \
				TAG_MERCHANTKEY_GET,TmpMerchantkey ,TAG_DOMAIN_GET,TAG_DOMAIN_POST, \
				Domain.c_str(),TAG_APPIP_GET,AppIp.c_str());

#if 0
		strcat(dest, "|");
		strcat(dest, TAG_CURR_FRAME);
		strcat(dest, szCurrExchange);
		strcat(dest, "&");
#endif
		//memcpy(dest+strlen(dest),"SNDTAG:", strlen("SNDTAG:"));
		memcpy(dest + strlen(dest), TAG_CRM_TOKEN, strlen(TAG_CRM_TOKEN));
		memcpy(dest + strlen(dest), strMt4Token.c_str(), strlen(strMt4Token.c_str()));

	}
	else if(strstr(src, TAG_START) != NULL)
	{
		//时间方式获取历史行情
		LOG_DEBUG("时间条方式获取历史行情");
		cTinyWrok.GetTagVal(src, (char * )TAG_START, szStart);
		cTinyWrok.GetTagVal(src, (char * )TAG_END, szEnd);
		cTinyWrok.GetTagVal(src, (char * )TAG_TIME_FRAME, szTimeFrame);
		cTinyWrok.GetTagVal(src, (char * )TAG_CURR_FRAME, szCurrExchange);
		//cTinyWrok.GetTagVal(src, (char* )TAG_APP_TOKEN, g_AppToken);

		if((nPos = strstr(szStart, " ")) != NULL) * nPos = '\0';
		if((nPos = strstr(szEnd, " ")) != NULL) * nPos = '\0';
		if((nPos = strstr(szCurrExchange, " ")) != NULL) * nPos = '\0';
		if((nPos = strstr(szTimeFrame, " ")) != NULL) * nPos = '\0';

		LOG_DEBUG("szMsgType[%s] start[%s]end[%s]timeframe[%s]currexchange[%s]",
				szMsgType,szStart, szEnd, szTimeFrame, szCurrExchange);
		//时间格式特殊处理
		strrpl(szTmpStart, szStart, "%3A", ":");
		strrpl(szTmpEnd, szEnd, "%3A", ":");
		memset(szStart, 0x00, sizeof(szStart));
		memset(szEnd, 0x00, sizeof(szTmpEnd));
		strrpl(szStart, szTmpStart, "%20", " ");
		strrpl(szEnd, szTmpEnd, "%20", " ");
		LOG_DEBUG("szMsgType[%s] start[%s]end[%s]timeframe[%s]currexchange[%s]apptoken[%s]srcid[%s]",
				szMsgType, szTmpStart, szTmpEnd, szTimeFrame, szCurrExchange, TAG_APP_TOKEN, strToken.c_str(), szSrcId);

		if(szStart[0] == '\0' || szEnd[0] == '\0'
				|| szTimeFrame[0] == '\0' || szCurrExchange[0] == '\0')
		{
			LOG_ERR("szMsgType[%s] 请求错误无法解析",szMsgType);
			return FAILE;
		}

		//redis查找数据
		Json::Value root;
		Json::Value arrayObj;
		Json::Value item;
		Json::FastWriter fast;
		std::string strRdsHis;

		int flag = FAILE ;
		stHistory stHis;
		char szTimeTmp[64] = {0};
		char szFild[64] = {0};
		char reqTimeZone[20] = {0};
		char rspTimeZone[20] = {0};

		std::string Merchantkey ;
		std::string AppIp;
		std::string Domain ;
		char TmpMerchantkey[120] = { 0 };
		memcpy(reqTimeZone, "+02:00", 6);
		memcpy(rspTimeZone, "-06:00", 6);
		memset(&stHis, 0x00, sizeof(stHis));
		gethis(szStart, szEnd, szTimeFrame, stHis);

		LOG_DEBUG("szMsgType[%s] timeframe[%s]count[%d]stime[%d],etime[%d],unit[%d]",
				szMsgType, szTimeFrame, stHis.count, stHis.sTime, stHis.eTime, stHis.unit);
#if 0
		for(int n = 0; n < stHis.count + 1; n++)
		{
			addtime(stHis.sTime, stHis.unit * n, szTimeTmp, reqTimeZone);
			sprintf(szFild, "%s,%s", szTimeFrame, szTimeTmp);
			LOG_INFO("EXCHANGE[%s]file[%s]", szCurrExchange, szFild);
			strRdsHis = pRds.hget(szCurrExchange, szFild);

			if(strRdsHis != "")
			{
				LOG_INFO("value[%s]", strRdsHis.c_str());

				if(reader.parse(strRdsHis, value))
				{
					arrayObj.append(value);

					LOG_INFO("value[%s]", fast.write(value).c_str());
					flag = SUCESS;
				}
			}
			else
				flag = FAILE;

			if(flag == FAILE) break;

		}

		if(flag == SUCESS)
		{
			root[TAG_HIS_BARS] = arrayObj;
			root[TAG_HIS_INFO][TAG_HIS_SYMBOL] = szCurrExchange;
			root[TAG_HIS_INFO][TAG_HIS_TIMEFRAME] = szTimeFrame;

			strcat(szStart, rspTimeZone);
			strcat(szEnd, rspTimeZone);
			root[TAG_HIS_INFO][TAG_HIS_START] = szStart;
			root[TAG_HIS_INFO][TAG_HIS_END] = szEnd;
			nRoute = 2;
			cTinyWrok.MakeService2AppRspHead(dest + strlen(dest), 1, fast.write(root).size(), (char *)"200", (char *)"OK");
			strcat(dest + strlen(dest), fast.write(root).c_str());

			goto LABLE;
		}
#endif
		strcat(dest, "URL:");
		if(strstr(strIsUser.c_str(), TAG_MY))
			strcat(dest, CrmConfig.strUrlBoMy.c_str());
		else
			strcat(dest, CrmConfig.strUrlBoHy.c_str());

		//LOG_INFO("buf[%s]", dest);
		strcat(dest, USER_GETHIS_TIME_CRM);
		//sprintf(dest+strlen(dest), "%s?%s%s&%s%s&%s%s",
		//szCurrExchange,TAG_START, szTmpStart, TAG_END, szTmpEnd, TAG_TIME_FRAME, szTimeFrame);

		if(src[MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN+1] == '1')
		{
			AppIp = CrmConfig.strIOS_AppIp;
			Domain = CrmConfig.strIOS_DoMain;
			Merchantkey = CrmConfig.strIOS_Merchantkey;
		}
		else
		{
			AppIp = CrmConfig.strAppIp;
			Domain = CrmConfig.strDoMain;
			Merchantkey = CrmConfig.strMerchantKey;
		}
		Base64_Encode(TmpMerchantkey,Merchantkey.c_str(), Merchantkey.size());
		sprintf(dest + strlen(dest), "|%s%s?%s%s&%s%s&%s%s|%s%s&%s%s%s&%s%s&",
				TAG_CURR_FRAME, szCurrExchange, TAG_START, szTmpStart, TAG_END, szTmpEnd, TAG_TIME_FRAME, szTimeFrame, \
				TAG_MERCHANTKEY_GET, TmpMerchantkey ,TAG_DOMAIN_GET,TAG_DOMAIN_POST, \
				Domain.c_str(),TAG_APPIP_GET,AppIp.c_str());


#if 0
		strcat(dest, "|");
		strcat(dest, TAG_CURR_FRAME);
		strcat(dest, szCurrExchange);
		strcat(dest, "&");
#endif
		//memcpy(dest+strlen(dest),"SNDTAG:", strlen("SNDTAG:"));
		memcpy(dest + strlen(dest), TAG_CRM_TOKEN, strlen(TAG_CRM_TOKEN));
		memcpy(dest + strlen(dest), strMt4Token.c_str(), strlen(strMt4Token.c_str()));
	}
	else
	{
		LOG_ERR("szMsgType[%s] bad req",szMsgType);
		return FAILE;
	}

LABLE:
	LOG_INFO("buf[%s]", dest);

	return SUCESS;
}

int CBusiness::UserhistoryRspApp(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	nRoute = 2;

	char szRspCode[10] = {0};
	char szRspMsg[128] = {0};
	char *nPos = NULL;
	int  nLen = 0 ;

	memset(dest, 0x00, strlen(dest));

	memcpy(dest, src, nHeadLen);
	nPos = cTinyWrok.FindSubstr(src, (char * )TAG_HTTP11);
	if (nPos != NULL)
	{
		strcat(dest + nHeadLen, nPos - strlen(TAG_HTTP11));
#if 0
		nPos = strstr(src, "\r\n\r\n");

		//缓存历史行情
		Json::Reader reader;
		Json::Value jshis;

		LOG_INFO("POS%s", nPos + 4);
		if(reader.parse(nPos + 4, jshis))
		{
			M_XRETURN((jshis[TAG_HIS_INFO][TAG_HIS_SYMBOL].isNull()), false, LOG_ERR("get %s err", TAG_HIS_SYMBOL));
			M_XRETURN((jshis[TAG_HIS_INFO][TAG_HIS_TIMEFRAME].isNull()), false, LOG_ERR("get %s err", TAG_HIS_TIMEFRAME));

			std::string strTimeFrame = jshis[TAG_HIS_INFO][TAG_HIS_TIMEFRAME].asString();
			std::string strHisSymbol = jshis[TAG_HIS_INFO][TAG_HIS_SYMBOL].asString();

			const Json::Value arrayHis = jshis[TAG_HIS_BARS];
			Json::FastWriter fast;
			LOG_INFO("arrsz[%d]timeframe[%s]Hissymbo[%s]", arrayHis.size(), strTimeFrame.c_str(), strHisSymbol.c_str());
			for(size_t n = 0; n < arrayHis.size(); n++)
			{

				M_XRETURN((arrayHis[n][TAG_HIS_DATETIME].isNull()), false, LOG_ERR("get %s err", TAG_HIS_SYMBOL));
				std::string strDateTime ;
				strDateTime += strTimeFrame;
				strDateTime += ",";
				strDateTime += arrayHis[n][TAG_HIS_DATETIME].asString();

				std::string strWrite = fast.write(arrayHis[n]);
				if(strWrite.length() >= 2)
					strWrite.erase(strWrite.end() - 1);

				//LOG_INFO("symbo[%s],rimeframe[%s]Write[%s]", strHisSymbol.c_str(), strTimeFrame.c_str(), strWrite.c_str());
				pRds.hset(strHisSymbol, strDateTime, strWrite);
				//rds.hset(strHisSymbol, strDateTime, strWrite, g_HistoryKey_SurviTime);
			}
			//pRds.setexpire(strHisSymbol, g_HistoryKey_SurviTime);
		}
#endif 
	}
	else
	{
		LOG_ERR("szMsgType[%s] 报文解析错误!",szMsgType);
		return FAILE;
	}


	//缓存行情数据到redis

	LOG_INFO("szMsgType[%s] UserhistoryRspApp sucess",szMsgType);
	LOG_DEBUG("buf[%s]", dest);
	return SUCESS;
}

int CBusiness::UserAuthReqCrm(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	//MT4账号验证
	//username=2542688&password=china12345&grant_type=password
	nRoute = 1;
	char szTmp[4096] = {0};
	//memcpy(dest, src, MSG_TYPE_LEN+SPLIT_LEN);
	memcpy(dest, src, nHeadLen);
	//strcat(dest, URL_BO);

	std::string strIsUser;
	char szMt4Pwd[MT4_PWD_LEN + 1];
	char szTmpMt4Pwd[MT4_PWD_LEN + 1];
	char szMt4User[MT4_USER_LEN];
	std::string Merchantkey ;
	std::string AppIp;
	std::string Domain ;
	char TmpMerchantkey[120] = { 0 };
	//char szAppToken[APP_TOKEN_LEN+1];

	memset(szMt4Pwd, 0x00, sizeof(szMt4Pwd));
	memset(szTmpMt4Pwd, 0x00, sizeof(szTmpMt4Pwd));
	memset(szMt4User, 0x00, sizeof(szMt4User));
	//memset(szAppToken, 0x00, sizeof(szAppToken));


	strrpl(szTmp, src, "%26", "&");

	cTinyWrok.GetTagVal(szTmp, (char * )TAG_USERNAME, szMt4User, ' ', MT4_USER_LEN);
	cTinyWrok.GetTagVal(szTmp, (char * )TAG_PWD, szTmpMt4Pwd, ' ', MT4_PWD_LEN);
	//cTinyWrok.GetTagVal(szTmp, (char* )TAG_APP_TOKEN, szAppToken, ' ');
	strrpl(szMt4Pwd, szTmpMt4Pwd, "%3D", "=");
	LOG_DEBUG("mt4pwd[%s]",szTmpMt4Pwd);
	LOG_INFO("szMsgType[%s] username[%s],  apptoken[%s] srcid[%s]",szMsgType, szMt4User, strToken.c_str(), szSrcId);

	if(szMt4User[0] == '\0' || szMt4Pwd == '\0' || strToken.empty())
	{
		LOG_ERR("szMsgType[%s] bad req",szMsgType);
		return FAILE;
	}

	std::string strMt4Token;
	std::string strType;
	std::string strBoUrl;
	std::string strRdsMt4Val = pRds.get(szMt4User);

	//issuer
	//RTrim(g_AppToken);
	std::string strRdsAppToken = pRds.get(strToken);
	if(strRdsAppToken != "")
	{
		cTinyWrok.GetJsonOneTagVal(strRdsAppToken, "issuer", strIsUser);
	}
	else
	{
		nRoute = 2;
		cTinyWrok.MakeService2AppRspErr(dest, ERR_LOGIN_OTHMACHINE, "user offline");
		LOG_ERR("szMsgType[%s] 获取token错误",szMsgType);
		goto LABLE;
	}

	if (strstr(strIsUser.c_str(), TAG_MY))
	{
		strBoUrl = CrmConfig.strUrlBoMy;
	}
	else
	{
		strBoUrl = CrmConfig.strUrlBoHy;
	}

	if (!strRdsMt4Val.empty())
	{
		LOG_DEBUG("need del mt4token");
		cTinyWrok.GetJsonOneTagVal(strRdsMt4Val, "mt4token", strMt4Token);
		cTinyWrok.GetJsonOneTagVal(strRdsMt4Val, "mt4tokentype", strType);
		sendToDelMT4(strType, strMt4Token, strBoUrl);
	}
	else
	{
		LOG_DEBUG("need not del mt4token");
	}

	strcat(dest, "URL:");
	strcat(dest, strBoUrl.c_str());
	strcat(dest, USER_AUTH_CRM);
	strcat(dest, "|");
	memcpy(dest + strlen(dest), "SNDTAG:", strlen("SNDTAG:"));

	memcpy(dest + MSG_TYPE_LEN + MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN, szMt4User, strlen(szMt4User));
	memcpy(dest + MSG_TYPE_LEN + MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN, szMt4Pwd, strlen(szMt4Pwd));
	if(strlen(dest) < sizeof(MsgHead))
		memset(dest + strlen(dest), ' ', sizeof(MsgHead) - strlen(dest));
	//LOG_DEBUG("dest[%s]", dest);


	sprintf(dest + strlen(dest), "grant_type=password&%s%s&%s%s", TAG_USERNAME, szMt4User, TAG_PWD, szMt4Pwd); 

	if(src[MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN+1] == '1')
	{
		AppIp = CrmConfig.strIOS_AppIp;
		Domain = CrmConfig.strIOS_DoMain;
		Merchantkey = CrmConfig.strIOS_Merchantkey;
	}
	else
	{
		AppIp = CrmConfig.strAppIp;
		Domain = CrmConfig.strDoMain;
		Merchantkey = CrmConfig.strMerchantKey;
	}
	Base64_Encode(TmpMerchantkey,Merchantkey.c_str(), Merchantkey.size());
	sprintf(dest+strlen(dest),"&%s%s&%s%s%s&%s%s",TAG_MERCHANTKEY_GET,TmpMerchantkey,TAG_DOMAIN_GET,TAG_DOMAIN_POST, \
			Domain.c_str(),TAG_APPIP_GET,AppIp.c_str());
	LOG_DEBUG("dest[%s]",dest);
LABLE:
	//LOG_INFO("dest[%s]", dest);

	return SUCESS;
}

int CBusiness::UserAuthRspApp(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	//mt4账号验证
	Json::Value value, valuebo;
	Json::FastWriter faster;
	Json::Reader reader;
	nRoute = 2;
	char szRspCode[10] = {0};
	char szRspMsg[128] = {0};
	char *nPos = NULL;
	char *ePos = NULL;
	int  nLen = 0 ;

	std::string strErrCode;
	char szMt4Pwd[MT4_PWD_LEN + 1];
	char szMt4User[MT4_USER_LEN + 1];
	char szFirstLine[128] = {0};
	memset(szMt4Pwd, 0x00, sizeof(szMt4Pwd));
	memset(dest, 0x00, strlen(dest));

	memcpy(szMt4User, src + MSG_TYPE_LEN + MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN, MT4_USER_LEN);
	RTrim(szMt4User);
	memcpy(szMt4Pwd, src + MSG_TYPE_LEN + MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN, MT4_PWD_LEN);
	RTrim(szMt4Pwd);
	//  memcpy(dest, src, MSG_TYPE_LEN+SPLIT_LEN);
	memcpy(dest, src, nHeadLen);
	//LOG_DEBUG("buf[%s]", dest);
	if((nPos = cTinyWrok.FindSubstr(src, (char * )"\r\n\r\n") ) != NULL)
	{
		if(reader.parse(nPos, valuebo))
		{
			GetHttpErrorCode(src, strErrCode);
			value[TAG_RESULT] = strErrCode;

			value[TAG_IS_NULL] = 1;
			value[TAG_DATA] = faster.write(valuebo);
			LOG_ERR("szMsgType[%s] err[%s]",szMsgType, strErrCode.c_str());
		}
		else
		{
			return FAILE;
		}

		//strcat(dest,faster.write(value).c_str());

		cTinyWrok.MakeService2AppRspHead(dest + strlen(dest), 1, strlen(faster.write(value).c_str()), (char *)"200", (char *)"OK");
		strcat(dest + strlen(dest), faster.write(value).c_str());
	}
	else
	{
		LOG_ERR("szMsgType[%s] 报文解析错误 dest[%s]!",szMsgType, dest);
		return FAILE;
	}

	//缓存MT4信息
	std::string strMt4User;
	std::string strTokenType;
	std::string strMt4Token;
	nPos = strstr(src, "\r\n\r\n");

	if(strErrCode == "200")
	{
		//成功
		cTinyWrok.GetJsonOneTagVal(nPos + strlen("\r\n") * 2, "login", strMt4User);
		cTinyWrok.GetJsonOneTagVal(nPos + strlen("\r\n") * 2, "access_token", strMt4Token);
		cTinyWrok.GetJsonOneTagVal(nPos + strlen("\r\n") * 2, "token_type", strTokenType);

		Json::Value mt4Val;
		Json::FastWriter fast;
		mt4Val["mt4passwork"] = Json::Value(szMt4Pwd);
		mt4Val["mt4tokentype"] = Json::Value(strTokenType);
		mt4Val["mt4token"] = Json::Value(strMt4Token);
		LOG_DEBUG("%s", fast.write(mt4Val).c_str());
		strRtrim(strMt4User);
		strLtrim(strMt4User);
		pRds.set(strMt4User, fast.write(mt4Val));
		//pRds.set(strMt4User, fast.write(mt4Val), gOnlineDelayTimeOut);

	}
	//memcpy(dest, src, strlen(src));
	LOG_INFO("szMsgType[%s] srcid[%s], MT4[%s] auth sucess",szMsgType, szSrcId, szMt4User);
	//LOG_INFO("buf[%s]", dest);
	return SUCESS;

}

int CBusiness::UserTmpregisterReqCrm(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	//获取用户注册模板
	LOG_INFO("src[%s]",src);
	memcpy(dest, src, nHeadLen);
	char   *pFilePath = NULL;
	FILE *fp = NULL;
	char line[40960] = {0};

	memset(line, 0x00, sizeof(line));

	if((pFilePath = getenv("REGIST_TMP_FILE_CFG")) == NULL)
	{
		LOG_ERR("szMsgType[%s] get configure err",szMsgType);
		return FAILE;
	}

	if((fp = fopen(pFilePath, "r")) != NULL )
	{
		while(fgets(line, sizeof(line), fp) != NULL)
		{
			if(line[0] == '#')
			{
				continue;
			}
			if( memcmp(line + 1, szSrcId, strlen(szSrcId)) == 0)
			{
				memset(line, 0x00, sizeof(line));
				if((fgets(line, sizeof(line), fp)) != NULL )
				{
					break;
				}
			}
		}

		nRoute = 2;
		cTinyWrok.MakeService2AppRspHead(dest + strlen(dest), 1, strlen(line), (char *)"200", (char *)"OK");
		strcat(dest, line);
		fclose(fp);
	}
	else
	{
		nRoute = 1;
		strcat(dest, "URL:");
		strcat(dest, USER_REGISTER_TMP_CRM);
	}

	//LOG_INFO("UserTmpregisterReqCrm srcid[%s] sucess", szSrcId);
	LOG_INFO("dest[%s]", dest);
	return SUCESS;
}

int CBusiness::UserTmpregisterRspApp(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	nRoute = 2;
	char *nPos = strstr(src, TAG_HTTP);

	memcpy(dest, src, nHeadLen);
	if (nPos != NULL)
	{
		strcat(dest, nPos);
	}

	LOG_DEBUG("buf[%s]", dest);

	LOG_DEBUG("get template of regist sucess");
	return SUCESS;
}

int CBusiness::UserAgreementRspApp(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	nRoute = 2;
	LOG_INFO("src[%s]",src);
	//拼接消息头
	//memcpy(dest, src, MSG_TYPE_LEN+SPLIT_LEN);
	memcpy(dest, src, nHeadLen);

	char *nPos = NULL;
	int  nLen = 0 ;

	memset(dest, 0x00, strlen(dest));

	char *szSerProEn = NULL;
	char *szSerProCn = NULL;
	szSerProEn = getenv("URL_SER_PRO_EN");
	szSerProCn = getenv("URL_SER_PRO_CN");

	if(szSerProEn == NULL || szSerProCn == NULL)
	{
		return FAILE;
	}

	Json::FastWriter fast;
	Json::Value jsonRsp;
	jsonRsp["result"] = Json::Value("0");
	jsonRsp["serproen"] = Json::Value(CrmConfig.strUrlSerProEn.c_str());
	jsonRsp["serprocn"] = Json::Value(CrmConfig.strUrlSerProCn.c_str());

	LOG_DEBUG("查询风险和服务条款应答报文[%s]", fast.write(jsonRsp).c_str());
	memcpy(dest, src, nHeadLen);
	cTinyWrok.MakeService2AppRspHead(dest + strlen(dest), 1, fast.write(jsonRsp).size(), (char *)"200", (char *)"OK");
	strcat(dest + strlen(dest), fast.write(jsonRsp).c_str());

	LOG_INFO("dest[%s]", dest);

	//LOG_DEBUG("get agreem sucess srcid[%s]", szSrcId);
	return SUCESS;
}

int CBusiness::UserawaretradingRspApp(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	return SUCESS;
}

int CBusiness::UserUploadlogRspApp(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	LOG_INFO("src[%s]",src);
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	nRoute = 2;
	char *pFilePath = NULL;
	FILE *fp = NULL;
	char szTime[256] = {0};
	char szBuf[256]  = {0};
	struct timeval tTimeVal;
	char szFile[256] = {0};

	gettimeofday(&tTimeVal, NULL);
	strftime(szBuf, sizeof (szBuf), "TIME:%Y-%m-%d_%H:%M:%S",
			localtime(&tTimeVal.tv_sec));
	snprintf(szTime, sizeof (szTime), "%s.%03ld",
			szBuf, tTimeVal.tv_usec / 1000);

	if((pFilePath = getenv("APP_ERR_LOG")) == NULL)
	{
		LOG_ERR("szMsgType[%s] get dir err",szMsgType);
		return FAILE;
	}

	cTinyWrok.GetTagVal(src, (char * )TAG_FILENAME, szFile + strlen(pFilePath), ' ');

	strncpy(szFile, pFilePath, strlen(pFilePath));

	if((fp = fopen(szFile, "a+")) != NULL)
	{
		fprintf(fp, "[%s][%s]\n", szTime, src + nHeadLen);
		fclose(fp);
	}

	Json::FastWriter fast;
	Json::Value jsonRsp;
	jsonRsp["result"] = Json::Value("0");

	LOG_DEBUG("szMsgType[%s] app err log[%s] srcid[%s]",szMsgType, fast.write(jsonRsp).c_str(), szSrcId);
	memcpy(dest, src, nHeadLen);
	cTinyWrok.MakeService2AppRspHead(dest + strlen(dest), 1, fast.write(jsonRsp).size(), (char *)"200", (char *)"OK");
	strcat(dest + strlen(dest), fast.write(jsonRsp).c_str());

	LOG_INFO("dest[%s]", dest);
}


int CBusiness::UserLogoutRspApp(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	LOG_INFO("src[%s]",src);
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	nRoute = 2;

	//拼接消息头
	//memcpy(dest, src, MSG_TYPE_LEN+SPLIT_LEN);
	memcpy(dest, src, nHeadLen);
	Json::Reader reader;
	Json::Value crm;
	Json::Value token;
	std::string strCrmUser;


	char *nPos = NULL;
	int  nLen = 0 ;
	//char szAppToken[APP_TOKEN_LEN+1] = {0};

	//memset(szAppToken, 0x00, sizeof(szAppToken));
	memset(dest, 0x00, strlen(dest));

	//cTinyWrok.GetTagVal(src, (char* )TAG_APP_TOKEN, szAppToken, ' ');
	//if(g_AppToken[0] == '\0')
	if(strToken.empty())
	{
		return FAILE;
	}

	//取token
	std::string strRdsToken = pRds.get(strToken);
	if(reader.parse(strRdsToken, token))
	{
		M_XRETURN((token["crmuser"].isNull()), false, LOG_ERR("szMsgType[%s] get use err",szMsgType));
		strCrmUser = token["crmuser"].asString();
	}

	DelMt4Token((char *)strToken.c_str(), pRds);

	//释放token
	pRds.del(strToken);
	memset(dest, 0x00, strlen(dest));

	Json::FastWriter fast;
	Json::Value jsonRsp;
	jsonRsp["result"] = Json::Value("0");

	memcpy(dest, src, nHeadLen);
	cTinyWrok.MakeService2AppRspHead(dest + strlen(dest), 1, fast.write(jsonRsp).size(), (char *)"200", (char *)"OK");
	strcat(dest + strlen(dest), fast.write(jsonRsp).c_str());

	//更新状态
	//userstatus
	std::string strRdsCrm = pRds.get(strCrmUser);
	if(reader.parse(strRdsCrm, crm))
	{
		//离线
		crm["userstatus"] = Json::Value("2");
		pRds.set(strCrmUser, fast.write(crm));
		//pRds.set(strCrmUser, fast.write(crm),gOnlineDelayTimeOut);

	}
	LOG_DEBUG("logout token[%s] srcid[%s]", strToken.c_str(), szSrcId);
	LOG_INFO("dest[%s]", dest);
	return SUCESS;
}

int CBusiness::UserAuthenticateRspApp(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	LOG_INFO("src[%s]",src);
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	nRoute = 2;

	//拼接消息头
	//memcpy(dest, src, MSG_TYPE_LEN+SPLIT_LEN);
	memset(dest, 0x00, strlen(dest));
	memcpy(dest, src, nHeadLen);

	char *nPos = NULL;
	int  nLen = 0 ;
	char AppVersion[150] = {0};
	char SymbolVersion[150] = {0};
	char SkinVersion[150] = {0};
	char IsIos[150] = {0};

	cTinyWrok.GetTagVal(src, (char * )TAG_VERSION, AppVersion, ' ');
	cTinyWrok.GetTagVal(src, (char * )TAG_SYMBOL_VER, SymbolVersion, ' ');
	cTinyWrok.GetTagVal(src, (char * )TAG_SKIN_VER, SkinVersion, ' ');
	cTinyWrok.GetTagVal(src, (char * )TAG_ISIOS, IsIos, ' ');

	LOG_DEBUG("szMsgType[%s] appVer[%s],symbolver[%s],skinversion[%s],isios[%s]",
			szMsgType,AppVersion, SymbolVersion, SkinVersion, IsIos);

	Json::Value root;
	Json::Value arrayObj;
	Json::Value item;
	Json::FastWriter fast;


	// get config according to srcid
	//stCrmApi CrmConfig ;
	//CrmConfig.strCompany = szSrcId;
	//GetCrmConfig(CrmConfig);
	LOG_DEBUG("szMsgType[%s] mid version!,iosmidversion[%s], anrmidversion[%s], company[%s]",szMsgType, CrmConfig.strAppMidIosVersion.c_str(),CrmConfig.strAppMidAnrVersion.c_str(), CrmConfig.strCompany.c_str());
	if(IsIos[0] == '1')
	{

		if(std::string::npos == CrmConfig.strAppMidIosVersion.find(AppVersion))
		{
			LOG_DEBUG("szMsgType[%s] no ios mid version!,version[%s]",szMsgType, AppVersion);
			//是否显示服务条款
			if(CrmConfig.strAppIosVersion != CrmConfig.strAppIosRevieweverUrl && AppVersion == CrmConfig.strAppIosRevieweverUrl)
			{
				root["showserpro"] = "0";
			}
			else
			{
				root["showserpro"] = "1";
			}
			if(CrmConfig.strAppIosVersion != AppVersion &&
					CrmConfig.strAppIosRevieweverUrl != AppVersion)
			{
				item["type"] = "1";
				item["version"] = CrmConfig.strAppIosVersion;
				item["uptype"] = CrmConfig.strMustUpdateIosVersion;
				item["iosverurl"] = CrmConfig.strIosVerUrl;
				arrayObj.append(item);
				LOG_DEBUG("szMsgType[%s] have new ver[%s], old ver[%s] RevieweverUrl[%s]",
						szMsgType,CrmConfig.strAppIosVersion.c_str(), AppVersion, CrmConfig.strAppIosRevieweverUrl.c_str());
			}
		}
		else
		{
			LOG_DEBUG("szMsgType[%s] this is a ios mid version!,version[%s]",szMsgType, AppVersion);
			root["showserpro"] = "1";

		}
		//是否显示服务条款
		if(CrmConfig.strAppIosSymbol != SymbolVersion)
		{
			item["type"] = "2";
			item["version"] = CrmConfig.strAppIosSymbol;
			item["uptype"] = CrmConfig.strMustUpdateIosSymbol;
			item["url"] = CrmConfig.strIosUpdateSymbolUrl;
			arrayObj.append(item);
			LOG_DEBUG("szMsgType[%s] 新配置更新",szMsgType);
		}
		if(CrmConfig.strAppIosSkin != SkinVersion)
		{
			item["type"] = "3";
			item["uptype"] = CrmConfig.strMustUpdateIosSkin;
			item["version"] = CrmConfig.strAppIosSkin;
			item["url"] = CrmConfig.strIosUpdateSkinUrl;
			arrayObj.append(item);
			LOG_DEBUG("szMsgType[%s] 新皮肤更新",szMsgType);
		}
		LOG_DEBUG("szMsgType[%s] IOS 系统",szMsgType);
	}
	else
	{
		if(std::string::npos == CrmConfig.strAppMidAnrVersion.find(AppVersion))
		{
			LOG_DEBUG("szMsgType[%s] no Anr mid version!,version[%s]",szMsgType, AppVersion);
			if(CrmConfig.strAppAdrVersion != AppVersion &&
					CrmConfig.strAppAdrRevieweverUrl != AppVersion)
			{
				item["type"] = "1";
				item["version"] = CrmConfig.strAppAdrVersion;
				item["uptype"] = CrmConfig.strMustUpdateAdrVersion;
				item["url"] = CrmConfig.strAdrUpdateVerUrl;
				arrayObj.append(item);
				LOG_DEBUG("szMsgType[%s] 新版本更新",szMsgType);
			}
			else
			{
				//暂时返回一个相同版本回去 规避一个前段安卓bug，避免死机。
				item["type"] = "1";
				item["version"] = AppVersion;
				item["uptype"] = CrmConfig.strMustUpdateIosVersion;
				arrayObj.append(item);
			}
		}
		else
		{
			LOG_DEBUG("szMsgType[%s] this is a Anr mid version!,version[%s]",szMsgType, AppVersion);
			//暂时返回一个相同版本回去 规避一个前段安卓bug，避免死机。
			item["type"] = "1";
			item["version"] = AppVersion;
			item["uptype"] = CrmConfig.strMustUpdateIosVersion;
			arrayObj.append(item);
		}
		if(CrmConfig.strAppAdrSymbol != SymbolVersion)
		{
			item["type"] = "2";
			item["version"] = CrmConfig.strAppAdrSymbol;
			item["uptype"] = CrmConfig.strMustUpdateAdrSymbol;
			item["url"] = CrmConfig.strAdrUpdateSymbolUrl;
			arrayObj.append(item);
			LOG_DEBUG("szMsgType[%s] 新配置更新",szMsgType);
		}
		if(CrmConfig.strAppAdrSkin != SkinVersion)
		{
			item["type"] = "3";
			item["version"] = CrmConfig.strAppAdrSkin;
			item["uptype"] = CrmConfig.strMustUpdateAdrSkin;
			item["url"] = CrmConfig.strAdrUpdateSkinUrl;
			arrayObj.append(item);
			LOG_DEBUG("szMsgType[%s] 新皮肤更新",szMsgType);
		}
		LOG_DEBUG("szMsgType[%s] ADR 系统",szMsgType);
	}

	root["list"] = arrayObj;
	root["service"] = Json::Value(CrmConfig.strServerUrl.c_str());
	root["port"] = Json::Value(CrmConfig.strServerPort.c_str());
	root["supportweb"] = Json::Value(CrmConfig.strSupportWeb.c_str());
	root["mintransamount_zh"] = Json::Value(CrmConfig.strMinTransAmount_zh.c_str());
	root["mintransamount_en"] = Json::Value(CrmConfig.strMinTransAmount_en.c_str());
	root["feedbackurl"] = Json::Value(CrmConfig.strFedBackUrl.c_str());
	LOG_DEBUG("%s", fast.write(root).c_str());

	cTinyWrok.MakeService2AppRspHead(dest + strlen(dest), 1, fast.write(root).size(), (char *)"200", (char *)"OK");
	strcat(dest + strlen(dest), fast.write(root).c_str());

	LOG_DEBUG("szMsgType[%s] App authenticate , srcid[%s]",szMsgType, szSrcId);
	LOG_INFO("dest[%s]", dest);
	return SUCESS;



}

int CBusiness::UserCrmTokenRspApp(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	nRoute = 2;

	memcpy(dest, src, nHeadLen);

	char *nPos = NULL;
	int  nLen = 0 ;

	memset(dest, 0x00, strlen(dest));

	//if(g_AppToken[0] == '0')
	if(strToken.empty())
	{
		LOG_ERR("get token err");
		return FAILE;
	}
	LOG_DEBUG("get crmtoken token[%s],srcId[%s]", strToken.c_str(), szSrcId);
	std::string strCrmUser;
	std::string strCrmToken;
	std::string strCrmTokenType;
	std::string strRdsCrmUser;
	std::string strRdsApptoken = pRds.get(strToken);
	if(strRdsApptoken != "")
	{
		cTinyWrok.GetJsonOneTagVal(strRdsApptoken, "crmuser", strCrmUser);
		if(strCrmUser != "")
		{
			strRdsCrmUser = pRds.get(strCrmUser);
			cTinyWrok.GetJsonOneTagVal(strRdsCrmUser, "crm_token", strCrmToken);
			cTinyWrok.GetJsonOneTagVal(strRdsCrmUser, "crm_token_type", strCrmTokenType);
			LOG_INFO("crmuser[%s]tokentype[%s],token[%s]",
					strCrmUser.c_str(), strCrmTokenType.c_str(), strCrmToken.c_str());

			Json::FastWriter fast;
			Json::Value jsonRsp;
			jsonRsp["crm_token_type"] = Json::Value(strCrmTokenType);
			jsonRsp["crm_token"] = Json::Value(strCrmToken);

			memcpy(dest, src, nHeadLen);
			cTinyWrok.MakeService2AppRspHead(dest + strlen(dest), 1, fast.write(jsonRsp).size(), (char *)"200", (char *)"OK");
			strcat(dest + strlen(dest), fast.write(jsonRsp).c_str());
			return SUCESS;
		}
	}

	LOG_INFO("dest[%s]", dest);
	return FAILE;
}



int CBusiness::UserArchiveTradeReqCrm(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	LOG_INFO("src[%s]",src);
	nRoute = 1;
	memcpy(dest, src, nHeadLen);

	char szTmp[2048] = { 0 };
	char szFirstLine[2048] = { 0 };
	char szMt4Acc[56] = { 0 };
	char szPageLen[10] = { 0 };
	char szPage[10] = { 0 };
	char szSort[10] = { 0 };
	long lTime;
	std::string strCrmUser;
	std::string strlupTime;
	std::string strIsUser;
	std::string strMt4User;
	std::string strMt4TokenType;
	std::string strMt4TokenTmp;
	std::string strMt4Token;
	std::string strRdsAppToken;
	Json::Reader reader;
	Json::Value value;

	std::string rdsCrmUser;
	std::string strCrmTokenType;
	std::string strCrmTokenTmp;
	std::string strCrmToken;

	std::string Merchantkey ;
	std::string AppIp;
	std::string Domain ;
	char TmpMerchantkey[120] = { 0 };
	char *nPos = strstr(src, TAG_HTTP);
	memcpy(szFirstLine, src, nPos - src - 1);
	//memcpy(szTmp, src, nPos - src - 1);
	LOG_DEBUG("szMsgType[%s] szFirstLine[%s]",szMsgType, szTmp);
	strrpl(szTmp, szFirstLine, "%26", "&");

	//cTinyWrok.GetTagVal(szTmp, (char*)TAG_APP_TOKEN, szAppToken, ' ');
	cTinyWrok.GetTagVal(szTmp, (char *)TAG_PAGE_LEN, szPageLen);
	cTinyWrok.GetTagVal(szTmp, (char *)TAG_PAGE, szPage);
	cTinyWrok.GetTagVal(szTmp, (char *)TAG_SORT, szSort);
	cTinyWrok.GetTagVal(szTmp, (char *)TAG_MT4ACC, szMt4Acc);

	//if ((nPos = strstr(g_AppToken, " ")) != NULL) *nPos = '\0';

	//RTrim(g_AppToken);
	LOG_DEBUG("szMsgType[%s] archive trade token[%s],srcid[%s],pagelen[%s],page[%s],sort[%s]",szMsgType, strToken.c_str(), szSrcId, szPageLen, szPage, szSort);

	//if (g_AppToken[0] == '\0' || szPage[0] == '\0' ||
	if (strToken.empty() || szPage[0] == '\0' ||
			szPageLen[0] == '\0' || szSort[0] == '\0' || szMt4Acc[0] == '\0')
	{
		LOG_ERR("szMsgType[%s] 请求错误无法解析",szMsgType);
		return FAILE;
	}

	strRdsAppToken = pRds.get(strToken);
	if (strRdsAppToken != "")
	{
		cTinyWrok.GetJsonOneTagVal(strRdsAppToken, "crmuser", strCrmUser);
		cTinyWrok.GetJsonOneTagVal(strRdsAppToken, "updatetime", strlupTime);
		cTinyWrok.GetJsonOneTagVal(strRdsAppToken, "issuer", strIsUser);
		strcat(dest, "URL:");
		if (strstr(strIsUser.c_str(), TAG_MY))
		{
			//strcat(dest, g_Url_Crm_My);
			strcat(dest, CrmConfig.strUrlCrmMy.c_str());
			LOG_DEBUG("szMsgType[%s] 用户归属[%s]",szMsgType, TAG_MY);
		}
		else
		{
			//strcat(dest, g_Url_Crm_Hy);
			strcat(dest, CrmConfig.strUrlCrmHy.c_str());
			LOG_DEBUG("szMsgType[%s] 用户归属[%s]",szMsgType, TAG_MY);
		}
	}


	lTime = get_tm_sec();
	LOG_DEBUG("szMsgType[%s] archive trade 取出来的时间[%s], 现在的时间[%ld]TimeOut[%d]",szMsgType, strlupTime.c_str(), lTime, gOnlineDelayTimeOut);

	if( strRdsAppToken == "" && lTime - atol(strlupTime.c_str()) > gOnlineDelayTimeOut )
	{
		nRoute = 2;
		cTinyWrok.MakeService2AppRspErr(dest, ERR_LOGIN_OTHMACHINE, "user offline");
		LOG_ERR("szMsgType[%s] user offline [%s]",szMsgType, strToken.c_str());
		goto LABLE;
	}


	rdsCrmUser = pRds.get(strCrmUser);
	//strCrmTokenType;
	//strCrmTokenTmp;
	//strCrmToken;
	if (rdsCrmUser != "")
	{
		cTinyWrok.GetJsonOneTagVal(rdsCrmUser, "crm_token", strCrmTokenTmp);
		cTinyWrok.GetJsonOneTagVal(rdsCrmUser, "crm_token_type", strCrmTokenType);
		strCrmToken += "Authorization:";
		strCrmToken += strCrmTokenType;
		strCrmToken += " ";
		strCrmToken += strCrmTokenTmp;
		LOG_DEBUG("szMsgType[%s] mt4token[%s]",szMsgType, strCrmToken.c_str());
	}
	else
	{
		LOG_ERR("szMsgType[%s] 获取token失败",szMsgType);
		return FAILE;
	}
	LOG_DEBUG("szMsgType[%s] archive trade  CRMUSER[%s], luptime[%ld], isuser[%s] srcid[%s]",
			szMsgType,strCrmUser.c_str(), strlupTime.c_str(), strIsUser.c_str(), szSrcId);

	strcat(dest, USER_GETARCHTRA__CRM);
	strcat(dest, "?");
	LOG_DEBUG("szMsgType[%s] archive trade buf[%s]",szMsgType, dest);

	if(src[MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN+1] == '1')
	{
		AppIp = CrmConfig.strIOS_AppIp;
		Domain = CrmConfig.strIOS_DoMain;
		Merchantkey = CrmConfig.strIOS_Merchantkey;
	}
	else
	{
		AppIp = CrmConfig.strAppIp;
		Domain = CrmConfig.strDoMain;
		Merchantkey = CrmConfig.strMerchantKey;
	}
	Base64_Encode(TmpMerchantkey,Merchantkey.c_str(), Merchantkey.size());
	sprintf(dest + strlen(dest), "%s%s&%s%s&%s%s|%s%smt4account:%s&%s%s&%s%s&%s%s%s&%s%s",
			TAG_SORT, szSort, TAG_PAGE, szPage, TAG_PAGE_LEN, szPageLen, TAG_SND, TAG_MT4ACC, szMt4Acc, TAG_CRM_TOKEN, strCrmToken.c_str(), \
			TAG_MERCHANTKEY_GET,TmpMerchantkey,TAG_DOMAIN_GET,TAG_DOMAIN_POST, \
			Domain.c_str(),TAG_APPIP_GET,AppIp.c_str());

LABLE:
	LOG_INFO("szMsgType[%s] archive trade buf[%s]",szMsgType, dest);

	return SUCESS;
}

int CBusiness::UserArchiveTradeRspApp(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	//LOG_DEBUG("archive trade query rsp");
	nRoute = 2;
	char szRspCode[10] = { 0 };
	char szRspMsg[128] = { 0 };
	char *nPos = NULL;
	int  nLen = 0;

	memset(dest, 0x00, strlen(dest));

	nPos = strstr(src, "\r\n\r\n");
	memcpy(dest, src, nHeadLen);
	if (nPos != NULL)
	{
		cTinyWrok.MakeService2AppRspHead(dest + strlen(dest), 1, strlen(nPos) - 4, (char *)"200", (char *)"OK");
		strcat(dest, nPos + 4);
	}
	else
	{
		LOG_ERR("szMsgType[%s] archive trade rsp 报文解析错误!",szMsgType);
		return FAILE;
	}

	LOG_INFO("archive trade rsp buf[%s]", dest);
	return SUCESS;
}


int CBusiness::UserWithdrawTemplateReqCrm(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	//LOG_INFO("szMsgType=%s",szMsgType);
	LOG_INFO("src[%s]",src);
	nRoute = 1;
	memcpy(dest, src, nHeadLen);

	char szTmp[2048] = { 0 };
	char szMt4Acc[56] = { 0 };
	char szLang[32] = { 0 };

	char *nPos = strstr(src, TAG_HTTP);
	memcpy(szTmp, src, nPos - src - 1);
	strrpl(szTmp, szTmp, "%26", "&");
	LOG_DEBUG("szMsgType[%s] withdraw template szTmp[%s] srcid[%s]",szMsgType, szTmp, szSrcId);

	//cTinyWrok.GetTagVal(szTmp, (char*)TAG_APP_TOKEN, szAppToken, ' ');
	cTinyWrok.GetTagVal(szTmp, (char *)TAG_MT4ACC, szMt4Acc);
	//cTinyWrok.GetTagVal(szTmp, (char*)TAG_LANGUAGE, szLang);
	cTinyWrok.GetTagVal(szTmp, (char *)TAG_LANG, szLang);

	//if ((nPos = strstr(g_AppToken, " ")) != NULL) *nPos = '\0';

	//RTrim(g_AppToken);

	if (strToken.empty() || szMt4Acc[0] == '\0')
	{
		LOG_ERR("szMsgType[%s] withdraw template parse apptoken or mt4 acct error",szMsgType);
		return FAILE;
	}
	LOG_DEBUG("szMsgType[%s] withdraw template mt4acct[%s] token[%s]",szMsgType, szMt4Acc, strToken.c_str());

	std::string urlPrefix;
	std::string strCrmToken;
	std::string strCode;
	std::string strReson;
	int ret;
	std::string Merchantkey ;
	std::string AppIp;
	std::string Domain ;
	char TmpMerchantkey[120] = { 0 };
	ret = GetUserTypeAndCrmToken((char *)strToken.c_str(), urlPrefix, strCrmToken, pRds, CrmConfig);
	LOG_DEBUG("szMsgType[%s] ret[%d]",szMsgType, ret);
	if(SUCESS != ret)
	{
		nRoute = 2;
		if(ret == atoi(ERR_OFFLINE))
		{
			strCode = ERR_OFFLINE;
			strReson = "user offline";
		}
		else if(ret == atoi(ERR_LOGIN_OTHMACHINE))
		{
			strCode = ERR_LOGIN_OTHMACHINE;
			strReson = "user login on other phone";
		}
		else
		{
			strCode = ERR_SERVER;
			strReson = "服务器繁忙,请稍后在试";
		}
		cTinyWrok.MakeService2AppRspErr(dest, strCode, strReson);
		goto LABLE;
	}

#if 0
	if (FAILE == GetUserTypeAndCrmToken(g_AppToken, urlPrefix, strCrmToken))
	{
		nRoute = 2;
		cTinyWrok.MakeService2AppRspErr(dest, ERR_OFFLINE, "user offline");
		LOG_ERR("withdraw template get crmtoken and user type from redis failed");
		goto LABLE;
	}
#endif

	strcat(dest, TAG_URL);
	strcat(dest, urlPrefix.c_str());
	//strcat(dest, strCrm.c_str());
	strcat(dest, USER_GETWITHDRAW_CRM_TMP);
	//strcat(dest, "?");
	// LOG_INFO("withdraw template buf[%s]", dest);

	if(src[MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN+1] == '1')
	{
		AppIp = CrmConfig.strIOS_AppIp;
		Domain = CrmConfig.strIOS_DoMain;
		Merchantkey = CrmConfig.strIOS_Merchantkey;
	}
	else
	{
		AppIp = CrmConfig.strAppIp;
		Domain = CrmConfig.strDoMain;
		Merchantkey = CrmConfig.strMerchantKey;
	}
	Base64_Encode(TmpMerchantkey,Merchantkey.c_str(), Merchantkey.size());
	sprintf(dest + strlen(dest), "|%s%smt4account:%s&%s%s&%s%s&%s%s%s&%s%s", TAG_SND, TAG_MT4ACC, szMt4Acc, TAG_CRM_TOKEN, strCrmToken.c_str(), \
			TAG_MERCHANTKEY_GET,TmpMerchantkey,TAG_DOMAIN_GET,TAG_DOMAIN_POST, \
			Domain.c_str(),TAG_APPIP_GET,AppIp.c_str());
	if (szLang[0] != '\0')
	{
		//sprintf(dest + strlen(dest), "&%sLang:%s", TAG_LANG, TAG_LANGUAGE_CRM_EN);
		//sprintf(dest + strlen(dest), "&%s%s", TAG_LANGUAGE, szLang);
		sprintf(dest + strlen(dest), "&%s%s%s", TAG_LANG, TAG_LANGUAGE_HEAD_CRM, szLang);
	}

LABLE:
	LOG_INFO("withdraw template buf[%s]", dest);
	//LOG_INFO("archive trade buf[%s]", dest);

	return SUCESS;
}

int CBusiness::UserWithdrawRspApp(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	nRoute = 2;
	char szTmp[1024] = {0};
	char *nPos = NULL;

	memset(szTmp, 0x00, sizeof(szTmp));
	memset(dest, 0x00, strlen(dest));
	char szRspCode[5] = {0};
	char szRspMsg[128] = {0};

	//memcpy(dest, src, MSG_TYPE_LEN+SPLIT_LEN);
	memcpy(dest, src, nHeadLen);

	cTinyWrok.GetTagVal(src, (char * )TAG_HTTP11, szTmp);
	LOG_DEBUG("szMsgType[%s] tmp[%s]",szMsgType, szTmp);
	if(szTmp[0] != '\0')
	{
		nPos = strstr(szTmp, " ");
		if(nPos != NULL)
		{
			//取错误码和错误原因
			memcpy(szRspCode, szTmp, nPos - szTmp);
			memcpy(szRspMsg, nPos + 1, strlen(szTmp) + szTmp - nPos - 1);
			LOG_DEBUG("szMsgType[%s] szRspCode[%s],szRspMsg[%s]nPos[%s]",szMsgType, szRspCode, szRspMsg, nPos);
		}
		else
		{
			LOG_ERR("szMsgType[%s] Get retcode err",szMsgType);
			return FAILE;
		}
	}
	else
	{
		LOG_ERR("szMsgType[%s] Get Tag[%s] err",szMsgType, TAG_HTTP11);
		return FAILE;
	}
	Json::Value crmRsp;
	if(strstr(szRspCode, "200"))
	{
		nPos = cTinyWrok.FindSubstr(src, (char * )TAG_HTTP11);
		if (nPos != NULL)
		{
			if((nPos = cTinyWrok.FindSubstr(src, (char * )"\r\n\r\n") ) != NULL)
			{
				cTinyWrok.MakeCrm2AppRspHead(src, dest + strlen(dest), 1, strlen(nPos));
				strcat(dest + strlen(dest), nPos);
			}
			else
			{
				LOG_ERR("szMsgType[%s] 解包错误",szMsgType);
				return FAILE;
			}
		}
		else
		{
			LOG_ERR("szMsgType[%s] 报文解析错误!",szMsgType);
			return FAILE;
		}
	}
	else
	{
		std::string errCode;
		if((nPos = cTinyWrok.FindSubstr(src, (char * )"\r\n\r\n") ) != NULL)
		{
			LOG_ERR("szMsgType[%s] nPos[%s]",szMsgType, nPos);
			Json::Reader reader;

			if(reader.parse(nPos, crmRsp))
			{
				//提取错误信息
				const Json::Value arrayMsg = crmRsp["msg"];
#if 0
				for (unsigned int i = 0; i < arrayMsg.size(); i++)
				{
					Json::Value msg = arrayMsg[i];
					Json::Value body = msg["body"];

					for (unsigned int j = 0; j < body.size(); j++)
					{
						errCode += body[i].asString();
						break;
					}
				}
#endif
			}
		}
		crmRsp[TAG_RESULT] = szRspCode;
		//LOG_INFO("errCode[%s]", errCode.c_str());
		//Json::Value jsonRsp = makeErrJson(__FUNCTION__, szRspCode, errCode.c_str());
		Json::FastWriter fast;
		//cTinyWrok.MakeService2AppRspHead(dest+strlen(dest), 1, fast.write(jsonRsp).size(), (char*)"200", (char*)"OK");
		cTinyWrok.MakeService2AppRspHead(dest + strlen(dest), 1, fast.write(crmRsp).size(), (char *)"200", (char *)"OK");
		strcat(dest, fast.write(crmRsp).c_str());
	}
	LOG_INFO("UserWithdrawRspApp srcid[%s] retcode[%s], retmsg[%s] sucess dest[%s]", szSrcId, szRspCode, szRspMsg, dest);
}

int CBusiness::UserWithdrawReqCrm(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	//LOG_INFO("szMsgType=%s",szMsgType);
	LOG_INFO("src[%s]",src);
	nRoute = 1;
	memcpy(dest, src, nHeadLen);

	char szTmp[2048] = { 0 };
	memset(szTmp, 0, sizeof(szTmp));

	char szFirstLine[2048] = { 0 };
	memset(szFirstLine, 0, sizeof(szFirstLine));

	char szMt4Acc[56] = { 0 };

	char *nPos = strstr(src, TAG_HTTP);
	memcpy(szFirstLine, src, nPos - src - 1);
	strrpl(szTmp, szFirstLine, "%26", "&");

	LOG_DEBUG("szMsgType[%s] withdraw temp[%s]",szMsgType, szTmp);

	char *sPos = strstr(src, "?");
	memset(szFirstLine, 0, sizeof(szFirstLine));
	memcpy(szFirstLine, sPos + 1,  nPos - (sPos + 1) - 1);
	strrpl(szFirstLine, szFirstLine, "%26", "&");
	LOG_DEBUG("szMsgType[%s] withdraw szFirstLine[%s]",szMsgType, szFirstLine);

	cTinyWrok.GetTagVal(szTmp, (char *)TAG_MT4ACC, szMt4Acc);

	//if ((nPos = strstr(g_AppToken, " ")) != NULL) *nPos = '\0';

	//RTrim(g_AppToken);
	LOG_INFO("szMsgType[%s] withdraw mt4acct[%s] token[%s]",szMsgType, szMt4Acc, strToken.c_str());

	if (strToken.empty() || szMt4Acc[0] == '\0')
	{
		LOG_ERR("szMsgType[%s] withdraw parse apptoken or mt4 acct error",szMsgType);
		return FAILE;
	}

#if 0
	// parse get request get post body content
	char clean[128];
	memset(clean, 0, sizeof(clean));
	sprintf(clean, "%s%s&", TAG_MT4ACC, szMt4Acc);
	strrpl(szFirstLine, szFirstLine, clean, "");

	memset(clean, 0, sizeof(clean));
	sprintf(clean, "%s%s&", TAG_APP_TOKEN, g_AppToken);
	strrpl(szFirstLine, szFirstLine, clean, "");

	memset(clean, 0, sizeof(clean));
	sprintf(clean, "%s%s", TAG_MT4ACC, szMt4Acc);
	strrpl(szFirstLine, szFirstLine, clean, "");

	memset(clean, 0, sizeof(clean));
	sprintf(clean, "%s%s", TAG_APP_TOKEN, g_AppToken);
	strrpl(szFirstLine, szFirstLine, clean, "");

	int len = strlen(szFirstLine);
	if (szFirstLine[len - 1] == '&')
	{
		szFirstLine[len - 1] = '\0';
	}
	LOG_INFO("withdraw post [%s]", szFirstLine);
#endif


	std::string urlPrefix;
	std::string strCrmToken;
	std::string strCode;
	std::string strReson;
	int ret;
	std::string Merchantkey ;
	std::string AppIp;
	std::string Domain ;
	char TmpMerchantkey[120] = { 0 };
	//ret = GetUserTypeAndCrmToken(g_AppToken, urlPrefix, strCrmToken);
	ret = GetUserTypeAndCrmToken((char *)strToken.c_str(), urlPrefix, strCrmToken, pRds, CrmConfig);
	LOG_INFO("szMsgType[%s] ret[%d]",szMsgType, ret);
	if(SUCESS != ret)
	{
		nRoute = 2;
		if(ret == atoi(ERR_OFFLINE))
		{
			strCode = ERR_OFFLINE;
			strReson = "user offline";
		}
		else if(ret == atoi(ERR_LOGIN_OTHMACHINE))
		{
			strCode = ERR_LOGIN_OTHMACHINE;
			strReson = "user login on other phone";
		}
		else
		{
			strCode = ERR_SERVER;
			strReson = "服务器繁忙,请稍后在试";
		}
		cTinyWrok.MakeService2AppRspErr(dest, strCode, strReson);
		goto LABLE;
	}

	strcat(dest, TAG_URL);
	strcat(dest, urlPrefix.c_str());
	//strcat(dest, strCrm.c_str());
	strcat(dest, USER_GETWITHDRAW_CRM);
	//strcat(dest, "?");

	if(src[MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN+1] == '1')
	{
		AppIp = CrmConfig.strIOS_AppIp;
		Domain = CrmConfig.strIOS_DoMain;
		Merchantkey = CrmConfig.strIOS_Merchantkey;
	}
	else
	{
		AppIp = CrmConfig.strAppIp;
		Domain = CrmConfig.strDoMain;
		Merchantkey = CrmConfig.strMerchantKey;
	}
	Base64_Encode(TmpMerchantkey,Merchantkey.c_str(), Merchantkey.size());
	sprintf(dest + strlen(dest), "|%s%smt4account:%s&%s%s&%s%s&%s%s%s&%s%s|%s%s", TAG_SND, TAG_MT4ACC, szMt4Acc, TAG_CRM_TOKEN, strCrmToken.c_str(),TAG_MERCHANTKEY_GET,TmpMerchantkey,TAG_DOMAIN_GET,TAG_DOMAIN_POST, \
			Domain.c_str(),TAG_APPIP_GET,AppIp.c_str(), TAG_POST, szFirstLine);

LABLE:
	LOG_INFO("withdraw buf[%s]", dest);
	//LOG_INFO("archive trade buf[%s]", dest);

	return SUCESS;
}

int CBusiness::UserWithdrawHistoryReqCrm(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	//LOG_INFO("withdraw template req");
	LOG_INFO("src[%s]",src);
	nRoute = 1;
	memcpy(dest, src, nHeadLen);

	char szTmp[2048] = { 0 };
	char szMt4Acc[56] = { 0 };
	char szPageLen[10] = { 0 };
	char szPage[10] = { 0 };

	char *nPos = strstr(src, TAG_HTTP);
	memcpy(szTmp, src, nPos - src - 1);
	strrpl(szTmp, szTmp, "%26", "&");
	LOG_DEBUG("szMsgType[%s] withdraw history szTmp[%s]",szMsgType, szTmp);

	//cTinyWrok.GetTagVal(szTmp, (char*)TAG_APP_TOKEN, szAppToken, ' ');
	cTinyWrok.GetTagVal(szTmp, (char *)TAG_MT4ACC, szMt4Acc);
	cTinyWrok.GetTagVal(szTmp, (char *)TAG_PAGE_LEN, szPageLen);
	cTinyWrok.GetTagVal(szTmp, (char *)TAG_PAGE, szPage);
	//if ((nPos = strstr(g_AppToken, " ")) != NULL) *nPos = '\0';

	//RTrim(g_AppToken);

	//if (g_AppToken[0] == '\0' || szMt4Acc[0] == '\0' || szPage[0] == '\0' || szPageLen[0] == '\0')
	if (strToken.empty() || szMt4Acc[0] == '\0' || szPage[0] == '\0' || szPageLen[0] == '\0')
	{
		LOG_ERR("szMsgType[%s] withdraw history parse param error",szMsgType);
		return FAILE;
	}

	LOG_DEBUG("szMsgType[%s] withdraw history mt4acct[%s] token[%s]",szMsgType, szMt4Acc, strToken.c_str());

	std::string urlPrefix;
	std::string strCrmToken;
	std::string strCode;
	std::string strReson;
	int ret;
	std::string Merchantkey ;
	std::string AppIp;
	std::string Domain ;
	char TmpMerchantkey[120] = { 0 };
	//ret = GetUserTypeAndCrmToken(g_AppToken, urlPrefix, strCrmToken);
	ret = GetUserTypeAndCrmToken((char *)strToken.c_str(), urlPrefix, strCrmToken, pRds, CrmConfig);
	LOG_DEBUG("szMsgType[%s] ret[%d]",szMsgType, ret);
	if(SUCESS != ret)
	{
		nRoute = 2;
		if(ret == atoi(ERR_OFFLINE))
		{
			strCode = ERR_OFFLINE;
			strReson = "user offline";
		}
		else if(ret == atoi(ERR_LOGIN_OTHMACHINE))
		{
			strCode = ERR_LOGIN_OTHMACHINE;
			strReson = "user login on other phone";
		}
		else
		{
			strCode = ERR_SERVER;
			strReson = "服务器繁忙,请稍后在试";
		}
		cTinyWrok.MakeService2AppRspErr(dest, strCode, strReson);
		goto LABLE;
	}

	strcat(dest, TAG_URL);
	strcat(dest, urlPrefix.c_str());
	//strcat(dest, strCrm.c_str());
	strcat(dest, USER_WITHDRAW_REQ_CRM);
	//strcat(dest, "?");

	if(src[MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN+1] == '1')
	{
		AppIp = CrmConfig.strIOS_AppIp;
		Domain = CrmConfig.strIOS_DoMain;
		Merchantkey = CrmConfig.strIOS_Merchantkey;
	}
	else
	{
		AppIp = CrmConfig.strAppIp;
		Domain = CrmConfig.strDoMain;
		Merchantkey = CrmConfig.strMerchantKey;
	}
	Base64_Encode(TmpMerchantkey,Merchantkey.c_str(), Merchantkey.size());
	sprintf(dest + strlen(dest), "%s%s&%s%s|%s%smt4account:%s&%s%s&%s%s&%s%s%s&%s%s",
			TAG_PAGE, szPage, TAG_PAGE_LEN, szPageLen, TAG_SND, TAG_MT4ACC, szMt4Acc, TAG_CRM_TOKEN, strCrmToken.c_str(), \
			TAG_MERCHANTKEY_GET,TmpMerchantkey,TAG_DOMAIN_GET,TAG_DOMAIN_POST, \
			Domain.c_str(),TAG_APPIP_GET,AppIp.c_str());

LABLE:
	LOG_INFO("withdraw history buf[%s]", dest);
	return SUCESS;
}

int CBusiness::UserWithdrawHistoryRspApp(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	nRoute = 2;
	char *nPos = NULL;

	memset(dest, 0x00, strlen(dest));

	nPos = strstr(src, "\r\n\r\n");
	memcpy(dest, src, nHeadLen);
	if (nPos != NULL)
	{
		cTinyWrok.MakeService2AppRspHead(dest + strlen(dest), 1, strlen(nPos) - 4, (char *)"200", (char *)"OK");
		strcat(dest, nPos + 4);
	}
	else
	{
		LOG_ERR("szMsgType[%s] withdraw history rsp parse packet error",szMsgType);
		std::string errcode;
		if (GetHttpErrorCode(src, errcode) == 0)
		{
			cTinyWrok.MakeService2AppRspErr(dest, errcode);
		}
		else
		{
			return FAILE;
		}
	}

	LOG_INFO("szMsgType[%s] withdraw history rsp buf[%s]",szMsgType, dest);
	return SUCESS;
}

int CBusiness::UserWithdrawDeleteReqCrm(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	LOG_INFO("src[%s]",src);
	nRoute = 1;
	memcpy(dest, src, nHeadLen);

	char szTmp[2048] = { 0 };
	memset(szTmp, 0, sizeof(szTmp));

	char szMt4Acc[56] = { 0 };

	char szRequest[128] = { 0 };
	char szPwd[CRM_PWD_LEN + 1] = { 0 };
	char szTmpPwd[CRM_PWD_LEN + 1] = { 0 };
	char szEmali[CRM_USER_LEN + 3] = { 0 };

	char *nPos = strstr(src, TAG_HTTP);
	memcpy(szTmp, src, nPos - src - 1);
	strrpl(szTmp, szTmp, "%26", "&");

	LOG_DEBUG("withdraw request temp[%s]", szTmp);

	//cTinyWrok.GetTagVal(szTmp, (char*)TAG_APP_TOKEN, szAppToken, ' ');
	cTinyWrok.GetTagVal(szTmp, (char *)TAG_MT4ACC, szMt4Acc);
	cTinyWrok.GetTagVal(szTmp, (char *)TAG_REQUESTID, szRequest);
	cTinyWrok.GetTagVal(src, (char *)TAG_EMAIL, szEmali);
	cTinyWrok.GetTagVal(src, (char *)TAG_PWD, szTmpPwd);
	strrpl(szEmali, szEmali, "%40", "@");
	strrpl(szTmpPwd, szTmpPwd, "%3D", "=");
	Base64_Decode(szPwd, szTmpPwd, strlen(szTmpPwd));

	//if ((nPos = strstr(g_AppToken, " ")) != NULL) *nPos = '\0';

	//RTrim(g_AppToken);
	//LOG_INFO("withdraw request mt4acct[%s] token[%s]", szMt4Acc, g_AppToken);
	LOG_DEBUG("withdraw request mt4acct[%s] token[%s]", szMt4Acc, strToken.c_str());

	//if (g_AppToken[0] == '\0' || szMt4Acc[0] == '\0' || szRequest[0] == '\0'
	if (strToken.empty() || szMt4Acc[0] == '\0' || szRequest[0] == '\0'
			|| szEmali[0] == '\0' || szPwd[0] == '\0')
	{
		LOG_ERR("withdraw request parse parameter error");
		return FAILE;
	}

	std::string urlPrefix;
	std::string strCrmToken;
	std::string strCode;
	std::string strReson;
	int ret;
	std::string Merchantkey ;
	std::string AppIp;
	std::string Domain ;
	char TmpMerchantkey[120] = { 0 };

	//ret = GetUserTypeAndCrmToken(g_AppToken, urlPrefix, strCrmToken);
	ret = GetUserTypeAndCrmToken((char *)strToken.c_str(), urlPrefix, strCrmToken, pRds, CrmConfig);
	LOG_DEBUG("ret[%d]", ret);
	if(SUCESS != ret)
	{
		nRoute = 2;
		if(ret == atoi(ERR_OFFLINE))
		{
			strCode = ERR_OFFLINE;
			strReson = "user offline";
		}
		else if(ret == atoi(ERR_LOGIN_OTHMACHINE))
		{
			strCode = ERR_LOGIN_OTHMACHINE;
			strReson = "user login on other phone";
		}
		else
		{
			strCode = ERR_SERVER;
			strReson = "服务器繁忙,请稍后在试";
		}
		cTinyWrok.MakeService2AppRspErr(dest, strCode, strReson);
		goto LABLE;
	}

	strcat(dest, TAG_URL);
	strcat(dest, urlPrefix.c_str());
	//strcat(dest, strCrm.c_str());
	strcat(dest, USER_WITHDRAW_REQ_CRM);

	if(src[MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN+1] == '1')
	{
		AppIp = CrmConfig.strIOS_AppIp;
		Domain = CrmConfig.strIOS_DoMain;
		Merchantkey = CrmConfig.strIOS_Merchantkey;
	}
	else
	{
		AppIp = CrmConfig.strAppIp;
		Domain = CrmConfig.strDoMain;
		Merchantkey = CrmConfig.strMerchantKey;
	}
	Base64_Encode(TmpMerchantkey,Merchantkey.c_str(), Merchantkey.size());
	sprintf(dest + strlen(dest), "/%s|%s%smt4account:%s&%s%s&%s%s&%s%s%s&%s%s|%s%s%s&%s%s",
			szRequest, TAG_SND, TAG_MT4ACC, szMt4Acc, TAG_CRM_TOKEN, strCrmToken.c_str(), TAG_MERCHANTKEY_GET,TmpMerchantkey,TAG_DOMAIN_GET,TAG_DOMAIN_POST, \
			Domain.c_str(),TAG_APPIP_GET,AppIp.c_str(),TAG_POST, TAG_EMAIL, szEmali, TAG_PWD, szPwd);

LABLE:
	LOG_DEBUG("withdraw request buf[%s]", dest);
	//LOG_INFO("archive trade buf[%s]", dest);

	return SUCESS;
}

int CBusiness::UserDepositTemplateReqCrm(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	LOG_INFO("src[%s]",src);
	nRoute = 1;
	memcpy(dest, src, nHeadLen);

	char szTmp[2048] = { 0 };
	char szMt4Acc[56] = { 0 };
	char szLang[56] = { 0 };

	char *nPos = strstr(src, TAG_HTTP);
	memcpy(szTmp, src, nPos - src - 1);
	strrpl(szTmp, szTmp, "%26", "&");
	LOG_DEBUG("szMsgType[%s] deposit template szTmp[%s]",szMsgType, szTmp);

	cTinyWrok.GetTagVal(szTmp, (char *)TAG_MT4ACC, szMt4Acc);
	//cTinyWrok.GetTagVal(szTmp, (char*)TAG_LANGUAGE, szLang);
	cTinyWrok.GetTagVal(szTmp, (char *)TAG_LANG, szLang);

	//if ((nPos = strstr(g_AppToken, " ")) != NULL) *nPos = '\0';

	//RTrim(g_AppToken);
	//if (g_AppToken[0] == '\0' || szMt4Acc[0] == '\0')
	if (strToken.empty() || szMt4Acc[0] == '\0')
	{
		LOG_ERR("szMsgType[%s] deposit template parse apptoken or mt4 acct error",szMsgType);
		return FAILE;
	}
	LOG_DEBUG("szMsgType[%s] deposit template mt4acct[%s] token[%s]",szMsgType, szMt4Acc, strToken.c_str());

	std::string urlPrefix;
	std::string strCrmToken;
	std::string strCode;
	std::string strReson;
	int ret;
	std::string Merchantkey ;
	std::string AppIp;
	std::string Domain ;
	char TmpMerchantkey[120] = { 0 };
	//ret = GetUserTypeAndCrmToken(g_AppToken, urlPrefix, strCrmToken);
	ret = GetUserTypeAndCrmToken((char *)strToken.c_str(), urlPrefix, strCrmToken, pRds, CrmConfig);
	LOG_DEBUG("szMsgType[%s] ret[%d]",szMsgType, ret);
	if(SUCESS != ret)
	{
		nRoute = 2;
		if(ret == atoi(ERR_OFFLINE))
		{
			strCode = ERR_OFFLINE;
			strReson = "user offline";
		}
		else if(ret == atoi(ERR_LOGIN_OTHMACHINE))
		{
			strCode = ERR_LOGIN_OTHMACHINE;
			strReson = "user login on other phone";
		}
		else
		{
			strCode = ERR_SERVER;
			strReson = "服务器繁忙,请稍后在试";
		}
		cTinyWrok.MakeService2AppRspErr(dest, strCode, strReson);
		goto LABLE;
	}

	strcat(dest, TAG_URL);
	strcat(dest, urlPrefix.c_str());
	//strcat(dest, strCrm.c_str());
	strcat(dest, USER_GETDEPOSIT_REQ_CRM);
	//strcat(dest, "?");
	LOG_DEBUG("szMsgType[%s] lan[%s]",szMsgType, szLang);
	sprintf(dest + strlen(dest), "|%s%smt4account:%s&%s%s", TAG_SND, TAG_MT4ACC, szMt4Acc, TAG_CRM_TOKEN, strCrmToken.c_str());
	if (szLang[0] != '\0')
	{
		sprintf(dest + strlen(dest), "&%s%s%s", TAG_LANG, TAG_LANGUAGE_HEAD_CRM, szLang);
		//sprintf(dest + strlen(dest), "&%sLang:%s", TAG_LANG, TAG_LANGUAGE_CRM_ZH);
	}

	if(src[MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN+1] == '1')
	{
		AppIp = CrmConfig.strIOS_AppIp;
		Domain = CrmConfig.strIOS_DoMain;
		Merchantkey = CrmConfig.strIOS_Merchantkey;
	}
	else
	{
		AppIp = CrmConfig.strAppIp;
		Domain = CrmConfig.strDoMain;
		Merchantkey = CrmConfig.strMerchantKey;
	}
	Base64_Encode(TmpMerchantkey,Merchantkey.c_str(), Merchantkey.size());
	sprintf(dest+strlen(dest),"&%s%s&%s%s%s&%s%s",TAG_MERCHANTKEY_GET,TmpMerchantkey,TAG_DOMAIN_GET,TAG_DOMAIN_POST, \
			Domain.c_str(),TAG_APPIP_GET,AppIp.c_str());
LABLE:
	LOG_INFO("deposit template buf[%s]", dest);

	return SUCESS;
}

int CBusiness::UserDepositReqCrm(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	LOG_INFO("src[%s]",src);
	nRoute = 1;
	memcpy(dest, src, nHeadLen);

	char szTmp[2048] = { 0 };
	memset(szTmp, 0, sizeof(szTmp));

	char szFirstLine[2048] = { 0 };
	memset(szFirstLine, 0, sizeof(szFirstLine));

	char szMt4Acc[56] = { 0 };
	char szGetwayId[56] = { 0 };
	char szNationalId[56] = { 0 };

	char *nPos = strstr(src, TAG_HTTP);
	memcpy(szFirstLine, src, nPos - src - 1);
	strrpl(szTmp, szFirstLine, "%26", "&");

	LOG_DEBUG("szMsgType[%s] deposit temp[%s]",szMsgType, szTmp);

	char *sPos = strstr(src, "?");
	memset(szFirstLine, 0, sizeof(szFirstLine));
	memcpy(szFirstLine, sPos + 1, nPos - (sPos + 1) - 1);
	strrpl(szFirstLine, szFirstLine, "%26", "&");
	LOG_DEBUG("szMsgType[%s] deposit szFirstLine[%s],",szMsgType, szFirstLine);

	//cTinyWrok.GetTagVal(szTmp, (char*)TAG_APP_TOKEN, g_AppToken, ' ');
	cTinyWrok.GetTagVal(szTmp, (char *)TAG_MT4ACC, szMt4Acc);
	cTinyWrok.GetTagVal(szTmp, (char *)TAG_GETWAYID, szGetwayId);

	//if ((nPos = strstr(g_AppToken, " ")) != NULL) *nPos = '\0';

	//RTrim(g_AppToken);
	LOG_DEBUG("szMsgType[%s] deposit mt4acct[%s] token[%s]",szMsgType, szMt4Acc, strToken.c_str());

	if (strToken.empty() || szMt4Acc[0] == '\0' || szGetwayId[0] == '\0')
	{
		LOG_ERR("szMsgType[%s] deposit parse get parameter error",szMsgType);
		return FAILE;
	}

	// parse get request get post body content

	int len = strlen(szFirstLine);
	if (szFirstLine[len - 1] == '&')
	{
		szFirstLine[len - 1] = '\0';
	}
	LOG_DEBUG("szMsgType[%s] deposit post [%s]",szMsgType, szFirstLine);

	std::string urlPrefix;
	std::string strCrmToken;
	std::string strCode;
	std::string strReson;
	int ret;
	std::string Merchantkey ;
	std::string AppIp;
	std::string Domain ;
	char TmpMerchantkey[120] = { 0 };
	//ret = GetUserTypeAndCrmToken(g_AppToken, urlPrefix, strCrmToken);
	ret = GetUserTypeAndCrmToken((char *)strToken.c_str(), urlPrefix, strCrmToken, pRds, CrmConfig);
	LOG_DEBUG("szMsgType[%s] ret[%d]",szMsgType, ret);
	if(SUCESS != ret)
	{
		nRoute = 2;
		if(ret == atoi(ERR_OFFLINE))
		{
			strCode = ERR_OFFLINE;
			strReson = "user offline";
		}
		else if(ret == atoi(ERR_LOGIN_OTHMACHINE))
		{
			strCode = ERR_LOGIN_OTHMACHINE;
			strReson = "user login on other phone";
		}
		else
		{
			strCode = ERR_SERVER;
			strReson = "服务器繁忙,请稍后在试";
		}
		cTinyWrok.MakeService2AppRspErr(dest, strCode, strReson);
		goto LABLE;
	}

	strcat(dest, TAG_URL);
	strcat(dest, urlPrefix.c_str());
	//strcat(dest, strCrm.c_str());
	strcat(dest, USER_GETDEPOSIT_REQ_CRM);

	if(src[MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN+1] == '1')
	{
		AppIp = CrmConfig.strIOS_AppIp;
		Domain = CrmConfig.strIOS_DoMain;
		Merchantkey = CrmConfig.strIOS_Merchantkey;
	}
	else
	{
		AppIp = CrmConfig.strAppIp;
		Domain = CrmConfig.strDoMain;
		Merchantkey = CrmConfig.strMerchantKey;
	}
	Base64_Encode(TmpMerchantkey,Merchantkey.c_str(), Merchantkey.size());
	sprintf(dest + strlen(dest), "|%s%s&%s%smt4account:%s&%s%s&%s%s&%s%s%s&%s%s|%s%s",
			TAG_GETWAYID, szGetwayId, TAG_SND, TAG_MT4ACC, szMt4Acc, TAG_CRM_TOKEN, strCrmToken.c_str(), TAG_MERCHANTKEY_GET,TmpMerchantkey ,TAG_DOMAIN_GET,TAG_DOMAIN_POST, \
			Domain.c_str(),TAG_APPIP_GET,AppIp.c_str(),TAG_POST, szFirstLine);


LABLE:
	LOG_INFO("deposit buf[%s]", dest);

	return SUCESS;
}

int CBusiness::UserDepositRspApp(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	nRoute = 2;
	char szTmp[1024] = {0};
	char *nPos = NULL;

	memset(szTmp, 0x00, sizeof(szTmp));
	memset(dest, 0x00, strlen(dest));
	char szRspCode[5] = {0};
	char szRspMsg[128] = {0};

	//memcpy(dest, src, MSG_TYPE_LEN+SPLIT_LEN);
	memcpy(dest, src, nHeadLen);

	cTinyWrok.GetTagVal(src, (char * )TAG_HTTP11, szTmp);
	LOG_DEBUG("tmp[%s]", szTmp);
	if(szTmp[0] != '\0')
	{
		nPos = strstr(szTmp, " ");
		if(nPos != NULL)
		{
			//取错误码和错误原因
			memcpy(szRspCode, szTmp, nPos - szTmp);
			memcpy(szRspMsg, nPos + 1, strlen(szTmp) + szTmp - nPos - 1);
			LOG_INFO("szMsgType[%s] szRspCode[%s],szRspMsg[%s]nPos[%s]",szMsgType, szRspCode, szRspMsg, nPos);
		}
		else
		{
			LOG_ERR("szMsgType[%s] Get retcode err",szMsgType);
			return FAILE;
		}
	}
	else
	{
		LOG_ERR("szMsgType[%s] Get Tag[%s] err",szMsgType, TAG_HTTP11);
		return FAILE;
	}

	if(strstr(szRspCode, "200"))
	{
		nPos = cTinyWrok.FindSubstr(src, (char * )TAG_HTTP11);
		if (nPos != NULL)
		{
			if((nPos = cTinyWrok.FindSubstr(src, (char * )"\r\n\r\n") ) != NULL)
			{
				cTinyWrok.MakeCrm2AppRspHead(src, dest + strlen(dest), 1, strlen(nPos));
				strcat(dest + strlen(dest), nPos);
			}
			else
			{
				LOG_ERR("szMsgType[%s] 解包错误",szMsgType);
				return FAILE;
			}
		}
		else
		{
			LOG_ERR("szMsgType[%s] 报文解析错误!",szMsgType);
			return FAILE;
		}
	}
	else
	{
		//Json::Value jsonRsp;
		Json::Value crmRsp;
		Json::FastWriter fast;
		std::string errCode;

		if((nPos = cTinyWrok.FindSubstr(src, (char * )"\r\n\r\n") ) != NULL)
		{
			LOG_INFO("nPos[%s]", nPos);
			Json::Reader reader;
			if(reader.parse(nPos, crmRsp))
			{
				//提取错误信息
				const Json::Value Msg = crmRsp["msg"];
#if 0
				Json::Value jsonRsp;
				jsonRsp["msg"] = Msg;
#endif
				//Json::Value arryBody = Msg["body"];
#if 0
				Json::Value arryBody;
				if(reader.parse("body", arryBody))
				{

					for (unsigned int j = 0; j < arryBody.size(); j++)
					{
						errCode += arryBody[j].asString();
						break;
					}
				}
#endif
			}
		}

#if 0
		Json::Value jsonRsp = makeErrJson(__FUNCTION__, szRspCode, errCode.c_str());
		Json::FastWriter fast;


		cTinyWrok.MakeService2AppRspHead(dest + strlen(dest), 1, fast.write(jsonRsp).size(), (char *)"200", (char *)"OK");
		strcat(dest, fast.write(jsonRsp).c_str());
#endif
		//jsonRsp[TAG_RESULT] = szRspCode;
		crmRsp[TAG_RESULT] = szRspCode;
		cTinyWrok.MakeService2AppRspHead(dest + strlen(dest), 1, fast.write(crmRsp).size(), (char *)"200", (char *)"OK");
		strcat(dest, fast.write(crmRsp).c_str());
		//sprintf(dest + strlen(dest), "{\"data\":%s,\"data_type\":0,\"isnull\": 1, \"%s\":\"%d\"}",nPos,TAG_RESULT, strErrCode.c_str());
		LOG_INFO("dest[%s]", dest);
	}
	//LOG_INFO("UserDepositRspApp srcid[%s] retcode[%s], retmsg[%s] sucess", szSrcId, szRspCode, szRspMsg);
	LOG_INFO("end dest[%s]", dest);
}

int CBusiness::UserDepositStatusReqCrm(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	LOG_INFO("src[%s]",src);
	nRoute = 1;
	memcpy(dest, src, nHeadLen);

	char szTmp[2048] = { 0 };
	char szMt4Acc[56] = { 0 };
	char szNationalId[56] = { 0 };

	//char szAppToken[APP_TOKEN_LEN + 1];
	//memset(szAppToken, 0x00, sizeof(szAppToken));


	char *nPos = strstr(src, TAG_HTTP);
	memcpy(szTmp, src, nPos - src - 1);
	strrpl(szTmp, szTmp, "%26", "&");
	LOG_DEBUG("szMsgType[%s] deposit status szTmp[%s]",szMsgType, szTmp);

	//cTinyWrok.GetTagVal(szTmp, (char*)TAG_APP_TOKEN, szAppToken, ' ');
	cTinyWrok.GetTagVal(szTmp, (char *)TAG_MT4ACC, szMt4Acc);
	cTinyWrok.GetTagVal(szTmp, (char *)TAG_NATIONAL_ID, szNationalId);

	//if ((nPos = strstr(g_AppToken, " ")) != NULL) *nPos = '\0'; //RTrim(g_AppToken);

	//if (g_AppToken[0] == '\0' || szMt4Acc[0] == '\0' || szNationalId[0] == '\0')
	if (strToken.empty() || szMt4Acc[0] == '\0' || szNationalId[0] == '\0')
	{
		LOG_ERR("szMsgType[%s] deposit status parse get parameter error",szMsgType);
		return FAILE;
	}
	LOG_DEBUG("szMsgType[%s] deposit status mt4acct[%s] token[%s],nationalid[%s]",szMsgType, szMt4Acc, strToken.c_str(), szNationalId);

	std::string urlPrefix;
	std::string strCrmToken;
	std::string strCode;
	std::string strReson;
	int ret;
	std::string Merchantkey ;
	std::string AppIp;
	std::string Domain ;
	char TmpMerchantkey[120] = { 0 };
	ret = GetUserTypeAndCrmToken((char *)strToken.c_str(), urlPrefix, strCrmToken, pRds, CrmConfig);
	LOG_DEBUG("szMsgType[%s] ret[%d]",szMsgType, ret);
	if(SUCESS != ret)
	{
		nRoute = 2;
		if(ret == atoi(ERR_OFFLINE))
		{
			strCode = ERR_OFFLINE;
			strReson = "user offline";
		}
		else if(ret == atoi(ERR_LOGIN_OTHMACHINE))
		{
			strCode = ERR_LOGIN_OTHMACHINE;
			strReson = "user login on other phone";
		}
		else
		{
			strCode = ERR_SERVER;
			strReson = "服务器繁忙,请稍后在试";
		}
		cTinyWrok.MakeService2AppRspErr(dest, strCode, strReson);
		goto LABLE;
	}

	strcat(dest, TAG_URL);
	strcat(dest, urlPrefix.c_str());
	//  strcat(dest, strCrm.c_str());
	strcat(dest, USER_DEPOSIT_STATUS_REQ_CRM);
	sprintf(dest + strlen(dest), "/%s|%s%smt4account:%s&%s%s",
			szNationalId, TAG_SND, TAG_MT4ACC, szMt4Acc, TAG_CRM_TOKEN, strCrmToken.c_str());

	if(src[MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN+1] == '1')
	{
		AppIp = CrmConfig.strIOS_AppIp;
		Domain = CrmConfig.strIOS_DoMain;
		Merchantkey = CrmConfig.strIOS_Merchantkey;
	}
	else
	{
		AppIp = CrmConfig.strAppIp;
		Domain = CrmConfig.strDoMain;
		Merchantkey = CrmConfig.strMerchantKey;
	}
	Base64_Encode(TmpMerchantkey,Merchantkey.c_str(), Merchantkey.size());
	sprintf(dest+strlen(dest),"&%s%s&%s%s%s&%s%s",TAG_MERCHANTKEY_GET,TmpMerchantkey,TAG_DOMAIN_GET,TAG_DOMAIN_POST, \
			Domain.c_str(),TAG_APPIP_GET,AppIp.c_str());
LABLE:
	LOG_INFO("deposit status buf[%s]", dest);

	return SUCESS;
}


int CBusiness::UserDepositUploadReqCrm(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	LOG_INFO("src[%s]",src);
	nRoute = 1;
	memcpy(dest, src, nHeadLen);


	int ret;
	std::string urlPrefix;
	std::string strCrmToken;
	std::string strCode;
	std::string strReson;
	char szMt4Acc[56] = { 0 };
	char *pMt4Acct = NULL;
	char *pAppToken = NULL;
	char *pSrcid = NULL;
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	char *nPos = NULL;
	std::string Merchantkey ;
	std::string AppIp;
	std::string Domain ;
	char TmpMerchantkey[120]= { 0 }  ; 
	nPos = strstr(src, TAG_MT4ACC_HIS);
	if (nPos == NULL)
	{
		LOG_ERR("szMsgType[%s] deposit upload parse get head mt4account",szMsgType);
		goto ERROR;
		//return FAILE;
	}
	cTinyWrok.GetTagVal(nPos, (char *)TAG_MT4ACC_HIS, szMt4Acc);

	pMt4Acct = eatSpace(szMt4Acc);

	//if (pMt4Acct[0] == '\0' || g_AppToken[0] == '\0')
	if (pMt4Acct[0] == '\0' || strToken.empty())
	{
		LOG_ERR("szMsgType[%s] deposit upload parse get parameter error,account[%s],token[%s]",szMsgType, pMt4Acct, strToken.c_str());
		goto ERROR;
		//return FAILE;
	}


	//ret = GetUserTypeAndCrmToken(g_AppToken, urlPrefix, strCrmToken);
	ret = GetUserTypeAndCrmToken((char *)strToken.c_str(), urlPrefix, strCrmToken, pRds, CrmConfig);
	LOG_DEBUG("szMsgType[%s] ret[%d]",szMsgType, ret);
	if(SUCESS != ret)
	{
		nRoute = 2;
		if(ret == atoi(ERR_OFFLINE))
		{
			strCode = ERR_OFFLINE;
			strReson = "user offline";
		}
		else if(ret == atoi(ERR_LOGIN_OTHMACHINE))
		{
			strCode = ERR_LOGIN_OTHMACHINE;
			strReson = "user login on other phone";
		}
		else
		{
			strCode = ERR_SERVER;
			strReson = "服务器繁忙,请稍后在试";
		}
		cTinyWrok.MakeService2AppRspErr(dest, strCode, strReson);
		goto LABLE;
	}

	LOG_DEBUG("szMsgType[%s] deposit upload mt4acct[%s] token[%s]url[%s]",szMsgType, pMt4Acct, strToken.c_str(), urlPrefix.c_str());
	strcat(dest, TAG_URL);
	strcat(dest, urlPrefix.c_str());
	//strcat(dest, strCrm.c_str());
	strcat(dest, USER_DEPOSIT_UPLOAD_REQ_CRM);
	sprintf(dest + strlen(dest), "|%s%smt4account:%s&%s%s&content-type=%s%s",
			TAG_SND, TAG_MT4ACC, pMt4Acct, TAG_CRM_TOKEN, strCrmToken.c_str(), "content-type:", g_ContentType);

	if(src[MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN+1] == '1')
	{
		AppIp = CrmConfig.strIOS_AppIp;
		Domain = CrmConfig.strIOS_DoMain;
		Merchantkey = CrmConfig.strIOS_Merchantkey;
	}
	else
	{
		AppIp = CrmConfig.strAppIp;
		Domain = CrmConfig.strDoMain;
		Merchantkey = CrmConfig.strMerchantKey;
	}
	Base64_Encode(TmpMerchantkey,Merchantkey.c_str(), Merchantkey.size());
	sprintf(dest+strlen(dest),"&%s%s&%s%s%s&%s%s",TAG_MERCHANTKEY_GET,TmpMerchantkey,TAG_DOMAIN_GET,TAG_DOMAIN_POST, \
			Domain.c_str(),TAG_APPIP_GET,AppIp.c_str());

LABLE:
	LOG_INFO("deposit upload buf[%s]", dest);

	return SUCESS;
ERROR:
	class CIpcMem mIpcMem;

	unsigned long long msgType = 0;
	//memcpy(szMsgType, src, MSG_TYPE_LEN);
	sscanf(szMsgType, "%016llu", &msgType);
	int key = msgType % 1000000000;
	LOG_ERR("msgtype[%d], key[%d]", msgType, key);
	if (mIpcMem.Create(key) != 0)
	{
		mIpcMem.Detach();
		mIpcMem.Delete();
	}

	return FAILE;
}

int CBusiness::UserGetnameReqCrm(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	nRoute = 1;
	LOG_INFO("src[%s]",src);
	//RTrim(g_AppToken);
	if(strToken.empty())
	{
		LOG_ERR("szMsgType[%s] appToken is null",szMsgType);
		return FAILE;
	}
	LOG_DEBUG("szMsgType[%s] getname token[%s]",szMsgType, strToken.c_str());
	std::string Mt4Acct = " ";
	std::string strMt4 = pRds.get(strToken);
	if(!strMt4.empty())
	{
		Json::Reader mt4reader;
		Json::Value mt4value;
		if(mt4reader.parse(strMt4, mt4value))
		{
			const Json::Value arrayMt4Acc = mt4value["mt4accounts"];
			if(arrayMt4Acc.size() != 0)
			{
				for (unsigned int i = 0; i < arrayMt4Acc.size(); i++)
				{
					Mt4Acct = arrayMt4Acc[i].asString();
					//LOG_INFO("MT4[%s]",strMt4Acc[i].c_str());
					break;
				}
			}
			//Mt4Acct = arrayMt4Acc[0].asString();
			LOG_DEBUG("szMsgType[%s] MT4[%s]",szMsgType, Mt4Acct.c_str());
		}

	}
	if (Mt4Acct.empty())
	{
		LOG_ERR("szMsgType[%s] get name parse get parameter error,account[%s],token[%s]",szMsgType, Mt4Acct.c_str(), strToken.c_str());
		return FAILE;
	}
	memcpy(dest, src, nHeadLen);
	std::string urlPrefix;
	std::string strCrmToken;
	std::string strCode;
	std::string strReson;
	int ret;

	std::string Merchantkey ;
	std::string AppIp;
	std::string Domain ;
	char TmpMerchantkey[120] = { 0 };

	ret = GetUserTypeAndCrmToken((char *)strToken.c_str(), urlPrefix, strCrmToken, pRds, CrmConfig);
	LOG_DEBUG("szMsgType[%s] ret[%d]",szMsgType, ret);
	if(SUCESS != ret)
	{
		nRoute = 2;
		if(ret == atoi(ERR_OFFLINE))
		{
			strCode = ERR_OFFLINE;
			strReson = "user offline";
		}
		else if(ret == atoi(ERR_LOGIN_OTHMACHINE))
		{
			strCode = ERR_LOGIN_OTHMACHINE;
			strReson = "user login on other phone";
		}
		else
		{
			strCode = ERR_SERVER;
			strReson = "服务器繁忙,请稍后在试";
		}
		cTinyWrok.MakeService2AppRspErr(dest, strCode, strReson);
		goto LABLE;
	}

	strcat(dest, TAG_URL);
	strcat(dest, urlPrefix.c_str());
	//strcat(dest, strCrm.c_str());
	strcat(dest, USER_NAME_CRM);
	//sprintf(dest,"%s/%s",dest,Mt4Acct.c_str());
	sprintf(dest + strlen(dest), "|%s%s%s&%s%s%s", TAG_MT4ACC, szSrcId, Mt4Acct.c_str(), TAG_SND, TAG_CRM_TOKEN, strCrmToken.c_str());

	if(src[MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN+1] == '1')
	{
		AppIp = CrmConfig.strIOS_AppIp;
		Domain = CrmConfig.strIOS_DoMain;
		Merchantkey = CrmConfig.strIOS_Merchantkey;
	}
	else
	{
		AppIp = CrmConfig.strAppIp;
		Domain = CrmConfig.strDoMain;
		Merchantkey = CrmConfig.strMerchantKey;
	}
	Base64_Encode(TmpMerchantkey,Merchantkey.c_str(), Merchantkey.size());
	sprintf(dest+strlen(dest),"&%s%s&%s%s%s&%s%s",TAG_MERCHANTKEY_GET,TmpMerchantkey,TAG_DOMAIN_GET,TAG_DOMAIN_POST, \
			Domain.c_str(),TAG_APPIP_GET,AppIp.c_str());
LABLE:
	LOG_INFO("getname buf[%s]", dest);

	return SUCESS;
}

int CBusiness::UserNameRspApp(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	nRoute = 2;
	char *nPos = NULL;

	memset(dest, 0x00, strlen(dest));

	nPos = strstr(src, "\r\n\r\n");
	memcpy(dest, src, nHeadLen);
	if (nPos != NULL)
	{
		cTinyWrok.MakeService2AppRspHead(dest + strlen(dest), 1, strlen(nPos) - 4, (char *)"200", (char *)"OK");
		strcat(dest, nPos + 4);
	}
	else
	{
		LOG_ERR("szMsgType[%s] withdraw history rsp parse packet error",szMsgType);
		std::string errcode;
		if (GetHttpErrorCode(src, errcode) == 0)
		{
			cTinyWrok.MakeService2AppRspErr(dest, errcode);
		}
		else
		{
			return FAILE;
		}
	}

	LOG_INFO("szMsgType[%s] withdraw history rsp buf[%s]",szMsgType, dest);
	return SUCESS;

}

int CBusiness::UserSetnameReqCrm(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	LOG_INFO("src[%s]",src);
	nRoute = 1;
	char szChineseName[64] = {0};
	memcpy(dest, src, nHeadLen);
	if (strToken.empty())
	{
		LOG_ERR("szMsgType[%s] set name err apptoken is null",szMsgType);
		return FAILE;
	}
	//RTrim(g_AppToken);
	LOG_DEBUG("name token[%s]", strToken.c_str());


	std::string Mt4Acct = " ";
	std::string strMt4 = pRds.get(strToken);
	if(!strMt4.empty())
	{
		Json::Reader mt4reader;
		Json::Value mt4value;
		if(mt4reader.parse(strMt4, mt4value))
		{
			const Json::Value arrayMt4Acc = mt4value["mt4accounts"];
			if(arrayMt4Acc.size() != 0)
			{
				for (unsigned int i = 0; i < arrayMt4Acc.size(); i++)
				{
					Mt4Acct = arrayMt4Acc[i].asString();
					break;
				}
			}
			Mt4Acct = szSrcId + Mt4Acct;
			LOG_INFO("MT4[%s]", Mt4Acct.c_str());
		}

	}
	std::string Merchantkey ;
	std::string AppIp;
	std::string Domain ;
	char TmpMerchantkey[120] = { 0 };
	if(src[MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN+1] == '1')
	{
		AppIp = CrmConfig.strIOS_AppIp;
		Domain = CrmConfig.strIOS_DoMain;
		Merchantkey = CrmConfig.strIOS_Merchantkey;
	}
	else
	{
		AppIp = CrmConfig.strAppIp;
		Domain = CrmConfig.strDoMain;
		Merchantkey = CrmConfig.strMerchantKey;
	}
	cTinyWrok.GetTagVal(src, (char *)TAG_ZHNAME, szChineseName, ' ');
	LOG_DEBUG("CHINESE NAME[%s]", szChineseName);

	std::string urlPrefix;
	std::string strCrmToken;
	std::string strCode;
	std::string strReson;
	int ret;

	ret = GetUserTypeAndCrmToken((char *)strToken.c_str(), urlPrefix, strCrmToken, pRds, CrmConfig);
	LOG_DEBUG("szMsgType[%s] ret[%d]",szMsgType, ret);
	if(SUCESS != ret)
	{
		nRoute = 2;
		if(ret == atoi(ERR_OFFLINE))
		{
			strCode = ERR_OFFLINE;
			strReson = "user offline";
		}
		else if(ret == atoi(ERR_LOGIN_OTHMACHINE))
		{
			strCode = ERR_LOGIN_OTHMACHINE;
			strReson = "user login on other phone";
		}
		else
		{
			strCode = ERR_SERVER;
			strReson = "服务器繁忙,请稍后在试";
		}
		cTinyWrok.MakeService2AppRspErr(dest, strCode, strReson);
		goto LABLE;
	}

	strcat(dest, TAG_URL);
	strcat(dest, urlPrefix.c_str());
	//strcat(dest, strCrm.c_str());
	strcat(dest, USER_SETNAME_CRM);
	Base64_Encode(TmpMerchantkey,Merchantkey.c_str(), Merchantkey.size());
	sprintf(dest + strlen(dest), "|%s%smt4account:%s&%s%s&%s%s&%s%s%s&%s%s|%s%s%s", TAG_SND, TAG_MT4ACC, Mt4Acct.c_str(), TAG_CRM_TOKEN, strCrmToken.c_str(), \
			TAG_MERCHANTKEY_GET,TmpMerchantkey,TAG_DOMAIN_GET,TAG_DOMAIN_POST, \
			Domain.c_str(),TAG_APPIP_GET,AppIp.c_str(), TAG_POST, TAG_ZHNAME, szChineseName);

LABLE:
	LOG_INFO("getname buf[%s]", dest);

	return SUCESS;
}

int CBusiness::UserProfileDataReqCrm(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	LOG_INFO("src[%s]",src);
	//LOG_INFO("acquire user profile");
	nRoute = 1;
	memcpy(dest, src, nHeadLen);

	char szTmp[2048] = { 0 };
	char szLang[56] = { 0 };

	char *nPos = strstr(src, TAG_HTTP);
	memcpy(szTmp, src, nPos - src - 1);
	//LOG_INFO("acquire user profile szTmp[%s]", szTmp);
	strrpl(szTmp, szTmp, "%26", "&");
	//LOG_DEBUG("acquire user profile szTmp[%s]", szTmp);

	//cTinyWrok.GetTagVal(szTmp, (char*)TAG_LANGUAGE, szLang);
	cTinyWrok.GetTagVal(szTmp, (char *)TAG_LANG, szLang);

	//if ((nPos = strstr(g_AppToken, " ")) != NULL) *nPos = '\0';

	//RTrim(g_AppToken);

	//if (g_AppToken[0] == '\0')
	if (strToken.empty())
	{
		LOG_ERR("szMsgType[%s] acquire user profile apptoken or mt4 acct error",szMsgType);
		return FAILE;
	}
	LOG_DEBUG("szMsgType[%s] acquire user profile   token[%s]",szMsgType, strToken.c_str());

	std::string urlPrefix;
	std::string strCrmToken;
	std::string strCode;
	std::string strReson;
	int ret;

	std::string Merchantkey ;
	std::string AppIp;
	std::string Domain ;
	char TmpMerchantkey[120] = { 0 };
	if(src[MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN+1] == '1')
	{
		AppIp = CrmConfig.strIOS_AppIp;
		Domain = CrmConfig.strIOS_DoMain;
		Merchantkey = CrmConfig.strIOS_Merchantkey;
	}
	else
	{
		AppIp = CrmConfig.strAppIp;
		Domain = CrmConfig.strDoMain;
		Merchantkey = CrmConfig.strMerchantKey;
	}
	//ret = GetUserTypeAndCrmToken(g_AppToken, urlPrefix, strCrmToken);
	ret = GetUserTypeAndCrmToken((char *)strToken.c_str(), urlPrefix, strCrmToken, pRds, CrmConfig);
	LOG_DEBUG("szMsgType[%s] ret[%d]",szMsgType, ret);
	if(SUCESS != ret)
	{
		nRoute = 2;
		if(ret == atoi(ERR_OFFLINE))
		{
			strCode = ERR_OFFLINE;
			strReson = "user offline";
		}
		else if(ret == atoi(ERR_LOGIN_OTHMACHINE))
		{
			strCode = ERR_LOGIN_OTHMACHINE;
			strReson = "user login on other phone";
		}
		else
		{
			strCode = ERR_SERVER;
			strReson = "服务器繁忙,请稍后在试";
		}
		cTinyWrok.MakeService2AppRspErr(dest, strCode, strReson);
		goto LABLE;
	}
	strcat(dest, TAG_URL);
	strcat(dest, urlPrefix.c_str());
	//strcat(dest, strCrm.c_str());
	strcat(dest, USER_PROFILE_DATA);
	//strcat(dest, "?");
	sprintf(dest + strlen(dest), "|%s%s%s", TAG_SND, TAG_CRM_TOKEN, strCrmToken.c_str());
	if (szLang[0] != '\0')
	{
		//sprintf(dest + strlen(dest), "&%sLang:%s", TAG_LANGUAGE, szLang);
		sprintf(dest + strlen(dest), "&%s%s%s", TAG_LANG, TAG_LANGUAGE_HEAD_CRM, szLang);
	}
	Base64_Encode(TmpMerchantkey,Merchantkey.c_str(), Merchantkey.size());
	sprintf(dest+strlen(dest),"&%s%s&%s%s%s&%s%s",TAG_MERCHANTKEY_GET,TmpMerchantkey,TAG_DOMAIN_GET,TAG_DOMAIN_POST, \
			Domain.c_str(),TAG_APPIP_GET,AppIp.c_str());
LABLE:
	LOG_INFO("szMsgType[%s] acquire user profile buf[%s]",szMsgType, dest);

	return SUCESS;
}

int CBusiness::UserProfileDataRspApp(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	nRoute = 2;
	char *nPos = NULL;

	memset(dest, 0x00, strlen(dest));

	nPos = strstr(src, "\r\n\r\n");
	memcpy(dest, src, nHeadLen);
	if (nPos != NULL)
	{
		cTinyWrok.MakeService2AppRspHead(dest + strlen(dest), 1, strlen(nPos) - 4, (char *)"200", (char *)"OK");
		strcat(dest, nPos + 4);
	}
	else
	{
		LOG_ERR("szMsgType[%s] profile data rsp parse packet error",szMsgType);
		std::string errcode;
		if (GetHttpErrorCode(src, errcode) == 0)
		{
			cTinyWrok.MakeService2AppRspErr(dest, errcode);
		}
		else
		{
			return FAILE;
		}
	}

	LOG_INFO("szMsgType[%s] profile data rsp buf[%s]",szMsgType, dest);
	return SUCESS;
}

int CBusiness::UserArchiveOnline(char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, src, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	nRoute = 2;
#if 0
	//szSrcId
	//取token
	Json::FastWriter fast;
	Json::Value jsonRsp;
	jsonRsp["result"] = Json::Value("0");
	jsonRsp["onlinenums"] = Json::Value(888);

	memcpy(dest, src, nHeadLen);
	cTinyWrok.MakeService2AppRspHead(dest + strlen(dest), 1, fast.write(jsonRsp).size(), (char *)"200", (char *)"OK");
	strcat(dest + strlen(dest), fast.write(jsonRsp).c_str());
#endif

	nRoute = 2;
	Json::Value jsonRsp;
	Json::Reader read;
	Json::Value value;
	std::string str_online;
#if 0
	for (size_t i = 0; i < MAX_CRM_API_COUNT; i++)
	{
		if (g_CrmApi[i].strCompany.compare(szSrcId) == 0)
		{
			if(g_CrmApi[i].strProUser.compare("0") == 0 )
			{
				jsonRsp["result"] = Json::Value(g_CrmApi[i].strProUser);
				jsonRsp["onlinenums"] = 0;
				goto LABLE;
			}
		}
	}
#endif
	std::string online_key ;

	if(CrmConfig.strProUser.compare("0") == 0)
	{
		jsonRsp["result"] = Json::Value(CrmConfig.strProUser);
		jsonRsp["onlinenums"] = 0;
		goto LABLE;
	}
	online_key = std::string(szSrcId) + "currentonline";
	str_online = pRds.get(online_key);
	jsonRsp["result"] = Json::Value(CrmConfig.strProUser);
	jsonRsp["onlinenums"] =  atoi(str_online.c_str());
#if 0
	str_online = pRds.get(ONLINE_USERS);
	if(!str_online.empty())
	{
		if(read.parse(str_online, value))
		{
			const Json::Value read_company = value[ONLINE_COMPANY];
			for(size_t i = 0; i < read_company.size(); i++)
			{
				std::string company_id = read_company[i]["company_id"].asString();
				if(company_id.compare(szSrcId) == 0)
				{
					jsonRsp["result"] = Json::Value(g_CrmApi[i].strProUser);
					jsonRsp["onlinenums"] = read_company[i]["vir_onlineusers"].asInt();
					goto LABLE;
				}

			}
		}

	}
	LOG_ERR("can't find onlinenums") ;
	jsonRsp["result"] = Json::Value("400");
	jsonRsp["onlinenums"] = 0;
#endif

LABLE:


	Json::FastWriter fast;
	memcpy(dest, src, nHeadLen);
	cTinyWrok.MakeService2AppRspHead(dest + strlen(dest), 1, fast.write(jsonRsp).size(), (char *)"200", (char *)"OK");
	strcat(dest + strlen(dest), fast.write(jsonRsp).c_str());

	LOG_INFO("UserArchiveOnline srcid[%s] dest[%s]", szSrcId, dest);
	return SUCESS;

}

int CBusiness::ReloadConfigParam( char *src, char *dest, int &nRoute, std::string strToken, CRedis &pRds, char *szSrcId, stCrmApi &CrmConfig )
{
	nRoute = 2;
	Json::Value value;
	Json::FastWriter fast;
	int nRet = LoadCrmConfig(pRds);
	memcpy(dest, src, nHeadLen);
	if(nRet != 0)
	{
		value["status"] = Json::Value("404");
		value["reslut"] = Json::Value("configure parameter could't find");
		cTinyWrok.MakeService2AppRspHead(dest + strlen(dest), 1, fast.write(value).size(), (char *)"404", (char *)"Not Found");
		return SUCESS;
	}
	value["status"] = Json::Value("200");
	value["reslut"] = Json::Value("sucess");
	cTinyWrok.MakeService2AppRspHead(dest + strlen(dest), 1, fast.write(value).size(), (char *)"200", (char *)"OK");
	strcat(dest + strlen(dest), fast.write(value).c_str());
	LOG_INFO("Reload configure parameter dest[%s]", dest);
	return SUCESS;
}
void CBusiness::Init()
{

	//init service api
	m_Apimap.clear();

	m_Apimap.insert(pair<std::string, busifun>("/App/register"            ,          &CBusiness::UserRegisterReqCrm));
	m_Apimap.insert(pair<std::string, busifun>(USER_REGISTER_CRM          ,          &CBusiness::UserRegisterRspApp));
	m_Apimap.insert(pair<std::string, busifun>("/App/login"               ,          &CBusiness::UserLoginReqCrm));
	m_Apimap.insert(pair<std::string, busifun>(USER_LOGIN_CRM             ,          &CBusiness::UserLoginRspApp));
	m_Apimap.insert(pair<std::string, busifun>("/App/historical"          ,          &CBusiness::UserhistoryReqCrm));
	m_Apimap.insert(pair<std::string, busifun>(USER_GETHIS_TIME_CRM       ,          &CBusiness::UserhistoryRspApp));
	m_Apimap.insert(pair<std::string, busifun>(USER_GETHIS_COUNT_CRM      ,          &CBusiness::UserhistoryRspApp));
	m_Apimap.insert(pair<std::string, busifun>("/App/MT4verify"           ,          &CBusiness::UserAuthReqCrm));
	m_Apimap.insert(pair<std::string, busifun>(USER_AUTH_CRM              ,          &CBusiness::UserAuthRspApp));
	m_Apimap.insert(pair<std::string, busifun>("/App/forpwd"              ,          &CBusiness::UserForPwdReqCrm));
	m_Apimap.insert(pair<std::string, busifun>(USER_FORPWD_CRM            ,          &CBusiness::UserForPwdRspApp));
	m_Apimap.insert(pair<std::string, busifun>("/App/tradehistory"        ,          &CBusiness::UserTradehistoryReqCrm));
	m_Apimap.insert(pair<std::string, busifun>(USER_GETTRDHIS__CRM        ,          &CBusiness::UserTradehistoryRspApp));
	m_Apimap.insert(pair<std::string, busifun>("/App/parameter"           ,          &CBusiness::UserTmpregisterReqCrm));
	m_Apimap.insert(pair<std::string, busifun>(USER_REGISTER_TMP_CRM      ,          &CBusiness::UserTmpregisterRspApp));
	m_Apimap.insert(pair<std::string, busifun>("/App/archivetrade"        ,          &CBusiness::UserArchiveTradeReqCrm));
	m_Apimap.insert(pair<std::string, busifun>(USER_GETARCHTRA__CRM       ,          &CBusiness::UserArchiveTradeRspApp));
	m_Apimap.insert(pair<std::string, busifun>("/App/withdrawtemplate"    ,          &CBusiness::UserWithdrawTemplateReqCrm));
	m_Apimap.insert(pair<std::string, busifun>("/App/withdraw"            ,          &CBusiness::UserWithdrawReqCrm));
	m_Apimap.insert(pair<std::string, busifun>(USER_GETWITHDRAW_CRM       ,          &CBusiness::UserWithdrawRspApp));
	m_Apimap.insert(pair<std::string, busifun>(USER_GETWITHDRAW_CRM_TMP   ,          &CBusiness::UserWithdrawRspApp));
	m_Apimap.insert(pair<std::string, busifun>("/App/v1/withdraw/hitory"  ,          &CBusiness::UserWithdrawHistoryReqCrm));
	m_Apimap.insert(pair<std::string, busifun>(USER_WITHDRAW_REQ_CRM      ,          &CBusiness::UserWithdrawHistoryRspApp));
	m_Apimap.insert(pair<std::string, busifun>("/App/v1/withdraw/del"     ,          &CBusiness::UserWithdrawDeleteReqCrm));
	m_Apimap.insert(pair<std::string, busifun>("/App/deposittemplate"     ,          &CBusiness::UserDepositTemplateReqCrm));
	m_Apimap.insert(pair<std::string, busifun>("/App/deposit/commit"      ,          &CBusiness::UserDepositReqCrm));
	m_Apimap.insert(pair<std::string, busifun>(USER_GETDEPOSIT_REQ_CRM    ,          &CBusiness::UserDepositRspApp));
	m_Apimap.insert(pair<std::string, busifun>("/App/deposit/status"      ,          &CBusiness::UserDepositStatusReqCrm));
	m_Apimap.insert(pair<std::string, busifun>(USER_DEPOSIT_STATUS_REQ_CRM,          &CBusiness::UserDepositRspApp));
	m_Apimap.insert(pair<std::string, busifun>("/App/deposit/upload"      ,          &CBusiness::UserDepositUploadReqCrm));
	m_Apimap.insert(pair<std::string, busifun>(USER_DEPOSIT_UPLOAD_REQ_CRM,          &CBusiness::UserDepositRspApp));
	m_Apimap.insert(pair<std::string, busifun>("/App/getname"             ,          &CBusiness::UserGetnameReqCrm));
	m_Apimap.insert(pair<std::string, busifun>("/App/setname"             ,          &CBusiness::UserSetnameReqCrm));
	m_Apimap.insert(pair<std::string, busifun>(USER_NAME_CRM              ,          &CBusiness::UserNameRspApp));
	m_Apimap.insert(pair<std::string, busifun>(USER_SETNAME_CRM           ,          &CBusiness::UserNameRspApp));
	m_Apimap.insert(pair<std::string, busifun>("/App/getprofile"          ,          &CBusiness::UserProfileDataReqCrm));
	m_Apimap.insert(pair<std::string, busifun>(USER_PROFILE_DATA          ,          &CBusiness::UserProfileDataRspApp));
	m_Apimap.insert(pair<std::string, busifun>("/App/getonlineUsers"      ,          &CBusiness::UserArchiveOnline));
	m_Apimap.insert(pair<std::string, busifun>("/App/uploadlog"           ,          &CBusiness::UserUploadlogRspApp));
	m_Apimap.insert(pair<std::string, busifun>("/App/heart"               ,          &CBusiness::UserHeartRspApp));
	m_Apimap.insert(pair<std::string, busifun>("/App/agreement"           ,          &CBusiness::UserAgreementRspApp));
	m_Apimap.insert(pair<std::string, busifun>("/App/awaretrading"        ,          &CBusiness::UserawaretradingRspApp));
	m_Apimap.insert(pair<std::string, busifun>("/App/logout"              ,          &CBusiness::UserLogoutRspApp));
	m_Apimap.insert(pair<std::string, busifun>("/App/appverification"     ,          &CBusiness::UserAuthenticateRspApp));
	m_Apimap.insert(pair<std::string, busifun>("/App/getcrmtoken"         ,          &CBusiness::UserCrmTokenRspApp));
	m_Apimap.insert(pair<std::string, busifun>("/Management/Reloadconfigparam"   ,          &CBusiness::ReloadConfigParam));
}
