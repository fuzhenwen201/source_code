#include "../../include/tyinhttp.h"

CTinyHttp::CTinyHttp()
{
}

CTinyHttp::~CTinyHttp()
{
}

void CTinyHttp::UnimpleMented(string& strUnimpleBuf)
{
	strUnimpleBuf += "HTTP/1.0 501 Method Not Implemented\r\n";
	strUnimpleBuf += SERVER_STRING;
	strUnimpleBuf += "Content-Type: text/html\r\n";
	strUnimpleBuf += "\r\n";
	strUnimpleBuf += "<HTML><HEAD><TITLE>Method Not Implemented\r\n";
	strUnimpleBuf += "</TITLE></HEAD>\r\n";
	strUnimpleBuf += "<BODY><P>HTTP request method not supported.\r\n";
	strUnimpleBuf += "</BODY></HTML>\r\n";
}

int CTinyHttp::GetLine(const char* szReqBuf, char *szDestbuf, int size)  
{  
	int i = 0;  

	while( (*szDestbuf++ = *szReqBuf++) != '\r' && i < (size - 1) ) i++;
	szDestbuf[i] = '\0';
	return(i);  
}  

void CTinyHttp::NotFound(string& strBuf)
{
	CONSTR(strBuf,"HTTP/1.0 404 NOT FOUND\r\n");
	CONSTR(strBuf,SERVER_STRING);  
	CONSTR(strBuf,"Content-Type: text/html\r\n");  
	CONSTR(strBuf,"\r\n");  
	CONSTR(strBuf,"<HTML><TITLE>Not Found</TITLE>\r\n");
	CONSTR(strBuf,"<BODY><P>The server could not fulfill\r\n");  
	CONSTR(strBuf,"your request because the resource specified\r\n");  
	CONSTR(strBuf,"is unavailable or nonexistent.\r\n");  
	CONSTR(strBuf,"</BODY></HTML>\r\n");  
}


void CTinyHttp::MakeReqHead(int flag)
{

}

int CTinyHttp::Equal(char a,char b)
{
	if(a>='A'&&a<='Z') a+='a'-'A';
	if(b>='A'&&b<='Z') b+='a'-'A';
	return a==b;
}

char* CTinyHttp::FindSubstr(char* s,char* sub)
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

int CTinyHttp::Str2Http(char* buf, char* dest, int flag)
{
	int nCnt = 0;

	while(*buf != '|' && *buf != '\0')
	{
		*dest = *buf;
		buf++;
		dest++;
		nCnt++;
	}

	if(nCnt && flag == 1)
	{
		*dest++ = '\r';
		*dest = '\n';
		nCnt += 2;
	}

	if(nCnt && flag == 2)
	{
		*dest++ = '&';
		nCnt++;
	}

	return nCnt;
}

