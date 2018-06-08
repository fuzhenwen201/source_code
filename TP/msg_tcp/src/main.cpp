
/*******************************************************************************************

*Department:	TP

*Decrible:  	msg 实时行情 推送推送服务器  
				
*Auhor:			Savin

*Createdtime:	2017-05-08

*ModifyTime:	

savin 2017-05-26:   	增加Longo模块
savin 2017-05-31:		增加历史行情查询模块

********************************************************************************************/

#include "Public.h"
#include "MemPoolMgr.h"
#include "LRModule.h"
#include "WokerModule.h"
#include "LogonModule.h"
#include "HistDataModule.h"
#include "Group_Spread.h"
#include "ChangeNotice.h"


int main(int argc,char *argv[])
{	

	if(-1 == ReadConfig("./conf/msg_rttcp_server.conf",stConfinfo)) return -1;
	InitLogSys();

	
	if(-1 == ChangeNoticeInit() )return -1;
	

	CMemPoolMgr *pPoolMgr = new CMemPoolMgr();
	if(NULL == pPoolMgr)
	{
		XERROR("new CMemPoolMgr fail.\n");
		return -1;
	}
	if (-1 == pPoolMgr->Init() )
	{
		XERROR("CMemPoolMgr create mem pool fail.\n");
		return -1;
	}
	

	printf("LogonStart ....\n");
	CLogonProcee logon;
	logon.LogonStart(); 

		
		
	printf("HistDataStart ....\n");
	CHistDataProcess histData;
	histData.HistDataStart();
	
	CWorkerProcee::WorkerBatchStart();
		
	
	if ( -1 == StartListenModule(stConfinfo.listen_port) )
	{
		XERROR("StartListenModule  fail.\n");
		return -1;
	}

	XINFO("SDGDistribute begin work ....\n");


	
	return 0;

}



