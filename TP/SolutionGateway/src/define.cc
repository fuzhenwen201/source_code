/*************************************************************************
	> File Name: define.cc
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Thu 09 Feb 2017 12:20:40 PM CST
 ************************************************************************/
#include<string.h>
#include"define.h"
#include<iomanip>
#include<ctime>
#include<chrono>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/types.h>
#include<ifaddrs.h>
#include<netinet/in.h>
#include<arpa/inet.h>

//namespace snetwork_xservice_solutiongateway {

HeaderTag::HeaderTag() :
	head(0x8F),
	reserved(0),
	tag(0),
	length(0) {
}

char DecLookupTable[] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,2,3,4,5,6,7,8,9,	/*0123456789*/
	0,0,0,0,0,0,0, /*:;<=>?@*/
	10,11,12,13,14,15, /*ABCDEF*/
	0,0,0,0,0,0,0,0,0,0,0,0,0, /*GHIJKLMNOPQRS*/
	0,0,0,0,0,0,0,0,0,0,0,0,0, /*TUVWXYZ[/]^_`*/
	10,11,12,13,14,15,	/*abcdef*/
};

char HexLookupTable[513] = {
	"000102030405060708090A0B0C0D0E0F"
	"101112131415161718191A1B1C1D1E1F"
	"202122232425262728292A2B2C2D2E2F"
	"303132333435363738393A3B3C3D3E3F"
	"404142434445464748494A4B4C4D4E4F"
	"505152535455565758595A5B5C5D5E5F"
	"606162636465666768696A6B6C6D6E6F"
	"707172737475767778797A7B7C7D7E7F"
	"808182838485868788898A8B8C8D8E8F"
	"909192939495969798999A9B9C9D9E9F"
	"A0A1A2A3A4A5A6A7A8A9AAABACADAEAF"
	"B0B1B2B3B4B5B6B7B8B9BABBBCBDBEBF"
	"C0C1C2C3C4C5C6C7C8C9CACBCCCDCECF"
	"D0D1D2D3D4D5D6D7D8D9DADBDCDDDEDF"
	"E0E1E2E3E4E5E6E7E8E9EAEBECEDEEEF"
	"F0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF"
};

char hexLookupTable[513] = {
	"000102030405060708090a0b0c0d0e0f"
	"101112131415161718191a1b1c1d1e1f"
	"202122232425262728292a2b2c2d2e2f"
	"303132333435363738393a3b3c3d3e3f"
	"404142434445464748494a4b4c4d4e4f"
	"505152535455565758595a5b5c5d5e5f"
	"606162636465666768696a6b6c6d6e6f"
	"707172737475767778797a7b7c7d7e7f"
	"808182838485868788898a8b8c8d8e8f"
	"909192939495969798999a9b9c9d9e9f"
	"a0a1a2a3a4a5a6a7a8a9aaabacadaeaf"
	"b0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
	"c0c1c2c3c4c5c6c7c8c9cacbcccdcecf"
	"d0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
	"e0e1e2e3e4e5e6e7e8e9eaebecedeeef"
	"f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff"
};

const char* AccountErrMsg[8] = {
	"success",
	"margin not enough",
	"formula error(prevbalance+changed+profit!=Margin+FreeMargin)",
	"json error or some field lose",
	"margin is invalid, may is negative",
	"pull is not exists",
	"reserved errormsg",
	"unknow error"
};

const char* POSITION_DB_NAME = "test";
const char* POSITION_DELETE_PULL_FORMAT = "DELETE FROM position_management WHERE groupid=%d and clientid=%d";
const char* PM_SEND_ORDER_FORMAT ="INSERT INTO pm_send_order_log_%8d(reqid,sessionid,groupid,clientid,assetid,direction,expiryperiod,amount,sgid,transfer,update_time)VALUE('%s','%s',%d,%d,%d,%d,%d,%d,%d,%d,%ld);";
const char* SG_SEND_ORDER_FORMAT ="INSERT INTO sg_send_order_log_%8d(reqid,groupid,clientid,assetid,direction,expiryperiod,amount,sgid,transfer,update_time)VALUE('%s',%d,%d,%d,%d,%d,%d,%d,%d,%ld);";

const char* SELECT_VALID_FORMAT ="SELECT balance,prevbalance,changed,margin,freemargin,profit,`status` FROM position_management WHERE groupid=%d AND clientid=%d";

const char* PM_ORDER_ANS_FORMAT ="INSERT INTO pm_order_ans_log_%8d(reqid,sessionid,groupid,clientid,orderid,orderstatus,assetid,direction,amount,payout,opentime,openprice,closeprice,passive,sgid,teid,update_time)VALUE('%s','%s',%d,%d,%d,%d,%d,%d,%d,%ld,%ld,%d,%d,%d,%d,%d,%ld)";

const char* SG_ORDER_ANS_FORMAT ="INSERT INTO sg_order_ans_log_%8d(reqid,groupid,clientid,orderid,orderstatus,assetid,direction,amount,payout,opentime,openprice,closeprice,passive,sgid,teid,update_time)VALUE('%s',%d,%d,%d,%d,%d,%d,%d,%ld,%ld,%d,%d,%d,%d,%d,%ld)";

const char* POSITION_UPDATE_VALID_FORMAT = "UPDATE position_management SET margin=margin+%d, freemargin=freemargin-%d WHERE groupid=%d AND clientid=%d";

const char* POSITION_UPDATE_PULL_FORMAT = "UPDATE position_management SET balance=%ld,freemargin=%d,prevbalance=%ld,changed=%ld WHERE groupid=%d AND clientid=%d";

const char* PM_VALIDATE_ORDER_FORMAT ="INSERT INTO pm_validate_order_log_%8d(reqid,sessionid,groupid,clientid,accountid,boaccountid,assetid,direction,expiryperiod,amount,orderstatus,margin,freemargin,profit,sgid,teid,update_time)VALUE('%s','%s',%d,%d,%d,%d,%d,%d,%d,%d,%d,%ld,%ld,%d,%d,%d,%ld)";

const char* SG_VALIDATE_ORDER_FORMAT ="INSERT INTO sg_validate_order_log_%8d(reqid,groupid,clientid,accountid,boaccountid,assetid,direction,expiryperiod,amount,orderstatus,margin,freemargin,profit,sgid,teid,update_time)VALUE('%s',%d,%d,%d,%d,%d,%d,%d,%d,%d,%ld,%ld,%d,%d,%d,%ld)";

const char* SG_CLIENT_LOGIN_FORMAT = "INSERT INTO sg_client_login_%8d(reqid,groupid,clientid,ipaddress,password,status,update_time)VALUE('%s',%d,%d,'%s','%s',%d,%ld);";
const char* SG_CLIENT_LOGOUT_FORMAT = "INSERT INTO sg_client_logout_%8d(reqid,clientid,groupid,status,update_time)VALUE('%s',%d,%d,%d,%ld);";
/*win: 
 * 1. profit=profit+payout
 * 2. margin=margin-amount
 * 3. balance=balance+payout
 * 4. freemargin=freemargin+payout+amount
 * Open+Change=Margin+FreeMargin-profit
 * Balance=Open+change+profit=Margin+FreeMargin
 * */
const char* POSITION_UPDATE_VALID_WIN_FORMAT = "UPDATE position_management SET profit=profit+%ld,freemargin=freemargin+%ld+%d,margin=margin-%d,balance=balance+%ld WHERE groupid=%d AND clientid=%d";
//const char* POSITION_UPDATE_VALID_WIN_FORMAT = "UPDATE position_management SET profit=profit+%ld-%d,freemargin=freemargin+%ld,margin=margin-%d,balance=balance+%ld WHERE groupid=%d AND clientid=%d";

/*loss: 
 * 1. profit=profit-amount
 * 2. margin=margin-amount
 * 3. balance=balance-amount
 * Open+Change=Margin+FreeMargin-profit
 * Balance=Open+change+profit=Margin+FreeMargin
 * */
const char* POSITION_UPDATE_VALID_LOSS_FORMAT = "UPDATE position_management SET profit=profit-%d,margin=margin-%d,balance=balance-%d WHERE groupid=%d AND clientid=%d";

