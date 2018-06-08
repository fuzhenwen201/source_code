#pragma once
#include <string>
#include "./hiredis/hiredis.h"
class CServerStatusMng
{
public:
	CServerStatusMng(std::string strIP, int iPort);
	~CServerStatusMng();

	bool Initialize();
	void ProcServerStatus(int lParam, int rParam);
	int GetRunStatus() const { return m_iRunStatus; }
	std::string GetMEServerProcInfo() const { return m_strProcInfo; }
	int GetProcRuleCnt() const { return m_iProcRuleCnt; }
private:
	std::string m_strIP;
	int         m_iPort;
	redisContext* m_pReidsContent;
	int m_iRunStatus;
	std::string m_strHostName;
	std::string m_strProcInfo;
	int m_iProcRuleCnt; //用来计数处理规则上改变的次数
};

