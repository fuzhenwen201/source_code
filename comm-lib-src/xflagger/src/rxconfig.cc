/*************************************************************************
	> File Name: component_config.cc
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Thu 06 Apr 2017 11:55:06 AM CST
 ************************************************************************/
#include"rxconfig.h"

DEFINE_bool(screen, false, "printf message on terminator");
DEFINE_bool(color, true, "message color");
DEFINE_string(loggerDir,"../logs", "logger directory");
DEFINE_string(loggerFile,"Component", "logger name");
DEFINE_string(componentID,"1", "Component Gateway ID");
DEFINE_int32(componentPort, 5002, "tcp connection port");

/* redis */
DEFINE_string(redisHost,"127.0.0.1", "redis host");
DEFINE_int32(redisPort, 6379, "redis port");
DEFINE_int32(redisDB,0,"redis DB");
DEFINE_string(redisAuth,"", "redis password");

/* pool size */
//DEFINE_int32(connSize, 5, "mysql connect count");
DEFINE_int32(threadSize, 5, "thread count");
DEFINE_int32(redisSize, 5, "redis connect count");

/* rabbitmq */
DEFINE_string(mqVHost, "/demo", "rabbitmq vhost");
DEFINE_string(mqExchange, "t_node", "rabbitmq exchange");
DEFINE_string(mqExchangeGroup, "t_node_grp", "rabbitmq exchange group");
DEFINE_string(mqBindKey, "test.test", "rabbitmq bind key");
DEFINE_string(mqQueueName, "test", "rabbitmq queue name");
DEFINE_string(mqAddress, "localhost", "rabbitmq IP address");
DEFINE_string(mqPort, "5672", "rabbitmq port");
DEFINE_string(mqUser, "demo", "rabbitmq user");
DEFINE_string(mqPassword, "demo", "rabbitmq password");

DEFINE_string(dbHost, "127.0.0.1", "db host");
DEFINE_string(dbUser, "root", "db user name");
DEFINE_string(dbPwd, "root", "db password");
DEFINE_string(dbName, "test", "database name");
DEFINE_int32(dbPort, 3306, "db port");
DEFINE_int32(dbMaxConn, 5, "db max connection");

RXConfig::RXConfig() {
}

RXConfig::~RXConfig() {
}

RXConfig::RXConfig(const RXConfig& r) {
	screen = r.screen;
	color = r.color;
	loggerDir = r.loggerDir;
	loggerFile = r.loggerFile;
	componentID = r.componentID;
	componentPort = r.componentPort;
	
	redisHost = r.redisHost;
	redisPort = r.redisPort;
	redisDB = r.redisDB;
	redisAuth = r.redisAuth;
	
	threadSize = r.threadSize;
	redisSize = r.redisSize;

	mqVHost = r.mqVHost;
	mqExchange = r.mqExchange;
	mqExchangeGroup = r.mqExchangeGroup;
	mqBindKey = r.mqBindKey;
	mqQueueName = r.mqQueueName;
	mqAddress = r.mqAddress;
	mqPort = r.mqPort;
	mqUser = r.mqUser;
	mqPassword = r.mqPassword;

	dbHost = r.dbHost;
	dbUser = r.dbUser;
	dbPwd = r.dbPwd;
	dbName = r.dbName;
	dbPort = r.dbPort;
	dbMaxConn = r.dbMaxConn;
}

RXConfig::RXConfig(RXConfig&& r) {
	screen = r.screen;
	color = r.color;
	loggerDir = std::move(r.loggerDir);
	loggerFile = std::move(r.loggerFile);
	componentID = std::move(r.componentID);
	componentPort = r.componentPort;
	
	redisHost = std::move(r.redisHost);
	redisPort = r.redisPort;
	redisDB = r.redisDB;
	redisAuth = std::move(r.redisAuth);
	
	threadSize = r.threadSize;
	redisSize = r.redisSize;

	mqVHost = std::move(r.mqVHost);
	mqExchange = std::move(r.mqExchange);
	mqExchangeGroup = std::move(r.mqExchangeGroup);
	mqBindKey = std::move(r.mqBindKey);
	mqQueueName = std::move(r.mqQueueName);
	mqAddress = std::move(r.mqAddress);
	mqPort = std::move(r.mqPort);
	mqUser = std::move(r.mqUser);
	mqPassword = std::move(r.mqPassword);

	dbHost = std::move(r.dbHost);
	dbUser = std::move(r.dbUser);
	dbPwd = std::move(r.dbPwd);
	dbName = std::move(r.dbName);
	dbPort = r.dbPort;
	dbMaxConn = r.dbMaxConn;
}

RXConfig& RXConfig::operator=(const RXConfig& r) {
	if (this == &r) {
		return *this;
	}

	screen = r.screen;
	color = r.color;
	loggerDir = r.loggerDir;
	loggerFile = r.loggerFile;
	componentID = r.componentID;
	componentPort = r.componentPort;
	
	redisHost = r.redisHost;
	redisPort = r.redisPort;
	redisDB = r.redisDB;
	redisAuth = r.redisAuth;
	
	threadSize = r.threadSize;
	redisSize = r.redisSize;

	mqVHost = r.mqVHost;
	mqExchange = r.mqExchange;
	mqExchangeGroup = r.mqExchangeGroup;
	mqBindKey = r.mqBindKey;
	mqQueueName = r.mqQueueName;
	mqAddress = r.mqAddress;
	mqPort = r.mqPort;
	mqUser = r.mqUser;
	mqPassword = r.mqPassword;

	dbHost = r.dbHost;
	dbUser = r.dbUser;
	dbPwd = r.dbPwd;
	dbName = r.dbName;
	dbPort = r.dbPort;
	dbMaxConn = r.dbMaxConn;

	return *this;
}

