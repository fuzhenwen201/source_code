/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _HISTORYDATA_GRANULARITY_H_
#define _HISTORYDATA_GRANULARITY_H_

#if defined(SBE_HAVE_CMATH)
/* cmath needed for std::numeric_limits<double>::quiet_NaN() */
#  include <cmath>
#  define SBE_FLOAT_NAN std::numeric_limits<float>::quiet_NaN()
#  define SBE_DOUBLE_NAN std::numeric_limits<double>::quiet_NaN()
#else
/* math.h needed for NAN */
#  include <math.h>
#  define SBE_FLOAT_NAN NAN
#  define SBE_DOUBLE_NAN NAN
#endif

#if __cplusplus >= 201103L
#  include <cstdint>
#  include <string>
#  include <cstring>
#endif

#if __cplusplus >= 201103L
#  define SBE_CONSTEXPR constexpr
#else
#  define SBE_CONSTEXPR
#endif

#include <sbe/sbe.h>

using namespace sbe;

namespace historydata {

class Granularity
{
public:

    enum Value 
    {
        S5 = (std::uint8_t)0,
        S10 = (std::uint8_t)1,
        S15 = (std::uint8_t)2,
        S30 = (std::uint8_t)3,
        M1 = (std::uint8_t)4,
        M5 = (std::uint8_t)5,
        M10 = (std::uint8_t)6,
        M15 = (std::uint8_t)7,
        M30 = (std::uint8_t)8,
        H1 = (std::uint8_t)9,
        H12 = (std::uint8_t)10,
        D = (std::uint8_t)11,
        W = (std::uint8_t)12,
        M = (std::uint8_t)13,
        NULL_VALUE = (std::uint8_t)255
    };

    static Granularity::Value get(const std::uint8_t value)
    {
        switch (value)
        {
            case 0: return S5;
            case 1: return S10;
            case 2: return S15;
            case 3: return S30;
            case 4: return M1;
            case 5: return M5;
            case 6: return M10;
            case 7: return M15;
            case 8: return M30;
            case 9: return H1;
            case 10: return H12;
            case 11: return D;
            case 12: return W;
            case 13: return M;
            case 255: return NULL_VALUE;
        }

        throw std::runtime_error("unknown value for enum Granularity [E103]");
    }
};
}
#endif
