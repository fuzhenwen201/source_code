/*************************************************************************
    > File Name: guuid.h
    > Author: Kevin
    > Mail: kevin.fu@fintechstar.cn 
    > Created Time: Fri 24 Nov 2017 11:42:09 AM CST
 ************************************************************************/
#ifndef _SNOWFLAK_GUUID_H_
#define _SNOWFLAK_GUUID_H_

#include <stdint.h>
#include <unistd.h>

namespace utils{
extern uint64_t get_time();
class unique_id_t{
    public:
        unique_id_t();
        ~unique_id_t();

        void set_epoch(uint64_t epoch);
        void set_machine(int32_t machine);
        int64_t generate();

    private:
        uint64_t epoch_;
        int32_t machine_;
        int32_t sequence_;
};
}

#endif