RXConfig& RXConfig::operator=(RXConfig&& r) {
	if (this == &r) {
		return *this;
	}

	screen = r.screen;
	color = r.color;
	loggerDir = std::move(r.loggerDir);
	loggerFile = std::move(r.loggerFile);
	componentID = std::move(r.componentID);
	componentPort = r.componentPort;
	
	redisHost = std::move(r.redisHost);
	redisPort = r.redisPort;
	redisDB = r.redisDB;
	redisAuth = std::move(r.redisAuth);
	
	threadSize = r.threadSize;
	redisSize = r.redisSize;

	mqVHost = std::move(r.mqVHost);
	mqExchange = std::move(r.mqExchange);
	mqExchangeGroup = std::move(r.mqExchangeGroup);
	mqBindKey = std::move(r.mqBindKey);
	mqQueueName = std::move(r.mqQueueName);
	mqAddress = std::move(r.mqAddress);
	mqPort = std::move(r.mqPort);
	mqUser = std::move(r.mqUser);
	mqPassword = std::move(r.mqPassword);

	dbHost = std::move(r.dbHost);
	dbUser = std::move(r.dbUser);
	dbPwd = std::move(r.dbPwd);
	dbName = std::move(r.dbName);
	dbPort = r.dbPort;
	dbMaxConn = r.dbMaxConn;

	return *this;
}

void RXConfig::Init(void) {
	/* component */
	Screen(FLAGS_screen);
	Color(FLAGS_color);
	LoggerDir(FLAGS_loggerDir);
	LoggerFile(FLAGS_loggerFile);
	ComponentID(FLAGS_componentID);
	ComponentPort(FLAGS_componentPort);

	/* redis */
	RedisHost(FLAGS_redisHost);
	RedisPort(FLAGS_redisPort);
	RedisDB(FLAGS_redisDB);
	RedisAuth(FLAGS_redisAuth);

	/* pool */
	//ConnSize(FLAGS_connSize);
	ThreadSize(FLAGS_threadSize);
	RedisSize(FLAGS_redisSize);

	/* rabbitmq */
	VHost(FLAGS_mqVHost);
	Exchange(FLAGS_mqExchange);
	ExchangeGroup(FLAGS_mqExchangeGroup);
	BindKey(FLAGS_mqBindKey);
	QueueName(FLAGS_mqQueueName);
	Address(FLAGS_mqAddress);
	Port(FLAGS_mqPort);
	User(FLAGS_mqUser);
	Password(FLAGS_mqPassword);

	DBHost(FLAGS_dbHost);
	DBUser(FLAGS_dbUser);
	DBPwd(FLAGS_dbPwd);
	DBName(FLAGS_dbName);
	DBPort(FLAGS_dbPort);
	DBMaxConn(FLAGS_dbMaxConn);
}


std::ostream& operator<<(std::ostream&os, const RXConfig& r) {
	os<<"screen:\t\t"<<r.screen
		<<"\n"<<"color:\t\t"<<r.color
		<<"\n"<<"loggerDir:\t"<<r.loggerDir
		<<"\n"<<"loggerFile:\t"<<r.loggerFile
		<<"\n"<<"componentID:\t"<<r.componentID
		<<"\n"<<"componentPort:\t"<<r.componentPort
		<<"\n"<<"redisHost:\t"<<r.redisHost
		<<"\n"<<"redisPort:\t"<<r.redisPort
		<<"\n"<<"redisDB:\t"<<r.redisDB
		<<"\n"<<"redisAuth:\t"<<r.redisAuth
		<<"\n"<<"threadSize:\t"<<r.threadSize
		<<"\n"<<"redisSize:\t"<<r.redisSize
		<<"\n"<<"mqVHost:\t"<<r.mqVHost
		<<"\n"<<"mqExchange:\t"<<r.mqExchange
		<<"\n"<<"mqExchangeGroup:"<<r.mqExchangeGroup
		<<"\n"<<"mqBindKey:\t"<<r.mqBindKey
		<<"\n"<<"mqQueueName:\t"<<r.mqQueueName
		<<"\n"<<"mqAddress:\t"<<r.mqAddress
		<<"\n"<<"mqPort:\t\t"<<r.mqPort
		<<"\n"<<"mqUser:\t\t"<<r.mqUser
		<<"\n"<<"mqPassword:\t"<<r.mqPassword
		<< "\ndbHost:		"<<r.dbHost
		<<"\ndbUser:		"<<r.dbUser
		<<"\ndbPwd:		"<<r.dbPwd
		<<"\ndbName:		"<<r.dbName
		<<"\ndbPort:		"<<r.dbPort
		<<"\ndbMaxConn:	"<<r.dbMaxConn
		<<"\n";

	return os;
}

/**RXFlagger begin**/
RXFlagger* RXFlagger::m_instance = nullptr;
std::mutex RXFlagger::m_mutex;

RXFlagger::RXFlagger() {
}

RXFlagger* RXFlagger::GetInstance(void) {
	if (m_instance == nullptr) {
		std::lock_guard<std::mutex> lk(m_mutex);
		if (m_instance == nullptr) {
			m_instance = new RXFlagger;
		}
	}

	return m_instance;
}

RXFlagger::~RXFlagger() {
	std::lock_guard<std::mutex> lk(m_mutex);
	google::ShutDownCommandLineFlags();
}

void RXFlagger::FlagConfig(RXConfig* cfg) {
	std::lock_guard<std::mutex> lk(m_mutex);
	cfg->Init();
}

void RXFlagger::ParseCommandLine(int* argc, char*** argv, bool cv) {
	std::lock_guard<std::mutex> lk(m_mutex);
    google::ParseCommandLineFlags(argc, argv, cv);
}
/**RXFlagger end**/
