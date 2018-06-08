//
// Created by alex on 12/22/16.
//

#include "tnoderabbitmanager.h"

using namespace snetwork_xservice_tnode;

std::mutex TNodeRabbitManager::mutex_;
RabbitManager* TNodeRabbitManager::rabbit_list_ = nullptr;


TNodeRabbit* TNodeRabbitManager::GetRabbit(uint32 id){
    RabbitManager*  rabbit_ptr;
    TNodeRabbit*    rabbit_instance;

    mutex_.lock();
    rabbit_ptr = rabbit_list_;

    while (nullptr != rabbit_ptr){

        if (id == rabbit_ptr->id){

            rabbit_instance = rabbit_ptr->rabbit;
            mutex_.unlock();
            return  rabbit_instance;
        }

        rabbit_ptr = rabbit_ptr->next;
    }

    // list having problem, constructor
    rabbit_ptr = new RabbitManager;
    rabbit_ptr->rabbit = new TNodeRabbit;
    rabbit_ptr->id = id;
    rabbit_ptr->next = rabbit_list_;

    rabbit_list_ = rabbit_ptr;
    rabbit_instance = rabbit_ptr->rabbit;

    mutex_.unlock();

    return rabbit_instance;
}

void TNodeRabbitManager::FreeRabbit(uint32 id){
    RabbitManager*  rabbit_ptr;
    RabbitManager*  rabbit_pre_ptr;

    mutex_.lock();
    rabbit_ptr = rabbit_list_;

    if (nullptr == rabbit_ptr){
        mutex_.unlock();
        return;
    }

    if (id == rabbit_ptr->id){
        rabbit_list_ = rabbit_ptr->next;
        delete rabbit_ptr->rabbit;
        delete rabbit_ptr;
        mutex_.unlock();
        return;
    }

    rabbit_pre_ptr = rabbit_ptr;
    rabbit_ptr = rabbit_ptr->next;

    while (nullptr != rabbit_ptr){

        if (id == rabbit_ptr->id){
            rabbit_pre_ptr->next = rabbit_ptr->next;
            delete rabbit_ptr->rabbit;
            delete rabbit_ptr;
            mutex_.unlock();
            return;
        }

        rabbit_pre_ptr = rabbit_ptr;
        rabbit_ptr = rabbit_ptr->next;
    }

    mutex_.unlock();
}

bool TNodeRabbitManager::Check(uint32 id, TNodeRabbit* rabbit){
    RabbitManager*  rabbit_ptr;

    mutex_.lock();
    rabbit_ptr = rabbit_list_;
    while (nullptr != rabbit_ptr){

        if (id == rabbit_ptr->id && rabbit == rabbit_ptr->rabbit){
            mutex_.unlock();
            return  true;
        }

        rabbit_ptr = rabbit_ptr->next;
    }

    mutex_.unlock();

    return false;
}