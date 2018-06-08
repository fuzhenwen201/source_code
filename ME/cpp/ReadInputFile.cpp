#include "ReadInputFile.h"
#include <sys/stat.h>
#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <muduo/base/Logging.h>
#include <algorithm>



CReadInputFile::CReadInputFile(std::string strDirPath)
{
	m_strPathDir = strDirPath;
	m_bIsRunning = true;
	m_pRingBuf = NULL;
	m_iSetCpuID = 255;
	m_threadID = 0;
	m_vtFile.clear();
}


CReadInputFile::~CReadInputFile()
{
	if (m_bIsRunning)
		ExitWork();
}


bool CReadInputFile::StartWork()
{
	m_bIsRunning = true;
	if (pthread_create(&m_threadID, NULL, ReadDataFromFile, this) != 0)
	{
		LOG_FATAL << "create CReadInputFile thread  failed";
		return false;
	}
	return true;
}


void* CReadInputFile::ReadDataFromFile(void* pParam)
{
	CReadInputFile* pReadFile = (CReadInputFile*)pParam;
	if (pReadFile->m_iSetCpuID < 255)
	{
		cpu_set_t cpuSet;
		CPU_ZERO(&cpuSet);
		CPU_SET(pReadFile->m_iSetCpuID, &cpuSet);
		int iRet = pthread_setaffinity_np(pthread_self(), sizeof(cpuSet), &cpuSet);
		if (iRet != 0)
		{
			LOG_ERROR << "set thread(CReadInputFile) affinity failed.error = " << iRet;
			return ((void*)0);
		}
	}

	
	while (pReadFile->m_bIsRunning)
	{
		if (!pReadFile->BrowseDir(pReadFile->m_vtFile))
		{
			return ((void*)0);
		}
		if (pReadFile->m_vtFile.empty())
		{
			usleep(3000);
			continue;
		}
		for (int i = 0; i < pReadFile->m_vtFile.size(); i++)
			pReadFile->ParserFileToBuffer(pReadFile->m_vtFile[i], pReadFile->m_pRingBuf);
		//for_each(vtFile.begin(), vtFile.end(), std::bind<bool>(&CReadInputFile::ParserFileToBuffer, _1 ,m_pRingBuf));
	}
	return ((void*)0);
}


bool CReadInputFile::ParserFileToBuffer(std::string strFileName, XRingBuffer* pBuffer,bool bIsDelFile)
{
	if (NULL == pBuffer)
	{
		LOG_FATAL << "ParserFileToBuffer input buffer can not be null";
		return false;
	}
	if (strFileName.empty())
	{
		LOG_ERROR << "ParserFileToBuffer input file name can not be empty";
		return false;
	}

	std::string strOpenFile = m_strPathDir + "//" + strFileName;
	struct stat filestat;
	stat(strOpenFile.c_str(), &filestat);
	if (filestat.st_size > MAX_READ_FILE_BUF_LEN)
	{
		LOG_ERROR << strOpenFile.c_str() << "'s file length is too big. file_size = " << filestat.st_size;
		return false;
	}
     //read file content to buffer
	FILE* fp = fopen(strOpenFile.c_str(), "r+");
	if (NULL == fp)
	{
		LOG_ERROR << "open file:" << strOpenFile.c_str() << "failed£ºerrorno = " << strerror(errno);
		return false;
	}
	memset(m_chContent, 0, sizeof(m_chContent));
	size_t iSize = fread(m_chContent, 1,MAX_READ_FILE_BUF_LEN, fp);

	//parser content to muduo buffer
	char *pTmp = m_chContent;
	for (char* pLineData = strsep(&pTmp,"\n"); pLineData != NULL; pLineData = strsep(&pTmp, "\n"))
	{
		STSubmitOrderInfo stSubmitOrder;
		int i = 0, iOrderLen = sizeof(stSubmitOrder);
		for (char* pSepData = strsep(&pLineData,","); pSepData != NULL; pSepData = strsep(&pLineData, ","),i++)
		{
			switch (i)
			{
			case 0:
				stSubmitOrder.lSubmitOrderID = atol(pSepData);
				break;
			case 1:
				stSubmitOrder.uiExchObjID = atoi(pSepData);
				break;
			case 2:
				stSubmitOrder.uiPlatformID = atoi(pSepData);
				break;
			case 3:
				stSubmitOrder.ucTransType = atoi(pSepData);
				break;
			case 4:
				stSubmitOrder.ucOrderType = atoi(pSepData);
				break;
			case 5:
				stSubmitOrder.lPrice = (long)(atof(pSepData)*FLOAT_PRICE_TO_LONG);
				break;
			case 6:
				stSubmitOrder.lNumber = (long)(atof(pSepData)*FLOAT_PRICE_TO_LONG);
				break;
			case 7:
				stSubmitOrder.uiOrderIndex = atoi(pSepData);
				break;
			case 8:
				stSubmitOrder.ucIsCancelOrder = atoi(pSepData);
				break;
			default:
				LOG_ERROR << "Unknow seprate index:" << i + 1;
				break;
			}
		}
		if (stSubmitOrder.uiExchObjID == 0)
		{
			continue;
		}
		stSubmitOrder.lSubmitTime = muduo::Timestamp::now().microSecondsSinceEpoch();
		while (1)
		{
			if (pBuffer->getFreeSize() >= iOrderLen)
			{
				if (!pBuffer->pushData(&stSubmitOrder, iOrderLen))
					LOG_ERROR << "Pust Submit Order to buffer failed. SubmitOrderID =" << stSubmitOrder.lSubmitOrderID;
				break;
			}
			else
				usleep(5);

		}

	}

	fclose(fp);
	if (bIsDelFile)
		remove(strOpenFile.c_str());

	
	return true;


}

bool CReadInputFile::BrowseDir(std::vector<std::string> &vtFileName)
{

	// check the parameter !  
	vtFileName.clear();
	if (m_strPathDir.empty())
	{
		std::cout << " dir_name is null ! " << std::endl;
		return false;
	}

	// check if dir_name is a valid dir  
	struct stat s;
	lstat(m_strPathDir.c_str(), &s);
	if (!S_ISDIR(s.st_mode))
	{
		std::cout << "dir_name is not a valid directory !" << std::endl;
		return false;
	}

	struct dirent * filename;    // return value for readdir()  
	DIR * dir= opendir(m_strPathDir.c_str());                  // return value for opendir()  
	if (NULL == dir)
	{
		std::cout << "Can not open dir " << m_strPathDir.c_str() <<" and error_no = "<< errno << std::endl;
		return false;
	}
//	std::cout << "Successfully opened the dir !" << std::endl;

	/* read all the files in the dir ~ */
	while ((filename = readdir(dir)) != NULL)
	{
		// get rid of "." and ".."  
		if (strcmp(filename->d_name, ".") == 0 ||
			strcmp(filename->d_name, "..") == 0)
			continue;
		std::cout << "find£º"<<filename->d_name << std::endl;
		vtFileName.push_back(filename->d_name);
	}
	closedir(dir);
	return true;

}