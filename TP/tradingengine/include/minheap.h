//
// Created by alex on 2/27/17.
//

#ifndef TRDINGENGINE_MINHEAP_H
#define TRDINGENGINE_MINHEAP_H

#include <assert.h>
#include <mutex>
#include "tradingengine.h"

namespace snetwork_tradingengine {

class MinHeap {
public:
    MinHeap(int32_t cap) : capacity_(cap), cur_size_(0) {

        array_ = new OrderAns *[capacity_];

        for (int32_t i = 0; i < capacity_; ++i) {
            array_[i] = nullptr;
        }
    }

    ~MinHeap() {
        assert(0 == cur_size_);
        delete[] array_;
    }

public:
    inline void PushOrder(OrderAns *order) {
        if (!order) {
            return;
        }

        mutex_.lock();

        if (cur_size_ >= capacity_) {
            resize();
        }

        int32_t hole = cur_size_++;
        int32_t parent = 0;

        for (; hole > 0; hole = parent) {
            parent = (hole - 1) / 2;
            if (array_[parent]->closetime <= order->closetime) {
                break;
            }
            array_[hole] = array_[parent];
        }
        array_[hole] = order;

        mutex_.unlock();
    }


    inline OrderAns *top() {
        mutex_.lock();

        if (empty()) {
            mutex_.unlock();
            return nullptr;
        }

        OrderAns * tmp = array_[0];
        mutex_.unlock();

        return tmp;
    }

    inline OrderAns * PopOrder() {
        mutex_.lock();

        if (empty()) {
            mutex_.unlock();
            return nullptr;
        }

        OrderAns * tmp = array_[0];
        if (array_[0]) {
            array_[0] = array_[--cur_size_];
            array_[cur_size_] = nullptr;
            percolate_down(0);
        }

        mutex_.unlock();

        return tmp;
    }

    inline bool empty() const { return cur_size_ == 0; }

private:
    inline void percolate_down(int32_t hole) {
        OrderAns *temp = array_[hole];
        int32_t child = 0;
        for (; ((hole * 2 + 1) <= (cur_size_ - 1)); hole = child) {
            child = hole * 2 + 1;
            if ((child < (cur_size_ - 1)) && (array_[child + 1]->closetime < array_[child]->closetime)) {
                ++child;
            }
            if (array_[child]->closetime < temp->closetime) {
                array_[hole] = array_[child];
            }
            else {
                break;
            }
        }
        array_[hole] = temp;
    }

    inline void resize() {
        OrderAns **temp = new OrderAns *[2 * capacity_];
        for (int32_t i = 0; i < 2 * capacity_; ++i) {
            temp[i] = NULL;
        }

        capacity_ = 2 * capacity_;
        for (int32_t i = 0; i < cur_size_; ++i) {
            temp[i] = array_[i];
        }
        delete[] array_;
        array_ = temp;
    }

private:
    OrderAns **array_;
    int32_t capacity_; //堆数组的容量
    int32_t cur_size_; //堆数组当前包含元素的个数

    std::mutex mutex_;
};

}

#endif //TRDINGENGINE_MINHEAP_H
