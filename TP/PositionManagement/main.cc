/*************************************************************************
	> File Name: main.cc
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Wed 22 Feb 2017 03:41:26 PM CST
 ************************************************************************/
#include<unistd.h>
#include<signal.h>
#include"tnode_adapter.h"
#include"ThreadPool.h"
#include"position_list.h"
#include"mysqlop.h"
#include"object_pool.h"
#include"redisop.h"
#include"solution_config.h"
#include"concurrentqueue.h"
#include"xdatetime.h"
#include"position_regex_json.h"
#include"position_subscribe.h"

using namespace snetwork_xservice_xflagger;

TNodeAministrator* g_tnode = nullptr; 
extern SafeList<PositionListNode> g_position_list; 
ThreadPool* g_threadPool = nullptr;
using RedisPool = ObjectPool<snetwork_xservice_db::Redis, 5>;
RedisPool* g_redisPool = nullptr;
/* sendorder pool */
using RegExSendOrderNode = RegExSendOrder<REQID_CSTR, GROUPID_INT, CLIENTID_INT, ASSETID_INT, DIRECTION_CHAR, EXPIRYPERIOD_INT, AMOUNT_INT, SESSIONID_CSTR,SGID_INT>;
using SendOrderPool = ObjectPool<RegExSendOrderNode, 10>;
SendOrderPool* g_sendOrderPool = nullptr;
/* orderans pool */
using RegExOrderAnsNode = RegExOrderAns<SESSIONID_CSTR, REQID_CSTR, GROUPID_INT, CLIENTID_INT, ORDERID_CSTR, ORDERSTATUS_INT,ASSETID_INT, DIRECTION_CHAR, AMOUNT_INT, EXPIRYPERIOD_INT,OPENTIME_LONG,OPENPRICE_INT,CLOSEPRICE_INT,PAYOUT_LONG, SGID_INT, TEID_INT>;
using OrderAnsPool = ObjectPool<RegExOrderAnsNode, 10>;
OrderAnsPool* g_orderAnsPool = nullptr;

using CMallocSizeNode = CMallocSize<char, CMALLOC_SIZE_TNODE>;
using CMallocSizePool = ObjectPool<CMallocSizeNode, 20>;
CMallocSizePool* g_cmallocSizePool = nullptr;

/* concurrentqueue */
using namespace moodycamel;
ConcurrentQueue<std::string> g_concurrent_send_order_list; 
ConcurrentQueue<std::string> g_concurrent_validate_order_list; 
ConcurrentQueue<std::string> g_concurrent_order_ans_list; 
std::string* g_send_order_list_bulk = nullptr;
std::string* g_validate_order_list_bulk = nullptr;
std::string* g_order_ans_list_bulk = nullptr;

std::string g_pmid;

void WriteToMySql(void);
void Init(void);
size_t InsertSendOrder(ssize_t size);
size_t InsertValidate(ssize_t size);
size_t InsertOrderAns(ssize_t size);
void RunList(void);
snetwork_xservice_db::ConnectionPool* g_connectionPool = nullptr;
int main(int argc, char* argv[]) {
	//InitDaemon();
	Init();
	signal(SIGPIPE, SIG_IGN);

	XConfig* cfg = new SXConfig;
	XFlagger* flagger = SXFlagger::GetInstance();
	flagger->ParseCommandLine(&argc, &argv, false);
	flagger->FlagXConfig(cfg);
	SXConfig* sxconfig = dynamic_cast<SXConfig*>(flagger->GetConfig());
	if (sxconfig == nullptr) {
		fprintf(stderr, "config error");

		exit(1);
	}
	std::cout<<*sxconfig<<std::endl;
	/* logger setting */

	std::string fileName (sxconfig->LoggerFile());
	if (!sxconfig->Screen()) {
		fileName = sxconfig->LoggerFile();
	}
	(*XLogger::GetObject(fileName.c_str())).StdErr(sxconfig->Screen()).Colour(sxconfig->Color()).Dir(sxconfig->LoggerDir());

	g_redisPool = RedisPool::GetInstance();
	g_cmallocSizePool = CMallocSizePool::GetInstance();
	g_sendOrderPool = SendOrderPool::GetInstance();
	g_orderAnsPool = OrderAnsPool::GetInstance();
	//g_connectionPool = snetwork_xservice_db::ConnectionPool::GetInstance();
	g_connectionPool = snetwork_xservice_db::ConnectionPool::GetInstance(sxconfig->MySqlHost().c_str(),
																		 sxconfig->MySqlUser().c_str(),
																		 sxconfig->MySqlPassword().c_str(),
																		 sxconfig->MySqlPort(),
																		 sxconfig->ConnSize());
	g_tnode = TNodeAministrator::GetInstance();
	g_threadPool = new ThreadPool(sxconfig->ThreadSize());
	g_pmid = sxconfig->SolutionID();
	//usleep(2000);

	/* sg->pm: SENDORDER.POSITION */
	Consumer* sendOrder = new Consumer(POSITIONMANAGEMENT_SUBSCRIBE_POSITION_EVENT, ETag::ESENDORDER);
	sendOrder->CreateSubscibe();

	/* te->pm: ORDERANS.POSITION */
	Consumer* orderAns = new Consumer(POSITIONMANAGEMENT_SUBSCRIBE_POSITION_EVENT, ETag::EORDERANS);
	orderAns->CreateSubscibe();

	/* sg/te->pm : requestACK*/
	//Consumer* system = new Consumer(POSITIONMANAGEMENT_SUBSCRIBE_POSITION_EVENT, ETag::EREQUESTACK);
	Consumer* system = new Consumer(sxconfig->SolutionID(), ETag::EREQUESTACK);
	system->CreateSubscibe();

	Consumer* pull = new Consumer(POSITIONMANAGEMENT_SUBSCRIBE_POSITION_EVENT, ETag::EPULL);
	pull->CreateSubscibe();

	Consumer* monitor = new Consumer(sxconfig->SolutionID(), ETag::EMONITOR);
	monitor->CreateSubscibe();

	//std::thread(RunList);
#if 0
	std::thread t1(WriteToMySql);
	while (1) {
		//usleep(1);
		pause();
	}
#else
	WriteToMySql();
#endif
	
	delete sendOrder;
	delete orderAns;
	delete system;
	delete pull;
	delete monitor;
	//t1.join();

	FREEXLOGGER;

	return 0;
	//pthread_exit(0);
}

