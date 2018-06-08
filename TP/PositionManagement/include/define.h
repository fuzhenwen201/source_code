/*************************************************************************
	> File Name: define.h
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Fri 03 Feb 2017 11:33:28 AM CST
 ************************************************************************/
#ifndef SOLUTIONGATEWAY_DEFINE_H_
#define SOLUTIONGATEWAY_DEFINE_H_

#include<chrono>

#include"bithack.h"
#include"xlogger.h"
#include<chrono>
#include<sys/time.h>
#include<stdexcept>

class StrLiteral
{
	public:
		template<std::size_t N>
			constexpr StrLiteral(const char(&arr)[N]) noexcept 
			: data{arr}, len{N - 1} {}

		constexpr char operator[](std::size_t i) const
		{
			return i < len ? data[i] : throw std::out_of_range("");
		}

		constexpr std::size_t size() const noexcept { return len; }
		constexpr operator const char*() const noexcept { return data; }
		constexpr const char* c_str() const noexcept { return data; }

	private:
		const char* const data;
		const std::size_t len;
};

// for uv_tcp connection
#define SOLUTIONGATEWAY_BACKLOG	128

// for SolutionGateway xlogger
#define SOLUTIONGATEWAY_LOGGER		"SolutionGateway"
//#define SOLUTIONGATEWAY_LOGGER_DIR	"/home/leaker/tmp/logger/solutiongateway"
#define SOLUTIONGATEWAY_LOGGER_DIR	"./tmp/logger/SolutionGateway"

// for Position Management xlogger
#define POSITIONMANAGEME_LOGGER		"PositionManagement"
//#define SOLUTIONGATEWAY_LOGGER_DIR	"/home/leaker/tmp/logger/PostionManagement"
#define POSITIONMANAGEME_LOGGER_DIR	"./tmp/logger/PositionManagement"

//#define SOLUTIONGATEWAY_REQUEST_ID_LEN	16
#define SOLUTIONGATEWAY_REQUEST_ID_LEN	33
#define REQUEST_ID_LEN	33
#define SESSION_ID_LEN	33
#define MAX_HTTP_HEADERS 20

#define MODEL_NAME_LEN 16

#define URL_MARK_DATA	"/md"
#define URL_SEND_ORDER	"/sendorder"

#if defined(KEEP_ALIVE_TEST)
#define SOLUTIONGATEWAY_PORT	5002
#define RESPONSE \
	"Connection: Keep-Alive\r\n" \
	"Content-Length: "
#else
#define SOLUTIONGATEWAY_PORT	5002
#define RESPONSE \
	"Content-Length: "
#endif

/*"Content-Length: 12\r\n" \ */
/* for Solution Gateway subscribe group name*/
constexpr StrLiteral SOLUTIONGATEWAY_SUBSCRIBE_USER {"USER"};
constexpr StrLiteral SOLUTIONGATEWAY_SUBSCRIBE_VALID {"VALID"};
constexpr StrLiteral SOLUTIONGATEWAY_SUBSCRIBE_ANS {"ORDANS"};

/* for Solution Gateway  publish group name*/
#define SOLUTIONGATEWAY_CHANNEL_SIZE 5
constexpr StrLiteral SOLUTIONGATEWAY_PUBLISH_SENDORDER_GROUP_NAME {"SENDORDER"};
constexpr StrLiteral SOLUTIONGATEWAY_PUBLISH_SENDORDER_ALL_EVENT {"$"};
constexpr StrLiteral SOLUTIONGATEWAY_PUBLISH_TRADINGENGIN {"TRADINGENGIN"};
constexpr StrLiteral SOLUTIONGATEWAY_PUBLISH_USERINFO_NAME {"USERINFO"};
constexpr StrLiteral SOLUTIONGATEWAY_PUBLISH_RISKMANAGEMENT_EVENT {"RM"};
constexpr StrLiteral SOLUTIONGATEWAY_PUBLISH_SYSTEM {"SYSTEM"};
constexpr StrLiteral SOLUTIONGATEWAY_PUBLISH_SYSTEM_EVENT {"POSITION"};

