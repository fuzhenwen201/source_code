#include "../../include/HttpCliBusiness.h"

char* gMerchantKey = NULL;
char* gAppIp = NULL;
char* gDoMain = NULL;
char* g_Url_Crm_My = NULL;



int Equal(char a,char b)
{
	if(a>='A'&&a<='Z') a+='a'-'A';
	if(b>='A'&&b<='Z') b+='a'-'A';
	return a==b;
}

char* FindSubstr(char* s,char* sub)
{
	char *p,*q;
	while(*s)
	{   
		for(p=s,q=sub; *q && Equal(*p, *q); p++,q++);
		if(*q=='\0') return s+strlen(sub);
		s++;
	}   
	return 0;
}

int FindChar(char* s,char ch)
{
	for(int i=0;s[i];i++)
	{
		if(ch==s[i])
			return i;
	}
	return 0;
}

void GetTagVal(char* buf, char* tag, char* dest, char end='\0')
{
	char* sPos = NULL;
	//LOG_INFO("buf[%s],tag[%s],dest[%s]", buf,tag,dest);
	if(sPos = FindSubstr(buf, tag))
	{
		// LOG_INFO("sPos[%s]", sPos);
		if(end == '\0')
		{
			while(*sPos && *sPos != '\r' && *sPos != '\n' && *sPos != '|' && *sPos != '?')
			{
				*dest = *sPos;
				dest++;
				sPos++;
			}
		}
		else
		{
			while(*sPos && *sPos != '\r' && *sPos != '\n' && *sPos != '|' && *sPos != end && *sPos != ' ' && *sPos != '&')
			{
				*dest = *sPos;
				dest++;
				sPos++;
			}
		}

		//LOG_INFO("buf[%s],tag[%s]", buf,tag);
	}
}

void HttpGetTagVal(char* buf, char* tag, char* dest, char end = '\0')
{
	char* sPos = NULL;
	// LOG_INFO("buf[%s],tag[%s],dest[%s]", buf,tag,dest);
	if (sPos = FindSubstr(buf, tag))
	{
		// LOG_INFO("sPos[%s]", sPos);
		if (end == '\0')
		{
			while (*sPos && *sPos != '\r' && *sPos != '\n' && *sPos != '|')
			{
				*dest = *sPos;
				dest++;
				sPos++;
			}
		}
		else
		{
			while (*sPos && *sPos != '\r' && *sPos != '\n' && *sPos != '|' && *sPos != end && *sPos != '\0')
			{
				*dest = *sPos;
				dest++;
				sPos++;
			}
		}
	}
}

void GetKeyFrText(char* buf, char tag , char* dest,char end )
{
	char* sPos = NULL;
	char* src = buf;
	int count = 0;
	while(*src != '\0' && count !=3)
	{
		if (*src++ == tag)
			count++;	 
	}
	src--;
	while(*src != end && *src !='?' && *src != '\0')
	{
		*dest++ = *src++;
	}

}


size_t OnWriteData_Post(void* buffer, size_t size, size_t nmemb, void* lpVoid)  
{
	std::string* str = reinterpret_cast<std::string*>(lpVoid);
	if (NULL == str || NULL == buffer)
	{
		return -1;
	}

	char* pData = reinterpret_cast<char*>(buffer);
	str->append(pData, size * nmemb);

	return nmemb;
}
static int g_size = 0;
size_t OnReadData_Post(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
	size_t real_size = nmemb * size;	
	char *src = (char*)buffer;
	pIpcMemBuf pMembuf = (pIpcMemBuf)lpVoid;
	char *dest = (char*)(pMembuf->mtext+g_size);
	size_t templen = pMembuf->msize-g_size ;
	if(templen <= real_size)
		real_size = templen;
	LOG_INFO("pMembuf->msize:[%d],real_size :[%d]",pMembuf->msize,real_size);
	g_size += real_size;
	memcpy(src,dest,real_size);
	//strncat(src,dest,real_size);
	//src[real_size] = '\0';
	return real_size;
}
HttpCliBusiness::HttpCliBusiness()
{

}

HttpCliBusiness::~HttpCliBusiness()
{

}

void HttpCliBusiness::InitHttpClient()
{
	// Get Path
	 if((gMerchantKey = getenv("MERCHANT_kEY")) == NULL)
		LOG_ERR("get MERCHANTkEY err!");

	if((gAppIp = getenv("APP_IP")) == NULL)
		LOG_ERR("get APPIP err!");

 	if((gDoMain = getenv("DOMAIN")) == NULL)
		LOG_ERR("get DOMAIN err!");

	if((g_Url_Crm_My = getenv("URL_CRM_MY"))==NULL)
		LOG_ERR("get env URL_CRM_MY err!");

	LOG_INFO("merchkey[%s],appip[%s],domain[%s],g_Url_Crm_My[%s]", gMerchantKey,gAppIp,gDoMain,g_Url_Crm_My);
	// init  
	m_FunMap.insert(make_pair(USER_LOGIN_CRM,&HttpCliBusiness::LoginCrm));
	m_FunMap.insert(make_pair(USER_AUTH_CRM,&HttpCliBusiness::AuthCrm));
	m_FunMap.insert(make_pair(USER_REGISTER_CRM,&HttpCliBusiness::RegisterCrm));
	m_FunMap.insert(make_pair(USER_FORPWD_CRM,&HttpCliBusiness::ForPwdCrm));
	m_FunMap.insert(make_pair(USER_GETHIS_COUNT_CRM,&HttpCliBusiness::UserGetHisCoutCrm));
	m_FunMap.insert(make_pair(USER_GETHIS_TIME_CRM,&HttpCliBusiness::UserGetHisTimeCrm));
	m_FunMap.insert(make_pair(USER_GETTRDHIS__CRM,&HttpCliBusiness::UserGetTrdhisCrm));
	m_FunMap.insert(make_pair(USER_GETARCHTRA__CRM,&HttpCliBusiness::UserGetArchtraCrm));

	m_FunMap.insert(make_pair(USER_GETWITHDRAW_CRM,&HttpCliBusiness::UserGetWithdrawCrm));
	m_FunMap.insert(make_pair(USER_GETWITHDRAW_CRM_TMP,&HttpCliBusiness::UserGetWithdrawCrmTmp));
	m_FunMap.insert(make_pair(USER_WITHDRAW_REQ_CRM,&HttpCliBusiness::UserWithdrawReq));
	m_FunMap.insert(make_pair(USER_GETDEPOSIT_REQ_CRM,&HttpCliBusiness::UserGetDepostReqCrm));
	m_FunMap.insert(make_pair(USER_DEPOSIT_STATUS_REQ_CRM,&HttpCliBusiness::UserDepostStatusReqCrm));
	m_FunMap.insert(make_pair(USER_DEPOSIT_UPLOAD_REQ_CRM,&HttpCliBusiness::UserDepostUploadReqCrm));
	m_FunMap.insert(make_pair(USER_REGISTER_TMP_CRM,&HttpCliBusiness::UserRegisterTmpCrm));
	m_FunMap.insert(make_pair(USER_NAME_CRM,&HttpCliBusiness::UserNameCrm));
	m_FunMap.insert(make_pair(USER_SETNAME_CRM,&HttpCliBusiness::UserSetNameCrm));
	m_FunMap.insert(make_pair(USER_PROFILE_DATA,&HttpCliBusiness::UserProfileDataCrm));
}

