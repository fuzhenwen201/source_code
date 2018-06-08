#include  "../include/cmsghandle.h"

CMsghandle::CMsghandle():m_Cbusy(this)
//CMsghandle::CMsghandle()
{
}

CMsghandle::~CMsghandle()
{
}


int CMsghandle::do_work(std::string strApi, char* src, char* dest, int& nRoute, std::string strToken, CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig)
{
	m_Cbusy.Do_work(strApi, src, dest, nRoute, strToken, pRds,szSrcId,CrmConfig);
	return 0;
}
