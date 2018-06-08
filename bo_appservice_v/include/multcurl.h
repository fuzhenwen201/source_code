#ifndef _MULTCURL_H_
#define _MULTCURL_H_

#include "curl/curl.h"
#include "curl/multi.h"
#include "const.h"
#include "log.h"
#include <map>
#include <semaphore.h>
#include <pthread.h>

struct HttpInfo
{
	char m_data[BUFSZ];
	size_t m_size;
	size_t m_status_code;
	void* m_ret;
	HttpInfo():m_size(0),m_status_code(-1),m_ret(NULL)
	{
		memset(m_data,0x00,sizeof(m_data));

	}
};

typedef void (*url_callback)(void*);
class MultHttpClient
{
	public:
		MultHttpClient();
		~MultHttpClient();
	public:
		int InitMultCurl();
		int AddCurlHandle(CURL *curl,HttpInfo *httpInfo);
		int Dispatch(url_callback fun);
#if 0
	protected:
		void SetHeaders(struct curl_slist *header ,const char* pHeader);
		void Post(struct CURL *curl,struct curl_slist *header,const char* pUrl, const char* body ,size_t size);
		void Get(struct CURL *curl,struct curl_slist *header,const char* pUrl);
		void SettingBuffer(struct CURL *curl,pFun read,void* r_param,pFun write,void* w_param);
		struct CURL * CreateCUrl();
		int Destory(struct CURL *curl,struct curl_slist *header);

		void SettingCURL(struct CURL *curl);
#endif 
		
	private:
		CURLM *m_cm;
		std::map<CURL*,HttpInfo*>multiurlmap;
		pthread_cond_t m_cond /*=PTHREAD_MUTEX_INITIALIZER*/; //申明条锁,并用宏进行初始化
		pthread_mutex_t m_mutex ;
		pthread_mutex_t m_cm_mutex;


};



#endif 