int HttpCliBusiness::RequestHttpCli(char* pParam,char* Url, string& strResponse)
{
	if(NULL == Url || pParam == NULL)
	{
		LOG_ERR("URL is NULL");
		return -1;
	}
	LOG_INFO("pParam :[%s]",pParam);
	int nRet = 0;
	HttpClient httpclient;
	httpclient.Create();
	char key[50] = { 0 };
	GetKeyFrText(pParam,'/',key,'|');
	LOG_INFO("key [%s]",key);

	map<string,pFun>::iterator iter = m_FunMap.find(key);
	if(iter != m_FunMap.end())
	{
		nRet = (this->*(iter->second))(pParam,Url,&httpclient,strResponse);
	}
	return nRet;
}

int HttpCliBusiness::LoginCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse)
{
	//char Url[128] = {0};
	char szLang[36] = {0};
	char SndTag[2048] = { 0 };
	char szPwd[CRM_PWD_LEN+1] = {0}; 
	char szTmpPwd[CRM_PWD_LEN+1] = {0};
	char SndTagTmp[2048] = {0};
	char szEmail[CRM_USER_LEN+1] = {0}; 
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
    char szDomain[50] = { 0 };
    char szAppIp[50] = { 0 };
	GetTagVal(pParam, (char*)TAG_URL, Url);
	GetTagVal(pParam, (char*)TAG_SND, SndTagTmp);
	GetTagVal(SndTagTmp, (char*)TAG_EMAIL, szEmail, '&');
	GetTagVal(SndTagTmp, (char*)TAG_PWD, szTmpPwd, '&');
	Base64_Decode(szPwd, szTmpPwd, strlen(szTmpPwd));
	sprintf(SndTag, "email=%s&password=%s", szEmail, szPwd);
	LOG_INFO("URL[%s],SndTaTmpg[%s],mail[%s]pwd[%s]pwd[%s]sndtag[%s]", Url, SndTagTmp,szEmail, szTmpPwd,szPwd,SndTag);
	if(Url[0] == '\0' || SndTag[0] == '\0')
	{
		LOG_INFO("get tag err!");
			return -1;
	}
	HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
    //LOG_INFO("Tmpmerchantkey[%s]",szTmpMerchantkey);
    Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
    //LOG_INFO("merchantkey[%s]",szMerchantKey);
    HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
    HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
	//set headers 
	HttpGetTagVal((char *)pParam, (char *)TAG_LANG, szLang, '&');
	pClient->SetHeaders("Accept: */*");
	pClient->SetHeaders("User-Agent: NetworkTest/1.0 (iPhone; iOS 9.3.2; Scale/2.00");
	pClient->SetHeaders("Accept-Encoding: gzip, deflate");
	pClient->SetHeaders(szMerchantKey);
	pClient->SetHeaders(szDomain);
	pClient->SetHeaders(szAppIp);
	pClient->SetHeaders(szLang);
	// set body
	//snprintf(SndTag,sizeof(SndTag),"%s&timestamp=%d",SndTag,time(NULL));
	char current_time[128] = { 0 };
	sprintf(current_time,"&timestamp=%d",time(NULL));
	strcat(SndTag, current_time);
	pClient->Post(Url, SndTag, strlen(SndTag));
	// callback function 
	pClient->SettingBuffer(NULL,NULL,OnWriteData_Post,(void *)&strResponse);
	// set culr 
	pClient->SettingCURL();
	// Run 
	pClient->Run();
	//HTTP/1.1 303 See Other
	char* nPos = NULL;
	string strLine = strResponse.substr(0,strResponse.find('\n'));
	
	LOG_INFO("first line[%s]", strLine.c_str());
	if(strLine != "" && strstr(strLine.c_str(), "303"))
	{
		strResponse = "";
		if((nPos = strstr(Url, "my")) != NULL) 
		{
			nPos[0] = 'h';
			LOG_INFO("URL[%s]", Url);
			pClient->Post(Url,SndTag, strlen(SndTag));
			pClient->Run();
		}
		else if((nPos = strstr(Url, "hy")) != NULL)
		{
			nPos[0] = 'm';
			LOG_INFO("URL[%s]", Url);
			pClient->Post(Url,SndTag, strlen(SndTag));
			pClient->Run();
		}
	}
	else if (strLine != "" && (strstr(strLine.c_str(),"301") || strstr(strLine.c_str(),"302")) )
	{
		  
	}
	return 0;
			
}

