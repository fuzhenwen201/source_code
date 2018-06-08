

#include<sqlite3.h>
#include "Group_Spread.h"


 
time_t CGroupSpread::m_nChangeTime_grpsecurity = -1 ;
time_t CGroupSpread::m_nChangeTime_bosymbols = -1 ;



int ReadGroupSymbolsCallback(void*pList ,int nCount,char** pValue,char** pName)
{
	map<unsigned int ,map<string,int>> 	*pGroupSymbolsList = (map<unsigned int ,map<string,int> > 	*)pList;

	if(NULL == pValue[0] || NULL == pValue[1] || NULL == pValue[2])return 0;

	unsigned int groupid = atoi(pValue[0]);
	int spread  = atoi(pValue[1]);
	string symbols = pValue[2];

	map<unsigned int,map<string,int>>::iterator itr;
	itr = pGroupSymbolsList->find(groupid);
	if (itr == pGroupSymbolsList->end())
	{
		map<string,int> data;
		data.insert( pair<string,int>(symbols,spread));
		pGroupSymbolsList->insert( pair<int ,map<string,int>>(groupid,data));
	}
	else
	{
		map<string,int> &data = itr->second;
		data.insert( pair<string,int>(symbols,spread));
	}

	

	
    return 0;
}



int CGroupSpread::ReadGroupSymbolsList()
{
	if( m_nChangePreTime_grpsecurity == m_nChangeTime_grpsecurity )return 0;

	m_nChangePreTime_grpsecurity = m_nChangeTime_grpsecurity ;
	
	char sSQL[256];
	memset(sSQL,0,sizeof(sSQL));
	strcpy(sSQL,"SELECT A.group_id,A.security_spread,B.symbol_list FROM group_fx A, security_fx B WHERE A.security_id = B.security_id;");

	char db[]="msg_service.db" ;
	sqlite3* pDBHandle;
	int nResult = sqlite3_open(db, &pDBHandle);
	if (nResult != SQLITE_OK) 
	{		
		XERROR("Cannot open databse: %s", sqlite3_errmsg(pDBHandle));		
		sqlite3_close(pDBHandle);	
		
		return -1;	
	}

	//m_GroupSymbolsList.clear();
	map<unsigned int ,map<string,int>> 		tmp_GroupSymbolsList;

	
	char* errmsg;
	nResult = sqlite3_exec(pDBHandle,sSQL,ReadGroupSymbolsCallback,&tmp_GroupSymbolsList,&errmsg);
    if (nResult != SQLITE_OK)
    {
    	XERROR("sqlite3_exec error:sql[%s],error:%s", sSQL,errmsg);		
		m_nChangeTime_grpsecurity= time(NULL);
        sqlite3_close(pDBHandle);
        sqlite3_free(errmsg);
		
        return -1;
    }

	
	sqlite3_close(pDBHandle);

	m_GroupSymbolsList.clear();
	m_GroupSymbolsList.swap(tmp_GroupSymbolsList);
	
	ShowGroupSymbolsList(1);
	
	return 0;
}







int CGroupSpread::ReadGroupBoSymbolsList()
{
	if( m_nChangePreTime_bosymbols == m_nChangeTime_bosymbols )return 0;

	m_nChangePreTime_bosymbols = m_nChangeTime_bosymbols ;
	
	char sSQL[256];
	memset(sSQL,0,sizeof(sSQL));
	strcpy(sSQL,"select a.group_id,a.bo_caltype,b.symbol_list from group_bo A,security_bo B where a.security_id = b.security_id;");

	char db[]="msg_service.db" ;
	sqlite3* pDBHandle;
	int nResult = sqlite3_open(db, &pDBHandle);
	if (nResult != SQLITE_OK) 
	{		
		XERROR("Cannot open databse: %s", sqlite3_errmsg(pDBHandle));		
		sqlite3_close(pDBHandle);	
		
		return -1;	
	}


	map<unsigned int ,map<string,int>> 		tmp_GroupSymbolsList;
	
	char* errmsg;
	nResult = sqlite3_exec(pDBHandle,sSQL,ReadGroupSymbolsCallback,&tmp_GroupSymbolsList,&errmsg);
    if (nResult != SQLITE_OK)
    {
    	XERROR("sqlite3_exec error:sql[%s],error:%s", sSQL,errmsg);	
		m_nChangeTime_bosymbols = time(NULL);
        sqlite3_close(pDBHandle);
        sqlite3_free(errmsg);
		
        return -1;
    }

	
	sqlite3_close(pDBHandle);

	m_GroupBoSymbolsList.clear();
	m_GroupBoSymbolsList.swap(tmp_GroupSymbolsList);
	
	ShowGroupSymbolsList(0);
	
	return 0;
}







