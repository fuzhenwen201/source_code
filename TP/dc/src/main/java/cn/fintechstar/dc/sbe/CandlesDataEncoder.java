/* Generated SBE (Simple Binary Encoding) message codec */
package cn.fintechstar.dc.sbe;

import org.agrona.MutableDirectBuffer;
import org.agrona.DirectBuffer;

@javax.annotation.Generated(value = {"datacenter.CandlesDataEncoder"})
@SuppressWarnings("all")
public class CandlesDataEncoder
{
    public static final int BLOCK_LENGTH = 64;
    public static final int TEMPLATE_ID = 1;
    public static final int SCHEMA_ID = 1;
    public static final int SCHEMA_VERSION = 2;

    private final CandlesDataEncoder parentMessage = this;
    private MutableDirectBuffer buffer;
    protected int offset;
    protected int limit;

    public int sbeBlockLength()
    {
        return BLOCK_LENGTH;
    }

    public int sbeTemplateId()
    {
        return TEMPLATE_ID;
    }

    public int sbeSchemaId()
    {
        return SCHEMA_ID;
    }

    public int sbeSchemaVersion()
    {
        return SCHEMA_VERSION;
    }

    public String sbeSemanticType()
    {
        return "";
    }

    public MutableDirectBuffer buffer()
    {
        return buffer;
    }

    public int offset()
    {
        return offset;
    }

    public CandlesDataEncoder wrap(final MutableDirectBuffer buffer, final int offset)
    {
        this.buffer = buffer;
        this.offset = offset;
        limit(offset + BLOCK_LENGTH);

        return this;
    }

    public CandlesDataEncoder wrapAndApplyHeader(
        final MutableDirectBuffer buffer, final int offset, final MessageHeaderEncoder headerEncoder)
    {
        headerEncoder
            .wrap(buffer, offset)
            .blockLength(BLOCK_LENGTH)
            .templateId(TEMPLATE_ID)
            .schemaId(SCHEMA_ID)
            .version(SCHEMA_VERSION);

        return wrap(buffer, offset + MessageHeaderEncoder.ENCODED_LENGTH);
    }

    public int encodedLength()
    {
        return limit - offset;
    }

    public int limit()
    {
        return limit;
    }

    public void limit(final int limit)
    {
        this.limit = limit;
    }

    public static int instrumentEncodingOffset()
    {
        return 0;
    }

    public static int instrumentEncodingLength()
    {
        return 32;
    }

    public static byte instrumentNullValue()
    {
        return (byte)0;
    }

    public static byte instrumentMinValue()
    {
        return (byte)32;
    }

    public static byte instrumentMaxValue()
    {
        return (byte)126;
    }

    public static int instrumentLength()
    {
        return 32;
    }

    public void instrument(final int index, final byte value)
    {
        if (index < 0 || index >= 32)
        {
            throw new IndexOutOfBoundsException("index out of range: index=" + index);
        }

        final int pos = this.offset + 0 + (index * 1);
        buffer.putByte(pos, value);
    }

    public static String instrumentCharacterEncoding()
    {
        return "ASCII";
    }

    public CandlesDataEncoder putInstrument(final byte[] src, final int srcOffset)
    {
        final int length = 32;
        if (srcOffset < 0 || srcOffset > (src.length - length))
        {
            throw new IndexOutOfBoundsException("Copy will go out of range: offset=" + srcOffset);
        }

        buffer.putBytes(this.offset + 0, src, srcOffset, length);

        return this;
    }

    public CandlesDataEncoder instrument(final String src)
    {
        final int length = 32;
        final byte[] bytes = src.getBytes(java.nio.charset.StandardCharsets.US_ASCII);
        if (bytes.length > length)
        {
            throw new IndexOutOfBoundsException("String too large for copy: byte length=" + bytes.length);
        }

        buffer.putBytes(this.offset + 0, bytes, 0, bytes.length);

        for (int start = bytes.length; start < length; ++start)
        {
            buffer.putByte(this.offset + 0 + start, (byte)0);
        }

        return this;
    }

    public static int granularityEncodingOffset()
    {
        return 32;
    }

    public static int granularityEncodingLength()
    {
        return 32;
    }

    public static byte granularityNullValue()
    {
        return (byte)0;
    }

    public static byte granularityMinValue()
    {
        return (byte)32;
    }

    public static byte granularityMaxValue()
    {
        return (byte)126;
    }

    public static int granularityLength()
    {
        return 32;
    }

    public void granularity(final int index, final byte value)
    {
        if (index < 0 || index >= 32)
        {
            throw new IndexOutOfBoundsException("index out of range: index=" + index);
        }

        final int pos = this.offset + 32 + (index * 1);
        buffer.putByte(pos, value);
    }

    public static String granularityCharacterEncoding()
    {
        return "ASCII";
    }