int HttpCliBusiness::AuthCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse)
{
	 
	//char Url[128] = {0};
	char szLang[32] = {0};
	char SndTag[2048] = { 0 };
	char SndTagTmp[2048] = {0};
	char szUser[MT4_USER_LEN+1] = {0}; 
	char szPwd[MT4_PWD_LEN+1] = {0}; 
	char szTmpPwd[MT4_PWD_LEN+1] = {0};
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
    char szDomain[50] = { 0 };
    char szAppIp[50] = { 0 };

	GetTagVal(pParam, (char*)TAG_URL, Url);
	LOG_INFO("URL:[%s]",Url);
	GetTagVal(pParam, (char*)TAG_SND, SndTagTmp);
	GetTagVal(SndTagTmp, (char*)TAG_USERNAME, szUser, '&');
	GetTagVal(SndTagTmp, (char*)TAG_PWD, szTmpPwd, '&');
	Base64_Decode(szPwd, szTmpPwd, strlen(szTmpPwd));
	sprintf(SndTag, "grant_type=password&username=%s&password=%s&timestamp=%d", szUser, szPwd,time(NULL));
	LOG_INFO("URL[%s],username[%s],password[%s],SndTag[%s]", Url, szUser, szPwd, SndTag);
	if(Url[0] == '\0' || SndTag[0] == '\0')
	{
		LOG_INFO("get tag err!");
		return -1;
	}
	HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
    //LOG_INFO("Tmpmerchantkey[%s]",szTmpMerchantkey);
    Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
    //LOG_INFO("merchantkey[%s]",szMerchantKey);
    HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
    HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
	//set headers 
	pClient->SetHeaders("Accept: */*");
	pClient->SetHeaders("User-Agent: NetworkTest/1.0 (iPhone; iOS 9.3.2; Scale/2.00");
	pClient->SetHeaders("Accept-Encoding: gzip, deflate");
	pClient->SetHeaders(szMerchantKey);
	pClient->SetHeaders(szDomain);
	pClient->SetHeaders(szAppIp);
	// set body

	pClient->Post(Url, SndTag, strlen(SndTag));
	// callback function 
	pClient->SettingBuffer(NULL,NULL,OnWriteData_Post,(void *)&strResponse);
	// set culr 
	pClient->SettingCURL();
	// Run 
	pClient->Run();
	int nRoute = 0;

	if(pParam[MSG_TYPE_LEN+MAC_LEN+CRM_USER_LEN+CRM_PWD_LEN+MT4_USER_LEN+MT4_PWD_LEN] == '2')
	{
		nRoute = 2;
	}
	return nRoute;

}

int HttpCliBusiness::RegisterCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse)
{
	//账号注册
	//char Url[128] = {0};
	 
	char SndTag[2048] = { 0 };
	char* nPos = NULL;
	char szTmp[2048] = {0};
	char SndTagTmp[2048] = {0};
	char szPwd[CRM_PWD_LEN+1] = {0}; 
	char szTmpPwd[CRM_PWD_LEN+1] = {0};
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
    char szDomain[50] = { 0 };
    char szAppIp[50] = { 0 };
	GetTagVal(pParam, (char*)TAG_URL, Url);
	GetTagVal(pParam, (char*)TAG_SND, SndTagTmp);


	nPos = strstr(SndTagTmp, TAG_PWD);
	GetTagVal(SndTagTmp, (char*)TAG_PWD, szTmpPwd, ' ');
	Base64_Decode(szPwd, szTmpPwd, strlen(szTmpPwd));
	LOG_INFO("PWD[%s], pwd[%s]", szTmpPwd, szPwd);

	memcpy(szTmp, SndTagTmp, nPos-SndTagTmp);
	sprintf(SndTag, "%s%s%s%s&timestamp=%d", szTmp,TAG_PWD,szPwd,nPos+strlen(TAG_PWD)+strlen(szTmpPwd),time(NULL));
	//TAG_PWD

	LOG_INFO("URL[%s],SndTagTmp[%s]SndTag[%s]", Url, SndTagTmp, SndTag);
	if(Url[0] == '\0' || SndTag[0] == '\0')
	{
		LOG_INFO("get tag err!");
		return -1;
	}
	char szLang[32] = { 0 };
	HttpGetTagVal((char *)pParam, (char *)TAG_LANG, szLang, '&');
	char TmpLang[32] = { 0 };
	sprintf(TmpLang,"lang: %s",szLang);
	LOG_INFO("lang:%s",TmpLang);
	HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
    //LOG_INFO("Tmpmerchantkey[%s]",szTmpMerchantkey);
    Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
    //LOG_INFO("merchantkey[%s]",szMerchantKey);
    HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
    HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
	//set headers 
	pClient->SetHeaders("Accept: */*");
	pClient->SetHeaders("User-Agent: NetworkTest/1.0 (iPhone; iOS 9.3.2; Scale/2.00");
	pClient->SetHeaders("Accept-Encoding: gzip, deflate");
	pClient->SetHeaders(szMerchantKey);
	pClient->SetHeaders(szDomain);
	pClient->SetHeaders(szAppIp);
	pClient->SetHeaders(TmpLang);

	// set body
	pClient->Post(Url, SndTag, strlen(SndTag));
	// callback function 
	pClient->SettingBuffer(NULL,NULL,OnWriteData_Post,(void *)&strResponse);
	// set culr 
	pClient->SettingCURL();
	// Run 
	pClient->Run();
	return 0;
	 

}

int HttpCliBusiness::ForPwdCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse)
{
	//char Url[128] = {0};
	char szLang[32] = {0};
	char SndTag[2048] = { 0 };
	GetTagVal(pParam, (char*)TAG_URL, Url);
	GetTagVal(pParam, (char*)TAG_SND, SndTag);
	LOG_INFO("URL[%s],SndTag[%s]", Url, SndTag);
	//if(Url[0] == '\0' || SndTag[0] == '\0')
	if(Url[0] == '\0' )
	{
		LOG_INFO("get tag err!");
		return -1;
	}
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
    char szDomain[50] = { 0 };
    char szAppIp[50] = { 0 };
    HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
    //LOG_INFO("Tmpmerchantkey[%s]",szTmpMerchantkey);
    Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
    //LOG_INFO("merchantkey[%s]",szMerchantKey);
    HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
    HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
	//set headers 
	pClient->SetHeaders("Accept: */*");
	pClient->SetHeaders("User-Agent: NetworkTest/1.0 (iPhone; iOS 9.3.2; Scale/2.00");
	pClient->SetHeaders("Accept-Encoding: gzip, deflate");
	pClient->SetHeaders(szMerchantKey);
	pClient->SetHeaders(szDomain);
	pClient->SetHeaders(szAppIp);
	// set body
	char current_time[128] = { 0 };
	sprintf(current_time,"&timestamp=%d",time(NULL));
	strcat(SndTag, current_time);
	pClient->Post(Url, SndTag, strlen(SndTag));
	// callback function 
	pClient->SettingBuffer(NULL,NULL,OnWriteData_Post,(void *)&strResponse);
	// set culr 
	pClient->SettingCURL();
	// Run 
	pClient->Run();
	return 0;
				 
}

