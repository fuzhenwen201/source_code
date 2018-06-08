#include "../../include/log.h"
#include "../../include/common.h"
#include "../../include/const.h"

long get_tm_usec()
{
	struct timeval tv;  
	gettimeofday(&tv, NULL);
	return tv.tv_sec*1000000+tv.tv_usec;
}

long get_tm_msec()
{
	struct timeval tv;  
	gettimeofday(&tv, NULL);
	return tv.tv_sec*1000+tv.tv_usec/1000;
}

long get_tm_sec()
{
	struct timeval tv;  
	gettimeofday(&tv, NULL);
	return tv.tv_sec;
}

string int2str(int n)
{
	ostringstream stream;
	stream<<n;
	return stream.str();
}

time_t str_to_time_t(const string& ATime, const string& AFormat="%d-%d-%d")  
{  
	struct tm tm_Temp;  
	time_t time_Ret;  
	try 
	{
		int i = sscanf(ATime.c_str(), AFormat.c_str(),// "%d/%d/%d %d:%d:%d" ,       
				&(tm_Temp.tm_year),   
				&(tm_Temp.tm_mon),   
				&(tm_Temp.tm_mday),  
				&(tm_Temp.tm_hour),  
				&(tm_Temp.tm_min),  
				&(tm_Temp.tm_sec),  
				&(tm_Temp.tm_wday),  
				&(tm_Temp.tm_yday));  

		tm_Temp.tm_year -= 1900;  
		tm_Temp.tm_mon --;  
		tm_Temp.tm_hour=0;  
		tm_Temp.tm_min=0;  
		tm_Temp.tm_sec=0;  
		tm_Temp.tm_isdst = 0;
		time_Ret = mktime(&tm_Temp);  
		return time_Ret;  
	} catch(...) {
		return 0;
	}
} 

time_t NowTime()
{
	time_t t_Now = time(0);
	struct tm* tm_Now = localtime(&t_Now);
	tm_Now->tm_hour =0;
	tm_Now->tm_min = 0;
	tm_Now->tm_sec = 0;
	return  mktime(tm_Now);  
}

string ltos(long l)   
{  
	ostringstream os;  
	os<<l;  
	string result;  
	istringstream is(os.str());  
	is>>result;  
	return result;  

}  

void rand_str(string& str,const int len) 
{
	for(int i=0;i<len;++i)
		str += 'A'+rand()%26;
}


void CreateToken(char* token, char* User)
{
	long pid = getpid();
	long tm = get_tm_usec();
	string rStr;
	rand_str(rStr, 30);
	sprintf(token, "%02d%smemdb%ld%s%ld", strlen(User),User,pid, rStr.c_str(), tm);
}

int RTrim(char* str)
{
	int i;
	for(i=strlen(str)-1; i>=0; i--)
		if (str[i] != ' ')
			break;

	str[i+1] = '\0';
	return 0;
}

int LRTrim(char* str)
{
	LTrim(str);
	RTrim(str);
}

int LTrim(char* str)
{
	char *tmp = str, *tail = str+strlen(str);

	while((*tmp==0x20) && tmp<tail) tmp++;
	if(tmp != str)
	{
		int len = strlen(tmp);
		memmove(str,tmp,len);
		str[len] = '\0';
		//strcpy(str, tmp);
	}
	return 0;
}

char * eatSpace(char *str)
{
	char *pRet = str;
	if (str == NULL)
		return pRet;
	int len = strlen(str);
	while (len--)
	{
		if (str[len] == ' ')
		{
			str[len] = '\0';
		}
		else
		{
			break;
		}
	}
	len = strlen(str);
	for (int i = 0; i < len; i++)
	{
		if (str[i] == ' ')
		{
			pRet++;
		}
		else
		{
			return pRet;
		}
	}
	return pRet;
}

string& strLtrim(string &s)   
{  
	if (s.empty())   
	{  
		return s;  
	}  
	s.erase(0,s.find_first_not_of(" "));  
	return s;  
} 

string& strRtrim(string &s)   
{  
	if (s.empty())   
	{  
		return s;  
	}  
	s.erase(s.find_last_not_of(" ") + 1);  
	return s;  
}

