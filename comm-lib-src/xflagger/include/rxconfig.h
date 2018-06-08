/*************************************************************************
	> File Name: component_config.h
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Thu 06 Apr 2017 10:40:46 AM CST
 ************************************************************************/
#ifndef _RXCONFIGURE_H
#define _RXCONFIGURE_H
#include<mutex>
#include<ostream>
#include<utility>
#include<gflags/gflags.h>

/* ComponentGateway config
 * logger directory
 * logger filename
 * mysql: user, password, ip, port
 * component ID
 * rabbit mq: ip, port
 * redis: ip, pwd
 * */
class RXConfig {
	public:
		RXConfig();
		virtual ~RXConfig();

		RXConfig(const RXConfig& r);
		RXConfig& operator=(const RXConfig& r);
		RXConfig(RXConfig&& r);
		RXConfig& operator=(RXConfig&& r);

		virtual void Init(void);

	public:
		/*setters */
		void Screen(bool flag) {
			screen = flag;
		}
		bool Screen(void) const {
			return screen;
		}

		void Color(bool flag) {
			color = flag;
		}
		bool Color(void) const {
			return color;
		}

		void LoggerDir(const char* loggerDir) {
			this->loggerDir = loggerDir;
		}
		void LoggerDir(const std::string& loggerDir) {
			this->loggerDir = loggerDir;
		}
		void LoggerDir(std::string&& loggerDir) {
			this->loggerDir = std::move(loggerDir);
		}
		const std::string& LoggerDir(void) const {
			return loggerDir;
		}

		void LoggerFile(const char* loggerFile) {
			this->loggerFile = loggerFile;
		}
		void LoggerFile(const std::string& loggerFile) {
			this->loggerFile = loggerFile;
		}
		void LoggerFile(std::string&& loggerFile) {
			this->loggerFile = std::move(loggerFile);
		}
		const std::string& LoggerFile(void) {
			return loggerFile;
		}

		void ComponentID(const char* componentID) {
			this->componentID = componentID;
		}
		void ComponentID(const std::string& componentID) {
			this->componentID = componentID;
		}
		void ComponentID(std::string&& componentID) {
			this->componentID = std::move(componentID);
		}
		const std::string& ComponentID(void) const {
			return componentID;
		}

		void ComponentPort(int port) {
			this->componentPort = port;
		}
		int ComponentPort(void) const {
			return componentPort;
		}

		/* redis */
		void RedisHost(const char* redisHost) {
			this->redisHost = redisHost;
		}
		void RedisHost(const std::string& redisHost) {
			this->redisHost = redisHost;
		}
		void RedisHost(std::string&& redisHost) {
			this->redisHost = std::move(redisHost);
		}
		const std::string& RedisHost(void) const {
			return redisHost;
		}

		void RedisPort(int redisPort) {
			this->redisPort = redisPort;
		}
		int RedisPort(void) const {
			return redisPort;
		}

		void RedisDB(int redisDB) {
			this->redisDB = redisDB;
		}
		int RedisDB(void) const {
			return redisDB;
		}

		void RedisAuth(const char* redisAuth) {
			this->redisAuth = redisAuth;
		}
		void RedisAuth(const std::string& redisAuth) {
			this->redisAuth = redisAuth;
		}
		void RedisAuth(std::string&& redisAuth) {
			this->redisAuth =std::move(redisAuth);
		}
		const std::string& RedisAuth(void) const {
			return redisAuth;
		}

		void ThreadSize (int threadSize) {
			if (threadSize > 20 || threadSize < 1) {
				threadSize = 10;
			}
			this->threadSize = threadSize;
		}
		int ThreadSize(void) const {
			return threadSize;
		}

		void RedisSize(int redisSize) {
			if (redisSize > 8|| redisSize < 1) {
				redisSize = 5;
			}
			this->redisSize = redisSize;
		}
		int RedisSize(void) const {
			return redisSize;
		}

		/* rabbitmq */
		void VHost(const char* vhost) {
			mqVHost = vhost;
		}
		void VHost(const std::string& vhost) {
			mqVHost = vhost;
		}
		void VHost(std::string&& vhost) {
			mqVHost = std::move(vhost);
		}
		const std::string& VHost(void) const {
			return mqVHost;
		}

		void Exchange(const char* exchang) {
			mqExchange = exchang;
		}
		void Exchange(const std::string& exchang) {
			mqExchange = exchang;
		}
		void Exchange(std::string&& exchang) {
			mqExchange = std::move(exchang);
		}
		const std::string& Exchange(void) const {
			return mqExchange;
		}

		void ExchangeGroup(const char* exchangeGroup) {
			mqExchangeGroup = exchangeGroup;
		}
		void ExchangeGroup(const std::string& exchangeGroup) {
			mqExchangeGroup = exchangeGroup;
		}
		void ExchangeGroup(std::string&& exchangeGroup) {
			mqExchangeGroup = std::move(exchangeGroup);
		}
		const std::string& ExchangeGroup(void) const {
			return mqExchangeGroup;
		}
		
