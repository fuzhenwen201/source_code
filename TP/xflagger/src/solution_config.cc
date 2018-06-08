/*************************************************************************
	> File Name: solution_config.cc
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Thu 06 Apr 2017 11:55:06 AM CST
 ************************************************************************/
#include"solution_config.h"
#include<gflags/gflags.h>

namespace snetwork_xservice_xflagger {

DEFINE_bool(screen, false, "printf message on terminator");
DEFINE_bool(color, true, "message color");
DEFINE_string(loggerDir,"../logs", "logger directory");
//DEFINE_string(loggerDir,"SolutionGateway", "logger directory");
DEFINE_string(loggerFile,"SolutionGateway", "logger name");
DEFINE_string(solutionID,"1", "Solution Gateway ID");
DEFINE_int32(solutionPort, 5002, "tcp connection port");

/* database */
DEFINE_string(mySqlHost,"35.154.147.154", "mysql host");
DEFINE_string(mySqlUser,"root", "mysql user");
DEFINE_string(mySqlPassword,"root", "mysql password");
DEFINE_int32(mySqlPort, 3306, "mysql port");
DEFINE_string(mySqlDBName, "test", "mysql database name");

/* redis */
DEFINE_string(redisHost,"127.0.0.1", "redis host");
DEFINE_int32(redisPort, 6379, "redis port");
DEFINE_int32(redisDB,0,"redis DB");
DEFINE_string(redisAuth,"hugo123456", "redis password");

/* pool size */
DEFINE_int32(connSize, 5, "mysql connect count");
DEFINE_int32(threadSize, 5, "thread count");
DEFINE_int32(redisSize, 5, "redis connect count");

/* rabbitmq */
DEFINE_string(mqVHost, "/demo", "rabbitmq vhost");
DEFINE_string(mqExchange, "t_node", "rabbitmq exchange");
DEFINE_string(mqExchangeGroup, "t_node_grp", "rabbitmq exchange group");
DEFINE_string(mqAddress, "localhost", "rabbitmq IP address");
DEFINE_string(mqPort, "5672", "rabbitmq port");
DEFINE_string(mqUser, "demo", "rabbitmq user");
DEFINE_string(mqPassword, "demo", "rabbitmq password");
DEFINE_string(sgcipher, "0123456789102540", "ciphertext");

SXConfig::SXConfig() {

}

SXConfig::~SXConfig() {
}

void SXConfig::Init(void) {
	/* solution */
	Screen(FLAGS_screen);
	Color(FLAGS_color);
	LoggerDir(FLAGS_loggerDir);
	LoggerFile(FLAGS_loggerFile);
	SolutionID(FLAGS_solutionID);
	SolutionPort(FLAGS_solutionPort);

	/* mysql */
	MySqlHost(FLAGS_mySqlHost);
	MySqlUser(FLAGS_mySqlUser);
	MySqlPassword(FLAGS_mySqlPassword);
	MySqlPort(FLAGS_mySqlPort);
	MySqlDBName(FLAGS_mySqlDBName);

	/* redis */
	RedisHost(FLAGS_redisHost);
	RedisPort(FLAGS_redisPort);
	RedisDB(FLAGS_redisDB);
	RedisAuth(FLAGS_redisAuth);

	/* pool */
	ConnSize(FLAGS_connSize);
	ThreadSize(FLAGS_threadSize);
	RedisSize(FLAGS_redisSize);

	/* rabbitmq */
	VHost(FLAGS_mqVHost);
	Exchange(FLAGS_mqExchange);
	ExchangeGroup(FLAGS_mqExchangeGroup);
	Address(FLAGS_mqAddress);
	Port(FLAGS_mqPort);
	User(FLAGS_mqUser);
	Password(FLAGS_mqPassword);
	Cipher(FLAGS_sgcipher);
}


std::ostream& operator<<(std::ostream&os, const SXConfig& r) {
	os<<"screen:\t"<<r.screen
		<<"\n"<<"color:\t"<<r.color
		<<"\n"<<"loggerDir:\t"<<r.loggerDir
		<<"\n"<<"loggerFile:\t"<<r.loggerFile
		<<"\n"<<"solutionID:\t"<<r.solutionID
		<<"\n"<<"solutionPort:\t"<<r.solutionPort
		<<"\n"<<"mySqlHost:\t"<<r.mySqlHost
		<<"\n"<<"mySqlUser:\t"<<r.mySqlUser
		<<"\n"<<"mySqlPassword:\t"<<r.mySqlPassword
		<<"\n"<<"mySqlPort:\t"<<r.mySqlPort
		<<"\n"<<"mySqlDBName:\t"<<r.mySqlDBName
		<<"\n"<<"redisHost:\t"<<r.redisHost
		<<"\n"<<"redisPort:\t"<<r.redisPort
		<<"\n"<<"redisDB:\t"<<r.redisDB
		<<"\n"<<"redisAuth:\t"<<r.redisAuth
		<<"\n"<<"connSize:\t"<<r.connSize
		<<"\n"<<"threadSize:\t"<<r.threadSize
		<<"\n"<<"redisSize:\t"<<r.redisSize
		<<"\n"<<"mqVHost:\t"<<r.mqVHost
		<<"\n"<<"mqExchange:\t"<<r.mqExchange
		<<"\n"<<"mqExchangeGroup:\t"<<r.mqExchangeGroup
		<<"\n"<<"mqAddress:\t"<<r.mqAddress
		<<"\n"<<"mqPort:\t"<<r.mqPort
		<<"\n"<<"mqUser:\t"<<r.mqUser
		<<"\n"<<"mqPassword:\t"<<r.mqPassword
	    <<"\n"<<"sgcipher:\t"<<r.sgcipher
		<<"\n";

	return os;
}

/**SXFlagger begin**/
SXFlagger* SXFlagger::m_instance = nullptr;
std::mutex SXFlagger::m_mutex;

SXFlagger::SXFlagger() : m_config(nullptr) {
}

SXFlagger* SXFlagger::GetInstance(void) {
	if (m_instance == nullptr) {
		std::lock_guard<std::mutex> lk(m_mutex);
		if (m_instance == nullptr) {
			m_instance = new SXFlagger;
		}
	}

	return m_instance;
}

SXFlagger::~SXFlagger() {
	if (m_config != nullptr) {
		delete m_config;
		m_config = nullptr;
	}
}

void SXFlagger::FlagXConfig(XConfig* cfg) {
	do {
		if (m_config == nullptr) {
			m_config = cfg;
			cfg->Init();

			break;
		}

#if 0
		if (cfg != m_config) {
			delete m_config;
			m_config = cfg;
			cfg->Init();

			break;
		}
#endif
	}while(false);
}
/**SXFlagger end**/

}
