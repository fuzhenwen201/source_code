#include "../../include/HttpCliMultiBusiness.h"

char *gMerchantKey = NULL;
char *gAppIp = NULL;
char *gDoMain = NULL;
char *g_Url_Crm_My = NULL;



int Equal(char a, char b)
{
	if(a >= 'A' && a <= 'Z') a += 'a' - 'A';
	if(b >= 'A' && b <= 'Z') b += 'a' - 'A';
	return a == b;
}

char *FindSubstr(char *s, char *sub)
{
	char *p, *q;
	while(*s)
	{
		for(p = s, q = sub; *q && Equal(*p, *q); p++, q++);
		if(*q == '\0') return s + strlen(sub);
		s++;
	}
	return 0;
}

int FindChar(char *s, char ch)
{
	for(int i = 0; s[i]; i++)
	{
		if(ch == s[i])
			return i;
	}
	return 0;
}

void GetTagVal(char *buf, char *tag, char *dest, char end = '\0')
{
	char *sPos = NULL;
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

void HttpGetTagVal(char *buf, char *tag, char *dest, char end = '\0')
{
	char *sPos = NULL;
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

void GetKeyFrText(char *buf, char tag , char *dest, char end )
{
	char *sPos = NULL;
	char *src = buf;
	int count = 0;
	while(*src != '\0' && count != 3)
	{
		if (*src++ == tag)
			count++;
	}
	src--;
	while(*src != end && *src != '?' && *src != '\0')
	{
		*dest++ = *src++;
	}

}


size_t OnWriteData_Post(void *buffer, size_t size, size_t nmemb, void *lpVoid)
{
#if 1
	//std::string* str = reinterpret_cast<std::string*>(lpVoid);

	if (lpVoid == NULL || NULL == buffer)
	{
		LOG_ERR("lpVoid or buffer is NULL");
		return size*nmemb;
	}
	std::string str ;
	size_t max_len = 16384;
	HttpInfo *response = reinterpret_cast<HttpInfo *>(lpVoid);
	if((max_len) <= (size*nmemb))
	{
		LOG_ERR("ERR:response length[%d] m_data[%s]",response->m_size,response->m_data);
		response->m_size = size*nmemb;
		return size*nmemb;
	}
	char *pData = reinterpret_cast<char *>(buffer);
	size_t len = (BUFSZ) > (response->m_size + size * nmemb) ? (size * nmemb) : 0;
	if(len == 0 )
	{
		LOG_ERR("ERR:response length[%d] m_data[%s]",response->m_size,response->m_data);
		return size*nmemb;
	}
	str.append(pData, size * nmemb);
	LOG_DEBUG("str %s size[%d]", str.c_str(),str.size());
	memcpy(response->m_data + response->m_size, str.c_str(), str.size());
	response->m_size += str.size();
	return nmemb * size;
#endif
#if 0
	char *pData = reinterpret_cast<char *>(buffer);
	HttpInfo *response = reinterpret_cast<HttpInfo *>(lpVoid);
	size_t len = BUFSZ > (response->m_size + size * nmemb) ? (size * nmemb) : 0;
	memcpy(response->m_data + response->m_size, pData, len);
	response->m_size += len;
	LOG_INFO("response data [%s],size[%d]",response->m_data,response->m_size);
	return nmemb * size;
#endif

}
//static int g_size = 0;
size_t OnReadData_Post(void *buffer, size_t size, size_t nmemb, void *lpVoid)
{

	long real_size = nmemb * size;
	char *src = (char *)buffer;
	//pIpcMemBuf pMembuf = (pIpcMemBuf)lpVoid;

	HttpResult *result = (HttpResult*)lpVoid;
	pIpcMemBuf pMembuf = ((CIpcMem*)(result->m_IpcMem))->GetBuf();
	char *dest = (char *)(pMembuf->mtext + result->m_IpcMem_Len);
	long templen = pMembuf->msize - result->m_IpcMem_Len ;
	if(templen <= real_size)
		real_size = templen;
	LOG_DEBUG("pMembuf->msize:[%d],real_size :[%ld]", pMembuf->msize, real_size);
	result->m_IpcMem_Len += real_size;
	memcpy(src, dest, real_size);

	return real_size;
}

size_t OnReadData2_Post(void * buffer,size_t size,size_t nmemb, void *lpVoid)
{
	if(lpVoid == NULL)
		return 0;
	char *src = (char*)lpVoid;
	memcpy(buffer,lpVoid,strlen(src)+1);
	//printf("buffer[%s]", buffer);
	delete (char*)lpVoid;
	return size*nmemb;
}
HttpResult::HttpResult(): m_IpcMem(NULL), m_headerlist(NULL)
{

}
HttpResult::~HttpResult()
{
	if(NULL != m_IpcMem)
	{
		m_IpcMem->Detach();
		m_IpcMem->Delete();
		delete m_IpcMem;
	}
	if(NULL != m_headerlist)
	{
		curl_slist_free_all(m_headerlist);
	}
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

	if((g_Url_Crm_My = getenv("URL_CRM_MY")) == NULL)
		LOG_ERR("get env URL_CRM_MY err!");

	LOG_INFO("merchkey[%s],appip[%s],domain[%s],g_Url_Crm_My[%s]", gMerchantKey, gAppIp, gDoMain, g_Url_Crm_My);

	// init
	m_FunMap.insert(make_pair(USER_LOGIN_CRM, &HttpCliBusiness::LoginCrm));
	m_FunMap.insert(make_pair(USER_AUTH_CRM, &HttpCliBusiness::AuthCrm));
	m_FunMap.insert(make_pair(USER_REGISTER_CRM, &HttpCliBusiness::RegisterCrm));
	m_FunMap.insert(make_pair(USER_FORPWD_CRM, &HttpCliBusiness::ForPwdCrm));
	m_FunMap.insert(make_pair(USER_GETHIS_COUNT_CRM, &HttpCliBusiness::UserGetHisCoutCrm));
	m_FunMap.insert(make_pair(USER_GETHIS_TIME_CRM, &HttpCliBusiness::UserGetHisTimeCrm));
	m_FunMap.insert(make_pair(USER_GETTRDHIS__CRM, &HttpCliBusiness::UserGetTrdhisCrm));
	m_FunMap.insert(make_pair(USER_GETARCHTRA__CRM, &HttpCliBusiness::UserGetArchtraCrm));

	m_FunMap.insert(make_pair(USER_GETWITHDRAW_CRM, &HttpCliBusiness::UserGetWithdrawCrm));
	m_FunMap.insert(make_pair(USER_GETWITHDRAW_CRM_TMP, &HttpCliBusiness::UserGetWithdrawCrmTmp));
	m_FunMap.insert(make_pair(USER_WITHDRAW_REQ_CRM, &HttpCliBusiness::UserWithdrawReq));
	m_FunMap.insert(make_pair(USER_GETDEPOSIT_REQ_CRM, &HttpCliBusiness::UserGetDepostReqCrm));
	m_FunMap.insert(make_pair(USER_DEPOSIT_STATUS_REQ_CRM, &HttpCliBusiness::UserDepostStatusReqCrm));
	m_FunMap.insert(make_pair(USER_DEPOSIT_UPLOAD_REQ_CRM, &HttpCliBusiness::UserDepostUploadReqCrm));
	m_FunMap.insert(make_pair(USER_REGISTER_TMP_CRM, &HttpCliBusiness::UserRegisterTmpCrm));
	m_FunMap.insert(make_pair(USER_NAME_CRM, &HttpCliBusiness::UserNameCrm));
	m_FunMap.insert(make_pair(USER_SETNAME_CRM, &HttpCliBusiness::UserSetNameCrm));
	m_FunMap.insert(make_pair(USER_PROFILE_DATA, &HttpCliBusiness::UserProfileDataCrm));
	m_FunMap.insert(make_pair(USER_LOGOUT_BO, &HttpCliBusiness::LogoutBO));
}

int HttpCliBusiness::RequestHttpCli(char *pParam, char *Url, void *Response, MultHttpClient *mult)
{
	if(NULL == Url || pParam == NULL)
	{
		LOG_ERR("URL is NULL");
		return -1;
	}
	//LOG_DEBUG("pParam :[%s]", pParam);
	int nRet = -1;
	AsncHttpClient httpclient;
	httpclient.Create();

	//httpclient.CreateCUrl();
	char key[50] = { 0 };
	GetKeyFrText(pParam, '/', key, '|');
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, pParam, MSG_TYPE_LEN);
	LOG_INFO("szMsgType[%s] key [%s]",szMsgType, key);
	string strKey = key;


	map<string, pFun>::iterator iter;
	for(iter = m_FunMap.begin(); iter != m_FunMap.end(); ++iter)
	{
		if(strKey.find(iter->first) != strKey.npos){
			LOG_INFO("api[%s]", iter->first.c_str());
			nRet = (this->*(iter->second))(pParam, Url, &httpclient, Response);

			strcat(((HttpInfo *)Response)->m_data, "|");
			strcat(((HttpInfo *)Response)->m_data, Url);
			strcat(((HttpInfo *)Response)->m_data, "|");
			((HttpInfo *)Response)->m_size = strlen(((HttpInfo *)Response)->m_data);
			((HttpInfo *)Response)->m_status_code = nRet;
			//LOG_INFO("CURL:%x", httpclient.GetCURL());
			HttpInfo* info = (HttpInfo*)Response;
			HttpResult *result = (HttpResult*)(info->m_ret);
			result->m_headerlist = httpclient.GetHeaderList();
			mult->AddCurlHandle(httpclient.GetCURL(), (HttpInfo *)Response);
			return nRet;
		}
	}

	return nRet;

}

int HttpCliBusiness::LoginCrm(char *pParam, char *Url, AsncHttpClient *pClient, void *Response)
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, pParam, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	//char Url[128] = {0};
	char szLang[36] = {0};
	char SndTag[2048] = { 0 };
	char szPwd[CRM_PWD_LEN + 1] = {0};
	char szTmpPwd[CRM_PWD_LEN + 1] = {0};
	char SndTagTmp[2048] = {0};
	char szEmail[CRM_USER_LEN + 1] = {0};
	char szMerchantKey[120] = { 0 };
	char szDomain[50] = { 0 };
	char szAppIp[50] = { 0 };
	GetTagVal(pParam, (char *)TAG_URL, Url);
	GetTagVal(pParam, (char *)TAG_SND, SndTagTmp);
	GetTagVal(SndTagTmp, (char *)TAG_EMAIL, szEmail, '&');
	GetTagVal(SndTagTmp, (char *)TAG_PWD, szTmpPwd, '&');
	Base64_Decode(szPwd, szTmpPwd, strlen(szTmpPwd));
	sprintf(SndTag, "email=%s&password=%s", szEmail, szPwd);
	LOG_DEBUG("URL[%s],SndTaTmpg[%s],mail[%s]sndtag[%s]", Url, SndTagTmp, szEmail, SndTag);
	if(Url[0] == '\0' || SndTag[0] == '\0')
	{
		LOG_ERR("szMsgType[%s] get tag err!",szMsgType);
		return -1;
	}
	//set headers
	HttpGetTagVal((char *)pParam, (char *)TAG_LANG, szLang, '&');
	char szTmpMerchantkey[120] = { 0 };
	HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
	LOG_INFO("Tmpmerchantkey[%s]",szTmpMerchantkey);
	Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
	LOG_INFO("merchantkey[%s]",szMerchantKey);
	HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
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
	sprintf(current_time, "&timestamp=%d", time(NULL));
	strcat(SndTag, current_time);
	LOG_DEBUG("sndtag[%s]", SndTag);
	pClient->Post(Url, SndTag, strlen(SndTag));
	// callback function
	pClient->SettingBuffer(NULL, NULL, OnWriteData_Post, (void *)Response);
	// set culr
	pClient->SettingCURL(0);

	return 0;

}

