/*************************************************************************
	> File Name: SEconfig.cc
	> Author: kevin
	> Mail: kevin@1stellar.com
	> Created Time: Wednesday, April 19, 2017 PM03:57:53 HKT
 ************************************************************************/

#include "SEconfig.h"
#include<gflags/gflags.h>
namespace snetwork_xservice_xflagger {

DEFINE_string(loggerDir, "SignalEngine", "logger directory");
DEFINE_string(loggerFile, "signalengine", "logger file name");
DEFINE_string(accessToken, "95854af4a3fd24fe70806de498949c95-dab7a4cabec827224aafce0d9d79b3c2", "access token");
DEFINE_string(domain, "https://api-fxpractice.oanda.com", "");
DEFINE_string(mqvhost, "/demo", "rabbitmq vhost");
DEFINE_string(mqExchange, "t_node", "rabbitmq exchange");
DEFINE_string(mqExchangeGroup, "t_node_grp", "rabbitmq exchange group");
DEFINE_string(mqAddress, "localhost", "rabbitmq ip address");
DEFINE_string(mqPort, "5672", "rabbitmq port");
DEFINE_string(mqUser, "demo", "rabbitmq user");
DEFINE_string(mqPassword, "demo", "rabbitmq password");

SEConfig::SEConfig() {
}

SEConfig::~SEConfig() {
}

void SEConfig::Init(void) {
	LoggerDir(FLAGS_loggerDir);
	LoggerFile(FLAGS_loggerFile);
	AccessToken(FLAGS_accessToken);
	Domain(FLAGS_domain);
	VHost(FLAGS_mqvhost);
	Exchange(FLAGS_mqExchange);
	ExchangeGroup(FLAGS_mqExchangeGroup);
	Address(FLAGS_mqAddress);
	Port(FLAGS_mqPort);
	User(FLAGS_mqUser);
	Password(FLAGS_mqPassword);
}

std::ostream& operator<<(std::ostream& os, const SEConfig& s) {
	os << "loggerDir:\t" << s.loggerDir
		<< "\n" << "loggerFile:\t" << s.loggerFile
		<< "\n" << "accessToken:\t" << s.accessToken
		<< "\n" << "domain:\t" << s.domain
		<< "\n" << "mqvhost:\t" << s.mqvhost
		<< "\n" << "mqExchange:\t" << s.mqExchange
		<< "\n" << "mqExchangeGroup:\t" << s.mqExchangeGroup
		<< "\n" << "mqAddress:\t" << s.mqAddress
		<< "\n" << "mqPort:\t" << s.mqPort
		<< "\n" << "mqUser:\t" << s.mqUser
		<< "\n" << "mqPassword:\t" << s.mqPassword << "\n";
	return os;
}

SEFlagger* SEFlagger::m_seflagger = nullptr;

SEFlagger::SEFlagger() : m_config(nullptr) {
}

SEFlagger::~SEFlagger() {
	if(nullptr != m_config) {
		delete m_config;
		m_config = nullptr;
	}
}

SEFlagger* SEFlagger::GetInstance(void) {
	if (nullptr == m_seflagger) {
		m_seflagger = new SEFlagger;
	}
	return m_seflagger;
}

void SEFlagger::FreeInstance(void) {
	if (nullptr != m_seflagger) {
		delete m_seflagger;
	}
	m_seflagger = nullptr;
}

void SEFlagger::FlagXConfig(XConfig* cfg) {
	if(nullptr != m_config)
		delete m_config;
	m_config = cfg;
	cfg->Init();
}

}