/*reject: 
 * 1. margin=margin-amount
 * 3. freemargin=freemargin+amount
 * Open+Change=Margin+FreeMargin-profit
 * Balance=Open+change+profit=Margin+FreeMargin
 * */
const char* POSITION_UPDATE_VALID_REJECT_FORMAT = "UPDATE position_management SET freemargin=freemargin+%d,margin=margin-%d WHERE groupid=%d AND clientid=%d";

const char* INSERT_POSITION_LOG_FORMAT ="INSERT INTO position_management_log_%8d(reqid,sessionid,groupid,clientid,accountid,balance,prevbalance,changed,margin,freemargin,profit,`type`,closebalance,`status`,`check`,update_time) VALUES('%s','%s',%d,%d,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%d,%ld,%d,%d,%ld)";

const char* INSERT_POSITION_INIT_FORMAT ="INSERT INTO position_management(groupid,clientid,accountid,balance,prevbalance,changed,margin,freemargin,profit,`type`,closebalance,`status`,`check`,update_time) VALUES(%d,%d,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%d,%ld,%d,%d,%ld)";

char* STRERROR(int n) {
	size_t size = 1024;
	char* str = (char*)malloc(size);
	if (str == nullptr) {
		return nullptr;
	}

	while (strerror_r(n, str, size) == nullptr &&
		   errno == ERANGE) {
		size <<= 1;
		str = (char*)realloc(str, size);
	}

	return str;
}

void HexToBinary(char* dst, char* src, size_t len) {
	if ((len & 1) || (src == nullptr) || (dst == nullptr)) {
		return;	/*odd len*/
	}

	char* pDst = dst;
	char* pSrc = src;

	for (size_t i=0; i<len; i +=2) {
		*pDst = DecLookupTable[*pSrc++]<<4;
		*pDst |= DecLookupTable[*pSrc++];
		++pDst;
	}
}

void GetIPAddress4(char* ipstr, const char* ifname) {
	struct ifaddrs* ifap;
	struct ifaddrs* ifa;
	struct sockaddr_in *sa;

	getifaddrs (&ifap);
	for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr->sa_family == AF_INET &&
			0 == strcmp(ifname, ifa->ifa_name)) {
			inet_ntop(AF_INET, &((struct sockaddr_in*)ifa->ifa_addr)->sin_addr, ipstr, INET_ADDRSTRLEN);
		}
	}

	freeifaddrs(ifap);
}

void DecToHex(char* dst, char* src, size_t len, bool lower) {
	if (dst==nullptr || src == nullptr || len == 0) {
		return;
	}

	char* pHex = nullptr;
	if (lower) {
		pHex = hexLookupTable;
	} else {
		pHex = HexLookupTable;
	}
	char* pDst = dst;
	char* pSrc = src;
	int pos = 0;

	for (size_t i=0; i<len; ++i) {
		pos = (*(unsigned char*)pSrc << 1); 
		*pDst++ = pHex[pos];
		*pDst = pHex[pos+1];
		++pDst;
		++pSrc;
	}

	strcpy(pDst, "123456789abc");
	strfry(pDst);
	dst[REQUEST_ID_LEN-1] = 0;
}

/* SendOrder begin ****/
SendOrder::SendOrder() :
	Direction(0),
	Transfer(0),
	GroupID(0),
	ClientID(0),
	AssetID(0),
	ExpiryPeriod(0),
	Amount(0),
	SGID(0),
	ReqID(nullptr),
	Json(nullptr) {
	ReqID = (char*) malloc(REQUEST_ID_LEN);
	memset(ReqID, 0, REQUEST_ID_LEN);
}

SendOrder::SendOrder(unsigned char direction, 
	const char* reqID,
	unsigned int groupID,
	unsigned int clientID,
	unsigned int assetID,
	unsigned int expiryPeriod,
	unsigned int amount,
	unsigned int sgID) :
	Direction(direction),
	Transfer(0),
	GroupID(groupID),
	ClientID(clientID),
	AssetID(assetID),
	ExpiryPeriod(expiryPeriod),
	Amount(amount),
	SGID(sgID),
	ReqID(nullptr),
	Json(nullptr) {
	ReqID = (char*) malloc(REQUEST_ID_LEN);
	memcpy(ReqID, reqID, REQUEST_ID_LEN);
}

SendOrder::~SendOrder() {
	if (ReqID != nullptr) {
		free(ReqID);
		ReqID = nullptr;
	}

	if (Json != nullptr) {
		free(Json);
		Json = nullptr;
	}
}

SendOrder::SendOrder(const SendOrder& r) {
	Direction = r.Direction;	
	Transfer = r.Transfer;
	GroupID = r.GroupID;	
	ClientID = r.ClientID;	
	AssetID = r.AssetID;	
	ExpiryPeriod = r.ExpiryPeriod;
	Amount = r.Amount;	
	SGID = r.SGID;		
	ReqID = (char*) malloc(REQUEST_ID_LEN);
	memcpy(ReqID, r.ReqID, REQUEST_ID_LEN);
	if (r.Json != nullptr) {
		size_t len = strlen(r.Json);
		Json = (char*) malloc (len + 1);
		strcpy(Json, r.Json);
		Json[len] = 0;
	} else {
		Json = nullptr;
	}
}

SendOrder& SendOrder::operator=(const SendOrder& r) {
	if (this == &r) {
		return *this;
	}

	Direction = r.Direction;	
	Transfer = r.Transfer;
	GroupID = r.GroupID;	
	ClientID = r.ClientID;	
	AssetID = r.AssetID;	
	ExpiryPeriod = r.ExpiryPeriod;
	Amount = r.Amount;	
	SGID = r.SGID;		
	memcpy(ReqID, r.ReqID, SOLUTIONGATEWAY_REQUEST_ID_LEN);

	if (r.Json != nullptr) {
		size_t len = strlen(r.Json);
		Json = (char*) malloc (len + 1);
		strcpy(Json, r.Json);
		Json[len] = 0;
	}

	return *this;
}

SendOrder::SendOrder(SendOrder&& r) {
	Direction = r.Direction;	
	Transfer = r.Transfer;
	GroupID = r.GroupID;	
	ClientID = r.ClientID;	
	AssetID = r.AssetID;	
	ExpiryPeriod = r.ExpiryPeriod;
	Amount = r.Amount;	
	SGID = r.SGID;		
	ReqID = r.ReqID;
	Json = r.Json;

	r.Direction = 0;	
	r.Transfer = 0;
	r.GroupID = 0;	
	r.ClientID = 0;	
	r.AssetID = 0;	
	r.ExpiryPeriod = 0;
	r.Amount = 0;	
	r.SGID = 0;		
	r.ReqID = nullptr;
	r.Json = nullptr;
}

SendOrder& SendOrder::operator=(SendOrder&& r) {
	if (this == &r) {
		return *this;
	}

	if (ReqID != nullptr) {
		free(ReqID);
		ReqID = nullptr;
	}

	if (Json != nullptr) {
		free(Json);
		Json = nullptr;
	}

	Direction = r.Direction;	
	Transfer = r.Transfer;
	GroupID = r.GroupID;	
	ClientID = r.ClientID;	
	AssetID = r.AssetID;	
	ExpiryPeriod = r.ExpiryPeriod;
	Amount = r.Amount;	
	SGID = r.SGID;		
	ReqID = r.ReqID;
	Json = r.Json;

	r.Direction = 0;	
	r.Transfer = 0;
	r.GroupID = 0;	
	r.ClientID = 0;	
	r.AssetID = 0;	
	r.ExpiryPeriod = 0;
	r.Amount = 0;	
	r.SGID = 0;		
	r.ReqID = nullptr;
	r.Json = nullptr;

	return *this;
}

const char* SendOrder::ToJson(void) {
	if (Json == nullptr) {
		asprintf(&Json, "{\"reqid\":\"%s\",\"groupid\":%d,\"clientid\":%d,\"assetid\":%d,\"direction\":%d,\"expiryperiod\":%d,\"amount\":%d,\"sessionid\":\"%s\",\"sgid\":%d}",
				 ReqID,
				 GroupID,
				 ClientID,
				 AssetID,
				 Direction,
				 ExpiryPeriod,
				 Amount,
				 ReqID,
				 SGID);
	}

	return Json;
}