void strrpl(char* pDstOut, char* pSrcIn, const char* pSrcRpl, const char* pDstRpl)
{ 
	char* pi = pSrcIn; 
	char* po = pDstOut; 

	int nSrcRplLen = strlen(pSrcRpl); 
	int nDstRplLen = strlen(pDstRpl); 

	char *p = NULL; 
	int nLen = 0; 

	do 
	{
		// 找到下一个替换点
		p = strstr(pi, pSrcRpl); 

		if(p != NULL) 
		{ 
			// 拷贝上一个替换点和下一个替换点中间的字符串
			nLen = p - pi; 
			memcpy(po, pi, nLen);

			// 拷贝需要替换的字符串
			memcpy(po + nLen, pDstRpl, nDstRplLen); 
		} 
		else 
		{ 
			strcpy(po, pi); 

			// 如果没有需要拷贝的字符串,说明循环应该结束
			break;
		} 

		pi = p + nSrcRplLen; 
		po = po + nLen + nDstRplLen; 

	} while (p != NULL); 
}





BYTE Decode_GetByte(char c)
{
	if(c == '+')
		return 62;
	else if(c == '/')
		return 63;
	else if(c <= '9')
		return (BYTE)(c - '0' + 52);
	else if(c == '=')
		return 64;
	else if(c <= 'Z')
		return (BYTE)(c - 'A');
	else if(c <= 'z')
		return (BYTE)(c - 'a' + 26);
	return 64;
}

int covstr(const char* src, char* dest, int len)
{
	for(int m=0; m<len/2*2; m++)
	{   
		m%2 != 1 ? 
			*(dest+m+1) = *(src+m):
			*(dest+m-1) = *(src+m);
	}   
	if(len%2==1)
	{   
		*(dest+len-1) = *(src+len-1);
	}   
	//printf("dest[%s]", dest);
}


//===================================
//    Base64 解码
//===================================
int Base64_Decode(char *pDest, const char *pSrc, int srclen)
{

	BYTE input[4];
	int i, index = 0;
	char szTmp[srclen+1];

	memset(szTmp, 0x00, sizeof(szTmp));
	covstr(pSrc, szTmp, srclen);

	for(i = 0; i < srclen; i += 4)
	{   
		input[0] = Decode_GetByte(szTmp[i]);
		input[1] = Decode_GetByte(szTmp[i + 1]);
		pDest[index++] = (input[0] << 2) + (input[1] >> 4); 

		if(szTmp[i + 2] != '=')
		{   
			input[2] = Decode_GetByte(szTmp[i + 2]);
			pDest[index++] = ((input[1] & 0x0f) << 4) + (input[2] >> 2); 
		}   

		if(szTmp[i + 3] != '=')
		{   
			input[3] = Decode_GetByte(szTmp[i + 3]);
			pDest[index++] = ((input[2] & 0x03) << 6) + (input[3]);
		}    
	}   

	pDest[index] = 0;
	return index;

}

char GetBase64Value(char ch)
{
	if ((ch >= 'A') && (ch <= 'Z'))
		return ch - 'A';
	if ((ch >= 'a') && (ch <= 'z'))
		return ch - 'a' + 26;
	if ((ch >= '0') && (ch <= '9'))
		return ch - '0' + 52;
	switch (ch)
	{
		case '+':
			return 62;
		case '/':
			return 63;
		case '=': /* base64 padding */
			return 0;
		default:
			return 0;
	}
}
int Base64Dec(char *buf,char*text,int size)
{
	if(size%4)
		return -1;
	unsigned char chunk[4];
	int parsenum=0;

	while(size>0)
	{
		chunk[0] = GetBase64Value(text[0]);
		chunk[1] = GetBase64Value(text[1]);
		chunk[2] = GetBase64Value(text[2]);
		chunk[3] = GetBase64Value(text[3]);

		*buf++ = (chunk[0] << 2) | (chunk[1] >> 4);
		*buf++ = (chunk[1] << 4) | (chunk[2] >> 2);
		*buf++ = (chunk[2] << 6) | (chunk[3]);

		text+=4;
		size-=4;
		parsenum+=3;
	}

	return parsenum;
} 

char Encode_GetChar(BYTE num)
{
	return 
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789"
		"+/="[num];
}

