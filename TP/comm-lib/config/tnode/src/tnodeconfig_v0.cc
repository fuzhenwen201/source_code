//
// Created by derek on 12/23/16.
//

#include "tnodeconfig_v0.h"

using namespace snetwork_xservice_tnode;

std::string TNodeConfigV0::GetConfigItem(const std::string &key)const{
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
