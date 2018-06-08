/*************************************************************************
    > File Name: main.cc
    > Author: Kevin
    > Mail: kevin.fu@fintechstar.cn
    > Created Time: Thu 11 Jan 2018 12:17:35 PM CST
 ************************************************************************/

#include "Subscriptionmsg.h"

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cout << "usage: " << argv[ 0 ] << " FILE." << std::endl;
		return 0;
	}

	STTnodeConfig config = {};
	RedisConfigInfo rediconfig = {};
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

	rediconfig.redisaddr = cfg.Getconfig("redisIP");
	rediconfig.redispass = cfg.Getconfig("redisPass");
	rediconfig.redisport = cfg.Getintconfig("redisPort");
	struct timeval timeout = {2, 0};    //2s的超时时间
	g_pRedisContext = redisConnectWithTimeout(rediconfig.redisaddr.c_str(), rediconfig.redisport, timeout);
	if((nullptr == g_pRedisContext) || g_pRedisContext->err){
		if(g_pRedisContext)
			XERROR("connect redis error:%s", g_pRedisContext->errstr);
		else
			XERROR("connect redis error: can't allocate redis context.");
        return 1;
	}

	auto *pRedisReply = (redisReply*)redisCommand(g_pRedisContext, "AUTH %s", rediconfig.redispass.c_str());  //执行AUTH命令
	if(pRedisReply->type == 6){
        XERROR("Redis authentication failed.");
		freeReplyObject(pRedisReply);
		return 1;
	}else{
		XINFO("Redis certification is successful.");
	}
	freeReplyObject(pRedisReply);

	try {
		OrderService subscription;
		Applications application;
        application.init("OrderMsgService.cfg", config, rediconfig);
        subscription.Init(config, rediconfig, stid);
		subscription.Run();
	} catch ( std::exception & e ) {
		XERROR("%s", e.what());
		return 1;
	}
}
