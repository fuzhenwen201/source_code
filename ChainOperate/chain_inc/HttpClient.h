#ifndef _HTTPCLIENT_H_
#define _HTTPCLIENT_H_

//#include "log.h"
//#include "tcp.h"
//#include "const.h"
//#include "common.h"
#include <curl/curl.h>
#include <stdlib.h>
#include <cstring>
typedef size_t (*pFun)(void* buffer, size_t size, size_t nmemb, void* lpVoid);

int DebugInfo(CURL *pcurl, curl_infotype itype, char * pData, size_t size, void *)  ;

class HttpClient
{
	public:
		HttpClient();
		~HttpClient();
		void SetHeaders(const char* pHeader);
		void Post(const char* pUrl, const char* body ,size_t size);
		void Post(const char* pUrl, size_t size);
		void Get(const char* pUrl);
		//void SettingBuffer(curl_read_callback read,void* r_param,curl_write_callback write,void* w_param);
		void SettingBuffer(pFun read,void* r_param,pFun write,void* w_param);
		void SettingCURL();
		void SetUserInfo(const char* userpwd);
		int Create();
		int Destory();
		int Run();
	private:
		struct curl_slist *m_headerlist;
		CURL *m_curl;

};



#endif