    public CandlesDataEncoder putGranularity(final byte[] src, final int srcOffset)
    {
        final int length = 32;
        if (srcOffset < 0 || srcOffset > (src.length - length))
        {
            throw new IndexOutOfBoundsException("Copy will go out of range: offset=" + srcOffset);
        }

        buffer.putBytes(this.offset + 32, src, srcOffset, length);

        return this;
    }

    public CandlesDataEncoder granularity(final String src)
    {
        final int length = 32;
        final byte[] bytes = src.getBytes(java.nio.charset.StandardCharsets.US_ASCII);
        if (bytes.length > length)
        {
            throw new IndexOutOfBoundsException("String too large for copy: byte length=" + bytes.length);
        }

        buffer.putBytes(this.offset + 32, bytes, 0, bytes.length);

        for (int start = bytes.length; start < length; ++start)
        {
            buffer.putByte(this.offset + 32 + start, (byte)0);
        }

        return this;
    }

    private final CandlesEncoder candles = new CandlesEncoder();

    public static long candlesId()
    {
        return 3;
    }

    public CandlesEncoder candlesCount(final int count)
    {
        candles.wrap(parentMessage, buffer, count);
        return candles;
    }

    public static class CandlesEncoder
    {
        private static final int HEADER_SIZE = 4;
        private final GroupSizeEncodingEncoder dimensions = new GroupSizeEncodingEncoder();
        private CandlesDataEncoder parentMessage;
        private MutableDirectBuffer buffer;
        private int count;
        private int index;
        private int offset;

        public void wrap(
            final CandlesDataEncoder parentMessage, final MutableDirectBuffer buffer, final int count)
        {
            if (count < 0 || count > 65534)
            {
                throw new IllegalArgumentException("count outside allowed range: count=" + count);
            }

            this.parentMessage = parentMessage;
            this.buffer = buffer;
            dimensions.wrap(buffer, parentMessage.limit());
            dimensions.blockLength((int)69);
            dimensions.numInGroup((int)count);
            index = -1;
            this.count = count;
            parentMessage.limit(parentMessage.limit() + HEADER_SIZE);
        }

        public static int sbeHeaderSize()
        {
            return HEADER_SIZE;
        }

        public static int sbeBlockLength()
        {
            return 69;
        }

        public CandlesEncoder next()
        {
            if (index + 1 >= count)
            {
                throw new java.util.NoSuchElementException();
            }

            offset = parentMessage.limit();
            parentMessage.limit(offset + sbeBlockLength());
            ++index;

            return this;
        }

        public static int openBidEncodingOffset()
        {
            return 0;
        }

        public static int openBidEncodingLength()
        {
            return 4;
        }

        public static long openBidNullValue()
        {
            return 4294967294L;
        }

        public static long openBidMinValue()
        {
            return 0L;
        }

        public static long openBidMaxValue()
        {
            return 4294967293L;
        }

        public CandlesEncoder openBid(final long value)
        {
            buffer.putInt(offset + 0, (int)value, java.nio.ByteOrder.BIG_ENDIAN);
            return this;
        }


        public static int openAskEncodingOffset()
        {
            return 4;
        }

        public static int openAskEncodingLength()
        {
            return 4;
        }

        public static long openAskNullValue()
        {
            return 4294967294L;
        }

        public static long openAskMinValue()
        {
            return 0L;
        }

        public static long openAskMaxValue()
        {
            return 4294967293L;
        }

        public CandlesEncoder openAsk(final long value)
        {
            buffer.putInt(offset + 4, (int)value, java.nio.ByteOrder.BIG_ENDIAN);
            return this;
        }


        public static int highBidEncodingOffset()
        {
            return 8;
        }

        public static int highBidEncodingLength()
        {
            return 4;
        }

        public static long highBidNullValue()
        {
            return 4294967294L;
        }

        public static long highBidMinValue()
        {
            return 0L;
        }

        public static long highBidMaxValue()
        {
            return 4294967293L;
        }

        public CandlesEncoder highBid(final long value)
        {
            buffer.putInt(offset + 8, (int)value, java.nio.ByteOrder.BIG_ENDIAN);
            return this;
        }


        public static int highAskEncodingOffset()
        {
            return 12;
        }

        public static int highAskEncodingLength()
        {
            return 4;
        }

        public static long highAskNullValue()
        {
            return 4294967294L;
        }

        public static long highAskMinValue()
        {
            return 0L;
        }

        public static long highAskMaxValue()
        {
            return 4294967293L;
        }

        public CandlesEncoder highAsk(final long value)
        {
            buffer.putInt(offset + 12, (int)value, java.nio.ByteOrder.BIG_ENDIAN);
            return this;
        }


        public static int lowBidEncodingOffset()
        {
            return 16;
        }

        public static int lowBidEncodingLength()
        {
            return 4;
        }

        public static long lowBidNullValue()
        {
            return 4294967294L;
        }

