#ifndef _CONST_H_
#define _CONST_H_

typedef unsigned char BYTE;


//通用返回错误
#define SUCESS 0
#define FAILE -1

//socket
#define ERR_SOCKET           -1000001
#define ERR_LISTEN           -1000002
#define ERR_BIND             -1000003
#define ERR_ACCEPT           -1000004
#define ERR_SELECT           -1000005
#define ERR_CONNECT          -1000006
#define ERR_READ             -1000007
#define ERR_WRITE            -1000008
#define ERR_SOCKOPT          -1000009
#define ERR_GETSOCKOPT       -1000010
#define ERR_EPOLLCTL         -1000011
#define ERR_SEND             -1000012
#define ERR_RECV             -1000013
#define ERR_FCNTL            -1000014
#define ERR_EPOLL_CTL        -1000015

//http 返回码
//#define ERR_OFFLINE          "-100101"  //用户不在线
#define ERR_SERVER           "500"  //服务器错误
#define ERR_OFFLINE          "220"  //用户不在线
#define ERR_LOGIN_OTHMACHINE "221"  //用户在别的机器登陆
//#define ERR_SERVICE          "-100201"  //格式出错

#define ERR_LIMIT_USER       "210"  //黑名单用户
#define USER_ONLINE_STATUS   "215"  //不显示在线人数
//threadPool
#define THREAD_NUM_DEF       100
#define THREAD_NUM_MAX       1000
#define THREAD_NUM_MIN       1

//对象池
#define SIZE_AUTO_EXPAND 0 ///自由增长
#define ARRAY_ALLOC_BASE 10 ///表示第一阶梯数组的块数是1024
#define MAX_ARRAY_INDEX 18 ///最大阶梯数

//线程状态
#define THREAD_RUNNING		1
#define THREAD_DIE			2
#define THREAD_FREE			0

//消息类型
#define MSG_TYPE_LEN        16
#define REQ_TYPE_LEN        3
#define SPLIT_LEN           1

//消息头
#define MAC_LEN            100
#define CRM_USER_LEN_LEN   2
#define CRM_USER_LEN       140
#define CRM_PWD_LEN        140
#define MT4_USER_LEN       140
#define MT4_PWD_LEN        140
#define ROUTE_LEN          3
#define SORCE_ID_LEN       15
#define SHM_KEY_LEN        15


//buff 长度
#define RCVBUFSZ            1024*32+1
#define SNDBUFSZ            1024*32+1
#define BUFSZ               1024*32+1

//buf 长度的长度
#define BUF_LEN_LEN         4 

#define M_TCP_LEN_PATH      120

#define MAX_CRM_API_COUNT 100


//App_token
#define APP_TOKEN_LEN      120
//Crm_token
#define CRM_TOKEN_LEN      500
//locserver
#define LOC_HEADER_LEN    4
#define LOC_BODY_LEN      9999
//数据库
#define DB_FLD_MAX        30 //列名最大长度
#define DB_COL_MAX        150 //最大列数

//日志级别
#define LEVEL_DEBUG         DEBUG
#define LEVEL_INFO          INFO
#define LEVEL_ERROR         ERROR


#define DEAULT_APP_IP "10.10.10.6"
//#define DEAULT_APP_IP "155.69.203.4"

//日志函数
//#define  LOG_DEBUG(...)     WriteLog("DEBUG", __FILE__,  __LINE__, __func__, __VA_ARGS__)
//#define  LOG_WARING(...)    WriteLog("WARING", __FILE__,  __LINE__, __func__, __VA_ARGS__)
//#define  LOG_INFO(...)      WriteLog("INFO", __FILE__,  __LINE__, __func__, __VA_ARGS__)
//#define  LOG_ERR(...)       WriteLog("ERR", __FILE__,  __LINE__, __func__, __VA_ARGS__)
//#define LOG_TRACE(format, ...) slog_trace(PRE_STR format VAL_STR ,##__VA_ARGS__)
//#define SLOG_TRACE(format, ...) slog_trace(PRE_STR format VAL_STR ,##__VA_ARGS__)
 
