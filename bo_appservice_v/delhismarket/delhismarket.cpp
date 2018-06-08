#include <stdio.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "../include/log.h"
#include "../include/const.h"
#include "../include/common.h"
#include "../include/redis.h"
#include <assert.h>
#include <unistd.h>


int main(int argc, char *argv[])
{

	InitLog("delhismarket", argv[1]);
	//初始化redis的IP和端口
	char *pRdsIp = NULL;
    char *pRdsPort = NULL;
    char *pRdsTimeout = NULL;
    char *pRdsPassword = NULL;
    char *pRdsIns = NULL;
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
	
    CRedis pRedis;
	int nRet = pRedis.connect(pRdsIp, atoi(pRdsPort), atoi(pRdsTimeout), pRdsPassword == NULL ? "" : pRdsPassword, pRdsIns == NULL ? "" : pRdsIns);
    if(nRet == FAILE)
    {
        LOG_ERR("connect reids err[%d]", nRet);
        return -1;
    }

    char* pSleepTime = NULL;
    char* pSpaceTime = NULL;
    M_XRETURN(((pSleepTime = getenv("HISTIME_SLEEP")) == NULL),
              NULL, LOG_ERR("get env HISTIME_SLEEP err!"));
    M_XRETURN(((pSpaceTime = getenv("HISTIME_SPACE")) == NULL),
              NULL, LOG_ERR("get env HISTIME_SPACE err!"));
    size_t histime_sleep = atoi(pSleepTime);
    size_t histime_space = atoi(pSpaceTime);
    // find all kinds
    while(1)
    {
        std::string kind = pRedis.get(HISMARKET);
        if(kind.empty())
        {
            sleep(histime_sleep);
            continue;
        }
        LOG_DEBUG("kind [%s]", kind.c_str());
        Json::Value value ;
        Json::Reader reaer;
        time_t curr_time = NowTime();
        if(reaer.parse(kind, value))
        {
            const Json::Value key = value["breed"];
            std::vector<std::string> keyvector;
            for(size_t i = 0 ; i < key.size(); i++)
            {
                pRedis.getkeyfield(key[i]["kinds"].asString(), keyvector);
                for(size_t j = 0; j < keyvector.size(); j++)
                {
                    std::string date = keyvector[j].substr(keyvector[j].find(',') + 1, 10);
                    time_t last_time = str_to_time_t(date, "%d-%d-%d") + histime_space;
                    //LOG_INFO("date[%s] current_time[%d] last_time[%d]",date.c_str(),curr_time,last_time);
                    if(last_time < curr_time)
                    {
                    	//LOG_INFO("delete suceess");
                        pRedis.hdel(key[i]["kinds"].asString(), keyvector[j]);
                    }
                }
                keyvector.clear();
            }

        }
        sleep(histime_sleep);
    }
    return 0;
}