/* SendOrder end ****/

/* OrderAns begin ***/
OrderAns::OrderAns() :
	OrderStatus(0),
	Direction(0),
	ReqID(nullptr),
	SessionID(nullptr),
	Json(nullptr),
	GroupID(0),
	ClientID(0),
	OrderID(0),
	AssetID(0),
	PayOut(0),
	Amount(0),
	OpenTime(0),
	OpenPrice(0),
	ClosePrice(0),
	SGID(0),
	TEID(0) {
	ReqID = (char*) malloc(REQUEST_ID_LEN);
	memset(ReqID, 0, REQUEST_ID_LEN);
	SessionID = (char*) malloc(SESSION_ID_LEN);
	memset(SessionID, 0, SESSION_ID_LEN);
}

OrderAns::OrderAns(unsigned char orderStatus, 
		unsigned char direction,
		const char* reqID,
		const char* sessionID,
		unsigned int groupID,
		unsigned int clientID,
		unsigned int orderID,
		unsigned int assetID,
		long payOut,
		int amount,
		unsigned int openTime,
		int openPrice,
		int closePrice,
		unsigned int sgID,
		unsigned int teID) :
	OrderStatus(orderStatus),
	Direction(direction),
	ReqID(nullptr),
	Json(nullptr),
	GroupID(groupID),
	ClientID(clientID),
	OrderID(orderID),
	AssetID(assetID),
	PayOut(payOut),
	Amount(amount),
	OpenTime(openTime),
	OpenPrice(openPrice),
	ClosePrice(closePrice),
	SGID(sgID),
	TEID(teID) {
	ReqID = (char*) malloc(REQUEST_ID_LEN);
	memcpy(ReqID, reqID, REQUEST_ID_LEN);
	SessionID = (char*) malloc(SESSION_ID_LEN);
	memcpy(SessionID, sessionID, SESSION_ID_LEN);
}

OrderAns::~OrderAns() {
	if (ReqID != nullptr) {
		free(ReqID);
		ReqID = nullptr;
	}

	if (SessionID != nullptr) {
		free(SessionID);
		SessionID = nullptr;
	}

	if (Json != nullptr) {
		free(Json);
		Json = nullptr;
	}
}

OrderAns::OrderAns(const OrderAns& r) {
	OrderStatus = r.OrderStatus;
	Direction = r.Direction;
	GroupID = r.GroupID;
	ClientID = r.ClientID;
	OrderID = r.OrderID;
	AssetID = r.AssetID;
	PayOut = r.PayOut;
	Amount = r.Amount;
	OpenTime = r.OpenTime;
	OpenPrice = r.OpenPrice;
	ClosePrice = r.ClosePrice;
	SGID = r.SGID;
	TEID = r.TEID;

	ReqID = (char*) malloc(REQUEST_ID_LEN);
	memcpy(ReqID, r.ReqID, REQUEST_ID_LEN);
	SessionID = (char*) malloc(SESSION_ID_LEN);
	memcpy(SessionID, r.SessionID, SESSION_ID_LEN);
	
	if (r.Json != nullptr) {
		size_t len = strlen(r.Json);
		Json = (char*) malloc (len+1);
		memcpy(Json, r.Json, len);
		Json[len] = 0;
	} else {
		Json = nullptr;
	}
}

OrderAns& OrderAns::operator=(const OrderAns& r) {
	if (this == &r) {
		return *this;
	}
	if (Json != nullptr) {
		free(Json);
		Json = nullptr;
	}

	OrderStatus = r.OrderStatus;
	Direction = r.Direction;
	GroupID = r.GroupID;
	ClientID = r.ClientID;
	OrderID = r.OrderID;
	AssetID = r.AssetID;
	PayOut = r.PayOut;
	Amount = r.Amount;
	OpenTime = r.OpenTime;
	OpenPrice = r.OpenPrice;
	ClosePrice = r.ClosePrice;
	SGID = r.SGID;
	TEID = r.TEID;
	memcpy(ReqID, r.ReqID, REQUEST_ID_LEN);
	memcpy(SessionID, r.SessionID, SESSION_ID_LEN);

	if (r.Json != nullptr) {
		size_t len = strlen(r.Json);
		Json = (char*) malloc (len+1);
		memcpy(Json, r.Json, len);
		Json[len] = 0;
	}

	return *this;
}

OrderAns::OrderAns(OrderAns&& r) {
	OrderStatus = r.OrderStatus;
	Direction = r.Direction;
	GroupID = r.GroupID;
	ClientID = r.ClientID;
	OrderID = r.OrderID;
	AssetID = r.AssetID;
	PayOut = r.PayOut;
	Amount = r.Amount;
	OpenTime = r.OpenTime;
	OpenPrice = r.OpenPrice;
	ClosePrice = r.ClosePrice;
	SGID = r.SGID;
	TEID = r.TEID;

	ReqID = r.ReqID;
	r.ReqID = nullptr;
	SessionID = r.SessionID;
	r.SessionID = nullptr;
	r.OrderStatus = 0;
	r.Direction = 0;
	r.GroupID = 0;
	r.ClientID = 0;
	r.OrderID = 0;
	r.AssetID = 0;
	r.PayOut = 0;
	r.Amount = 0;
	r.OpenTime = 0;
	r.OpenPrice = 0;
	r.ClosePrice = 0;
	r.SGID = 0;
	r.TEID = 0;
}

OrderAns& OrderAns::operator=(OrderAns&& r) {
	if (this == &r) {
		return *this;
	}

	if (ReqID != nullptr) {
		free(ReqID);
		ReqID = nullptr;
	}

	if (SessionID != nullptr) {
		free(SessionID);
		SessionID = nullptr;
	}

	if (Json != nullptr) {
		free(Json);
		Json = nullptr;
	}

	OrderStatus = r.OrderStatus;
	Direction = r.Direction;
	GroupID = r.GroupID;
	ClientID = r.ClientID;
	OrderID = r.OrderID;
	AssetID = r.AssetID;
	PayOut = r.PayOut;
	Amount = r.Amount;
	OpenTime = r.OpenTime;
	OpenPrice = r.OpenPrice;
	ClosePrice = r.ClosePrice;
	SGID = r.SGID;
	TEID = r.TEID;

	ReqID = r.ReqID;
	r.ReqID = nullptr;
	SessionID = r.SessionID;
	r.SessionID = nullptr;
	r.OrderStatus = 0;
	r.Direction = 0;
	r.GroupID = 0;
	r.ClientID = 0;
	r.OrderID = 0;
	r.AssetID = 0;
	r.PayOut = 0;
	r.Amount = 0;
	r.OpenTime = 0;
	r.OpenPrice = 0;
	r.ClosePrice = 0;
	r.SGID = 0;
	r.TEID = 0;

	return *this;
}

const char* OrderAns::ToJson(void) {
	if (Json == nullptr) {
		asprintf(&Json, "{\"reqid\":\"%s\",\"sessionid\":\"%s\",\"groupid\":%d,\"clientid\":%d,\"orderid\":%d,\"orderstatus\":%d,\"assetid\":%d,\"direction\":%d,\"amount\":%d,\"payout\":%ld,\"opentime\":%d,\"openprice\":%d,\"closeprice\":%d,\"sgid\":%d,\"teid\":%d}", 
				ReqID, SessionID, GroupID, ClientID, OrderID, OrderStatus, AssetID, Direction, Amount,PayOut, OpenTime, OpenPrice, ClosePrice, SGID, TEID);
	}

	return Json;
}
/* OrderAns end ***/