        public static long lowBidMinValue()
        {
            return 0L;
        }

        public static long lowBidMaxValue()
        {
            return 4294967293L;
        }

        public CandlesEncoder lowBid(final long value)
        {
            buffer.putInt(offset + 16, (int)value, java.nio.ByteOrder.BIG_ENDIAN);
            return this;
        }


        public static int lowAskEncodingOffset()
        {
            return 20;
        }

        public static int lowAskEncodingLength()
        {
            return 4;
        }

        public static long lowAskNullValue()
        {
            return 4294967294L;
        }

        public static long lowAskMinValue()
        {
            return 0L;
        }

        public static long lowAskMaxValue()
        {
            return 4294967293L;
        }

        public CandlesEncoder lowAsk(final long value)
        {
            buffer.putInt(offset + 20, (int)value, java.nio.ByteOrder.BIG_ENDIAN);
            return this;
        }


        public static int closeBidEncodingOffset()
        {
            return 24;
        }

        public static int closeBidEncodingLength()
        {
            return 4;
        }

        public static long closeBidNullValue()
        {
            return 4294967294L;
        }

        public static long closeBidMinValue()
        {
            return 0L;
        }

        public static long closeBidMaxValue()
        {
            return 4294967293L;
        }

        public CandlesEncoder closeBid(final long value)
        {
            buffer.putInt(offset + 24, (int)value, java.nio.ByteOrder.BIG_ENDIAN);
            return this;
        }


        public static int closeAskEncodingOffset()
        {
            return 28;
        }

        public static int closeAskEncodingLength()
        {
            return 4;
        }

        public static long closeAskNullValue()
        {
            return 4294967294L;
        }

        public static long closeAskMinValue()
        {
            return 0L;
        }

        public static long closeAskMaxValue()
        {
            return 4294967293L;
        }

        public CandlesEncoder closeAsk(final long value)
        {
            buffer.putInt(offset + 28, (int)value, java.nio.ByteOrder.BIG_ENDIAN);
            return this;
        }


        public static int volumnEncodingOffset()
        {
            return 32;
        }

        public static int volumnEncodingLength()
        {
            return 4;
        }

        public static long volumnNullValue()
        {
            return 4294967294L;
        }

        public static long volumnMinValue()
        {
            return 0L;
        }

        public static long volumnMaxValue()
        {
            return 4294967293L;
        }

        public CandlesEncoder volumn(final long value)
        {
            buffer.putInt(offset + 32, (int)value, java.nio.ByteOrder.BIG_ENDIAN);
            return this;
        }


        public static int completeEncodingOffset()
        {
            return 36;
        }

        public static int completeEncodingLength()
        {
            return 1;
        }

        public CandlesEncoder complete(final BooleanType value)
        {
            buffer.putByte(offset + 36, (byte)value.value());
            return this;
        }

        public static int timeEncodingOffset()
        {
            return 37;
        }

        public static int timeEncodingLength()
        {
            return 32;
        }

        public static byte timeNullValue()
        {
            return (byte)0;
        }

        public static byte timeMinValue()
        {
            return (byte)32;
        }

        public static byte timeMaxValue()
        {
            return (byte)126;
        }

        public static int timeLength()
        {
            return 32;
        }

        public void time(final int index, final byte value)
        {
            if (index < 0 || index >= 32)
            {
                throw new IndexOutOfBoundsException("index out of range: index=" + index);
            }

            final int pos = this.offset + 37 + (index * 1);
            buffer.putByte(pos, value);
        }

        public static String timeCharacterEncoding()
        {
            return "ASCII";
        }

        public CandlesEncoder putTime(final byte[] src, final int srcOffset)
        {
            final int length = 32;
            if (srcOffset < 0 || srcOffset > (src.length - length))
            {
                throw new IndexOutOfBoundsException("Copy will go out of range: offset=" + srcOffset);
            }

            buffer.putBytes(this.offset + 37, src, srcOffset, length);

            return this;
        }

        public CandlesEncoder time(final String src)
        {
            final int length = 32;
            final byte[] bytes = src.getBytes(java.nio.charset.StandardCharsets.US_ASCII);
            if (bytes.length > length)
            {
                throw new IndexOutOfBoundsException("String too large for copy: byte length=" + bytes.length);
            }

            buffer.putBytes(this.offset + 37, bytes, 0, bytes.length);

            for (int start = bytes.length; start < length; ++start)
            {
                buffer.putByte(this.offset + 37 + start, (byte)0);
            }

            return this;
        }
    }


    public String toString()
    {
        return appendTo(new StringBuilder(100)).toString();
    }

    public StringBuilder appendTo(final StringBuilder builder)
    {
        CandlesDataDecoder writer = new CandlesDataDecoder();
        writer.wrap(buffer, offset, BLOCK_LENGTH, SCHEMA_VERSION);

        return writer.appendTo(builder);
    }
}