//#define LOG_DEBUG(format, ...)  SLOG_DEBUG(format, ...)
//#define LOG_INFO(format, ...)  SLOG_INFO(format, ...)
//#define LOG_WARING(format, ...)  SLOG_WARN(format, ...)
//#define LOG_ERR(format, ...)  SLOG_ERROR(format, ...)
//接口指令
#define CMD_LEN         	   3 

//CRM指令
#define CMD_REGISTER_CRM       "101" //注册
#define CMD_REGISTER_TPL_CRM   "102" //注册
#define CMD_LOGIN_CRM          "103" //登陆
#define CMD_LOGIN_TPL_CRM      "104" //登陆
#define CMD_DEPOSIT_CRM        "105" //入金
#define CMD_DEPOSIT_TPL_CRM    "106" //入金
#define CMD_WITHDRAWALS_CRM    "107" // 出金
#define CMD_WITHDRAWALS_TPL_CRM "108" // 出金
#define CMD_FORPWD_CRM         "109" //忘记密码
#define CMD_FORPWD_TPL_CRM     "110" //忘记密码
#define CMD_WITHDRAW_HISTORY_CRM "111" // 出金历史记录
#define CMD_WITHDRAW_DEL_CRM   "112" // 删除出金历史记录
#define CMD_DEPOSIT_STATUS_CRM  "113" //入金
#define CMD_DEPOSIT_UPLOAD_CRM  "114" //入金
#define CMD_PROFILE_DATA        "115" //获取用户信息


#define CMD_GETACCINFO_BO      "201" //获取MT4账号信息
#define CMD_AUTH_BO            "202" //账号信息验证
#define CMD_HISDATA_BO         "203" //获取历史行情 
#define CMD_TRDHIS_BO          "204" //获取历史记录

//APP指令
#define CMD_LOGIN_APP           "301"
#define CMD_DEPOSIT_APP         "302" //入金
#define CMD_WITHDRAWALS_APP     "303" //出金


//特殊消息类型 
#define MSG_CRM_TOKEN_EXTENDED  "0999999999999999"


//网址
//#define URL_CRM_HY             "https://hy.starfishfx.com"
//#define URL_CRM_MY             "https://my.starfishfx.com"
//#define URL_CRM_TEST           "https://pt.starfishfx.com"   //测试环境
//#define URL_CRM_TEST           "https://my.starfishfx.com"   //生成环境
//#define URL_BO               "https://mtemp.starfishfx.com" //测试
//#define URL_BO_MY              "http://119.81.24.229:5999"
//#define URL_BO_HY              "http://119.81.24.229:7333"