//===================================
//    Base64 编码
//===================================
int Base64_Encode(char *pDest, const char *pSrc, int srclen)
{
	BYTE input[3], output[4];
	int i, index_src = 0, index_dest = 0;
	for(i = 0; i < srclen; i += 3)
	{
		//char [0]
		input[0] = pSrc[index_src++];
		output[0] = (BYTE)(input[0] >> 2);
		pDest[index_dest++] = Encode_GetChar(output[0]);

		//char [1]
		if(index_src < srclen)
		{
			input[1] = pSrc[index_src++];
			output[1] = (BYTE)(((input[0] & 0x03) << 4) + (input[1] >> 4));
			pDest[index_dest++] = Encode_GetChar(output[1]);
		}
		else
		{
			output[1] = (BYTE)((input[0] & 0x03) << 4);
			pDest[index_dest++] = Encode_GetChar(output[1]);
			pDest[index_dest++] = '=';
			pDest[index_dest++] = '=';
			break;
		}

		//char [2]
		if(index_src < srclen)
		{
			input[2] = pSrc[index_src++];
			output[2] = (BYTE)(((input[1] & 0x0f) << 2) + (input[2] >> 6));
			pDest[index_dest++] = Encode_GetChar(output[2]);
		}
		else
		{
			output[2] = (BYTE)((input[1] & 0x0f) << 2);
			pDest[index_dest++] = Encode_GetChar(output[2]);
			pDest[index_dest++] = '=';
			break;
		}

		//char [3]
		output[3] = (BYTE)(input[2] & 0x3f);
		pDest[index_dest++] = Encode_GetChar(output[3]);
	}
	//null-terminator
	pDest[index_dest] = 0;
	return index_dest;
}


int CvtBigToSml(char* str, int len)
{
	for(int n=0; n<len; n++)
	{   
		if('A' <= str[n] && str[n] <= 'Z')
		{   
			str[n] = str[n] + 'a' - 'A';
		}   
	}  
}


time_t convert_str_to_tm(char * str_time)  
{
	struct tm tt;
	memset(&tt,0,sizeof(tt));
	tt.tm_year=atoi(str_time)-1900;
	tt.tm_mon=atoi(str_time+5)-1;
	tt.tm_mday=atoi(str_time+8);
	tt.tm_hour=atoi(str_time+11);
	tt.tm_min=atoi(str_time+14);
	tt.tm_sec=atoi(str_time+17);
	return mktime(&tt);
}

int gethis(string sTime, string eTime,string timeframe,  stHistory& his)
{
	if(sTime == eTime)
		return FAILE;

	his.sTime =  convert_str_to_tm((char*)sTime.c_str());
	his.eTime =  convert_str_to_tm((char*)eTime.c_str());

	if(timeframe == "M1")
	{
		his.unit = 60*1;
	}
	else if(timeframe == "M5")
	{
		his.unit = 60*5;
	}
	else if(timeframe == "M30")
	{
		his.unit = 30*60;
	}
	else if(timeframe == "H1")
	{
		his.unit = 60*60*1;
	}
	else if(timeframe == "H4")
	{
		his.unit = 60*60*4;
	}
	else if(timeframe == "D1")
	{
		his.unit = 60*60*24*1;
	}
	else if(timeframe == "W1")
	{
		his.unit = 60*60*24*7;
	}
	else if(timeframe == "MN")
	{
		his.unit = 60*60*24*30;
	}
	else if(timeframe == "M15")
	{
		his.unit = 60*60*24*15;
	}
	else
	{
		LOG_INFO("timeframe not find");
		return false;
	}
	his.count = (his.eTime-his.sTime)/his.unit;
	LOG_INFO("UNIT[%d],stime[%s][%ld],etime[%s][%ld]", 
			his.unit, sTime.c_str(),his.sTime, eTime.c_str(), his.eTime);
	return SUCESS;
}

void addtime(time_t srctm, long unit, char* destime, char* szTiemZome)
{
	struct tm * t = NULL;
	srctm += unit;
	t = gmtime(&srctm);
	sprintf(destime, "%04d-%02d-%02dT%02d:%02d:%02d%s", 
			1900+t->tm_year , 1+t->tm_mon, t->tm_mday, t->tm_hour+8, t->tm_min, t->tm_sec, szTiemZome);
	LOG_INFO("TIME[%s]", destime);

}

