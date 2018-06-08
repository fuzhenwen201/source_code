

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/time.h>
#include <signal.h>


#include "BaseThread.h"
#include "xlogger.h"

#include "Public.h"
#include "ConfigFileReader.h"


STServiceConfig  stServiceConfig;

MarketMap CoinPairMap;


void InitCoinPair()
{
	vector<std::string>   symbolslist = {
"AUDCAD",
"AUDSGD",
"CADSGD",
"EURCAD",
"GBPJPY",
"NZDJPY",
"SGDJPY",
"USDJPY",
"ZARJPY",
"AUDCHF",
"AUDUSD",
"EURCHF",
"GBPAUD",
"GBPNZD",
"TRYJPY",
"USDSGD",
"CADCHF",
"CHFJPY",
"EURJPY",
"EURSGD",
"GBPCAD",
"NZDCAD",
"NZDUSD",
"USDCAD",
"USDHKD",
"AUDJPY",
"EURTRY",
"GBPCHF",
"GBPSGD",
"NZDCHF",
"USDCHF",
"USDTRY",
"AUDNZD",
"CADJPY",
"EURAUD",
"EURGBP",
"EURNZD",
"EURUSD",
"GBPUSD",
"USDCNH",
"USDINR"

	};

	 for(int i = 0; i < SymbolBUTT; ++i){
       
        CoinPairMap[symbolslist[i]] = i;
    }
}





uint64_t GetTimestamp(const char *date)
{
    int year,month,day,hour,min,sec,usec;
    assert(NULL != date);

    sscanf(date, "%04d%02d%02d-%d:%d:%d.%d", &year,&month,&day,&hour,&min,&sec,&usec);
   
    struct tm stm;
    memset(&stm,0,sizeof(stm));

    stm.tm_year = year - 1900;
    stm.tm_mon = month - 1;
    stm.tm_mday = day;
    stm.tm_hour = hour;
    stm.tm_min = min;
    stm.tm_sec = sec;

   
   	time_t time0 = mktime(&stm) ;
	time_t time1 = time(&time0) * 1000000 + usec ;
   
	return time1 ;
   
}




uint64_t GetUTCCurTimestamp()
{
    struct timeval  curtime;
    gettimeofday(&curtime,NULL);

    uint64_t cut_timestamp  = curtime.tv_sec * 1000000 + curtime.tv_usec ;
    return cut_timestamp;
}



int ReadServerConfig()
{
	
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigaction( SIGPIPE, &sa, 0 );
	
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


	if (NULL == (buf = config.GetConfigName("nReadDBInterTime")))
	{
		printf("read nReadDBInterTime error \n");
		return -1;
	}
	stServiceConfig.nReadDBInterTime = atoi(buf);

	

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


	// db change mq info
	
	if (NULL == (buf = config.GetConfigName("mq_vhost_chge")))
	{
		printf("read RabbitMQInfo->mq_vhost_chge error \n");
		return -1;
	}
	stServiceConfig.mq_vhost_chge = buf;


	
	if (NULL == (buf = config.GetConfigName("mq_exchangeGroup_chge")))
	{
		printf("read RabbitMQInfo->mq_exchangeGroup_chge error \n");
		return -1;
	}
	stServiceConfig.mq_exchangeGroup_chge = buf;




	
	if (NULL == (buf = config.GetConfigName("mq_address_chge")))
	{
		printf("read RabbitMQInfo->mq_address_chge error \n");
		return -1;
	}
	stServiceConfig.mq_address_chge = buf;

	
	
	if (NULL == (buf = config.GetConfigName("mq_port_chge")))
	{
		printf("read RabbitMQInfo->mq_port_chge error \n");
		return -1;
	}
	stServiceConfig.mq_port_chge = buf;

	
	
	if (NULL == (buf = config.GetConfigName("mq_user_chge")))
	{
		printf("read RabbitMQInfo->mq_user_chge error \n");
		return -1;
	}
	stServiceConfig.mq_user_chge = buf;


	
	if (NULL == (buf = config.GetConfigName("mq_password_chge")))
	{
		printf("read RabbitMQInfo->mq_password_chge error \n");
		return -1;
	}
	stServiceConfig.mq_password_chge = buf;


	
	if (NULL == (buf = config.GetConfigName("mq_bindingkey_chge")))
	{
		printf("read RabbitMQInfo->mq_bindingkey_chge error \n");
		return -1;
	}
	stServiceConfig.mq_bindingkey_chge = buf;



	if (NULL == (buf = config.GetConfigName("mq_queueid_chge")))
	{
		printf("read RabbitMQInfo->mq_queueid_chge error \n");
		return -1;
	}
	stServiceConfig.mq_queueid_chge = atoi(buf);

	// end change 




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



	if (NULL == (buf = config.GetConfigName("DelayMicrosecond")))
	{
		printf("read DelayMicrosecond error \n");
		return -1;
	}
	stServiceConfig.nDelayMicrosecond = atol(buf) ;
	

	return 0;
	

}


void InitLogSys()
{
	(*XLogger::GetObject("OnezFixMarket")).StdErr(stServiceConfig.onlyscreenshowlog).Colour(true).Dir(stServiceConfig.logdir).BuffDelay(1);
	XINFO("InitLogSys ....");
}





