#pragma once

#include "StockExcDef.h"
class CMyProtobuf
{
public:
	CMyProtobuf();
	~CMyProtobuf();

	static CMyProtobuf* GetInstance();
	bool ParserSubOrderToInnerData(char *pMsg, int nMsgLe, STSubmitOrderInfo& stSubOrder);
	bool SearlizeInnerDealInfoToString(const STExechOrderinfo& stSubOrder, int iFlowNumber, std::string &strOutput);

private:
	static CMyProtobuf* m_pMyProtobuf;
};

