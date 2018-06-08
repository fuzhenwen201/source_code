/* Generated SBE (Simple Binary Encoding) message codec */
package cn.fintechstar.traddc.sbe;

@javax.annotation.Generated(value = {"datacenter.BooleanType"})
public enum BooleanType
{
    T((short)0),
    F((short)1),
    NULL_VAL((short)255);

    private final short value;

    BooleanType(final short value)
    {
        this.value = value;
    }

    public short value()
    {
        return value;
    }

    public static BooleanType get(final short value)
    {
        switch (value)
        {
            case 0: return T;
            case 1: return F;
        }

        if ((short)255 == value)
        {
            return NULL_VAL;
        }

        throw new IllegalArgumentException("Unknown value: " + value);
    }
}
