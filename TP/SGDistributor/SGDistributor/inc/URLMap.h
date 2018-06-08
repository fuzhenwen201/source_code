
/*******************************************************************************************

*Department:	TP

*Decrible:  	SG 本地IP URL 映射

				备注,请先安装sqlite的开发库: 
				sudo apt-get install libsqlite3-dev
				
			
*Auhor:			Savin Chen

*Createdtime:	2017-08-15

*ModifyTime:	


********************************************************************************************/




#ifndef SGIP_URL_HEAD_H
#define SGIP_URL_HEAD_H
#include <time.h>

#include "Public.h"

#define URLMAXLEN 66


typedef struct __STSGLocalIPPort
{
	unsigned int 		local_ip;
	unsigned short 		local_port;
	char				url[URLMAXLEN];

}STSGLocalIPPort;


class CSgIpUrlMap
{
public:

	CSgIpUrlMap();

	int 			ReadURLMap();
	int				GetURLFromSGIp(unsigned int sgipport,char *url);
	unsigned int 	GetSGipFromURL(const char *sgurl);
	void 			ShowUrlMapList();
	
	
	map<unsigned int,STSGLocalIPPort *> 	m_SGipUrlList;
	 
	int 									m_nReadDBTime;
	 
	time_t 						 			m_nChangePreTime;	 
	static time_t							m_nChangeTime;
	 

	
	
};



#endif //SGIP_URL_HEAD_H