/* ValidateOrder begin ***/
ValidateOrder::ValidateOrder() :
	BOAccountID(0),
	Direction(0),
	OrderStatus(0),
	ReqID(nullptr),
	SessionID(nullptr),
	Json(nullptr),
	Status(0),
	GroupID(0),
	ClientID(0),
	AssetID(0),
	ExpiryPeriod(0),
	Amount(0),
	SGID(0),
	AccountID(0),
	Margin(0),
	FreeMargin(0),
	Profit(0),
	PrevBalance(0),
	Changed(0),
	CloseBalance(0) {
	ReqID = (char*) malloc(REQUEST_ID_LEN);
	memset(ReqID, 0, REQUEST_ID_LEN);
	SessionID = (char*) malloc(SESSION_ID_LEN);
	memset(SessionID, 0, SESSION_ID_LEN);
}

ValidateOrder::ValidateOrder(unsigned char boAccountID,
		unsigned char direction,
		unsigned char orderStatus,
		const char* reqID,
		const char* sessionID,
		unsigned int groupID,
		unsigned int clientID,
		unsigned int assetID,
		unsigned int expiryPeriod,
		unsigned int amount,
		unsigned int sgID,
		unsigned long accountID,
		long margin,
		long freeMargin,
		long profit):
	BOAccountID(boAccountID),
	Direction(direction),
	OrderStatus(orderStatus),
	ReqID(nullptr),
	SessionID(nullptr),
	Json(nullptr),
	Status(0),
	GroupID(groupID),
	ClientID(clientID),
	AssetID(assetID),
	ExpiryPeriod(expiryPeriod),
	Amount(amount),
	SGID(sgID),
	AccountID(accountID),
	Margin(margin),
	FreeMargin(freeMargin),
	Profit(profit),
	PrevBalance(0),
	Changed(0),
	CloseBalance(0) {
	ReqID = (char*) malloc(REQUEST_ID_LEN);
	memcpy(ReqID, reqID, REQUEST_ID_LEN);
	SessionID = (char*) malloc(SESSION_ID_LEN);
	memcpy(SessionID, sessionID, SESSION_ID_LEN);
}

ValidateOrder::~ValidateOrder() {
	if (ReqID != nullptr) {
		free(ReqID);
		ReqID = nullptr;
	}

	if (SessionID != nullptr) {
		free(SessionID);
		SessionID = nullptr;
	}

	if (Json != nullptr) {
		free(Json);
		Json = nullptr;
	}
}

ValidateOrder::ValidateOrder(const ValidateOrder& r) {
	BOAccountID = r.BOAccountID;
	Direction = r.Direction;
	OrderStatus = r.OrderStatus;
	Status = r.Status;
	GroupID = r.GroupID;
	ClientID = r.ClientID;
	AssetID = r.AssetID;
	ExpiryPeriod = r.ExpiryPeriod;
	Amount = r.Amount;
	SGID = r.SGID;
	AccountID = r.AccountID;
	Margin = r.Margin;
	FreeMargin = r.FreeMargin;
	Profit = r.Profit;
	PrevBalance = r.PrevBalance;
	Changed = r.Changed;
	CloseBalance = r.CloseBalance;
	ReqID = (char*) malloc(REQUEST_ID_LEN);
	memcpy(ReqID, r.ReqID, REQUEST_ID_LEN);
	SessionID = (char*) malloc(SESSION_ID_LEN);
	memcpy(SessionID, r.SessionID, SESSION_ID_LEN);
	
	if (r.Json != nullptr) {
		size_t len = strlen(r.Json);
		Json = (char*) malloc (len+1);
		memcpy(Json, r.Json, len);
		Json[len] = 0;
	} else {
		Json = nullptr;
	}
}

ValidateOrder& ValidateOrder::operator=(const ValidateOrder& r) { 
	if (this == &r) { 
		return *this;
	}

	if (Json != nullptr) {
		free(Json);
		Json = nullptr;
	}

	BOAccountID = r.BOAccountID;
	Direction = r.Direction;
	OrderStatus = r.OrderStatus;
	Status = r.Status;
	GroupID = r.GroupID;
	ClientID = r.ClientID;
	AssetID = r.AssetID;
	ExpiryPeriod = r.ExpiryPeriod;
	Amount = r.Amount;
	SGID = r.SGID;
	AccountID = r.AccountID;
	Margin = r.Margin;
	FreeMargin = r.FreeMargin;
	Profit = r.Profit;
	PrevBalance = r.PrevBalance;
	Changed = r.Changed;
	CloseBalance = r.CloseBalance;
	memcpy(ReqID, r.ReqID, REQUEST_ID_LEN);
	memcpy(SessionID, r.SessionID, SESSION_ID_LEN);

	if (r.Json != nullptr) {
		size_t len = strlen(r.Json);
		Json = (char*) malloc (len+1);
		memcpy(Json, r.Json, len);
		Json[len] = 0;
	} else {
		Json = nullptr;
	}

	return *this;
}

ValidateOrder::ValidateOrder(ValidateOrder&& r) {
	BOAccountID = r.BOAccountID;
	Direction = r.Direction;
	OrderStatus = r.OrderStatus;
	Status = r.Status;
	GroupID = r.GroupID;
	ClientID = r.ClientID;
	AssetID = r.AssetID;
	ExpiryPeriod = r.ExpiryPeriod;
	Amount = r.Amount;
	SGID = r.SGID;
	AccountID = r.AccountID;
	Margin = r.Margin;
	FreeMargin = r.FreeMargin;
	Profit = r.Profit;
	PrevBalance = r.PrevBalance;
	Changed = r.Changed;
	CloseBalance = r.CloseBalance;

	ReqID = r.ReqID;
	r.ReqID = nullptr;
	SessionID = r.SessionID;
	r.SessionID = nullptr;
	Json = r.Json;
	r.Json = nullptr;
	r.BOAccountID = 0;
	r.Direction = 0;
	r.OrderStatus = 0;
	r.GroupID = 0;
	r.ClientID = 0;
	r.AssetID = 0;
	r.ExpiryPeriod = 0;
	r.Amount = 0;
	r.SGID = 0;
	r.AccountID = 0;
	r.Margin = 0;
	r.FreeMargin = 0;
	r.Profit = 0;
	r.Status = 0;
	r.PrevBalance = 0;
	r.Changed = 0;
	r.CloseBalance = 0;
}

ValidateOrder& ValidateOrder::operator=(ValidateOrder&& r) {
	if (this == &r) {
		return *this;
	}

	if (ReqID != nullptr) {
		free(ReqID);
		ReqID = nullptr;
	}

	if (SessionID != nullptr) {
		free(SessionID);
		SessionID = nullptr;
	}

	if (Json != nullptr) {
		free(Json);
		Json = nullptr;
	}

	BOAccountID = r.BOAccountID;
	Direction = r.Direction;
	OrderStatus = r.OrderStatus;
	Status = r.Status;
	GroupID = r.GroupID;
	ClientID = r.ClientID;
	AssetID = r.AssetID;
	ExpiryPeriod = r.ExpiryPeriod;
	Amount = r.Amount;
	SGID = r.SGID;
	AccountID = r.AccountID;
	Margin = r.Margin;
	FreeMargin = r.FreeMargin;
	Profit = r.Profit;
	PrevBalance = r.PrevBalance;
	Changed = r.Changed;
	CloseBalance = r.CloseBalance;

	ReqID = r.ReqID;
	r.ReqID = nullptr;
	SessionID = r.SessionID;
	r.SessionID = nullptr;
	Json = r.Json;
	r.Json = nullptr;
	r.BOAccountID = 0;
	r.Direction = 0;
	r.OrderStatus = 0;
	r.GroupID = 0;
	r.ClientID = 0;
	r.AssetID = 0;
	r.ExpiryPeriod = 0;
	r.Amount = 0;
	r.SGID = 0;
	r.AccountID = 0;
	r.Margin = 0;
	r.FreeMargin = 0;
	r.Profit = 0;
	r.Status = 0;
	r.PrevBalance = 0;
	r.Changed = 0;
	r.CloseBalance = 0;

	return *this;
}

