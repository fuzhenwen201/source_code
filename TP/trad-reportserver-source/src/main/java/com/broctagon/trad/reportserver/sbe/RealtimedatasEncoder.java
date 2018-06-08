/* Generated SBE (Simple Binary Encoding) message codec */
package com.broctagon.trad.reportserver.sbe;

import org.agrona.MutableDirectBuffer;
import org.agrona.DirectBuffer;

@javax.annotation.Generated(value = {"realtime.RealtimedatasEncoder"})
@SuppressWarnings("all")
public class RealtimedatasEncoder
{
    public static final int BLOCK_LENGTH = 0;
    public static final int TEMPLATE_ID = 1;
    public static final int SCHEMA_ID = 1;
    public static final int SCHEMA_VERSION = 0;

    private final RealtimedatasEncoder parentMessage = this;
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

    public RealtimedatasEncoder wrap(final MutableDirectBuffer buffer, final int offset)
    {
        this.buffer = buffer;
        this.offset = offset;
        limit(offset + BLOCK_LENGTH);

        return this;
    }

    public RealtimedatasEncoder wrapAndApplyHeader(
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

    private final RealtimeDataListEncoder realtimeDataList = new RealtimeDataListEncoder();

    public static long realtimeDataListId()
    {
        return 1;
    }

    public RealtimeDataListEncoder realtimeDataListCount(final int count)
    {
        realtimeDataList.wrap(parentMessage, buffer, count);
        return realtimeDataList;
    }

    public static class RealtimeDataListEncoder
    {
        private static final int HEADER_SIZE = 4;
        private final GroupSizeEncodingEncoder dimensions = new GroupSizeEncodingEncoder();
        private RealtimedatasEncoder parentMessage;
        private MutableDirectBuffer buffer;
        private int count;
        private int index;
        private int offset;

        public void wrap(
            final RealtimedatasEncoder parentMessage, final MutableDirectBuffer buffer, final int count)
        {
            if (count < 0 || count > 65534)
            {
                throw new IllegalArgumentException("count outside allowed range: count=" + count);
            }

            this.parentMessage = parentMessage;
            this.buffer = buffer;
            dimensions.wrap(buffer, parentMessage.limit());
            dimensions.blockLength((int)28);
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
            return 28;
        }

        public RealtimeDataListEncoder next()
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

        public static int statusEncodingOffset()
        {
            return 0;
        }

        public static int statusEncodingLength()
        {
            return 4;
        }

        public static long statusNullValue()
        {
            return 4294967294L;
        }

        public static long statusMinValue()
        {
            return 0L;
        }

        public static long statusMaxValue()
        {
            return 4294967293L;
        }

        public RealtimeDataListEncoder status(final long value)
        {
            buffer.putInt(offset + 0, (int)value, java.nio.ByteOrder.BIG_ENDIAN);
            return this;
        }


        public static int bidEncodingOffset()
        {
            return 4;
        }

        public static int bidEncodingLength()
        {
            return 4;
        }

        public static int bidNullValue()
        {
            return -2147483648;
        }

        public static int bidMinValue()
        {
            return -2147483647;
        }

        public static int bidMaxValue()
        {
            return 2147483647;
        }

        public RealtimeDataListEncoder bid(final int value)
        {
            buffer.putInt(offset + 4, value, java.nio.ByteOrder.BIG_ENDIAN);
            return this;
        }


        public static int askEncodingOffset()
        {
            return 8;
        }

        public static int askEncodingLength()
        {
            return 4;
        }

        public static int askNullValue()
        {
            return -2147483648;
        }

        public static int askMinValue()
        {
            return -2147483647;
        }

        public static int askMaxValue()
        {
            return 2147483647;
        }

        public RealtimeDataListEncoder ask(final int value)
        {
            buffer.putInt(offset + 8, value, java.nio.ByteOrder.BIG_ENDIAN);
            return this;
        }


        public static int timestampEncodingOffset()
        {
            return 12;
        }

        public static int timestampEncodingLength()
        {
            return 8;
        }

        public static long timestampNullValue()
        {
            return 0xffffffffffffffffL;
        }

        public static long timestampMinValue()
        {
            return 0x0L;
        }

        public static long timestampMaxValue()
        {
            return 0xfffffffffffffffeL;
        }

        public RealtimeDataListEncoder timestamp(final long value)
        {
            buffer.putLong(offset + 12, value, java.nio.ByteOrder.BIG_ENDIAN);
            return this;
        }


        public static int instrumentEncodingOffset()
        {
            return 20;
        }

        public static int instrumentEncodingLength()
        {
            return 8;
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
            return 8;
        }

        public void instrument(final int index, final byte value)
        {
            if (index < 0 || index >= 8)
            {
                throw new IndexOutOfBoundsException("index out of range: index=" + index);
            }

            final int pos = this.offset + 20 + (index * 1);
            buffer.putByte(pos, value);
        }

        public static String instrumentCharacterEncoding()
        {
            return "ASCII";
        }

        public RealtimeDataListEncoder putInstrument(final byte[] src, final int srcOffset)
        {
            final int length = 8;
            if (srcOffset < 0 || srcOffset > (src.length - length))
            {
                throw new IndexOutOfBoundsException("Copy will go out of range: offset=" + srcOffset);
            }

            buffer.putBytes(this.offset + 20, src, srcOffset, length);

            return this;
        }

        public RealtimeDataListEncoder instrument(final String src)
        {
            final int length = 8;
            final byte[] bytes = src.getBytes(java.nio.charset.StandardCharsets.US_ASCII);
            if (bytes.length > length)
            {
                throw new IndexOutOfBoundsException("String too large for copy: byte length=" + bytes.length);
            }

            buffer.putBytes(this.offset + 20, bytes, 0, bytes.length);

            for (int start = bytes.length; start < length; ++start)
            {
                buffer.putByte(this.offset + 20 + start, (byte)0);
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
        RealtimedatasDecoder writer = new RealtimedatasDecoder();
        writer.wrap(buffer, offset, BLOCK_LENGTH, SCHEMA_VERSION);

        return writer.appendTo(builder);
    }
}
