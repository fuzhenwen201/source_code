#include "../../include/multcurl.h"

MultHttpClient::MultHttpClient(): m_cm(NULL)
{

}

MultHttpClient::~MultHttpClient()
{

}

int MultHttpClient::InitMultCurl()
{
    pthread_cond_init(&m_cond, NULL);
    pthread_mutex_init(&m_mutex, NULL);
    pthread_mutex_init(&m_cm_mutex,NULL);
    //curl_global_init(CURL_GLOBAL_ALL);
    m_cm = curl_multi_init();
}

int MultHttpClient::AddCurlHandle(CURL *curl, HttpInfo *httpInfo)
{

    
    int still_running = 0;
#if 0
    while(CURLM_CALL_MULTI_PERFORM == curl_multi_perform(m_cm, &still_running))
    {
        LOG_INFO("AddCurlHandle still_running %d",still_running);
    }
#endif 
    pthread_mutex_lock(&m_mutex);
    multiurlmap.insert(std::pair<CURL *, HttpInfo *>(curl, httpInfo));
    pthread_mutex_lock(&m_cm_mutex);
    curl_multi_add_handle(m_cm, curl);
    pthread_mutex_unlock(&m_cm_mutex);
    if(multiurlmap.size() == 1)
    {
        pthread_cond_signal(&m_cond);
        LOG_INFO("signal.......");
    }
    pthread_mutex_unlock(&m_mutex);
}

int MultHttpClient::Dispatch(url_callback fun)
{
     
    while(1)
    {

        int still_running = 0;
        int msgs_left = 0;
        CURLMsg *msg = NULL;

        pthread_mutex_lock(&m_mutex);

        if(multiurlmap.size() == 0)
        {
            
            LOG_INFO("wait before....");
            pthread_cond_wait(&m_cond, &m_mutex);
           
            LOG_INFO("wait after.....");
        }
        pthread_mutex_unlock(&m_mutex);
#if 1
        LOG_DEBUG("still_running .......");
        //curl_multi_perform(m_cm, &still_running);
        int run_num = 0 ;

        do
        {
            
            int numfds = 0;
            
            //mc = curl_multi_perform(m_cm, &still_running);
             
            int res = curl_multi_wait(m_cm, NULL, 0, 30 * 1000, &numfds);
            
            if(res != CURLM_OK)
            {
                LOG_ERR("error: curl_multi_wait() returned %d", res);
                //fprintf(stderr,"error: curl_multi_wait() returned %d\n", res);
                //pthread_mutex_unlock(&m_mutex);
                break;
            }
            pthread_mutex_lock(&m_cm_mutex);
            while(CURLM_CALL_MULTI_PERFORM == curl_multi_perform(m_cm, &still_running))
            {
                LOG_DEBUG("AddCurlHandle still_running %d",still_running);
            }
            
            LOG_DEBUG("still_running value[%d]",still_running);
            pthread_mutex_unlock(&m_cm_mutex); 
           
        }
        while(still_running);
         
#endif

        //pthread_mutex_unlock(&m_mutex);
        CURL *curl = NULL;
        while ((msg = curl_multi_info_read(m_cm, &msgs_left)))
        {
            if (msg->msg == CURLMSG_DONE)
            {
                curl = msg->easy_handle;
                CURLcode return_code = msg->data.result;
                if(return_code != CURLE_OK  && return_code != CURLE_OPERATION_TIMEDOUT)
                {
                    pthread_mutex_lock(&m_mutex);
                    std::map<CURL *, HttpInfo *>::iterator iter = multiurlmap.find(curl);
                    if(iter != multiurlmap.end())
                    {
                        fun(iter->second);
                        multiurlmap.erase(iter);
                        pthread_mutex_lock(&m_cm_mutex);
                        curl_multi_remove_handle(m_cm, curl);
                        pthread_mutex_unlock(&m_cm_mutex);
                    }
                    LOG_ERR("CURL error code: %d", msg->data.result);
                    pthread_mutex_unlock(&m_mutex);
                    continue;
                }

                // Get HTTP status code
                
                long http_status_code = 0;
                char *szUrl = NULL;
                struct curl_slist *list = NULL;
                curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_status_code);
                curl_easy_getinfo(curl, CURLINFO_PRIVATE, &szUrl);
                if(http_status_code == 301)
                {
                    pthread_mutex_lock(&m_cm_mutex);
                    char *nPos = NULL;
                    LOG_INFO("301  %s", szUrl);
                    if((nPos = strstr(szUrl, "my")) != NULL)
                    {
                        nPos[0] = 'h';
                        LOG_INFO("URL[%s]", szUrl);
                        curl_easy_setopt(curl, CURLOPT_URL, szUrl);
                        curl_multi_perform(m_cm, &still_running);
                        //pClient->Post(Url, SndTag, strlen(SndTag));

                    }
                    else if((nPos = strstr(szUrl, "hy")) != NULL)
                    {
                        nPos[0] = 'm';
                        LOG_INFO("URL[%s]", szUrl);
                        curl_easy_setopt(curl, CURLOPT_URL, szUrl);
                        curl_multi_perform(m_cm, &still_running);
                    }
                    pthread_mutex_unlock(&m_cm_mutex);
                    continue;
                }
                LOG_INFO("GET of %s  %d", szUrl, http_status_code);
                //curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &list);
                pthread_mutex_lock(&m_mutex);
                std::map<CURL *, HttpInfo *>::iterator iter = multiurlmap.find(curl);
                if(iter != multiurlmap.end())
                {
                    fun(iter->second);
                    multiurlmap.erase(iter);
                    pthread_mutex_lock(&m_cm_mutex);
                    curl_multi_remove_handle(m_cm, curl);
                    pthread_mutex_unlock(&m_cm_mutex);
                    //curl_slist_free_all(list);
                    curl_easy_cleanup(curl);
                    //curl = NULL;
                    //pthread_mutex_unlock(&m_mutex);
                }
                pthread_mutex_unlock(&m_mutex);
            }

            else
            {
                //printf("stilling remain curl_handle\n");
                LOG_ERR("error: after curl_multi_info_read(), CURLMsg=%d", msg->msg);
            }

            
        }
        //pthread_mutex_unlock(&m_mutex);
    }
}
