#include "MyProtobuf.h"
#include "ME.Interface.pb.h"

CMyProtobuf* CMyProtobuf::m_pMyProtobuf = NULL;

CMyProtobuf::CMyProtobuf()
{
}


CMyProtobuf::~CMyProtobuf()
{
}

CMyProtobuf* CMyProtobuf::GetInstance()
{
	if (NULL == m_pMyProtobuf)
	{
		m_pMyProtobuf = new CMyProtobuf;
	}
	return m_pMyProtobuf;
}

bool CMyProtobuf::ParserSubOrderToInnerData(char *pMsg, int iMsgLenth, STSubmitOrderInfo& stSubOrder)
{
	::MEProInterface::IntervalMsgHeader stMsgHeader;
	if (!stMsgHeader.ParseFromArray(pMsg, iMsgLenth))
	{
		LOG_ERROR << "Parse array to interface msg failed";
		return false;
	}
	if (stMsgHeader.has_stsubmitorder())
	{
		::MEOrderAPI::ST_SUBMIT_ORDER stProtoSubOrder = stMsgHeader.stsubmitorder();
		memcpy(stSubOrder.chUsrOrderID, stProtoSubOrder.strorderid().c_str(), (stProtoSubOrder.strorderid().size() > MAX_USR_ORDERID_LEN) ? MAX_USR_ORDERID_LEN : stProtoSubOrder.strorderid().size());
		stSubOrder.lNumber = stProtoSubOrder.lnumber();
		stSubOrder.lPrice = stProtoSubOrder.lprice();
		stSubOrder.lSubmitOrderID = stProtoSubOrder.ucrecordid();
		stSubOrder.lSubmitTime = stProtoSubOrder.lsubmittime();
		stSubOrder.ucIsCancelOrder = 0;
		stSubOrder.ucOrderType = stProtoSubOrder.ucordertype();
		stSubOrder.ucTransStatus = 0;
		stSubOrder.ucTransType = stProtoSubOrder.uctranstype();
		stSubOrder.uiExchObjID = stProtoSubOrder.uiexchobjid();
		stSubOrder.uiOrderIndex = stProtoSubOrder.uiorderindex();
		stSubOrder.uiPlatformID = stProtoSubOrder.uiuserid();
	}
	else if (stMsgHeader.has_stcancelorder())
	{
		::MEOrderAPI::ST_SUBMIT_CAMCEL_ORDER stCancelOrder = stMsgHeader.stcancelorder();
		memcpy(stSubOrder.chUsrOrderID, stCancelOrder.strorderid().c_str(), (stCancelOrder.strorderid().size() > MAX_USR_ORDERID_LEN) ? MAX_USR_ORDERID_LEN : stCancelOrder.strorderid().size());
		stSubOrder.lPrice = stCancelOrder.lprice();
		stSubOrder.lSubmitOrderID = stCancelOrder.lorderidfrme();
		stSubOrder.ucIsCancelOrder = 1;
		stSubOrder.ucTransType = stCancelOrder.uctranstype();
		stSubOrder.uiExchObjID = stCancelOrder.uiexchobjid();
		stSubOrder.uiOrderIndex = stCancelOrder.uiorderindex();
		stSubOrder.uiPlatformID = stCancelOrder.uiuserid();
	}
	else
	{
		LOG_ERROR << "can not found submit order or cancel order info in IntervalMsgHeader";
		return false;
	}
	return true;

}

bool CMyProtobuf::SearlizeInnerDealInfoToString(const STExechOrderinfo& stInnerOrderDeal, int iFlowNumber, std::string &strOutput)
{
	strOutput.clear();
	::MESubcribeAPI::ST_TRANS_ORDER_INFO* pstOrderDealInfo = new ::MESubcribeAPI::ST_TRANS_ORDER_INFO;
	pstOrderDealInfo->set_straskorderid(stInnerOrderDeal.chAskUsrOrderID);
	pstOrderDealInfo->set_strbidorderid(stInnerOrderDeal.chBidUsrOrderID);
	pstOrderDealInfo->set_uibiduserid(stInnerOrderDeal.uiBidUsrID);
	pstOrderDealInfo->set_uiaskuserid(stInnerOrderDeal.uiAskUsrID);
	pstOrderDealInfo->set_fprice(stInnerOrderDeal.lPrice);
	pstOrderDealInfo->set_fnumber(stInnerOrderDeal.lNumber);
	pstOrderDealInfo->set_ucexchtype(stInnerOrderDeal.ucExchType);
	pstOrderDealInfo->set_ucorderstatus(2);
	pstOrderDealInfo->set_ucrecordid(stInnerOrderDeal.lExchID);
	pstOrderDealInfo->set_uiexchobjid(stInnerOrderDeal.uiSymbolID);
	pstOrderDealInfo->set_mebidorderid(stInnerOrderDeal.lBidOrderID);
	pstOrderDealInfo->set_measkorderid(stInnerOrderDeal.lAskOrderID);

	::MEProInterface::IntervalMsgHeader stProHeader;
	stProHeader.set_iflownumber(iFlowNumber);
	stProHeader.set_icmd(EN_INTERVAL_MARKET_TRANS_DATA);
	stProHeader.set_imsgtype(2);
	stProHeader.set_allocated_sttransorder(pstOrderDealInfo);
	if (!stProHeader.SerializeToString(&strOutput))
	{
		LOG_ERROR << "Serialize deal info to string failed";
		return false;
	}
	return true;
}