int HttpCliBusiness::UserGetHisCoutCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse)
{
	//获取历史行情
	//char Url[128] = {0};
	char TmpUrl[128] = { 0 };
	char SndTag[2048] = { 0 };
	GetTagVal(pParam, (char*)TAG_URL, Url);
	//GetTagVal(pParam, (char*)TAG_SND, SndTag);
	char szSymbol[64] = { 0 }; 
	HttpGetTagVal((char *)pParam, (char *)TAG_CURR_FRAME, szSymbol);
	sprintf(TmpUrl,"%s%s",Url,szSymbol);
	LOG_INFO("URL[%s]", TmpUrl);
	if(Url[0] == '\0')
	{
		LOG_INFO("get tag err!");
		return -1 ;
	}
	char szToken[2048] = { 0 };
	char szMt4Acct[56] = { 0 };
	char szLang[32] = { 0 };
	
	HttpGetTagVal((char *)pParam, (char *)TAG_MT4ACC, szMt4Acct, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_CRM_TOKEN, szToken, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_LANG, szLang, '&');
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
    char szDomain[50] = { 0 };
    char szAppIp[50] = { 0 };
    HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
    //LOG_INFO("Tmpmerchantkey[%s]",szTmpMerchantkey);
    Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
    //LOG_INFO("merchantkey[%s]",szMerchantKey);
    HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
    HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
	//set headers 
	pClient->SetHeaders("Accept: */*");
	pClient->SetHeaders("User-Agent: NetworkTest/1.0 (iPhone; iOS 9.3.2; Scale/2.00");
	pClient->SetHeaders("Accept-Encoding: gzip, deflate");
	pClient->SetHeaders(szMerchantKey);
	pClient->SetHeaders(szDomain);
	pClient->SetHeaders(szAppIp);
	pClient->SetHeaders(szLang);
	pClient->SetHeaders(szToken);
	pClient->SetHeaders(szMt4Acct);

	// set body
	pClient->Get(TmpUrl);
	// callback function 
	pClient->SettingBuffer(NULL,NULL,OnWriteData_Post,(void *)&strResponse);
	// set culr 
	pClient->SettingCURL();
	// Run 
	pClient->Run();
	return 0;
}

int HttpCliBusiness::UserGetHisTimeCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse)
{
	//获取历史行情
	//char Url[128] = {0};
	char TmpUrl[256] = { 0 };
	char SndTag[2048] = { 0 };
	GetTagVal(pParam, (char*)TAG_URL, Url);
	GetTagVal(pParam, (char*)TAG_SND, SndTag);
	char szSymbol[128] = { 0 }; 
	HttpGetTagVal((char *)pParam, (char *)TAG_CURR_FRAME, szSymbol);
	//HttpGetTagVal((char *)pParam, (char *)TAG_URL, TmpUrl);
	sprintf(TmpUrl,"%s%s",Url,szSymbol);
	LOG_INFO("URL[%s]", TmpUrl);
	if(Url[0] == '\0')
	{
		LOG_INFO("get tag err!");
		return -1 ;
	}
	char szToken[2048] = { 0 };
	char szMt4Acct[56] = { 0 };
	char szLang[32] = { 0 };
	HttpGetTagVal((char *)pParam, (char *)TAG_MT4ACC, szMt4Acct, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_CRM_TOKEN, szToken, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_LANG, szLang, '&');
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
    char szDomain[50] = { 0 };
    char szAppIp[50] = { 0 };
    HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
    //LOG_INFO("Tmpmerchantkey[%s]",szTmpMerchantkey);
    Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
    //LOG_INFO("merchantkey[%s]",szMerchantKey);
    HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
    HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
	//set headers 
	pClient->SetHeaders("Accept: */*");
	pClient->SetHeaders("User-Agent: NetworkTest/1.0 (iPhone; iOS 9.3.2; Scale/2.00");
	pClient->SetHeaders("Accept-Encoding: gzip, deflate");
	pClient->SetHeaders(szMerchantKey);
	pClient->SetHeaders(szDomain);
	pClient->SetHeaders(szAppIp);
	pClient->SetHeaders(szLang);
	pClient->SetHeaders(szToken);
	pClient->SetHeaders(szMt4Acct);
	// set body
	pClient->Get(TmpUrl);
	// callback function 
	pClient->SettingBuffer(NULL,NULL,OnWriteData_Post,(void *)&strResponse);
	// set culr 
	pClient->SettingCURL();
	// Run 
	pClient->Run();
	return 0;
}

int HttpCliBusiness::UserGetTrdhisCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse)
{
	//获取历史记录
	//char Url[128] = {0};
	char SndTag[2048] = { 0 };
	int nFlag = atoi(CMD_TRDHIS_BO);
 	char szUrl[256] = { 0 };
	GetTagVal(pParam, (char*)TAG_URL, Url);
	GetTagVal(pParam, (char*)TAG_SND, SndTag);
	LOG_INFO("trade history URL[%s],SndTag[%s]", Url, SndTag);
	if(Url[0] == '\0' || SndTag[0] == '\0')
	{
		LOG_INFO("trade history get tag err!");
		return -1;
	}
	char szToken[2048] = { 0 };
	char szMt4Acct[56] = { 0 };
	char szLang[32] = { 0 };
	string lan(TAG_LANGUAGE_HEAD_CRM);
	HttpGetTagVal(pParam, (char*)TAG_URL, szUrl);
	HttpGetTagVal((char *)pParam, (char *)TAG_MT4ACC, szMt4Acct, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_CRM_TOKEN, szToken, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_LANG, szLang, '&');
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
    char szDomain[50] = { 0 };
    char szAppIp[50] = { 0 };
    HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
    //LOG_INFO("Tmpmerchantkey[%s]",szTmpMerchantkey);
    Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
    //LOG_INFO("merchantkey[%s]",szMerchantKey);
    HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
    HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
	//set headers 
	pClient->SetHeaders("Accept: */*");
	pClient->SetHeaders("User-Agent: NetworkTest/1.0 (iPhone; iOS 9.3.2; Scale/2.00");
	pClient->SetHeaders("Accept-Encoding: gzip, deflate");
	pClient->SetHeaders(szMerchantKey);
	pClient->SetHeaders(szDomain);
	pClient->SetHeaders(szAppIp);
	pClient->SetHeaders(szMt4Acct);
	pClient->SetHeaders(szToken);
	pClient->SetHeaders(szLang);

	// set body
	pClient->Get(szUrl);
	// callback function 
	pClient->SettingBuffer(NULL,NULL,OnWriteData_Post,(void *)&strResponse);
	// set culr 
	pClient->SettingCURL();
	// Run 
	pClient->Run();

	return 0;

}

