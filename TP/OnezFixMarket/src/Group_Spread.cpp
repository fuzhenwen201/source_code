#include <time.h>
#include <sys/stat.h>
#include<sqlite3.h>

#include "Group_Spread.h" 
#include "Public.h"	



static char db[]="symbol_spread.db" ;


time_t CGroupSpread::m_nChangeTime = -1 ;



CGroupSpread::CGroupSpread()
{
	m_nChangePreTime = 0;
	m_nReadDBTime = 0;
}








int ReadSymbolSpreadCallback(void* arg,int nCount,char** pValue,char** pName)
{
	map<string, int> 	*pSymbolSpreadList = (map<string, int> 	*)arg;
	
	int spread = atoi(pValue[1]);
	
	pSymbolSpreadList->insert(pair<string ,int>(pValue[0],spread));
	
    return 0;
}


 
int CGroupSpread::ReadSymbolSpreadList()
{

	if( m_nChangePreTime == m_nChangeTime )return 0;

	m_nChangePreTime = m_nChangeTime ;
	
	
	char sSQL[]= "SELECT symbol,symbol_spread FROM symbol_spread; " ;
	
	
	sqlite3* pDBHandle;
	int nResult = sqlite3_open(db, &pDBHandle);
	if (nResult != SQLITE_OK) 
	{		
		XERROR("Cannot open databse[%s]: %s", db , sqlite3_errmsg(pDBHandle));		
		sqlite3_close(pDBHandle);	
		return -1;	
	}


	m_SymbolSpreadList.clear();
	char* errmsg;
	nResult = sqlite3_exec(pDBHandle,sSQL,ReadSymbolSpreadCallback,&m_SymbolSpreadList,&errmsg);
    if (nResult != SQLITE_OK)
    {
    	XERROR("sqlite3_exec error:sql[%s],error:%s", sSQL,errmsg);		
        sqlite3_close(pDBHandle);
        sqlite3_free(errmsg);
        return -1;
    }

	
	
	sqlite3_close(pDBHandle);
	

	

	ShowSymbolSpreadList();
	
	return 0;
}




int CGroupSpread::GetSymbolSpread( const char *symbol,int &symbolspread)
{
	map<string, int>::iterator itr;
	itr = m_SymbolSpreadList.find(symbol);
	if (itr == m_SymbolSpreadList.end())
	{
		XERROR("GetSymbolSpread find don't	symbol:%s, \n",symbol);
		return -1;
	}

	symbolspread = itr->second;

	return 0;
}
	





int CGroupSpread::CalAskBidSpread(int &ask, int &bid, int totalspread)
{
	
	totalspread *= 10; // spread 从小数后第5位开始算,所以要 *10;
	int a = totalspread/2 ;
	int b = totalspread - a;

	ask = ask + a ;
	bid = bid - b ;
	

	return 0;
}



void 	CGroupSpread::ShowSymbolSpreadList()
{
	map<string, int>::iterator itr;

	printf("\n");
	for(itr=m_SymbolSpreadList.begin();itr!=m_SymbolSpreadList.end();itr++)
	{
		XINFO("Show Symbol:%s, Spread:%d",(itr->first).c_str(),itr->second);
	}

}





