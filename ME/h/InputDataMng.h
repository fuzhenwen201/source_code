#pragma once
#include "ServerStatusMng.h"
#include <assert.h>
#include "XRingBuffer.h"
#include "MyTNodeConsumer.h"
#include <map>
class CInputDataMng
{
public:
	CInputDataMng(std::string strHost, int iPort, std::string strvhost, std::string strExchGrp, std::string strUsr, std::string strPassword);
	~CInputDataMng();
	void RegisterSeverStatus(const CServerStatusMng* pSerStatus) { assert(pSerStatus != NULL); m_pSerStatus = pSerStatus; }
	void ProcInputData(long lParam);
	void EixtThread() { m_bIsRunning = false; }
	void RegisterRingBuf(XRingBuffer* pBuf) { assert(pBuf != NULL); m_pRegistInputBuf = pBuf; }
	void DistributeInput();
	void StopInputData();
	void StartInitial(bool bIsCreateTnodeBySelf = true);
	void StartRunInputData();


private:
	const CServerStatusMng* m_pSerStatus;
	bool m_bIsRunning;
	XRingBuffer* m_pRegistInputBuf;
	//CMyTNodeConsumer* m_pConsumer;
	int m_iProcRuleCnt;
	std::string m_strProcRule;

	//MQ information
	std::string m_strIP;
	int m_iPort;
	std::string m_strvhost;
	std::string m_strExchGrp; 
	std::string m_strUsr;
	std::string m_strPassword;
	snetwork_xservice_tnode::TNode* m_pTnode;
//	std::vector<CMyTNodeConsumer*> m_vtpTnodeConsumer;
	std::map<UINT, CMyTNodeConsumer*> m_mapTnodeConsumer;
	int  m_iRunStatus;
	bool m_bIsCreateTnodeBySelf;
};

