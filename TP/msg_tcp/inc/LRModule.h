
/*******************************************************************************************

*Department:	TP

*Decrible:  	监听,读 模块类
				1. 本模块基于 libevent，负责监听,读 事件
				
				
*Auhor:			Savin

*Createdtime:	2017-04-20

*ModifyTime:	


********************************************************************************************/


#ifndef BASELRMODULE_HEAD_H
#define BASELRMODULE_HEAD_H

#include "Public.h"
#include "BaseThread.h"

class CBaseLRModule:public CBaseThread
{
public:
	CBaseLRModule(int nPort,int isOutService);
	~CBaseLRModule(){}

	void 			Run();
	

private:
	
	int 			Init();
	static int 		ReadData(STDataNode *pDataNode);
	static void 	ListenAccept(int listensock,short event,void* arg);
	static void 	ReadCallback(int sock,short event,void* arg);
	

	int 			m_nPort;
	int 			m_isOutService;	

};


#endif //BASELRMODULE_HEAD_H