/*solutiongateway login */
constexpr StrLiteral SOLUTIONGATEWAY_PUBLISH_LOGIN_NAME {"LOGIN"};
constexpr StrLiteral SOLUTIONGATEWAY_PUBLISH_LOGIN_EVENT {"CLIENTMANAGE"};

/* solutiongate logout */
constexpr StrLiteral SOLUTIONGATEWAY_PUBLISH_LOGOUT_NAME {"LOGOUT"};
constexpr StrLiteral SOLUTIONGATEWAY_PUBLISH_LOGOUT_EVENT {"CLIENTMANAGE"};

/* solutiongate forcelogout */
constexpr StrLiteral SOLUTIONGATEWAY_PUBLISH_FORCELOGOUT_NAME {"FORCELOGOUT"};
constexpr StrLiteral SOLUTIONGATEWAY_PUBLISH_FORCELOGOUT_EVENT {"CLIENTMANAGE"};

/*SolutionGateway OCPOSITON */
constexpr StrLiteral SOLUTIONGATEWAY_PUBLISH_OCPOSITION_NAME {"OCPOSITION"};
constexpr StrLiteral SOLUTIONGATEWAY_PUBLISH_OCPOSITION_EVENT {"CLIENTMANAGE"};

/*SolutionGateway MONITOR*/
//MONITORANS.[SGID] //MonitorAns
constexpr StrLiteral SOLUTIONGATEWAY_PUBLISH_MONITOR_NAME {"MONITORANS"};

constexpr StrLiteral SOLUTIONGATEWAY_SUBSCRIBE_LOGIN_NAME {"LOGIN"};
constexpr StrLiteral SOLUTIONGATEWAY_SUBSCRIBE_LOGOUT_NAME {"LOGOUT"};
constexpr StrLiteral SOLUTIONGATEWAY_SUBSCRIBE_FORCELOGOUT_NAME {"FORCELOGOUT"};

constexpr StrLiteral SOLUTIONGATEWAY_SUBSCRIBE_USERINFO_NAME {"USERINFO"};
constexpr StrLiteral SOLUTIONGATEWAY_SUBSCRIBE_OCPOSITION_NAME {"OCPOSITION"};
// MONITOR.CM/RM/SG //MonitorReq
constexpr StrLiteral SOLUTIONGATEWAY_SUBSCRIBE_MONITOR_NAME {"MONITOR"};


/*for position management subscribe group name & event */
/* for position management <->te/sg */
constexpr StrLiteral POSITIONMANAGEMENT_SUBSCRIBE_SENDORDER {"SENDORDER"};
constexpr StrLiteral POSITIONMANAGEMENT_SUBSCRIBE_ORDERANS {"ORDERANS"};
constexpr StrLiteral POSITIONMANAGEMENT_SUBSCRIBE_SYSTEM {"SYSTEM"};
constexpr StrLiteral POSITIONMANAGEMENT_SUBSCRIBE_POSITION_EVENT {"POSITION"};
/*for positIion management <-> client management*/
constexpr StrLiteral POSITIONMANAGEMENT_SUBSCRIBE_PUSH {"PUSH"};
constexpr StrLiteral POSITIONMANAGEMENT_SUBSCRIBE_PULL {"PULL"};
constexpr StrLiteral POSITIONMANAGEMENT_SUBSCRIBE_MODIFY {"MODIFY"};

/*for position management publish group name & event */
constexpr StrLiteral POSITIONMANAGEMENT_PUBLISH_VALIDATE {"VALIDATE"};
constexpr StrLiteral POSITIONMANAGEMENT_PUBLISH_VALIDATE_SG_EVENT {"MT"};
constexpr StrLiteral POSITIONMANAGEMENT_PUBLISH_VALIDATE_RM_EVENT {"MT.RM"};
constexpr StrLiteral POSITIONMANAGEMENT_PUBLISH_VALID {"VALID"};
constexpr StrLiteral POSITIONMANAGEMENT_PUBLISH_USER {"USER"};
constexpr StrLiteral POSITIONMANAGEMENT_PUBLISH_TRADINGENGIN {"TRADINGENGIN"};
/*for position management <-> solution gateway*/
constexpr StrLiteral POSITIONMANAGEMENT_PUBLISH_ORDERANS {"ORDERANS"};
/* offline orderans */
constexpr StrLiteral POSITIONMANAGEMENT_PUBLISH_OFFLINE_ORDERANS_NAME {"OFFLINEORDERANS"};
constexpr StrLiteral POSITIONMANAGEMENT_PUBLISH_OFFLINE_ORDERANS_EVENT {"CLIENTMANAGE"};