//HTTP
#define TAG_HTTPOK        "200 OK"
#define TAG_URL           "URL:"
#define TAG_MY            "my"
#define TAG_HY            "hy"
#define TAG_SND           "SNDTAG:"
#define TAG_POST          "POSTTAG:"
#define TAG_SPLIT         "|"
#define TAG_COM           ".com"
#define TAG_APP           "/App"
#define TAG_HTTP          "HTTP"
#define TAG_HTTP11        "HTTP/1.1 "
#define TAG_HTTP10        "HTTP/1.0 "
#define TAG_RSPCODE       "RSPCODE:"
#define TAG_RSPMSG        "RSPMSG:"
#define TAG_BODY          "{\"msg\""
#define TAG_MSG           "msg"
#define TAG_BODY_LOGIN    "{\""
#define TAG_MT4ACC        "mt4account="
#define TAG_MT4ACC_HIS    "mt4account:"
#define TAG_USERNAME      "username="
#define TAG_BODY_START    "{"
#define TAG_CONT_LEN      "Content-Length:"
#define TAG_GRAN_TYPE     "grant_type="
#define TAG_COUNT         "count="
#define TAG_START         "start="
#define TAG_END           "end="
#define TAG_TIME_FRAME    "timeframe="
#define TAG_CURR_FRAME    "currexchange="
#define TAG_EMAIL         "email="
#define TAG_RESULT        "result"
#define TAG_IS_NULL       "isnull"
#define TAG_DATA          "data"
#define TAG_DESCRIPTION   "description"
#define TAG_APPVER        "appver"
#define TAG_DESCEN		  "desc_en"
#define TAG_DESCCN		  "desc_cn"
#define TAG_FILENAME      "filename="
#define TAG_TEXT          "text="
#define TAG_PWD           "password="
#define TAG_MAC           "macaddress="
#define TAG_SER_PRO_EN    "serproen="
#define TAG_SER_PRO_CN    "serprocn="
#define TAG_PAGE          "page="
#define TAG_PAGE_LEN      "page-length="
#define TAG_SORT          "sort="
#define TAG_ACCESS_TOKEN  "access_token="
#define TAG_APP_TOKEN     "apptoken="
#define TAG_APP_TOKEN_HEAD "apptoken:"
#define TAG_CRM_TOKEN     "crmtoken="
#define TAG_MT4_TOKEN     "mt4token="
#define TAG_ENCOD         "Transfer-Encoding:"
#define TAG_VERSION       "appversion="
#define TAG_SKIN_VER      "skinversion="
#define TAG_SYMBOL_VER    "symbolversion="
#define TAG_ISIOS         "isios="
#define TAG_lable         ",\"label\":\""
#define TAG_CONT_TYPE     "Content-Type: application/json; charset=UTF-8\r\n"
#define TAG_REQUESTID     "requestid="
#define TAG_GETWAYID      "getwayid="
#define TAG_GET           "GET "
#define TAG_NATIONAL_ID	  "national_id="
//#define TAG_NATIONAL_ID	  "nationalid="
#define TAG_LANG	      "lang="
#define TAG_SORCE_ID      "sourceid"
#define TAG_SORCE_ID_GET  "sourceid="
#define TAG_SORCE_ID_POST "sourceid:"
#define TAG_IPHONE        "iPhone:"
#define TAG_LANGUAGE_HEAD "Accept-Language:"
#define TAG_LANGUAGE_HEAD_CRM "lang:"
#define TAG_LANGUAGE      "langue="
#define TAG_LANGUAGE_CRM_ZH "zh"
#define TAG_LANGUAGE_CRM_EN "en"
#define TAG_ZHNAME        "zh_name="
#define TAG_HIS_DATETIME  "datetime"
#define TAG_HIS_OPEN      "open"
#define TAG_HIS_HIGH      "high"
#define TAG_HIS_LOW       "low"
#define TAG_HIS_CLOSE     "close"
#define TAG_HIS_IP        "IPAddress="

#define TAG_HIS_INFO      "info"
#define TAG_HIS_BARS      "bars"
#define TAG_HIS_SYMBOL    "symbol"
#define TAG_HIS_TIMEFRAME "timeframe"
#define TAG_HIS_START     "start"
#define TAG_HIS_END       "end"
#define TAG_USER_AGENT_HEAD "USER-AGENT:"
#define TAG_IPHONE_HEAD  "iPhone"

#define TAG_HIS_DATETIME "datetime"
#define TAG_HIS_OPEN     "open"
#define TAG_HIS_HIGH     "high"
#define TAG_HIS_LOW      "low"
#define TAG_HIS_CLOSE    "close"

#define TAG_MERCHANTKEY_GET  "MerchantKey="
#define TAG_MERCHANTKEY_POST "MerchantKey:"
#define TAG_DOMAIN_GET      "Domain="
#define TAG_DOMAIN_POST      "Domain:"
#define TAG_APPIP_GET        "AppIP="
#define TAG_APPIP_POST       "AppIP:"
#define TAG_CLIENT_IP        "HTTP_CLIENT_IP"