const char* ValidateOrder::ToJson(void) {
#if 0
	if (Json == nullptr) {
		asprintf(&Json, "{\"reqid\":\"%s\",\"groupid\":%d,\"clientid\":%d,\"accountid\":%ld,\"boaccountid\":%d,\"assetid\":%d,\"direction\":%d,\"expiryperiod\":%d,\"amount\":%d,\"sgid\":%d,\"orderstatus\":%d,\"margin\":%ld,\"freemargin\":%ld,\"profit\":%ld}", 
				ReqID, GroupID, ClientID, AccountID, BOAccountID, AssetID, Direction, ExpiryPeriod, Amount, SGID, OrderStatus, Margin, FreeMargin, Profit);
	}
#endif
	if (Json == nullptr) {
		asprintf(&Json, "{\"reqid\":\"%s\",\"sessionid\":\"%s\",\"groupid\":%d,\"clientid\":%d,\"accountid\":%ld,\"boaccountid\":%d,\"assetid\":%d,\"direction\":%d,\"expiryperiod\":%d,\"amount\":%d,\"sgid\":%d,\"orderstatus\":%d,\"margin\":%ld,\"freemargin\":%ld,\"profit\":%ld,\"status\":%d,\"prevbalance\":%ld,\"changed\":%ld,\"closebalance\":%ld}", 
				ReqID, SessionID, GroupID, ClientID, AccountID, BOAccountID, AssetID, Direction, ExpiryPeriod, Amount, SGID, OrderStatus, Margin, FreeMargin, Profit, Status, PrevBalance, Changed, CloseBalance);
	}

	return Json;
}
/* ValidateOrder end ***/

/* RequestAck begin **/
RequestAck::RequestAck() : 
	ReqStatus(0),
	ReqID(nullptr),	
	SGID_TEID(0),
	Json(nullptr) {
	ReqID = (char*) malloc(SOLUTIONGATEWAY_REQUEST_ID_LEN * sizeof(char));
	memset(ReqID, 0, SOLUTIONGATEWAY_REQUEST_ID_LEN * sizeof(char));
}

RequestAck::RequestAck(unsigned char reqStatus, const char* reqID, unsigned int sgid_teid) : 
	ReqStatus(0),
	ReqID(nullptr),	
	SGID_TEID(sgid_teid),
	Json(nullptr) {
	ReqID = (char*) malloc(SOLUTIONGATEWAY_REQUEST_ID_LEN * sizeof(char));
	memcpy(ReqID, reqID, SOLUTIONGATEWAY_REQUEST_ID_LEN * sizeof(char));
	//Data = data;
}

RequestAck::~RequestAck() {
	if (ReqID != nullptr) {
		free(ReqID);
		ReqID = nullptr;
	}

	if (Json != nullptr) {
		free(Json);
		Json = nullptr;
	}
}

RequestAck::RequestAck(const RequestAck& r) {
	ReqStatus = r.ReqStatus;
	ReqID = (char*) malloc(SOLUTIONGATEWAY_REQUEST_ID_LEN * sizeof(char));
	memcpy(ReqID, r.ReqID, SOLUTIONGATEWAY_REQUEST_ID_LEN * sizeof(char));
	SGID_TEID = r.SGID_TEID;
	
	if (r.Json != nullptr) {
		size_t len = strlen(r.Json);
		Json = (char*) malloc (len+1);
		memcpy(Json, r.Json, len);
		Json[len] = 0;
	} else {
		Json = nullptr;
	}
}

RequestAck& RequestAck::operator=(const RequestAck& r) {
	if (this != &r) {
		return *this;
	}

	if (Json != nullptr) {
		free(Json);
		Json = nullptr;
	}

	ReqStatus = r.ReqStatus;
	memcpy(ReqID, r.ReqID, SOLUTIONGATEWAY_REQUEST_ID_LEN * sizeof(char));
	SGID_TEID = r.SGID_TEID;

	if (r.Json != nullptr) {
		size_t len = strlen(r.Json);
		Json = (char*) malloc (len+1);
		memcpy(Json, r.Json, len);
		Json[len] = 0;
	}

	return *this;
}

RequestAck::RequestAck(RequestAck&& r) {
	ReqStatus = r.ReqStatus;
	ReqID = r.ReqID;
	SGID_TEID = r.SGID_TEID;
	Json = r.Json;

	r.ReqID = nullptr;
	r.ReqStatus = 0;
	r.SGID_TEID = 0;
	r.Json = nullptr;
}

RequestAck& RequestAck::operator=(RequestAck&& r) {
	if (this != &r) {
		return *this;
	}

	if (ReqID != nullptr) {
		free(ReqID);
		ReqID = nullptr;
	}

	if (Json != nullptr) {
		free(Json);
		Json = nullptr;
	}

	ReqStatus = r.ReqStatus;
	ReqID = r.ReqID;
	SGID_TEID = r.SGID_TEID;
	Json = r.Json;

	r.ReqID = nullptr;
	r.ReqStatus = 0;
	r.SGID_TEID = 0;
	r.Json = nullptr;

	return *this;
}

const char* RequestAck::ToJson(void) {
	if (Json == nullptr) {
		asprintf(&Json, "{\"reqid\":\"%s\",\"reqstatus\":%d,\"sgid_teid\":%d}", ReqID, ReqStatus, SGID_TEID);
	}

	return Json;
}
/* RequestAck end **/ 

/** PositionAccount begin ***/
PositionAccount::PositionAccount() : 
	Json(nullptr),
	Type(0),	
	Status(0),	
	GroupID(0),	
	ClientID(0),	
	Check(0),	
	ErrMsgIndex(0),	
	ReqID(nullptr),	
	SessionID(nullptr),	
	AccountID(0),	
	Balance(0),	
	PrevBalance(0),	
	Changed(0),	
	Margin(0),
	FreeMargin(0),
	Profit(0),
	CloseBalance(0) {
	ReqID = (char*) malloc(REQUEST_ID_LEN);
	memset(ReqID, 0, REQUEST_ID_LEN);
	SessionID = (char*) malloc(SESSION_ID_LEN);
	memset(SessionID, 0, SESSION_ID_LEN);
}

#if 0
PositionAccount::PositionAccount(long reqID, unsigned int sgid_teid) : 
	ReqStatus(0),
	ReqID(nullptr),	
	SGID_TEID(sgid_teid),
	Json(nullptr) {
}
#endif

PositionAccount::~PositionAccount() {
	if (ReqID != nullptr) {
		free(ReqID);
		ReqID = nullptr;
	}

	if (SessionID != nullptr) {
		free(SessionID);
		SessionID = nullptr;
	}

	if (Json != nullptr) {
		free(Json);
		Json = nullptr;
	}
}

PositionAccount::PositionAccount(const PositionAccount& r) {
	Type = r.Type;
	Status = r.Status;
	GroupID = r.GroupID;
	ClientID = r.ClientID;
	Check = r.Check;
	ErrMsgIndex = r.ErrMsgIndex;
	//ReqID = r.ReqID;
	ReqID = (char*) malloc(REQUEST_ID_LEN);
	memcpy(ReqID, r.ReqID, REQUEST_ID_LEN);
	SessionID = (char*) malloc(REQUEST_ID_LEN);
	memcpy(SessionID, r.SessionID, REQUEST_ID_LEN);
	AccountID = r.AccountID;
	Balance = r.Balance;
	PrevBalance = r.PrevBalance;
	Changed = r.Changed;
	Margin = r.Margin;
	FreeMargin = r.FreeMargin;
	Profit = r.Profit;
	CloseBalance = r.CloseBalance;

	if (r.Json != nullptr) {
		size_t len = strlen(r.Json);
		Json = (char*) malloc (len+1);
		memcpy(Json, r.Json, len);
		Json[len] = 0;
	} else {
		Json = nullptr;
	}
}

