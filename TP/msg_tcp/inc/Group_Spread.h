
/*******************************************************************************************

*Department:	TP

*Decrible:  	货币分组, spread 等新增功能

				备注,请先安装sqlite的开发库: 
				sudo apt-get install libsqlite3-dev
				
			
*Auhor:			Savin Chen

*Createdtime:	2017-07-26

*ModifyTime:	2017-09-18 调整spread模块，新增security功能


********************************************************************************************/




#ifndef GROUP_SPREAD_HEAD_H
#define GROUP_SPREAD_HEAD_H

#include "Public.h"



typedef struct __STGroupSymbolsData
{
	list<string> SymbolsList;
	int 		groupspread;
	string      symbolstring;

}STGroupSymbolsData;



class CGroupSpread
{
public:
	
	CGroupSpread(){ m_nChangePreTime_grpsecurity = 0 ; m_nChangePreTime_bosymbols = 0 ;}

	int  	ReadGroupSymbolsList();
	int		ReadGroupBoSymbolsList();

	// valuetype : 0 == bo, other ==> fx
	int  	GetValueFromGroupid(int valuetype,unsigned int groupid, char *requestid, const char *symbol,int &groupspread);
	int		GetSymbolsFromGroupid(int valuetype,unsigned int groupid, char *requestid,char *symbollist,int memlen ,int &datalen) ;

	
	void 	ShowGroupSymbolsList(int valuetype);

	int 	CalAskBidSpread(int &ask, int &bid, int groupsread);
	int 	CalBo_AskBid(int caltype,int &ask, int &bid);
	
	map<unsigned int ,map<string,int>> 		m_GroupSymbolsList;
	map<unsigned int ,map<string,int>> 		m_GroupBoSymbolsList;
	
	time_t 									m_nChangePreTime_grpsecurity;	
	static time_t 							m_nChangeTime_grpsecurity;

	time_t 									m_nChangePreTime_bosymbols;	
	static time_t 							m_nChangeTime_bosymbols;
	
};



#endif //GROUP_SPREAD_HEAD_H


