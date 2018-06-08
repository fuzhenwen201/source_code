/*************************************************************************
	> File Name: mysqlop.cc
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Sun 15 Jan 2017 08:17:35 PM PST
 ************************************************************************/
#include<unistd.h>
#include<cstdlib>
#include<cstring>
#include"mysqlop.h"
#include<error.h>

static char* STRERROR(int n) {
	size_t size = 1024;
	char* str = (char*)malloc(size);
	if (str == nullptr) {
		return nullptr;
	}

	while (strerror_r(n, str, size) != nullptr &&
		   errno == ERANGE) {
		size = (1<<size);
		str = (char*)realloc(str, size);
	}
	
	return str;
}

ConnectionPool* ConnectionPool::m_pInstance = NULL;
std::mutex ConnectionPool::m_mutex;

/***  MySql begin *******/
MySql::MySql(const char* host, const char* user, const char* pwd, int port) 
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
#if 0
/* sharedMemoryNode begin */
SharedMemoryNode::SharedMemoryNode() {
	m_index = 0;
	m_active = false;
}
/* sharedMemoryNode end */
#endif	

/**** connectionPool  begin ********/
ConnectionPool::ConnectionPool(const char* host, const char* user, const char* pwd, int port, unsigned char max) :
	m_maxConnection(max),
	m_currConnection(0),
	m_waitTimeOut(0),
	m_host(nullptr),
	m_user(nullptr),
	m_pwd(nullptr),
	m_port(3306) {
	m_list.clear();
	asprintf(&m_host, "%s", host);
	asprintf(&m_user, "%s", user);
	asprintf(&m_pwd, "%s", pwd);
	m_port = port;
	Init(m_host, m_user, m_pwd, port);
}

ConnectionPool::~ConnectionPool() {
	if (m_host != nullptr) {
		free(m_host);
		m_host = nullptr;
	}

	if (m_user != nullptr) {
		free(m_user);
		m_user = nullptr;
	}

	if (m_pwd != nullptr) {
		free(m_pwd);
		m_pwd = nullptr;
	}

	for (auto& it : m_list) {
		delete it;
		it = nullptr;
	}
}

