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

        std::string GetConfigItem(const std::string &key) const;

        // getters
        std::string VHost() const;

        std::string Exchange() const;

        std::string ExchangeGroup() const;

        std::string Address() const;

        std::string Port() const;

        std::string User() const;

        std::string Password() const;

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

    inline std::string TNodeConfigV0::VHost() const{
        return this->vhost;
    }

    inline std::string TNodeConfigV0::Exchange() const{
        return this->exchange;
    }

    inline std::string TNodeConfigV0::ExchangeGroup() const{
        return this->exchange_grp;
    }

    inline std::string TNodeConfigV0::Address() const{
        return this->address;
    }

    inline std::string TNodeConfigV0::Port() const{
        return this->port;
    }

    inline std::string TNodeConfigV0::User() const{
        return this->user;
    }

    inline std::string TNodeConfigV0::Password() const{
        return this->password;
    }

    inline TNodeConfigV0& TNodeConfigV0::VHost(std::string vhost) {
        this->vhost = vhost;
        return *this;
    }

    inline TNodeConfigV0& TNodeConfigV0::Exchange(std::string exchange){
        this->exchange = exchange;
        return *this;
    }

    inline TNodeConfigV0& TNodeConfigV0::ExchangeGroup(std::string exchange_grp){
        this->exchange_grp = exchange_grp;
        return *this;
    }

    inline TNodeConfigV0& TNodeConfigV0::Address(std::string address){
        this->address = address;
        return *this;
    }

    inline TNodeConfigV0& TNodeConfigV0::Port(std::string port){
        this->port = port;
        return *this;
    }

    inline TNodeConfigV0& TNodeConfigV0::User(std::string user){
        this->user = user;
        return *this;
    }

    inline TNodeConfigV0& TNodeConfigV0::Password(std::string password) {
        this->password = password;
        return *this;
    }
}

#endif //TNODECONFIG_V0_H
