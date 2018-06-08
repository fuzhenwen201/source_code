//
// Created by derek on 12/23/16.
//

#include "tnodeconfig_v0.h"

using namespace snetwork_xservice_tnode;

std::string TNodeConfigV0::GetConfigItem(std::string key){
    if ("vhost" == key){
        return this->VHost();
    }else if("exchange" == key){
        return this->Exchange();
    }else if("exchange_grp" == key){
        return this->ExchangeGroup();
    }else if("address" == key){
        return this->Address();
    }else if("port" == key){
        return this->Port();
    }else if("user" == key){
        return this->User();
    }else if("password" == key){
        return this->Password();
    }else {
        return ""; // unknown key// this condition shall be saved into error
    }
}

TNodeConfigV0::TNodeConfigV0() {
}

TNodeConfigV0::~TNodeConfigV0(){
}

// getters
std::string TNodeConfigV0::VHost() {
    return this->vhost;
}

std::string TNodeConfigV0::Exchange(){
    return this->exchange;
}

std::string TNodeConfigV0::ExchangeGroup(){
    return this->exchange_grp;
}

std::string TNodeConfigV0::Address(){
    return this->address;
}

std::string TNodeConfigV0::Port(){
    return this->port;
}

std::string TNodeConfigV0::User(){
    return this->user;
}

std::string TNodeConfigV0::Password(){
    return this->password;
}

// setters
TNodeConfigV0& TNodeConfigV0::VHost(std::string vhost) {
    this->vhost = vhost;
    return *this;
}

TNodeConfigV0& TNodeConfigV0::Exchange(std::string exchange){
    this->exchange = exchange;
    return *this;
}

TNodeConfigV0& TNodeConfigV0::ExchangeGroup(std::string exchange_grp){
    this->exchange_grp = exchange_grp;
    return *this;
}

TNodeConfigV0& TNodeConfigV0::Address(std::string address){
    this->address = address;
    return *this;
}

TNodeConfigV0& TNodeConfigV0::Port(std::string port){
    this->port = port;
    return *this;
}

TNodeConfigV0& TNodeConfigV0::User(std::string user){
    this->user = user;
    return *this;
}

TNodeConfigV0& TNodeConfigV0::Password(std::string password) {
    this->password = password;
    return *this;
}