constexpr StrLiteral POSITIONMANAGEMENT_PUBLISH_REPORT {"REPORT"};
constexpr StrLiteral POSITIONMANAGEMENT_PUBLISH_VALIDATE_ALL_EVENT {"$"};
#define VALIDATE_ALLTE_SGID_LEN_EVENT   64
#define ORDERANS_RM_SGID_LEN_EVENT   64
constexpr StrLiteral POSITIONMANAGEMENT_PUBLISH_VALIDATE_ALLTE_EVENT {"MT.5151.5152.5153.5154.5155.5156.5157.5158.5159"};
constexpr StrLiteral POSITIONMANAGEMENT_PUBLISH_ORDERANS_RM_EVENT {"MT.RM"};
constexpr StrLiteral POSITIONMANAGEMENT_PUBLISH_MONITOR_EVENT {"MONITOR"};

constexpr StrLiteral POSITIONMANAGEMENT_PUBLISH_PULL {"PULL"};
constexpr StrLiteral POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT {"CLIENTMANAGE"};
constexpr StrLiteral POSITIONMANAGEMENT_PUBLISH_MODIFYANS {"MODIFYANS"};
constexpr StrLiteral POSITIONMANAGEMENT_PUBLISH_ACCUPDATE {"ACCUPDATE"};
constexpr StrLiteral POSITIONMANAGEMENT_PUBLISH_ACCUPDATE_ALL_EVENT {"$"};
constexpr StrLiteral POSITIONMANAGEMENT_PUBLISH_SYSTEM {"ACCUPDATE"};

constexpr StrLiteral HKEYVALIDATE {"position"};

/* PM publish message to center logger man*/
constexpr StrLiteral POSITIONMANAGEMENT_PUBLISH_NAME_CLG {"CLG"};
constexpr StrLiteral POSITIONMANAGEMENT_PUBLISH_EVENT_CLG {"POSITION"};
constexpr StrLiteral POSITIONMANAGEMENT_PUBLISH_QUENUENAME {"PULL.CLIENTMANAGE"};

#define PM_BULK_SIZE	500
#define PM_BULK_TIME	10
#define PM_ELAPSED_SECONDS	1

/* bit flag*/
#define ACK_BIT_FLAG		1
#define REJECT_BIT_FLAG		(1 << 1)
#define SG_BIT_FLAG		(1 << 2)
#define PM_BIT_FLAG		(1 << 3)
#define TE_BIT_FLAG		(1 << 4)
#define MT_BIT_FLAG		(1 << 5)
#define CM_BIT_FLAG		(1 << 6)
#define PRICE_BIT_FLAG	(1 << 7)

#define ORDER_STATUS_ACK 1
#define ORDER_STATUS_INVALID (1 << 1)
#define ORDER_STATUS_WIN (1 << 2)
#define ORDER_STATUS_LOSS (1 << 3)
#define ORDER_STATUS_REJECT (1 << 4)
#define ORDER_STATUS_CLOSE	(1 << 7)

#define ACCOUNT_BIT_SUCCESS  1
#define ACCOUNT_BIT_MAGIN_NOTENOUGH  2
#define ACCOUNT_BIT_FORMULA  3
#define ACCOUNT_BIT_FORMAT  4
#define ACCOUNT_BIT_MAGIN_INVALID  5
#define ACCOUNT_BIT_PULL_NOT_EXIST  6
#define ACCOUNT_BIT_PULL_INSERT_EXIST  7
#define ACCOUNT_BIT_FREEMARGIN_INVALID	8
#define ACCOUNT_BIT_ALL_ERROR  9

#define ACCOUNT_BIT_TYPE_INIT  1
#define ACCOUNT_BIT_TYPE_DEPOSITE  (1 << 1)
#define ACCOUNT_BIT_TYPE_WITHDRAWAL  (1 << 2)