#if 0
void ConnectionPool::FirstInit(const char* host, const char* user, const char* pwd, short port) {
	do {
		MYSQL* pMySql = (MYSQL*)NULL;
		pMySql = mysql_init((MYSQL*)NULL);
		if ((MYSQL*)NULL != pMySql) {
			/* first set charset utf8
			 * mysql_option(mysql, charset, utf8)
			 * must be call before real_connect
			 */
			mysql_options(pMySql, MYSQL_SET_CHARSET_NAME, "utf8");
			if ((MYSQL*)NULL != mysql_real_connect(pMySql, host, user, pwd, NULL, port, NULL, 0)) {
				bool bReconnect = false;
				mysql_options(pMySql, MYSQL_OPT_RECONNECT, &bReconnect);
				// get default wait_time
				int ret = mysql_query(pMySql, "SHOW VARIABLES LIKE 'wait_timeout'");
				if (0 != ret) {
					/* query fail*/
					fprintf(stderr, mysql_error(pMySql));
					mysql_close(pMySql);
					usleep(1000);

					continue;
				}

				MYSQL_RES* rs = mysql_store_result(pMySql);
				if (nullptr == rs) {
					fprintf(stderr, mysql_error(pMySql));
					mysql_close(pMySql);
					usleep(1000);

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
				fprintf(stderr, mysql_error(pMySql));
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
			/* first set charset utf8
			 * mysql_option(mysql, charset, utf8)
			 * must be call before real_connect
			 */
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
				fprintf(stderr, mysql_error(pMySql));
				mysql_close(pMySql);
			}
		} else {
			usleep(2000);
			fprintf(stderr, mysql_error(pMySql));
			mysql_close(pMySql);
		}
	}
}
#else
void ConnectionPool::FirstInit(const char* host, const char* user, const char* pwd, int port) {
	do {
		//MYSQL mySql;
		MYSQL* pMySql = mysql_init(NULL);
		if ((MYSQL*)NULL != pMySql) {
			/* first set charset utf8
			 * mysql_option(mysql, charset, utf8)
			 * must be call before real_connect
			 */
			mysql_options(pMySql, MYSQL_SET_CHARSET_NAME, "utf8");
			//if ((MYSQL*)NULL != mysql_real_connect(pMySql, host, user, pwd, NULL, port, NULL, 0)) {
			if ((MYSQL*)NULL != mysql_real_connect(pMySql, host, user, pwd, NULL, port, NULL, CLIENT_IGNORE_SIGPIPE)) {
				bool bReconnect = false;
				mysql_options(pMySql, MYSQL_OPT_RECONNECT, &bReconnect);
				// get default wait_time
				int ret = mysql_query(pMySql, "SHOW VARIABLES LIKE 'wait_timeout'");
				if (0 != ret) {
					/* query fail*/
					fprintf(stderr, mysql_error(pMySql));
					mysql_close(pMySql);
					usleep(1000);

					continue;
				}

				MYSQL_RES* rs = mysql_store_result(pMySql);
				if (nullptr == rs) {
					fprintf(stderr, mysql_error(pMySql));
					mysql_close(pMySql);
					usleep(1000);

					continue;
				}

				MYSQL_ROW row;
				while ((row = mysql_fetch_row(rs))) {
					m_waitTimeOut = atoll(row[1]) / 2;	
				}

				mysql_free_result(rs);
				MySql* mysql = new MySql(host, user, pwd, port);
				//MySql mysql(host, user, pwd, port);
				mysql->SetWaitTimeOut(m_waitTimeOut);
				mysql->SetIndex(m_currConnection);
				//mysql->SetMySQL(pMySql);
				mysql->SetMySQL(pMySql);
				mysql->SetActiveTime(time(NULL));
				m_list.push_front(mysql);
				++m_currConnection;

				break;
			} else {
				// should be write log
				fprintf(stderr, mysql_error(pMySql));
				mysql_close(pMySql);
			}
		}
		usleep(5000);
	}while(true);
}

void ConnectionPool::SecondInit(const char* host, const char* user, const char* pwd, int port) {
	while (m_currConnection != m_maxConnection) {
		//MYSQL mySql;
		MYSQL* pMySql = mysql_init(NULL);
		if ((MYSQL*)NULL != pMySql) {
			/* first set charset utf8
			 * mysql_option(mysql, charset, utf8)
			 * must be call before real_connect
			 */
			mysql_options(pMySql, MYSQL_SET_CHARSET_NAME, "utf8");
			//if ((MYSQL*)NULL != mysql_real_connect(pMySql, host, user, pwd, NULL, port, NULL, 0)) {
			if ((MYSQL*)NULL != mysql_real_connect(pMySql, host, user, pwd, NULL, port, NULL, CLIENT_IGNORE_SIGPIPE)) {
				bool bReconnect = false;
				mysql_options(pMySql, MYSQL_OPT_RECONNECT, &bReconnect);
				MySql* mysql = new MySql(host, user, pwd, port);
				//MySql mysql(host, user, pwd, port);
				mysql->SetWaitTimeOut(m_waitTimeOut);
				mysql->SetIndex(m_currConnection);
				mysql->SetMySQL(pMySql);
				mysql->SetActiveTime(time(NULL));
				m_list.push_front(mysql);
				++m_currConnection;
			} else {
				usleep(2000);
				fprintf(stderr, mysql_error(pMySql));
				mysql_close(pMySql);
			}
		} else {
			usleep(2000);
			//fprintf(stderr, mysql_error(&mySql));
			//mysql_close(&mySql);
			fprintf(stderr, mysql_error(pMySql));
			mysql_close(pMySql);
		}
	}
}
#endif

void ConnectionPool::Init(const char* host, const char* user, const char* pwd, int port) {
#if 0
	if ((m_mutex_sem = sem_open(SEM_MUTEX_MYSQL_NAME, O_CREAT, 0666, 0)) == SEM_FAILED) {
		char* str = STRERROR_R(errno);
		fprintf(stderr, "ERROR: sem_open(%s)|%s", SEM_MUTEX_MYSQL_NAME, str);
		free(str);
	}
#endif
	FirstInit(host, user, pwd, port);
	SecondInit(host, user, pwd, port);
}

ConnectionPool* ConnectionPool::GetInstance(const char* host, const char* user, const char* pwd, int port, unsigned char max) {
	if (m_pInstance == nullptr) {
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_pInstance == nullptr) {
			//m_pInstance = new ConnectionPool();
			m_pInstance = new ConnectionPool(host, user, pwd, port, max);
		}
	}

	return m_pInstance;
}

MySql* ConnectionPool::GetConnect(bool& isConnect) {
	isConnect = false;
	std::lock_guard<std::mutex> lock(m_mutex);
	if (m_list.empty()) {
		fprintf(stderr, "Connection list is empty");
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
			fprintf(stderr, mysql_error(mysql->GetMySql()));
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
			/* first set charset utf8
			 * mysql_option(mysql, charset, utf8)
			 * must be call before real_connect
			 */
			mysql_options(pMySql, MYSQL_SET_CHARSET_NAME, "utf8");
			//if ((MYSQL*)NULL != mysql_real_connect(pMySql, mysql->GetHost().c_str(), mysql->GetUser().c_str(), mysql->GetPwd().c_str(), NULL, mysql->GetPort(), NULL, 0)) {
			if ((MYSQL*)NULL != mysql_real_connect(pMySql, mysql->GetHost().c_str(), mysql->GetUser().c_str(), mysql->GetPwd().c_str(), NULL, mysql->GetPort(), NULL, CLIENT_IGNORE_SIGPIPE)) {
				bool bReconnect = false;
				mysql_options(pMySql, MYSQL_OPT_RECONNECT, &bReconnect);
				// get default wait_time
				int ret = mysql_query(pMySql, "SHOW VARIABLES LIKE 'wait_timeout'");
				if (0 != ret) {
					/* query fail*/
					fprintf(stderr, mysql_error(pMySql));
					mysql_close(pMySql);
					usleep(2000);

					continue;
				}

				MYSQL_RES* rs = mysql_store_result(pMySql);
				if (nullptr == rs) {
					fprintf(stderr, mysql_error(pMySql));
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
				fprintf(stderr, mysql_error(pMySql));
				mysql_close(pMySql);
			}
		}

		sleep(cnt);
		cnt = (cnt << 1);
	}while(cnt < MAX_RECONNECTION);

	fprintf(stderr, "fail: Out of Max Reconnect\n");
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
		usleep(10000);
		++cnt;
	}while(cnt < 20);

	if (nullptr == m_pMySql) {
		fprintf(stderr, "Over max range of reconnect count\n");
	}

	return m_pMySql;
}

