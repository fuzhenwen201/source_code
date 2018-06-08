#ifndef _CProcessingData_H_
#define _CProcessingData_H_

#include <map>
#include <string>
#include "const.h"
#include "log.h"
#include "common.h"
#include "session.h"
using namespace std;

struct UserInfo
{
	char key[30];
	int m_LastTime;
	int m_crmTime;
	char m_CrmPwd[15];
	char m_Crmtoken[CRM_TOKEN_LEN];
	char m_Apptoken[APP_TOKEN_LEN];
	int m_session;
};

class ProcessingData
{
	public:
		static ProcessingData* GetInstance();
		int AnalysisData(char* srcData,char* destData);
	protected:
		ProcessingData();
		~ProcessingData();
		void FindCrmCount(char* srcData,char* destData);
		//int PackData(void* srcData,char* destData);
		//int UnpackData(char*srcData, void* destData);
		//int InsertCrmtoken(char* pCrmtoken);
		//int RemoveCrmtoken(char* pCrmtoken);
		//int UpdateCrmtoken(char* pCrmtoken);
	private:
		static ProcessingData* m_ProcessingData;
		std::map<string,UserInfo> m_UserMap;
};



#endif 