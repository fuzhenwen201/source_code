/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _HISTORYDATA_INSTRUMENT_H_
#define _HISTORYDATA_INSTRUMENT_H_

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

class Instrument
{
public:

    enum Value 
    {
        AUD_CAD = (std::uint8_t)0,
        AUD_CHF = (std::uint8_t)1,
        AUD_HKD = (std::uint8_t)2,
        AUD_JPY = (std::uint8_t)3,
        AUD_NZD = (std::uint8_t)4,
        AUD_SGD = (std::uint8_t)5,
        AUD_USD = (std::uint8_t)6,
        CAD_CHF = (std::uint8_t)7,
        CAD_HKD = (std::uint8_t)8,
        CAD_JPY = (std::uint8_t)9,
        CAD_SGD = (std::uint8_t)10,
        CHF_HKD = (std::uint8_t)11,
        CHF_JPY = (std::uint8_t)12,
        CHF_ZAR = (std::uint8_t)13,
        EUR_AUD = (std::uint8_t)14,
        EUR_CAD = (std::uint8_t)15,
        EUR_CHF = (std::uint8_t)16,
        EUR_CZK = (std::uint8_t)17,
        EUR_DKK = (std::uint8_t)18,
        EUR_GBP = (std::uint8_t)19,
        EUR_HKD = (std::uint8_t)20,
        EUR_HUF = (std::uint8_t)21,
        EUR_JPY = (std::uint8_t)22,
        EUR_NOK = (std::uint8_t)23,
        EUR_NZD = (std::uint8_t)24,
        EUR_PLN = (std::uint8_t)25,
        EUR_SEK = (std::uint8_t)26,
        EUR_SGD = (std::uint8_t)27,
        EUR_TRY = (std::uint8_t)28,
        EUR_USD = (std::uint8_t)29,
        EUR_ZAR = (std::uint8_t)30,
        GBP_AUD = (std::uint8_t)31,
        GBP_CAD = (std::uint8_t)32,
        GBP_CHF = (std::uint8_t)33,
        GBP_HKD = (std::uint8_t)34,
        GBP_JPY = (std::uint8_t)35,
        GBP_NZD = (std::uint8_t)36,
        GBP_PLN = (std::uint8_t)37,
        GBP_SGD = (std::uint8_t)38,
        GBP_USD = (std::uint8_t)39,
        GBP_ZAR = (std::uint8_t)40,
        HKD_JPY = (std::uint8_t)41,
        NZD_CAD = (std::uint8_t)42,
        NZD_CHF = (std::uint8_t)43,
        NZD_HKD = (std::uint8_t)44,
        NZD_JPY = (std::uint8_t)45,
        NZD_SGD = (std::uint8_t)46,
        NZD_USD = (std::uint8_t)47,
        SGD_CHF = (std::uint8_t)48,
        SGD_HKD = (std::uint8_t)49,
        SGD_JPY = (std::uint8_t)50,
        TRY_JPY = (std::uint8_t)51,
        USD_CAD = (std::uint8_t)52,
        USD_CHF = (std::uint8_t)53,
        USD_CNH = (std::uint8_t)54,
        USD_CZK = (std::uint8_t)55,
        USD_DKK = (std::uint8_t)56,
        USD_HKD = (std::uint8_t)57,
        USD_HUF = (std::uint8_t)58,
        USD_INR = (std::uint8_t)59,
        USD_JPY = (std::uint8_t)60,
        USD_MXN = (std::uint8_t)61,
        USD_NOK = (std::uint8_t)62,
        USD_PLN = (std::uint8_t)63,
        USD_SAR = (std::uint8_t)64,
        USD_SEK = (std::uint8_t)65,
        USD_SGD = (std::uint8_t)66,
        USD_THB = (std::uint8_t)67,
        USD_TRY = (std::uint8_t)68,
        USD_ZAR = (std::uint8_t)69,
        ZAR_JPY = (std::uint8_t)70,
        FINCOIN_USD = (std::uint8_t)71,
        SymbolBUTT = (std::uint8_t)72,
        NULL_VALUE = (std::uint8_t)255
    };

    static Instrument::Value get(const std::uint8_t value)
    {
        switch (value)
        {
            case 0: return AUD_CAD;
            case 1: return AUD_CHF;
            case 2: return AUD_HKD;
            case 3: return AUD_JPY;
            case 4: return AUD_NZD;
            case 5: return AUD_SGD;
            case 6: return AUD_USD;
            case 7: return CAD_CHF;
            case 8: return CAD_HKD;
            case 9: return CAD_JPY;
            case 10: return CAD_SGD;
            case 11: return CHF_HKD;
            case 12: return CHF_JPY;
            case 13: return CHF_ZAR;
            case 14: return EUR_AUD;
            case 15: return EUR_CAD;
            case 16: return EUR_CHF;
            case 17: return EUR_CZK;
            case 18: return EUR_DKK;
            case 19: return EUR_GBP;
            case 20: return EUR_HKD;
            case 21: return EUR_HUF;
            case 22: return EUR_JPY;
            case 23: return EUR_NOK;
            case 24: return EUR_NZD;
            case 25: return EUR_PLN;
            case 26: return EUR_SEK;
            case 27: return EUR_SGD;
            case 28: return EUR_TRY;
            case 29: return EUR_USD;
            case 30: return EUR_ZAR;
            case 31: return GBP_AUD;
            case 32: return GBP_CAD;
            case 33: return GBP_CHF;
            case 34: return GBP_HKD;
            case 35: return GBP_JPY;
            case 36: return GBP_NZD;
            case 37: return GBP_PLN;
            case 38: return GBP_SGD;
            case 39: return GBP_USD;
            case 40: return GBP_ZAR;
            case 41: return HKD_JPY;
            case 42: return NZD_CAD;
            case 43: return NZD_CHF;
            case 44: return NZD_HKD;
            case 45: return NZD_JPY;
            case 46: return NZD_SGD;
            case 47: return NZD_USD;
            case 48: return SGD_CHF;
            case 49: return SGD_HKD;
            case 50: return SGD_JPY;
            case 51: return TRY_JPY;
            case 52: return USD_CAD;
            case 53: return USD_CHF;
            case 54: return USD_CNH;
            case 55: return USD_CZK;
            case 56: return USD_DKK;
            case 57: return USD_HKD;
            case 58: return USD_HUF;
            case 59: return USD_INR;
            case 60: return USD_JPY;
            case 61: return USD_MXN;
            case 62: return USD_NOK;
            case 63: return USD_PLN;
            case 64: return USD_SAR;
            case 65: return USD_SEK;
            case 66: return USD_SGD;
            case 67: return USD_THB;
            case 68: return USD_TRY;
            case 69: return USD_ZAR;
            case 70: return ZAR_JPY;
            case 71: return FINCOIN_USD;
            case 72: return SymbolBUTT;
            case 255: return NULL_VALUE;
        }

        throw std::runtime_error("unknown value for enum Instrument [E103]");
    }
};
}
#endif