#define USER_NAME_CRM          "/api/v1/user/zh-name"
#define USER_SETNAME_CRM       "/api/v1/profile/zh-name"
#define USER_PROFILE_DATA      "/api/v1/profile"
#define USER_AUTH_CRM          "/auth"
#define USER_LOGOUT_BO         "/logout"
#define USER_LOGIN_CRM         "/api/v1/login"				// 用户登录
#define USER_REGISTER_CRM      "/api/v1/register"			// 用户注册
#define USER_REGISTER_TMP_CRM  "/get/tmp/register"			// 用户注册
#define USER_GETHIS_TIME_CRM   "/history/"					// 获取历史行情
#define USER_GETHIS_COUNT_CRM  "/history/last/"				// 获取历史行情
#define USER_GETTRDHIS__CRM    "/api/v1/mt4/trade"			// 获取历史记录
#define USER_GETARCHTRA__CRM   "/api/v1/mt4/archive-trade"  // 获取存档历史记录
#define USER_FORPWD_CRM        "/api/v1/forgot-password"	// 忘记密码

//#define USER_GETWITHDRAW_CRM_TMP   "/api/v1/withdraw/funds"		// 出金
#define USER_GETWITHDRAW_CRM_TMP   "/api/v1/withdraw/get-withdraw"
#define USER_GETWITHDRAW_CRM   "/api/v1/withdraw/fund"		// 出金
#define USER_WITHDRAW_REQ_CRM  "/api/v1/withdraw/request"	// 出金记录

#define USER_GETDEPOSIT_REQ_CRM   "/api/v1/deposit/gateways"	// 入金模板
#define USER_DEPOSIT_STATUS_REQ_CRM	"/api/deposit/status"		// 入金状态
#define USER_DEPOSIT_UPLOAD_REQ_CRM	"/api/v1/deposit/upload"	// 银行电汇上传文件接口

#define USER_LOGIN_APP         "/login"

#define METHOD_POST     "POST /crm/auth/register HTTP/1.1\r\n"
#define METHOD_GET      "GET / HTTP/1.1\r\n"
#define FLAG_GET        1
#define FLAG_POST       2

#define HISMARKET "historicalmarket"
#define SPACETIME 259200
#define SLEEPTIME 86400
// online users 
#define ONLINE_USERS "online_users"
#define ONLINE_COMPANY "online_company"
//获取偏移量
#define OFFSET_OF(type, member) (unsigned long)(&(((type *)0)->member)) 


//定义新结构体
#define M_NVAR(vd, pst, st) \
	do \
{ \
	st *pst;\
	pst = (st*)vd;\
} while(0);


#define Str(x) #x 
#define Xstr(x) Str(x) 
#define OP plus 


#define DM_MAX_ARGC 10;


//判断一个条件exp，执行动作stmt，然后返回ret 
#define M_XRETURN(exp, ret, stmt) \
	if (exp) \
{ \
	stmt; \
	return (ret); \
}
#define M_VRETURN(exp, stmt) \
	if (exp) \
{ \
	stmt; \
	return; \
}
#define M_VRETURNP(exp, stmt) \
	if (exp) \
{ \
	stmt; \
	return NULL; \
}
#define M_XGOTO(exp, label, stmt) \
	if (exp) \
{ \
	stmt; \
	goto label; \
}
#define M_XBREAK(exp, stmt) \
	if (exp) \
{ \
	stmt; \
	break; \
}
#define M_XCONTINUE(exp, stmt) \
	if (exp) \
{ \
	stmt; \
	continue; \
}
#define M_XWARN(exp, stmt) \
	if (exp) \
{ \
	stmt; \
}

#define M_ZRETURN(exp, ret) \
	if (exp) \
{ \
	return (ret); \
}
#define M_YRETURN(exp) \
	if (exp) \
{ \
	return; \
}
#define M_ZGOTO(exp, label) \
	if (exp) \
{ \
	goto label; \
}
#define M_ZBREAK(exp) \
	if (exp) \
{ \
	break; \
}
#define M_ZCONTINUE(exp) \
	if (exp) \
{ \
	continue; \
}

#endif
