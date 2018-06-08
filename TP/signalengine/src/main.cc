#include <iostream>
#include <thread>
#include <zconf.h>
#include "xconfig.h"
#include "SEconfig.h"
#include "xlogger.h"
#include "tnodeconfig_v0.h"
#include "marketdata.h"
#include "marketdataagent.h"

using namespace snetwork_signalengine_marketdata;

#if 1
int main(int argc, char* argv[]) {
    MarketData market;
    uint32_t errcode;
    snetwork_xservice_tnode::TNodeConfigV0 cfg;

	using namespace snetwork_xservice_xflagger;
    XConfig* xcfg = new SEConfig;
    XFlagger* flagger = SEFlagger::GetInstance();
	flagger->ParseCommandLine(&argc, &argv, false);
    flagger->FlagXConfig(xcfg);
    SEConfig* seconfig = dynamic_cast<SEConfig*>(flagger->GetConfig());
    if(nullptr == seconfig) {
        XERROR("\nERROR:Failed to get the configuration file!\n");
        return 0;
    }
    XLOGGER(seconfig->LoggerFile().c_str())
            .StdErr(false)
            .Colour(true)
            .Dir(seconfig->LoggerDir().c_str());

    market.SetAccessToken(seconfig->AccessToken().c_str());
    market.SetDomain(seconfig->Domain().c_str());

    cfg
    .VHost(seconfig->VHost().c_str())
    .Exchange(seconfig->Exchange().c_str())
    .ExchangeGroup(seconfig->ExchangeGroup().c_str())
    .Address(seconfig->Address().c_str())
    .Port(seconfig->Port().c_str())
    .User(seconfig->User().c_str())
    .Password(seconfig->Password().c_str());

    SEFlagger::FreeInstance();
    flagger = nullptr;

    errcode = market.Init(cfg);
    if (UNIVERSAL_MARKET_SUCCESS != errcode){
        return errcode;
    }
#if 0
    std::thread thread = std::thread([&]{
        MarketDataAgent agent;
        snetwork_xservice_tnode::TNode tNode;

        std::string price;
        std::string symbol = "EUR_USD";

        sleep(5);

        tNode.Config(cfg);
        tNode.Bind(1);
        tNode.Run();

        agent.Init(tNode, "test");

        while (1){
            agent.GetMarketData(symbol, price);

            std::cout << price << std::endl;

            sleep(1);
        }
    });

    thread.detach();
#endif
    market.Run();

    return 0;
}

#endif


#if 0
int main() {
    MarketData market;
    uint32_t errcode;
    snetwork_xservice_tnode::TNodeConfigV0 cfg;

    XLOGGER("signalengine")
            .StdErr(false)
            .Colour(true)
            .Dir("./log");

    market.SetAccessToken("95854af4a3fd24fe70806de498949c95-dab7a4cabec827224aafce0d9d79b3c2");
    market.SetDomain("https://api-fxpractice.oanda.com");

    cfg
    .VHost("/demo")
    .Exchange("t_node")
    .ExchangeGroup("t_node_grp")
    .Address("localhost")
    .Port("5672")
    .User("demo")
    .Password("demo");

    errcode = market.Init(cfg);
    if (UNIVERSAL_MARKET_SUCCESS != errcode){
        return errcode;
    }

    market.Run();

    return 0;
}

#endif



#if 0
int main(){
    MarketDataAgent agent;
    snetwork_xservice_tnode::TNodeConfigV0 cfg;
    cfg
    .VHost("/demo")
    .Exchange("t_node")
    .ExchangeGroup("t_node_grp")
    .Address("localhost")
    .Port("5672")
    .User("demo")
    .Password("demo");


    snetwork_xservice_tnode::TNode tNode;

    tNode.Config(cfg);
    tNode.Bind(1);
    tNode.Run();

    agent.Init(tNode, "test");

    std::string price;
    std::string symbol = "EUR_USD";

    while (1){
        agent.GetMarketData(symbol, price);

        std::cout << price << std::endl;

        sleep(1);
    }

    return 0;
}
#endif