int MySqlDB::Close(void) {
	if (nullptr == m_pMySql) {
		m_bError = true;
		fprintf(stderr, "MySql connect is nullptr\n");

		return -1;
	}

	ConnectionPool::GetInstance()->ReleaseConnect(m_pMySql);
	m_pMySql = nullptr;

	return 0;
}

int MySqlDB::SelectDB(const char* db_name) {
	if (db_name == nullptr || strlen(db_name) == 0) {
		m_bError = true;
		fprintf(stderr, "DB name is nullptr or empty\n");

		return -1;
	}

	if (nullptr == m_pMySql) {
		m_bError = true;
		fprintf(stderr, "MySql connect is nullptr\n");

		return -1;
	}

	if (mysql_select_db(m_pMySql->GetMySql(), db_name)) {
		fprintf(stderr, "error: select db=%s|%s",db_name, mysql_error(m_pMySql->GetMySql()));
	}

	return 0;
}

ssize_t MySqlDB::ExecSql(const char* sql) {
	m_bError = false;
	if (nullptr == sql || 0 == strlen(sql)) {
		fprintf(stderr, "execute sql is nullptr or empty\n");
		m_bError = true;

		return -1;
	}

	if (nullptr == m_pMySql) {
		m_bError = true;
		fprintf(stderr, "MySql connect is nullptr\n");

		return -1;
	}

	MYSQL* mysql = m_pMySql->GetMySql();
	if (0 != mysql_query(mysql, sql)) {
		fprintf(stderr, "%s|sql=%s",mysql_error(mysql), sql);
		m_bError = true;

		return -1;
	}

	return mysql_affected_rows(mysql);
}

MySqlRecordSet MySqlDB::QuerySql(const char* sql) {
	m_bError = false;
	MySqlRecordSet mysqlRecordSet; 

	if (nullptr == sql || 0 == strlen(sql)) {
		fprintf(stderr, "query sql is nullptr or empty\n");
		m_bError = true;

		return std::move(mysqlRecordSet);
	}

	if (nullptr == m_pMySql) {
		m_bError = true;
		fprintf(stderr, "MySql connect is nullptr\n");

		return std::move(mysqlRecordSet);
	}

	MYSQL* mysql = m_pMySql->GetMySql();
	if (0 != mysql_query(mysql, sql)) {
		fprintf(stderr, "%s|sql=%s",mysql_error(mysql), sql);
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
			if(NULL == mysql_row[col]) {
				vec.push_back(std::string(""));
			} else {
				vec.push_back(mysql_row[col]);
			}
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
		fprintf(stderr, "MySql connect is nullptr\n");

		return -1;
	}

	return mysql_insert_id(m_pMySql->GetMySql());
}

void MySqlDB::StartTransaction(void) {
	m_bError = false;
	if (nullptr == m_pMySql) {
		m_bError = true;
		fprintf(stderr, "MySql connect is nullptr\n");

		return;
	}

	MYSQL* mysql = m_pMySql->GetMySql();
	if (0 != mysql_query(mysql, "START TRANSACTION")) {
		fprintf(stderr, mysql_error(mysql));
		m_bError = true;

		return;
	}
}

void MySqlDB::Commint(void) {
	m_bError = false;
	if (nullptr == m_pMySql) {
		m_bError = true;
		fprintf(stderr, "MySql connect is nullptr\n");

		return;
	}

	MYSQL* mysql = m_pMySql->GetMySql();
	if (0 != mysql_query(mysql, "COMMIT")) {
		fprintf(stderr, mysql_error(mysql));
		m_bError = true;

		return;
	}
}

void MySqlDB::RollBack(void) {
	m_bError = false;
	if (nullptr == m_pMySql) {
		m_bError = true;
		fprintf(stderr, "MySql connect is nullptr\n");

		return;
	}

	MYSQL* mysql = m_pMySql->GetMySql();
	if (0 != mysql_query(mysql, "ROLLBACK")) {
		fprintf(stderr, mysql_error(mysql));
		m_bError = true;

		return;
	}
}

const char* MySqlDB::GetMySqlErrMsg(void) {
	if (nullptr != m_pMySql) {
		m_bError = true;
		//XINFO("connectID=%d, mysql_error:%s, isConnect:%d", m_connectID, mysql_error(m_pMySql->GetMySql()), m_bConnect);

		return mysql_error(m_pMySql->GetMySql());
	}

	return "";
}
/**** MySqlDB end *******************/
