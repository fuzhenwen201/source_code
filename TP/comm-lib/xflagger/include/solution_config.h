/*************************************************************************
	> File Name: solution_config.h
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Thu 06 Apr 2017 10:40:46 AM CST
 ************************************************************************/
#ifndef SOLUTION_CONFIG_H_
#define SOLUTION_CONFIG_H_
#include"xconfig.h"
#include<mutex>
#include<ostream>

namespace snetwork_xservice_xflagger {
/* SolutionGateway config
 * logger directory
 * logger filename
 * mysql: user, password, ip, port
 * solution ID
 * rabbit mq: ip, port
 * redis: ip, pwd
 * */
class SXConfig : public XConfig {
	public:
		SXConfig();
		virtual ~SXConfig();

		SXConfig(const SXConfig& r) = delete;
		SXConfig& operator=(const SXConfig& r) = delete;
		SXConfig(SXConfig&& r) = delete;
		SXConfig& operator=(SXConfig&& r) = delete;

		virtual void Init(void) override;

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

		void LoggerDir(const std::string& loggerDir) {
			this->loggerDir = loggerDir;
		}
		const std::string& LoggerDir(void) const {
			return loggerDir;
		}

		void LoggerFile(const std::string& loggerFile) {
			this->loggerFile = loggerFile;
		}
		const std::string& LoggerFile(void) {
			return loggerFile;
		}

		void SolutionID(const std::string& solutionID) {
			this->solutionID = solutionID;
		}
		const std::string& SolutionID(void) const {
			return solutionID;
		}

		void SolutionPort(int port) {
			this->solutionPort = port;
		}
		int SolutionPort(void) const {
			return solutionPort;
		}

		/* mysql */
		void MySqlHost(const std::string& mySqlHost) {
			this->mySqlHost = mySqlHost;
		}
		const std::string& MySqlHost(void) const {
			return mySqlHost;
		}

		void MySqlUser(const std::string& mySqlUser) {
			this->mySqlUser = mySqlUser;
		}
		const std::string& MySqlUser(void) const {
			return mySqlUser;
		}

		void MySqlPassword(const std::string& mySqlPassword) {
			this->mySqlPassword = mySqlPassword;
		}
		const std::string& MySqlPassword(void) const {
			return mySqlPassword;
		}

		void MySqlPort(int mySqlPort) {
			this->mySqlPort = mySqlPort;
		}
		int MySqlPort(void) const {
			return mySqlPort;
		}

		void MySqlDBName(const std::string& mySqlDBName) {
			this->mySqlDBName = mySqlDBName;
		}
		const std::string& MySqlDBName(void) const {
			return mySqlDBName;
		}

		/* redis */
		void RedisHost(const std::string& redisHost) {
			this->redisHost = redisHost;
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

		void RedisAuth(const std::string& redisAuth) {
			this->redisAuth = redisAuth;
		}
		const std::string& RedisAuth(void) const {
			return redisAuth;
		}

		/* pool */
		void ConnSize(int connSize) {
			if (connSize > 20 || connSize < 1) {
				connSize = 10;
			}
			this->connSize = connSize;
		}
		int ConnSize(void) const {
			return connSize;
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
		void VHost(const std::string& vhost) {
			mqVHost = vhost;
		}
		const std::string& VHost(void) const {
			return mqVHost;
		}

		void Exchange(const std::string& exchang) {
			mqExchange = exchang;
		}
		const std::string& Exchange(void) const {
			return mqExchange;
		}

		void ExchangeGroup(const std::string& exchangeGroup) {
			mqExchangeGroup = exchangeGroup;
		}
		const std::string& ExchangeGroup(void) const {
			return mqExchangeGroup;
		}
		
		void BindKey(const std::string& bindKey) {
			mqBindKey = bindKey;
		}
		const std::string& BindKey(void) const {
			return mqBindKey;
		}

		void QueueName(const std::string& queueName) {
			mqQueueName = queueName;
		}
		const std::string& QueueName(void) const {
			return mqQueueName;
		}

		void Address(const std::string& address) {
			mqAddress = address;
		}
		const std::string& Address(void) const {
			return mqAddress;
		}

		void Port(const std::string& port) {
			mqPort = port;
		}
		const std::string& Port(void) const {
			return mqPort;
		}

		void User(const std::string& user) {
			mqUser = user;
		}
		const std::string& User(void) const {
			return mqUser;
		}

		void Password(const std::string& password) {
			mqPassword = password;
		}
		const std::string& Password(void) const {
			return mqPassword;
		}

		void Cipher(const std::string& cipher) {
			sgcipher = cipher;
		}

		const std::string& Cipher(void) const {
			return sgcipher;
		}

		friend std::ostream& operator<<(std::ostream& os, const SXConfig& r);

	private:
		/* solution */
		bool screen;
		bool color;
		std::string loggerDir;
		std::string loggerFile;
		std::string solutionID;
		int solutionPort;
		/* database */
		std::string mySqlHost;
		std::string mySqlUser;
		std::string mySqlPassword;
		int mySqlPort;
		std::string mySqlDBName;

		/* redis */
		std::string redisHost;
		int redisPort;
		int redisDB;
		std::string redisAuth;

		/* pool size */
		int connSize;   /* mysql*/
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
		std::string sgcipher;
};

class SXFlagger : public XFlagger {
	private:
		SXFlagger();
		virtual ~SXFlagger();

		SXFlagger(const SXFlagger& r) = delete;
		SXFlagger& operator=(const SXFlagger& r) = delete;
		SXFlagger(SXFlagger&& r) = delete;
		SXFlagger& operator=(SXFlagger&& r) = delete;

	public:
		virtual void FlagXConfig(XConfig* cfg) override ;
		virtual XConfig* GetConfig(void) const override {
			return m_config;
		}
		
	public:
		static SXFlagger* GetInstance(void);
		
	private:
		XConfig* m_config;
		static SXFlagger* m_instance;
		static std::mutex m_mutex;
};
}

#endif