PositionAccount& PositionAccount::operator=(const PositionAccount& r) {
	if (this != &r) {
		return *this;
	}

	if (Json != nullptr) {
		free(Json);
		Json = nullptr;
	}

	Type = r.Type;
	Status = r.Status;
	GroupID = r.GroupID;
	ClientID = r.ClientID;
	Check = r.Check;
	ErrMsgIndex = r.ErrMsgIndex;
	//ReqID = r.ReqID;
	memcpy(ReqID, r.ReqID, REQUEST_ID_LEN);
	memcpy(SessionID, r.SessionID, REQUEST_ID_LEN);
	AccountID = r.AccountID;
	Balance = r.Balance;
	PrevBalance = r.PrevBalance;
	Changed = r.Changed;
	Margin = r.Margin;
	FreeMargin = r.FreeMargin;
	Profit = r.Profit;
	CloseBalance = r.CloseBalance;

	if (r.Json != nullptr) {
		size_t len = strlen(r.Json);
		Json = (char*) malloc (len+1);
		memcpy(Json, r.Json, len);
		Json[len] = 0;
	}

	return *this;
}

PositionAccount::PositionAccount(PositionAccount&& r) {
	Json = r.Json;
	Type = r.Type;
	Status = r.Status;
	GroupID = r.GroupID;
	ClientID = r.ClientID;
	Check = r.Check;
	ErrMsgIndex = r.ErrMsgIndex;
	ReqID = r.ReqID;
	SessionID = r.SessionID;
	AccountID = r.AccountID;
	Balance = r.Balance;
	PrevBalance = r.PrevBalance;
	Changed = r.Changed;
	Margin = r.Margin;
	FreeMargin = r.FreeMargin;
	Profit = r.Profit;
	CloseBalance = r.CloseBalance;

	r.Json = nullptr;
	r.Type = 0;
	r.Status = 0;
	r.GroupID = 0;
	r.ClientID = 0;
	r.Check = 0;
	r.ErrMsgIndex = 0;
	r.ReqID = nullptr;
	r.SessionID = nullptr;
	r.AccountID = 0;
	r.Balance = 0;
	r.PrevBalance = 0;
	r.Changed = 0;
	r.Margin = 0;
	r.FreeMargin = 0;
	r.Profit = 0;
	r.CloseBalance = 0;
}

PositionAccount& PositionAccount::operator=(PositionAccount&& r) {
	if (this != &r) {
		return *this;
	}

	if (ReqID != nullptr) {
		free(ReqID);
		ReqID = nullptr;
	}

	if (SessionID != nullptr) {
		free(SessionID);
		SessionID = nullptr;
	}

	if (Json != nullptr) {
		free(Json);
		Json = nullptr;
	}

	Json = r.Json;
	Type = r.Type;
	Status = r.Status;
	GroupID = r.GroupID;
	ClientID = r.ClientID;
	Check = r.Check;
	ErrMsgIndex = r.ErrMsgIndex;
	ReqID = r.ReqID;
	SessionID = r.SessionID;
	AccountID = r.AccountID;
	Balance = r.Balance;
	PrevBalance = r.PrevBalance;
	Changed = r.Changed;
	Margin = r.Margin;
	FreeMargin = r.FreeMargin;
	Profit = r.Profit;
	CloseBalance = r.CloseBalance;

	r.Json = nullptr;
	r.Type = 0;
	r.Status = 0;
	r.GroupID = 0;
	r.ClientID = 0;
	r.Check = 0;
	r.ErrMsgIndex = 0;
	r.ReqID = nullptr;
	r.SessionID = nullptr;
	r.AccountID = 0;
	r.Balance = 0;
	r.PrevBalance = 0;
	r.Changed = 0;
	r.Margin = 0;
	r.FreeMargin = 0;
	r.Profit = 0;
	r.CloseBalance = 0;

	return *this;
}

PositionAccount::PositionAccount(const ValidateOrder& r) {
	Status = r.Status;
	GroupID = r.GroupID;
	ClientID = r.ClientID;
	ReqID = (char*) malloc(REQUEST_ID_LEN);
	memcpy(ReqID, r.ReqID, REQUEST_ID_LEN);
	SessionID = (char*) malloc(SESSION_ID_LEN);
	memcpy(SessionID, r.SessionID, SESSION_ID_LEN);
	AccountID = r.AccountID;
	PrevBalance = r.PrevBalance;
	Changed = r.Changed;
	Margin = r.Margin;
	FreeMargin = r.FreeMargin;
	Profit = r.Profit;
	CloseBalance = r.CloseBalance;
	ErrMsgIndex = 0;
	Json = nullptr;

#if 0
	if (r.Json != nullptr) {
		size_t len = strlen(r.Json);
		Json = (char*) malloc (len+1);
		memcpy(Json, r.Json, len);
		Json[len] = 0;
	} else {
		Json = nullptr;
	}
#endif
}

const char* PositionAccount::ToJson(void) {
	if (Json == nullptr) {
		do {
			if (B_IS_SET(Status, ErrMsgIndex)) {
				break;
			} else if (ErrMsgIndex == 8) {
				ErrMsgIndex = 7;
				break;
			}

			++ErrMsgIndex;
		} while (true);

		asprintf(&Json, "{\"reqid\":\"%s\",\"sessionid\":\"%s\",\"groupid\":%d,\"clientid\":%d,\"accountid\":%ld,\"prevbalance\":%ld,\"changed\":%ld,\"margin\":%ld,\"freemargin\":%ld,\"profit\":%ld,\"type\":%d,\"closebalance\":%ld,\"status\":%d,\"errmsg\":\"%s\"}", 
				 ReqID, 
				 SessionID, 
				 GroupID,
				 ClientID,
				 AccountID,
				 //Balance,
				 PrevBalance,
				 Changed,
				 Margin,
				 FreeMargin,
				 Profit,
				 Type,
				 CloseBalance,
				 Status,
				 AccountErrMsg[ErrMsgIndex]);
	}

	return Json;
}

bool PositionAccount::VerifyData(int& status) const {
	if (Margin < 0) {
		status = ACCOUNT_BIT_MAGIN_INVALID;

		return true;
	}

	if (PrevBalance + Changed + Profit != Margin + FreeMargin) {
		status = ACCOUNT_BIT_FORMULA;

		return true;
	}

	status = ACCOUNT_BIT_SUCCESS;

	return false;
}
/** PositionAccount end ***/

/** OCPosition begin ***/
OCPosition::OCPosition() : 
	Json(nullptr),
	Type(0),	
	Status(0),	
	GroupID(0),	
	ClientID(0),	
	Check(0),	
	//ReqID(0),	
	ReqID(nullptr),	
	AccountID(0),	
	Balance(0),	
	PrevBalance(0),	
	Changed(0),	
	Margin(0),
	FreeMargin(0),
	Profit(0),
	CloseBalance(0) {
	ReqID = (char*) malloc(REQUEST_ID_LEN);
	memset(ReqID, 0, REQUEST_ID_LEN);
	SessionID = (char*) malloc(SESSION_ID_LEN);
	memset(SessionID, 0, SESSION_ID_LEN);
}

#if 0
OCPosition::OCPosition(long reqID, unsigned int sgid_teid) : 
	ReqStatus(0),
	ReqID(nullptr),	
	SGID_TEID(sgid_teid),
	Json(nullptr) {
}
#endif

OCPosition::~OCPosition() {
	if (ReqID != nullptr) {
		free(ReqID);
		ReqID = nullptr;
	}

	if (SessionID != nullptr) {
		free(SessionID);
		SessionID = nullptr;
	}

	if (Json != nullptr) {
		free(Json);
		Json = nullptr;
	}
}

OCPosition::OCPosition(const OCPosition& r) {
	Type = r.Type;
	Status = r.Status;
	GroupID = r.GroupID;
	ClientID = r.ClientID;
	Check = r.Check;
	ReqID = (char*) malloc(REQUEST_ID_LEN);
	memcpy(ReqID, r.ReqID, REQUEST_ID_LEN);
	SessionID = (char*) malloc(SESSION_ID_LEN);
	memcpy(SessionID, r.SessionID, SESSION_ID_LEN);
	AccountID = r.AccountID;
	Balance = r.Balance;
	PrevBalance = r.PrevBalance;
	Changed = r.Changed;
	Margin = r.Margin;
	FreeMargin = r.FreeMargin;
	Profit = r.Profit;
	CloseBalance = r.CloseBalance;

	if (r.Json != nullptr) {
		size_t len = strlen(r.Json);
		Json = (char*) malloc (len+1);
		memcpy(Json, r.Json, len);
		Json[len] = 0;
	} else {
		Json = nullptr;
	}
}