int HttpCliBusiness::UserGetArchtraCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse)
{
	//获取历史记录
	//char Url[128] = {0};
	char SndTag[2048] = { 0 };
	int nFlag = atoi(CMD_TRDHIS_BO);
	char TmpUrl[128] = { 0 };
	GetTagVal(pParam, (char*)TAG_URL, Url);
	HttpGetTagVal((char*)pParam,(char*)TAG_URL,TmpUrl);
	GetTagVal(pParam, (char*)TAG_SND, SndTag);
	LOG_INFO("trade history URL[%s],SndTag[%s]", Url, SndTag);
	if(Url[0] == '\0' || SndTag[0] == '\0')
	{
		LOG_INFO("trade history get tag err!");
		return -1;
	}
	char szToken[2048] = { 0 };
	char szMt4Acct[56] = { 0 };
	char szLang[32] = { 0 };
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
    char szDomain[50] = { 0 };
    char szAppIp[50] = { 0 };
    HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
    //LOG_INFO("Tmpmerchantkey[%s]",szTmpMerchantkey);
    Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
    //LOG_INFO("merchantkey[%s]",szMerchantKey);
    HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
    HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
	string lan(TAG_LANGUAGE_HEAD_CRM);

	HttpGetTagVal((char *)pParam, (char *)TAG_MT4ACC, szMt4Acct, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_CRM_TOKEN, szToken, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_LANG, szLang, '&');

	//set headers 
	pClient->SetHeaders("Accept: */*");
	pClient->SetHeaders("User-Agent: NetworkTest/1.0 (iPhone; iOS 9.3.2; Scale/2.00");
	pClient->SetHeaders("Accept-Encoding: gzip, deflate");
	pClient->SetHeaders(szMerchantKey);
	pClient->SetHeaders(szDomain);
	pClient->SetHeaders(szAppIp);
	pClient->SetHeaders(szMt4Acct);
	pClient->SetHeaders(szToken);
	pClient->SetHeaders(szLang);

	 
	// set body
	pClient->Get(TmpUrl);
	// callback function 
	pClient->SettingBuffer(NULL,NULL,OnWriteData_Post,(void *)&strResponse);
	// set culr 
	pClient->SettingCURL();
	// Run 
	pClient->Run();
	return 0;
}

int HttpCliBusiness::UserGetWithdrawCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse)
{
	// 出金
	//char Url[128] = {0};
	char SndTag[2048] = { 0 };
	GetTagVal(pParam, (char*)TAG_URL, Url);
	GetTagVal(pParam, (char*)TAG_SND, SndTag);
	if (Url[0] == '\0' || SndTag[0] == '\0')
	{
		LOG_ERR("withdraw get url or acct error!");
		return -1;
	}
	LOG_INFO("withdraw URL[%s],SndTag[%s]", Url, SndTag);
	char postTag[2048];
	memset(postTag, 0, sizeof(postTag));
	GetTagVal(pParam, (char*)TAG_POST, postTag);
	//set headers 
	char szToken[2048] = { 0 };
	char szMt4Acct[56] = { 0 };
	char szLang[32] = { 0 };
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
    char szDomain[50] = { 0 };
    char szAppIp[50] = { 0 };
    HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
    //LOG_INFO("Tmpmerchantkey[%s]",szTmpMerchantkey);
    Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
    //LOG_INFO("merchantkey[%s]",szMerchantKey);
    HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
    HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_MT4ACC, szMt4Acct, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_CRM_TOKEN, szToken, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_LANG, szLang, '&');
	pClient->SetHeaders("Accept: */*");
	pClient->SetHeaders("User-Agent: NetworkTest/1.0 (iPhone; iOS 9.3.2; Scale/2.00");
	pClient->SetHeaders("Accept-Encoding: gzip, deflate");
	pClient->SetHeaders(szMerchantKey);
	pClient->SetHeaders(szDomain);
	pClient->SetHeaders(szAppIp);
	pClient->SetHeaders(szMt4Acct);
	pClient->SetHeaders(szToken);
	pClient->SetHeaders(szLang);

	// set body
	if(postTag[0] != '\0')
	{
		pClient->Post(Url, postTag, strlen(postTag));
	}
	else
	{
		pClient->Get(Url);
	}
	
	// callback function 
	pClient->SettingBuffer(NULL,NULL,OnWriteData_Post,(void *)&strResponse);
	// set culr 
	pClient->SettingCURL();
	// Run 
	pClient->Run();

	return 0;
}
int HttpCliBusiness::UserGetWithdrawCrmTmp(char* pParam, char* Url,HttpClient *pClient,string& strResponse)
{
	// 出金
	//char Url[128] = {0};
	char SndTag[2048] = { 0 };
	GetTagVal(pParam, (char*)TAG_URL, Url);
	GetTagVal(pParam, (char*)TAG_SND, SndTag);
	if (Url[0] == '\0' || SndTag[0] == '\0')
	{
		LOG_ERR("withdraw get url or acct error!");
		return -1;
	}
	LOG_INFO("withdraw URL[%s],SndTag[%s]", Url, SndTag);
	char postTag[2048];
	memset(postTag, 0, sizeof(postTag));
	GetTagVal(pParam, (char*)TAG_POST, postTag);
	//set headers 
	char szToken[2048] = { 0 };
	char szMt4Acct[56] = { 0 };
	char szLang[32] = { 0 };
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
    char szDomain[50] = { 0 };
    char szAppIp[50] = { 0 };
    HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
    //LOG_INFO("Tmpmerchantkey[%s]",szTmpMerchantkey);
    Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
    //LOG_INFO("merchantkey[%s]",szMerchantKey);
    HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
    HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_MT4ACC, szMt4Acct, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_CRM_TOKEN, szToken, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_LANG, szLang, '&');
	pClient->SetHeaders("Accept: */*");
	pClient->SetHeaders("User-Agent: NetworkTest/1.0 (iPhone; iOS 9.3.2; Scale/2.00");
	pClient->SetHeaders("Accept-Encoding: gzip, deflate");
	pClient->SetHeaders(szMerchantKey);
	pClient->SetHeaders(szDomain);
	pClient->SetHeaders(szAppIp);
	pClient->SetHeaders(szMt4Acct);
	pClient->SetHeaders(szToken);
	pClient->SetHeaders(szLang);

	// set body
	//if(postTag[0] != '\0')
	//{
		pClient->Post(Url, postTag, strlen(postTag));
	//}
	//else
	//{
	//	pClient->Get(Url);
	//}
	// callback function 
	pClient->SettingBuffer(NULL,NULL,OnWriteData_Post,(void *)&strResponse);
	// set culr 
	pClient->SettingCURL();
	// Run 
	pClient->Run();

	return 0;
	 
}
int HttpCliBusiness::UserWithdrawReq(char* pParam, char* Url,HttpClient *pClient,string& strResponse)
{
	//出金历史记录
	//char Url[128] = {0};
	char SndTag[2048] = { 0 };
	GetTagVal(pParam, (char*)TAG_URL, Url);
	GetTagVal(pParam, (char*)TAG_SND, SndTag);
	LOG_INFO("withdraw history URL[%s],SndTag[%s]", Url, SndTag);
	if (Url[0] == '\0' || SndTag[0] == '\0')
	{
		LOG_INFO("withdraw request get tag err!");
		return -1;
	}
	LOG_INFO("withdraw retuest URL[%s],SndTag[%s]", Url, SndTag);
	char postTag[2048];
	memset(postTag, 0, sizeof(postTag));
	GetTagVal(pParam, (char*)TAG_POST, postTag);
	//set headers 
	char szToken[2048] = { 0 };
	char szMt4Acct[56] = { 0 };
	char szLang[32] = { 0 };
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
    char szDomain[50] = { 0 };
    char szAppIp[50] = { 0 };
    HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
    //LOG_INFO("Tmpmerchantkey[%s]",szTmpMerchantkey);
    Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
    //LOG_INFO("merchantkey[%s]",szMerchantKey);
    HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
    HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_MT4ACC, szMt4Acct, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_CRM_TOKEN, szToken, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_LANG, szLang, '&');
	pClient->SetHeaders("Accept: */*");
	pClient->SetHeaders("User-Agent: NetworkTest/1.0 (iPhone; iOS 9.3.2; Scale/2.00");
	pClient->SetHeaders("Accept-Encoding: gzip, deflate");
	pClient->SetHeaders(szMerchantKey);
	pClient->SetHeaders(szDomain);
	pClient->SetHeaders(szAppIp);
	pClient->SetHeaders(szMt4Acct);
	pClient->SetHeaders(szToken);
	pClient->SetHeaders(szLang);
	 // set body
	if (postTag[0] == '\0')
	{
		pClient->Get(Url);
	}
	else
	{
		pClient->Post(Url, postTag,strlen(postTag));
	}
	// callback function 
	pClient->SettingBuffer(NULL,NULL,OnWriteData_Post,(void *)&strResponse);
	// set culr 
	pClient->SettingCURL();
	// Run 
	pClient->Run();

	return 0;
}

