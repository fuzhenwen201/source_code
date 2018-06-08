

#include <string.h>
#include <errno.h>
#include <stdio.h>


#include "BaseThread.h"
#include "xlogger.h"

#include "Public.h"
#include "ConfigFileReader.h"


STServiceConfig  stServiceConfig;

MarketMap CoinPairMap;


void InitCoinPair()
{
	vector<std::string>   symbolslist = {"AUD/CAD","AUD/CHF","AUD/HKD","AUD/JPY","AUD/NZD","AUD/SGD","AUD/USD","CAD/CHF","CAD/HKD","CAD/JPY","CAD/SGD","CHF/HKD","CHF/JPY",\
    "CHF/ZAR","EUR/AUD","EUR/CAD","EUR/CHF","EUR/CZK","EUR/DKK","EUR/GBP","EUR/HKD","EUR/HUF","EUR/JPY","EUR/NOK","EUR/NZD","EUR/PLN","EUR/SEK","EUR/SGD",\
    "EUR/TRY","EUR/USD","EUR/ZAR","GBP/AUD","GBP/CAD","GBP/CHF","GBP/HKD","GBP/JPY","GBP/NZD","GBP/PLN","GBP/SGD","GBP/USD","GBP/ZAR","HKD/JPY","NZD/CAD",\
    "NZD/CHF","NZD/HKD","NZD/JPY","NZD/SGD","NZD/USD","SGD/CHF","SGD/HKD","SGD/JPY","TRY/JPY","USD/CAD","USD/CHF","USD/CNH","USD/CZK","USD/DKK","USD/HKD",\
    "USD/HUF","USD/INR","USD/JPY","USD/MXN","USD/NOK","USD/PLN","USD/SAR","USD/SEK","USD/SGD","USD/THB","USD/TRY","USD/ZAR","ZAR/JPY","FINCOIN/USD"};

	 for(int i = 0; i < SymbolBUTT; ++i){
       
        CoinPairMap[symbolslist[i]] = i;
    }
}




uint64_t GetTimestamp(const char *date)
{
    int32_t year,month,day,hour,min,sec,usec;

    assert(nullptr != date);

				//20170608-06:18:16.386
    sscanf(date, "%d-%d-%dT%d:%d:%d.%dZ", &year,&month,&day,&hour,&min,&sec,&usec);

    struct tm stm;
    memset(&stm,0,sizeof(stm));

    stm.tm_year = year - 1900;
    stm.tm_mon = month - 1;
    stm.tm_mday = day;
    stm.tm_hour = hour;
    stm.tm_min = min;
    stm.tm_sec = sec;

    return  ((uint64_t)mktime(&stm)) * 1000000 + usec;
}




int ReadServerConfig()
{
	
	
	
	char *buf = NULL;
	CConfigFileReader config;	
		
	if( -1 == config.LoadFile(SERVICECONFIG)  ) 
	{
		printf("open config file: %s,fail:%s\n",SERVICECONFIG, strerror(errno));
		return -1;
	}

	//-1 ----------------
	
	if (NULL == (buf = config.GetConfigName("SenderCompID")))
	{
		printf("read SenderCompID error \n");
		return -1;
	}
	stServiceConfig.sSender = buf ;

	if (NULL == (buf = config.GetConfigName("TargetCompID")))
	{
		printf("read TargetCompID error \n");
		return -1;
	}
	stServiceConfig.sTarget = buf ;

	
	if (NULL == (buf = config.GetConfigName("Username")))
	{
		printf("read SenderCompID error \n");
		return -1;
	}
	stServiceConfig.spUsername = buf ;
	memset(fortexUserName,0,sizeof(fortexUserName));
	strncpy(fortexUserName,buf,sizeof(fortexUserName)-1);
	//strncpy(fortexUserName,"helloworld",sizeof(fortexUserName)-1);

	if (NULL == (buf = config.GetConfigName("Password")))
	{
		printf("read TargetCompID error \n");
		return -1;
	}
	stServiceConfig.spPassword = buf ;	
	memset(fortexPassword,0,sizeof(fortexPassword));
	strncpy(fortexPassword,buf,sizeof(fortexPassword)-1);
	
	
	
	//-2 ----------------
	
	if (NULL == (buf = config.GetConfigName("nReadCoinfileInterTime")))
	{
		printf("read nReadCoinfileInterTime error \n");
		return -1;
	}
	stServiceConfig.nReadCoinfileInterTime = atoi(buf);

	if (NULL == (buf = config.GetConfigName("nMempoolNodeCount")))
	{
		printf("read nMempoolNodeCount error \n");
		return -1;
	}
	stServiceConfig.nMempoolNodeCount = atoi(buf);


	if (NULL == (buf = config.GetConfigName("nRealtimeSenderCount")))
	{
		printf("read nRealtimeSenderCount error \n");
		return -1;
	}
	stServiceConfig.nRealtimeSenderCount = atoi(buf);
		

	//-3 ------------------
	if (NULL == (buf = config.GetConfigName("mq_vhost")))
	{
		printf("read RabbitMQInfo->mq_vhost error \n");
		return -1;
	}
	stServiceConfig.mq_vhost = buf;


	
	if (NULL == (buf = config.GetConfigName("mq_exchange")))
	{
		printf("read RabbitMQInfo->mq_exchange error \n");
		return -1;
	}
	stServiceConfig.mq_exchange = buf;


	
	if (NULL == (buf = config.GetConfigName("mq_exchangeGroup")))
	{
		printf("read RabbitMQInfo->mq_exchangeGroup error \n");
		return -1;
	}
	stServiceConfig.mq_exchangeGroup = buf;


	
	if (NULL == (buf = config.GetConfigName("mq_address")))
	{
		printf("read RabbitMQInfo->mq_address error \n");
		return -1;
	}
	stServiceConfig.mq_address = buf;

	
	
	if (NULL == (buf = config.GetConfigName("mq_port")))
	{
		printf("read RabbitMQInfo->mq_port error \n");
		return -1;
	}
	stServiceConfig.mq_port = buf;

	
	
	if (NULL == (buf = config.GetConfigName("mq_user")))
	{
		printf("read RabbitMQInfo->mq_user error \n");
		return -1;
	}
	stServiceConfig.mq_user = buf;


	
	if (NULL == (buf = config.GetConfigName("mq_password")))
	{
		printf("read RabbitMQInfo->mq_password error \n");
		return -1;
	}
	stServiceConfig.mq_password = buf;


	
	if (NULL == (buf = config.GetConfigName("mq_queueid")))
	{
		printf("read RabbitMQInfo->mq_queueid error \n");
		return -1;
	}
	stServiceConfig.mq_queue = buf;


	if (NULL == (buf = config.GetConfigName("logdir")))
	{
		printf("read RabbitMQInfo->logdir error \n");
		return -1;
	}
	stServiceConfig.logdir = buf;

	if (NULL == (buf = config.GetConfigName("onlyscreenshowlog")))
	{
		printf("read onlyscreenshowlog error \n");
		return -1;
	}
	if (1 == atoi(buf))
		stServiceConfig.onlyscreenshowlog = true;
	else
		stServiceConfig.onlyscreenshowlog = false;


	return 0;
	

}


void InitLogSys()
{
	(*XLogger::GetObject("fix_se")).StdErr(stServiceConfig.onlyscreenshowlog).Colour(true).Dir(stServiceConfig.logdir).BuffDelay(1);
	XINFO("InitLogSys ....");
}





