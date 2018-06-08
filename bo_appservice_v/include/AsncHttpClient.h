#ifndef _HTTPCLIENT_H_
#define _HTTPCLIENT_H_

#include "log.h"
#include "tcp.h"
#include "const.h"
#include "common.h"
#include "curl/curl.h"

typedef size_t (*pFun)(void* buffer, size_t size, size_t nmemb, void* lpVoid);

int DebugInfo(CURL *pcurl, curl_infotype itype, char * pData, size_t size, void *)  ;

class AsncHttpClient
{
	public:
		AsncHttpClient();
		~AsncHttpClient();
		CURL* GetCURL() { return m_curl; }
		curl_slist* GetHeaderList() { return m_headerlist;}
		void SetHeaders(const char* pHeader);
		void Post(const char* pUrl, const char* body ,size_t size);
		void Get(const char* pUrl);
		//void SettingBuffer(curl_read_callback read,void* r_param,curl_write_callback write,void* w_param);
		void SettingBuffer(pFun read,void* r_param,pFun write,void* w_param);
		void SettingCURL();
		void SettingCURL(int signal);
		int Create();
		int Destory();
		 
	private:
		struct curl_slist *m_headerlist;
		CURL *m_curl;

};



#endif