OCPosition& OCPosition::operator=(const OCPosition& r) {
	if (this != &r) {
		return *this;
	}

	if (Json != nullptr) {
		free(Json);
		Json = nullptr;
	}

	Type = r.Type;
	Status = r.Status;
	GroupID = r.GroupID;
	ClientID = r.ClientID;
	Check = r.Check;
	memcpy(ReqID, r.ReqID, REQUEST_ID_LEN);
	memcpy(SessionID, r.SessionID, SESSION_ID_LEN);
	AccountID = r.AccountID;
	Balance = r.Balance;
	PrevBalance = r.PrevBalance;
	Changed = r.Changed;
	Margin = r.Margin;
	FreeMargin = r.FreeMargin;
	Profit = r.Profit;
	CloseBalance = r.CloseBalance;

	if (r.Json != nullptr) {
		size_t len = strlen(r.Json);
		Json = (char*) malloc (len+1);
		memcpy(Json, r.Json, len);
		Json[len] = 0;
	}

	return *this;
}

OCPosition::OCPosition(OCPosition&& r) {
	Json = r.Json;
	Type = r.Type;
	Status = r.Status;
	GroupID = r.GroupID;
	ClientID = r.ClientID;
	Check = r.Check;
	ReqID = r.ReqID;
	SessionID = r.SessionID;
	AccountID = r.AccountID;
	Balance = r.Balance;
	PrevBalance = r.PrevBalance;
	Changed = r.Changed;
	Margin = r.Margin;
	FreeMargin = r.FreeMargin;
	Profit = r.Profit;
	CloseBalance = r.CloseBalance;

	r.Json = nullptr;
	r.Type = 0;
	r.Status = 0;
	r.GroupID = 0;
	r.ClientID = 0;
	r.Check = 0;
	r.ReqID = nullptr;
	r.SessionID = nullptr;
	r.AccountID = 0;
	r.Balance = 0;
	r.PrevBalance = 0;
	r.Changed = 0;
	r.Margin = 0;
	r.FreeMargin = 0;
	r.Profit = 0;
	r.CloseBalance = 0;
}

OCPosition& OCPosition::operator=(OCPosition&& r) {
	if (this != &r) {
		return *this;
	}

	if (ReqID != nullptr) {
		free(ReqID);
		ReqID = nullptr;
	}

	if (SessionID != nullptr) {
		free(SessionID);
		SessionID = nullptr;
	}

	if (Json != nullptr) {
		free(Json);
		Json = nullptr;
	}

	Json = r.Json;
	Type = r.Type;
	Status = r.Status;
	GroupID = r.GroupID;
	ClientID = r.ClientID;
	Check = r.Check;
	ReqID = r.ReqID;
	SessionID = r.SessionID;
	AccountID = r.AccountID;
	Balance = r.Balance;
	PrevBalance = r.PrevBalance;
	Changed = r.Changed;
	Margin = r.Margin;
	FreeMargin = r.FreeMargin;
	Profit = r.Profit;
	CloseBalance = r.CloseBalance;

	r.Json = nullptr;
	r.Type = 0;
	r.Status = 0;
	r.GroupID = 0;
	r.ClientID = 0;
	r.Check = 0;
	r.ReqID = nullptr;
	r.SessionID = nullptr;
	r.AccountID = 0;
	r.Balance = 0;
	r.PrevBalance = 0;
	r.Changed = 0;
	r.Margin = 0;
	r.FreeMargin = 0;
	r.Profit = 0;
	r.CloseBalance = 0;

	return *this;
}

OCPosition::OCPosition(const ValidateOrder& r) {
	Status = r.Status;
	GroupID = r.GroupID;
	ClientID = r.ClientID;
	ReqID = (char*) malloc(REQUEST_ID_LEN);
	memcpy(ReqID, r.ReqID, REQUEST_ID_LEN);
	SessionID = (char*) malloc(SESSION_ID_LEN);
	memcpy(SessionID, r.SessionID, SESSION_ID_LEN);
	AccountID = r.AccountID;
	PrevBalance = r.PrevBalance;
	Changed = r.Changed;
	Margin = r.Margin;
	FreeMargin = r.FreeMargin;
	Profit = r.Profit;
	CloseBalance = r.CloseBalance;

	Type = 0;
	Check = 0;
	Balance = 0;

	if (r.Json != nullptr) {
		size_t len = strlen(r.Json);
		Json = (char*) malloc (len+1);
		memcpy(Json, r.Json, len);
		Json[len] = 0;
	} else {
		Json = nullptr;
	}
}

const char* OCPosition::ToJson(void) {
	if (Json == nullptr) {
		asprintf(&Json, "{\"reqid\":\"%s\",\"sessionid\":\"%s\",\"groupid\":%d,\"clientid\":%d,\"accountid\":%ld,\"balance\":%ld,\"prevbalance\":%ld,\"changed\":%ld,\"margin\":%ld,\"freemargin\":%ld,\"profit\":%ld,\"type\":%d,\"closebalance\":%ld,\"status\":%d}", 
				 ReqID, 
				 SessionID,
				 GroupID,
				 ClientID,
				 AccountID,
				 Balance,
				 PrevBalance,
				 Changed,
				 Margin,
				 FreeMargin,
				 Profit,
				 Type,
				 CloseBalance,
				 Status);
				 //Check);
	}

	return Json;
}

MonitorReq::MonitorReq() : 
	Json(nullptr),
	ReqID(nullptr),
	Mid(0),
	TimeStamp(0) {
	ReqID = (char*) malloc(REQUEST_ID_LEN);
	memset(ReqID, 0, REQUEST_ID_LEN);
}

MonitorReq::MonitorReq(char* reqID, 
	uint32_t mid, 
	uint64_t timeStamp) :
	Json(nullptr),
	ReqID(nullptr),
	Mid(mid),
	TimeStamp(timeStamp) {
	ReqID = (char*) malloc(REQUEST_ID_LEN);
	memcpy(ReqID, reqID, REQUEST_ID_LEN);
}

MonitorReq::~MonitorReq() {
	if(Json != nullptr) {
		free(Json);
		Json = nullptr;
	}

	if(ReqID != nullptr) {
		free(ReqID);
		ReqID = nullptr;
	}
}

MonitorReq::MonitorReq(const MonitorReq& r) {
	Mid = r.Mid;
	TimeStamp = r.TimeStamp;

	ReqID = (char*) malloc(REQUEST_ID_LEN);
	memcpy(ReqID, r.ReqID, REQUEST_ID_LEN);

	if (r.Json != nullptr) {
		size_t len = strlen(r.Json);
		Json = (char*) malloc (len+1);
		memcpy(Json, r.Json, len);
		Json[len] = 0;
	} else {
		Json = nullptr;
	}
}

MonitorReq& MonitorReq::operator=(const MonitorReq& r) {
	if(this == &r) {
		return *this;
	}

	if(Json != nullptr) {
		free(Json);
		Json = nullptr;
	}

	// if(ReqID != nullptr) {
	// 	free(ReqID);
	// 	ReqID = nullptr;
	// }

	Mid = r.Mid;
	TimeStamp = r.TimeStamp;

//	ReqID = (char*) malloc(REQUEST_ID_LEN);
	memcpy(ReqID, r.ReqID, REQUEST_ID_LEN);

	if (r.Json != nullptr) {
		size_t len = strlen(r.Json);
		Json = (char*) malloc (len+1);
		memcpy(Json, r.Json, len);
		Json[len] = 0;
	}

	return *this;
}

MonitorReq::MonitorReq(MonitorReq&& r) {
	Json = r.Json;
	ReqID = r.ReqID;
	Mid = r.Mid;
	TimeStamp = r.TimeStamp;

	r.Json = nullptr;
	r.ReqID = nullptr;
	r.Mid = 0;
	r.TimeStamp = 0;
}