int CTinyHttp::PackHttp(pHttpMap buf, char* dest, int method)
{

	int len = 0;
	int offset = 0;
	if(!buf) return FAILE;

	memcpy(dest, buf, MSG_TYPE_LEN + REQ_TYPE_LEN);

	offset += MSG_TYPE_LEN;
	offset += REQ_TYPE_LEN;

	LOG_INFO("dest[%s]!,offset[%d]", dest, offset);
	if(method == FLAG_GET)
	{
		memcpy(dest+offset, METHOD_GET, strlen(METHOD_GET));
		offset += strlen(METHOD_GET);
	}
	else if( method == FLAG_POST)
	{
		memcpy(dest+offset, METHOD_POST, sizeof(METHOD_POST));
		offset += strlen(METHOD_POST);
	}
	else
	{
		LOG_INFO("method[%d] err!", method);
		return FAILE;
	}
	LOG_INFO("dest[%s]offset[%d]!", dest,offset);
#if 1 
	offset += Str2Http(buf->szReqHost, dest+offset,1);
	offset += Str2Http(buf->szReqUserAgent, dest+offset,1);
	offset += Str2Http(buf->szReqConnection, dest+offset,1);
	offset += Str2Http(buf->szReqAcceptCharset, dest+offset,1);
	offset += Str2Http(buf->szReqAcceptEncoding, dest+offset,1);
	offset += Str2Http(buf->szReqAcceptLanguage, dest+offset,1);
	offset += Str2Http(buf->szReqAccept, dest+offset,1);

	offset += Str2Http(buf->szReqCountryId, dest+offset,2);
	offset += Str2Http(buf->szReqEmail, dest+offset,2);
	offset += Str2Http(buf->szReqUserName, dest+offset,2);
	offset += Str2Http(buf->szReqLeverage, dest+offset,2);
	offset += Str2Http(buf->szReqPassportId, dest+offset,2);
	offset += Str2Http(buf->szReqPassword, dest+offset,2);
	offset += Str2Http(buf->szReqPhone, dest+offset,2);
	offset += Str2Http(buf->szReqIpAddress, dest+offset,2);
	offset += Str2Http(buf->MerchantKeyAND, dest+offset,2);
#endif
	*(dest+offset-1) = '\r';
	*(dest+offset) = '\n';

}

void CTinyHttp::UnpackHttp(char* buf)
{
	int   len = 0;
	char* pos = NULL;
	char* pEpos = NULL;

	LOG_INFO("start unpack!");
	if (GetType(buf) == 1)
	{
		//用户注册
		memset(&m_HttpMap, '|', sizeof(m_HttpMap));
		memcpy(m_HttpMap.szMsgId, buf, MSG_TYPE_LEN);
		memcpy(m_HttpMap.szType, "001", REQ_TYPE_LEN);
		GetTagVal(buf, (char*)"country_id=", m_HttpMap.szReqCountryId);
		GetTagVal(buf, (char*)"email=", m_HttpMap.szReqEmail);
		GetTagVal(buf, (char*)"first_name=", m_HttpMap.szReqUserName);
		GetTagVal(buf, (char*)"leverage=", m_HttpMap.szReqLeverage);
		GetTagVal(buf, (char*)"passport_id=", m_HttpMap.szReqPassportId);
		GetTagVal(buf, (char*)"password=", m_HttpMap.szReqPassword);
		GetTagVal(buf, (char*)"phone=", m_HttpMap.szReqPhone);
		LOG_INFO("countid=[%s],email[%s],firstname[%s],leverage[%s], passwordid=[%s]paddword[%s]phone[%s]", 
				m_HttpMap.szReqCountryId, m_HttpMap.szReqEmail, m_HttpMap.szReqUserName,m_HttpMap.
				szReqLeverage, m_HttpMap.szReqPassportId, m_HttpMap.szReqPassword,m_HttpMap.szReqPhone);
	}
	LOG_INFO("end unpack!");
}

void CTinyHttp::GetTagVal(char* buf, char* tag, char* dest)
{
	char* sPos = NULL;
	if(sPos = FindSubstr(buf, tag))
	{
		while(*sPos && *sPos != '\r' && *sPos != '\n' && *sPos != '&')
		{
			*dest = *sPos;
			dest++;
			sPos++;

		}
	}
}

void CTinyHttp::GetTagVal(char* buf, char* tag, char* dest,char end,size_t count)
{
	char* sPos = NULL;
	if(sPos = FindSubstr(buf, tag))
	{
		while(*sPos && *sPos != '\r' && *sPos != '\n' && *sPos != '&' && *sPos != '\0' && *sPos != end && count > 0)
		{
			*dest = *sPos;
			dest++;
			sPos++;
			count--;
		}
	}
}

int  CTinyHttp::GetType(char* buf)
{
	if(strstr(buf,USER_REGISTER_CRM))
		m_Type = 1;
	else
		m_Type = 9999;

	LOG_INFO("类型[%d]",m_Type);

	return m_Type;
}

