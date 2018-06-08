//
// Created by alex on 2/27/17.
//

#ifndef TRDINGENGINE_QUEUE_MANAGER_H
#define TRDINGENGINE_QUEUE_MANAGER_H

#include <cstdint>
#include <mutex>

namespace snetwork_tradingengine {

enum ErrNo {
    QUEUE_MANAGER_SUCCESS = 0,
    QUEUE_MANAGER_ERROR = 0xFFFFFFFF
};

enum QMState {
    QUEUE_MANAGER_STATE_FREE = 0,
    QUEUE_MANAGER_STATE_USED
};

typedef struct LinkStruct {
    LinkStruct *pre;
    LinkStruct *next;
    uint32_t state;
} Link;

class QueueManager {
public:
    QueueManager(){
        used_ = nullptr;
        free_ = nullptr;
        link_array_ = nullptr;
        length_ = 0;
    }

    ~QueueManager(){
        lock_.lock();

        used_ = nullptr;
        free_ = nullptr;

        if (nullptr != link_array_){
            delete []link_array_;
            link_array_ = nullptr;
        }

        lock_.unlock();
    }

    inline uint32_t Init(uint32_t length){
        if (0 == length){
            return QUEUE_MANAGER_ERROR;
        }

        link_array_ = new Link[length];

        for(uint32_t i = 0; i < length ; ++i){
            link_array_[i].pre = nullptr;
            link_array_[i].next = free_;
            link_array_[i].state = QUEUE_MANAGER_STATE_FREE;

            if (nullptr != free_){
                free_->pre = &link_array_[i];
            }

            free_ = &link_array_[i];
        }

        length_ = length;

        return QUEUE_MANAGER_SUCCESS;
    }

    inline uint32_t Alloc(){
        lock_.lock();

        if (nullptr == free_){
            lock_.unlock();
            return QUEUE_MANAGER_ERROR;
        }

        Link *tmp = free_;
        if (QUEUE_MANAGER_STATE_FREE != tmp->state){
            lock_.unlock();
            XERROR("\nalloc state exception\n");
            return QUEUE_MANAGER_ERROR;
        }

        uint32_t id = (uint32_t)(tmp - link_array_);

        free_ = free_->next;
        if (nullptr != free_){
            free_->pre = nullptr;
        }

        //not need tmp->pre = nullptr;
        tmp->next = used_;

        if (nullptr != used_){
            used_->pre = tmp;
        }
        used_ = tmp;
        used_->state = QUEUE_MANAGER_STATE_USED;

        lock_.unlock();

        return id;
    }

    inline void Free(uint32_t id){
        Link *curr;
        Link *pre;
        Link *next;

        lock_.lock();

        if (id >= length_){
            lock_.unlock();
            return;
        }

        curr = &link_array_[id];
        if (QUEUE_MANAGER_STATE_USED != curr->state){
            lock_.unlock();
            XERROR("\ninvalid free id\n");
            return;
        }

        if (used_ == curr){
            used_ = curr->next;
            if (nullptr != used_){
                used_->pre = nullptr;
            }
        } else {
            //curr->pre != nullptr is true
            pre = curr->pre;
            pre->next = curr->next;
            next = curr->next;
            if (nullptr != next){
                next->pre = pre;
            }
        }

        curr->pre = nullptr;
        curr->next = free_;
        curr->state = QUEUE_MANAGER_STATE_FREE;

        if (nullptr != free_){
            free_->pre = curr;
        }
        free_ = curr;

        lock_.unlock();
    }

private:
    Link *used_;
    Link *free_;
    std::mutex lock_;
    Link *link_array_;
    uint32_t length_;
};

}


#endif //TRDINGENGINE_QUEUE_MANAGER_H
