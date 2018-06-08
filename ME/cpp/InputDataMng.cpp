#include "InputDataMng.h"


extern snetwork_xservice_tnode::TNode* g_pTNode;

CInputDataMng::CInputDataMng(std::string strHost, int iPort, std::string strvhost, std::string strExchGrp, std::string strUsr, std::string strPassword)
{
	m_pSerStatus = NULL;
	m_bIsRunning = false;
//	m_pConsumer = NULL;
	m_iProcRuleCnt = 0;
	m_strProcRule = "";
	m_strIP = strHost;
	m_iPort = iPort;
	m_strvhost = strvhost;
	m_strExchGrp = strExchGrp;
	m_strUsr = strPassword;
	m_pTnode = NULL;
	m_mapTnodeConsumer.clear();
	m_iRunStatus = 0;
	m_bIsCreateTnodeBySelf = true;

}


CInputDataMng::~CInputDataMng()
{
	if (m_pTnode)
	{
		m_pTnode->StopConsumer();
		if (m_bIsCreateTnodeBySelf)
		   delete m_pTnode;
	}
	
	for (auto iter = m_mapTnodeConsumer.begin(); iter != m_mapTnodeConsumer.end(); iter++)
	{
		delete iter->second;
	}
	m_mapTnodeConsumer.clear();
	
}
void CInputDataMng::StartInitial(bool bIsCreateTnodeBySelf)
{
	if (m_pTnode)
	{
		LOG_ERROR << "pTonde is not null when is ready to initial";
		return;
	}
	m_bIsCreateTnodeBySelf = bIsCreateTnodeBySelf;
	if (m_bIsCreateTnodeBySelf)
	{
		STTnodeConfig tnodeconfig;
		tnodeconfig.mq_vhost = m_strvhost;
		tnodeconfig.mq_exchange_group = m_strExchGrp;
		tnodeconfig.mq_host = m_strIP;
		tnodeconfig.mq_port = m_iPort;
		tnodeconfig.mq_user = m_strUsr;
		tnodeconfig.mq_passwd = m_strPassword;
		m_pTnode = new snetwork_xservice_tnode::TNode(tnodeconfig);
		if (-1 == m_pTnode->Init())
		{
			LOG_FATAL << "tnode(Inputdata) init fail , pls check ! \n";
			return ;
		}
	}
	else
		m_pTnode = g_pTNode;


}

void CInputDataMng::DistributeInput()
{
	if (!m_pTnode)
	{
		LOG_ERROR << "pNode is null when is ready to DistributeInput.";
		return;
	}
//  	if (m_mapTnodeConsumer.size() > 0)
//  	{
//  		LOG_ERROR << "pConusmer is not null when is ready to distribute input resources.";
//  	}
	if (m_strProcRule.empty())
	{
		LOG_ERROR << "Proc rule can not be empty.";
		return;
	}

	if (m_strProcRule.size() > 800)
	{
		LOG_ERROR << "Proc rule length is too Large. length = " << m_strProcRule.length();
		return;
	}
	//目前一期暂不支持对策略的更改
	if (m_mapTnodeConsumer.size() == 0)
	{
		int iLen = m_strProcRule.size() + 2;
		char* pRuleInfo = new char[iLen];
		memset(pRuleInfo, 0, iLen);
		memcpy(pRuleInfo, m_strProcRule.c_str(), m_strProcRule.size());
		std::vector<UINT> vtUsrID;
		vtUsrID.clear();
		std::string strTmp;
		for (char* pSigUsrRule = strsep(&pRuleInfo, ","); pSigUsrRule != NULL; pSigUsrRule = strsep(&pRuleInfo, ","))
		{
			strTmp.clear();
			strTmp = pSigUsrRule;
			int iPos = strTmp.find("(");
			if (iPos == -1)
			{
				LOG_ERROR << "Parser ME proc rule error. error_string = " << strTmp;
				return;
			}
			UINT uiUsrID = atoi(strTmp.substr(0, iPos).c_str());
			vtUsrID.push_back(uiUsrID);
		}
		m_mapTnodeConsumer.clear();
		char chKeyInfo[100] = { 0 };
		for (int i = 0; i < vtUsrID.size(); i++)
		{
			memset(chKeyInfo, 0, sizeof(chKeyInfo));
			sprintf(chKeyInfo, "OrderInfo.Sub.FrTable.%d", vtUsrID[i]);
			CMyTNodeConsumer* pConsumer = new CMyTNodeConsumer(chKeyInfo, "MatchingEngine");
			pConsumer->RegisterDataBuffer(m_pRegistInputBuf);
			m_pTnode->AddTNodeConsumer(pConsumer);
			m_mapTnodeConsumer[vtUsrID[i]] = pConsumer;
		}
		if (pRuleInfo)
		{
			delete[]pRuleInfo;
		}

	}
	
	

}

void CInputDataMng::StartRunInputData()
{
	if (!m_pTnode)
	{
		LOG_ERROR << "pNode is null when is ready to StartRunInputData.";
		return;
	}
	m_pTnode->RunConsumer();
}


void CInputDataMng::StopInputData()
{
	if (!m_pTnode)
	{
		LOG_ERROR << "pNode is null when is ready to StopInputData.";
		return;
	}
	m_pTnode->StopConsumer();
}

void CInputDataMng::ProcInputData(long lParam)
{
	m_bIsRunning = true;
	assert(m_pSerStatus != NULL);
    //在这里暂时只用一个输入线程

	while (m_bIsRunning)
	{
		if (m_iRunStatus == m_pSerStatus->GetRunStatus())
		{
			usleep(10000);
			continue;
		}
		m_iRunStatus = m_pSerStatus->GetRunStatus();
		if (0 == m_pSerStatus->GetRunStatus())
			StopInputData();
		else
		{
			if (m_iProcRuleCnt != m_pSerStatus->GetProcRuleCnt())
			{
				DistributeInput();
			}
			StartRunInputData();
		}
	}
}
