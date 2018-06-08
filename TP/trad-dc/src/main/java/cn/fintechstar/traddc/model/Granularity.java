/* Generated SBE (Simple Binary Encoding) message codec */
package cn.fintechstar.traddc.model;

@javax.annotation.Generated(value = {"datacenter.Granularity"})
public enum Granularity
{
    S5((short)0),
    S10((short)1),
    S15((short)2),
    S30((short)3),
    M1((short)4),
    M5((short)5),
    M10((short)6),
    M15((short)7),
    M30((short)8),
    H1((short)9),
    H4((short)10),
    H12((short)11),
    D((short)12),
    W((short)13),
    M((short)14),
    NULL_VAL((short)255);

    private final short value;

    Granularity(final short value)
    {
        this.value = value;
    }

    public short value()
    {
        return value;
    }

    public static Granularity get(final short value)
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
            case 10: return H4;
            case 11: return H12;
            case 12: return D;
            case 13: return W;
            case 14: return M;
        }

        if ((short)255 == value)
        {
            return NULL_VAL;
        }

        throw new IllegalArgumentException("Unknown value: " + value);
    }
}