// valuetype : 0 == bo, other ==> fx
int  CGroupSpread::GetValueFromGroupid(int valuetype,unsigned int groupid, char *requestid, const char *symbol,int &groupspread)
{
	map<unsigned int ,map<string,int>>::iterator itr;

	if(0 == valuetype)
	{
		itr = m_GroupBoSymbolsList.find(groupid);
		if (itr == m_GroupBoSymbolsList.end())
		{
			XERROR("GetValueFromGroupid find groupid don't the bo group,  groupid:%d,bo symbol:%s,requestid:%s\n",
					groupid , symbol ,requestid);
			return -1;
		}
	}
	else
	{
		itr = m_GroupSymbolsList.find(groupid);
		if (itr == m_GroupSymbolsList.end())
		{
			XERROR("GetValueFromGroupid find groupid don't the group,  groupid:%d,symbol:%s,requestid:%s\n",
					groupid , symbol ,requestid);
			return -1;
		}
	}


	map<string,int> &data = itr->second;
	map<string,int>::iterator itr2;
	for (map<string,int>::iterator itr2=data.begin(); itr2!=data.end(); ++itr2)
	{
		string symbols = itr2->first;

		size_t found = symbols.find(symbol);
  		if (found!=std::string::npos)
  		{
  			groupspread = itr2->second; 
			//XINFO("GetValueFromGroupid : groupid:%d,symbol:%s,requestid:%s, value:%d\n",groupid , symbol ,requestid, groupspread);
			return 0;
		}
		
	}

	XERROR("GetValueFromGroupid find don't the symbol in group,  groupid:%d,symbol:%s,requestid:%s\n",
				groupid , symbol ,requestid);

	return -1;




}


// valuetype : 0 == bo, other ==> fx
int	CGroupSpread::GetSymbolsFromGroupid(int valuetype,unsigned int groupid, char *requestid,char *symbollist, int memlen , int &datalen) 
{
	
	map<unsigned int ,map<string,int>>::iterator itr;

	if(0 == valuetype)
	{
		itr = m_GroupBoSymbolsList.find(groupid);
		if (itr == m_GroupBoSymbolsList.end())
		{
			XERROR("GetValueFromGroupid find groupid don't the bo group,  groupid:%d,requestid:%s\n",
					groupid , requestid);

			datalen = 0;
			//strncpy(symbollist,"-1",datalen);
			return -1;
		}
	}
	else
	{
		itr = m_GroupSymbolsList.find(groupid);
		if (itr == m_GroupSymbolsList.end())
		{
			XERROR("GetValueFromGroupid find groupid don't the group,  groupid:%d,requestid:%s\n",
					groupid , requestid);
			datalen = 0;
			//strncpy(symbollist,"-1",datalen);
			return -1;
		}
	}






	int first = 0;
	map<string,int> &data = itr->second;
	map<string,int>::iterator itr2;

	string symbols;
	
	for (map<string,int>::iterator itr2=data.begin(); itr2!=data.end(); ++itr2)
	{
		string symbol = itr2->first;

		
		if(0 == first)
		{
			symbols += symbol  ;
			first = 1;
			
			continue;
		}
		

		symbols += ";" ;
		symbols += symbol  ;
		
		
	}

	datalen = symbols.length();
	XINFO("GetSymbolsFromGroupid , datalen:%d,symbol list:%s\n",datalen,symbols.c_str());
	
	if(datalen >memlen )
	{
		XERROR("logon data GetSymbolsFromGroupid db'symbol_list length is greater than %d , groupid:%d,requestid:%s\n",
					memlen,groupid,requestid);

		datalen = 0 ;
		//datalen = 2;
		//strncpy(symbollist,"-1",datalen);
		XINFO("GetSymbolsFromGroupid , datalen:%d, symbol list:%s\n",datalen,symbols.c_str());
		return -1;
	}

	
	strncpy(symbollist,symbols.c_str(),datalen);


	return 0;


}







// valuetype : 0 == bo, other ==> fx
void 	CGroupSpread::ShowGroupSymbolsList(int valuetype)
{
	map<unsigned int ,map<string,int>>::iterator itr;
	
	map<unsigned int ,map<string,int>> *pList = NULL;

	
	if(0 == valuetype)
	{
		pList = &m_GroupBoSymbolsList;
	}
	else
	{
		pList = &m_GroupSymbolsList;
	}

	for(itr=pList->begin();itr!=pList->end();itr++)
	{
		int first = 1;
		map<string,int> &data = itr->second;
		map<string,int>::iterator itr2;
		string symbols ;
		for (map<string,int>::iterator itr2=data.begin(); itr2!=data.end(); ++itr2)
		{
			XINFO("Show Group:%d, symbols:%s, Spread:%d",itr->first, (itr2->first).c_str(), itr2->second );
		}
	}
}




int CGroupSpread::CalAskBidSpread(int &ask, int &bid, int groupsread)
{
	//groupsread *= 10; // spread 从小数后第5位开始算,所以要 *10;
	int a = groupsread/2 ;
	int b = groupsread - a;

	int value = (ask+bid)/2 ;
	int value2 = (ask+bid) - value ;
	ask = value + a ;
	bid = value2 - b ;
	
	return 0;
}



/*
caltype:
1. ask=bid, bid=bid)
2. ask=ask, bid=ask)
3. ask=(ask+bid)/2, bid=(ask+bid)/2
*/
int CGroupSpread::CalBo_AskBid(int caltype,int &ask, int &bid)
{

	switch(caltype)
	{
		case 2:
			bid = ask;
			break;
		case 3:
			ask = (ask + bid)/2 ;
			bid = ask;
			break;
		default:
			// case 1
			ask = bid;
	}

	return 0;
}



