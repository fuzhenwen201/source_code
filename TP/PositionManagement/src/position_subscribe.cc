/*************************************************************************
	> File Name: position_subscribe.cc
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Tue 28 Feb 2017 07:22:33 PM CST
 ************************************************************************/
#include<mutex>
#include<future>
#include<string.h>
#include<type_traits>
#include"position_subscribe.h"
#include"tnode_adapter.h"
#include"ThreadPool.h"
#include<uuid/uuid.h>
#include<rapidjson/document.h>
#include<rapidjson/writer.h>
#include<rapidjson/rapidjson.h>
#include<rapidjson/stringbuffer.h>

#include"position_list.h"
#include"mysqlop.h"
#include"object_pool.h"
#include"redisop.h"
#include"solution_config.h"
#include"xdatetime.h"
#include"publish_exception.h"
#include"concurrentqueue.h"

using namespace snetwork_xservice_xflagger;
extern ThreadPool* g_threadPool;
using MySqlRecordSet = snetwork_xservice_db::MySqlRecordSet;
using MySqlDB = snetwork_xservice_db::MySqlDB;
using RedisPool = ObjectPool<snetwork_xservice_db::Redis, 5>;
extern RedisPool* g_redisPool;
SafeList<PositionListNode> g_position_list;

using namespace moodycamel;
extern ConcurrentQueue<std::string> g_concurrent_send_order_list; 
extern ConcurrentQueue<std::string> g_concurrent_validate_order_list; 
extern ConcurrentQueue<std::string> g_concurrent_order_ans_list; 

/*SubscribeSendOrder begin ****/
regex_t SubscribeSendOrder::re;

int SubscribeSendOrder::StaticInit(void) {
	int ret = 0;
	char errmsg[1024];

	if ((ret = regcomp(&re, SENDORDER_REGEX, REG_EXTENDED)) != 0) {
		regerror(ret, &re, errmsg, 1023);
		errmsg[1023] = 0;
		fprintf(stderr, "%s", errmsg);
	}

	return ret;
}