int HttpCliBusiness::UserGetDepostReqCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse)
{
	// 入金
	//char Url[128] = {0};
	 
	char SndTag[2048] = { 0 };
	GetTagVal(pParam, (char*)TAG_URL, Url);
	GetTagVal(pParam, (char*)TAG_SND, SndTag);
	if (Url[0] == '\0' || SndTag[0] == '\0')
	{
		LOG_ERR("deposit get url or acct error!");
		return -1;
	}
	LOG_INFO("deposit URL[%s],SndTag[%s]", Url, SndTag);
	char postTag[2048];
	memset(postTag, 0, sizeof(postTag));
	GetTagVal(pParam, (char*)TAG_POST, postTag);
	//set headers 
	char szToken[2048] = { 0 };
	char szMt4Acct[56] = { 0 };
	char szLang[32] = { 0 };
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
    char szDomain[50] = { 0 };
    char szAppIp[50] = { 0 };
    HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
    //LOG_INFO("Tmpmerchantkey[%s]",szTmpMerchantkey);
    Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
    //LOG_INFO("merchantkey[%s]",szMerchantKey);
    HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
    HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_MT4ACC, szMt4Acct, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_CRM_TOKEN, szToken, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_LANG, szLang, '&');
	pClient->SetHeaders("Accept: */*");
	pClient->SetHeaders("User-Agent: NetworkTest/1.0 (iPhone; iOS 9.3.2; Scale/2.00");
	pClient->SetHeaders("Accept-Encoding: gzip, deflate");
	pClient->SetHeaders(szMerchantKey);
	pClient->SetHeaders(szDomain);
	pClient->SetHeaders(szAppIp);
	pClient->SetHeaders(szMt4Acct);
	pClient->SetHeaders(szToken);
	pClient->SetHeaders(szLang);
	 // set body
	if (postTag[0] == '\0')
	{
		pClient->Get(Url);
	}
	else
	{
		char TmpUrl[128] = { 0 };
		char szGetwayId[56] = { 0 };
		HttpGetTagVal(pParam, (char*)TAG_GETWAYID, szGetwayId,'&');
		sprintf(TmpUrl,"%s/%s",Url,szGetwayId);
		LOG_INFO("deposit URL[%s]", TmpUrl, SndTag);
		pClient->Post(TmpUrl, postTag,strlen(postTag));
	}
	
	// callback function 
	pClient->SettingBuffer(NULL,NULL,OnWriteData_Post,(void *)&strResponse);
	// set culr 
	pClient->SettingCURL();
	// Run 
	pClient->Run();

	return 0;

}