		void BindKey(const char* bindKey) {
			mqBindKey = bindKey;
		}
		void BindKey(const std::string& bindKey) {
			mqBindKey = bindKey;
		}
		void BindKey(std::string&& bindKey) {
			mqBindKey = std::move(bindKey);
		}
		const std::string& BindKey(void) const {
			return mqBindKey;
		}

		void QueueName(const char* queueName) {
			mqQueueName = queueName;
		}
		void QueueName(const std::string& queueName) {
			mqQueueName = queueName;
		}
		void QueueName(std::string&& queueName) {
			mqQueueName = std::move(queueName);
		}
		const std::string& QueueName(void) const {
			return mqQueueName;
		}

		void Address(const char* address) {
			mqAddress = address;
		}
		void Address(const std::string& address) {
			mqAddress = address;
		}
		void Address(std::string&& address) {
			mqAddress = std::move(address);
		}
		const std::string& Address(void) const {
			return mqAddress;
		}

		void Port(const char* port) {
			mqPort = port;
		}
		void Port(const std::string& port) {
			mqPort = port;
		}
		void Port(std::string&& port) {
			mqPort = std::move(port);
		}
		const std::string& Port(void) const {
			return mqPort;
		}

		void User(const char* user) {
			mqUser = user;
		}
		void User(const std::string& user) {
			mqUser = user;
		}
		void User(std::string&& user) {
			mqUser = std::move(user);
		}
		const std::string& User(void) const {
			return mqUser;
		}

		void Password(const char* password) {
			mqPassword = password;
		}
		void Password(const std::string& password) {
			mqPassword = password;
		}
		void Password(std::string&& password) {
			mqPassword = std::move(password);
		}
		const std::string& Password(void) const {
			return mqPassword;
		}

		void DBHost(const char* dbHost) {
			this->dbHost = dbHost;
		}
		void DBHost(const std::string& dbHost) {
			this->dbHost = dbHost;
		}
		void DBHost(std::string&& dbHost) {
			this->dbHost = std::move(dbHost);
		}
		const std::string& DBHost(void) const {
			return dbHost;
		}
			
		void DBUser(const char* dbUser) {
			this->dbUser = dbUser;
		}
		void DBUser(const std::string& dbUser) {
			this->dbUser = dbUser;
		}
		void DBUser(std::string&& dbUser) {
			this->dbUser = std::move(dbUser);
		}
		const std::string& DBUser(void) const {
			return dbUser;
		}

		void DBPwd(const char* dbPwd) {
			this->dbPwd = dbPwd;
		}
		void DBPwd(const std::string& dbPwd) {
			this->dbPwd = dbPwd;
		}
		void DBPwd(std::string&& dbPwd) {
			this->dbPwd = std::move(dbPwd);
		}
		const std::string& DBPwd(void) const {
			return dbPwd;
		}

		void DBName(const char* dbName) {
			this->dbName = dbName;
		}
		void DBName(const std::string& dbName) {
			this->dbName = dbName;
		}
		void DBName(std::string&& dbName) {
			this->dbName = std::move(dbName);
		}
		const std::string& DBName(void) const {
			return dbName;
		}

		void DBPort(int dbPort) {
			this->dbPort = dbPort;
		}
		int DBPort(void) const {
			return dbPort;
		}

		void DBMaxConn(int dbMaxConn) {
			this->dbMaxConn = dbMaxConn;
		}
		int DBMaxConn(void) const {
			return dbMaxConn;
		}

		friend std::ostream& operator<<(std::ostream& os, const RXConfig& r);

	private:
		/* component */
		bool screen;
		bool color;
		std::string loggerDir;
		std::string loggerFile;
		std::string componentID;
		int componentPort;

		/* redis */
		std::string redisHost;
		int redisPort;
		int redisDB;
		std::string redisAuth;

		/* pool size */
		int threadSize;
		int redisSize;

		/* rabbitmq */
		std::string mqVHost; 
		std::string mqExchange; 
		std::string mqExchangeGroup;
		std::string mqBindKey;
		std::string mqQueueName;
		std::string mqAddress;
		std::string mqPort;
		std::string mqUser;
		std::string mqPassword;
		std::string dbHost;
		std::string dbUser;
		std::string dbPwd;
		std::string dbName;
		int dbPort;
		int dbMaxConn;
};

class RXFlagger {
	private:
		RXFlagger();
		
	public:
		virtual ~RXFlagger();

		RXFlagger(const RXFlagger& r) = delete;
		RXFlagger& operator=(const RXFlagger& r) = delete;
		RXFlagger(RXFlagger&& r) = delete;
		RXFlagger& operator=(RXFlagger&& r) = delete;

	public:
		void FlagConfig(RXConfig* cfg);
		void ParseCommandLine(int* argc, char*** argv, bool cv);
		
	public:
		static RXFlagger* GetInstance(void);
		
	private:
		static RXFlagger* m_instance;
		static std::mutex m_mutex;
};
#endif

