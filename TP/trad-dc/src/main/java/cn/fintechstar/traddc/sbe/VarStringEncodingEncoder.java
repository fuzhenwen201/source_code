/* Generated SBE (Simple Binary Encoding) message codec */
package cn.fintechstar.traddc.sbe;

import org.agrona.MutableDirectBuffer;

@javax.annotation.Generated(value = {"datacenter.VarStringEncodingEncoder"})
@SuppressWarnings("all")
public class VarStringEncodingEncoder
{
    public static final int ENCODED_LENGTH = -1;
    private int offset;
    private MutableDirectBuffer buffer;

    public VarStringEncodingEncoder wrap(final MutableDirectBuffer buffer, final int offset)
    {
        this.buffer = buffer;
        this.offset = offset;

        return this;
    }

    public MutableDirectBuffer buffer()
    {
        return buffer;
    }

    public int offset()
    {
        return offset;
    }

    public int encodedLength()
    {
        return ENCODED_LENGTH;
    }

    public static int lengthEncodingOffset()
    {
        return 0;
    }

    public static int lengthEncodingLength()
    {
        return 4;
    }

    public static long lengthNullValue()
    {
        return 4294967294L;
    }

    public static long lengthMinValue()
    {
        return 0L;
    }

    public static long lengthMaxValue()
    {
        return 1073741824L;
    }

    public VarStringEncodingEncoder length(final long value)
    {
        buffer.putInt(offset + 0, (int)value, java.nio.ByteOrder.BIG_ENDIAN);
        return this;
    }


    public static int varDataEncodingOffset()
    {
        return 4;
    }

    public static int varDataEncodingLength()
    {
        return -1;
    }

    public static short varDataNullValue()
    {
        return (short)255;
    }

    public static short varDataMinValue()
    {
        return (short)0;
    }

    public static short varDataMaxValue()
    {
        return (short)254;
    }

    public String toString()
    {
        return appendTo(new StringBuilder(100)).toString();
    }

    public StringBuilder appendTo(final StringBuilder builder)
    {
        VarStringEncodingDecoder writer = new VarStringEncodingDecoder();
        writer.wrap(buffer, offset);

        return writer.appendTo(builder);
    }
}