int CTinyHttp::CvtApp2Out(char* buf, char* dest, int flag)
{
	//判断接口类型
	if(strstr(buf,USER_LOGIN_APP))
	{
		//用户注册
	}
}

void CTinyHttp::GetTagVal(char* buf, char* tag, char* dest, char end)
{
	char* sPos = NULL;
	if(sPos = FindSubstr(buf, tag))
	{
		while(*sPos && *sPos != '\r' && *sPos != '\n' && *sPos != '&' && *sPos != '\0' && *sPos != end)
		{
			*dest = *sPos;
			dest++;
			sPos++;

		}
	}

}

int CTinyHttp::Con2Str(char* buf, char * split, char* constr)
{
	if(buf == NULL || constr == NULL)
		return -FAILE;
	if(split == NULL)
		strcat(buf, constr);
	else
	{
		strcat(buf,split);
		strcat(buf,constr);
	}

	return SUCESS;
}

void CTinyHttp::MakeService2AppRspHead(char* szHeadRsp, 
		int nFla,int nLen, char* szHttpCode, char* szReason)
{
	sprintf(szHeadRsp, "%s%s %s\r\n", TAG_HTTP11, szHttpCode, szReason);
	strcat(szHeadRsp, "Cache-Control: no-cache\r\n");
	strcat(szHeadRsp, "Pragma: no-cache\r\n");
	sprintf(szHeadRsp+strlen(szHeadRsp), "%s%d%s", TAG_CONT_LEN, nLen, "\r\n");
	//strcat(szHeadRsp, TAG_CONT_TYPE);
	strcat(szHeadRsp, "Expires: -1\r\n");
	strcat(szHeadRsp, "Server: Microsoft-HTTPAPI/2.0\r\n");
	strcat(szHeadRsp, "\r\n");
}


//用户不在线
void CTinyHttp::MakeService2AppRspErr(char* dest, string strCode, string strReason, string strAppVer)
{
	Json::FastWriter fast;
	Json::Value jsonRsp;
	jsonRsp[TAG_RESULT]= Json::Value(strCode); 
	jsonRsp[TAG_DESCRIPTION]= Json::Value(strReason); 
	if(strAppVer != "")
		jsonRsp[TAG_APPVER]= Json::Value(strAppVer);

	MakeService2AppRspHead(dest+strlen(dest), 1, fast.write(jsonRsp).size(), (char*)"200", (char*)"OK");
	strcat(dest, fast.write(jsonRsp).c_str());
}

void CTinyHttp::MakeService2AppRspErr(char* dest, string strCode)
{
	Json::FastWriter fast;
	Json::Value jsonRsp;
	jsonRsp[TAG_RESULT] = Json::Value(strCode);

	MakeService2AppRspHead(dest + strlen(dest), 1, fast.write(jsonRsp).size(), (char*)"200", (char*)"OK");
	strcat(dest, fast.write(jsonRsp).c_str());
}


void CTinyHttp::MakeCrm2AppRspHead(char* src, char* szHeadRsp, int nFlag, int nLen)
{
	char* sPos = NULL;
	char* ePos = NULL;
	if((sPos = strstr(src, TAG_HTTP)) && (ePos = strstr(src, "\r\n")))
	{    
		memcpy(szHeadRsp, sPos, ePos - sPos + strlen("\r\n"));
		memcpy(szHeadRsp, sPos, ePos - sPos + strlen("\r\n"));
		strcat(szHeadRsp, "Cache-Control: no-cache\r\n");
		strcat(szHeadRsp, "Pragma: no-cache\r\n");
		sprintf(szHeadRsp+strlen(szHeadRsp), "%s%d%s", TAG_CONT_LEN, nLen, "\r\n");
		strcat(szHeadRsp, TAG_CONT_TYPE);
		strcat(szHeadRsp, "Expires: -1\r\n");
		strcat(szHeadRsp, "Server: Microsoft-HTTPAPI/2.0\r\n");
		strcat(szHeadRsp, "Date: Wed, 17 Aug 2016 12:38:32 GMT\r\n");
		strcat(szHeadRsp, "\r\n");
	}    

	LOG_INFO("head[%s]", szHeadRsp);

}

