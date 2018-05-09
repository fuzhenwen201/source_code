/*************************************************************************
    > File Name: guuid.cpp
    > Author: Kevin
    > Mail: kevin.fu@fintechstar.cn 
    > Created Time: Fri 24 Nov 2017 11:51:42 AM CST
 ************************************************************************/

#include "guuid.h"
#include <sys/time.h>

namespace utils{
    uint64_t get_time(){
        struct timeval tv;
        gettimeofday(&tv, NULL);
        uint64_t time = tv.tv_usec;
        time /= 1000;
        time += (uint64_t)(tv.tv_sec * 1000);
        return time;
    }

    unique_id_t::unique_id_t(){
        epoch_ = 0;
        machine_ = 0;
        sequence_ = 0;
    }

    unique_id_t::~unique_id_t(){

    }

    void unique_id_t::set_epoch(uint64_t epoch){
        epoch_ = epoch;
    }

    void unique_id_t::set_machine(int32_t machine){
        machine_ = machine;
    }

    int64_t unique_id_t::generate(){
        int64_t value = 0;
        uint64_t time = get_time() - epoch_;

        // 保留后41位时间
        value = time << 22;

        // 中间10位是机器ID
        value |= (machine_ & 0x3FF) << 12;

        // 最后12位是sequenceID
        value |= sequence_++ & 0xFFF;
        if (sequence_ == 0x1000)
        {
            sequence_ = 0;
        }

        return value;
    }
}