int HttpCliBusiness::AuthCrm(char *pParam, char *Url, AsncHttpClient *pClient, void *Response)
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, pParam, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	//char Url[128] = {0};
	char szLang[32] = {0};
	char SndTag[2048] = { 0 };
	char SndTagTmp[2048] = {0};
	char szUser[MT4_USER_LEN + 1] = {0};
	char szPwd[MT4_PWD_LEN + 1] = {0};
	char szTmpPwd[MT4_PWD_LEN + 1] = {0};
	char szMerchantKey[120] = { 0 };

	char szDomain[50] = { 0 };
	char szAppIp[50] = { 0 };

	GetTagVal(pParam, (char *)TAG_URL, Url);
	LOG_INFO("szMsgType=[%s] URL:[%s] param=[%s]",szMsgType, Url,pParam);
	GetTagVal(pParam, (char *)TAG_SND, SndTagTmp);
	GetTagVal(SndTagTmp, (char *)TAG_USERNAME, szUser, '&');
	GetTagVal(SndTagTmp, (char *)TAG_PWD, szTmpPwd, '&');
	LOG_DEBUG("password[%s]", szTmpPwd);
	Base64_Decode(szPwd, szTmpPwd, strlen(szTmpPwd));
	sprintf(SndTag, "grant_type=password&username=%s&password=%s&timestamp=%d", szUser, szPwd, time(NULL));
	LOG_DEBUG("URL[%s],username[%s],password[%s],SndTag[%s]", Url, szUser, szPwd, SndTag);
	if(Url[0] == '\0' || SndTag[0] == '\0')
	{
		LOG_ERR("szMsgType[%s] get tag err!",szMsgType);
		return -1;
	}
	//set headers
	char szTmpMerchantkey[120] = { 0 };
	HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
	Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
	HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
	pClient->SetHeaders("Accept: */*");
	pClient->SetHeaders("User-Agent: NetworkTest/1.0 (iPhone; iOS 9.3.2; Scale/2.00");
	pClient->SetHeaders("Accept-Encoding: gzip, deflate");
	pClient->SetHeaders(szMerchantKey);
	pClient->SetHeaders(szDomain);
	pClient->SetHeaders(szAppIp);
	// set body

	LOG_DEBUG("URL[%s],SndTag[%s]", Url, SndTag);
	pClient->Post(Url, SndTag, strlen(SndTag));
	// callback function
	pClient->SettingBuffer(NULL, NULL, OnWriteData_Post, (void *)Response);
	// set culr
	pClient->SettingCURL(0);
	// Run
	//pClient->Run();
	int nRoute = 0;

	if(pParam[MSG_TYPE_LEN + MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN] == '2')
	{
		nRoute = 2;
	}
	return nRoute;

}

