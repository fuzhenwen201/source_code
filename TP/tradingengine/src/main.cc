#include <iostream>
#include "xlogger.h"
#include "tradingengine.h"
#include "TEconfig.h"
#include "tnodeconfig_v0.h"

using namespace snetwork_tradingengine;

int main(int argc, char* argv[]) {
    TradingBO Bo;
    uint32_t errcode;
    snetwork_xservice_tnode::TNodeConfigV0 quota_cfg;
    snetwork_xservice_tnode::TNodeConfigV0 order_cfg;

    using namespace snetwork_xservice_xflagger;
    Teconfig* xcfg = new Teconfig;
    xcfg->ParseCommandLine(&argc, &argv, false);
    XLOGGER(xcfg->Getconfig("loggerfile").c_str())
            .StdErr(xcfg->Getboolconfig("screen"))
            .Colour(xcfg->Getboolconfig("color"))
             .Dir(xcfg->Getconfig("loggerdir").c_str());

    order_cfg
    .VHost(xcfg->Getconfig("mqvhost").c_str())
    .Exchange(xcfg->Getconfig("mqExchange").c_str())
    .ExchangeGroup(xcfg->Getconfig("mqExchangeGroup").c_str())
    .Address(xcfg->Getconfig("mqAddress").c_str())
    .Port(xcfg->Getconfig("mqPort").c_str())
    .User(xcfg->Getconfig("mqUser").c_str())
    .Password(xcfg->Getconfig("mqPassword").c_str());

    quota_cfg
    .VHost(xcfg->Getconfig("quota_mqvhost").c_str())
    .Exchange(xcfg->Getconfig("quota_mqExchange").c_str())
    .ExchangeGroup(xcfg->Getconfig("quota_mqExchangeGroup").c_str())
    .Address(xcfg->Getconfig("quota_mqAddress").c_str())
    .Port(xcfg->Getconfig("quota_mqPort").c_str())
    .User(xcfg->Getconfig("quota_mqUser").c_str())
    .Password(xcfg->Getconfig("quota_mqPassword").c_str());

    bool Botrading = xcfg->Getboolconfig("Botrading");
    errcode = Bo.Init(Botrading);
    if (UNIVERSAL_TRADING_SUCCESS != errcode){
        XERROR("BO initialization failed!");
        return errcode;
    }

    /*std::string payratio = xcfg->Getconfig("payratio");

    for (uint32_t i = 0; i < symbol_size; ++i){
        Bo.SetPayRatio(i, (uint32_t)std::stoi(payratio, nullptr));
    }*/

    Bo.Run(quota_cfg, order_cfg, xcfg->Getconfig("id").c_str());
    return 0;
}

