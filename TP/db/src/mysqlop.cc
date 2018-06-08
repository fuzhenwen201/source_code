/*************************************************************************
	> File Name: mysqlop.cc
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Sun 15 Jan 2017 08:17:35 PM PST
 ************************************************************************/
#include<unistd.h>
#include<cstdlib>
#include<cstring>
#include"glog/logging.h"
#include"mysqlop.h"

namespace snetwork_xservice_db {
ConnectionPool* ConnectionPool::m_pInstance = NULL;
std::mutex ConnectionPool::m_mutex;

/***  MySql begin *******/
MySql::MySql(const char* host, const char* user, const char* pwd, unsigned short port) 
	: m_host(host)
	, m_user(user)
	, m_pwd(pwd)
	, m_port(port)
	, m_index(0)
	, m_pMySql(nullptr) 
	, m_activeTime(0)
	, m_waitTimeOut(0)
	, m_reConnectCnt(0) {
}

MySql::~MySql() {
	m_host.clear();
	m_user.clear();
	m_pwd.clear();

	if (m_pMySql != nullptr) {
		mysql_close(m_pMySql);
		m_pMySql = nullptr;
	}
}
/***  MySql end *******/

/**** connectionPool  begin ********/
ConnectionPool::ConnectionPool(unsigned char max)
	: m_maxConnection(max)
	, m_currConnection(0)
	, m_waitTimeOut(0) {
	m_list.clear();
}

ConnectionPool::~ConnectionPool() {
	for (auto& it : m_list) {
		delete it;
		it = nullptr;
	}
}

void ConnectionPool::FirstInit(const char* host, const char* user, const char* pwd, short port) {
	do {
		MYSQL* pMySql = (MYSQL*)NULL;
		pMySql = mysql_init((MYSQL*)NULL);
		if ((MYSQL*)NULL != pMySql) {

			mysql_options(pMySql, MYSQL_SET_CHARSET_NAME, "utf8");
			if ((MYSQL*)NULL != mysql_real_connect(pMySql, host, user, pwd, NULL, port, NULL, 0)) {
				bool bReconnect = false;
				mysql_options(pMySql, MYSQL_OPT_RECONNECT, &bReconnect);
				// get default wait_time
				int ret = mysql_query(pMySql, "SHOW VARIABLES LIKE 'wait_timeout'");
				if (0 != ret) {
					/* query fail*/
					LOG(ERROR) << "Error:" << mysql_error(pMySql);
					mysql_close(pMySql);

					continue;
				}

				MYSQL_RES* rs = mysql_store_result(pMySql);
				if (nullptr == rs) {
					LOG(ERROR) << "Error:" << mysql_error(pMySql);
					mysql_close(pMySql);

					continue;
				}

				MYSQL_ROW row;
				while ((row = mysql_fetch_row(rs))) {
					m_waitTimeOut = atoll(row[1]) / 2;	
				}

				mysql_free_result(rs);
				MySql* mysql = new MySql(host, user, pwd, port);
				mysql->SetWaitTimeOut(m_waitTimeOut);
				mysql->SetIndex(m_currConnection);
				mysql->SetMySQL(pMySql);
				mysql->SetActiveTime(time(NULL));
				m_list.push_front(mysql);
				++m_currConnection;

				break;
			} else {
				// should be write log
				LOG(ERROR) << "Error:" << mysql_error(pMySql);
				mysql_close(pMySql);
			}
		}
		usleep(5000);
	}while(true);
}

void ConnectionPool::SecondInit(const char* host, const char* user, const char* pwd, short port) {
	while (m_currConnection != m_maxConnection) {
		MYSQL* pMySql = (MYSQL*)NULL;
		pMySql = mysql_init((MYSQL*)NULL);
		if ((MYSQL*)NULL != pMySql) {

			mysql_options(pMySql, MYSQL_SET_CHARSET_NAME, "utf8");
			if ((MYSQL*)NULL != mysql_real_connect(pMySql, host, user, pwd, NULL, port, NULL, 0)) {
				bool bReconnect = false;
				mysql_options(pMySql, MYSQL_OPT_RECONNECT, &bReconnect);
				MySql* mysql = new MySql(host, user, pwd, port);
				mysql->SetWaitTimeOut(m_waitTimeOut);
				mysql->SetIndex(m_currConnection);
				mysql->SetMySQL(pMySql);
				mysql->SetActiveTime(time(NULL));
				m_list.push_front(mysql);
				++m_currConnection;
			} else {
				usleep(2000);
				LOG(ERROR) << "Error:" << mysql_error(pMySql);
				mysql_close(pMySql);
			}
		} else {
			usleep(2000);
			LOG(ERROR) << "Error:" << mysql_error(pMySql);
			mysql_close(pMySql);
		}
	}
}

void ConnectionPool::Init(const char* host, const char* user, const char* pwd, short port) {
	FirstInit(host, user, pwd, port);
	SecondInit(host, user, pwd, port);
}

ConnectionPool* ConnectionPool::GetInstance(void) {
	if (m_pInstance == nullptr) {
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_pInstance == nullptr) {
			m_pInstance = new ConnectionPool();
		}
	}

