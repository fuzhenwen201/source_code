//
// Created by alex on 12/22/16.
//

#ifndef TNODE_RABBIT_MANAGER_H
#define TNODE_RABBIT_MANAGER_H

#include <mutex>
#include "tnoderabbit.h"

namespace snetwork_xservice_tnode
{
    typedef struct RabbitManagerStruct{
        uint32      id;
        TNodeRabbit* rabbit;
        struct RabbitManagerStruct* next;
    } RabbitManager;

    class TNodeRabbitManager {

    public:
        static TNodeRabbit* GetRabbit(uint32 id);
        static void FreeRabbit(uint32 id);

        static bool Check(uint32 id, TNodeRabbit* rabbit);

    private:
        static std::mutex mutex_;
        static RabbitManager* rabbit_list_;
    };

}


#endif //TNODE_RABBIT_MANAGER_H
