#pragma once
#include <muduo/net/Buffer.h>
#include "XRingBuffer.h"
#include <vector>
#include <string>

typedef muduo::net::Buffer MuduoBuff;
#define MAX_READ_FILE_BUF_LEN 4*1024*1024

class CReadInputFile
{
public:
	CReadInputFile(std::string strDirPath);
	~CReadInputFile();

	static void* ReadDataFromFile(void* pParam);
	bool BrowseDir(std::vector<std::string> &vtFileName);
	void RegisterBuf(XRingBuffer* pBuf) { m_pRingBuf = pBuf; }
	bool ParserFileToBuffer(std::string strFileName, XRingBuffer* pBuffer, bool bIsDelFile = true);
	bool StartWork();
	void SetRunCpu(int iCpuID) { m_iSetCpuID = iCpuID; }
	void ExitWork() {	m_bIsRunning = false; pthread_join(m_threadID, NULL);}
protected:
//	MuduoBuff* m_pBuffer;
	XRingBuffer* m_pRingBuf;
	std::string m_strPathDir;
	char      m_chContent[MAX_READ_FILE_BUF_LEN];
	bool      m_bIsRunning;
	int m_iSetCpuID;
	pthread_t m_threadID;
	std::vector<std::string> m_vtFile;
};