/* accept tcp connect */
#define SOLUTIONGATEWAY_HEADER_SENDORDER 1
#define SOLUTIONGATEWAY_HEADER_ORDERANS 2
#define SOLUTIONGATEWAY_HEADER_LOGIN 8
#define SOLUTIONGATEWAY_HEADER_LOGOUT 4
#define SOLUTIONGATEWAY_HEADER_USERINFO 16
#define SOLUTIONGATEWAY_HEADER_OPOSITION 32
#define SOLUTIONGATEWAY_HEADER_CPOSITION 64
#define SOLUTIONGATEWAY_HEADER_OCPOSITION 96


/* orderans passive
 * 0: TE->PM
 * 1: PM->SG*/
#define ORDER_ANS_PASSIVE_TEPM	0
#define ORDER_ANS_PASSIVE_PMSG	1
#define ORDER_ANS_PASSIVE_TESG	2

/* for regex function */
#define ORDERANS_FIELDS 17
constexpr StrLiteral ORDERANS_REGEX {"^\\{\"sessionid\":\"([0-9a-f]{32})\",\"reqid\":\"([0-9a-f]{32})\",\"groupid\":([0-9]{1,}),\"clientid\":([0-9]{1,}),\"orderid\":\"([0-9a-f]{32})\",\"orderstatus\":([0-9]{1,}),\"assetid\":([0-9]{1,}),\"direction\":([01]{1}),\"amount\":([0-9]{1,}),\"expiryperiod\":([0-9]{1,}),\"opentime\":([0-9]{1,}),\"openprice\":([0-9]{1,}),\"closeprice\":([0-9]{1,}),\"payout\":(-?[0-9]{1,}),\"sgid\":([0-9]{1,}),\"teid\":([0-9]{1,})\\}[ \t\r\n]*$"};

#define SENDORDER_FIELDS 10
constexpr StrLiteral SENDORDER_REGEX {"^\\{\"reqid\":\"([0-9a-f]{32})\",\"groupid\":([0-9]{1,}),\"clientid\":([0-9]{1,}),\"assetid\":([0-9]{1,}),\"direction\":([01]{1}),\"expiryperiod\":([0-9]{1,}),\"amount\":([0-9]{1,}),\"sessionid\":\"([0-9a-f]{32})\",\"sgid\":([0-9]{1,})\\}[ \t\r\n]*$"};

enum ETag {
	EREQUESTACK = 0x0004,
	/* sendorder */
	ESENDORDER = 0x0011,
	EORDERANS = 0x0012,
	EVALIDATEORDER = 0x0014,
	/* login */
	ELOGIN = 0x0021,
	ELOGINANS = 0x0022, /*34*/
	EUSERINFON = 0x0024,
	EOPOSITION = 0x0026,
	ECPOSITION = 0x0028,
	EOCPOSITION = 0x002A, /*42*/
	/*logout */
	ELOGOUT = 0x0031,
	ELOGOUTANS = 0x0032,
	EUSERINFOT = 0x0034,
	/*deposite*/
	EPULL = 0x0041,
	EPULLANS = 0x0042,
	EOFFLINEORDERANS = 0x0044,
	/*force logout */
	EFORCELOGOUT = 0x0051,
	EFORCELOGOUTANS = 0x0052,

	EMONITOR = 0xA001,
	EMONTORANS = 0xA002,
	/* center logger */
	ECENTERLOG = 0xE001,
};

enum ELogin {
	ELOGINDEFAULT = 0x00,
	ELOGINSUCCESS = 0x01,
	ELOGININVALID = 0x02,
	ELOGINUNWRITE = 0x04,
	ELOGINCLOSE = 0x08,
	ELOGINLOGOUT = 0x10,
};

enum EPullType {
	EPDEFAULT = 0x00,
	EPINIT = 0x01,  /* login: CM->PM*/
	EPDEPOSITE = 0x02,
	EPWITHDRAWAL = 0x04,
	EPFEEDBACK = 0x08,
	EPUSERINFO = 0x10, /* login CM->SG*/
	EPLOGOUT = 0x20, /* logout (CM->PM) */
	EPFORCELOGOUT = 0x40, /* forcelogout(CM->PM) */
};

struct HeaderTag {
	HeaderTag();
	unsigned char head;
	unsigned char reserved;
	unsigned short tag;
	unsigned int length;
};