void RunList(void) {
	std::chrono::system_clock::time_point nw;
	std::chrono::system_clock::time_point end;
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds;
	int sleepTime = 0;

	while (1) {
		g_position_list.Remove([&](PositionListNode& r) {
			if ((1 == r.m_sg_system) && 
				(1 == r.m_te_system) &&
				(1 == r.m_sg_sendorder)) {
				if (1 == r.m_pm_orderans) {
					return true;
				} else {
					if ((1 == r.m_te_orderans_openprice) &&
						(1 == r.m_te_orderans_closeprice)) {
						return true;
					} else {
						/* time out need to remove */
						nw = std::chrono::system_clock::now();
						if (r.m_stamp > r.m_openTime) {
							elapsed_seconds = nw - r.m_stamp;
						} else {
							elapsed_seconds = nw - r.m_openTime;
						}

						if (2 * r.m_expiryPeriod < elapsed_seconds.count()) {
							/* time out, remove list, and async write to database */
							XINFO("sendorder=%s timeout", r.m_reqID);
#if 0
							/* need to complete */
							SendOrderListNode tmp = std::move(r);
#endif
							return true;
						}
					}
				}

			}
			return false;
		});

		end = std::chrono::system_clock::now();
		elapsed_seconds = end - start;
		sleepTime = elapsed_seconds.count() - SAFE_LIST_ELAPSED_SECONDS;
		start = std::move(end);

		if (sleepTime > 0) {
			sleep(elapsed_seconds.count());
		}
	}
}


void Init(void) {
	g_send_order_list_bulk = new std::string[PM_BULK_SIZE];
	g_validate_order_list_bulk = new std::string[PM_BULK_SIZE];
	g_order_ans_list_bulk = new std::string[PM_BULK_SIZE];

	/*sendorder regex init */
	//RegExSendOrder<REQID_CSTR, GROUPID_INT, CLIENTID_INT, ASSETID_INT, DIRECTION_CHAR, EXPIRYPERIOD_INT, AMOUNT_INT, SESSIONID_CSTR,SGID_INT>::StaticInit();
	/* orderans regex init */
	//RegExOrderAns<SESSIONID_CSTR, REQID_CSTR, GROUPID_INT, CLIENTID_INT, ORDERID_CSTR, ORDERSTATUS_INT,ASSETID_INT, DIRECTION_CHAR, AMOUNT_INT, EXPIRYPERIOD_INT,OPENTIME_LONG,OPENPRICE_INT,CLOSEPRICE_INT,PAYOUT_LONG, SGID_INT, TEID_INT>::StaticInit();
	SubscribeSendOrder::StaticInit();
	SubscribeOrderAns::StaticInit();
}

