//
// Created by alex on 12/22/16.
//

#ifndef TNODE_CONFIG_H
#define TNODE_CONFIG_H

#include <string>

namespace snetwork_xservice_tnode
{

    class TNodeConfig {

    public:
        virtual std::string GetConfigItem(std::string key) = 0;

    };

}

#endif //TNODE_CONFIG_H
