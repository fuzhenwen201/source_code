#include "ServerStatusMng.h"
#include "./muduo/base/Logging.h"
#include <unistd.h>
#include <errno.h>

CServerStatusMng::CServerStatusMng(std::string strIP, int iPort)
{
	m_strIP = strIP;
	m_iPort = iPort;
	m_pReidsContent = NULL;
	m_iRunStatus = 1;
	m_strHostName.clear();
	m_strProcInfo.clear();
	m_iProcRuleCnt = 0;

}


CServerStatusMng::~CServerStatusMng()
{
	m_pReidsContent = NULL;
}
//执行初始化操作
bool CServerStatusMng::Initialize()
{
	char chHostName[200] = { 0 };
	if (gethostname(chHostName, sizeof(chHostName)))
	{
		LOG_ERROR << "gethostname failed. errorno =" << errno;
		return false;
	}
	m_strHostName = chHostName;
	struct timeval stTimeout = { 3, 0 }; //2s的超时时间
	m_pReidsContent = (redisContext*)redisConnectWithTimeout(m_strIP.c_str(), m_iPort, stTimeout);
	if ((NULL == m_pReidsContent) || (m_pReidsContent->err))
	{
		if (m_pReidsContent)
		{
			LOG_ERROR << "connect error:" << m_pReidsContent->errstr ;
		}
		else
		{
			LOG_ERROR << "connect error: can't allocate redis context." ;
		}
		return false;
	}
	return true;

}


void CServerStatusMng::ProcServerStatus(int lParam, int rParam)
{
	if (NULL == m_pReidsContent)
	{
		LOG_ERROR << "please initial connect to redis firstly";
		return;
	}
	if (0 == m_iRunStatus )
	{
		LOG_DEBUG << "Server status is stop";
		return;
	}
	char chKey[200] = { 0 };
	sprintf(chKey, "_ME_%s", m_strHostName.c_str());
	std::string strKey(chKey);
	//首先获取
	redisReply* pReply = (redisReply*)redisCommand(m_pReidsContent, "HGET _ME_%s RunStatus", m_strHostName.c_str());
	if (pReply == NULL || (pReply->str, "nil") == 0)
	{
		LOG_ERROR << "can not get redis hash field. key is _ME_" << m_strHostName << " and filedName is RunStatus";
		if (pReply)
			freeReplyObject(pReply);
		return;
	}
	m_iRunStatus = atoi(pReply->str);
	freeReplyObject(pReply);
	if (0 == m_iRunStatus)
	{
		LOG_INFO << "read redis status, this me_server should stop severice";
		return;
	}
	//获取该ME下的处理信息
	 pReply = (redisReply*)redisCommand(m_pReidsContent, "HGET _ME_%s ME_Server", m_strHostName.c_str());
	if (pReply != NULL)
	{
		if (strcmp(pReply->str, "nil") == 0)
		{
			LOG_ERROR << "can not get redis hash field. key is _ME_" << m_strHostName << " and filedName is ME_Server";
			return;
		}
		std::string strProc(pReply->str);
		if (0 != m_strProcInfo.compare(strProc))
		{
			m_iProcRuleCnt++;
			m_strProcInfo = strProc;
		}
		freeReplyObject(pReply);
	}
	else
	{
		LOG_ERROR << "HGET _ME_SERVER Proc Rule failed.";
	}

	//上报时间
    //如果状态仍然允许，则进行状态的上报
	time_t now = time(0);// 基于当前系统的当前日期/时间  
	tm *ltm = localtime(&now);
	char chTime[50] = { 0 };
	sprintf(chTime, "%04d-%02d-%02d %02d:%02d:%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
	pReply = (redisReply*)redisCommand(m_pReidsContent, "HSET _ME_%s LastRunTime %s ", m_strHostName.c_str(), chTime);
	if (pReply != NULL)
		freeReplyObject(pReply);
	else
	{
		LOG_ERROR << "HSET _ME_" << m_strHostName << " LastRunTime failed";
	}
	
}