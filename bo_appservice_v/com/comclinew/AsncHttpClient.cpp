#include"../../include/AsncHttpClient.h"

int DebugInfo(CURL *pcurl, curl_infotype itype, char * pData, size_t size, void *)  
{  
#if 1
	if(itype == CURLINFO_TEXT)  
	{  
		LOG_DEBUG("TEXT:[%s]", pData);
	}  
	else if(itype == CURLINFO_HEADER_IN)  
	{  
		LOG_DEBUG("HEADER_IN[%s]:", pData);
	}  
	else if(itype == CURLINFO_HEADER_OUT)  
	{  
		LOG_INFO("HEADER_OUT:[%s]SIZE:[%d]", pData,size);
	}  
	else if(itype == CURLINFO_DATA_IN)  
	{  
		LOG_DEBUG("DATA_IN:[%s]",pData);  
	}  
	else if(itype == CURLINFO_DATA_OUT)  
	{  
		LOG_DEBUG("DATA_OUT:[%s],SIZE:[%d]", pData,size);  
	}  
	return 0;  
#endif

}  

 
AsncHttpClient::AsncHttpClient(): m_headerlist(NULL),m_curl(NULL)
{
	//curl_slist_free_all(m_headerlist);
}
AsncHttpClient::~AsncHttpClient()
{
	//Destory();
}

void AsncHttpClient::SetHeaders(const char* pHeader)
{
	m_headerlist = curl_slist_append(m_headerlist, pHeader);
}

void AsncHttpClient::Post(const char* pUrl, const char* body ,size_t size)
{
	
	if(size >= 16*1024)
	{
		curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, size);
		//m_headerlist = curl_slist_append(headers,"Expect:");
	}
	else
	{
		//curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, body);
		curl_easy_setopt(m_curl,CURLOPT_COPYPOSTFIELDS,body);
	}
	curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_headerlist);// 改协议头
	curl_easy_setopt(m_curl, CURLOPT_URL, pUrl);   // set url 
	curl_easy_setopt(m_curl, CURLOPT_POST, 1); 
}

void AsncHttpClient::Get(const char* pUrl)
{
	curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_headerlist);// 改协议头
	curl_easy_setopt(m_curl, CURLOPT_URL, pUrl);   // set url 

}		

int AsncHttpClient::Create()
{
	//curl_global_init(CURL_GLOBAL_ALL);
	m_curl = curl_easy_init(); // init 
	if(!m_curl)
	{
		return -1;
	}
	curl_easy_setopt(m_curl, CURLOPT_HEADER, 1L);
	
	return 0;
}

int AsncHttpClient::Destory()
{
	if(m_headerlist != NULL)
		curl_slist_free_all(m_headerlist);
	if(m_curl != NULL)
		curl_easy_cleanup(m_curl);
	return 0;
}

 
void AsncHttpClient::SettingBuffer(pFun read,void* r_param,pFun write,void* w_param)
{
	curl_easy_setopt(m_curl, CURLOPT_READFUNCTION, read);  
	curl_easy_setopt(m_curl, CURLOPT_READDATA, r_param);
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, write);  
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, w_param); 
}
void AsncHttpClient::SettingCURL()
{

	curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, false);  
	curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, false);  

	curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1);  
		// 超时时间 
	curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT, 300);  
	curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 300);

	//Web服务器一般会重定向链接，比如访问http:/xxx/x1.do自动转到http:/xxx/x2.do  
	//所以一定要设置CURLOPT_FOLLOWLOCATION为1,否则重定向后的数据不会返回。  
	//curl_easy_setopt(curl, CURLOPT_URL, pUrl);  
	curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION,1);  

#if 1
	curl_easy_setopt(m_curl, CURLOPT_DEBUGFUNCTION, DebugInfo);//打印完整的调试信息  
	curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1);//打印调试信息  
#endif

	curl_easy_setopt(m_curl, CURLOPT_ACCEPT_ENCODING, "gzip");  
}

void AsncHttpClient::SettingCURL(int signal)
{
	curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, false);  
	curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, false);  

	curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1);  
		// 超时时间 
	curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT, 30);  
	curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 30);

	//Web服务器一般会重定向链接，比如访问http:/xxx/x1.do自动转到http:/xxx/x2.do  
	//所以一定要设置CURLOPT_FOLLOWLOCATION为1,否则重定向后的数据不会返回。  
	//curl_easy_setopt(curl, CURLOPT_URL, pUrl);  
	curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION,1);  

#if 0
	curl_easy_setopt(m_curl, CURLOPT_DEBUGFUNCTION, DebugInfo);//打印完整的调试信息  
	curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1);//打印调试信息  
#endif

	curl_easy_setopt(m_curl, CURLOPT_ACCEPT_ENCODING, "gzip");  
}