int HttpCliBusiness::LogoutBO(char *pParam, char *Url, AsncHttpClient *pClient, void *Response)
{
	char szMsgType[MSG_TYPE_LEN + 1] = { 0 };
	memcpy(szMsgType, pParam, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s, param=%s", szMsgType,pParam);
	//char Url[128] = {0};
	char szLang[32] = { 0 };
	char SndTag[2048] = { 0 };
	char SndTagTmp[2048] = { 0 };
	char szUser[MT4_USER_LEN + 1] = { 0 };
	char szPwd[MT4_PWD_LEN + 1] = { 0 };
	char szTmpPwd[MT4_PWD_LEN + 1] = { 0 };
	char szMerchantKey[120] = { 0 };
	char szToken[2048] = { 0 };
	char szDomain[50] = { 0 };
	char szAppIp[50] = { 0 };

	GetTagVal(pParam, (char *)TAG_URL, Url);

	HttpGetTagVal((char *)pParam, (char *)TAG_CRM_TOKEN, szToken);
	LOG_INFO("szMsgType=[%s] URL:[%s], token:%s", szMsgType, Url,szToken);

	if (Url[0] == '\0')
	{
		LOG_ERR("szMsgType[%s] get tag err!", szMsgType);
		return -1;
	}
	//set headers
	char szTmpMerchantkey[120] = { 0 };
	HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
	Base64Dec(szMerchantKey, szTmpMerchantkey, strlen(szTmpMerchantkey));
	HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
	pClient->SetHeaders("Accept: */*");
	pClient->SetHeaders("User-Agent: NetworkTest/1.0 (iPhone; iOS 9.3.2; Scale/2.00");
	pClient->SetHeaders("Accept-Encoding: gzip, deflate");

	pClient->SetHeaders(szToken);

	// set body

	pClient->Post(Url, SndTag, strlen(SndTag));
	// callback function
	pClient->SettingBuffer(NULL, NULL, OnWriteData_Post, (void *)Response);
	// set culr
	pClient->SettingCURL(0);
	// Run
	//pClient->Run();
	int nRoute = 0;

	if (pParam[MSG_TYPE_LEN + MAC_LEN + CRM_USER_LEN + CRM_PWD_LEN + MT4_USER_LEN + MT4_PWD_LEN] == '3')
	{
		nRoute = 3;
	}
	return nRoute;

}


int HttpCliBusiness::RegisterCrm(char *pParam, char *Url, AsncHttpClient *pClient, void *Response)
{
	char szMsgType[MSG_TYPE_LEN + 1] = { 0 };
	memcpy(szMsgType, pParam, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s", szMsgType);
	//账号注册
	//char Url[128] = {0};

	char SndTag[2048] = { 0 };
	char *nPos = NULL;
	char szTmp[2048] = { 0 };
	char SndTagTmp[2048] = { 0 };
	char szPwd[CRM_PWD_LEN + 1] = { 0 };
	char szTmpPwd[CRM_PWD_LEN + 1] = { 0 };
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
	char szDomain[50] = { 0 };
	char szAppIp[50] = { 0 };
	char szCliIp[50] = { 0 };

	GetTagVal(pParam, (char *)TAG_URL, Url);
	GetTagVal(pParam, (char *)TAG_SND, SndTagTmp);


	nPos = strstr(SndTagTmp, TAG_PWD);
	GetTagVal(SndTagTmp, (char *)TAG_PWD, szTmpPwd, ' ');
	GetTagVal(SndTagTmp, (char *)TAG_HIS_IP, szCliIp, ' ');
	Base64_Decode(szPwd, szTmpPwd, strlen(szTmpPwd));
	//LOG_INFO("SndTagTmp[%s], szCliIp[%s]", SndTagTmp, szCliIp);

	memcpy(szTmp, SndTagTmp, nPos - SndTagTmp);
	sprintf(SndTag, "%s%s%s%s&timestamp=%d", szTmp, TAG_PWD, szPwd, nPos + strlen(TAG_PWD) + strlen(szTmpPwd), time(NULL));
	//TAG_PWD

	LOG_DEBUG("szMsgType[%s] URL[%s],SndTagTmp[%s]SndTag[%s]", szMsgType, Url, SndTagTmp, SndTag);
	if (Url[0] == '\0' || SndTag[0] == '\0')
	{
		LOG_ERR("szMsgType[%s] get tag err!", szMsgType);
		return -1;
	}
	char szLang[32] = { 0 };
	HttpGetTagVal((char *)pParam, (char *)TAG_LANG, szLang, '&');
	char TmpLang[32] = { 0 };
	sprintf(TmpLang, "lang: %s", szLang);
	char tmpIp[50] = { 0 };
	sprintf(tmpIp, "%s: %s", (char*)TAG_CLIENT_IP,szCliIp);
	LOG_INFO("tmpIp:%s", tmpIp);
	//set headers
	HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
	Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
	HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
	pClient->SetHeaders("Accept: */*");
	pClient->SetHeaders("User-Agent: NetworkTest/1.0 (iPhone; iOS 9.3.2; Scale/2.00");
	pClient->SetHeaders("Accept-Encoding: gzip, deflate");
	pClient->SetHeaders(tmpIp);
	pClient->SetHeaders(szMerchantKey);
	pClient->SetHeaders(szDomain);
	pClient->SetHeaders(szAppIp);
	pClient->SetHeaders(TmpLang);

	// set body
	pClient->Post(Url, SndTag, strlen(SndTag));
	// callback function
	pClient->SettingBuffer(NULL, NULL, OnWriteData_Post, (void *)Response);
	// set culr
	pClient->SettingCURL();
	// Run
	//pClient->Run();
	return 0;


}

int HttpCliBusiness::ForPwdCrm(char *pParam, char *Url, AsncHttpClient *pClient, void *Response)
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, pParam, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	//char Url[128] = {0};
	char szLang[32] = {0};
	char SndTag[2048] = { 0 };
	//char *SndTag = new char[2048];
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
	char szDomain[50] = { 0 };
	char szAppIp[50] = { 0 };
	GetTagVal(pParam, (char *)TAG_URL, Url);
	GetTagVal(pParam, (char *)TAG_SND, SndTag);
	LOG_DEBUG("szMsgType[%s] URL[%s],SndTag[%s]",szMsgType, Url, SndTag);
	//if(Url[0] == '\0' || SndTag[0] == '\0')
	if(Url[0] == '\0' )
	{
		LOG_INFO("szMsgType[%s] get tag err!",szMsgType);
		return -1;
	}
	//set headers
	HttpGetTagVal((char *)pParam, (char *)TAG_LANG, szLang, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
	Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
	HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
	pClient->SetHeaders("Accept: */*");
	pClient->SetHeaders("User-Agent: NetworkTest/1.0 (iPhone; iOS 9.3.2; Scale/2.00");
	pClient->SetHeaders("Accept-Encoding: gzip, deflate");
	pClient->SetHeaders(szMerchantKey);
	pClient->SetHeaders(szDomain);
	pClient->SetHeaders(szAppIp);
	pClient->SetHeaders(szLang);
	// set body
	char current_time[128] = { 0 };
	sprintf(current_time, "&timestamp=%d", time(NULL));
	strcat(SndTag, current_time);
	pClient->Post(Url, SndTag, strlen(SndTag));
	// callback function
	pClient->SettingBuffer(NULL , NULL, OnWriteData_Post, (void *)Response);
	// set culr
	pClient->SettingCURL();
	// Run
	//pClient->Run();
	return 0;

}

int HttpCliBusiness::UserGetHisCoutCrm(char *pParam, char *Url, AsncHttpClient *pClient, void *Response)
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, pParam, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s,src=%s",szMsgType,pParam);
	//获取历史行情
	//char Url[128] = {0};
	char TmpUrl[128] = { 0 };
	char SndTag[2048] = { 0 };
	GetTagVal(pParam, (char *)TAG_URL, Url);
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
	char szDomain[50] = { 0 };
	char szAppIp[50] = { 0 };
	//GetTagVal(pParam, (char*)TAG_SND, SndTag);
	char szSymbol[64] = { 0 };
	HttpGetTagVal((char *)pParam, (char *)TAG_CURR_FRAME, szSymbol);
	sprintf(TmpUrl, "%s%s", Url, szSymbol);
	LOG_DEBUG("szMsgType[%s] URL[%s]",szMsgType, TmpUrl);
	if(Url[0] == '\0')
	{
		LOG_ERR("szMsgType[%s] get tag err!",szMsgType);
		return -1 ;
	}
	char szToken[2048] = { 0 };
	char szMt4Acct[56] = { 0 };
	char szLang[32] = { 0 };

	HttpGetTagVal((char *)pParam, (char *)TAG_MT4ACC, szMt4Acct, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_CRM_TOKEN, szToken, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_LANG, szLang, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
	Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
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
	pClient->SettingBuffer(NULL, NULL, OnWriteData_Post, (void *)Response);
	// set culr
	pClient->SettingCURL();
	// Run
	//pClient->Run();
	return 0;
}

int HttpCliBusiness::UserGetHisTimeCrm(char *pParam, char *Url, AsncHttpClient *pClient, void *Response)
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, pParam, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	//获取历史行情
	//char Url[128] = {0};
	char TmpUrl[256] = { 0 };
	char SndTag[2048] = { 0 };
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
	char szDomain[50] = { 0 };
	char szAppIp[50] = { 0 };
	GetTagVal(pParam, (char *)TAG_URL, Url);
	GetTagVal(pParam, (char *)TAG_SND, SndTag);
	char szSymbol[128] = { 0 };
	HttpGetTagVal((char *)pParam, (char *)TAG_CURR_FRAME, szSymbol);
	//HttpGetTagVal((char *)pParam, (char *)TAG_URL, TmpUrl);
	sprintf(TmpUrl, "%s%s", Url, szSymbol);
	LOG_DEBUG("szMsgType[%s] URL[%s]",szMsgType, TmpUrl);
	if(Url[0] == '\0')
	{
		LOG_ERR("szMsgType[%s] get tag err!",szMsgType);
		return -1 ;
	}
	char szToken[2048] = { 0 };
	char szMt4Acct[56] = { 0 };
	char szLang[32] = { 0 };
	HttpGetTagVal((char *)pParam, (char *)TAG_MT4ACC, szMt4Acct, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_CRM_TOKEN, szToken, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_LANG, szLang, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
	Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
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
	pClient->SettingBuffer(NULL, NULL, OnWriteData_Post, (void *)Response);
	// set culr
	pClient->SettingCURL();
	// Run
	//pClient->Run();
	return 0;
}

int HttpCliBusiness::UserGetTrdhisCrm(char *pParam, char *Url, AsncHttpClient *pClient, void *Response)
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, pParam, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	//获取历史记录
	//char Url[128] = {0};
	char SndTag[2048] = { 0 };
	int nFlag = atoi(CMD_TRDHIS_BO);
	char szUrl[256] = { 0 };
	GetTagVal(pParam, (char *)TAG_URL, Url);
	GetTagVal(pParam, (char *)TAG_SND, SndTag);
	LOG_INFO("szMsgType[%s] trade history URL[%s],SndTag[%s]",szMsgType, Url, SndTag);
	if(Url[0] == '\0' || SndTag[0] == '\0')
	{
		LOG_INFO("szMsgType[%s] trade history get tag err!",szMsgType);
		return -1;
	}
	char szToken[2048] = { 0 };
	char szMt4Acct[56] = { 0 };
	char szLang[32] = { 0 };
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
	char szDomain[50] = { 0 };
	char szAppIp[50] = { 0 };

	string lan(TAG_LANGUAGE_HEAD_CRM);
	HttpGetTagVal(pParam, (char*)TAG_URL, szUrl);
	HttpGetTagVal((char *)pParam, (char *)TAG_MT4ACC, szMt4Acct, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_CRM_TOKEN, szToken, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_LANG, szLang, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
	Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
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
	pClient->SettingBuffer(NULL, NULL, OnWriteData_Post, (void *)Response);
	// set culr
	pClient->SettingCURL();
	// Run
	//pClient->Run();

	return 0;

}

int HttpCliBusiness::UserGetArchtraCrm(char *pParam, char *Url, AsncHttpClient *pClient, void *Response)
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, pParam, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	//获取历史记录
	//char Url[128] = {0};
	char SndTag[2048] = { 0 };
	int nFlag = atoi(CMD_TRDHIS_BO);
	char TmpUrl[128] = { 0 };
	GetTagVal(pParam, (char *)TAG_URL, Url);
	HttpGetTagVal((char *)pParam, (char *)TAG_URL, TmpUrl);
	GetTagVal(pParam, (char *)TAG_SND, SndTag);
	LOG_DEBUG("szMsgType[%s] trade history URL[%s],SndTag[%s]",szMsgType, Url, SndTag);
	if(Url[0] == '\0' || SndTag[0] == '\0')
	{
		LOG_ERR("szMsgType[%s] trade history get tag err!",szMsgType);
		return -1;
	}
	char szToken[2048] = { 0 };
	char szMt4Acct[56] = { 0 };
	char szLang[32] = { 0 };
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
	char szDomain[50] = { 0 };
	char szAppIp[50] = { 0 };
	string lan(TAG_LANGUAGE_HEAD_CRM);

	HttpGetTagVal((char *)pParam, (char *)TAG_MT4ACC, szMt4Acct, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_CRM_TOKEN, szToken, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_LANG, szLang, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
	Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
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
	pClient->Get(TmpUrl);
	// callback function
	pClient->SettingBuffer(NULL, NULL, OnWriteData_Post, (void *)Response);
	// set culr
	pClient->SettingCURL();
	// Run
	//pClient->Run();
	return 0;
}

int HttpCliBusiness::UserGetWithdrawCrm(char *pParam, char *Url, AsncHttpClient *pClient, void *Response)
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, pParam, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	// 出金
	//char Url[128] = {0};
	char SndTag[2048] = { 0 };
	GetTagVal(pParam, (char *)TAG_URL, Url);
	GetTagVal(pParam, (char *)TAG_SND, SndTag);
	if (Url[0] == '\0' || SndTag[0] == '\0')
	{
		LOG_ERR("szMsgType[%s] withdraw get url or acct error!",szMsgType);
		return -1;
	}
	LOG_DEBUG("szMsgType[%s] withdraw URL[%s],SndTag[%s]",szMsgType, Url, SndTag);
	char postTag[2048];
	memset(postTag, 0, sizeof(postTag));
	GetTagVal(pParam, (char *)TAG_POST, postTag);
	//set headers
	char szToken[2048] = { 0 };
	char szMt4Acct[56] = { 0 };
	char szLang[32] = { 0 };
	char szMerchantKey[50] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
	char szDomain[50] = { 0 };
	char szAppIp[50] = { 0 };
	HttpGetTagVal((char *)SndTag, (char *)TAG_MT4ACC, szMt4Acct, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_CRM_TOKEN, szToken, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_LANG, szLang, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
	Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
	HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
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
	pClient->SettingBuffer(NULL, NULL, OnWriteData_Post, (void *)Response);
	// set culr
	pClient->SettingCURL();
	// Run
	//pClient->Run();

	return 0;
}
int HttpCliBusiness::UserGetWithdrawCrmTmp(char *pParam, char *Url, AsncHttpClient *pClient, void *Response)
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, pParam, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	// 出金
	//char Url[128] = {0};
	char SndTag[2048] = { 0 };
	GetTagVal(pParam, (char *)TAG_URL, Url);
	GetTagVal(pParam, (char *)TAG_SND, SndTag);
	if (Url[0] == '\0' || SndTag[0] == '\0')
	{
		LOG_ERR("szMsgType[%s] withdraw get url or acct error!",szMsgType);
		return -1;
	}
	//LOG_INFO("withdraw URL[%s],SndTag[%s]", Url, SndTag);
	char postTag[2048];
	memset(postTag, 0, sizeof(postTag));
	GetTagVal(pParam, (char *)TAG_POST, postTag);
	//set headers
	char szToken[2048] = { 0 };
	char szMt4Acct[56] = { 0 };
	char szLang[32] = { 0 };
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
	char szDomain[50] = { 0 };
	char szAppIp[50] = { 0 };
	HttpGetTagVal((char *)SndTag, (char *)TAG_MT4ACC, szMt4Acct, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_CRM_TOKEN, szToken, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_LANG, szLang, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
	Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
	HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
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
	pClient->SettingBuffer(NULL, NULL, OnWriteData_Post, (void *)Response);
	// set culr
	pClient->SettingCURL();
	// Run
	//pClient->Run();

	return 0;

}
int HttpCliBusiness::UserWithdrawReq(char *pParam, char *Url, AsncHttpClient *pClient, void *Response)
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, pParam, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	//出金历史记录
	//char Url[128] = {0};
	char SndTag[2048] = { 0 };
	GetTagVal(pParam, (char *)TAG_URL, Url);
	GetTagVal(pParam, (char *)TAG_SND, SndTag);
	//LOG_INFO("withdraw history URL[%s],SndTag[%s]", Url, SndTag);
	if (Url[0] == '\0' || SndTag[0] == '\0')
	{
		LOG_ERR("szMsgType[%s] withdraw request get tag err!",szMsgType);
		return -1;
	}
	LOG_DEBUG("szMsgType[%s] withdraw retuest URL[%s],SndTag[%s]",szMsgType, Url, SndTag);
	char postTag[2048];
	memset(postTag, 0, sizeof(postTag));
	GetTagVal(pParam, (char *)TAG_POST, postTag);
	//set headers
	char szToken[2048] = { 0 };
	char szMt4Acct[56] = { 0 };
	char szLang[32] = { 0 };
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
	char szDomain[50] = { 0 };
	char szAppIp[50] = { 0 };
	HttpGetTagVal((char *)SndTag, (char *)TAG_MT4ACC, szMt4Acct, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_CRM_TOKEN, szToken, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_LANG, szLang, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
	Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
	HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
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
		pClient->Post(Url, postTag, strlen(postTag));
	}
	// callback function
	pClient->SettingBuffer(NULL, NULL, OnWriteData_Post, (void *)Response);
	// set culr
	pClient->SettingCURL();
	// Run
	//pClient->Run();

	return 0;
}

int HttpCliBusiness::UserGetDepostReqCrm(char *pParam, char *Url, AsncHttpClient *pClient, void *Response)
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, pParam, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	// 入金
	//char Url[128] = {0};

	char SndTag[2048] = { 0 };
	GetTagVal(pParam, (char *)TAG_URL, Url);
	GetTagVal(pParam, (char *)TAG_SND, SndTag);
	if (Url[0] == '\0' || SndTag[0] == '\0')
	{
		LOG_ERR("szMsgType[%s] deposit get url or acct error!",szMsgType);
		return -1;
	}
	LOG_DEBUG("szMsgType[%s] deposit URL[%s],SndTag[%s]",szMsgType, Url, SndTag);
	char postTag[2048];
	memset(postTag, 0, sizeof(postTag));
	GetTagVal(pParam, (char *)TAG_POST, postTag);
	//set headers
	char szToken[2048] = { 0 };
	char szMt4Acct[56] = { 0 };
	char szLang[32] = { 0 };
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
	char szDomain[50] = { 0 };
	char szAppIp[50] = { 0 };
	HttpGetTagVal((char *)SndTag, (char *)TAG_MT4ACC, szMt4Acct, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_CRM_TOKEN, szToken, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_LANG, szLang, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
	Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
	HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
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
		HttpGetTagVal(pParam, (char *)TAG_GETWAYID, szGetwayId, '&');
		sprintf(TmpUrl, "%s/%s", Url, szGetwayId);
		LOG_DEBUG("szMsgType[%s] deposit URL[%s]",szMsgType, TmpUrl, SndTag);
		pClient->Post(TmpUrl, postTag, strlen(postTag));
	}

	// callback function
	pClient->SettingBuffer(NULL, NULL, OnWriteData_Post, (void *)Response);
	// set culr
	pClient->SettingCURL();
	// Run
	//pClient->Run();

	return 0;

}