//namespace snetwork_xservice_solutiongateway {
/* from hex to binary */
extern char DecLookupTable[];
void HexToBinary(char* dst, char* src, size_t len);
/* from dec to hex string */
extern char HexLookupTable[513];
extern char hexLookupTable[513];
#define PULL_MSG_SIZE	10
extern const char* AccountErrMsg[PULL_MSG_SIZE];
//void DecToHex(char* dst, char* src, size_t len = 16, bool lower = true);
void DecToHex(char* dst, char* src, size_t len = 10, bool lower = true);

#if 0
typedef struct {
	uv_tcp_t handle;
	http_parser parser;
	struct sockaddr_in peer_name;
	//http_request request;
} client_t;
#endif

extern const char* POSITION_DB_NAME;
extern const char* PM_SEND_ORDER_FORMAT;
extern const char* POSITION_DELETE_PULL_FORMAT;
extern const char* SG_SEND_ORDER_FORMAT;
extern const char* SELECT_VALID_FORMAT;
extern const char* SELECT_VALID_FORMAT_STR;
extern const char* PM_ORDER_ANS_FORMAT;
extern const char* SG_ORDER_ANS_FORMAT;
extern const char* POSITION_UPDATE_VALID_FORMAT;
extern const char* POSITION_UPDATE_VALID_FORMAT_STR;
extern const char* POSITION_UPDATE_PULL_FORMAT;
extern const char* PM_VALIDATE_ORDER_FORMAT;
extern const char* SG_VALIDATE_ORDER_FORMAT;
extern const char* POSITION_UPDATE_VALID_WIN_FORMAT;
extern const char* POSITION_UPDATE_VALID_WIN_FORMAT_STR;
extern const char* POSITION_UPDATE_VALID_LOSS_FORMAT;
extern const char* POSITION_UPDATE_VALID_LOSS_FORMAT_STR;
extern const char* POSITION_UPDATE_VALID_REJECT_FORMAT;
extern const char* POSITION_UPDATE_VALID_REJECT_FORMAT_STR;
extern const char* POSITION_UPDATE_VALID_CLOSE_FORMAT;
extern const char* POSITION_UPDATE_VALID_CLOSE_FORMAT_STR;
extern const char* INSERT_POSITION_LOG_FORMAT; 
extern const char* INSERT_POSITION_INIT_FORMAT;

extern const char* SG_CLIENT_LOGIN_FORMAT;
extern const char* SG_CLIENT_LOGOUT_FORMAT;
template<typename T>
inline void FREE(T** t) {
	if (*t != nullptr) {
		free(*t);
		*t = nullptr;
	}
}

template<typename T>
inline void DELETE(T** t) {
	delete(*t);
	*t = nullptr;
}

inline long GetMicroSecond(void) {
	struct timespec t;
	clock_gettime(CLOCK_REALTIME, &t);
	return t.tv_sec*1000000 + t.tv_nsec/1000;
}

/* tag 0x0001 */
struct SendOrder {
	public:
		SendOrder();
		SendOrder(unsigned char direction, 
				  const char* reqID,
				  unsigned int groupID,
				  unsigned int clientID,
				  unsigned int assetID,
				  unsigned int expiryPeriod,
				  unsigned int amount,
				  unsigned int sgID);
		~SendOrder();
		SendOrder(const SendOrder& r);
		SendOrder& operator=(const SendOrder& r);
		SendOrder(SendOrder&& r);
		SendOrder& operator=(SendOrder&& r);

		const char* ToJson(void);

	public:
		unsigned char Direction;	/* up or down*/
		unsigned char Transfer; /* transfer */
		unsigned int GroupID;		/* universal unique */
		unsigned int ClientID;		/* group unique */
		unsigned int AssetID;		/* instrument */
		unsigned int ExpiryPeriod;	/* time period eg. 5 second */
		unsigned int Amount;		/* amount of inverstment */
		unsigned int SGID;			/* solution gateway seq ID */
		char* ReqID;				/* Universal uniq 16B */
		char* Json;
	};