int getApiByReq(string strSrcIn, string& strDstOut, const char* pEnd, int nModulId)
{
	char* pStart;
	char* sPos=NULL;
	char* ePos=NULL;

	//LOG_INFO("EndStr[%s], nModulId[%d], strReqApi[%s]", pEnd, nModulId, strSrcIn.c_str());
	if (pEnd == NULL)
	{
		LOG_ERR("args is null");
		return FAILE;
	}
	switch(nModulId)
	{
		case 1:
			pStart = (char*)"GET ";
			sPos = strstr((char*)strSrcIn.c_str(), pStart);
			if (sPos == NULL)
			{
				LOG_ERR("get pStart[%s] err", pStart);
				return FAILE;
			}
			if((ePos = strstr(sPos, "historical")) == NULL)
				ePos = strstr((char*)strSrcIn.c_str(), pEnd);
			else
				ePos += strlen("historical");
			strDstOut = strSrcIn.substr(sPos+strlen(pStart) - strSrcIn.c_str(), ePos - sPos - strlen(pStart));
			break;
		case 2:
			pStart = (char*)"POST ";
			sPos = strstr((char*)strSrcIn.c_str(), pStart);
			ePos = strstr((char*)strSrcIn.c_str(), pEnd);

			strDstOut = strSrcIn.substr(sPos+strlen(pStart) - strSrcIn.c_str(), ePos - sPos - strlen(pStart));
			break;

		case 3:
			bool bNum;
			LOG_INFO("rsp by appclient");
			pStart = (char*)"http";
			sPos = strstr((char*)strSrcIn.c_str(), pStart);
			if (sPos == NULL)
			{
				LOG_ERR("get pStart err");
				return FAILE;
			}

			while(*sPos && *sPos != '\r' && *sPos != '\n' )
			{
				if(bNum == true && *sPos == '/')
					break;
				if (isdigit(*sPos) || *sPos == 'm')
				{
					bNum = true;
				}
				else
				{
					bNum = false;
				}
				sPos++;

			}   

			//LOG_DEBUG("SPOS[%s]", sPos);
			if(bNum == true && *sPos)
			{
				ePos = strstr((char*)strSrcIn.c_str(), pEnd);
				if (ePos == NULL || ePos <=sPos)
				{
					LOG_ERR("get api err");
					return FAILE;
				}
				strDstOut = strSrcIn.substr(sPos - strSrcIn.c_str(), ePos - sPos);
			}
			else
			{
				LOG_ERR("get api err");
				return FAILE;
			}
			break;
		default:
			break;
	}

}


void formatUrl(string &s) 
{
	if( !s.empty() )
	{   
		s.erase(0,s.find_first_not_of(" "));
		s.erase(s.find_last_not_of(" ") + 1); 
		s.erase(s.find_last_not_of("/") + 1); 
	}   

}
void formatConfig(stCrmApi &conf)
{
	formatUrl(conf.strUrlCrmHy); 
	formatUrl(conf.strUrlCrmMy); 
	formatUrl(conf.strUrlBoHy); 
	formatUrl(conf.strUrlBoMy); 
}

void DelSrcId(char* s, char* szSrcId)
{
	char* nPos = NULL;
	if ((nPos = strstr(s, TAG_EMAIL)) != NULL || (nPos = strstr(s, TAG_MT4ACC_HIS)) != NULL
			|| (nPos = strstr(s, TAG_MT4ACC)) != NULL || (nPos = strstr(s, TAG_USERNAME)) != NULL )
	{
		while(*nPos && *nPos != '=' && *nPos != ':')
			nPos++;

		if(*nPos == '=' || *nPos == ':')
			nPos++;

		int nLen = strlen(s);
		int nLeft = (nLen - strlen(szSrcId)) - (nPos - s);
		char ltmp[4096] = {0};
		memcpy(ltmp, nPos + strlen(szSrcId), nLeft);
		LOG_DEBUG("nPos[%s] idlen[%d] left len[%d]", nPos, strlen(szSrcId), nLeft);
		memcpy(nPos , ltmp, nLeft);
		s[nLen - strlen(szSrcId)] = 0 ;
	}
}
