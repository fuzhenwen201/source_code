//
// Created by alex on 12/16/16.
//

#ifndef TNODE_H
#define TNODE_H

#include "snetwork_typedef.h"
#include "tnodeerrno.h"
#include "tnodeconfig.h"
#include "tnodechannel.h"


namespace snetwork_xservice_tnode{

    struct RabbitConfigStruct;
    typedef struct RabbitConfigStruct RabbitConfig;
    class TNodeRabbit;

    class TNode {
    public:
        TNode();
        ~TNode();

        virtual void Config(TNodeConfig &cfg);
        virtual void Bind(uint32 id);
        virtual void UnBind(uint32 id);
        virtual void Run();
        virtual void Stop();
        virtual bool IsReady();
        virtual bool Check();
        virtual TNodeChannel GetChannel();
        virtual TNodeChannel GetChannel(uint32 channel_id);

    private:
        uint32 id_;
        RabbitConfig* config_;
        TNodeRabbit* rabbit_;

    };

}

#endif //TNODE_H