void CTinyHttp::MakeService2AppRspBody(char* szBodyRsp, int nFlag)
{
	if(nFlag == 1)
	{
		LOG_INFO("在线心跳报文体");
		sprintf(szBodyRsp,"%s", "{\"result\": 0,\"description\": \"sucess\"}");
	}
	else if(nFlag == 2)
	{
		LOG_INFO("获取风险条款和风险记录");
		char* szSerProEn = NULL;
		char* szSerProCn = NULL;
		szSerProEn = getenv("URL_SER_PRO_EN");
		szSerProCn = getenv("URL_SER_PRO_CN");
		LOG_INFO("serproen[%s], serprocn[%s]", szSerProEn, szSerProCn);
		sprintf(szBodyRsp,"{\"result\": 0,\"%s\": \"%s\", \"%s\": \"%s\"}",
				TAG_SER_PRO_EN, szSerProEn, TAG_SER_PRO_CN, szSerProCn);
	}
	else if(nFlag == 3)
	{
		sprintf(szBodyRsp,"%s", "{\"result\": 0}");
	}
	else if(nFlag == 4)
	{
		LOG_INFO("App 验证接口");
		sprintf(szBodyRsp,"{\"result\":0,\"service\":\"10.10.10.1\",\"port\":\"2016\",\"list\":[{\"type\":\"1\",\"description\":\"has new version, please update\"},{\"type\":\"2\",\"description\":\"http://aa.com/android.apk\"}]}");
	}

}

int CTinyHttp::MakeMT4AuthReq(string mt4acct, string mt4pwd, char* dest,  string bourl)
{
	if(mt4acct == "" || mt4pwd == "")
		LOG_INFO("err mt4acct or mt4pwd");
	strcat(dest, "URL:");
#if 0
	flag?
		strcat(dest, CrmApiConfig.strUrlBoMy.c_str()):
		strcat(dest, CrmApiConfig.strUrlBoHy.c_str());
#endif
		//strcat(dest, g_Url_Bo_My):
		//strcat(dest, g_Url_Bo_Hy);
	strcat(dest, bourl.c_str());
	strcat(dest, USER_AUTH_CRM);
	strcat(dest, "|");
	memcpy(dest+strlen(dest),"SNDTAG:", strlen("SNDTAG:"));

	sprintf(dest+strlen(dest), "%s%s&%s%s&grant_type=password",
			TAG_USERNAME, mt4acct.c_str(), TAG_PWD, mt4pwd.c_str());

	//LOG_INFO("buf[%s]", dest);
}


int CTinyHttp::GetJsonOneTagVal(string strSrc, string strTag, string& strVal)
{
	Json::Value value;
	Json::Reader reader;
	if(reader.parse(strSrc,value))
	{
		M_XRETURN((value[strTag].isNull()), false, LOG_ERR("get [%s] err",strTag.c_str()));

		strVal = value[strTag].asString();

		//LOG_INFO("Tag[%s], value[%s]", \
				strTag.c_str(), strVal.c_str());
	}
}

int CTinyHttp::GetMT4AuthRspVal(char* src, string& strToken, string& strTokenType)
{
	Json::Value value;
	Json::Reader reader;
	if(reader.parse(src,value))
	{
		M_XRETURN((value["token_type"].isNull()), false, LOG_ERR("get token_type err"));
		M_XRETURN((value["access_token"].isNull()), false, LOG_ERR("get token err"));

		strTokenType = value["token_type"].asString();
		strToken = value["access_token"].asString();

		LOG_INFO("token[%s], token_type[%s]", 
				strToken.c_str(), strTokenType.c_str());
	}
}