MonitorReq& MonitorReq::operator=(MonitorReq&& r) {
	if(this == &r) {
		return *this;
	}

	if(Json != nullptr) {
		free(Json);
		Json = nullptr;
	}

	if(ReqID != nullptr) {
		free(ReqID);
		ReqID = nullptr;
	}

	Json = r.Json;
	ReqID = r.ReqID;
	Mid = r.Mid;
	TimeStamp = r.TimeStamp;

	r.Json = nullptr;
	r.ReqID = nullptr;
	r.Mid = 0;
	r.TimeStamp = 0;

	return *this;
}

const char* MonitorReq::ToJson(void) {
	if (Json == nullptr) {
		asprintf(&Json, "{\"reqid\":\"%s\",\"mid\":%d,\"timeStamp\":%ld}", 
				 ReqID, 
				 Mid,
				 TimeStamp);
	}

	return Json;
}

/*** MonitorAns begin ***/
MonitorAns::MonitorAns() :
	Json(nullptr),
	ReqID(nullptr),
	Status(0),
	Name(nullptr),
	Pid(0),
	Mid(0),
	TimeStamp(0),
	StartTimes(0) {
	ReqID = (char*) malloc(REQUEST_ID_LEN);
	memset(ReqID, 0, REQUEST_ID_LEN);
	Name = (char*) malloc(MODEL_NAME_LEN);
	memset(Name, 0, MODEL_NAME_LEN);
}

MonitorAns::MonitorAns(const char* reqID, 
	char status, 
	const char* name,
	uint32_t pid,
	uint32_t mid,
	uint64_t timeStamp,
	uint64_t startTimes) : 
	Json(nullptr),
	ReqID(nullptr),
	Status(status),
	Name(nullptr),
	Pid(pid),
	Mid(mid),
	TimeStamp(timeStamp),
	StartTimes(startTimes) {
	ReqID = (char*) malloc(REQUEST_ID_LEN * sizeof(char));
	memcpy(ReqID, reqID, REQUEST_ID_LEN);

	Name = (char*) malloc(MODEL_NAME_LEN + 1);
	int len = strlen(name);
	if(len > MODEL_NAME_LEN) {
		len = MODEL_NAME_LEN;
	}
	memcpy(Name, name, len);
	Name[len] = 0;
	//strncpy(Name, name, MODEL_NAME_LEN);
}	

MonitorAns::~MonitorAns() {
	if(Json != nullptr) {
		free(Json);
		Json = nullptr;
	}

	if(ReqID != nullptr) {
		free(ReqID);
		ReqID = nullptr;
	}

	if(Name != nullptr) {
		free(Name);
		Name = nullptr;
	}
}

MonitorAns::MonitorAns(const MonitorAns& r) {
	Status = r.Status;
	Pid = r.Pid;
	Mid = r.Mid;
	TimeStamp = r.TimeStamp;
	StartTimes = r.StartTimes;

	ReqID = (char*) malloc(REQUEST_ID_LEN);
	memcpy(ReqID, r.ReqID, REQUEST_ID_LEN);

	Name = (char*) malloc(MODEL_NAME_LEN);
	memcpy(Name, r.Name, MODEL_NAME_LEN);

	if (r.Json != nullptr) {
		size_t len = strlen(r.Json);
		Json = (char*) malloc (len+1);
		memcpy(Json, r.Json, len);
		Json[len] = 0;
	} else {
		Json = nullptr;
	}
}

MonitorAns& MonitorAns::operator=(const MonitorAns& r) {
	if(this == &r) {
		return *this;
	}

	if(Json != nullptr) {
		free(Json);
		Json = nullptr;
	}

	memcpy(ReqID, r.ReqID, REQUEST_ID_LEN);
	Status = r.Status;
	memcpy(Name, r.Name, REQUEST_ID_LEN);
	Pid = r.Pid;
	Mid = r.Mid;
	TimeStamp = r.TimeStamp;
	StartTimes = r.StartTimes;

	if (r.Json != nullptr) {
		size_t len = strlen(r.Json);
		Json = (char*) malloc (len+1);
		memcpy(Json, r.Json, len);
		Json[len] = 0;
	}

	return *this;
}

MonitorAns::MonitorAns(MonitorAns&& r) {
	Json = r.Json;
	ReqID = r.ReqID;
	Status = r.Status;
	Name = r.Name;
	Pid = r.Pid;
	Mid = r.Mid;
	TimeStamp = r.TimeStamp;
	StartTimes = r.StartTimes;

	r.Json = nullptr;
	r.ReqID = nullptr;
	r.Status = 0;
	r.Name = nullptr;
	r.Pid = 0;
	r.Mid = 0;
	r.TimeStamp = 0;
	r.StartTimes = 0;
}

MonitorAns& MonitorAns::operator=(MonitorAns&& r) {
	if(this == &r) {
		return *this;
	}

	if(Json != nullptr) {
		free(Json);
		Json = nullptr;
	}
	
	if(ReqID != nullptr) {
		free(ReqID);
		ReqID = nullptr;
	}

	if(Name != nullptr) {
		free(Name);
		Name = nullptr;
	}

	Json = r.Json;
	ReqID = r.ReqID;
	Status = r.Status;
	Name = r.Name;
	Pid = r.Pid;
	Mid = r.Mid;
	TimeStamp = r.TimeStamp;
	StartTimes = r.StartTimes;

	r.Json = nullptr;
	r.ReqID = nullptr;
	r.Status = 0;
	r.Name = nullptr;
	r.Pid = 0;
	r.Mid = 0;
	r.TimeStamp = 0;
	r.StartTimes = 0;

	return *this;
}

const char* MonitorAns::ToJson(void) {
	if (Json == nullptr) {
		asprintf(&Json, "{\"reqid\":\"%s\",\"status\":%d,\"name\":\"%s\",\"pid\":%d,\"mid\":%d,\"timestamp\":%ld,\"starttimes\":%ld}", 
				 ReqID, 
				 Status,
				 Name,
				 Pid,
				 Mid,
				 TimeStamp,
				 StartTimes);
	}

	return Json;
}
/*** MonitorAns end ***/

/*** DecoMonitorAns begin ***/
DecoMonitorAns::DecoMonitorAns(MonitorAns& _monitorAns, 
							   const char* sockIP,
							   unsigned short port,
							   int currUser,
							   int maxUser) :
	monitorAns(_monitorAns),
	Port(port),
	SockIP(sockIP),
	CurrUser(currUser),
	MaxUser(maxUser) {
}

const char* DecoMonitorAns::ToJson(void) {
	if (Json != nullptr) {
		free(Json);
	}

	asprintf(&Json, "{\"reqid\":\"%s\",\"status\":%d,\"name\":\"%s\",\"pid\":%d,\"mid\":%d,\"timestamp\":%ld,\"starttimes\":%ld,\"port\":%d,\"sockip\":\"%s\",\"curruser\":%d,\"maxuser\":%d}", 
			 monitorAns.ReqID, 
			 monitorAns.Status,
			 monitorAns.Name,
			 monitorAns.Pid,
			 monitorAns.Mid,
			 monitorAns.TimeStamp,
			 monitorAns.StartTimes,
			 Port,
			 SockIP,
			 CurrUser,
			 MaxUser);

	return Json;
}
/*** DecoMonitorAns end ***/

void InitDaemon() {
	pid_t pid;
	int i = 0;

	if ((pid = fork()) == -1) {
		printf("Fork error !\n");
		exit(1);
	}
	if (pid != 0) {
		exit(0);        
	}

	setsid();           
	if ((pid = fork()) == -1) {
		printf("Fork error !\n");
		exit(-1);
	}
	if (pid != 0) {
		exit(0);        
	}
	chdir("./");      
	umask(0);           
	for (; i < getdtablesize(); ++i) {
		close(i);       
	}
}
//}

char* SimpleXorEncrypt(char* event, unsigned int eventLen, int base) {
	int i = 0;
	for(; i < eventLen; i++) {
		event[i] ^= base;
	}
	// event[eventLen - 1] = 0;
	return event;
}

char* SimpleXorDecrypt(char* event, unsigned int eventLen, int base) {
	int i = 0;
	for (; i < eventLen; ++i)
	{
		event[i] ^= base;
	}
	// event[eventLen - 1] = 0;
	return event;
}
