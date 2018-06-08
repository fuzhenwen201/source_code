//
// Created by alex on 12/15/16.
//

#ifndef SNETWORK_TYPEDEF_H
#define SNETWORK_TYPEDEF_H

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__linux__)
#include <stdint-gcc.h>
#endif

namespace snetwork_xservice_tnode
{
    typedef uint64_t u64int;
    typedef int64_t s64int;
    typedef uint8_t u8int;
    typedef int8_t s8int;
    typedef uint16_t u16int;
    typedef int16_t s16int;
    typedef uintptr_t uintptr;
    typedef intptr_t intptr;
    typedef uint32_t u32int;
    typedef int32_t s32int;

    typedef s8int int8;
    typedef u8int uint8;
    typedef s16int int16;
    typedef u16int uint16;
    typedef s32int int32;
    typedef u32int uint32;
    typedef s64int int64;
    typedef u64int uint64;

    typedef float float32;
    typedef double float64;

}

#if defined(__cplusplus)
}
#endif

#endif //SNETWORK_TYPEDEF_H

