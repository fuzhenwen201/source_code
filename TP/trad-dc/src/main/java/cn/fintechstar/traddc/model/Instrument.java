package cn.fintechstar.traddc.model;

public enum Instrument
{
    AUD_CAD((short)0),
    AUD_CHF((short)1),
    AUD_HKD((short)2),
    AUD_JPY((short)3),
    AUD_NZD((short)4),
    AUD_SGD((short)5),
    AUD_USD((short)6),
    CAD_CHF((short)7),
    CAD_HKD((short)8),
    CAD_JPY((short)9),
    CAD_SGD((short)10),
    CHF_HKD((short)11),
    CHF_JPY((short)12),
    CHF_ZAR((short)13),
    EUR_AUD((short)14),
    EUR_CAD((short)15),
    EUR_CHF((short)16),
    EUR_CZK((short)17),
    EUR_DKK((short)18),
    EUR_GBP((short)19),
    EUR_HKD((short)20),
    EUR_HUF((short)21),
    EUR_JPY((short)22),
    EUR_NOK((short)23),
    EUR_NZD((short)24),
    EUR_PLN((short)25),
    EUR_SEK((short)26),
    EUR_SGD((short)27),
    EUR_TRY((short)28),
    EUR_USD((short)29),
    EUR_ZAR((short)30),
    GBP_AUD((short)31),
    GBP_CAD((short)32),
    GBP_CHF((short)33),
    GBP_HKD((short)34),
    GBP_JPY((short)35),
    GBP_NZD((short)36),
    GBP_PLN((short)37),
    GBP_SGD((short)38),
    GBP_USD((short)39),
    GBP_ZAR((short)40),
    HKD_JPY((short)41),
    NZD_CAD((short)42),
    NZD_CHF((short)43),
    NZD_HKD((short)44),
    NZD_JPY((short)45),
    NZD_SGD((short)46),
    NZD_USD((short)47),
    SGD_CHF((short)48),
    SGD_HKD((short)49),
    SGD_JPY((short)50),
    TRY_JPY((short)51),
    USD_CAD((short)52),
    USD_CHF((short)53),
    USD_CNH((short)54),
    USD_CZK((short)55),
    USD_DKK((short)56),
    USD_HKD((short)57),
    USD_HUF((short)58),
    USD_INR((short)59),
    USD_JPY((short)60),
    USD_MXN((short)61),
    USD_NOK((short)62),
    USD_PLN((short)63),
    USD_SAR((short)64),
    USD_SEK((short)65),
    USD_SGD((short)66),
    USD_THB((short)67),
    USD_TRY((short)68),
    USD_ZAR((short)69),
    ZAR_JPY((short)70),
    FINCOIN_USD((short)71),
    SymbolBUTT((short)72),
    NULL_VAL((short)255);

    private final short value;

    Instrument(final short value)
    {
        this.value = value;
    }

    public short value()
    {
        return value;
    }

    public static Instrument get(final short value)
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
        }

        if ((short)255 == value)
        {
            return NULL_VAL;
        }

        throw new IllegalArgumentException("Unknown value: " + value);
    }
}