/* 2B tag 0x0002 */
struct OrderAns {
	public:
		OrderAns();
		OrderAns(unsigned char orderStatus, 
				 unsigned char direction,
				 const char* reqID,
				 const char* sessionID,
				 const char* orderID,
				 unsigned int groupID,
				 unsigned int clientID,
				 //unsigned int orderID,
				 unsigned int assetID,
				 long payOut,
				 int amount,
				 long openTime,
				 int openPrice,
				 int closePrice,
				 unsigned int sgID,
				 unsigned int teID);
		~OrderAns();
		OrderAns(const OrderAns& r);
		OrderAns& operator=(const OrderAns& r);
		OrderAns(OrderAns&& r);
		OrderAns& operator=(OrderAns&& r);
		
	public:
		const char* ToJson(void);

	public:
		/*OrderStatus
		  bit0: 1: openprice/ACK
		  bit1: 1: invalid
		  bit2: 1: WIN
		  bit3: 1: LOSS
		  bit4: 1: REJECT
		  */
		unsigned char OrderStatus; /* 1B */
		unsigned char Direction;	/* up or down*/
		char* ReqID;				/* Universal uniq 16B */
		char* SessionID;				/* Universal uniq 16B */
		char* OrderID;
		char* Json;
		unsigned int GroupID;		/* universal unique 4B*/
		unsigned int ClientID;		/* group unique 4B*/
		//unsigned int OrderID; /* server unique 4B */
		unsigned int AssetID;		/* instrument */
		long PayOut; /* the amount of payout 8B */
		int Amount;
		long OpenTime;	/* open Time(Serve Provide) 4B */
		int OpenPrice;	/* open Price(Serve Provide) 4B */
		int ClosePrice;	/* close Price(Serve Provide) 4B */
		unsigned int SGID;			/* solution gateway seq ID */
		unsigned int TEID;			/* solution gateway seq ID */
};

class PositionAccount;
/* 2B tag 0x0003 */
struct ValidateOrder {
	public:
		ValidateOrder();
		ValidateOrder(unsigned char boAccountID,
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
					  long profit);
		~ValidateOrder();
		ValidateOrder(const ValidateOrder& r);
		ValidateOrder& operator=(const ValidateOrder& r);
		ValidateOrder(ValidateOrder&& r);
		ValidateOrder& operator=(ValidateOrder&& r);
		explicit ValidateOrder(const PositionAccount& position);

	public:
		const char* ToJson(void);

	public:
		unsigned char BOAccountID; /* 1B */
		unsigned char Direction;	/* up or down*/
		/*OrderStatus
		  bit0: 1: openprice/ACK
		  bit1: 1: invalid
		  bit2: 1: WIN
		  bit3: 1: LOSS
		  bit4: 1: REJECT
		  */
		unsigned char OrderStatus; /* 1B */
		char* ReqID;				/* Universal uniq 16B */
		char* SessionID;				/* Universal uniq 16B */
		char* Json;
		short Status;
		int GroupID;		/* universal unique */
		int ClientID;		/* group unique */
		int AssetID;		/* instrument */
		int ExpiryPeriod;	/* time period eg. 5 second */
		int Amount;		/* amount of inverstment */
		int SGID;			/* solution gateway seq ID */
		unsigned long AccountID;	/*8B */
		long Margin;	/*8B */
		long FreeMargin;/*pcredit*/
		long Profit;
		long PrevBalance;
		long Changed;
		long CloseBalance;
};

/* RequestAck begin **/ 
struct RequestAck {
	public:
		RequestAck();
		RequestAck(unsigned char reqStatus, const char* reqID, unsigned int sgid_teid=0);
		~RequestAck();

		RequestAck(const RequestAck& r);
		RequestAck& operator=(const RequestAck& r);
		RequestAck(RequestAck&& r);
		RequestAck& operator=(RequestAck&& r);

	public:
		const char* ToJson(void);

		/*
		 RequestStatus 
		 create: 0 
		 bit0: ACK: 1, 
		 bint1: REJECT: 2
		 SG bit2: 0:初始值 1: 接收者/发送者
		 PM bit3: 0:初始值 1: 接收者/发送者
		 TE bit4: 0:初始值 1: 接收者/发送者
		 MT bit5: 0:初始值 1: 接收者/发送者
		 CM bit6: 0:初始值 1: 接收者/发送者
		 */
		unsigned char ReqStatus; 
		char* ReqID;				/* Universal uniq 16B */
		unsigned int SGID_TEID;	/* Dynamic */
		char* Json;
};
/* RequestAck end **/ 