	return m_pInstance;
}

MySql* ConnectionPool::GetConnect(bool& isConnect) {
	isConnect = false;
	std::lock_guard<std::mutex> lock(m_mutex);
	if (m_list.empty()) {
		LOG(WARNING) << "Connection list is empty.";
		isConnect = false;

		return nullptr;
	}

	MySql* mysql = m_list.front();
	m_list.pop_front();
	--m_currConnection;

	/* first: mysql_ping */
	if (difftime(time(NULL), mysql->GetActiveTime()) >= mysql->GetWaitTimeOut()) {
		if (0 == mysql_ping(mysql->GetMySql())) {
			/* connection still alive */
			mysql->SetActiveTime(time(NULL));
			isConnect = true;

			return mysql;
		} else {
			isConnect = false;
			LOG(ERROR) << mysql_error(mysql->GetMySql());
			/* need to reconnection */
			ReConnect(mysql, isConnect);
		}
	} else {
		isConnect = true;
	}

	return mysql;
}

void ConnectionPool::ReConnect(MySql* mysql, bool& isConnect) {
	mysql_close(mysql->GetMySql());
	size_t cnt = 1;

	do {
		MYSQL* pMySql = (MYSQL*)NULL;
		pMySql = mysql_init((MYSQL*)NULL);
		if ((MYSQL*)NULL != pMySql) {

			mysql_options(pMySql, MYSQL_SET_CHARSET_NAME, "utf8");
			if ((MYSQL*)NULL != mysql_real_connect(pMySql, mysql->GetHost().c_str(), mysql->GetUser().c_str(), mysql->GetPwd().c_str(), NULL, mysql->GetPort(), NULL, 0)) {
				bool bReconnect = false;
				mysql_options(pMySql, MYSQL_OPT_RECONNECT, &bReconnect);
				// get default wait_time
				int ret = mysql_query(pMySql, "SHOW VARIABLES LIKE 'wait_timeout'");
				if (0 != ret) {
					/* query fail*/
					LOG(ERROR) << "Error:" << mysql_error(pMySql);
					mysql_close(pMySql);
					usleep(2000);

					continue;
				}

				MYSQL_RES* rs = mysql_store_result(pMySql);
				if (nullptr == rs) {
					LOG(ERROR) << "Error:" << mysql_error(pMySql);
					mysql_close(pMySql);
					usleep(2000);

					continue;
				}

				MYSQL_ROW row;
				while ((row = mysql_fetch_row(rs))) {
					m_waitTimeOut = atoll(row[0]) / 2;	
				}

				mysql_free_result(rs);
				mysql->SetWaitTimeOut(m_waitTimeOut);
				mysql->SetMySQL(pMySql);
				mysql->SetActiveTime(time(NULL));
				isConnect =true;

				return;
			} else {
				// should be write log
				LOG(ERROR) << "Error:" << mysql_error(pMySql);
				mysql_close(pMySql);
			}
		}

		sleep(cnt);
		cnt = (cnt << 1);
	}while(cnt < MAX_RECONNECTION);

	LOG(ERROR) << "fail: Out of Max Reconnect.";
	isConnect = false;

	return;
}

void ConnectionPool::ReleaseConnect(MySql* mysql) {
	std::lock_guard<std::mutex> lock(m_mutex);
	auto it = m_list.before_begin();
	for (auto& tmp : m_list) {
		++it;
	}

	++m_currConnection;
	m_list.insert_after(it, mysql);
}
/**** connectionPool  end ********/

/**** Field begin *******************/
Field::Field(const char* name, size_t index)
	: m_name(name==nullptr?"":name)
	, m_index(index) {
}

Field::Field(const std::string& name, size_t index)
	: m_name(name)
	, m_index(index) {
}

Field::Field(std::string&& name, size_t index)
	: m_name(std::move(name))
	, m_index(index) {
}

Field::~Field() {
	m_name.clear();
}

Field::Field(const Field& r) {
	m_name = r.m_name;
	m_index = r.m_index;
}

Field::Field(Field&& r) {
	m_name = std::move(r.m_name);
	m_index = r.m_index;
	r.m_index = 0;
}

Field& Field::operator=(const Field& r) { 
	if (this == &r) {
		return *this;
	}

	m_name = r.m_name;
	m_index = r.m_index;

	return *this;
}

