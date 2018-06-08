/* Generated SBE (Simple Binary Encoding) message codec */
package com.broctagon.trad.reportserver.sbe;

import org.agrona.MutableDirectBuffer;
import org.agrona.DirectBuffer;

@javax.annotation.Generated(value = {"realtime.RealtimedatasDecoder"})
@SuppressWarnings("all")
public class RealtimedatasDecoder
{
    public static final int BLOCK_LENGTH = 0;
    public static final int TEMPLATE_ID = 1;
    public static final int SCHEMA_ID = 1;
    public static final int SCHEMA_VERSION = 0;

    private final RealtimedatasDecoder parentMessage = this;
    private DirectBuffer buffer;
    protected int offset;
    protected int limit;
    protected int actingBlockLength;
    protected int actingVersion;

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

    public DirectBuffer buffer()
    {
        return buffer;
    }

    public int offset()
    {
        return offset;
    }

    public RealtimedatasDecoder wrap(
        final DirectBuffer buffer, final int offset, final int actingBlockLength, final int actingVersion)
    {
        this.buffer = buffer;
        this.offset = offset;
        this.actingBlockLength = actingBlockLength;
        this.actingVersion = actingVersion;
        limit(offset + actingBlockLength);

        return this;
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

    private final RealtimeDataListDecoder realtimeDataList = new RealtimeDataListDecoder();

    public static long realtimeDataListDecoderId()
    {
        return 1;
    }

    public static int realtimeDataListDecoderSinceVersion()
    {
        return 0;
    }

    public RealtimeDataListDecoder realtimeDataList()
    {
        realtimeDataList.wrap(parentMessage, buffer);
        return realtimeDataList;
    }

    public static class RealtimeDataListDecoder
        implements Iterable<RealtimeDataListDecoder>, java.util.Iterator<RealtimeDataListDecoder>
    {
        private static final int HEADER_SIZE = 4;
        private final GroupSizeEncodingDecoder dimensions = new GroupSizeEncodingDecoder();
        private RealtimedatasDecoder parentMessage;
        private DirectBuffer buffer;
        private int count;
        private int index;
        private int offset;
        private int blockLength;

        public void wrap(
            final RealtimedatasDecoder parentMessage, final DirectBuffer buffer)
        {
            this.parentMessage = parentMessage;
            this.buffer = buffer;
            dimensions.wrap(buffer, parentMessage.limit());
            blockLength = dimensions.blockLength();
            count = dimensions.numInGroup();
            index = -1;
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

        public int actingBlockLength()
        {
            return blockLength;
        }

        public int count()
        {
            return count;
        }

        public java.util.Iterator<RealtimeDataListDecoder> iterator()
        {
            return this;
        }

        public void remove()
        {
            throw new UnsupportedOperationException();
        }

        public boolean hasNext()
        {
            return (index + 1) < count;
        }

        public RealtimeDataListDecoder next()
        {
            if (index + 1 >= count)
            {
                throw new java.util.NoSuchElementException();
            }

            offset = parentMessage.limit();
            parentMessage.limit(offset + blockLength);
            ++index;

            return this;
        }

        public static int statusId()
        {
            return 2;
        }

        public static int statusSinceVersion()
        {
            return 0;
        }

        public static int statusEncodingOffset()
        {
            return 0;
        }

        public static int statusEncodingLength()
        {
            return 4;
        }

        public static String statusMetaAttribute(final MetaAttribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case EPOCH: return "unix";
                case TIME_UNIT: return "nanosecond";
                case SEMANTIC_TYPE: return "";
            }

            return "";
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

        public long status()
        {
            return (buffer.getInt(offset + 0, java.nio.ByteOrder.BIG_ENDIAN) & 0xFFFF_FFFFL);
        }


        public static int bidId()
        {
            return 3;
        }

        public static int bidSinceVersion()
        {
            return 0;
        }

        public static int bidEncodingOffset()
        {
            return 4;
        }

        public static int bidEncodingLength()
        {
            return 4;
        }

        public static String bidMetaAttribute(final MetaAttribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case EPOCH: return "unix";
                case TIME_UNIT: return "nanosecond";
                case SEMANTIC_TYPE: return "";
            }

            return "";
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

        public int bid()
        {
            return buffer.getInt(offset + 4, java.nio.ByteOrder.BIG_ENDIAN);
        }


        public static int askId()
        {
            return 4;
        }

        public static int askSinceVersion()
        {
            return 0;
        }

        public static int askEncodingOffset()
        {
            return 8;
        }

        public static int askEncodingLength()
        {
            return 4;
        }

        public static String askMetaAttribute(final MetaAttribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case EPOCH: return "unix";
                case TIME_UNIT: return "nanosecond";
                case SEMANTIC_TYPE: return "";
            }

            return "";
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

        public int ask()
        {
            return buffer.getInt(offset + 8, java.nio.ByteOrder.BIG_ENDIAN);
        }


        public static int timestampId()
        {
            return 5;
        }

        public static int timestampSinceVersion()
        {
            return 0;
        }

        public static int timestampEncodingOffset()
        {
            return 12;
        }

        public static int timestampEncodingLength()
        {
            return 8;
        }

        public static String timestampMetaAttribute(final MetaAttribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case EPOCH: return "unix";
                case TIME_UNIT: return "nanosecond";
                case SEMANTIC_TYPE: return "";
            }

            return "";
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

        public long timestamp()
        {
            return buffer.getLong(offset + 12, java.nio.ByteOrder.BIG_ENDIAN);
        }


        public static int instrumentId()
        {
            return 6;
        }

        public static int instrumentSinceVersion()
        {
            return 0;
        }

        public static int instrumentEncodingOffset()
        {
            return 20;
        }

        public static int instrumentEncodingLength()
        {
            return 8;
        }

        public static String instrumentMetaAttribute(final MetaAttribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case EPOCH: return "unix";
                case TIME_UNIT: return "nanosecond";
                case SEMANTIC_TYPE: return "";
            }

            return "";
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

        public byte instrument(final int index)
        {
            if (index < 0 || index >= 8)
            {
                throw new IndexOutOfBoundsException("index out of range: index=" + index);
            }

            final int pos = this.offset + 20 + (index * 1);

            return buffer.getByte(pos);
        }


        public static String instrumentCharacterEncoding()
        {
            return "ASCII";
        }

        public int getInstrument(final byte[] dst, final int dstOffset)
        {
            final int length = 8;
            if (dstOffset < 0 || dstOffset > (dst.length - length))
            {
                throw new IndexOutOfBoundsException("Copy will go out of range: offset=" + dstOffset);
            }

            buffer.getBytes(this.offset + 20, dst, dstOffset, length);

            return length;
        }

        public String instrument()
        {
            final byte[] dst = new byte[8];
            buffer.getBytes(this.offset + 20, dst, 0, 8);

            int end = 0;
            for (; end < 8 && dst[end] != 0; ++end);

            return new String(dst, 0, end, java.nio.charset.StandardCharsets.US_ASCII);
        }



        public String toString()
        {
            return appendTo(new StringBuilder(100)).toString();
        }

        public StringBuilder appendTo(final StringBuilder builder)
        {
            builder.append('(');
            //Token{signal=BEGIN_FIELD, name='status', referencedName='null', description='null', id=2, version=0, deprecated=0, encodedLength=0, offset=0, componentTokenCount=3, encoding=Encoding{presence=REQUIRED, primitiveType=null, byteOrder=LITTLE_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='unix', timeUnit=nanosecond, semanticType='null'}}
            //Token{signal=ENCODING, name='uint32', referencedName='null', description='null', id=-1, version=0, deprecated=0, encodedLength=4, offset=0, componentTokenCount=1, encoding=Encoding{presence=REQUIRED, primitiveType=UINT32, byteOrder=BIG_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='unix', timeUnit=nanosecond, semanticType='null'}}
            builder.append("status=");
            builder.append(status());
            builder.append('|');
            //Token{signal=BEGIN_FIELD, name='bid', referencedName='null', description='null', id=3, version=0, deprecated=0, encodedLength=0, offset=4, componentTokenCount=3, encoding=Encoding{presence=REQUIRED, primitiveType=null, byteOrder=LITTLE_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='unix', timeUnit=nanosecond, semanticType='null'}}
            //Token{signal=ENCODING, name='int32', referencedName='null', description='null', id=-1, version=0, deprecated=0, encodedLength=4, offset=4, componentTokenCount=1, encoding=Encoding{presence=REQUIRED, primitiveType=INT32, byteOrder=BIG_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='unix', timeUnit=nanosecond, semanticType='null'}}
            builder.append("bid=");
            builder.append(bid());
            builder.append('|');
            //Token{signal=BEGIN_FIELD, name='ask', referencedName='null', description='null', id=4, version=0, deprecated=0, encodedLength=0, offset=8, componentTokenCount=3, encoding=Encoding{presence=REQUIRED, primitiveType=null, byteOrder=LITTLE_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='unix', timeUnit=nanosecond, semanticType='null'}}
            //Token{signal=ENCODING, name='int32', referencedName='null', description='null', id=-1, version=0, deprecated=0, encodedLength=4, offset=8, componentTokenCount=1, encoding=Encoding{presence=REQUIRED, primitiveType=INT32, byteOrder=BIG_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='unix', timeUnit=nanosecond, semanticType='null'}}
            builder.append("ask=");
            builder.append(ask());
            builder.append('|');
            //Token{signal=BEGIN_FIELD, name='timestamp', referencedName='null', description='null', id=5, version=0, deprecated=0, encodedLength=0, offset=12, componentTokenCount=3, encoding=Encoding{presence=REQUIRED, primitiveType=null, byteOrder=LITTLE_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='unix', timeUnit=nanosecond, semanticType='null'}}
            //Token{signal=ENCODING, name='uint64', referencedName='null', description='null', id=-1, version=0, deprecated=0, encodedLength=8, offset=12, componentTokenCount=1, encoding=Encoding{presence=REQUIRED, primitiveType=UINT64, byteOrder=BIG_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='unix', timeUnit=nanosecond, semanticType='null'}}
            builder.append("timestamp=");
            builder.append(timestamp());
            builder.append('|');
            //Token{signal=BEGIN_FIELD, name='instrument', referencedName='null', description='null', id=6, version=0, deprecated=0, encodedLength=0, offset=20, componentTokenCount=3, encoding=Encoding{presence=REQUIRED, primitiveType=null, byteOrder=LITTLE_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='unix', timeUnit=nanosecond, semanticType='null'}}
            //Token{signal=ENCODING, name='InstrumentType', referencedName='null', description='null', id=-1, version=0, deprecated=0, encodedLength=8, offset=20, componentTokenCount=1, encoding=Encoding{presence=REQUIRED, primitiveType=CHAR, byteOrder=BIG_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='ASCII', epoch='unix', timeUnit=nanosecond, semanticType='String'}}
            builder.append("instrument=");
            for (int i = 0; i < instrumentLength() && instrument(i) > 0; i++)
            {
                builder.append((char)instrument(i));
            }
            builder.append(')');
            return builder;
        }
    }


    public String toString()
    {
        return appendTo(new StringBuilder(100)).toString();
    }

    public StringBuilder appendTo(final StringBuilder builder)
    {
        final int originalLimit = limit();
        limit(offset + actingBlockLength);
        builder.append("[realtimedatas](sbeTemplateId=");
        builder.append(TEMPLATE_ID);
        builder.append("|sbeSchemaId=");
        builder.append(SCHEMA_ID);
        builder.append("|sbeSchemaVersion=");
        if (parentMessage.actingVersion != SCHEMA_VERSION)
        {
            builder.append(parentMessage.actingVersion);
            builder.append('/');
        }
        builder.append(SCHEMA_VERSION);
        builder.append("|sbeBlockLength=");
        if (actingBlockLength != BLOCK_LENGTH)
        {
            builder.append(actingBlockLength);
            builder.append('/');
        }
        builder.append(BLOCK_LENGTH);
        builder.append("):");
        //Token{signal=BEGIN_GROUP, name='RealtimeDataList', referencedName='null', description='null', id=1, version=0, deprecated=0, encodedLength=28, offset=0, componentTokenCount=21, encoding=Encoding{presence=REQUIRED, primitiveType=null, byteOrder=LITTLE_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='null', timeUnit=null, semanticType='null'}}
        builder.append("realtimeDataList=[");
        RealtimeDataListDecoder realtimeDataList = realtimeDataList();
        if (realtimeDataList.count() > 0)
        {
            while (realtimeDataList.hasNext())
            {
                realtimeDataList.next().appendTo(builder);
                builder.append(',');
            }
            builder.setLength(builder.length() - 1);
        }
        builder.append(']');

        limit(originalLimit);

        return builder;
    }
}