class ValidateOrder;
/*** position management begin ***/
struct PositionAccount {
	public:
		PositionAccount();
		//PositionAccount(unsigned char reqStatus, const char* reqID, unsigned int sgid_teid=0);
		~PositionAccount();

		PositionAccount(const PositionAccount& r);
		PositionAccount& operator=(const PositionAccount& r);
		PositionAccount(PositionAccount&& r);
		PositionAccount& operator=(PositionAccount&& r);

		explicit PositionAccount(const ValidateOrder& validate);
		//explicit PositionAccount(ValidateOrder&& validate);

	public:
		const char* ToJson(void);
		bool VerifyData(int& status) const;

	public:
		char* Json;
		short Type;
		short Status;
		unsigned int GroupID;		/* universal unique */
		unsigned int ClientID;		/* group unique */
		int SGID;			/* sgid */
		int Check;
		int ErrMsgIndex;
		char* ReqID;				/* Universal uniq 16B */
		char* SessionID;				/* Universal uniq 16B */
		unsigned long AccountID;
		long Balance;	/*8B */
		long PrevBalance;
		long Changed;
		long Margin;
		long FreeMargin;
		long Profit;
		long CloseBalance;
};
/*** position management end ***/

/*** position management begin ***/
struct OCPosition {
	public:
		OCPosition();
		//OCPosition(unsigned char reqStatus, const char* reqID, unsigned int sgid_teid=0);
		~OCPosition();

		OCPosition(const OCPosition& r);
		OCPosition& operator=(const OCPosition& r);
		OCPosition(OCPosition&& r);
		OCPosition& operator=(OCPosition&& r);

		explicit OCPosition(const ValidateOrder& validate);
		//OCPosition& operator=(const ValidateOrder& validate);

	public:
		const char* ToJson(void);
		bool VerifyData(int& status) const;

	public:
		char* Json;
		short Type;
		short Status;
		unsigned int GroupID;		/* universal unique */
		unsigned int ClientID;		/* group unique */
		int Check;
		char* ReqID;				/* Universal uniq 16B */
		char* SessionID;				/* Universal uniq 16B */
		unsigned long AccountID;
		long Balance;	/*8B */
		long PrevBalance;
		long Changed;
		long Margin;
		long FreeMargin;
		long Profit;
		long CloseBalance;
};
/*** position management end ***/
//}

/***MonitorReq start***/
struct  MonitorReq {
public:
	MonitorReq();
	MonitorReq(char* reqID, 
			uint32_t mid, 
			uint64_t timeStamp);
	~MonitorReq();
	MonitorReq(const MonitorReq& r);
	MonitorReq& operator=(const MonitorReq& r);
	MonitorReq(MonitorReq&& r);
	MonitorReq& operator=(MonitorReq&& r);

public:
	const char* ToJson(void);

public:
	char* Json;
	char* ReqID; //[32]; //
	uint32_t Mid;
	uint64_t TimeStamp;//long
};

/***MonitorReq end***/

/***MonitorAns start***/
struct MonitorAns {
public:
	MonitorAns();
	MonitorAns(const char* reqID, 
			char status, 
			const char* name,
			uint32_t pid,
			uint32_t mid,
			uint64_t timeStamp,
			uint64_t startTimes);
	~MonitorAns();
	MonitorAns(const MonitorAns& r);
	MonitorAns& operator=(const MonitorAns& r);
	MonitorAns(MonitorAns&& r);
	MonitorAns& operator=(MonitorAns&& r);

public:
	const char* ToJson(void);

public:	
	char* Json;
	char* ReqID; //[32];
	char  Status;
//	cha	  pad[2] // don't worry about this for now
	char* Name; //[16];
	uint32_t 	Pid;
	uint32_t 	Mid;
	uint64_t	TimeStamp;
	uint64_t	StartTimes;	
};
/***MonitorAns end***/
// status { unknown=0, initiate,  booting,  running,  stopping,  stopped}

void InitDaemon();

#endif 