Field& Field::operator=(Field&& r) { 
	if (this == &r) {
		return *this;
	}

	m_name = std::move(r.m_name);
	m_index = r.m_index;
	r.m_index = 0;

	return *this;
}
/**** Field end *******************/
/**** MySqlRecordSet begin *******************/
MySqlRecordSet::MySqlRecordSet(size_t rows, size_t cols)
	: m_rows(rows)
	, m_cols(cols) {
	m_fields.reserve(cols);
	m_recordSets.reserve(rows);
	m_nullString.clear();
}

MySqlRecordSet::~MySqlRecordSet() {
	m_rows = 0;
	m_cols = 0;
	std::vector<Field>().swap(m_fields);
	std::vector<std::vector<std::string>>().swap(m_recordSets);
}

MySqlRecordSet::MySqlRecordSet(const MySqlRecordSet& r) {
	m_rows = r.m_rows;
	m_cols = r.m_cols;
	m_fields = r.m_fields;
	m_recordSets = r.m_recordSets;
	m_nullString = r.m_nullString;
}

MySqlRecordSet& MySqlRecordSet::operator=(const MySqlRecordSet& r) {
	if (this == &r) {
		return *this;
	}

	m_rows = r.m_rows;
	m_cols = r.m_cols;
	m_fields = r.m_fields;
	m_recordSets = r.m_recordSets;
	m_nullString = r.m_nullString;

	return *this;
}

MySqlRecordSet::MySqlRecordSet(MySqlRecordSet&& r) {
	m_rows = r.m_rows;
	m_cols = r.m_cols;
	m_fields = std::move(r.m_fields);
	m_recordSets = std::move(r.m_recordSets);
	m_nullString = std::move(r.m_nullString);

	r.m_rows = 0;
	r.m_cols = 0;
}

MySqlRecordSet& MySqlRecordSet::operator=(MySqlRecordSet&& r) {
	if (this == &r) {
		return *this;
	}

	m_rows = r.m_rows;
	m_cols = r.m_cols;
	m_fields = std::move(r.m_fields);
	m_recordSets = std::move(r.m_recordSets);
	m_nullString = std::move(r.m_nullString);

	return *this;
}

ssize_t MySqlRecordSet::GetFieldIndex(const char* name) const {
	ssize_t index = -1;
	for (auto& field : m_fields) {
		if (field.m_name == name) {
			index = field.m_index;
		}
	}

	return index;
}

ssize_t MySqlRecordSet::GetFieldIndex(const std::string& name) const {
	ssize_t index = -1;
	for (auto& field : m_fields) {
		if (field.m_name == name) {
			index = field.m_index;
		}
	}

	return index;
}

const std::string& MySqlRecordSet::GetFieldByName(size_t row, const char* colName) const {
	if (row >= m_rows) {
		return m_nullString;
	}

	ssize_t col = GetFieldIndex(colName);
	if (-1 == col) {
		return m_nullString;
	}

	return m_recordSets[row][col];
}

const std::string& MySqlRecordSet::GetFieldByName(size_t row, const std::string& colName) const {
	if (row >= m_rows) {
		return m_nullString;
	}

	ssize_t col = GetFieldIndex(colName);
	if (-1 == col) {
		return m_nullString;
	}

	return m_recordSets[row][col];
}

const std::string& MySqlRecordSet::GetFieldByID(size_t row, size_t col) const {
	if (row >= m_rows || col >= m_cols) {
		return m_nullString;
	}

	return m_recordSets[row][col];
}
/**** MySqlRecordSet end*******************/

/**** MySqlDB begin *******************/
MySqlDB::MySqlDB() 
	: m_bConnect(false)
	, m_bError(false)
	, m_connectID(0)
	, m_pMySql(nullptr) {
}

MySqlDB::~MySqlDB() {
	if (nullptr != m_pMySql) {
		ConnectionPool::GetInstance()->ReleaseConnect(m_pMySql);
		m_pMySql = nullptr;
	}
}

const MySql* MySqlDB::Open(int connectID) {
	size_t cnt = 0;
	do {
		m_pMySql = ConnectionPool::GetInstance()->GetConnect(m_bConnect);
		if (nullptr != m_pMySql) {
			break;
		}
		++cnt;
	}while(cnt < 20);

	if (nullptr == m_pMySql) {
		LOG(ERROR) << "Over max range of reconnect count.";
	}

	return m_pMySql;
}

int MySqlDB::Close(void) {
	if (nullptr == m_pMySql) {
		m_bError = true;
		LOG(ERROR) << "MySql connect is nullptr.";

		return -1;
	}

	ConnectionPool::GetInstance()->ReleaseConnect(m_pMySql);
	m_pMySql = nullptr;

	return 0;
}