int HttpCliBusiness::UserDepostStatusReqCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse)
{
	// 入金
	//char Url[128] = {0};
 
	char SndTag[2048] = { 0 };
	GetTagVal(pParam, (char*)TAG_URL, Url);
	GetTagVal(pParam, (char*)TAG_SND, SndTag);
	if (Url[0] == '\0' || SndTag[0] == '\0')
	{
		LOG_ERR("deposit status get url or acct error!");
		return -1;
	}
	LOG_INFO("deposit status URL[%s],SndTag[%s]", Url, SndTag);
	//set headers 
	char szToken[2048] = { 0 };
	char szMt4Acct[56] = { 0 };
	char szLang[32] = { 0 };
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
    char szDomain[50] = { 0 };
    char szAppIp[50] = { 0 };
    HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
    //LOG_INFO("Tmpmerchantkey[%s]",szTmpMerchantkey);
    Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
    //LOG_INFO("merchantkey[%s]",szMerchantKey);
    HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
    HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_MT4ACC, szMt4Acct, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_CRM_TOKEN, szToken, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_LANG, szLang, '&');
	pClient->SetHeaders("Accept: */*");
	pClient->SetHeaders("User-Agent: NetworkTest/1.0 (iPhone; iOS 9.3.2; Scale/2.00");
	pClient->SetHeaders("Accept-Encoding: gzip, deflate");
	pClient->SetHeaders(szMerchantKey);
	pClient->SetHeaders(szDomain);
	pClient->SetHeaders(szAppIp);
	pClient->SetHeaders(szMt4Acct);
	pClient->SetHeaders(szToken);
	pClient->SetHeaders(szLang);
	 // set body 
	pClient->Get(Url);
	// callback function 
	pClient->SettingBuffer(NULL,NULL,OnWriteData_Post,(void *)&strResponse);
	// set culr 
	pClient->SettingCURL();
	// Run 
	pClient->Run();

	return 0;

	//getReq(Url, SndTag, strResponse, (char*)CMD_DEPOSIT_STATUS_CRM);  //sucess
}