int HttpCliBusiness::UserDepostStatusReqCrm(char *pParam, char *Url, AsncHttpClient *pClient, void *Response)
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, pParam, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	// 入金
	//char Url[128] = {0};

	char SndTag[2048] = { 0 };
	GetTagVal(pParam, (char *)TAG_URL, Url);
	GetTagVal(pParam, (char *)TAG_SND, SndTag);
	if (Url[0] == '\0' || SndTag[0] == '\0')
	{
		LOG_ERR("szMsgType[%s] deposit status get url or acct error!",szMsgType);
		return -1;
	}
	LOG_DEBUG("szMsgType[%s] deposit status URL[%s],SndTag[%s]",szMsgType, Url, SndTag);
	//set headers
	char szToken[2048] = { 0 };
	char szMt4Acct[56] = { 0 };
	char szLang[32] = { 0 };
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
	char szDomain[50] = { 0 };
	char szAppIp[50] = { 0 };
	HttpGetTagVal((char *)SndTag, (char *)TAG_MT4ACC, szMt4Acct, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_CRM_TOKEN, szToken, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_LANG, szLang, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
	Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
	HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
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
	pClient->SettingBuffer(NULL, NULL, OnWriteData_Post, (void *)Response);
	// set culr
	pClient->SettingCURL();
	// Run
	//pClient->Run();

	return 0;

	//getReq(Url, SndTag, strResponse, (char*)CMD_DEPOSIT_STATUS_CRM);  //sucess
}

