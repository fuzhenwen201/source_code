//
// Created by derek on 12/23/16.
//

#ifndef TNODECONFIG_V0_H
#define TNODECONFIG_V0_H
#include "tnodeconfig.h"

namespace snetwork_xservice_tnode{

    class TNodeConfigV0: public TNodeConfig {

    public:

        TNodeConfigV0();

        ~TNodeConfigV0();

        std::string GetConfigItem(std::string key);

        // getters
        std::string VHost();

        std::string Exchange();

        std::string ExchangeGroup();

        std::string Address();

        std::string Port();

        std::string User();

        std::string Password();

        // setters
        TNodeConfigV0& VHost(std::string);

        TNodeConfigV0& Exchange(std::string);

        TNodeConfigV0& ExchangeGroup(std::string);

        TNodeConfigV0& Address(std::string);

        TNodeConfigV0& Port(std::string);

        TNodeConfigV0& User(std::string);

        TNodeConfigV0& Password(std::string);

    private:

        std::string vhost;
        std::string exchange;
        std::string exchange_grp;
        std::string address;
        std::string port;
        std::string user;
        std::string password;

    };

}

#endif //TNODECONFIG_V0_H