int HttpCliBusiness::UserDepostUploadReqCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse)
{
	// 入金上传文件
	//char Url[128] = {0};
	 
	char SndTag[2048] = { 0 };
	GetTagVal(pParam, (char*)TAG_URL, Url);
	GetTagVal(pParam, (char*)TAG_SND, SndTag);
	if (Url[0] == '\0' || SndTag[0] == '\0')
	{
		LOG_ERR("deposit upload get url or acct error!");
		return -1;
	}
	//set headers 
	char szToken[2048] = { 0 };
	char szMt4Acct[56] = { 0 };
	char szLang[32] = { 0 };
	char szContentType[120] = { 0 };
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
    char szDomain[50] = { 0 };
    char szAppIp[50] = { 0 };
    HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
    //LOG_INFO("Tmpmerchantkey[%s]",szTmpMerchantkey);
    Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
    //LOG_INFO("merchantkey[%s]",szMerchantKey);
    HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
    HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_MT4ACC, szMt4Acct, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_CRM_TOKEN, szToken, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_LANG, szLang, '&');
	HttpGetTagVal((char *)SndTag, (char *)"content-type=", szContentType, '&');
	pClient->SetHeaders("Expect:");
	pClient->SetHeaders("Accept: */*");
	pClient->SetHeaders("User-Agent: NetworkTest/1.0 (iPhone; iOS 9.3.2; Scale/2.00");
	pClient->SetHeaders("Accept-Encoding: gzip, deflate");
	pClient->SetHeaders(szMerchantKey);
	pClient->SetHeaders(szDomain);
	pClient->SetHeaders(szAppIp);
	pClient->SetHeaders(szMt4Acct);
	pClient->SetHeaders(szToken);
	pClient->SetHeaders(szLang);
	pClient->SetHeaders(szContentType);
	 // set body 
	CIpcMem mIpcMem;
	pMsgHead pHead = (pMsgHead)pParam;
	g_size = 0;
	LOG_INFO("msgtype1: %s",pHead->szMsgType);
	unsigned long long msgType = 0;
	sscanf(pParam, "%016llu", &msgType);
	int key = msgType % 1000000000;
	LOG_INFO("msgtype2: %d, %d", msgType,key);
	if (key <= 0)
	{
		LOG_ERR("depost upload key error");
		return -1;
	}
	if (mIpcMem.Create(key) != 0)
	{
		 
		LOG_ERR("deposit upload create shm error");
		return -1;
	}
	pIpcMemBuf pMembuf = mIpcMem.GetBuf();
			
	//printf("pMembuf:%x\n",pMembuf->mtext);
	LOG_INFO("pMembuf:size[%ld]",pMembuf->msize);	 
	pClient->Post(Url,pMembuf->msize);
	// callback function 
	pClient->SettingBuffer(OnReadData_Post,pMembuf,OnWriteData_Post,(void *)&strResponse);
	// set culr 
	pClient->SettingCURL();
	// Run 
	pClient->Run();
	// free share memory 
	mIpcMem.Detach();
	mIpcMem.Delete();

	return 0;
}
int HttpCliBusiness::UserRegisterTmpCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse)
{
	//获取用户模板
	//char Url[128] = {0};
	 char szLang[32] = { 0 };
	char SndTag[2048] = { 0 };
	//char szLang[80] = {0};
	//char SndTag[2048] = { 0 };
	int nFlag = atoi(CMD_REGISTER_TPL_CRM);
	sprintf(Url, "%s%s", g_Url_Crm_My, USER_REGISTER_CRM);
	LOG_INFO("URL[%s]", Url);
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
    char szDomain[50] = { 0 };
    char szAppIp[50] = { 0 };
    HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
    //LOG_INFO("Tmpmerchantkey[%s]",szTmpMerchantkey);
    Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
    //LOG_INFO("merchantkey[%s]",szMerchantKey);
    HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
    HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
	//set headers 
	pClient->SetHeaders("Accept: */*");
	pClient->SetHeaders("User-Agent: NetworkTest/1.0 (iPhone; iOS 9.3.2; Scale/2.00");
	pClient->SetHeaders("Accept-Encoding: gzip, deflate");
	pClient->SetHeaders(szMerchantKey);
	pClient->SetHeaders(szDomain);
	pClient->SetHeaders(szAppIp);
	 // set body 
	pClient->Get(Url);
	// callback function 
	pClient->SettingBuffer(NULL,NULL,OnWriteData_Post,(void *)&strResponse);
	// set culr 
	pClient->SettingCURL();
	// Run 
	pClient->Run();

	return 0;
	//getReq(Url, SndTag, strResponse, (char*)CMD_REGISTER_TPL_CRM);
}
int HttpCliBusiness::UserNameCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse)
{
	//用户姓名
	//char Url[128] = {0};
 
	char SndTag[2048] = { 0 };
	char TmpUrl[128] = { 0 };
	GetTagVal(pParam, (char*)TAG_URL, Url);
	GetTagVal(pParam, (char*)TAG_SND, SndTag);
	if (Url[0] == '\0' || SndTag[0] == '\0')
	{
		LOG_ERR("deposit get url or acct error!");
		return -1;
	}
	LOG_INFO("getname URL[%s],SndTag[%s]", Url, SndTag);
	 
	//set headers 
	char szToken[2048] = { 0 };
	char szMt4Acct[56] = { 0 };
	char szLang[32] = { 0 };
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
    char szDomain[50] = { 0 };
    char szAppIp[50] = { 0 };
    HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
    //LOG_INFO("Tmpmerchantkey[%s]",szTmpMerchantkey);
    Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
    //LOG_INFO("merchantkey[%s]",szMerchantKey);
    HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
    HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
	HttpGetTagVal(pParam, (char *)TAG_MT4ACC, szMt4Acct, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_CRM_TOKEN, szToken, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_LANG, szLang, '&');
	sprintf(TmpUrl,"%s/%s",Url,szMt4Acct);
	pClient->SetHeaders("Accept: */*");
	pClient->SetHeaders("User-Agent: NetworkTest/1.0 (iPhone; iOS 9.3.2; Scale/2.00");
	pClient->SetHeaders("Accept-Encoding: gzip, deflate");
	pClient->SetHeaders(szMerchantKey);
	pClient->SetHeaders(szDomain);
	pClient->SetHeaders(szAppIp);
	//pClient->SetHeaders(szMt4Acct);
	pClient->SetHeaders(szToken);
	pClient->SetHeaders(szLang);
	 // set body 
	pClient->Get(TmpUrl);
	// callback function 
	pClient->SettingBuffer(NULL,NULL,OnWriteData_Post,(void *)&strResponse);
	// set culr 
	pClient->SettingCURL();
	// Run 
	pClient->Run();

	return 0;

}
int HttpCliBusiness::UserSetNameCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse)
{
	//char Url[128] = {0};
	 
	char SndTag[2048] = { 0 };
	GetTagVal(pParam, (char*)TAG_URL, Url);
	GetTagVal(pParam, (char*)TAG_SND, SndTag);
	if (Url[0] == '\0' || SndTag[0] == '\0')
	{
		LOG_ERR("set Chinese name acct error!");
		return -1;
	}
	LOG_INFO("seetname URL[%s],SndTag[%s]", Url, SndTag); 
	char postTag[2048];
	memset(postTag, 0, sizeof(postTag));
	GetTagVal(pParam, (char*)TAG_POST, postTag);
	//set headers 
	char szToken[2048] = { 0 };
	char szMt4Acct[56] = { 0 };
	char szLang[32] = { 0 };
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
    char szDomain[50] = { 0 };
    char szAppIp[50] = { 0 };
    HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
    //LOG_INFO("Tmpmerchantkey[%s]",szTmpMerchantkey);
    Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
    //LOG_INFO("merchantkey[%s]",szMerchantKey);
    HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
    HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_MT4ACC, szMt4Acct, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_CRM_TOKEN, szToken, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_LANG, szLang, '&');
	pClient->SetHeaders("Accept: */*");
	pClient->SetHeaders("User-Agent: NetworkTest/1.0 (iPhone; iOS 9.3.2; Scale/2.00");
	pClient->SetHeaders("Accept-Encoding: gzip, deflate");
	pClient->SetHeaders(szMerchantKey);
	pClient->SetHeaders(szDomain);
	pClient->SetHeaders(szAppIp);
	pClient->SetHeaders(szMt4Acct);
	pClient->SetHeaders(szToken);
	pClient->SetHeaders(szLang);
	 // set body 
	pClient->Post(Url, postTag, strlen(postTag));
	// callback function 
	pClient->SettingBuffer(NULL,NULL,OnWriteData_Post,(void *)&strResponse);
	// set culr 
	pClient->SettingCURL();
	// Run 
	pClient->Run();

	return 0;
	//PostReq(Url, buf.mtext, (char*)CMD_DEPOSIT_TPL_CRM, strResponse, true, NULL, NULL, NULL, NULL);//sucess

}
int HttpCliBusiness::UserProfileDataCrm(char* pParam, char* Url,HttpClient *pClient,string& strResponse)
{
	//char Url[128] = {0};
	 
	char SndTag[2048] = { 0 };
	GetTagVal(pParam, (char*)TAG_URL, Url);
	GetTagVal(pParam, (char*)TAG_SND, SndTag);
	if (Url[0] == '\0' || SndTag[0] == '\0')
	{    
		LOG_ERR("user profile data get url or acct error!");
		return -1;
	}    
	LOG_INFO("user profile data URL[%s],SndTag[%s]", Url, SndTag);
	char postTag[2048];
	memset(postTag, 0, sizeof(postTag));
	GetTagVal(pParam, (char*)TAG_POST, postTag);
	//set headers 
	char szToken[2048] = { 0 };
	char szMt4Acct[56] = { 0 };
	char szLang[32] = { 0 };
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
    char szDomain[50] = { 0 };
    char szAppIp[50] = { 0 };
    HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
    //LOG_INFO("Tmpmerchantkey[%s]",szTmpMerchantkey);
    Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
    //LOG_INFO("merchantkey[%s]",szMerchantKey);
    HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
    HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_MT4ACC, szMt4Acct, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_CRM_TOKEN, szToken, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_LANG, szLang, '&');
	pClient->SetHeaders("Accept: */*");
	pClient->SetHeaders("User-Agent: NetworkTest/1.0 (iPhone; iOS 9.3.2; Scale/2.00");
	pClient->SetHeaders("Accept-Encoding: gzip, deflate");
	pClient->SetHeaders(szMerchantKey);
	pClient->SetHeaders(szDomain);
	pClient->SetHeaders(szAppIp);
	pClient->SetHeaders(szMt4Acct);
	pClient->SetHeaders(szToken);
	pClient->SetHeaders(szLang);
	 // set body
	if (postTag[0] == '\0')
	{
		pClient->Get(Url);
	}
	else
	{
		pClient->Post(Url, postTag,strlen(postTag));
	}
	
	// callback function 
	pClient->SettingBuffer(NULL,NULL,OnWriteData_Post,(void *)&strResponse);
	// set culr 
	pClient->SettingCURL();
	// Run 
	pClient->Run();

	return 0;
	//getReq(Url, SndTag, strResponse, (char*)CMD_PROFILE_DATA);  //sucess
}
