
#include <sys/stat.h>

#include<sqlite3.h>
#include "URLMap.h"


 
#include "Public.h"	



static char db[]="sgd_urlmap.db" ;

time_t CSgIpUrlMap::m_nChangeTime = -1;


CSgIpUrlMap::CSgIpUrlMap()
{
	m_nReadDBTime = 0;
	m_nChangePreTime = 0;
}





int ReadUrlMapCallback(void* arg,int nCount,char** pValue,char** pName)
{
			
	map<unsigned int,STSGLocalIPPort *>  	*pSGipUrlList = (map<unsigned int,STSGLocalIPPort *> 	*)arg;
	
	STSGLocalIPPort * sginfo = new STSGLocalIPPort;
	
	
	sginfo->local_ip = ntohl(striptoip(pValue[0]));
	sginfo->local_port= atoi(pValue[1]);
	memcpy(sginfo->url,pValue[2],URLMAXLEN-1);
	pSGipUrlList->insert(pair<unsigned int ,STSGLocalIPPort *>(sginfo->local_ip + sginfo->local_port ,sginfo ));
	
    return 0;
}





int  CSgIpUrlMap::ReadURLMap()
{
	if( m_nChangePreTime == m_nChangeTime )return 0;

	m_nChangePreTime = m_nChangeTime ;
	
	char sSQL[]= "SELECT sg_localip,sg_localport,URL FROM sg_ipurl_map" ;
	
	
	sqlite3* pDBHandle;
	int nResult = sqlite3_open(db, &pDBHandle);
	if (nResult != SQLITE_OK) 
	{		
		XERROR("Cannot open databse[%s]: %s", db , sqlite3_errmsg(pDBHandle));		
		sqlite3_close(pDBHandle);	
		return -1;	
	}


	m_SGipUrlList.clear();
	char* errmsg;
	nResult = sqlite3_exec(pDBHandle,sSQL,ReadUrlMapCallback,&m_SGipUrlList,&errmsg);
    if (nResult != SQLITE_OK)
    {
    	XERROR("sqlite3_exec error:sql[%s],error:%s", sSQL,errmsg);		
        sqlite3_close(pDBHandle);
        sqlite3_free(errmsg);
        return -1;
    }

	
	
	sqlite3_close(pDBHandle);
	

	ShowUrlMapList();

	return 0;

}




int			CSgIpUrlMap::GetURLFromSGIp(unsigned int sgipport,char *url)
{
	assert(url!=NULL);
	
	map<unsigned int,STSGLocalIPPort *>::iterator itr ;
	itr = m_SGipUrlList.find(sgipport);
	if(itr == m_SGipUrlList.end())
	{	
		XERROR("GetURLFromSGIp fail,sgip:%s\n", iptostrip(htonl(sgipport)));	
		return -1;
	}

	STSGLocalIPPort * sginfo = (STSGLocalIPPort *)(itr->second) ;

	strncpy(url,sginfo->url,URLMAXLEN -1);

	return 0;
}




unsigned int	CSgIpUrlMap::GetSGipFromURL(const char *sgurl)
{
	
	
	map<unsigned int,STSGLocalIPPort *>::iterator itr ;
	for (itr=m_SGipUrlList.begin(); itr!=m_SGipUrlList.end(); ++itr)
    {
		//const char *p =  (itr->second).c_str() ;
		STSGLocalIPPort * sginfo = (STSGLocalIPPort *)(itr->second) ;
		if(0 == strcmp(sgurl,sginfo->url))
		{
			return  sginfo->local_ip;
		}
	}
	
	XERROR("GetSGipFromURL fail,sgurl:%s\n", sgurl );	
	return 0;

}




void 	CSgIpUrlMap::ShowUrlMapList()
{
	
	map<unsigned int,STSGLocalIPPort *>::iterator itr ;
	for (itr=m_SGipUrlList.begin(); itr!=m_SGipUrlList.end(); ++itr)
	{
		STSGLocalIPPort * sginfo = (STSGLocalIPPort *)(itr->second) ;
		XINFO("URL: sg_localip:%s,sg_localport:%d,sgurl:%s",  iptostrip(htonl(sginfo->local_ip)), sginfo->local_port, sginfo->url);
	}
	

}