int MySqlDB::SelectDB(const char* db_name) {
	if (db_name == nullptr || strlen(db_name) == 0) {
		m_bError = true;
		LOG(ERROR) << "DB name is nullptr or empty.";

		return -1;
	}

	if (nullptr == m_pMySql) {
		m_bError = true;
		LOG(ERROR) << "MySql connect is nullptr.";

		return -1;
	}

	mysql_select_db(m_pMySql->GetMySql(), db_name);

	return 0;
}

ssize_t MySqlDB::ExecSql(const char* sql) {
	m_bError = false;
	if (nullptr == sql || 0 == strlen(sql)) {
		LOG(ERROR) << "execute sql is nullptr or empty.";
		m_bError = true;

		return -1;
	}

	if (nullptr == m_pMySql) {
		m_bError = true;
		LOG(ERROR) << "MySql connect is nullptr.";

		return -1;
	}

	MYSQL* mysql = m_pMySql->GetMySql();
	if (0 != mysql_query(mysql, sql)) {
		LOG(ERROR) << mysql_error(mysql) << "|sql=" << sql;
		m_bError = true;

		return -1;
	}

	return mysql_affected_rows(mysql);
}

MySqlRecordSet MySqlDB::QuerySql(const char* sql) {
	m_bError = false;
	MySqlRecordSet mysqlRecordSet; 

	if (nullptr == sql || 0 == strlen(sql)) {
		LOG(ERROR) << "query sql is nullptr or empty.";
		m_bError = true;

		return std::move(mysqlRecordSet);
	}

	if (nullptr == m_pMySql) {
		m_bError = true;
		LOG(ERROR) << "MySql connect is nullptr\n";

		return std::move(mysqlRecordSet);
	}

	MYSQL* mysql = m_pMySql->GetMySql();
	if (0 != mysql_query(mysql, sql)) {
		LOG(ERROR) << mysql_error(mysql) << "|sql=" << sql;
		m_bError = true;

		return std::move(mysqlRecordSet);
	}

	MYSQL_RES* result = mysql_store_result(mysql);
	size_t rows = mysql_num_rows(result);
	size_t cols = mysql_num_fields(result);
	mysqlRecordSet.SetSize(rows, cols);
	size_t index = 0;
	MYSQL_FIELD* mysql_field = nullptr;
	while ((mysql_field = mysql_fetch_field(result))) {
		Field field(mysql_field->name, index++);
		mysqlRecordSet.GetFields().push_back(std::move(field));
	}

	MYSQL_ROW mysql_row;
	while ((mysql_row = mysql_fetch_row(result))) {
		std::vector<std::string> vec;
		vec.reserve(cols);
		for (size_t col = 0; col < cols; ++col) {
			vec.push_back(mysql_row[col]);
		}
		mysqlRecordSet.GetRecordSets().push_back(std::move(vec));
	}

	mysql_free_result(result);

	return std::move(mysqlRecordSet);
}

ssize_t MySqlDB::GetInsertID(void) {
	m_bError = false;
	if (nullptr == m_pMySql) {
		m_bError = true;
		LOG(ERROR) << "MySql connect is nullptr.";

		return -1;
	}

	return mysql_insert_id(m_pMySql->GetMySql());
}

void MySqlDB::StartTransaction(void) {
	m_bError = false;
	if (nullptr == m_pMySql) {
		m_bError = true;
		LOG(ERROR) << "MySql connect is nullptr.";

		return;
	}

	MYSQL* mysql = m_pMySql->GetMySql();
	if (0 != mysql_query(mysql, "START TRANSACTION")) {
		LOG(ERROR) << mysql_error(mysql);
		m_bError = true;

		return;
	}
}

void MySqlDB::Commint(void) {
	m_bError = false;
	if (nullptr == m_pMySql) {
		m_bError = true;
		LOG(ERROR) << "MySql connect is nullptr.";

		return;
	}

	MYSQL* mysql = m_pMySql->GetMySql();
	if (0 != mysql_query(mysql, "COMMIT")) {
		LOG(ERROR) << mysql_error(mysql);
		m_bError = true;

		return;
	}
}

void MySqlDB::RollBack(void) {
	m_bError = false;
	if (nullptr == m_pMySql) {
		m_bError = true;
		LOG(ERROR) << "MySql connect is nullptr\n";

		return;
	}

	MYSQL* mysql = m_pMySql->GetMySql();
	if (0 != mysql_query(mysql, "ROLLBACK")) {
		LOG(ERROR) << mysql_error(mysql);
		m_bError = true;

		return;
	}
}

const char* MySqlDB::GetMySqlErrMsg(void) {
	if (nullptr != m_pMySql) {
		m_bError = true;
		return mysql_error(m_pMySql->GetMySql());
	}

	return "";
}

/**** MySqlDB end *******************/
}
