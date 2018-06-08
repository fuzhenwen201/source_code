/*************************************************************************
    > File Name: main.cc
    > Author: Kevin
    > Mail: kevin.fu@fintechstar.cn
    > Created Time: Thu 11 Jan 2018 12:17:35 PM CST
 ************************************************************************/

#include "SubscriptionService.h"
redisContext *g_pRedisContext = nullptr;

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cout << "usage: " << argv[ 0 ] << " FILE." << std::endl;
		return 0;
	}

	STTnodeConfig config;
	using namespace snetwork_xservice_xflagger;
	Config cfg;
	cfg.ParseCommandLine(&argc, &argv, false);
	XLOGGER(cfg.Getconfig("loggerfile").c_str())
			.StdErr(cfg.Getboolconfig("screen"))
			.Colour(cfg.Getboolconfig("color"))
			.Dir(cfg.Getconfig("loggerdir"));

	config.mq_exchange_group = cfg.Getconfig("mqExchangeGroup");
	config.mq_vhost = cfg.Getconfig("mqvhost");
	config.mq_host = cfg.Getconfig("mqAddress");
	config.mq_passwd = cfg.Getconfig("mqPassword");
	config.mq_port = cfg.Getintconfig("mqPort");
	config.mq_user = cfg.Getconfig("mqUser");
	std::string stid = cfg.Getconfig("id");

	RedisConfigInfo rediconfig = {};
	rediconfig.redisaddr = cfg.Getconfig("redisIP");
	rediconfig.redispass = cfg.Getconfig("redisPass");
	rediconfig.redisport = cfg.Getintconfig("redisPort");

	try {
		Applications application;
		SubscriptionService subscription;
		application.init("SubscriptionServer.cfg", config);
		subscription.Init(config, rediconfig, stid);
		subscription.Run();
	} catch ( std::exception & e ) {
		std::cout << e.what() << std::endl;
		return 1;
	}
}