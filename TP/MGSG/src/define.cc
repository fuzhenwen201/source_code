/*************************************************************************
	> File Name: define.cc
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Thu 09 Feb 2017 12:20:40 PM CST
 ************************************************************************/
#include<string.h>
#include"define.h"
#include <iomanip>
#include <ctime>
#include <chrono>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

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
	if(lower) {
		strcpy(pDst, "23456789abcd");
	} else {
		strcpy(pDst, "23456789ABCD");
	}

	strfry(pDst);
	dst[REQUEST_ID_LEN-1] = 0;
//	*pDst = 0;
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
