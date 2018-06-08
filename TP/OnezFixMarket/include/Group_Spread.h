
/*******************************************************************************************

*Department:	TP

*Decrible:  	货币分组, spread 等新增功能

				备注,请先安装sqlite的开发库: 
				sudo apt-get install libsqlite3-dev
				
			
*Auhor:			Savin Chen

*Createdtime:	2017-07-26

*ModifyTime:	


********************************************************************************************/




#ifndef GROUP_SPREAD_HEAD_H
#define GROUP_SPREAD_HEAD_H

#include "Public.h"





class CGroupSpread
{
public:

	CGroupSpread();

	int			CheckDBFile();
	int 		ReadSymbolSpreadList();

	int			GetSymbolSpread(const char *symbol,int &symbolspread);

	int 		CalAskBidSpread(int &ask, int &bid, int totalspread);

	void 		ShowSymbolSpreadList();
	
	
	
	 map<string, int> 				m_SymbolSpreadList;
	 
	
	 int m_nReadDBTime;
	 
	time_t 							m_nChangePreTime;	
	static time_t 					m_nChangeTime;
	
	
};



#endif //GROUP_SPREAD_HEAD_H