void WriteToMySql(void) {
	int sleepTime = 0;
	size_t sendOrderCnt = 0;
	size_t sendOrderIndex = 0;
	size_t validateCnt = 0;
	size_t validateIndex = 0;
	size_t orderAnsCnt = 0;
	size_t orderAnsIndex = 0;
	size_t second_time = 0;
	long ret = 0;
	CCalcTime calc;
	sleep(5);

	calc.Begin();

	while (1) {
		do {
			sendOrderCnt = g_concurrent_send_order_list.try_dequeue_bulk(&g_send_order_list_bulk[sendOrderIndex], PM_BULK_SIZE - sendOrderIndex);
			if (sendOrderCnt == PM_BULK_SIZE - sendOrderIndex ||
				(second_time >= PM_BULK_TIME &&
				 (sendOrderIndex + sendOrderCnt) != 0)) {
				/* insert sendorder to database */
				InsertSendOrder(sendOrderIndex + sendOrderCnt);
				sendOrderIndex = 0;

				break;
			} else if (sendOrderCnt != 0) {
				sendOrderIndex += sendOrderCnt;

				break;
			} else {
				break;
			}
		}while (false);

		do {
			validateCnt = g_concurrent_validate_order_list.try_dequeue_bulk(&g_validate_order_list_bulk[validateIndex], PM_BULK_SIZE - validateIndex);
			if (validateCnt == PM_BULK_SIZE - validateIndex ||
				(second_time >= PM_BULK_TIME &&
				 (validateIndex + validateCnt) != 0)) {
				/* insert sendorder to database */
				InsertValidate(validateIndex + validateCnt);
				validateIndex = 0;

				break;
			} else if (validateCnt != 0) {
				validateIndex += validateCnt;

				break;
			} else {
				break;
			}
		}while (false);
		
		do {
			orderAnsCnt = g_concurrent_order_ans_list.try_dequeue_bulk(&g_order_ans_list_bulk[orderAnsIndex], PM_BULK_SIZE - orderAnsIndex);
			if (orderAnsCnt == PM_BULK_SIZE - orderAnsIndex ||
				(second_time >= PM_BULK_TIME &&
				 (orderAnsIndex + orderAnsCnt) != 0)) {
				/* insert sendorder to database */
				InsertOrderAns(orderAnsIndex + orderAnsCnt);
				orderAnsIndex = 0;

				break;
			} else if (orderAnsCnt != 0) {
				orderAnsIndex += orderAnsCnt;

				break;
			} else {
				break;
			}
		}while (false);

		ret = calc.End();
		sleepTime = PM_ELAPSED_SECONDS - ret/1000000;
		if (sleepTime > 0) {
			sleep(sleepTime);
			if (second_time >= PM_BULK_TIME) {
				second_time = 0;
			} else {
				second_time += PM_ELAPSED_SECONDS;
			}
		} else {
			if (second_time >= PM_BULK_TIME) {
				second_time = 0;
			} else {
				second_time -= sleepTime;
			}
		}
		ret = calc.End();
	}
}

size_t InsertSendOrder(ssize_t size) {
	CDateTime day;
	char* tmpDay = day.AddDay(0);
	std::string sql("INSERT INTO pm_send_order_log_");
	sql.append(tmpDay).append("(reqid,sessionid,groupid,clientid,assetid,direction,expiryperiod,amount,sgid,transfer,update_time)VALUES");
	free(tmpDay);
	tmpDay = nullptr;
	for (ssize_t i=0; i<size; ++i) {
		sql += g_send_order_list_bulk[i];
	}

	sql[sql.length()-1] = ';';
	snetwork_xservice_db::MySqlDB db;
	db.Open();
	db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

	db.StartTransaction();
	const char* sqlbuf = sql.c_str();
	ssize_t rows = db.ExecSql(sqlbuf);
	if (rows != size) {
		db.RollBack();
		XERROR("%s,con=%d|sql=", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
	} else {
		db.Commint();
	}
	db.Close();

	return rows;
}

size_t InsertValidate(ssize_t size) {
	CDateTime day;
	char* tmpDay = day.AddDay(0);
	std::string sql("INSERT INTO pm_validate_order_log_");
	sql.append(tmpDay).append("(reqid,sessionid,groupid,clientid,accountid,boaccountid,assetid,direction,expiryperiod,amount,orderstatus,margin,freemargin,profit,sgid,teid,update_time)VALUES");
	free(tmpDay);
	tmpDay = nullptr;

	for (ssize_t i=0; i<size; ++i) {
		sql += g_validate_order_list_bulk[i];
	}

	sql[sql.length()-1] = ';';
	snetwork_xservice_db::MySqlDB db;
	db.Open();
	db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

	db.StartTransaction();
	const char* sqlbuf = sql.c_str();
	ssize_t rows = db.ExecSql(sqlbuf);
	if (rows != size) {
		db.RollBack();
		XERROR("%s,con=%d|sql=", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
	} else {
		db.Commint();
	}
	db.Close();

	return rows;
}

size_t InsertOrderAns(ssize_t size) {
	CDateTime day;
	char* tmpDay = day.AddDay(0);
	std::string sql("INSERT INTO pm_order_ans_log_");
	sql.append(tmpDay).append("(reqid,sessionid,groupid,clientid,orderid,orderstatus,assetid,direction,amount,payout,opentime,openprice,closeprice,passive,sgid,teid,update_time)VALUES");
	free(tmpDay);
	tmpDay = nullptr;

	for (ssize_t i=0; i<size; ++i) {
		sql += g_order_ans_list_bulk[i];
	}

	sql[sql.length()-1] = ';';
	snetwork_xservice_db::MySqlDB db;
	db.Open();
	db.SelectDB(dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig())->MySqlDBName().c_str());

	db.StartTransaction();
	const char* sqlbuf = sql.c_str();
	ssize_t rows = db.ExecSql(sqlbuf);
	if (rows != size) {
		db.RollBack();
		XERROR("%s,con=%d|sql=", db.GetMySqlErrMsg(), db.IsConnect(), sqlbuf);
	} else {
		db.Commint();
	}
	db.Close();

	return rows;
}