int HttpCliBusiness::UserDepostUploadReqCrm(char *pParam, char *Url, AsncHttpClient *pClient, void *Response)
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, pParam, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	// 入金上传文件
	//char Url[128] = {0};
	HttpInfo* info = (HttpInfo*)Response;
	HttpResult *result = (HttpResult*)(info->m_ret);
	char SndTag[2048] = { 0 };
	GetTagVal(pParam, (char *)TAG_URL, Url);
	GetTagVal(pParam, (char *)TAG_SND, SndTag);
	if (Url[0] == '\0' || SndTag[0] == '\0')
	{
		LOG_ERR("szMsgType[%s] deposit upload get url or acct error!",szMsgType);
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
	HttpGetTagVal((char *)SndTag, (char *)TAG_MT4ACC, szMt4Acct, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_CRM_TOKEN, szToken, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_LANG, szLang, '&');
	HttpGetTagVal((char *)SndTag, (char *)"content-type=", szContentType, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
	Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
	HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
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
	CIpcMem *mIpcMem = new CIpcMem();
	pMsgHead pHead = (pMsgHead)pParam;
	//g_size = 0;
	//LOG_INFO("msgtype1: %s", pHead->szMsgType);
	unsigned long long msgType = 0;
	sscanf(pParam, "%016llu", &msgType);
	int key = msgType % 1000000000;
	LOG_DEBUG("msgtype2: %d, %d", msgType, key);
	if (key <= 0)
	{
		LOG_ERR("szMsgType[%s] depost upload key error",szMsgType);
		return -1;
	}
	if (mIpcMem->Create(key) != 0)
	{

		LOG_ERR("szMsgType[%s] deposit upload create shm error",szMsgType);
		return -1;
	}
	pIpcMemBuf pMembuf = mIpcMem->GetBuf();

	// printf("pMembuf:%x\n", pMembuf->mtext);
	LOG_DEBUG("szMsgType[%s] pMembuf size[%ld]",szMsgType, pMembuf->msize);
	if(pMembuf->msize > sizeof(pMembuf->mtext))
	{
		LOG_ERR("szMsgType[%s] the size of upload FILE is too big ",szMsgType);
		return -1;
	}
	pClient->Post(Url, pMembuf->mtext, pMembuf->msize);
	// callback function

	pClient->SettingBuffer(OnReadData_Post, result, OnWriteData_Post, (void *)Response);
	// set culr
	pClient->SettingCURL();

	result->m_IpcMem = mIpcMem;
	result->m_IpcMem_Len = 0;
	// Run
	//pClient->Run();
	// free share memory
	//mIpcMem.Detach();
	//mIpcMem.Delete();

	return 0;
}
int HttpCliBusiness::UserRegisterTmpCrm(char *pParam, char *Url, AsncHttpClient *pClient, void *Response)
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, pParam, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	//获取用户模板
	//char Url[128] = {0};
	char szLang[32] = { 0 };
	char SndTag[2048] = { 0 };
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
	char szDomain[50] = { 0 };
	char szAppIp[50] = { 0 };
	//char szLang[80] = {0};
	//char SndTag[2048] = { 0 };
	int nFlag = atoi(CMD_REGISTER_TPL_CRM);
	sprintf(Url, "%s%s", g_Url_Crm_My, USER_REGISTER_CRM);
	LOG_DEBUG("szMsgType[%s] URL[%s]",szMsgType, Url);
	//set headers
	HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
	Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
	HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
	pClient->SetHeaders("Accept: */*");
	pClient->SetHeaders("User-Agent: NetworkTest/1.0 (iPhone; iOS 9.3.2; Scale/2.00");
	pClient->SetHeaders("Accept-Encoding: gzip, deflate");
	pClient->SetHeaders(szMerchantKey);
	pClient->SetHeaders(szDomain);
	pClient->SetHeaders(szAppIp);
	// set body
	pClient->Get(Url);
	// callback function
	pClient->SettingBuffer(NULL, NULL, OnWriteData_Post, (void *)Response);
	// set culr
	pClient->SettingCURL();
	// Run
	//pClient->Run();

	return 0;
	//getReq(Url, SndTag, strResponse, (char*)CMD_REGISTER_TPL_CRM);
}
int HttpCliBusiness::UserNameCrm(char *pParam, char *Url, AsncHttpClient *pClient, void *Response)
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, pParam, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	//用户姓名
	//char Url[128] = {0};

	char SndTag[2048] = { 0 };
	char TmpUrl[128] = { 0 };
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
	char szDomain[50] = { 0 };
	char szAppIp[50] = { 0 };
	GetTagVal(pParam, (char *)TAG_URL, Url);
	GetTagVal(pParam, (char *)TAG_SND, SndTag);
	if (Url[0] == '\0' || SndTag[0] == '\0')
	{
		LOG_ERR("szMsgType[%s] deposit get url or acct error!",szMsgType);
		return -1;
	}
	LOG_DEBUG("szMsgType[%s] getname URL[%s],SndTag[%s]",szMsgType, Url, SndTag);

	//set headers
	char szToken[2048] = { 0 };
	char szMt4Acct[56] = { 0 };
	char szLang[32] = { 0 };
	HttpGetTagVal(pParam, (char *)TAG_MT4ACC, szMt4Acct, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_CRM_TOKEN, szToken, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_LANG, szLang, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
	Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
	HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
	sprintf(TmpUrl, "%s/%s", Url, szMt4Acct);
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
	pClient->SettingBuffer(NULL, NULL, OnWriteData_Post, (void *)Response);
	// set culr
	pClient->SettingCURL();
	// Run
	//pClient->Run();

	return 0;

}
int HttpCliBusiness::UserSetNameCrm(char *pParam, char *Url, AsncHttpClient *pClient, void *Response)
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, pParam, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	//char Url[128] = {0};

	char SndTag[2048] = { 0 };
	GetTagVal(pParam, (char *)TAG_URL, Url);
	GetTagVal(pParam, (char *)TAG_SND, SndTag);
	if (Url[0] == '\0' || SndTag[0] == '\0')
	{
		LOG_ERR("szMsgType[%s] set Chinese name acct error!",szMsgType);
		return -1;
	}
	LOG_DEBUG("szMsgType[%s] seetname URL[%s],SndTag[%s]",szMsgType, Url, SndTag);
	char postTag[2048];
	memset(postTag, 0, sizeof(postTag));
	GetTagVal(pParam, (char *)TAG_POST, postTag);
	//set headers
	char szToken[2048] = { 0 };
	char szMt4Acct[56] = { 0 };
	char szLang[32] = { 0 };
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
	char szDomain[50] = { 0 };
	char szAppIp[50] = { 0 };
	HttpGetTagVal((char *)SndTag, (char *)TAG_MT4ACC, szMt4Acct, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_CRM_TOKEN, szToken, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_LANG, szLang, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
	Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
	HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
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
	pClient->SettingBuffer(NULL, NULL, OnWriteData_Post, (void *)Response);
	// set culr
	pClient->SettingCURL();
	// Run
	//pClient->Run();

	return 0;
	//PostReq(Url, buf.mtext, (char*)CMD_DEPOSIT_TPL_CRM, strResponse, true, NULL, NULL, NULL, NULL);//sucess

}
int HttpCliBusiness::UserProfileDataCrm(char *pParam, char *Url, AsncHttpClient *pClient, void *Response)
{
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	memcpy(szMsgType, pParam, MSG_TYPE_LEN);
	LOG_INFO("szMsgType=%s",szMsgType);
	//char Url[128] = {0};

	char SndTag[2048] = { 0 };
	GetTagVal(pParam, (char *)TAG_URL, Url);
	GetTagVal(pParam, (char *)TAG_SND, SndTag);
	if (Url[0] == '\0' || SndTag[0] == '\0')
	{
		LOG_ERR("szMsgType[%s] user profile data get url or acct error!",szMsgType);
		return -1;
	}
	LOG_DEBUG("szMsgType[%s] user profile data URL[%s],SndTag[%s]",szMsgType, Url, SndTag);
	char postTag[2048];
	memset(postTag, 0, sizeof(postTag));
	GetTagVal(pParam, (char *)TAG_POST, postTag);
	//set headers
	char szToken[2048] = { 0 };
	char szMt4Acct[56] = { 0 };
	char szLang[32] = { 0 };
	char szMerchantKey[120] = { 0 };
	char szTmpMerchantkey[120] = { 0 };
	char szDomain[50] = { 0 };
	char szAppIp[50] = { 0 };
	HttpGetTagVal((char *)SndTag, (char *)TAG_MT4ACC, szMt4Acct, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_CRM_TOKEN, szToken, '&');
	HttpGetTagVal((char *)SndTag, (char *)TAG_LANG, szLang, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_MERCHANTKEY_GET, szTmpMerchantkey, '&');
	Base64Dec(szMerchantKey,szTmpMerchantkey,strlen(szTmpMerchantkey));
	HttpGetTagVal((char *)pParam, (char *)TAG_DOMAIN_GET, szDomain, '&');
	HttpGetTagVal((char *)pParam, (char *)TAG_APPIP_GET, szAppIp, '&');
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
		pClient->Post(Url, postTag, strlen(postTag));
	}

	// callback function
	pClient->SettingBuffer(NULL, NULL, OnWriteData_Post, (void *)Response);
	// set culr
	pClient->SettingCURL();
	// Run
	//pClient->Run();

	return 0;
	//getReq(Url, SndTag, strResponse, (char*)CMD_PROFILE_DATA);  //sucess
}
