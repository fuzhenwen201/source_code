
/*******************************************************************************************

*Department:	TP

*Decrible:  	sg distribute main 
				

*Auhor:			Savin

*Createdtime:	2017-04-20

*ModifyTime:	


********************************************************************************************/

#include "Public.h"
#include "MemPoolMgr.h"
#include "LRModule.h"
#include "WokerModule.h"

#include "InService.h"
#include "ChangeNotice.h"


int main(int argc,char *argv[])
{

	if(-1 == ReadConfig("./conf/sgdistributor.conf",stConfinfo))return -1;
	InitLogSys();
	
	XINFO("SDGDistribute begin ....\n");

	if(-1 == ChangeNoticeInit())return -1;


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


	CBaseLRModule outService(stConfinfo.nOutServicePort);
	outService.Start();
	if(1 == g_ExitFlag)
	{
		XERROR("create outservice L/R Module fail \n");
		return -1;
	}
	XINFO("Out Listen Service start ...\n");

	CWorkerProcee::WorkerBatchStart();

	CInserviceWork::GetInstance()->Run();
	


	XINFO("SDGDistribute begin work ....\n");
	
	CheckExit();

}