SubscribeSendOrder::SubscribeSendOrder(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeSendOrder::Done(const char* event, unsigned int eventLen) {
	long recTime = GetMicroSecond();
#if defined(DEBUG)
	XINFO("SendOrder Comsumer	msg:%s, len:%d\n", event, eventLen);
#endif
	/* 1. parse json => sendorder */
	rapidjson::Document d;
	d.Parse(event, eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s",event);

		return;
	}

	rapidjson::Value::ConstMemberIterator it;

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();

	if(((it = d.FindMember("sessionid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* sessionid = it->value.GetString();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int sgid = it->value.GetInt();
	char sgidTmp[10];
	sprintf(sgidTmp, "%d", sgid);

  	if(((it = d.FindMember("groupid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int groupid = it->value.GetInt();

  	if(((it = d.FindMember("clientid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int clientid = it->value.GetInt();

  	if(((it = d.FindMember("assetid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int assetid = it->value.GetInt();

  	if(((it = d.FindMember("amount")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int amount = it->value.GetInt();

  	if(((it = d.FindMember("direction")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int direction = it->value.GetInt();

  	if(((it = d.FindMember("expiryperiod")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int expiryperiod = it->value.GetInt();

	char* sqlbuf = nullptr;
#if 0
	CDateTime dateTime;
	char* tmpDay = dateTime.AddDay(0);
	int day = atoi(tmpDay);
	FREE(&tmpDay);
	
	asprintf(&sqlbuf, PM_SEND_ORDER_FORMAT, 
			 day,
			 reqid,
			 sessionid,
			 groupid,
			 clientid,
			 assetid,
			 direction,
			 expiryperiod,
			 amount,
			 sgid,
			 0,
			 recTime);
	
	g_threadPool->Enqueue([sqlbuf] {
		snetwork_xservice_db::MySqlDB db;
		db.Open();
		db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

		ssize_t rows = db.ExecSql(sqlbuf);
		db.Close();

		free(sqlbuf);
	});
	sqlbuf = nullptr;
#endif

	/*publish ACK: USER.SGID */
	RequestAck ack;
	memcpy(ack.ReqID, reqid, REQUEST_ID_LEN);
	ack.ReqStatus = (ACK_BIT_FLAG | PM_BIT_FLAG);
	const char* msg_ack = ack.ToJson();
#if defined(DEBUG)
	XINFO("publish ACK|PM->SG=%s", msg_ack);
#endif
	TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_USER,
			sgidTmp,
			msg_ack, 
			strlen(msg_ack)+1, 
			ETag::EREQUESTACK);
	
	std::string sendOrderNode;
	sendOrderNode.append("('").append(reqid).append("','").append(sessionid).append("',").append(std::to_string(groupid)).append(",").append(std::to_string(clientid)).append(",").append(std::to_string(assetid)).append(",").append(std::to_string(direction)).append(",").append(std::to_string(expiryperiod)).append(",").append(std::to_string(amount)).append(",").append(sgidTmp).append(",0,").append(std::to_string(recTime)).append("),");
	g_concurrent_send_order_list.enqueue(std::move(sendOrderNode));

	/* 2. query validate */
	/* 2.1 first query redis */
	snetwork_xservice_db::Redis* redis = g_redisPool->GetObject();
	char key[64];
	sprintf(key,"%s_%d_%d", HKEYVALIDATE.c_str(), groupid, clientid);
	std::string margin = std::move(redis->HGet(key, "margin"));
	std::string freeMarginTmp = std::move(redis->HGet(key, "freemargin"));
	std::string profit = std::move(redis->HGet(key, "profit"));
	std::string status = std::move(redis->HGet(key, "status"));
	std::string prevBalance = std::move(redis->HGet(key, "prevbalance"));
	std::string changed = std::move(redis->HGet(key, "changed"));
	std::string closeBalance = std::move(redis->HGet(key, "closebalance"));
	g_redisPool->ReleaseObject(redis);
	MySqlRecordSet rs_position;
	if (freeMarginTmp.empty()) {
		/*2.2 redis no record, query mysql */
		MySqlDB db;
		db.Open();
		db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

		asprintf(&sqlbuf, SELECT_VALID_FORMAT, groupid, clientid);
		rs_position = db.QuerySql(sqlbuf);
		if (db.IsError()) {
			XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
			db.Close();
		} else {
			db.Close();
		}

		size_t rows = rs_position.GetRows();
		if (rows == 0) {
			XERROR("Empty result|sql=%s", sqlbuf);
			FREE(&sqlbuf);

			return;
		} else {
			FREE(&sqlbuf);
			/* Query freemargin from mysql, and compare with sendorder.amount */
			margin = rs_position.GetFieldByName(0, "margin");
			freeMarginTmp = rs_position.GetFieldByName(0, "freemargin");
			profit = rs_position.GetFieldByName(0, "profit");
			status = rs_position.GetFieldByName(0, "status");
			prevBalance = rs_position.GetFieldByName(0, "prevbalance");
			changed = rs_position.GetFieldByName(0, "changed");
			closeBalance = rs_position.GetFieldByName(0, "closebalance");
		}
	}
	long freemargin = atol(freeMarginTmp.c_str());

	ValidateOrder validateOrder;
	memcpy(validateOrder.ReqID, reqid, REQUEST_ID_LEN);
	memcpy(validateOrder.SessionID, sessionid, SESSION_ID_LEN);
	validateOrder.GroupID = groupid;
	validateOrder.ClientID = clientid;
	validateOrder.AssetID = assetid;
	validateOrder.Direction = direction;
	validateOrder.ExpiryPeriod = expiryperiod;
	validateOrder.Amount = amount;
	validateOrder.SGID = sgid;
	validateOrder.FreeMargin = freemargin;
	validateOrder.Margin = atol(margin.c_str());
	validateOrder.Profit = atol(profit.c_str());
	validateOrder.Status = atoi(status.c_str());
	validateOrder.PrevBalance = atol(prevBalance.c_str());
	validateOrder.Changed = atoi(changed.c_str());
	validateOrder.CloseBalance = atol(closeBalance.c_str());

	/*ret set recTime for publish validate */
	recTime = GetMicroSecond();
	/* 3.1 if validata is invalid, publish validate, valid
	 * and publish orderans to SG */
	if (freemargin < amount) {
		//validateOrder.OrderStatus = ORDER_STATUS_INVALID ;
		validateOrder.OrderStatus = ORDER_STATUS_INVALID|ORDER_STATUS_CLOSE;

#if 0
		CDateTime dateTime;
		char* tmpDay = dateTime.AddDay(0);
		int day = atoi(tmpDay);
		//char* tmpTime = dateTime.GetDateTime(CDateTime::EDATETIME);
		FREE(&tmpDay);

		//const char* msg_validate = validateOrder.ToJson();
		asprintf(&sqlbuf, PM_ORDER_ANS_FORMAT,
				 day,
				 reqid,
				 sessionid,
				 groupid,
				 clientid,
				 0, /*orderid*/
				 ORDER_STATUS_INVALID, /*orderstatus*/
				 assetid, /*assetid*/
				 direction,
				 amount,
				 0, /* payout */
				 0, /* opentime */
				 0, /* openprice */
				 0, /* closeprice */
				 ORDER_ANS_PASSIVE_PMSG, /* passive */
				 sgid, /* sgid */
				 0, /* teid */
				 recTime);

		g_threadPool->Enqueue([sqlbuf] {
			snetwork_xservice_db::MySqlDB db;
			db.Open();
			db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

			ssize_t rows = db.ExecSql(sqlbuf);
			db.Close();

			free(sqlbuf);
		});
#endif
		const char* msg_validate = validateOrder.ToJson();
		std::string orderAnsNode;
		//orderAnsNode.append("('").append(reqid).append("','").append(sessionid).append("',").append(std::to_string(groupid)).append(",").append(std::to_string(clientid)).append(",0,").append(std::to_string(ORDER_STATUS_INVALID|ORDER_STATUS_CLOSE)).append(",").append(std::to_string(assetid)).append(",").append(std::to_string(direction)).append(",").append(std::to_string(amount)).append(",0,0,0,0,").append(std::to_string(ORDER_ANS_PASSIVE_PMSG)).append(",").append(sgidTmp).append(",0,").append(std::to_string(recTime)).append("),");
		/* temp: orderid as string */
		OrderAns orderAns;
		uuid_t uuid;
		uuid_generate_time_safe(uuid);
		DecToHex(orderAns.OrderID, (char*)&uuid);
		orderAns.OpenTime = GetMicroSecond();
		orderAns.OrderStatus = ORDER_STATUS_INVALID|ORDER_STATUS_CLOSE;
		memcpy(orderAns.ReqID, reqid, REQUEST_ID_LEN);
		memcpy(orderAns.SessionID, sessionid, SESSION_ID_LEN);
		orderAns.GroupID = groupid;
		orderAns.ClientID = clientid;
		orderAns.AssetID = assetid;
		orderAns.Direction = direction;
		orderAns.Amount = amount;
		orderAns.SGID = sgid;
		orderAnsNode.append("('").append(orderAns.ReqID).append("','").append(orderAns.SessionID).append("',").append(std::to_string(orderAns.GroupID)).append(",").append(std::to_string(orderAns.ClientID)).append(",'").append(orderAns.OrderID).append("',").append(std::to_string(orderAns.OrderStatus)).append(",").append(std::to_string(orderAns.AssetID)).append(",").append(std::to_string(orderAns.Direction)).append(",").append(std::to_string(orderAns.Amount)).append(",0,").append(std::to_string(orderAns.OpenTime)).append(",0,0,0,").append(std::to_string(ORDER_ANS_PASSIVE_PMSG)).append(",").append(sgidTmp).append(",0,").append(std::to_string(recTime)).append("),");
		g_concurrent_order_ans_list.enqueue(std::move(orderAnsNode));

#if 0
		/*publish validate.sgid */
		TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_VALID,
													   "1",
													   msg_validate, 
													   strlen(msg_validate)+1, 
													   ETag::EVALIDATEORDER);
		XINFO("publish validate.sgid|PM->SG=%s", msg_validate);
#endif
#if 0
		asprintf(&sqlbuf, PM_VALIDATE_ORDER_FORMAT,
				 day,    /*table name*/
				 reqid,	
				 sessionid,
				 groupid,
				 clientid,
				 0,			/* accountid */
				 0,			/* boaccountid */
				 assetid,			/* assetid */
				 direction,	/* direction */
				 expiryperiod,
				 amount,
				 validateOrder.OrderStatus, /*orderstatus*/
				 validateOrder.Margin, /*margin*/
				 validateOrder.FreeMargin, /*pcredit*/
				 validateOrder.Profit,
				 sgid,
				 0,		/*teid*/
				 recTime);

		g_threadPool->Enqueue([sqlbuf] {
			snetwork_xservice_db::MySqlDB db;
			db.Open();
			db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

			ssize_t rows = db.ExecSql(sqlbuf);
			db.Close();

			free(sqlbuf);
		});
		sqlbuf = nullptr;
#endif

		/*publish validate.ALLTE */
		TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_VALIDATE,
				//POSITIONMANAGEMENT_PUBLISH_VALIDATE_ALL_EVENT, 
				POSITIONMANAGEMENT_PUBLISH_VALIDATE_ALLTE_EVENT, 
				msg_validate, 
				strlen(msg_validate)+1, 
				ETag::EVALIDATEORDER);
#if defined(DEBUG)
		XINFO("publish VALIDATE.$ |PM->TE=%s", msg_validate);
#endif

		std::string validateOrderNode;
		validateOrderNode.append("('").append(reqid).append("','").append(sessionid).append("',").append(std::to_string(groupid)).append(",").append(std::to_string(clientid)).append(",0,0,").append(std::to_string(assetid)).append(",").append(std::to_string(direction)).append(",").append(std::to_string(expiryperiod)).append(",").append(std::to_string(amount)).append(",").append(std::to_string(validateOrder.OrderStatus)).append(",").append(std::to_string(validateOrder.Margin)).append(",").append(std::to_string(validateOrder.FreeMargin)).append(",").append(std::to_string(validateOrder.Profit)).append(",").append(sgidTmp).append(",0,").append(std::to_string(recTime)).append("),");
		g_concurrent_validate_order_list.enqueue(std::move(validateOrderNode));

#if 0
		OrderAns orderAns;
		memcpy(orderAns.ReqID, reqid, REQUEST_ID_LEN);
		memcpy(orderAns.SessionID, sessionid, SESSION_ID_LEN);
		orderAns.GroupID = groupid;
		orderAns.ClientID = clientid;
		orderAns.AssetID = assetid;
		orderAns.Direction = direction;
		orderAns.Amount = amount;
		orderAns.SGID = sgid;
		//orderAns.OrderStatus = ORDER_STATUS_INVALID;
		//orderAns.OrderStatus = ORDER_STATUS_INVALID|ORDER_STATUS_CLOSE;
#endif

		/*publish ordAns.sgid */
		char orderAnsEvent[ORDERANS_RM_SGID_LEN_EVENT];
		sprintf(orderAnsEvent, "%s.%s", POSITIONMANAGEMENT_PUBLISH_ORDERANS_RM_EVENT.c_str(), sgidTmp);
		TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_ORDERANS,
				//sgidTmp,
				orderAnsEvent,
				orderAns.ToJson(), 
				strlen(orderAns.ToJson())+1, 
				ETag::EORDERANS);
		
#if defined(DEBUG)
		XINFO("publish ORDERANS.SGID|PM->SG=%s", orderAns.ToJson());
#endif
	} else {
	/* 3.2 if validata is valid, pubulish validata and valid,
	 * then wait orderans(for openprice), 
	 * and wait untill the close pirce of orderans*/
		validateOrder.OrderStatus = 0;
		//validateOrder.Balance = atol(balance.c_str());
		asprintf(&sqlbuf, POSITION_UPDATE_VALID_FORMAT, amount, amount, groupid, clientid);
#if !defined(MULTIPLE_THREAD)
		g_threadPool->Enqueue([sqlbuf] {
#endif
			snetwork_xservice_db::MySqlDB db;
			db.Open();
			db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

			db.StartTransaction();
			ssize_t rows = db.ExecSql(sqlbuf);
			if (rows == -1) {
				XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
			}
			db.Commint();
			db.Close();

			free(sqlbuf);
#if !defined(MULTIPLE_THREAD)
		});
#endif
		sqlbuf = nullptr;

#if 0
		CDateTime dateTime;
		char* tmpDay = dateTime.AddDay(0);
		int day = atoi(tmpDay);
		//char* tmpTime = dateTime.GetDateTime(CDateTime::EDATETIME);
		FREE(&tmpDay);

		asprintf(&sqlbuf, PM_VALIDATE_ORDER_FORMAT,
				 day,    /*table name*/
				 reqid,	
				 sessionid,
				 groupid,
				 clientid,
				 0,			/* accountid */
				 0,			/* boaccountid */
				 assetid,			/* assetid */
				 direction,	/* direction */
				 expiryperiod,
				 amount,
				 0, /*orderstatus*/
				 validateOrder.Margin, /*margin*/
				 validateOrder.FreeMargin, /*pcredit*/
				 validateOrder.Profit,
				 sgid,
				 0,		/*teid*/
				 recTime);
				 //GetMicroSecond());

		/*modify database freemargin-amount, and lock this amount: margin+amount*/
		g_threadPool->Enqueue([sqlbuf] {
			snetwork_xservice_db::MySqlDB db;
			db.Open();
			db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

			ssize_t rows = db.ExecSql(sqlbuf);
			db.Close();

			free(sqlbuf);
		});
		sqlbuf = nullptr;
#endif
		validateOrder.FreeMargin -= validateOrder.Amount;
		validateOrder.Margin += validateOrder.Amount;

		std::string validateOrderNode;
		validateOrderNode.append("('").append(reqid).append("','").append(sessionid).append("',").append(std::to_string(groupid)).append(",").append(std::to_string(clientid)).append(",0,0,").append(std::to_string(assetid)).append(",").append(std::to_string(direction)).append(",").append(std::to_string(expiryperiod)).append(",").append(std::to_string(amount)).append(",0,").append(std::to_string(validateOrder.Margin)).append(",").append(std::to_string(validateOrder.FreeMargin)).append(",").append(std::to_string(validateOrder.Profit)).append(",").append(sgidTmp).append(",0,").append(std::to_string(recTime)).append("),");
		g_concurrent_validate_order_list.enqueue(std::move(validateOrderNode));

		const char* msg_validate = validateOrder.ToJson();
#if 0
		/*publish valid.sgid */
		TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_VALID,
				sgidTmp,
				msg_validate, 
				strlen(msg_validate)+1, 
				ETag::EVALIDATEORDER);

#if defined(DEBUG)
		XINFO("publish valid.sgid|PM->SG=%s", msg_validate);
#endif

		/*publish validate.$*/
		TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_VALIDATE,
				POSITIONMANAGEMENT_PUBLISH_VALIDATE_ALL_EVENT, 
				msg_validate, 
				strlen(msg_validate)+1, 
				ETag::EVALIDATEORDER);
#if defined(DEBUG)
		XINFO("publish validate.$|PM->TE=%s", msg_validate);
#endif
#else
		/*publish validate.ALLTE.SGID */
		char validateEvent[VALIDATE_ALLTE_SGID_LEN_EVENT];
		//memset(validateEvent, 0, sizeof(validateEvent));
		sprintf(validateEvent, "%s.%s", POSITIONMANAGEMENT_PUBLISH_VALIDATE_ALLTE_EVENT.c_str(), sgidTmp);
		TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_VALIDATE,
				validateEvent, 
				msg_validate, 
				strlen(msg_validate)+1, 
				ETag::EVALIDATEORDER);
#if defined(DEBUG)
		XINFO("publish validate.%s|PM->TE|SG=%s", validateEvent, msg_validate);
#endif
#endif
	}
}

/* parse by regex */
void SubscribeSendOrder::Done(char* event, unsigned int eventLen) {
	long recTime = GetMicroSecond();
#if defined(DEBUG)
	XINFO("SendOrder Comsumer	msg:%s, len:%d\n", event, eventLen);
#endif
	thread_local regmatch_t rm[SENDORDER_FIELDS];

	int ret = regexec(&re, event, SENDORDER_FIELDS, rm, 0);
	if (ret != 0) {
		XERROR("format error, len=%zu|%s\n", eventLen, event);

		return;
	}
	char* sqlbuf = nullptr;

	event[rm[1].rm_eo] = 0;
	event[rm[2].rm_eo] = 0;
	event[rm[3].rm_eo] = 0;
	event[rm[4].rm_eo] = 0;
	event[rm[5].rm_eo] = 0;
	event[rm[6].rm_eo] = 0;
	event[rm[7].rm_eo] = 0;
	event[rm[8].rm_eo] = 0;
	event[rm[9].rm_eo] = 0;
	const char* reqid = &event[rm[1].rm_so];			/* reqid */
	const char* groupid =&event[rm[2].rm_so];		/* groupid */
	const char* clientid = &event[rm[3].rm_so];		/* clientid */
	const char* assetid = &event[rm[4].rm_so];		/* assetid */
	const char* direction = &event[rm[5].rm_so];		/* direction */
	const char* expiryperiod = &event[rm[6].rm_so];	/*expiryperiod */
	const char* amount = &event[rm[7].rm_so];		/* amount */
	const char* sessionid = &event[rm[8].rm_so];			/* sessionid */
	const char* sgid = &event[rm[9].rm_so];	/* sgid */

	/*publish ACK: USER.SGID */
	RequestAck ack;
	memcpy(ack.ReqID, reqid, REQUEST_ID_LEN);
	ack.ReqStatus = (ACK_BIT_FLAG | PM_BIT_FLAG);
	const char* msg_ack = ack.ToJson();
	TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_USER,
			sgid,
			msg_ack, 
			strlen(msg_ack)+1, 
			ETag::EREQUESTACK);
#if defined(DEBUG)
	XINFO("publish ACK|PM->SG=%s", msg_ack);
#endif
	
	std::string sendOrderNode;
	sendOrderNode.append("('").append(reqid).append("','").append(sessionid).append("',").append(groupid).append(",").append(clientid).append(",").append(assetid).append(",").append(direction).append(",").append(expiryperiod).append(",").append(amount).append(",").append(sgid).append(",0,").append(std::to_string(recTime)).append("),");
	g_concurrent_send_order_list.enqueue(std::move(sendOrderNode));

#if 1
	/* 2. query validate */
	/* 2.1 first query redis */
	snetwork_xservice_db::Redis* redis = g_redisPool->GetObject();
	char key[64];
	//sprintf(key,"%s_%d_%d", HKEYVALIDATE, groupid, clientid);
	sprintf(key,"%s_%s_%s", HKEYVALIDATE.c_str(), groupid, clientid);
	std::string margin = std::move(redis->HGet(key, "margin"));
	std::string freeMarginTmp = std::move(redis->HGet(key, "freemargin"));
	std::string profit = std::move(redis->HGet(key, "profit"));
	std::string status = std::move(redis->HGet(key, "status"));
	std::string prevBalance = std::move(redis->HGet(key, "prevbalance"));
	std::string changed = std::move(redis->HGet(key, "changed"));
	std::string closeBalance = std::move(redis->HGet(key, "closebalance"));
	g_redisPool->ReleaseObject(redis);
	MySqlRecordSet rs_position;
	if (freeMarginTmp.empty()) {
		/*2.2 redis no record, query mysql */
		MySqlDB db;
		db.Open();
		db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

		//asprintf(&sqlbuf, SELECT_VALID_FORMAT, groupid, clientid);
		asprintf(&sqlbuf, SELECT_VALID_FORMAT_STR, groupid, clientid);
		rs_position = db.QuerySql(sqlbuf);
		if (db.IsError()) {
			XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
			db.Close();
		} else {
			db.Close();
		}

		size_t rows = rs_position.GetRows();
		if (rows == 0) {
			XERROR("Empty result|sql=%s", sqlbuf);
			FREE(&sqlbuf);

			return;
		} else {
			FREE(&sqlbuf);
			/* Query freemargin from mysql, and compare with sendorder.amount */
			margin = rs_position.GetFieldByName(0, "margin");
			freeMarginTmp = rs_position.GetFieldByName(0, "freemargin");
			profit = rs_position.GetFieldByName(0, "profit");
			status = rs_position.GetFieldByName(0, "status");
			prevBalance = rs_position.GetFieldByName(0, "prevbalance");
			changed = rs_position.GetFieldByName(0, "changed");
			closeBalance = rs_position.GetFieldByName(0, "closebalance");
		}
	}
	long freemargin = atol(freeMarginTmp.c_str());

	ValidateOrder validateOrder;
	memcpy(validateOrder.ReqID, reqid, REQUEST_ID_LEN);
	memcpy(validateOrder.SessionID, sessionid, SESSION_ID_LEN);
	validateOrder.GroupID = atoi(groupid);
	validateOrder.ClientID = atoi(clientid);
	validateOrder.AssetID = atoi(assetid);
	validateOrder.Direction = atoi(direction);
	validateOrder.ExpiryPeriod = atoi(expiryperiod);
	validateOrder.Amount = atoi(amount);
	validateOrder.SGID = atoi(sgid);
	validateOrder.FreeMargin = freemargin;
	validateOrder.Margin = atol(margin.c_str());
	validateOrder.Profit = atol(profit.c_str());
	validateOrder.Status = atoi(status.c_str());
	validateOrder.PrevBalance = atol(prevBalance.c_str());
	validateOrder.Changed = atoi(changed.c_str());
	validateOrder.CloseBalance = atol(closeBalance.c_str());

	/*ret set recTime for publish validate */
	recTime = GetMicroSecond();
	/* 3.1 if validata is invalid, publish validate, valid
	 * and publish orderans to SG */
	if (freemargin < validateOrder.Amount) {
		//validateOrder.OrderStatus = ORDER_STATUS_INVALID ;
		validateOrder.OrderStatus = ORDER_STATUS_INVALID|ORDER_STATUS_CLOSE;

		const char* msg_validate = validateOrder.ToJson();
		std::string orderAnsNode;
		//orderAnsNode.append("('").append(reqid).append("','").append(sessionid).append("',").append(groupid).append(",").append(clientid).append(",0,").append(std::to_string(ORDER_STATUS_INVALID|ORDER_STATUS_CLOSE)).append(",").append(assetid).append(",").append(direction).append(",").append(amount).append(",0,0,0,0,").append(std::to_string(ORDER_ANS_PASSIVE_PMSG)).append(",").append(sgid).append(",0,").append(std::to_string(recTime)).append("),");
		OrderAns orderAns;
		uuid_t uuid;
		uuid_generate_time_safe(uuid);
		DecToHex(orderAns.OrderID, (char*)&uuid);
		memcpy(orderAns.ReqID, reqid, REQUEST_ID_LEN);
		memcpy(orderAns.SessionID, sessionid, SESSION_ID_LEN);
		orderAns.GroupID = validateOrder.GroupID;
		orderAns.ClientID = validateOrder.ClientID;
		orderAns.AssetID = validateOrder.AssetID;
		orderAns.Direction = validateOrder.Direction;
		orderAns.Amount = validateOrder.Amount;
		orderAns.SGID = validateOrder.SGID;
		orderAns.OrderStatus = ORDER_STATUS_INVALID|ORDER_STATUS_CLOSE;
		orderAns.OpenTime = GetMicroSecond();
		orderAnsNode.append("('").append(orderAns.ReqID).append("','").append(orderAns.SessionID).append("',").append(groupid).append(",").append(clientid).append(",'").append(orderAns.OrderID).append("',").append(std::to_string(orderAns.OrderStatus)).append(",").append(assetid).append(",").append(direction).append(",").append(amount).append(",0,").append(std::to_string(orderAns.OpenTime)).append(",0,0,").append(std::to_string(ORDER_ANS_PASSIVE_PMSG)).append(",").append(sgid).append(",0,").append(std::to_string(recTime)).append("),");
		g_concurrent_order_ans_list.enqueue(std::move(orderAnsNode));

		// /*publish validate.$*/
		/*publish validate.ALLTE*/
		TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_VALIDATE,
				//POSITIONMANAGEMENT_PUBLISH_VALIDATE_ALL_EVENT, 
				POSITIONMANAGEMENT_PUBLISH_VALIDATE_ALLTE_EVENT, 
				msg_validate, 
				strlen(msg_validate)+1, 
				ETag::EVALIDATEORDER);
#if defined(DEBUG)
		XINFO("publish VALIDATE.ALLTEM |PM->TE=%s", msg_validate);
#endif

		std::string validateOrderNode;
		validateOrderNode.append("('").append(reqid).append("','").append(sessionid).append("',").append(groupid).append(",").append(clientid).append(",0,0,").append(assetid).append(",").append(direction).append(",").append(expiryperiod).append(",").append(amount).append(",").append(std::to_string(validateOrder.OrderStatus)).append(",").append(std::to_string(validateOrder.Margin)).append(",").append(std::to_string(validateOrder.FreeMargin)).append(",").append(std::to_string(validateOrder.Profit)).append(",").append(sgid).append(",0,").append(std::to_string(recTime)).append("),");
		g_concurrent_validate_order_list.enqueue(std::move(validateOrderNode));

#if 0
		OrderAns orderAns;
		memcpy(orderAns.ReqID, reqid, REQUEST_ID_LEN);
		memcpy(orderAns.SessionID, sessionid, SESSION_ID_LEN);
		orderAns.GroupID = validateOrder.GroupID;
		orderAns.ClientID = validateOrder.ClientID;
		orderAns.AssetID = validateOrder.AssetID;
		orderAns.Direction = validateOrder.Direction;
		orderAns.Amount = validateOrder.Amount;
		orderAns.SGID = validateOrder.SGID;
		//orderAns.OrderStatus = ORDER_STATUS_INVALID;
		orderAns.OrderStatus = ORDER_STATUS_INVALID|ORDER_STATUS_CLOSE;
#endif

		/*publish orderAns.sgid */
		char orderAnsEvent[ORDERANS_RM_SGID_LEN_EVENT];
		sprintf(orderAnsEvent, "%s.%s", POSITIONMANAGEMENT_PUBLISH_ORDERANS_RM_EVENT.c_str(), sgid);
		TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_ORDERANS,
				//sgig,
				orderAnsEvent,
				orderAns.ToJson(), 
				strlen(orderAns.ToJson())+1, 
				ETag::EORDERANS);
		
#if defined(DEBUG)
		XINFO("publish ORDERANS.SGID|PM->SG=%s", orderAns.ToJson());
#endif
	} else {
	/* 3.2 if validata is valid, pubulish validata and valid,
	 * then wait orderans(for openprice), 
	 * and wait untill the close pirce of orderans*/
		validateOrder.OrderStatus = 0;
		//validateOrder.Balance = atol(balance.c_str());
		//asprintf(&sqlbuf, POSITION_UPDATE_VALID_FORMAT, amount, amount, groupid, clientid);
		asprintf(&sqlbuf, POSITION_UPDATE_VALID_FORMAT_STR, amount, amount, groupid, clientid);
#if !defined(MULTIPLE_THREAD)
		g_threadPool->Enqueue([sqlbuf] {
#endif
			snetwork_xservice_db::MySqlDB db;
			db.Open();
			db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

			db.StartTransaction();
			ssize_t rows = db.ExecSql(sqlbuf);
			if (rows == -1) {
				XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
			}
			db.Commint();
			db.Close();

			free(sqlbuf);
#if !defined(MULTIPLE_THREAD)
		});
#endif
		sqlbuf = nullptr;

		validateOrder.FreeMargin -= validateOrder.Amount;
		validateOrder.Margin += validateOrder.Amount;

		std::string validateOrderNode;
		validateOrderNode.append("('").append(reqid).append("','").append(sessionid).append("',").append(groupid).append(",").append(clientid).append(",0,0,").append(assetid).append(",").append(direction).append(",").append(expiryperiod).append(",").append(amount).append(",0,").append(std::to_string(validateOrder.Margin)).append(",").append(std::to_string(validateOrder.FreeMargin)).append(",").append(std::to_string(validateOrder.Profit)).append(",").append(sgid).append(",0,").append(std::to_string(recTime)).append("),");
		g_concurrent_validate_order_list.enqueue(std::move(validateOrderNode));

		const char* msg_validate = validateOrder.ToJson();
#if 0
		/*publish valid.sgid */
		TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_VALID,
				sgid,
				msg_validate, 
				strlen(msg_validate)+1, 
				ETag::EVALIDATEORDER);
#if defined(DEBUG)
		XINFO("publish valid.sgid|PM->SG=%s", msg_validate);
#endif

		/*publish validate.$*/
		TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_VALIDATE,
				POSITIONMANAGEMENT_PUBLISH_VALIDATE_ALL_EVENT, 
				msg_validate, 
				strlen(msg_validate)+1, 
				ETag::EVALIDATEORDER);
#if defined(DEBUG)
		XINFO("publish validate.$|PM->TE=%s", msg_validate);
#endif
#else
		/*publish validate.ALLTE.SGID */
		char validateEvent[VALIDATE_ALLTE_SGID_LEN_EVENT];
		sprintf(validateEvent, "%s.%s", POSITIONMANAGEMENT_PUBLISH_VALIDATE_ALLTE_EVENT.c_str(), sgid);
		TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_VALIDATE,
				validateEvent, 
				msg_validate, 
				strlen(msg_validate)+1, 
				ETag::EVALIDATEORDER);
#if defined(DEBUG)
		XINFO("publish validate.%s|PM->TE|SG=%s", validateEvent, msg_validate);
#endif
#endif
	}
#endif
}
/*SubscribeSendOrder end ****/

/*SubscribeSystem begin ****/
SubscribeSystem::SubscribeSystem(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeSystem::Done(const char* event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("msg:%s, len:%d\n", event, eventLen);
#endif
}

void SubscribeSystem::Done(char* event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("msg:%s, len:%d\n", event, eventLen);
#endif
}
/*SubscribeSystem end ****/

/*SubscribeOrderAns begin ****/
regex_t SubscribeOrderAns::re;

int SubscribeOrderAns::StaticInit(void) {
	int ret = 0;
	char errmsg[1024];

	if ((ret = regcomp(&re, ORDERANS_REGEX, REG_EXTENDED)) != 0) {
		regerror(ret, &re, errmsg, 1023);
		errmsg[1023] = 0;
		fprintf(stderr, "%s", errmsg);
	}

	return ret;
}

SubscribeOrderAns::SubscribeOrderAns(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeOrderAns::Done(const char* event, unsigned int eventLen) {
	long recTime = GetMicroSecond();
#if defined(DEBUG)
	XINFO("msg:%s, len:%d\n", event, eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse(event, eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error|event=%s", event);

		return;
	}

	rapidjson::Value::ConstMemberIterator it;

	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* reqid = it->value.GetString();

	if(((it = d.FindMember("sessionid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* sessionid = it->value.GetString();

	if(((it = d.FindMember("orderid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	const char* orderid = it->value.GetString();

	if(((it = d.FindMember("orderstatus")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int orderstatus = it->value.GetInt();

	if(((it = d.FindMember("groupid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int groupid = it->value.GetInt();

	if(((it = d.FindMember("clientid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int clientid = it->value.GetInt();

	//int orderid = 999;
	if(((it = d.FindMember("assetid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int assetid = it->value.GetInt();

	if(((it = d.FindMember("direction")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int direction = it->value.GetInt();

	if(((it = d.FindMember("amount")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int amount = it->value.GetInt();

	if(((it = d.FindMember("payout")) == d.MemberEnd()) || !it->value.IsInt64()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	long payout = it->value.GetInt64();

	if(((it = d.FindMember("opentime")) == d.MemberEnd()) || !it->value.IsInt64()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	long opentime = it->value.GetInt64();

	if(((it = d.FindMember("openprice")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int openprice = it->value.GetInt();

	if(((it = d.FindMember("closeprice")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int closeprice = it->value.GetInt();

	if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int sgid = it->value.GetInt();
	char sgidTmp[10];
	sprintf(sgidTmp, "%d", sgid);

	if(((it = d.FindMember("expiryperiod")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int expiryperiod = it->value.GetInt();

	if(((it = d.FindMember("teid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
		XINFO("field no exit or type error");
#endif

		return;
	}
	int teid = it->value.GetInt();

	/*asyn write order ans to database*/
	ssize_t rows = 0;
	char* sqlbuf = nullptr;
	bool closePriceFlag = false;

#if !defined(MULTIPLE_THREAD)
	std::future<ssize_t> updateThread;
#endif
	ssize_t updateRow = 0;

	std::string orderAnsNode;
	//orderAnsNode.append("('").append(reqid).append("','").append(sessionid).append("',").append(std::to_string(groupid)).append(",").append(std::to_string(clientid)).append(",999,").append(std::to_string(orderstatus)).append(",").append(std::to_string(assetid)).append(",").append(std::to_string(direction)).append(",").append(std::to_string(amount)).append(",").append(std::to_string(payout)).append(",").append(std::to_string(opentime)).append(",").append(std::to_string(openprice)).append(",").append(std::to_string(closeprice)).append(",").append(std::to_string(ORDER_ANS_PASSIVE_TEPM)).append(",").append(sgidTmp).append(",").append(std::to_string(teid)).append(",").append(std::to_string(recTime)).append("),");
	orderAnsNode.append("('").append(reqid).append("','").append(sessionid).append("',").append(std::to_string(groupid)).append(",").append(std::to_string(clientid)).append(",'").append(orderid).append("',").append(std::to_string(orderstatus)).append(",").append(std::to_string(assetid)).append(",").append(std::to_string(direction)).append(",").append(std::to_string(amount)).append(",").append(std::to_string(payout)).append(",").append(std::to_string(opentime)).append(",").append(std::to_string(openprice)).append(",").append(std::to_string(closeprice)).append(",").append(std::to_string(ORDER_ANS_PASSIVE_TEPM)).append(",").append(sgidTmp).append(",").append(std::to_string(teid)).append(",").append(std::to_string(recTime)).append("),");
	g_concurrent_order_ans_list.enqueue(std::move(orderAnsNode));

	RequestAck ack;
	memcpy(ack.ReqID, reqid, REQUEST_ID_LEN);
	if (B_IS_SET(orderstatus, 7)) { /*close price*/
		ack.ReqStatus = (ACK_BIT_FLAG | PM_BIT_FLAG |PRICE_BIT_FLAG);
		closePriceFlag = true;
	} else { /* openprice */
		ack.ReqStatus = (ACK_BIT_FLAG | PM_BIT_FLAG);
	}
	const char* msg_ack = ack.ToJson();
	char teidTmp[10];
	memset(teidTmp, 0, sizeof(teidTmp));
	sprintf(teidTmp, "%d", teid);
	TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_TRADINGENGIN,
												   teidTmp,
												   msg_ack, 
												   strlen(msg_ack)+1, 
												   ETag::EREQUESTACK);
	if (false == closePriceFlag) {
		return;
	}

	do {
		if (B_IS_SET(orderstatus, 2)) {
			/* WIN */
			/*const char* POSITION_UPDATE_VALID_WIN_FORMAT = 
			 * "UPDATE position_management 
			 * SET profit=profit+%ld,
			 * freemargin=freemargin+%ld+%d,
			 * margin=margin-%d,
			 * balance=balance+%ld 
			 * WHERE groupid=%d 
			 * AND clientid=%d";
			 * */
			asprintf(&sqlbuf, POSITION_UPDATE_VALID_WIN_FORMAT,
					 payout,
					 payout,
					 amount,
					 amount,
					 payout,
					 groupid,
					 clientid);
#if !defined(MULTIPLE_THREAD)
			updateThread = g_threadPool->Enqueue([sqlbuf] {
#endif
				snetwork_xservice_db::MySqlDB db;
				db.Open();
				db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());
				db.StartTransaction();

				ssize_t rows = db.ExecSql(sqlbuf);
				if (rows == -1) {
					XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
				}

				db.Commint();
				db.Close();
				free(sqlbuf);

#if !defined(MULTIPLE_THREAD)
				return rows;
			});
#endif
			sqlbuf = nullptr;
			break;
		}

		if (B_IS_SET(orderstatus, 3)) {
			/* LOSS*/
			/* const char* POSITION_UPDATE_VALID_LOSS_FORMAT = 
			 * "UPDATE position_management 
			 * SET profit=profit-%d,
			 * margin=margin-%d,
			 * balance=balance-%d 
			 * WHERE groupid=%d 
			 * AND clientid=%d"; 
			 * */
			asprintf(&sqlbuf, POSITION_UPDATE_VALID_LOSS_FORMAT,
					 amount,
					 amount,
					 amount,
					 groupid,
					 clientid);
#if !defined(MULTIPLE_THREAD)
			updateThread = g_threadPool->Enqueue([sqlbuf] {
#endif
				snetwork_xservice_db::MySqlDB db;
				db.Open();
				db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());
				db.StartTransaction();

				ssize_t rows = db.ExecSql(sqlbuf);
				if (rows == -1) {
					XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
				}

				db.Commint();
				db.Close();
				free(sqlbuf);

#if !defined(MULTIPLE_THREAD)
				return rows;
			});
#endif
			sqlbuf = nullptr;
			break;
		}

		if (B_IS_SET(orderstatus, 4) || 
			B_IS_SET(orderstatus, 5)) {
			/* REJECT or drawn */
			/*rollback: unlock margin and add it to freemargin*/
			/* const char* POSITION_UPDATE_VALID_REJECT_FORMAT = 
			 * "UPDATE position_management 
			 * SET freemargin=freemargin+%d,
			 * margin=margin-%d 
			 * WHERE groupid=%d 
			 * AND clientid=%d"; 
			 * */
			asprintf(&sqlbuf, POSITION_UPDATE_VALID_REJECT_FORMAT,
					 amount,
					 amount,
					 groupid,
					 clientid);
#if !defined(MULTIPLE_THREAD)
			updateThread = g_threadPool->Enqueue([sqlbuf] {
#endif
				snetwork_xservice_db::MySqlDB db;
				db.Open();
				db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());
				db.StartTransaction();

				ssize_t rows = db.ExecSql(sqlbuf);
				if (rows == -1) {
					XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
				}

				db.Commint();
				db.Close();
				free(sqlbuf);

#if !defined(MULTIPLE_THREAD)
				return rows;
			});
#endif
			sqlbuf = nullptr;
			break;
		}
	}while(false);



#if !defined(MULTIPLE_THREAD)
	updateThread.wait();
	updateRow = updateThread;
#else
	updateRow = rows;
#endif
	if (updateRow == -1) {
		/* TT logout */
		XWARNING("TT had logout, send the ORDERANS to CM|%s", event);
		TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_OFFLINE_ORDERANS_NAME,
													   POSITIONMANAGEMENT_PUBLISH_OFFLINE_ORDERANS_EVENT,
													   event, 
													   eventLen, 
													   ETag::EOFFLINEORDERANS);

		return;
	}

	snetwork_xservice_db::Redis* redis = g_redisPool->GetObject();
	char key[64];
	sprintf(key,"%s_%d_%d", HKEYVALIDATE.c_str(), groupid, clientid);
	std::string margin = std::move(redis->HGet(key, "margin"));
	std::string freeMarginTmp = std::move(redis->HGet(key, "freemargin"));
	std::string profit = std::move(redis->HGet(key, "profit"));
	std::string status = std::move(redis->HGet(key, "status"));
	std::string prevBalance = std::move(redis->HGet(key, "prevbalance"));
	std::string changed = std::move(redis->HGet(key, "changed"));
	std::string closeBalance = std::move(redis->HGet(key, "closebalance"));
	std::string ssgidTmp = std::move(redis->HGet(key, "sgid"));
	g_redisPool->ReleaseObject(redis);
	MySqlRecordSet rs_position;
	if (freeMarginTmp.empty()) {
		/*2.2 redis no record, query mysql */
		MySqlDB db;
		db.Open();
		db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

		asprintf(&sqlbuf, SELECT_VALID_FORMAT, groupid, clientid);
		rs_position = db.QuerySql(sqlbuf);
		if (db.IsError()) {
			XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
			db.Close();
		} else {
			db.Close();
		}

		size_t rows = rs_position.GetRows();
		if (rows == 0) {
			XWARNING("Empty result|sql=%s", sqlbuf);
			FREE(&sqlbuf);
			/* TT logout */
			XWARNING("TT had logout, send the ORDERANS to CM|%s", event);
			TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_OFFLINE_ORDERANS_NAME,
														   POSITIONMANAGEMENT_PUBLISH_OFFLINE_ORDERANS_EVENT,
														   event, 
														   eventLen, 
														   ETag::EOFFLINEORDERANS);

			return;
		} else {
			FREE(&sqlbuf);
			/* Query freemargin from mysql, and compare with sendorder.amount */
			margin = rs_position.GetFieldByName(0, "margin");
			freeMarginTmp = rs_position.GetFieldByName(0, "freemargin");
			profit = rs_position.GetFieldByName(0, "profit");
			status = rs_position.GetFieldByName(0, "status");
			prevBalance = rs_position.GetFieldByName(0, "prevbalance");
			changed = rs_position.GetFieldByName(0, "changed");
			closeBalance = rs_position.GetFieldByName(0, "closebalance");
		}
	}
	long freemargin = atol(freeMarginTmp.c_str());

	ValidateOrder validateOrder;
	memcpy(validateOrder.ReqID, reqid, REQUEST_ID_LEN);
	memcpy(validateOrder.SessionID, sessionid, SESSION_ID_LEN);
	validateOrder.OrderStatus = orderstatus;
	validateOrder.GroupID = groupid;
	validateOrder.ClientID = clientid;
	validateOrder.AssetID = assetid;
	validateOrder.Direction = direction;
	validateOrder.ExpiryPeriod = expiryperiod;
	validateOrder.Amount = amount;
	//validateOrder.SGID = sgid;
	validateOrder.SGID = atol(ssgidTmp.c_str());
	validateOrder.FreeMargin = freemargin;
	validateOrder.Margin = atol(margin.c_str());
	validateOrder.Profit = atol(profit.c_str());
	validateOrder.Status = atoi(status.c_str());
	validateOrder.PrevBalance = atol(prevBalance.c_str());
	validateOrder.Changed = atoi(changed.c_str());
	validateOrder.CloseBalance = atol(closeBalance.c_str());

	std::string validateOrderNode;
	validateOrderNode.append("('").append(reqid).append("','").append(sessionid).append("',").append(std::to_string(groupid)).append(",").append(std::to_string(clientid)).append(",0,0,").append(std::to_string(assetid)).append(",").append(std::to_string(direction)).append(",").append(std::to_string(expiryperiod)).append(",").append(std::to_string(amount)).append(",").append(std::to_string(orderstatus)).append(",").append(std::to_string(validateOrder.Margin)).append(",").append(std::to_string(validateOrder.FreeMargin)).append(",").append(std::to_string(validateOrder.Profit)).append(",").append(ssgidTmp).append(",0,").append(std::to_string(recTime)).append("),");
	g_concurrent_validate_order_list.enqueue(std::move(validateOrderNode));

	if (strcmp(ssgidTmp.c_str(), sgidTmp) != 0 &&
		!ssgidTmp.empty() 
		&& ssgidTmp != "0") {
		/*publish orderans to new SGID */
		TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_ORDERANS,
													   ssgidTmp.c_str(),
													   event, 
													   eventLen, 
													   ETag::EORDERANS);
#if defined(DEBUG)
		XINFO("publish orderans.%s|PM->SG=%s", ssgidTmp.c_str(),event);
#endif
	}

	char validateEvent[VALIDATE_ALLTE_SGID_LEN_EVENT];
	//sprintf(validateEvent, "%s.%s", POSITIONMANAGEMENT_PUBLISH_VALIDATE_SG_EVENT, sgidTmp);
	if (ssgidTmp == "0" || ssgidTmp.empty()) {
		sprintf(validateEvent, "%s.%s", POSITIONMANAGEMENT_PUBLISH_VALIDATE_SG_EVENT.c_str(), sgidTmp);
		PositionAccount account_info(validateOrder);
		TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL,
													   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT, 
													   account_info.ToJson(), 
													   strlen(account_info.ToJson())+1, 
													   ETag::EPULLANS);
	} else {
		sprintf(validateEvent, "%s.%s", POSITIONMANAGEMENT_PUBLISH_VALIDATE_SG_EVENT.c_str(), ssgidTmp.c_str());
	}
	TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_VALIDATE.c_str(),
												   validateEvent,
												   validateOrder.ToJson(), 
												   strlen(validateOrder.ToJson())+1, 
												   ETag::EVALIDATEORDER);
#if defined(DEBUG)
	XINFO("publish validate.sgid|PM->SG=%s", validateOrder.ToJson());
#endif
}

/* parse by regex */
void SubscribeOrderAns::Done(char* event, unsigned int eventLen) {
	long recTime = GetMicroSecond();
	char* offline = (char*)malloc(eventLen);
	memcpy(offline, event, eventLen);
#if defined(DEBUG)
	XINFO("msg:%s, len:%d\n", event, eventLen);
#endif
	thread_local regmatch_t rm[ORDERANS_FIELDS];

	int ret = regexec(&re, event, ORDERANS_FIELDS, rm, 0);
	if (ret != 0) {
		XERROR("format error, len=%zu|%s\n", eventLen, event);

		return;
	}

	event[rm[1].rm_eo] = 0;
	event[rm[2].rm_eo] = 0;
	event[rm[3].rm_eo] = 0;
	event[rm[4].rm_eo] = 0;
	event[rm[5].rm_eo] = 0;
	event[rm[6].rm_eo] = 0;
	event[rm[7].rm_eo] = 0;
	event[rm[8].rm_eo] = 0;
	event[rm[9].rm_eo] = 0;
	event[rm[10].rm_eo] = 0;
	event[rm[11].rm_eo] = 0;
	event[rm[12].rm_eo] = 0;
	event[rm[13].rm_eo] = 0;
	event[rm[14].rm_eo] = 0;
	event[rm[15].rm_eo] = 0;
	event[rm[16].rm_eo] = 0;
	const char* sessionid = &event[rm[1].rm_so];			/* sessionid */
	const char* reqid = &event[rm[2].rm_so];			/* reqid */
	const char* groupid =&event[rm[3].rm_so];		/* groupid */
	const char* clientid = &event[rm[4].rm_so];		/* clientid */
	const char* orderid = &event[rm[5].rm_so];			/* orderid */
	const char* orderstatus = &event[rm[6].rm_so];		/* orderstatus */
	int orderstatusTmp = atoi(orderstatus);		/* orderstatus */
	const char* assetid = &event[rm[7].rm_so];		/* assetid */
	const char* direction = &event[rm[8].rm_so];		/* direction */
	const char* amount = &event[rm[9].rm_so];		/* amount */
	const char* expiryperiod = &event[rm[10].rm_so];	/*expiryperiod */
	const char* opentime = &event[rm[11].rm_so];	/* opentime */
	const char* openprice = &event[rm[12].rm_so];	/* openprice */
	const char* closeprice = &event[rm[13].rm_so];	/* closeprice */
	const char* payout = &event[rm[14].rm_so];	/* payout */
	const char* sgid = &event[rm[15].rm_so];	/* sgid */
	const char* teid = &event[rm[16].rm_so];	/* teid */

	/*asyn write order ans to database*/
	ssize_t rows = 0;
	char* sqlbuf = nullptr;
	bool closePriceFlag = false;

#if !defined(MULTIPLE_THREAD)
	std::future<ssize_t> updateThread;
#endif
	ssize_t updateRow = 0;

	std::string orderAnsNode;
	//orderAnsNode.append("('").append(reqid).append("','").append(sessionid).append("',").append(groupid).append(",").append(clientid).append(",999,").append(orderstatus).append(",").append(assetid).append(",").append(direction).append(",").append(amount).append(",").append(payout).append(",").append(opentime).append(",").append(openprice).append(",").append(closeprice).append(",").append(std::to_string(ORDER_ANS_PASSIVE_TEPM)).append(",").append(sgid).append(",").append(teid).append(",").append(std::to_string(recTime)).append("),");
	orderAnsNode.append("('").append(reqid).append("','").append(sessionid).append("',").append(groupid).append(",").append(clientid).append(",'").append(orderid).append("',").append(orderstatus).append(",").append(assetid).append(",").append(direction).append(",").append(amount).append(",").append(payout).append(",").append(opentime).append(",").append(openprice).append(",").append(closeprice).append(",").append(std::to_string(ORDER_ANS_PASSIVE_TEPM)).append(",").append(sgid).append(",").append(teid).append(",").append(std::to_string(recTime)).append("),");
	g_concurrent_order_ans_list.enqueue(std::move(orderAnsNode));

	RequestAck ack;
	memcpy(ack.ReqID, reqid, REQUEST_ID_LEN);
	if (B_IS_SET(orderstatusTmp, 7)) { /*close price*/
		ack.ReqStatus = (ACK_BIT_FLAG | PM_BIT_FLAG |PRICE_BIT_FLAG);
		closePriceFlag = true;
	} else { /* openprice */
		ack.ReqStatus = (ACK_BIT_FLAG | PM_BIT_FLAG);
	}
	const char* msg_ack = ack.ToJson();
	TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_TRADINGENGIN,
												   teid,
												   msg_ack, 
												   strlen(msg_ack)+1, 
												   ETag::EREQUESTACK);

	if (false == closePriceFlag) {
		FREE(&offline);

		return;
	}

	do {
		if (B_IS_SET(orderstatusTmp, 2)) {
			/* WIN */
			/* const char* POSITION_UPDATE_VALID_WIN_FORMAT_STR = 
			 * "UPDATE position_management 
			 * SET profit=profit+%s,
			 * freemargin=freemargin+%s+%s,
			 * margin=margin-%s,
			 * balance=balance+%s 
			 * WHERE groupid=%s 
			 * AND clientid=%s" 
			 * */
			asprintf(&sqlbuf, POSITION_UPDATE_VALID_WIN_FORMAT_STR,
					 payout,
					 payout,
					 amount,
					 amount,
					 payout,
					 groupid,
					 clientid);
#if !defined(MULTIPLE_THREAD)
			updateThread = g_threadPool->Enqueue([sqlbuf] {
#endif
				snetwork_xservice_db::MySqlDB db;
				db.Open();
				db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());
				db.StartTransaction();

				ssize_t rows = db.ExecSql(sqlbuf);
				if (rows == -1) {
					XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
				}

				db.Commint();
				db.Close();

				free(sqlbuf);

#if !defined(MULTIPLE_THREAD)
				return rows;
			});
#endif
			sqlbuf = nullptr;
			break;
		}

		if (B_IS_SET(orderstatusTmp, 3)) {
			/* LOSS*/
			/* const char* POSITION_UPDATE_VALID_LOSS_FORMAT_STR = 
			 * "UPDATE position_management 
			 * SET profit=profit-%s,
			 * margin=margin-%s,
			 * balance=balance-%s 
			 * WHERE groupid=%s 
			 * AND clientid=%s"
			 * */
			asprintf(&sqlbuf, POSITION_UPDATE_VALID_LOSS_FORMAT_STR,
					 amount,
					 amount,
					 amount,
					 groupid,
					 clientid);
#if !defined(MULTIPLE_THREAD)
			updateThread = g_threadPool->Enqueue([sqlbuf] {
#endif
				snetwork_xservice_db::MySqlDB db;
				db.Open();
				db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());
				db.StartTransaction();

				ssize_t rows = db.ExecSql(sqlbuf);
				if (rows == -1) {
					XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
				}

				db.Commint();
				db.Close();
				free(sqlbuf);

#if !defined(MULTIPLE_THREAD)
				return rows;
			});
#endif
			sqlbuf = nullptr;
			break;
		}

		if (B_IS_SET(orderstatusTmp, 4) || 
			B_IS_SET(orderstatusTmp, 5)) {
			/* REJECT or drawn */
			/*rollback: unlock margin and add it to freemargin*/
			/* const char* POSITION_UPDATE_VALID_REJECT_FORMAT_STR = 
			 * "UPDATE position_management 
			 * SET freemargin=freemargin+%s,
			 * margin=margin-%s 
			 * WHERE groupid=%s 
			 * AND clientid=%s";
			 * */
			asprintf(&sqlbuf, POSITION_UPDATE_VALID_REJECT_FORMAT_STR,
					 amount,
					 amount,
					 groupid,
					 clientid);
#if !defined(MULTIPLE_THREAD)
			updateThread = g_threadPool->Enqueue([sqlbuf] {
#endif
				snetwork_xservice_db::MySqlDB db;
				db.Open();
				db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());
				db.StartTransaction();

				ssize_t rows = db.ExecSql(sqlbuf);
				if (rows == -1) {
					XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
				}

				db.Commint();
				db.Close();
				free(sqlbuf);

#if !defined(MULTIPLE_THREAD)
				return rows;
			});
#endif
			sqlbuf = nullptr;
			break;
		}
	}while(false);

#if !defined(MULTIPLE_THREAD)
	updateThread.wait();
	updateRow = updateThread;
#else
	updateRow = rows;
#endif
	if (updateRow == -1) {
		/* TT logout */
		XWARNING("TT had logout, send the ORDERANS to CM|%s", offline);
		TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_OFFLINE_ORDERANS_NAME,
													   POSITIONMANAGEMENT_PUBLISH_OFFLINE_ORDERANS_EVENT,
													   offline, 
													   eventLen, 
													   ETag::EOFFLINEORDERANS);
		FREE(&offline);

		return;
	}

	//FREE(&offline);
	snetwork_xservice_db::Redis* redis = g_redisPool->GetObject();
	char key[64];
	sprintf(key,"%s_%s_%s", HKEYVALIDATE.c_str(), groupid, clientid);
	std::string margin = std::move(redis->HGet(key, "margin"));
	std::string freeMarginTmp = std::move(redis->HGet(key, "freemargin"));
	std::string profit = std::move(redis->HGet(key, "profit"));
	std::string status = std::move(redis->HGet(key, "status"));
	std::string prevBalance = std::move(redis->HGet(key, "prevbalance"));
	std::string changed = std::move(redis->HGet(key, "changed"));
	std::string closeBalance = std::move(redis->HGet(key, "closebalance"));
	std::string sgidTmp = std::move(redis->HGet(key, "sgid"));
	g_redisPool->ReleaseObject(redis);
	MySqlRecordSet rs_position;
	if (freeMarginTmp.empty()) {
		/*2.2 redis no record, query mysql */
		MySqlDB db;
		db.Open();
		db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

		// asprintf(&sqlbuf, SELECT_VALID_FORMAT, groupid, clientid);
		asprintf(&sqlbuf, SELECT_VALID_FORMAT_STR, groupid, clientid);
		rs_position = db.QuerySql(sqlbuf);
		if (db.IsError()) {
			XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
			db.Close();
		} else {
			db.Close();
		}

		size_t rows = rs_position.GetRows();
		if (rows == 0) {
			XWARNING("Empty result|sql=%s", sqlbuf);
			FREE(&sqlbuf);
			/* TT logout */
			XWARNING("TT had logout, send the ORDERANS to CM|%s", offline);
			TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_OFFLINE_ORDERANS_NAME,
														   POSITIONMANAGEMENT_PUBLISH_OFFLINE_ORDERANS_EVENT,
														   offline, 
														   eventLen, 
														   ETag::EOFFLINEORDERANS);
			FREE(&offline);

			return;
		} else {
			FREE(&sqlbuf);
			/* Query freemargin from mysql, and compare with sendorder.amount */
			margin = rs_position.GetFieldByName(0, "margin");
			freeMarginTmp = rs_position.GetFieldByName(0, "freemargin");
			profit = rs_position.GetFieldByName(0, "profit");
			status = rs_position.GetFieldByName(0, "status");
			prevBalance = rs_position.GetFieldByName(0, "prevbalance");
			changed = rs_position.GetFieldByName(0, "changed");
			closeBalance = rs_position.GetFieldByName(0, "closebalance");
		}
	}
	long freemargin = atol(freeMarginTmp.c_str());

	ValidateOrder validateOrder;
	memcpy(validateOrder.ReqID, reqid, REQUEST_ID_LEN);
	memcpy(validateOrder.SessionID, sessionid, SESSION_ID_LEN);
	validateOrder.OrderStatus = orderstatusTmp;
	validateOrder.GroupID = atoi(groupid);
	validateOrder.ClientID = atoi(clientid);
	validateOrder.AssetID = atoi(assetid);
	validateOrder.Direction = atoi(direction);
	validateOrder.ExpiryPeriod = atoi(expiryperiod);
	validateOrder.Amount = atoi(amount);
	//validateOrder.SGID = atoi(sgid);
	validateOrder.SGID = atol(sgidTmp.c_str());
	validateOrder.FreeMargin = freemargin;
	validateOrder.Margin = atol(margin.c_str());
	validateOrder.Profit = atol(profit.c_str());
	validateOrder.Status = atoi(status.c_str());
	validateOrder.PrevBalance = atol(prevBalance.c_str());
	validateOrder.Changed = atoi(changed.c_str());
	validateOrder.CloseBalance = atol(closeBalance.c_str());

	std::string validateOrderNode;
	validateOrderNode.append("('").append(reqid).append("','").append(sessionid).append("',").append(groupid).append(",").append(clientid).append(",0,0,").append(assetid).append(",").append(direction).append(",").append(expiryperiod).append(",").append(amount).append(",").append(orderstatus).append(",").append(std::to_string(validateOrder.Margin)).append(",").append(std::to_string(validateOrder.FreeMargin)).append(",").append(std::to_string(validateOrder.Profit)).append(",").append(sgid).append(",0,").append(std::to_string(recTime)).append("),");
	g_concurrent_validate_order_list.enqueue(std::move(validateOrderNode));
	
	if (strcmp(sgidTmp.c_str(), sgid) != 0 &&
		!sgidTmp.empty() 
		&& sgidTmp != "0") {
		/*publish orderans to new SGID */
		TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_ORDERANS,
													   sgidTmp.c_str(),
													   offline, 
													   eventLen, 
													   ETag::EORDERANS);
#if defined(DEBUG)
		XINFO("publish orderans.sgid|PM->SG=%s", offline);
#endif
		FREE(&offline);
	}

	char validateEvent[VALIDATE_ALLTE_SGID_LEN_EVENT];
	//sprintf(validateEvent, "%s.%s", POSITIONMANAGEMENT_PUBLISH_VALIDATE_SG_EVENT, sgid);
	if (sgidTmp == "0" || sgidTmp.empty()) {
		sprintf(validateEvent, "%s.%s", POSITIONMANAGEMENT_PUBLISH_VALIDATE_SG_EVENT.c_str(), sgid);
		/* user info need sync to CM */
		PositionAccount account_info(validateOrder);
		TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL,
													   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT, 
													   account_info.ToJson(), 
													   strlen(account_info.ToJson())+1, 
													   ETag::EPULLANS);
	} else {
		sprintf(validateEvent, "%s.%s", POSITIONMANAGEMENT_PUBLISH_VALIDATE_SG_EVENT.c_str(), sgidTmp.c_str());
	}
	TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_VALIDATE.c_str(),
												   validateEvent,
												   validateOrder.ToJson(), 
												   strlen(validateOrder.ToJson())+1, 
												   ETag::EVALIDATEORDER);

	FREE(&offline);
#if defined(DEBUG)
	XINFO("publish validate.sgid|PM->SG=%s", validateOrder.ToJson());
#endif
}

/*SubscribeOrderAns end ****/

/*SubscribePush begin ****/
SubscribePush::SubscribePush(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribePush::Done(const char* event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("msg:%s, len:%d\n", event, eventLen);
#endif
}

void SubscribePush::Done(char* event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("msg:%s, len:%d\n", event, eventLen);
#endif
}
/*SubscribePush end ****/

/*SubscribePull begin ****/
SubscribePull::SubscribePull(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribePull::Done(char* event, unsigned int eventLen) {
	long recTime = GetMicroSecond();
#if defined(DEBUG)
	XINFO("msg:%s, len:%d\n", event, eventLen);
#endif
	PositionAccount account_info;
	snetwork_xservice_db::Redis* redis = nullptr;
	char key[64];
	memset(key, 0, sizeof(key));
	rapidjson::Document d;
	bool flag = false;

#if !defined(MULTIPLE_THREAD)
	std::future<ssize_t> updateThread;
#endif
	ssize_t updateRow = 0;

	do {
		d.Parse(event, eventLen);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error|event=%s", event);

			flag = true;
			break;
		}

		rapidjson::Value::ConstMemberIterator it;

		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		const char* reqid = it->value.GetString();
		memcpy(account_info.ReqID, reqid, REQUEST_ID_LEN);

		if(((it = d.FindMember("sessionid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		const char* sessionid = it->value.GetString();
		memcpy(account_info.SessionID, sessionid, SESSION_ID_LEN);

		if(((it = d.FindMember("groupid")) == d.MemberEnd()) || !it->value.IsUint()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.GroupID = it->value.GetUint(); 

		if(((it = d.FindMember("clientid")) == d.MemberEnd()) || !it->value.IsUint()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.ClientID = it->value.GetUint(); 

		if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.SGID = it->value.GetInt(); 

		if(((it = d.FindMember("accountid")) == d.MemberEnd()) || !it->value.IsUint64()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.AccountID = it->value.GetUint64(); 

		if(((it = d.FindMember("prevbalance")) == d.MemberEnd()) || !it->value.IsInt64()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.PrevBalance = it->value.GetInt64(); 

		if(((it = d.FindMember("changed")) == d.MemberEnd()) || !it->value.IsInt64()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.Changed = it->value.GetInt64();

		if(((it = d.FindMember("margin")) == d.MemberEnd()) || !it->value.IsInt64()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.Margin = it->value.GetInt64();

		if(((it = d.FindMember("freemargin")) == d.MemberEnd()) || !it->value.IsInt64()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.FreeMargin = it->value.GetInt64();

		if(((it = d.FindMember("profit")) == d.MemberEnd()) || !it->value.IsInt64()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.Profit = it->value.GetInt64();

		if(((it = d.FindMember("closebalance")) == d.MemberEnd()) || !it->value.IsInt64()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.CloseBalance = it->value.GetInt64();

		if(((it = d.FindMember("status")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.Status = it->value.GetInt();

		if(((it = d.FindMember("type")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.Type = it->value.GetInt();
	} while(false);
	
	sprintf(key,"%s_%d_%d", HKEYVALIDATE.c_str(), account_info.GroupID, account_info.ClientID);
	if (true == flag) {
		account_info.Status = ACCOUNT_BIT_FORMAT;

		TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL.c_str(),
													   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT.c_str(), 
													   account_info.ToJson(), 
													   strlen(account_info.ToJson())+1, 
													   ETag::EPULLANS);
		PMException pmException(event,account_info.Status, AccountErrMsg[account_info.ErrMsgIndex]);
		pmException.PublishException();

		return;
	}

	int status = 0;
	if (account_info.VerifyData(status)) { 
		/* error */
		account_info.Status = status;
		account_info.Check = 0;
		TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL.c_str(),
													   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT.c_str(), 
													   account_info.ToJson(), 
													   strlen(account_info.ToJson())+1, 
													   ETag::EPULLANS);

		PMException pmException(event,account_info.Status, AccountErrMsg[account_info.ErrMsgIndex]);
		pmException.PublishException();

		return;
	} else {
		account_info.Check = 1;
		account_info.Status = status;
		account_info.Balance = account_info.FreeMargin + account_info.Margin;
	}

	ssize_t rows = 0;
	char* sqlbuf = nullptr;
	CDateTime dateTime;
	char* tmpDay = dateTime.AddDay(0);
	int day = atoi(tmpDay);
	FREE(&tmpDay);
	//char* tmpTime = dateTime.GetDateTime(CDateTime::EDATETIME);

	/* 1. insert record to mysql as log */
	asprintf(&sqlbuf, INSERT_POSITION_LOG_FORMAT,
			 day,
			 account_info.ReqID,
			 account_info.SessionID,
			 account_info.GroupID,
			 account_info.ClientID,
			 account_info.SGID,
			 account_info.AccountID,
			 account_info.Balance,
			 account_info.PrevBalance,
			 account_info.Changed,
			 account_info.Margin,
			 account_info.FreeMargin,
			 account_info.Profit,
			 account_info.Type,
			 account_info.CloseBalance,
			 account_info.Status,
			 account_info.Check,
			 //tmpTime);
			 //GetMicroSecond());
			 recTime);
#if !defined(MULTIPLE_THREAD)
	g_threadPool->Enqueue([sqlbuf] {
#endif
		snetwork_xservice_db::MySqlDB db;
		db.Open();
		//db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());
		//dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig());
		db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

#if !defined(MULTIPLE_THREAD)
		ssize_t rows = db.ExecSql(sqlbuf);
#else
		rows = db.ExecSql(sqlbuf);
#endif
		db.Close();

		free(sqlbuf);
#if !defined(MULTIPLE_THREAD)
	});
#endif
	sqlbuf = nullptr;
	
	/* type:
	 * 0: default
	 * 1: init(first asyn: CM->PM)
	 * 2: deposite
	 * 4: withdrawal
	 * 8: feedback(PM->CM, PM push to CM)
	 * 16:userinfo
	 * 32: logout
	 * 64: forcelogout
	 */
	do {
		if (EPullType::EPINIT == account_info.Type) {
			/* If it is first login, insert record into database */
			/* 1. insert record to mysql position_management */
			asprintf(&sqlbuf, INSERT_POSITION_INIT_FORMAT,
					 account_info.GroupID,
					 account_info.ClientID,
					 account_info.SGID,
					 account_info.AccountID,
					 account_info.Balance,
					 account_info.PrevBalance,
					 account_info.Changed,
					 account_info.Margin,
					 account_info.FreeMargin,
					 account_info.Profit,
					 account_info.Type,
					 account_info.CloseBalance,
					 account_info.Status,
					 account_info.Check,
					 recTime);
#if !defined(MULTIPLE_THREAD)
			updateThread = g_threadPool->Enqueue([sqlbuf] {
#endif
				snetwork_xservice_db::MySqlDB db;
				db.Open();
				db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());
				db.StartTransaction();

				ssize_t rows = db.ExecSql(sqlbuf);
				if (-1 == rows) {
					XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
				}
				db.Commint();
				db.Close();

				free(sqlbuf);
#if !defined(MULTIPLE_THREAD)

				return rows;
			});
#endif
			//sqlbuf = nullptr;

#if !defined(MULTIPLE_THREAD)
			updateThread.wait();
			updateRow = updateThread;
#else
			updateRow = rows;
#endif

			if (-1 == updateRow) {
				/* database exists, maybe should be merge datbase */
				redis = g_redisPool->GetObject();
				int exists = redis->ExistsKey(key);
				std::string pro =redis->HGet(key, "profit");
				bool needUpdate = false;
				if (pro.empty() || exists != 1) {
					needUpdate = true;
					g_redisPool->ReleaseObject(redis);
				} else {
					/*reis */
					long freemargin = atol(redis->HGet(key, "freemargin").c_str());
					long profit = atol(redis->HGet(key, "profit").c_str());
					long margin = atol(redis->HGet(key, "margin").c_str());
					long prevbalance = atol(redis->HGet(key, "prevbalance").c_str());
					long changed = atol(redis->HGet(key, "changed").c_str());
					g_redisPool->ReleaseObject(redis);

					if (account_info.PrevBalance == prevbalance &&
						account_info.Changed == changed) {
						/* don't merge */
						account_info.FreeMargin = freemargin;
						account_info.Profit = profit;
						account_info.Margin = margin;
						account_info.Balance = account_info.FreeMargin + account_info.Margin;
					} else {
						/* merge datas */
						account_info.FreeMargin = profit + account_info.PrevBalance + account_info.Changed - margin;
						account_info.Profit = profit;
						account_info.Margin = margin;
						account_info.Balance = account_info.FreeMargin + account_info.Margin;
						needUpdate = true;
					}
				}

				needUpdate = true;
				if (needUpdate) {
					asprintf(&sqlbuf, POSITION_UPDATE_PULL_FORMAT,
							 account_info.Balance, 
							 account_info.FreeMargin,
							 account_info.PrevBalance,
							 account_info.Changed,
							 account_info.SGID,
							 account_info.GroupID, 
							 account_info.ClientID);
#if !defined(MULTIPLE_THREAD)
					updateThread = g_threadPool->Enqueue([sqlbuf] {
#endif
						snetwork_xservice_db::MySqlDB db;
						db.Open();
						db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

						db.StartTransaction();
						ssize_t rows = db.ExecSql(sqlbuf);
						if (rows == -1) {
							XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
						}
						db.Commint();
						db.Close();
						free(sqlbuf);

#if !defined(MULTIPLE_THREAD)
						return rows;
					});
#endif
					sqlbuf = nullptr;

#if !defined(MULTIPLE_THREAD)
					updateThread.wait();
					updateRow = updateThread;
#else
					updateRow = rows;
#endif

					if (updateRow == 1) {
						account_info.Status = ACCOUNT_BIT_SUCCESS;
					} else {
						account_info.Status = ACCOUNT_BIT_ALL_ERROR;
					}
				}

				account_info.Check = 0;
				TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL,
															   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT, 
															   account_info.ToJson(), 
															   strlen(account_info.ToJson())+1, 
															   ETag::EPULLANS);

				return;
			} else {
				account_info.Status = ACCOUNT_BIT_SUCCESS ;
				account_info.Check = 0;
				TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL,
															   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT, 
															   account_info.ToJson(), 
															   strlen(account_info.ToJson())+1, 
															   ETag::EPULLANS);
				return;
			}
		}

		if (account_info.Type == EPullType::EPLOGOUT || 
			account_info.Type == EPullType::EPFORCELOGOUT) {
			redis = g_redisPool->GetObject();
			//char key[64];
			//sprintf(key,"%s_%d_%d", HKEYVALIDATE, account_info.GroupID, account_info.ClientID);
			int exists = redis->ExistsKey(key);
			std::string pro =redis->HGet(key, "profit");
			if (pro.empty() || exists != 1) {
				/*not exist, query mysql */
				account_info.Status = ACCOUNT_BIT_PULL_NOT_EXIST;
				g_redisPool->ReleaseObject(redis);

				asprintf(&sqlbuf, SELECT_VALID_FORMAT, account_info.GroupID, account_info.ClientID);
				MySqlDB db;
				db.Open();
				db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());
				MySqlRecordSet rs_position = db.QuerySql(sqlbuf);

				if (db.IsError()) {
					XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
					db.Close();
				} else {
					db.Close();
				}

				size_t rows = -1;
				rows = rs_position.GetRows();
				if (rows == 1) {
					account_info.Status = ACCOUNT_BIT_SUCCESS;
					long freemargin = atol(rs_position.GetFieldByName(0, "freemargin").c_str());
					long profit = atol(rs_position.GetFieldByName(0, "profit").c_str());
					long margin = atol(rs_position.GetFieldByName(0, "margin").c_str());
					long prevbalance = atol(rs_position.GetFieldByName(0, "prevbalance").c_str());
					long changed = atol(rs_position.GetFieldByName(0, "changed").c_str());

					if (account_info.PrevBalance == prevbalance ||
						account_info.Changed == changed) {
						/* don't merge */
						account_info.FreeMargin = freemargin;
						account_info.Profit = profit;
						account_info.Margin = margin;
						account_info.Balance = account_info.FreeMargin + account_info.Margin;
					} else {
						account_info.FreeMargin = profit + account_info.PrevBalance + account_info.Changed - margin;
						account_info.Profit = profit;
						account_info.Margin = margin;
						account_info.Balance = account_info.FreeMargin + account_info.Margin;
					}
				} else {
					if (rows == 0) {
						account_info.Status = ACCOUNT_BIT_PULL_NOT_EXIST;
					} else {
						account_info.Status = ACCOUNT_BIT_ALL_ERROR;
					}
				}
			} else {
				/* redis exist */
				long freemargin = atol(redis->HGet(key, "freemargin").c_str());
				long profit = atol(redis->HGet(key, "profit").c_str());
				long margin = atol(redis->HGet(key, "margin").c_str());
				long prevbalance = atol(redis->HGet(key, "prevbalance").c_str());
				long changed = atol(redis->HGet(key, "changed").c_str());
				g_redisPool->ReleaseObject(redis);

				account_info.Status = ACCOUNT_BIT_SUCCESS;
				if (account_info.PrevBalance == prevbalance &&
					account_info.Changed == changed) {
					/* don't merge */
					account_info.FreeMargin = freemargin;
					account_info.Profit = profit;
					account_info.Margin = margin;
					account_info.Balance = account_info.FreeMargin + account_info.Margin;
				} else {
					account_info.FreeMargin = profit + account_info.PrevBalance + account_info.Changed - margin;
					account_info.Profit = profit;
					account_info.Margin = margin;
					account_info.Balance = account_info.FreeMargin + account_info.Margin;
				}
			}

			if (account_info.Margin == 0) {
				asprintf(&sqlbuf, POSITION_DELETE_PULL_FORMAT, 
						 account_info.GroupID, 
						 account_info.ClientID);
#if !defined(MULTIPLE_THREAD)
				updateThread = g_threadPool->Enqueue([sqlbuf] {
#endif
					snetwork_xservice_db::MySqlDB db;
					db.Open();
					db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());
					db.StartTransaction();

					ssize_t rows = db.ExecSql(sqlbuf);
					if (-1 == rows) {
						XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
						db.RollBack();
					} else {
						if (rows == 1) {
							redis = g_redisPool->GetObject();
							redis->Del(key);
							g_redisPool->ReleaseObject(redis);
						}
						db.Commint();
					}
					/* logout, delete mysql record */
					db.Close();
					free(sqlbuf);

#if !defined(MULTIPLE_THREAD)
					return rows;
				});
#endif
				sqlbuf = nullptr;
#if !defined(MULTIPLE_THREAD)
				updateThread.wait();
				updateRow = updateThread;
#else
				updateRow = rows;
#endif
				if (-1 == updateRow) {
					account_info.Status = ACCOUNT_BIT_PULL_NOT_EXIST;
				} else {
					account_info.Status = ACCOUNT_BIT_SUCCESS ;
				}
			} else {
				account_info.SGID = 0;;
				asprintf(&sqlbuf, POSITION_UPDATE_PULL_FORMAT,
						 account_info.Balance, 
						 account_info.FreeMargin,
						 account_info.PrevBalance,
						 account_info.Changed,
						 account_info.SGID,
						 account_info.GroupID, 
						 account_info.ClientID);
#if !defined(MULTIPLE_THREAD)
				updateThread = g_threadPool->Enqueue([sqlbuf] {
#endif
					snetwork_xservice_db::MySqlDB db;
					db.Open();
					db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

					db.StartTransaction();
					ssize_t rows = db.ExecSql(sqlbuf);
					if (rows == -1) {
						XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
					}
					db.Commint();
					db.Close();
					free(sqlbuf);

#if !defined(MULTIPLE_THREAD)
					return rows;
				});
#endif
				sqlbuf = nullptr;

#if !defined(MULTIPLE_THREAD)
				updateThread.wait();
				updateRow = updateThread;
#else
				updateRow = rows;
#endif

				if (updateRow == 1) {
					account_info.Status = ACCOUNT_BIT_SUCCESS;
				} else {
					account_info.Status = ACCOUNT_BIT_ALL_ERROR;
				}
			}

			TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL,
														   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT, 
														   account_info.ToJson(), 
														   strlen(account_info.ToJson())+1, 
														   ETag::EPULLANS);

			return;
		}
	} while (false);

	/*2. query validate , if exists, modify; otherwise, insert it*/
	//snetwork_xservice_db::Redis* redis = g_redisPool->GetObject();
	redis = g_redisPool->GetObject();
	//char key[64];
	//sprintf(key,"%s_%d_%d", HKEYVALIDATE, account_info.GroupID, account_info.ClientID);
	int exists = redis->ExistsKey(key);
	std::string pro =redis->HGet(key, "profit");
	if (pro.empty() || exists != 1) {
		/*not exist, query mysql */
		account_info.Status = ACCOUNT_BIT_PULL_NOT_EXIST;
		g_redisPool->ReleaseObject(redis);

		asprintf(&sqlbuf, SELECT_VALID_FORMAT, account_info.GroupID, account_info.ClientID);
		MySqlDB db;
		db.Open();
		db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());
		MySqlRecordSet rs_position = db.QuerySql(sqlbuf);

		if (db.IsError()) {
			XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
			db.Close();
		} else {
			db.Close();
		}

		size_t rows = -1;
		rows = rs_position.GetRows();
		if (rows == 0) {
			XWARNING("Empty result|sql=%s", sqlbuf);

			if (account_info.Type == ACCOUNT_BIT_TYPE_INIT) {
				account_info.Status = ACCOUNT_BIT_SUCCESS;

				/* 1. insert record to mysql position_management */
				asprintf(&sqlbuf, INSERT_POSITION_INIT_FORMAT,
						 account_info.GroupID,
						 account_info.ClientID,
						 account_info.AccountID,
						 account_info.Balance,
						 account_info.PrevBalance,
						 account_info.Changed,
						 account_info.Margin,
						 account_info.FreeMargin,
						 account_info.Profit,
						 account_info.Type,
						 account_info.CloseBalance,
						 account_info.Status,
						 account_info.Check,
						 recTime);

#if !defined(MULTIPLE_THREAD)
				updateThread = g_threadPool->Enqueue([sqlbuf] {
#endif
					snetwork_xservice_db::MySqlDB db;
					db.Open();
					db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());
					db.StartTransaction();

					ssize_t rows = db.ExecSql(sqlbuf);
					if (-1 == rows) {
						XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
					}
					db.Commint();
					db.Close();
					free(sqlbuf);

#if !defined(MULTIPLE_THREAD)
					return rows;
				});
#endif
				sqlbuf = nullptr;

#if !defined(MULTIPLE_THREAD)
				updateThread.wait();
				updateRow = updateThread;
#else
				updateRow = rows;
#endif
				if (-1 == updateRow) {
					account_info.Status = ACCOUNT_BIT_ALL_ERROR;
				} else {
					account_info.Status = ACCOUNT_BIT_SUCCESS ;
				}

				TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL,
															   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT, 
															   account_info.ToJson(), 
															   strlen(account_info.ToJson())+1, 
															   ETag::EPULLANS);
			} else {
				account_info.Status = ACCOUNT_BIT_ALL_ERROR;
				TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL,
															   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT, 
															   account_info.ToJson(), 
															   strlen(account_info.ToJson())+1, 
															   ETag::EPULLANS);

				PMException pmException(event,account_info.Status, AccountErrMsg[account_info.ErrMsgIndex]);
				pmException.PublishException();
			}

			return;
		} else if(rows == -1) {
			FREE(&sqlbuf);
			account_info.Status = ACCOUNT_BIT_ALL_ERROR;
			TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL,
														   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT, 
														   account_info.ToJson(), 
														   strlen(account_info.ToJson())+1, 
														   ETag::EPULLANS);

			PMException pmException(event,account_info.Status, AccountErrMsg[account_info.ErrMsgIndex]);
			pmException.PublishException();

			return;
		} else {
			FREE(&sqlbuf);
			account_info.Status = ACCOUNT_BIT_SUCCESS;
			int sgid = atoi(rs_position.GetFieldByName(0, "sgid").c_str());
			long profit = atol(rs_position.GetFieldByName(0, "profit").c_str());
			long margin = atol(rs_position.GetFieldByName(0, "margin").c_str());
			//account_info.FreeMargin = profit + account_info.PrevBalance + account_info.Changed - margin;
			long retFreeMarginTmp = profit + account_info.PrevBalance + account_info.Changed - margin;
			if (retFreeMarginTmp <0) {
				account_info.Status = ACCOUNT_BIT_FREEMARGIN_INVALID;
				TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL,
															   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT, 
															   account_info.ToJson(), 
															   strlen(account_info.ToJson())+1, 
															   ETag::EPULLANS);

				PMException pmException(event,account_info.Status, AccountErrMsg[account_info.ErrMsgIndex]);
				pmException.PublishException();

				return;
			}
			account_info.FreeMargin = retFreeMarginTmp;
			account_info.Profit = profit;
			account_info.Margin = margin;
			account_info.Balance = account_info.FreeMargin + account_info.Margin;

			asprintf(&sqlbuf, POSITION_UPDATE_PULL_FORMAT,
					 account_info.Balance, 
					 account_info.FreeMargin,
					 account_info.PrevBalance,
					 account_info.Changed,
					 account_info.SGID,
					 account_info.GroupID, 
					 account_info.ClientID);
#if !defined(MULTIPLE_THREAD)
			updateThread = g_threadPool->Enqueue([sqlbuf] {
#endif
				snetwork_xservice_db::MySqlDB db;
				db.Open();
				db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

				db.StartTransaction();
				ssize_t rows = db.ExecSql(sqlbuf);
				if (rows == -1) {
					XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
				}
				db.Commint();
				db.Close();
				free(sqlbuf);

#if !defined(MULTIPLE_THREAD)
				return rows;
			});
#endif
			sqlbuf = nullptr;

#if !defined(MULTIPLE_THREAD)
			updateThread.wait();
			updateRow = updateThread;
#else
			updateRow = rows;
#endif

#if 0
			if (account_info.Type == EPullType::EPLOGOUT ||
				account_info.Type == EPullType::EPFORCELOGOUT) {
				/* logout, delete mysql record */
				asprintf(&sqlbuf, POSITION_DELETE_PULL_FORMAT, 
						 account_info.GroupID, 
						 account_info.ClientID);
#if !defined(MULTIPLE_THREAD)
				g_threadPool->Enqueue([sqlbuf] {
#endif
				snetwork_xservice_db::MySqlDB db;
				db.Open();
				db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());
				db.StartTransaction();

				ssize_t rows = db.ExecSql(sqlbuf);
				if (rows == -1) {
					XERROR("%s,con=%d|sql=", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
				}
				db.Commint();
				db.Close();

				free(sqlbuf);
#if !defined(MULTIPLE_THREAD)
			});
#endif
				sqlbuf = nullptr;
			}
#endif
			/*publish PULL.CLIENTMANAGE*/
			TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL.c_str(),
														   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT.c_str(), 
														   account_info.ToJson(), 
														   strlen(account_info.ToJson())+1, 
														   ETag::EPULLANS);

			if (account_info.Type == EPullType::EPDEPOSITE ||
				account_info.Type == EPullType::EPWITHDRAWAL) {
				char validateEvent[VALIDATE_ALLTE_SGID_LEN_EVENT];
				sprintf(validateEvent, "%s.%d", POSITIONMANAGEMENT_PUBLISH_VALIDATE_RM_EVENT.c_str(), sgid);
				//const char* msg_validate = account_info.ToJson();
				ValidateOrder validateOrder(account_info);
				const char* msg_validate = validateOrder.ToJson();
				TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_VALIDATE.c_str(),
															   validateEvent, 
															   msg_validate, 
															   strlen(msg_validate)+1, 
															   ETag::EVALIDATEORDER);
			}
		}

		return;
	} else {
		/*2.2 redis exists, get data from redis */
		int sgid = atoi(redis->HGet(key, "sgid").c_str());
		long profit = atol(redis->HGet(key, "profit").c_str());
		long margin = atol(redis->HGet(key, "margin").c_str());
		g_redisPool->ReleaseObject(redis);
		long retFreeMarginTmp = profit + account_info.PrevBalance + account_info.Changed - margin;
		if (retFreeMarginTmp <0) {
			account_info.Status = ACCOUNT_BIT_FREEMARGIN_INVALID;
			TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL.c_str(),
														   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT.c_str(), 
														   account_info.ToJson(), 
														   strlen(account_info.ToJson())+1, 
														   ETag::EPULLANS);

			PMException pmException(event,account_info.Status, AccountErrMsg[account_info.ErrMsgIndex]);
			pmException.PublishException();

			return;
		}
		//account_info.FreeMargin = profit + account_info.PrevBalance + account_info.Changed - margin;
		account_info.FreeMargin = retFreeMarginTmp;
		account_info.Profit = profit;
		account_info.Margin = margin;
		account_info.Balance = account_info.FreeMargin + account_info.Margin;

		// const char* POSITION_UPDATE_PULL_FORMAT = "UPDATE position_management SET balance=%ld,freemargin=%d,prevbalance=%ld,changed=%ld,sgid=%d WHERE groupid=%d AND clientid=%d";
		asprintf(&sqlbuf, POSITION_UPDATE_PULL_FORMAT,
				 account_info.Balance, 
				 account_info.FreeMargin,
				 account_info.PrevBalance,
				 account_info.Changed,
				 account_info.SGID,
				 account_info.GroupID, 
				 account_info.ClientID);
#if !defined(MULTIPLE_THREAD)
		updateThread = g_threadPool->Enqueue([sqlbuf] {
#endif
			snetwork_xservice_db::MySqlDB db;
			db.Open();
			db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());
			db.StartTransaction();

			ssize_t rows = db.ExecSql(sqlbuf);
			if (rows == -1) {
				XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
			}
			db.Commint();
			db.Close();
			free(sqlbuf);

#if !defined(MULTIPLE_THREAD)
			return rows;
		});
#endif
		sqlbuf = nullptr;
#if !defined(MULTIPLE_THREAD)
		updateThread.wait();
		updateRow = updateThread;
#else
		updateRow = rows;
#endif

#if 0
		if (account_info.Type == EPullType::EPLOGOUT || 
			account_info.Type == EPullType::EPFORCELOGOUT) {
			/* logout, delete mysql record */
			redis = g_redisPool->GetObject();
			redis->Del(key);
			g_redisPool->ReleaseObject(redis);

			asprintf(&sqlbuf, POSITION_DELETE_PULL_FORMAT, 
					 account_info.GroupID, 
					 account_info.ClientID);
#if !defined(MULTIPLE_THREAD)
			g_threadPool->Enqueue([sqlbuf] {
#endif
				snetwork_xservice_db::MySqlDB db;
				db.Open();
				db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

				ssize_t rows = db.ExecSql(sqlbuf);
				db.Close();
				free(sqlbuf);
#if !defined(MULTIPLE_THREAD)
			});
#endif
			sqlbuf = nullptr;
		}
#endif
		/*publish PULL.CLIENTMANAGE*/
		account_info.Status = ACCOUNT_BIT_SUCCESS;
		TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL.c_str(),
													   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT.c_str(), 
													   account_info.ToJson(), 
													   strlen(account_info.ToJson())+1, 
													   ETag::EPULLANS);

		if (account_info.Type == EPullType::EPDEPOSITE ||
			account_info.Type == EPullType::EPWITHDRAWAL) {
			char validateEvent[VALIDATE_ALLTE_SGID_LEN_EVENT];
			sprintf(validateEvent, "%s.%d", POSITIONMANAGEMENT_PUBLISH_VALIDATE_RM_EVENT.c_str(), sgid);
			//const char* msg_validate = account_info.ToJson();
			ValidateOrder validateOrder(account_info);
			const char* msg_validate = validateOrder.ToJson();
#if defined(DEBUG)
			XINFO("\n\n+++++validate of pull|%s\naccount_info|%s\n", msg_validate, account_info.ToJson());
#endif
			TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_VALIDATE.c_str(),
														   validateEvent, 
														   msg_validate, 
														   strlen(msg_validate)+1, 
														   ETag::EVALIDATEORDER);
		}
		//g_redisPool->ReleaseObject(redis);
	}
}

void SubscribePull::Done(const char* event, unsigned int eventLen) {
	long recTime = GetMicroSecond();
#if defined(DEBUG)
	XINFO("msg:%s, len:%d\n", event, eventLen);
#endif
	PositionAccount account_info;
	snetwork_xservice_db::Redis* redis = nullptr;
	char key[64];
	memset(key, 0, sizeof(key));
	rapidjson::Document d;
	bool flag = false;

#if !defined(MULTIPLE_THREAD)
	std::future<ssize_t> updateThread;
#endif
	ssize_t updateRow = 0;

	do {
		d.Parse(event, eventLen);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error|event=%s", event);

			flag = true;
			break;
		}

		rapidjson::Value::ConstMemberIterator it;

		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		const char* reqid = it->value.GetString();
		memcpy(account_info.ReqID, reqid, REQUEST_ID_LEN);

		if(((it = d.FindMember("sessionid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		const char* sessionid = it->value.GetString();
		memcpy(account_info.SessionID, sessionid, SESSION_ID_LEN);

		if(((it = d.FindMember("groupid")) == d.MemberEnd()) || !it->value.IsUint()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.GroupID = it->value.GetUint(); 

		if(((it = d.FindMember("clientid")) == d.MemberEnd()) || !it->value.IsUint()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.ClientID = it->value.GetUint(); 

		if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.SGID = it->value.GetInt(); 

		if(((it = d.FindMember("accountid")) == d.MemberEnd()) || !it->value.IsUint64()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.AccountID = it->value.GetUint64(); 

		if(((it = d.FindMember("prevbalance")) == d.MemberEnd()) || !it->value.IsInt64()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.PrevBalance = it->value.GetInt64(); 

		if(((it = d.FindMember("changed")) == d.MemberEnd()) || !it->value.IsInt64()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.Changed = it->value.GetInt64();

		if(((it = d.FindMember("margin")) == d.MemberEnd()) || !it->value.IsInt64()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.Margin = it->value.GetInt64();

		if(((it = d.FindMember("freemargin")) == d.MemberEnd()) || !it->value.IsInt64()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.FreeMargin = it->value.GetInt64();

		if(((it = d.FindMember("profit")) == d.MemberEnd()) || !it->value.IsInt64()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.Profit = it->value.GetInt64();

		if(((it = d.FindMember("closebalance")) == d.MemberEnd()) || !it->value.IsInt64()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.CloseBalance = it->value.GetInt64();

		if(((it = d.FindMember("status")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.Status = it->value.GetInt();

		if(((it = d.FindMember("type")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.Type = it->value.GetInt();
	} while(false);
	
	sprintf(key,"%s_%d_%d", HKEYVALIDATE.c_str(), account_info.GroupID, account_info.ClientID);
	if (true == flag) {
		account_info.Status = ACCOUNT_BIT_FORMAT;

		TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL.c_str(),
													   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT.c_str(), 
													   account_info.ToJson(), 
													   strlen(account_info.ToJson())+1, 
													   ETag::EPULLANS);
		PMException pmException(event,account_info.Status, AccountErrMsg[account_info.ErrMsgIndex]);
		pmException.PublishException();

		return;
	}

	int status = 0;
	if (account_info.VerifyData(status)) { 
		/* error */
		account_info.Status = status;
		account_info.Check = 0;
		TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL.c_str(),
													   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT.c_str(), 
													   account_info.ToJson(), 
													   strlen(account_info.ToJson())+1, 
													   ETag::EPULLANS);

		PMException pmException(event,account_info.Status, AccountErrMsg[account_info.ErrMsgIndex]);
		pmException.PublishException();

		return;
	} else {
		account_info.Check = 1;
		account_info.Status = status;
		account_info.Balance = account_info.FreeMargin + account_info.Margin;
	}

	ssize_t rows = 0;
	char* sqlbuf = nullptr;
	CDateTime dateTime;
	char* tmpDay = dateTime.AddDay(0);
	int day = atoi(tmpDay);
	FREE(&tmpDay);
	//char* tmpTime = dateTime.GetDateTime(CDateTime::EDATETIME);

	/* 1. insert record to mysql as log */
	asprintf(&sqlbuf, INSERT_POSITION_LOG_FORMAT,
			 day,
			 account_info.ReqID,
			 account_info.SessionID,
			 account_info.GroupID,
			 account_info.ClientID,
			 account_info.SGID,
			 account_info.AccountID,
			 account_info.Balance,
			 account_info.PrevBalance,
			 account_info.Changed,
			 account_info.Margin,
			 account_info.FreeMargin,
			 account_info.Profit,
			 account_info.Type,
			 account_info.CloseBalance,
			 account_info.Status,
			 account_info.Check,
			 //tmpTime);
			 //GetMicroSecond());
			 recTime);
#if !defined(MULTIPLE_THREAD)
	g_threadPool->Enqueue([sqlbuf] {
#endif
		snetwork_xservice_db::MySqlDB db;
		db.Open();
		//db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());
		//dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig());
		db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

#if !defined(MULTIPLE_THREAD)
		ssize_t rows = db.ExecSql(sqlbuf);
#else
		rows = db.ExecSql(sqlbuf);
#endif
		db.Close();

		free(sqlbuf);
#if !defined(MULTIPLE_THREAD)
	});
#endif
	sqlbuf = nullptr;
	
	/* type:
	 * 0: default
	 * 1: init(first asyn: CM->PM)
	 * 2: deposite
	 * 4: withdrawal
	 * 8: feedback(PM->CM, PM push to CM)
	 * 16:userinfo
	 * 32: logout
	 * 64: forcelogout
	 */
	do {
		if (EPullType::EPINIT == account_info.Type) {
			/* If it is first login, insert record into database */
			/* 1. insert record to mysql position_management */
			asprintf(&sqlbuf, INSERT_POSITION_INIT_FORMAT,
					 account_info.GroupID,
					 account_info.ClientID,
					 account_info.SGID,
					 account_info.AccountID,
					 account_info.Balance,
					 account_info.PrevBalance,
					 account_info.Changed,
					 account_info.Margin,
					 account_info.FreeMargin,
					 account_info.Profit,
					 account_info.Type,
					 account_info.CloseBalance,
					 account_info.Status,
					 account_info.Check,
					 recTime);
#if !defined(MULTIPLE_THREAD)
			updateThread = g_threadPool->Enqueue([sqlbuf] {
#endif
				snetwork_xservice_db::MySqlDB db;
				db.Open();
				db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());
				db.StartTransaction();

				ssize_t rows = db.ExecSql(sqlbuf);
				if (-1 == rows) {
					XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
				}
				db.Commint();
				db.Close();

				free(sqlbuf);
#if !defined(MULTIPLE_THREAD)

				return rows;
			});
#endif
			sqlbuf = nullptr;

#if !defined(MULTIPLE_THREAD)
			updateThread.wait();
			updateRow = updateThread;
#else
			updateRow = rows;
#endif

			if (-1 == updateRow) {
				/* database exists, maybe should be merge datbase */
				redis = g_redisPool->GetObject();
				int exists = redis->ExistsKey(key);
				std::string pro =redis->HGet(key, "profit");
				bool needUpdate = false;
				if (pro.empty() || exists != 1) {
					needUpdate = true;
					g_redisPool->ReleaseObject(redis);
				} else {
					/*reis */
					long freemargin = atol(redis->HGet(key, "freemargin").c_str());
					long profit = atol(redis->HGet(key, "profit").c_str());
					long margin = atol(redis->HGet(key, "margin").c_str());
					long prevbalance = atol(redis->HGet(key, "prevbalance").c_str());
					long changed = atol(redis->HGet(key, "changed").c_str());
					g_redisPool->ReleaseObject(redis);

					if (account_info.PrevBalance == prevbalance &&
						account_info.Changed == changed) {
						/* don't merge */
						account_info.FreeMargin = freemargin;
						account_info.Profit = profit;
						account_info.Margin = margin;
						account_info.Balance = account_info.FreeMargin + account_info.Margin;
					} else {
						/* merge datas */
						account_info.FreeMargin = profit + account_info.PrevBalance + account_info.Changed - margin;
						account_info.Profit = profit;
						account_info.Margin = margin;
						account_info.Balance = account_info.FreeMargin + account_info.Margin;
						needUpdate = true;
					}
				}

				needUpdate = true;
				if (needUpdate) {
					asprintf(&sqlbuf, POSITION_UPDATE_PULL_FORMAT,
							 account_info.Balance, 
							 account_info.FreeMargin,
							 account_info.PrevBalance,
							 account_info.Changed,
							 account_info.SGID,
							 account_info.GroupID, 
							 account_info.ClientID);
#if !defined(MULTIPLE_THREAD)
					updateThread = g_threadPool->Enqueue([sqlbuf] {
#endif
						snetwork_xservice_db::MySqlDB db;
						db.Open();
						db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

						db.StartTransaction();
						ssize_t rows = db.ExecSql(sqlbuf);
						if (rows == -1) {
							XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
						}
						db.Commint();
						db.Close();
						free(sqlbuf);

#if !defined(MULTIPLE_THREAD)
						return rows;
					});
#endif
					sqlbuf = nullptr;

#if !defined(MULTIPLE_THREAD)
					updateThread.wait();
					updateRow = updateThread;
#else
					updateRow = rows;
#endif

					if (updateRow == 1) {
						account_info.Status = ACCOUNT_BIT_SUCCESS;
					} else {
						account_info.Status = ACCOUNT_BIT_ALL_ERROR;
					}
				}

				account_info.Check = 0;
				TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL.c_str(),
															   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT.c_str(), 
															   account_info.ToJson(), 
															   strlen(account_info.ToJson())+1, 
															   ETag::EPULLANS);

				return;
			} else {
				account_info.Status = ACCOUNT_BIT_SUCCESS ;
				account_info.Check = 0;
				TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL.c_str(),
															   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT.c_str(), 
															   account_info.ToJson(), 
															   strlen(account_info.ToJson())+1, 
															   ETag::EPULLANS);
				return;
			}
		}

		if (account_info.Type == EPullType::EPLOGOUT || 
			account_info.Type == EPullType::EPFORCELOGOUT) {
			redis = g_redisPool->GetObject();
			//char key[64];
			//sprintf(key,"%s_%d_%d", HKEYVALIDATE, account_info.GroupID, account_info.ClientID);
			int exists = redis->ExistsKey(key);
			std::string pro =redis->HGet(key, "profit");
			if (pro.empty() || exists != 1) {
				/*not exist, query mysql */
				account_info.Status = ACCOUNT_BIT_PULL_NOT_EXIST;
				g_redisPool->ReleaseObject(redis);

				asprintf(&sqlbuf, SELECT_VALID_FORMAT, account_info.GroupID, account_info.ClientID);
				MySqlDB db;
				db.Open();
				db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());
				MySqlRecordSet rs_position = db.QuerySql(sqlbuf);

				if (db.IsError()) {
					XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
					db.Close();
				} else {
					db.Close();
				}

				size_t rows = -1;
				rows = rs_position.GetRows();
				if (rows == 1) {
					account_info.Status = ACCOUNT_BIT_SUCCESS;
					long freemargin = atol(rs_position.GetFieldByName(0, "freemargin").c_str());
					long profit = atol(rs_position.GetFieldByName(0, "profit").c_str());
					long margin = atol(rs_position.GetFieldByName(0, "margin").c_str());
					long prevbalance = atol(rs_position.GetFieldByName(0, "prevbalance").c_str());
					long changed = atol(rs_position.GetFieldByName(0, "changed").c_str());

					if (account_info.PrevBalance == prevbalance ||
						account_info.Changed == changed) {
						/* don't merge */
						account_info.FreeMargin = freemargin;
						account_info.Profit = profit;
						account_info.Margin = margin;
						account_info.Balance = account_info.FreeMargin + account_info.Margin;
					} else {
						account_info.FreeMargin = profit + account_info.PrevBalance + account_info.Changed - margin;
						account_info.Profit = profit;
						account_info.Margin = margin;
						account_info.Balance = account_info.FreeMargin + account_info.Margin;
					}
				} else {
					if (rows == 0) {
						account_info.Status = ACCOUNT_BIT_PULL_NOT_EXIST;
					} else {
						account_info.Status = ACCOUNT_BIT_ALL_ERROR;
					}
				}
			} else {
				/* redis exist */
				long freemargin = atol(redis->HGet(key, "freemargin").c_str());
				long profit = atol(redis->HGet(key, "profit").c_str());
				long margin = atol(redis->HGet(key, "margin").c_str());
				long prevbalance = atol(redis->HGet(key, "prevbalance").c_str());
				long changed = atol(redis->HGet(key, "changed").c_str());
				g_redisPool->ReleaseObject(redis);

				account_info.Status = ACCOUNT_BIT_SUCCESS;
				if (account_info.PrevBalance == prevbalance &&
					account_info.Changed == changed) {
					/* don't merge */
					account_info.FreeMargin = freemargin;
					account_info.Profit = profit;
					account_info.Margin = margin;
					account_info.Balance = account_info.FreeMargin + account_info.Margin;
				} else {
					account_info.FreeMargin = profit + account_info.PrevBalance + account_info.Changed - margin;
					account_info.Profit = profit;
					account_info.Margin = margin;
					account_info.Balance = account_info.FreeMargin + account_info.Margin;
				}
			}

			if (account_info.Margin == 0) {
				asprintf(&sqlbuf, POSITION_DELETE_PULL_FORMAT, 
						 account_info.GroupID, 
						 account_info.ClientID);
#if !defined(MULTIPLE_THREAD)
				updateThread = g_threadPool->Enqueue([sqlbuf] {
#endif
					snetwork_xservice_db::MySqlDB db;
					db.Open();
					db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());
					db.StartTransaction();

					ssize_t rows = db.ExecSql(sqlbuf);
					if (-1 == rows) {
						XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
						db.RollBack();
					} else {
						if (rows == 1) {
							redis = g_redisPool->GetObject();
							redis->Del(key);
							g_redisPool->ReleaseObject(redis);
						}
						db.Commint();
					}
					/* logout, delete mysql record */
					db.Close();
					free(sqlbuf);

#if !defined(MULTIPLE_THREAD)
					return rows;
				});
#endif
				sqlbuf = nullptr;
#if !defined(MULTIPLE_THREAD)
				updateThread.wait();
				updateRow = updateThread;
#else
				updateRow = rows;
#endif
				/*publish PULL.CLIENTMANAGE*/
				//account_info.Status = ACCOUNT_BIT_SUCCESS;
				if (-1 == updateRow) {
					account_info.Status = ACCOUNT_BIT_PULL_NOT_EXIST;
				} else {
					account_info.Status = ACCOUNT_BIT_SUCCESS ;
				}
			} else {
				account_info.SGID = 0;
				asprintf(&sqlbuf, POSITION_UPDATE_PULL_FORMAT,
						 account_info.Balance, 
						 account_info.FreeMargin,
						 account_info.PrevBalance,
						 account_info.Changed,
						 account_info.SGID,
						 account_info.GroupID, 
						 account_info.ClientID);
#if !defined(MULTIPLE_THREAD)
				updateThread = g_threadPool->Enqueue([sqlbuf] {
#endif
					snetwork_xservice_db::MySqlDB db;
					db.Open();
					db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

					db.StartTransaction();
					ssize_t rows = db.ExecSql(sqlbuf);
					if (rows == -1) {
						XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
					}
					db.Commint();
					db.Close();
					free(sqlbuf);

#if !defined(MULTIPLE_THREAD)
					return rows;
				});
#endif
				sqlbuf = nullptr;

#if !defined(MULTIPLE_THREAD)
				updateThread.wait();
				updateRow = updateThread;
#else
				updateRow = rows;
#endif

				if (updateRow == 1) {
					account_info.Status = ACCOUNT_BIT_SUCCESS;
				} else {
					account_info.Status = ACCOUNT_BIT_ALL_ERROR;
				}
			}

			TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL.c_str(),
														   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT.c_str(), 
														   account_info.ToJson(), 
														   strlen(account_info.ToJson())+1, 
														   ETag::EPULLANS);

			return;
		}
	} while (false);

	/*2. query validate , if exists, modify; otherwise, insert it*/
	//snetwork_xservice_db::Redis* redis = g_redisPool->GetObject();
	redis = g_redisPool->GetObject();
	//char key[64];
	//sprintf(key,"%s_%d_%d", HKEYVALIDATE, account_info.GroupID, account_info.ClientID);
	int exists = redis->ExistsKey(key);
	std::string pro =redis->HGet(key, "profit");
	if (pro.empty() || exists != 1) {
		/*not exist, query mysql */
		account_info.Status = ACCOUNT_BIT_PULL_NOT_EXIST;
		g_redisPool->ReleaseObject(redis);

		asprintf(&sqlbuf, SELECT_VALID_FORMAT, account_info.GroupID, account_info.ClientID);
		MySqlDB db;
		db.Open();
		db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());
		MySqlRecordSet rs_position = db.QuerySql(sqlbuf);

		if (db.IsError()) {
			XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
			db.Close();
		} else {
			db.Close();
		}

		size_t rows = -1;
		rows = rs_position.GetRows();
		if (rows == 0) {
			XWARNING("Empty result|sql=%s", sqlbuf);

			if (account_info.Type == ACCOUNT_BIT_TYPE_INIT) {
				account_info.Status = ACCOUNT_BIT_SUCCESS;

				/* 1. insert record to mysql position_management */
				asprintf(&sqlbuf, INSERT_POSITION_INIT_FORMAT,
						 account_info.GroupID,
						 account_info.ClientID,
						 account_info.AccountID,
						 account_info.Balance,
						 account_info.PrevBalance,
						 account_info.Changed,
						 account_info.Margin,
						 account_info.FreeMargin,
						 account_info.Profit,
						 account_info.Type,
						 account_info.CloseBalance,
						 account_info.Status,
						 account_info.Check,
						 recTime);

#if !defined(MULTIPLE_THREAD)
				updateThread = g_threadPool->Enqueue([sqlbuf] {
#endif
					snetwork_xservice_db::MySqlDB db;
					db.Open();
					db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());
					db.StartTransaction();

					ssize_t rows = db.ExecSql(sqlbuf);
					if (-1 == rows) {
						XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
					}
					db.Commint();
					db.Close();
					free(sqlbuf);

#if !defined(MULTIPLE_THREAD)
					return rows;
				});
#endif
				sqlbuf = nullptr;

#if !defined(MULTIPLE_THREAD)
				updateThread.wait();
				updateRow = updateThread;
#else
				updateRow = rows;
#endif
				if (-1 == updateRow) {
					account_info.Status = ACCOUNT_BIT_ALL_ERROR;
				} else {
					account_info.Status = ACCOUNT_BIT_SUCCESS ;
				}

				TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL.c_str(),
															   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT.c_str(), 
															   account_info.ToJson(), 
															   strlen(account_info.ToJson())+1, 
															   ETag::EPULLANS);
			} else {
				account_info.Status = ACCOUNT_BIT_ALL_ERROR;
				TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL.c_str(),
															   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT.c_str(), 
															   account_info.ToJson(), 
															   strlen(account_info.ToJson())+1, 
															   ETag::EPULLANS);

				PMException pmException(event,account_info.Status, AccountErrMsg[account_info.ErrMsgIndex]);
				pmException.PublishException();
			}

			return;
		} else if(rows == -1) {
			FREE(&sqlbuf);
			account_info.Status = ACCOUNT_BIT_ALL_ERROR;
			TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL.c_str(),
														   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT.c_str(), 
														   account_info.ToJson(), 
														   strlen(account_info.ToJson())+1, 
														   ETag::EPULLANS);

			PMException pmException(event,account_info.Status, AccountErrMsg[account_info.ErrMsgIndex]);
			pmException.PublishException();

			return;
		} else {
			FREE(&sqlbuf);
			account_info.Status = ACCOUNT_BIT_SUCCESS;
			int sgid = atoi(rs_position.GetFieldByName(0, "sgid").c_str());
			long profit = atol(rs_position.GetFieldByName(0, "profit").c_str());
			long margin = atol(rs_position.GetFieldByName(0, "margin").c_str());
			//account_info.FreeMargin = profit + account_info.PrevBalance + account_info.Changed - margin;
			long retFreeMarginTmp = profit + account_info.PrevBalance + account_info.Changed - margin;
			if (retFreeMarginTmp <0) {
				account_info.Status = ACCOUNT_BIT_FREEMARGIN_INVALID;
				TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL.c_str(),
															   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT.c_str(), 
															   account_info.ToJson(), 
															   strlen(account_info.ToJson())+1, 
															   ETag::EPULLANS);

				PMException pmException(event,account_info.Status, AccountErrMsg[account_info.ErrMsgIndex]);
				pmException.PublishException();

				return;
			}
			account_info.FreeMargin = retFreeMarginTmp;
			account_info.Profit = profit;
			account_info.Margin = margin;
			account_info.Balance = account_info.FreeMargin + account_info.Margin;

			asprintf(&sqlbuf, POSITION_UPDATE_PULL_FORMAT,
					 account_info.Balance, 
					 account_info.FreeMargin,
					 account_info.PrevBalance,
					 account_info.Changed,
					 account_info.SGID,
					 account_info.GroupID, 
					 account_info.ClientID);
#if !defined(MULTIPLE_THREAD)
			updateThread = g_threadPool->Enqueue([sqlbuf] {
#endif
				snetwork_xservice_db::MySqlDB db;
				db.Open();
				db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

				db.StartTransaction();
				ssize_t rows = db.ExecSql(sqlbuf);
				if (rows == -1) {
					XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
				}
				db.Commint();
				db.Close();
				free(sqlbuf);

#if !defined(MULTIPLE_THREAD)
				return rows;
			});
#endif
			sqlbuf = nullptr;

#if !defined(MULTIPLE_THREAD)
			updateThread.wait();
			updateRow = updateThread;
#else
			updateRow = rows;
#endif

#if 0
			if (account_info.Type == EPullType::EPLOGOUT ||
				account_info.Type == EPullType::EPFORCELOGOUT) {
				/* logout, delete mysql record */
				asprintf(&sqlbuf, POSITION_DELETE_PULL_FORMAT, 
						 account_info.GroupID, 
						 account_info.ClientID);
#if !defined(MULTIPLE_THREAD)
				g_threadPool->Enqueue([sqlbuf] {
#endif
				snetwork_xservice_db::MySqlDB db;
				db.Open();
				db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());
				db.StartTransaction();

				ssize_t rows = db.ExecSql(sqlbuf);
				if (rows == -1) {
					XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
				}
				db.Commint();
				db.Close();

				free(sqlbuf);
#if !defined(MULTIPLE_THREAD)
			});
#endif
				sqlbuf = nullptr;
			}
#endif
			/*publish PULL.CLIENTMANAGE*/
			TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL.c_str(),
														   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT.c_str(), 
														   account_info.ToJson(), 
														   strlen(account_info.ToJson())+1, 
														   ETag::EPULLANS);

			if (account_info.Type == EPullType::EPDEPOSITE ||
				account_info.Type == EPullType::EPWITHDRAWAL) {
				char validateEvent[VALIDATE_ALLTE_SGID_LEN_EVENT];
				sprintf(validateEvent, "%s.%d", POSITIONMANAGEMENT_PUBLISH_VALIDATE_RM_EVENT.c_str(), sgid);
				//const char* msg_validate = account_info.ToJson();
				ValidateOrder validateOrder(account_info);
				const char* msg_validate = validateOrder.ToJson();
				TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_VALIDATE.c_str(),
															   validateEvent, 
															   msg_validate, 
															   strlen(msg_validate)+1, 
															   ETag::EVALIDATEORDER);
			}
		}

		return;
	} else {
		/*2.2 redis exists, get data from redis */
		int sgid = atol(redis->HGet(key, "sgid").c_str());
		long profit = atol(redis->HGet(key, "profit").c_str());
		long margin = atol(redis->HGet(key, "margin").c_str());
		g_redisPool->ReleaseObject(redis);
		//account_info.FreeMargin = profit + account_info.PrevBalance + account_info.Changed - margin;
		long retFreeMarginTmp = profit + account_info.PrevBalance + account_info.Changed - margin;
		if (retFreeMarginTmp <0) {
			account_info.Status = ACCOUNT_BIT_FREEMARGIN_INVALID;
			TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL.c_str(),
														   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT.c_str(), 
														   account_info.ToJson(), 
														   strlen(account_info.ToJson())+1, 
														   ETag::EPULLANS);

			PMException pmException(event,account_info.Status, AccountErrMsg[account_info.ErrMsgIndex]);
			pmException.PublishException();

			return;
		}
		account_info.FreeMargin = retFreeMarginTmp;
		account_info.Profit = profit;
		account_info.Margin = margin;
		account_info.Balance = account_info.FreeMargin + account_info.Margin;

		// const char* POSITION_UPDATE_PULL_FORMAT = "UPDATE position_management SET balance=%ld,freemargin=%d,prevbalance=%ld,changed=%ld,sgid=%d WHERE groupid=%d AND clientid=%d";
		asprintf(&sqlbuf, POSITION_UPDATE_PULL_FORMAT,
				 account_info.Balance, 
				 account_info.FreeMargin,
				 account_info.PrevBalance,
				 account_info.Changed,
				 account_info.SGID,
				 account_info.GroupID, 
				 account_info.ClientID);
#if !defined(MULTIPLE_THREAD)
		updateThread = g_threadPool->Enqueue([sqlbuf] {
#endif
			snetwork_xservice_db::MySqlDB db;
			db.Open();
			db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());
			db.StartTransaction();

			ssize_t rows = db.ExecSql(sqlbuf);
			if (rows == -1) {
				XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
			}
			db.Commint();
			db.Close();
			free(sqlbuf);

#if !defined(MULTIPLE_THREAD)
			return rows;
		});
#endif
		sqlbuf = nullptr;
#if !defined(MULTIPLE_THREAD)
		updateThread.wait();
		updateRow = updateThread;
#else
		updateRow = rows;
#endif

#if 0
		if (account_info.Type == EPullType::EPLOGOUT || 
			account_info.Type == EPullType::EPFORCELOGOUT) {
			/* logout, delete mysql record */
			redis = g_redisPool->GetObject();
			redis->Del(key);
			g_redisPool->ReleaseObject(redis);

			asprintf(&sqlbuf, POSITION_DELETE_PULL_FORMAT, 
					 account_info.GroupID, 
					 account_info.ClientID);
#if !defined(MULTIPLE_THREAD)
			g_threadPool->Enqueue([sqlbuf] {
#endif
				snetwork_xservice_db::MySqlDB db;
				db.Open();
				db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

				ssize_t rows = db.ExecSql(sqlbuf);
				db.Close();
				free(sqlbuf);
#if !defined(MULTIPLE_THREAD)
			});
#endif
			sqlbuf = nullptr;
		}
#endif
		/*publish PULL.CLIENTMANAGE*/
		account_info.Status = ACCOUNT_BIT_SUCCESS;
		TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL.c_str(),
													   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT.c_str(), 
													   account_info.ToJson(), 
													   strlen(account_info.ToJson())+1, 
													   ETag::EPULLANS);

		if (account_info.Type == EPullType::EPDEPOSITE ||
			account_info.Type == EPullType::EPWITHDRAWAL) {
			char validateEvent[VALIDATE_ALLTE_SGID_LEN_EVENT];
			sprintf(validateEvent, "%s.%d", POSITIONMANAGEMENT_PUBLISH_VALIDATE_RM_EVENT.c_str(), sgid);
			//const char* msg_validate = account_info.ToJson();
			ValidateOrder validateOrder(account_info);
			const char* msg_validate = validateOrder.ToJson();
			TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_VALIDATE.c_str(),
														   validateEvent, 
														   msg_validate, 
														   strlen(msg_validate)+1, 
														   ETag::EVALIDATEORDER);
		}
		//g_redisPool->ReleaseObject(redis);
	}
}

#if 0
void SubscribePull::Done(const char* event, unsigned int eventLen) {
	long recTime = GetMicroSecond();
#if defined(DEBUG)
	XINFO("msg:%s, len:%d\n", event, eventLen);
#endif
	PositionAccount account_info;
	snetwork_xservice_db::Redis* redis = nullptr;
	char key[64];
	memset(key, 0, sizeof(key));
	rapidjson::Document d;
	bool flag = false;

#if !defined(MULTIPLE_THREAD)
	std::future<ssize_t> updateThread;
#endif
	ssize_t updateRow = 0;

	do {
		d.Parse(event, eventLen);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error|event=%s", event);

			flag = true;
			break;
		}

		rapidjson::Value::ConstMemberIterator it;

		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		const char* reqid = it->value.GetString();
		memcpy(account_info.ReqID, reqid, REQUEST_ID_LEN);

		if(((it = d.FindMember("sessionid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		const char* sessionid = it->value.GetString();
		memcpy(account_info.SessionID, sessionid, SESSION_ID_LEN);

		if(((it = d.FindMember("groupid")) == d.MemberEnd()) || !it->value.IsUint()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.GroupID = it->value.GetUint(); 

		if(((it = d.FindMember("clientid")) == d.MemberEnd()) || !it->value.IsUint()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.ClientID = it->value.GetUint(); 

		if(((it = d.FindMember("sgid")) == d.MemberEnd()) || !it->value.IsUint()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.SGID = it->value.GetUint(); 

		if(((it = d.FindMember("accountid")) == d.MemberEnd()) || !it->value.IsUint64()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.AccountID = it->value.GetUint64(); 

		if(((it = d.FindMember("prevbalance")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.PrevBalance = it->value.GetInt64(); 

		if(((it = d.FindMember("changed")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.Changed = it->value.GetInt64();

		if(((it = d.FindMember("margin")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.Margin = it->value.GetInt64();

		if(((it = d.FindMember("freemargin")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.FreeMargin = it->value.GetInt64();

		if(((it = d.FindMember("profit")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.Profit = it->value.GetInt64();

		if(((it = d.FindMember("closebalance")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.CloseBalance = it->value.GetInt64();

		if(((it = d.FindMember("status")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.Status = it->value.GetInt();

		if(((it = d.FindMember("type")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.Type = it->value.GetInt();
	} while(false);
	
	sprintf(key,"%s_%d_%d", HKEYVALIDATE, account_info.GroupID, account_info.ClientID);
	if (true == flag) {
		account_info.Status = ACCOUNT_BIT_FORMAT;

		TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL,
													   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT, 
													   account_info.ToJson(), 
													   strlen(account_info.ToJson())+1, 
													   ETag::EPULLANS);
		PMException pmException(event,account_info.Status, AccountErrMsg[account_info.ErrMsgIndex]);
		pmException.PublishException();

		return;
	}

	int status = 0;
	if (account_info.VerifyData(status)) { 
		/* error */
		account_info.Status = status;
		account_info.Check = 0;
		TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL,
													   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT, 
													   account_info.ToJson(), 
													   strlen(account_info.ToJson())+1, 
													   ETag::EPULLANS);

		PMException pmException(event,account_info.Status, AccountErrMsg[account_info.ErrMsgIndex]);
		pmException.PublishException();

		return;
	} else {
		account_info.Check = 1;
		account_info.Status = status;
		account_info.Balance = account_info.FreeMargin + account_info.Margin;
	}

	ssize_t rows = 0;
	char* sqlbuf = nullptr;
	CDateTime dateTime;
	char* tmpDay = dateTime.AddDay(0);
	int day = atoi(tmpDay);
	FREE(&tmpDay);
	//char* tmpTime = dateTime.GetDateTime(CDateTime::EDATETIME);

	/* 1. insert record to mysql as log */
	asprintf(&sqlbuf, INSERT_POSITION_LOG_FORMAT,
			 day,
			 account_info.ReqID,
			 account_info.SessionID,
			 account_info.GroupID,
			 account_info.ClientID,
			 account_info.SGID,
			 account_info.AccountID,
			 account_info.Balance,
			 account_info.PrevBalance,
			 account_info.Changed,
			 account_info.Margin,
			 account_info.FreeMargin,
			 account_info.Profit,
			 account_info.Type,
			 account_info.CloseBalance,
			 account_info.Status,
			 account_info.Check,
			 //tmpTime);
			 //GetMicroSecond());
			 recTime);
#if !defined(MULTIPLE_THREAD)
	g_threadPool->Enqueue([sqlbuf] {
#endif
		snetwork_xservice_db::MySqlDB db;
		db.Open();
		//db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());
		//dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig());
		db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

#if !defined(MULTIPLE_THREAD)
		ssize_t rows = db.ExecSql(sqlbuf);
#else
		rows = db.ExecSql(sqlbuf);
#endif
		db.Close();

		free(sqlbuf);
#if !defined(MULTIPLE_THREAD)
	});
#endif
	sqlbuf = nullptr;
	
	/* type:
	 * 0: default
	 * 1: init(first asyn: CM->PM)
	 * 2: deposite
	 * 4: withdrawal
	 * 8: feedback(PM->CM, PM push to CM)
	 * 16:userinfo
	 * 32: logout
	 * 64: forcelogout
	 */
	do {
		if (EPullType::EPINIT == account_info.Type) {
			/* If it is first login, insert record into database */
			/* 1. insert record to mysql position_management */
			asprintf(&sqlbuf, INSERT_POSITION_INIT_FORMAT,
					 account_info.GroupID,
					 account_info.ClientID,
					 account_info.SGID,
					 account_info.AccountID,
					 account_info.Balance,
					 account_info.PrevBalance,
					 account_info.Changed,
					 account_info.Margin,
					 account_info.FreeMargin,
					 account_info.Profit,
					 account_info.Type,
					 account_info.CloseBalance,
					 account_info.Status,
					 account_info.Check,
					 recTime);
#if !defined(MULTIPLE_THREAD)
			updateThread = g_threadPool->Enqueue([sqlbuf] {
#endif
				snetwork_xservice_db::MySqlDB db;
				db.Open();
				db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());
				db.StartTransaction();

				ssize_t rows = db.ExecSql(sqlbuf);
				if (-1 == rows) {
					XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
				}
				db.Commint();
				db.Close();

				free(sqlbuf);
#if !defined(MULTIPLE_THREAD)

				return rows;
			});
#endif
			sqlbuf = nullptr;

#if !defined(MULTIPLE_THREAD)
			updateThread.wait();
			updateRow = updateThread;
#else
			updateRow = rows;
#endif

			if (-1 == updateRow) {
				account_info.Status = ACCOUNT_BIT_PULL_INSERT_EXIST;
			} else {
				account_info.Status = ACCOUNT_BIT_SUCCESS ;
			}
			account_info.Check = 0;
			TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL,
													   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT, 
													   account_info.ToJson(), 
													   strlen(account_info.ToJson())+1, 
													   ETag::EPULLANS);
			//break;
			return;
		}

		if (account_info.Type == EPullType::EPLOGOUT || 
			account_info.Type == EPullType::EPFORCELOGOUT) {

			asprintf(&sqlbuf, POSITION_DELETE_PULL_FORMAT, 
					 account_info.GroupID, 
					 account_info.ClientID);
#if !defined(MULTIPLE_THREAD)
			updateThread = g_threadPool->Enqueue([sqlbuf] {
#endif
				snetwork_xservice_db::MySqlDB db;
				db.Open();
				db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());
				db.StartTransaction();

				ssize_t rows = db.ExecSql(sqlbuf);
				if (-1 == rows) {
					XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
				}
				db.Commint();
				/* logout, delete mysql record */
				redis = g_redisPool->GetObject();
				redis->Del(key);
				g_redisPool->ReleaseObject(redis);
				db.Close();
				free(sqlbuf);

#if !defined(MULTIPLE_THREAD)
				return rows;
			});
#endif
			sqlbuf = nullptr;
#if !defined(MULTIPLE_THREAD)
			updateThread.wait();
			updateRow = updateThread;
#else
			updateRow = rows;
#endif
			/*publish PULL.CLIENTMANAGE*/
			//account_info.Status = ACCOUNT_BIT_SUCCESS;
			if (-1 == updateRow) {
				account_info.Status = ACCOUNT_BIT_PULL_NOT_EXIST;
			} else {
				account_info.Status = ACCOUNT_BIT_SUCCESS ;
			}
			TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL,
														   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT, 
														   account_info.ToJson(), 
														   strlen(account_info.ToJson())+1, 
														   ETag::EPULLANS);

			return;
		}
	} while (false);

	/*2. query validate , if exists, modify; otherwise, insert it*/
	//snetwork_xservice_db::Redis* redis = g_redisPool->GetObject();
	redis = g_redisPool->GetObject();
	//char key[64];
	//sprintf(key,"%s_%d_%d", HKEYVALIDATE, account_info.GroupID, account_info.ClientID);
	int exists = redis->ExistsKey(key);
	std::string pro =redis->HGet(key, "profit");
	if (pro.empty() || exists != 1) {
		/*not exist, query mysql */
		account_info.Status = ACCOUNT_BIT_PULL_NOT_EXIST;
		g_redisPool->ReleaseObject(redis);

		asprintf(&sqlbuf, SELECT_VALID_FORMAT, account_info.GroupID, account_info.ClientID);
		MySqlDB db;
		db.Open();
		db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());
		MySqlRecordSet rs_position = db.QuerySql(sqlbuf);

		if (db.IsError()) {
			XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
			db.Close();
		} else {
			db.Close();
		}

		size_t rows = -1;
		rows = rs_position.GetRows();
		if (rows == 0) {
			XWARNING("Empty result|sql=%s", sqlbuf);

			if (account_info.Type == ACCOUNT_BIT_TYPE_INIT) {
				account_info.Status = ACCOUNT_BIT_SUCCESS;

				/* 1. insert record to mysql position_management */
				asprintf(&sqlbuf, INSERT_POSITION_INIT_FORMAT,
						 account_info.GroupID,
						 account_info.ClientID,
						 account_info.AccountID,
						 account_info.Balance,
						 account_info.PrevBalance,
						 account_info.Changed,
						 account_info.Margin,
						 account_info.FreeMargin,
						 account_info.Profit,
						 account_info.Type,
						 account_info.CloseBalance,
						 account_info.Status,
						 account_info.Check,
						 recTime);

#if !defined(MULTIPLE_THREAD)
				updateThread = g_threadPool->Enqueue([sqlbuf] {
#endif
					snetwork_xservice_db::MySqlDB db;
					db.Open();
					db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());
					db.StartTransaction();

					ssize_t rows = db.ExecSql(sqlbuf);
					if (-1 == rows) {
						XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
					}
					db.Commint();
					db.Close();
					free(sqlbuf);

#if !defined(MULTIPLE_THREAD)
					return rows;
				});
#endif
				sqlbuf = nullptr;

#if !defined(MULTIPLE_THREAD)
				updateThread.wait();
				updateRow = updateThread;
#else
				updateRow = rows;
#endif
				if (-1 == updateRow) {
					account_info.Status = ACCOUNT_BIT_ALL_ERROR;
				} else {
					account_info.Status = ACCOUNT_BIT_SUCCESS ;
				}

				TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL,
															   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT, 
															   account_info.ToJson(), 
															   strlen(account_info.ToJson())+1, 
															   ETag::EPULLANS);
			} else {
				account_info.Status = ACCOUNT_BIT_ALL_ERROR;
				TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL,
															   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT, 
															   account_info.ToJson(), 
															   strlen(account_info.ToJson())+1, 
															   ETag::EPULLANS);

				PMException pmException(event,account_info.Status, AccountErrMsg[account_info.ErrMsgIndex]);
				pmException.PublishException();
			}

			return;
		} else if(rows == -1) {
			account_info.Status = ACCOUNT_BIT_ALL_ERROR;
			TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL,
														   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT, 
														   account_info.ToJson(), 
														   strlen(account_info.ToJson())+1, 
														   ETag::EPULLANS);

			PMException pmException(event,account_info.Status, AccountErrMsg[account_info.ErrMsgIndex]);
			pmException.PublishException();

			return;
		} else {
			FREE(&sqlbuf);
			account_info.Status = ACCOUNT_BIT_SUCCESS;
			long profit = atol(rs_position.GetFieldByName(0, "profit").c_str());
			long margin = atol(rs_position.GetFieldByName(0, "margin").c_str());
			account_info.FreeMargin = profit + account_info.PrevBalance + account_info.Changed - margin;
			account_info.Profit = profit;
			account_info.Margin = margin;
			account_info.Balance = account_info.FreeMargin + account_info.Margin;

			asprintf(&sqlbuf, POSITION_UPDATE_PULL_FORMAT,
					 account_info.Balance, 
					 account_info.FreeMargin,
					 account_info.PrevBalance,
					 account_info.Changed,
					 account_info.SGID,
					 account_info.GroupID, 
					 account_info.ClientID);
#if !defined(MULTIPLE_THREAD)
			updateThread = g_threadPool->Enqueue([sqlbuf] {
#endif
				snetwork_xservice_db::MySqlDB db;
				db.Open();
				db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

				db.StartTransaction();
				ssize_t rows = db.ExecSql(sqlbuf);
				if (rows == -1) {
					XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
				}
				db.Commint();
				db.Close();
				free(sqlbuf);

#if !defined(MULTIPLE_THREAD)
				return rows;
			});
#endif
			sqlbuf = nullptr;

#if !defined(MULTIPLE_THREAD)
			updateThread.wait();
			updateRow = updateThread;
#else
			updateRow = rows;
#endif

#if 0
			if (account_info.Type == EPullType::EPLOGOUT ||
				account_info.Type == EPullType::EPFORCELOGOUT) {
				/* logout, delete mysql record */
				asprintf(&sqlbuf, POSITION_DELETE_PULL_FORMAT, 
						 account_info.GroupID, 
						 account_info.ClientID);
#if !defined(MULTIPLE_THREAD)
				g_threadPool->Enqueue([sqlbuf] {
#endif
				snetwork_xservice_db::MySqlDB db;
				db.Open();
				db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());
				db.StartTransaction();

				ssize_t rows = db.ExecSql(sqlbuf);
				if (rows == -1) {
					XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
				}
				db.Commint();
				db.Close();

				free(sqlbuf);
#if !defined(MULTIPLE_THREAD)
			});
#endif
				sqlbuf = nullptr;
			}
#endif
			/*publish PULL.CLIENTMANAGE*/
			TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL,
														   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT, 
														   account_info.ToJson(), 
														   strlen(account_info.ToJson())+1, 
														   ETag::EPULLANS);
		}

		return;
	} else {
		/*2.2 redis exists, get data from redis */
		long profit = atol(redis->HGet(key, "profit").c_str());
		long margin = atol(redis->HGet(key, "margin").c_str());
		g_redisPool->ReleaseObject(redis);
		account_info.FreeMargin = profit + account_info.PrevBalance + account_info.Changed - margin;
		account_info.Profit = profit;
		account_info.Margin = margin;
		account_info.Balance = account_info.FreeMargin + account_info.Margin;

		// const char* POSITION_UPDATE_PULL_FORMAT = "UPDATE position_management SET balance=%ld,freemargin=%d,prevbalance=%ld,changed=%ld,sgid=%d WHERE groupid=%d AND clientid=%d";
		asprintf(&sqlbuf, POSITION_UPDATE_PULL_FORMAT,
				 account_info.Balance, 
				 account_info.FreeMargin,
				 account_info.PrevBalance,
				 account_info.Changed,
				 account_info.SGID,
				 account_info.GroupID, 
				 account_info.ClientID);
#if !defined(MULTIPLE_THREAD)
		updateThread = g_threadPool->Enqueue([sqlbuf] {
#endif
			snetwork_xservice_db::MySqlDB db;
			db.Open();
			db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());
			db.StartTransaction();

			ssize_t rows = db.ExecSql(sqlbuf);
			if (rows == -1) {
				XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
			}
			db.Commint();
			db.Close();
			free(sqlbuf);

#if !defined(MULTIPLE_THREAD)
			return rows;
		});
#endif
		sqlbuf = nullptr;
#if !defined(MULTIPLE_THREAD)
		updateThread.wait();
		updateRow = updateThread;
#else
		updateRow = rows;
#endif

#if 0
		if (account_info.Type == EPullType::EPLOGOUT || 
			account_info.Type == EPullType::EPFORCELOGOUT) {
			/* logout, delete mysql record */
			redis = g_redisPool->GetObject();
			redis->Del(key);
			g_redisPool->ReleaseObject(redis);

			asprintf(&sqlbuf, POSITION_DELETE_PULL_FORMAT, 
					 account_info.GroupID, 
					 account_info.ClientID);
#if !defined(MULTIPLE_THREAD)
			g_threadPool->Enqueue([sqlbuf] {
#endif
				snetwork_xservice_db::MySqlDB db;
				db.Open();
				db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

				ssize_t rows = db.ExecSql(sqlbuf);
				db.Close();
				free(sqlbuf);
#if !defined(MULTIPLE_THREAD)
			});
#endif
			sqlbuf = nullptr;
		}
#endif
		/*publish PULL.CLIENTMANAGE*/
		account_info.Status = ACCOUNT_BIT_SUCCESS;
		TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL,
													   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT, 
													   account_info.ToJson(), 
													   strlen(account_info.ToJson())+1, 
													   ETag::EPULLANS);
		//g_redisPool->ReleaseObject(redis);
	}
}
#endif

#if 0
void SubscribePull::Done(const char* event, unsigned int eventLen) {
	long recTime = GetMicroSecond();
#if defined(DEBUG)
	XINFO("msg:%s, len:%d\n", event, eventLen);
#endif
	PositionAccount account_info;
	rapidjson::Document d;
	bool flag = false;

	do {
		d.Parse(event, eventLen);
		if (d.HasParseError() || !d.IsObject()) {
			XERROR("parser json error|event=%s", event);

			flag = true;
			break;
		}

		rapidjson::Value::ConstMemberIterator it;

		if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		const char* reqid = it->value.GetString();
		memcpy(account_info.ReqID, reqid, REQUEST_ID_LEN);

		if(((it = d.FindMember("sessionid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		const char* sessionid = it->value.GetString();
		memcpy(account_info.SessionID, sessionid, SESSION_ID_LEN);

		if(((it = d.FindMember("groupid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.GroupID = it->value.GetInt(); 

		if(((it = d.FindMember("clientid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.ClientID = it->value.GetInt(); 

		if(((it = d.FindMember("accountid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.AccountID = it->value.GetInt(); 

		if(((it = d.FindMember("prevbalance")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.PrevBalance = it->value.GetInt64(); 

		if(((it = d.FindMember("changed")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.Changed = it->value.GetInt64();

		if(((it = d.FindMember("margin")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.Margin = it->value.GetInt64();

		if(((it = d.FindMember("freemargin")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.FreeMargin = it->value.GetInt64();

		if(((it = d.FindMember("profit")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.Profit = it->value.GetInt64();

		if(((it = d.FindMember("closebalance")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.CloseBalance = it->value.GetInt64();

		if(((it = d.FindMember("status")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.Status = it->value.GetInt();

		if(((it = d.FindMember("type")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)	
			XINFO("field no exit or type error");
#endif

			flag = true;
			break;
		}
		account_info.Type = it->value.GetInt();
	} while(false);
	
	if (true == flag) {
		account_info.Status = ACCOUNT_BIT_FORMAT;

		TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL,
													   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT, 
													   account_info.ToJson(), 
													   strlen(account_info.ToJson())+1, 
													   ETag::EPULLANS);
		PMException pmException(event,account_info.Status, AccountErrMsg[account_info.ErrMsgIndex]);
		pmException.PublishException();

		return;
	}

	int status = 0;
	if (account_info.VerifyData(status)) { 
		/* error */
		account_info.Status = status;
		account_info.Check = 0;
		TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL,
													   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT, 
													   account_info.ToJson(), 
													   strlen(account_info.ToJson())+1, 
													   ETag::EPULLANS);

		PMException pmException(event,account_info.Status, AccountErrMsg[account_info.ErrMsgIndex]);
		pmException.PublishException();

		return;
	} else {
		account_info.Check = 1;
		account_info.Status = status;
		account_info.Balance = account_info.FreeMargin + account_info.Margin;
	}

	ssize_t rows = 0;
	char* sqlbuf = nullptr;
	CDateTime dateTime;
	char* tmpDay = dateTime.AddDay(0);
	int day = atoi(tmpDay);
	FREE(&tmpDay);
	//char* tmpTime = dateTime.GetDateTime(CDateTime::EDATETIME);

	/* 1. insert record to mysql as log */
	asprintf(&sqlbuf, INSERT_POSITION_LOG_FORMAT,
			 day,
			 account_info.ReqID,
			 account_info.SessionID,
			 account_info.GroupID,
			 account_info.ClientID,
			 account_info.AccountID,
			 account_info.Balance,
			 account_info.PrevBalance,
			 account_info.Changed,
			 account_info.Margin,
			 account_info.FreeMargin,
			 account_info.Profit,
			 account_info.Type,
			 account_info.CloseBalance,
			 account_info.Status,
			 account_info.Check,
			 //tmpTime);
			 //GetMicroSecond());
			 recTime);
#if !defined(MULTIPLE_THREAD)
	g_threadPool->Enqueue([sqlbuf] {
#endif
		snetwork_xservice_db::MySqlDB db;
		db.Open();
		//db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());
		//dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig());
		db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

#if !defined(MULTIPLE_THREAD)
		ssize_t rows = db.ExecSql(sqlbuf);
#else
		rows = db.ExecSql(sqlbuf);
#endif
		db.Close();

		free(sqlbuf);
#if !defined(MULTIPLE_THREAD)
	});
#endif
	sqlbuf = nullptr;
	
	/* type:
	 * 0: default
	 * 1: init(first asyn: CM->PM)
	 * 2: deposite
	 * 4: withdrawal
	 * 8: feedback(PM->CM, PM push to CM)
	 * 16:userinfo
	 */
	do {
		if (EPullType::EPINIT == account_info.Type) {
			/* If it is first login, insert record into database */
			/* 1. insert record to mysql position_management */
			asprintf(&sqlbuf, INSERT_POSITION_INIT_FORMAT,
					 account_info.GroupID,
					 account_info.ClientID,
					 account_info.AccountID,
					 account_info.Balance,
					 account_info.PrevBalance,
					 account_info.Changed,
					 account_info.Margin,
					 account_info.FreeMargin,
					 account_info.Profit,
					 account_info.Type,
					 account_info.CloseBalance,
					 account_info.Status,
					 account_info.Check,
					 recTime);
#if !defined(MULTIPLE_THREAD)
			g_threadPool->Enqueue([sqlbuf] {
#endif
				snetwork_xservice_db::MySqlDB db;
				db.Open();
				db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

				ssize_t rows = db.ExecSql(sqlbuf);
				db.Close();

				free(sqlbuf);
#if !defined(MULTIPLE_THREAD)
			});
#endif
			sqlbuf = nullptr;

			account_info.Status = ACCOUNT_BIT_SUCCESS ;
			account_info.Check = 0;
			TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL,
													   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT, 
													   account_info.ToJson(), 
													   strlen(account_info.ToJson())+1, 
													   ETag::EPULLANS);
			//break;
			return;
		}
	} while (false);

	/*2. query validate , if exists, modify; otherwise, insert it*/
	snetwork_xservice_db::Redis* redis = g_redisPool->GetObject();
	char key[64];
	sprintf(key,"%s_%d_%d", HKEYVALIDATE, account_info.GroupID, account_info.ClientID);
	int exists = redis->ExistsKey(key);
	std::string pro =redis->HGet(key, "profit");
	if (pro.empty() || exists != 1) {
		/*not exist, query mysql */
		account_info.Status = ACCOUNT_BIT_PULL_NOT_EXIST;
		g_redisPool->ReleaseObject(redis);

		MySqlDB db;
		db.Open();
		db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

		asprintf(&sqlbuf, SELECT_VALID_FORMAT, account_info.GroupID, account_info.ClientID);
		MySqlRecordSet rs_position = db.QuerySql(sqlbuf);
		if (db.IsError()) {
			XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
			db.Close();
		} else {
			db.Close();
		}

		size_t rows = -1;
		rows = rs_position.GetRows();
		if (rows == 0) {
			XWARNING("Empty result|sql=%s", sqlbuf);

			if (account_info.Type == ACCOUNT_BIT_TYPE_INIT) {
				account_info.Status = ACCOUNT_BIT_SUCCESS;

				/* 1. insert record to mysql position_management */
				asprintf(&sqlbuf, INSERT_POSITION_INIT_FORMAT,
						 account_info.GroupID,
						 account_info.ClientID,
						 account_info.AccountID,
						 account_info.Balance,
						 account_info.PrevBalance,
						 account_info.Changed,
						 account_info.Margin,
						 account_info.FreeMargin,
						 account_info.Profit,
						 account_info.Type,
						 account_info.CloseBalance,
						 account_info.Status,
						 account_info.Check,
						 recTime);

#if !defined(MULTIPLE_THREAD)
				g_threadPool->Enqueue([sqlbuf] {
#endif
					snetwork_xservice_db::MySqlDB db;
					db.Open();
					db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

					ssize_t rows = db.ExecSql(sqlbuf);
					db.Close();

					free(sqlbuf);
#if !defined(MULTIPLE_THREAD)
				});
#endif
				sqlbuf = nullptr;
				
				TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL,
															   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT, 
															   account_info.ToJson(), 
															   strlen(account_info.ToJson())+1, 
															   ETag::EPULLANS);
			} else {

				//account_info.Status = ACCOUNT_BIT_ALL_ERROR;
				TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL,
															   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT, 
															   account_info.ToJson(), 
															   strlen(account_info.ToJson())+1, 
															   ETag::EPULLANS);

				PMException pmException(event,account_info.Status, AccountErrMsg[account_info.ErrMsgIndex]);
				pmException.PublishException();
			}

			return;
		} else if(rows == -1) {
			account_info.Status = ACCOUNT_BIT_ALL_ERROR;
			TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL,
														   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT, 
														   account_info.ToJson(), 
														   strlen(account_info.ToJson())+1, 
														   ETag::EPULLANS);

			PMException pmException(event,account_info.Status, AccountErrMsg[account_info.ErrMsgIndex]);
			pmException.PublishException();

			return;
		} else {
			FREE(&sqlbuf);
			account_info.Status = ACCOUNT_BIT_SUCCESS;
			long profit = atol(rs_position.GetFieldByName(0, "profit").c_str());
			long margin = atol(rs_position.GetFieldByName(0, "margin").c_str());
			account_info.FreeMargin = profit + account_info.PrevBalance + account_info.Changed - margin;
			account_info.Profit = profit;
			account_info.Margin = margin;
			account_info.Balance = account_info.FreeMargin + account_info.Margin;

			asprintf(&sqlbuf, POSITION_UPDATE_PULL_FORMAT,
					 account_info.Balance, 
					 account_info.FreeMargin,
					 account_info.PrevBalance,
					 account_info.Changed,
					 account_info.SGID,
					 account_info.GroupID, 
					 account_info.ClientID);
#if !defined(MULTIPLE_THREAD)
			g_threadPool->Enqueue([sqlbuf] {
#endif
				snetwork_xservice_db::MySqlDB db;
				db.Open();
				db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

				db.StartTransaction();
				ssize_t rows = db.ExecSql(sqlbuf);
				if (rows == -1) {
					XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
				}
				db.Commint();
				db.Close();

				free(sqlbuf);
#if !defined(MULTIPLE_THREAD)
			});
#endif
			sqlbuf = nullptr;

			if (account_info.Type == EPullType::EPLOGOUT ||
				account_info.Type == EPullType::EPFORCELOGOUT) {
				/* logout, delete mysql record */
				asprintf(&sqlbuf, POSITION_DELETE_PULL_FORMAT, 
						 account_info.GroupID, 
						 account_info.ClientID);
#if !defined(MULTIPLE_THREAD)
				g_threadPool->Enqueue([sqlbuf] {
#endif
				snetwork_xservice_db::MySqlDB db;
				db.Open();
				db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

				ssize_t rows = db.ExecSql(sqlbuf);
				db.Close();

				free(sqlbuf);
#if !defined(MULTIPLE_THREAD)
			});
#endif
				sqlbuf = nullptr;
			}

			/*publish PULL.CLIENTMANAGE*/
			TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL,
														   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT, 
														   account_info.ToJson(), 
														   strlen(account_info.ToJson())+1, 
														   ETag::EPULLANS);
		}

		return;
	} else {
		/*2.2 redis exists, get data from redis */
		long profit = atol(redis->HGet(key, "profit").c_str());
		long margin = atol(redis->HGet(key, "margin").c_str());
		g_redisPool->ReleaseObject(redis);
		account_info.FreeMargin = profit + account_info.PrevBalance + account_info.Changed - margin;
		account_info.Margin = margin;
		account_info.Profit = profit;
		account_info.Balance = account_info.FreeMargin + account_info.Margin;

		asprintf(&sqlbuf, POSITION_UPDATE_PULL_FORMAT,
				 account_info.Balance, 
				 account_info.FreeMargin,
				 account_info.PrevBalance,
				 account_info.Changed,
				 account_info.SGID,
				 account_info.GroupID, 
				 account_info.ClientID);
#if !defined(MULTIPLE_THREAD)
		g_threadPool->Enqueue([sqlbuf] {
#endif
			snetwork_xservice_db::MySqlDB db;
			db.Open();
			db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

			db.StartTransaction();
			ssize_t rows = db.ExecSql(sqlbuf);
			if (rows == -1) {
				XERROR("%s,con=%d|sql=%s", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
			}
			db.Commint();
			db.Close();

			free(sqlbuf);
#if !defined(MULTIPLE_THREAD)
		});
#endif
		sqlbuf = nullptr;

		if (account_info.Type == EPullType::EPLOGOUT || 
			account_info.Type == EPullType::EPFORCELOGOUT) {
			/* logout, delete mysql record */
			redis = g_redisPool->GetObject();
			redis->Del(key);
			g_redisPool->ReleaseObject(redis);

			asprintf(&sqlbuf, POSITION_DELETE_PULL_FORMAT, 
					 account_info.GroupID, 
					 account_info.ClientID);
#if !defined(MULTIPLE_THREAD)
			g_threadPool->Enqueue([sqlbuf] {
#endif
				snetwork_xservice_db::MySqlDB db;
				db.Open();
				db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

				ssize_t rows = db.ExecSql(sqlbuf);
				db.Close();
				free(sqlbuf);
#if !defined(MULTIPLE_THREAD)
			});
#endif
			sqlbuf = nullptr;
		}

#if 0
		redis = g_redisPool->GetObject();
		redis->Del(key);
#endif
		/*publish PULL.CLIENTMANAGE*/
		account_info.Status = ACCOUNT_BIT_SUCCESS;
		TNodeAministrator::GetInstance()->PublishGroup(POSITIONMANAGEMENT_PUBLISH_PULL,
													   POSITIONMANAGEMENT_PUBLISH_POSITION_EVENT, 
													   account_info.ToJson(), 
													   strlen(account_info.ToJson())+1, 
													   ETag::EPULLANS);
		//g_redisPool->ReleaseObject(redis);
	}
}
#endif
/*SubscribePull end ****/

/*SubscribeMonitor begin ****/
SubscribeEchoReq::SubscribeEchoReq(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeEchoReq::Done(char* event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("msg:%s, len:%d\n", event, eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse(event, eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error\n");

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
            XINFO("field no exit or type error");
#endif

			return;
	}
	const char* reqid = it->value.GetString();

	if(((it = d.FindMember("mid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
            XINFO("field no exit or type error");
#endif

			return;
	}
	int mid = it->value.GetInt();

	if(((it = d.FindMember("timestamp")) == d.MemberEnd()) || !it->value.IsInt64()) {
#if defined(DEBUG)
            XINFO("field no exit or type error");
#endif

			return;
	}
	uint64_t timestamp = it->value.GetInt64();

	SXConfig* sxconfig = dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig());
	int modelId = atoi(sxconfig->SolutionID().c_str());
	std::string modelName = std::string("PM") + sxconfig->SolutionID();

	MonitorAns monitorAns(reqid, 3, modelName.c_str(), getpid(), modelId, GetMicroSecond(), timestamp);
	const char* monitorAck = monitorAns.ToJson();

#if defined(DEBUG)
	XINFO("publish ACK| SG->MT", monitorAck);
#endif
	TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_MONITOR_NAME, 
														std::to_string(mid).c_str(),
														monitorAck,
														strlen(monitorAck)+1,
														ETag::EMONTORANS);
}

void SubscribeEchoReq::Done(const char* event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("msg:%s, len:%d\n", event, eventLen);
#endif
	/* Parse Json */
	rapidjson::Document d;
	d.Parse(event, eventLen);
	if (d.HasParseError() || !d.IsObject()) {
		XERROR("parser json error\n");

		return;
	}

	rapidjson::Value::ConstMemberIterator it;
	if(((it = d.FindMember("reqid")) == d.MemberEnd()) || !it->value.IsString()) {
#if defined(DEBUG)
            XINFO("field no exit or type error");
#endif

			return;
	}
	const char* reqid = it->value.GetString();

	if(((it = d.FindMember("mid")) == d.MemberEnd()) || !it->value.IsInt()) {
#if defined(DEBUG)
            XINFO("field no exit or type error");
#endif

			return;
	}
	int mid = it->value.GetInt();

	if(((it = d.FindMember("timestamp")) == d.MemberEnd()) || !it->value.IsInt64()) {
#if defined(DEBUG)
            XINFO("field no exit or type error");
#endif

			return;
	}
	uint64_t timestamp = it->value.GetInt64();

	SXConfig* sxconfig = dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig());
	int modelId = atoi(sxconfig->SolutionID().c_str());
	std::string modelName = std::string("PM") + sxconfig->SolutionID();

	MonitorAns monitorAns(reqid, 3, modelName.c_str(), getpid(), modelId, GetMicroSecond(), timestamp);
	const char* monitorAck = monitorAns.ToJson();

#if defined(DEBUG)
	XINFO("publish ACK| SG->MT", monitorAck);
#endif
	TNodeAministrator::GetInstance()->PublishGroup(SOLUTIONGATEWAY_PUBLISH_MONITOR_NAME, 
														std::to_string(mid).c_str(),
														monitorAck,
														strlen(monitorAck)+1,
														ETag::EMONTORANS);
}
/*SubscribeMonitor end ****/

/*SubscribeModify begin ****/
SubscribeModify::SubscribeModify(Consumer* consumer, const char* eventName, const char* groupName) :
	Subscribe(consumer, eventName, groupName) {
}

void SubscribeModify::Done(const char* event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("msg:%s, len:%d\n", event, eventLen);
#endif
}

void SubscribeModify::Done(char* event, unsigned int eventLen) {
#if defined(DEBUG)
	XINFO("msg:%s, len:%d\n", event, eventLen);
#endif
}
/*SubscribeModify end ****/
