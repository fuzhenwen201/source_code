/* Generated SBE (Simple Binary Encoding) message codec */
package cn.fintechstar.traddc.sbe;

import org.agrona.DirectBuffer;

import cn.fintechstar.traddc.model.Granularity;
import cn.fintechstar.traddc.model.Instrument;

@javax.annotation.Generated(value = {"datacenter.CandlesDataDecoder"})
@SuppressWarnings("all")
public class CandlesDataDecoder
{
    public static final int BLOCK_LENGTH = 2;
    public static final int TEMPLATE_ID = 1;
    public static final int SCHEMA_ID = 1;
    public static final int SCHEMA_VERSION = 1;

    private final CandlesDataDecoder parentMessage = this;
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

    public CandlesDataDecoder wrap(
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

    public static int instrumentId()
    {
        return 1;
    }

    public static int instrumentSinceVersion()
    {
        return 0;
    }

    public static int instrumentEncodingOffset()
    {
        return 0;
    }

    public static int instrumentEncodingLength()
    {
        return 1;
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

    public Instrument instrument()
    {
        return Instrument.get(((short)(buffer.getByte(offset + 0) & 0xFF)));
    }


    public static int granularityId()
    {
        return 2;
    }

    public static int granularitySinceVersion()
    {
        return 0;
    }

    public static int granularityEncodingOffset()
    {
        return 1;
    }

    public static int granularityEncodingLength()
    {
        return 1;
    }

    public static String granularityMetaAttribute(final MetaAttribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case EPOCH: return "unix";
            case TIME_UNIT: return "nanosecond";
            case SEMANTIC_TYPE: return "";
        }

        return "";
    }

    public Granularity granularity()
    {
        return Granularity.get(((short)(buffer.getByte(offset + 1) & 0xFF)));
    }


    private final CandlesDecoder candles = new CandlesDecoder();

    public static long candlesDecoderId()
    {
        return 3;
    }

    public static int candlesDecoderSinceVersion()
    {
        return 0;
    }

    public CandlesDecoder candles()
    {
        candles.wrap(parentMessage, buffer);
        return candles;
    }

    public static class CandlesDecoder
        implements Iterable<CandlesDecoder>, java.util.Iterator<CandlesDecoder>
    {
        private static final int HEADER_SIZE = 4;
        private final GroupSizeEncodingDecoder dimensions = new GroupSizeEncodingDecoder();
        private CandlesDataDecoder parentMessage;
        private DirectBuffer buffer;
        private int count;
        private int index;
        private int offset;
        private int blockLength;

        public void wrap(
            final CandlesDataDecoder parentMessage, final DirectBuffer buffer)
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
            return 69;
        }

        public int actingBlockLength()
        {
            return blockLength;
        }

        public int count()
        {
            return count;
        }

        public java.util.Iterator<CandlesDecoder> iterator()
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

        public CandlesDecoder next()
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

        public static int openBidId()
        {
            return 4;
        }

        public static int openBidSinceVersion()
        {
            return 0;
        }

        public static int openBidEncodingOffset()
        {
            return 0;
        }

        public static int openBidEncodingLength()
        {
            return 4;
        }

        public static String openBidMetaAttribute(final MetaAttribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case EPOCH: return "unix";
                case TIME_UNIT: return "nanosecond";
                case SEMANTIC_TYPE: return "";
            }

            return "";
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

        public long openBid()
        {
            return (buffer.getInt(offset + 0, java.nio.ByteOrder.BIG_ENDIAN) & 0xFFFF_FFFFL);
        }


        public static int openAskId()
        {
            return 5;
        }

        public static int openAskSinceVersion()
        {
            return 0;
        }

        public static int openAskEncodingOffset()
        {
            return 4;
        }

        public static int openAskEncodingLength()
        {
            return 4;
        }

        public static String openAskMetaAttribute(final MetaAttribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case EPOCH: return "unix";
                case TIME_UNIT: return "nanosecond";
                case SEMANTIC_TYPE: return "";
            }

            return "";
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

        public long openAsk()
        {
            return (buffer.getInt(offset + 4, java.nio.ByteOrder.BIG_ENDIAN) & 0xFFFF_FFFFL);
        }


        public static int highBidId()
        {
            return 6;
        }

        public static int highBidSinceVersion()
        {
            return 0;
        }

        public static int highBidEncodingOffset()
        {
            return 8;
        }

        public static int highBidEncodingLength()
        {
            return 4;
        }

        public static String highBidMetaAttribute(final MetaAttribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case EPOCH: return "unix";
                case TIME_UNIT: return "nanosecond";
                case SEMANTIC_TYPE: return "";
            }

            return "";
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

        public long highBid()
        {
            return (buffer.getInt(offset + 8, java.nio.ByteOrder.BIG_ENDIAN) & 0xFFFF_FFFFL);
        }


        public static int highAskId()
        {
            return 7;
        }

        public static int highAskSinceVersion()
        {
            return 0;
        }

        public static int highAskEncodingOffset()
        {
            return 12;
        }

        public static int highAskEncodingLength()
        {
            return 4;
        }

        public static String highAskMetaAttribute(final MetaAttribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case EPOCH: return "unix";
                case TIME_UNIT: return "nanosecond";
                case SEMANTIC_TYPE: return "";
            }

            return "";
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

        public long highAsk()
        {
            return (buffer.getInt(offset + 12, java.nio.ByteOrder.BIG_ENDIAN) & 0xFFFF_FFFFL);
        }


        public static int lowBidId()
        {
            return 8;
        }

        public static int lowBidSinceVersion()
        {
            return 0;
        }

        public static int lowBidEncodingOffset()
        {
            return 16;
        }

        public static int lowBidEncodingLength()
        {
            return 4;
        }

        public static String lowBidMetaAttribute(final MetaAttribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case EPOCH: return "unix";
                case TIME_UNIT: return "nanosecond";
                case SEMANTIC_TYPE: return "";
            }

            return "";
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

        public long lowBid()
        {
            return (buffer.getInt(offset + 16, java.nio.ByteOrder.BIG_ENDIAN) & 0xFFFF_FFFFL);
        }


        public static int lowAskId()
        {
            return 9;
        }

        public static int lowAskSinceVersion()
        {
            return 0;
        }

        public static int lowAskEncodingOffset()
        {
            return 20;
        }

        public static int lowAskEncodingLength()
        {
            return 4;
        }

        public static String lowAskMetaAttribute(final MetaAttribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case EPOCH: return "unix";
                case TIME_UNIT: return "nanosecond";
                case SEMANTIC_TYPE: return "";
            }

            return "";
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

        public long lowAsk()
        {
            return (buffer.getInt(offset + 20, java.nio.ByteOrder.BIG_ENDIAN) & 0xFFFF_FFFFL);
        }


        public static int closeBidId()
        {
            return 10;
        }

        public static int closeBidSinceVersion()
        {
            return 0;
        }

        public static int closeBidEncodingOffset()
        {
            return 24;
        }

        public static int closeBidEncodingLength()
        {
            return 4;
        }

        public static String closeBidMetaAttribute(final MetaAttribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case EPOCH: return "unix";
                case TIME_UNIT: return "nanosecond";
                case SEMANTIC_TYPE: return "";
            }

            return "";
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

        public long closeBid()
        {
            return (buffer.getInt(offset + 24, java.nio.ByteOrder.BIG_ENDIAN) & 0xFFFF_FFFFL);
        }


        public static int closeAskId()
        {
            return 11;
        }

        public static int closeAskSinceVersion()
        {
            return 0;
        }

        public static int closeAskEncodingOffset()
        {
            return 28;
        }

        public static int closeAskEncodingLength()
        {
            return 4;
        }

        public static String closeAskMetaAttribute(final MetaAttribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case EPOCH: return "unix";
                case TIME_UNIT: return "nanosecond";
                case SEMANTIC_TYPE: return "";
            }

            return "";
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

        public long closeAsk()
        {
            return (buffer.getInt(offset + 28, java.nio.ByteOrder.BIG_ENDIAN) & 0xFFFF_FFFFL);
        }


        public static int volumnId()
        {
            return 12;
        }

        public static int volumnSinceVersion()
        {
            return 0;
        }

        public static int volumnEncodingOffset()
        {
            return 32;
        }

        public static int volumnEncodingLength()
        {
            return 4;
        }

        public static String volumnMetaAttribute(final MetaAttribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case EPOCH: return "unix";
                case TIME_UNIT: return "nanosecond";
                case SEMANTIC_TYPE: return "";
            }

            return "";
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

        public long volumn()
        {
            return (buffer.getInt(offset + 32, java.nio.ByteOrder.BIG_ENDIAN) & 0xFFFF_FFFFL);
        }


        public static int completeId()
        {
            return 13;
        }

        public static int completeSinceVersion()
        {
            return 0;
        }

        public static int completeEncodingOffset()
        {
            return 36;
        }

        public static int completeEncodingLength()
        {
            return 1;
        }

        public static String completeMetaAttribute(final MetaAttribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case EPOCH: return "unix";
                case TIME_UNIT: return "nanosecond";
                case SEMANTIC_TYPE: return "";
            }

            return "";
        }

        public BooleanType complete()
        {
            return BooleanType.get(((short)(buffer.getByte(offset + 36) & 0xFF)));
        }


        public static int timeId()
        {
            return 14;
        }

        public static int timeSinceVersion()
        {
            return 0;
        }

        public static int timeEncodingOffset()
        {
            return 37;
        }

        public static int timeEncodingLength()
        {
            return 32;
        }

        public static String timeMetaAttribute(final MetaAttribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case EPOCH: return "unix";
                case TIME_UNIT: return "nanosecond";
                case SEMANTIC_TYPE: return "";
            }

            return "";
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

        public byte time(final int index)
        {
            if (index < 0 || index >= 32)
            {
                throw new IndexOutOfBoundsException("index out of range: index=" + index);
            }

            final int pos = this.offset + 37 + (index * 1);

            return buffer.getByte(pos);
        }


        public static String timeCharacterEncoding()
        {
            return "ASCII";
        }

        public int getTime(final byte[] dst, final int dstOffset)
        {
            final int length = 32;
            if (dstOffset < 0 || dstOffset > (dst.length - length))
            {
                throw new IndexOutOfBoundsException("Copy will go out of range: offset=" + dstOffset);
            }

            buffer.getBytes(this.offset + 37, dst, dstOffset, length);

            return length;
        }

        public String time()
        {
            final byte[] dst = new byte[32];
            buffer.getBytes(this.offset + 37, dst, 0, 32);

            int end = 0;
            for (; end < 32 && dst[end] != 0; ++end);

            return new String(dst, 0, end, java.nio.charset.StandardCharsets.US_ASCII);
        }



        public String toString()
        {
            return appendTo(new StringBuilder(100)).toString();
        }

        public StringBuilder appendTo(final StringBuilder builder)
        {
            builder.append('(');
            //Token{signal=BEGIN_FIELD, name='openBid', referencedName='null', description='null', id=4, version=0, deprecated=0, encodedLength=0, offset=0, componentTokenCount=3, encoding=Encoding{presence=REQUIRED, primitiveType=null, byteOrder=LITTLE_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='unix', timeUnit=nanosecond, semanticType='null'}}
            //Token{signal=ENCODING, name='Number', referencedName='null', description='null', id=-1, version=0, deprecated=0, encodedLength=4, offset=0, componentTokenCount=1, encoding=Encoding{presence=REQUIRED, primitiveType=UINT32, byteOrder=BIG_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='unix', timeUnit=nanosecond, semanticType='null'}}
            builder.append("openBid=");
            builder.append(openBid());
            builder.append('|');
            //Token{signal=BEGIN_FIELD, name='openAsk', referencedName='null', description='null', id=5, version=0, deprecated=0, encodedLength=0, offset=4, componentTokenCount=3, encoding=Encoding{presence=REQUIRED, primitiveType=null, byteOrder=LITTLE_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='unix', timeUnit=nanosecond, semanticType='null'}}
            //Token{signal=ENCODING, name='Number', referencedName='null', description='null', id=-1, version=0, deprecated=0, encodedLength=4, offset=4, componentTokenCount=1, encoding=Encoding{presence=REQUIRED, primitiveType=UINT32, byteOrder=BIG_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='unix', timeUnit=nanosecond, semanticType='null'}}
            builder.append("openAsk=");
            builder.append(openAsk());
            builder.append('|');
            //Token{signal=BEGIN_FIELD, name='highBid', referencedName='null', description='null', id=6, version=0, deprecated=0, encodedLength=0, offset=8, componentTokenCount=3, encoding=Encoding{presence=REQUIRED, primitiveType=null, byteOrder=LITTLE_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='unix', timeUnit=nanosecond, semanticType='null'}}
            //Token{signal=ENCODING, name='Number', referencedName='null', description='null', id=-1, version=0, deprecated=0, encodedLength=4, offset=8, componentTokenCount=1, encoding=Encoding{presence=REQUIRED, primitiveType=UINT32, byteOrder=BIG_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='unix', timeUnit=nanosecond, semanticType='null'}}
            builder.append("highBid=");
            builder.append(highBid());
            builder.append('|');
            //Token{signal=BEGIN_FIELD, name='highAsk', referencedName='null', description='null', id=7, version=0, deprecated=0, encodedLength=0, offset=12, componentTokenCount=3, encoding=Encoding{presence=REQUIRED, primitiveType=null, byteOrder=LITTLE_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='unix', timeUnit=nanosecond, semanticType='null'}}
            //Token{signal=ENCODING, name='Number', referencedName='null', description='null', id=-1, version=0, deprecated=0, encodedLength=4, offset=12, componentTokenCount=1, encoding=Encoding{presence=REQUIRED, primitiveType=UINT32, byteOrder=BIG_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='unix', timeUnit=nanosecond, semanticType='null'}}
            builder.append("highAsk=");
            builder.append(highAsk());
            builder.append('|');
            //Token{signal=BEGIN_FIELD, name='lowBid', referencedName='null', description='null', id=8, version=0, deprecated=0, encodedLength=0, offset=16, componentTokenCount=3, encoding=Encoding{presence=REQUIRED, primitiveType=null, byteOrder=LITTLE_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='unix', timeUnit=nanosecond, semanticType='null'}}
            //Token{signal=ENCODING, name='Number', referencedName='null', description='null', id=-1, version=0, deprecated=0, encodedLength=4, offset=16, componentTokenCount=1, encoding=Encoding{presence=REQUIRED, primitiveType=UINT32, byteOrder=BIG_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='unix', timeUnit=nanosecond, semanticType='null'}}
            builder.append("lowBid=");
            builder.append(lowBid());
            builder.append('|');
            //Token{signal=BEGIN_FIELD, name='lowAsk', referencedName='null', description='null', id=9, version=0, deprecated=0, encodedLength=0, offset=20, componentTokenCount=3, encoding=Encoding{presence=REQUIRED, primitiveType=null, byteOrder=LITTLE_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='unix', timeUnit=nanosecond, semanticType='null'}}
            //Token{signal=ENCODING, name='Number', referencedName='null', description='null', id=-1, version=0, deprecated=0, encodedLength=4, offset=20, componentTokenCount=1, encoding=Encoding{presence=REQUIRED, primitiveType=UINT32, byteOrder=BIG_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='unix', timeUnit=nanosecond, semanticType='null'}}
            builder.append("lowAsk=");
            builder.append(lowAsk());
            builder.append('|');
            //Token{signal=BEGIN_FIELD, name='closeBid', referencedName='null', description='null', id=10, version=0, deprecated=0, encodedLength=0, offset=24, componentTokenCount=3, encoding=Encoding{presence=REQUIRED, primitiveType=null, byteOrder=LITTLE_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='unix', timeUnit=nanosecond, semanticType='null'}}
            //Token{signal=ENCODING, name='Number', referencedName='null', description='null', id=-1, version=0, deprecated=0, encodedLength=4, offset=24, componentTokenCount=1, encoding=Encoding{presence=REQUIRED, primitiveType=UINT32, byteOrder=BIG_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='unix', timeUnit=nanosecond, semanticType='null'}}
            builder.append("closeBid=");
            builder.append(closeBid());
            builder.append('|');
            //Token{signal=BEGIN_FIELD, name='closeAsk', referencedName='null', description='null', id=11, version=0, deprecated=0, encodedLength=0, offset=28, componentTokenCount=3, encoding=Encoding{presence=REQUIRED, primitiveType=null, byteOrder=LITTLE_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='unix', timeUnit=nanosecond, semanticType='null'}}
            //Token{signal=ENCODING, name='Number', referencedName='null', description='null', id=-1, version=0, deprecated=0, encodedLength=4, offset=28, componentTokenCount=1, encoding=Encoding{presence=REQUIRED, primitiveType=UINT32, byteOrder=BIG_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='unix', timeUnit=nanosecond, semanticType='null'}}
            builder.append("closeAsk=");
            builder.append(closeAsk());
            builder.append('|');
            //Token{signal=BEGIN_FIELD, name='volumn', referencedName='null', description='null', id=12, version=0, deprecated=0, encodedLength=0, offset=32, componentTokenCount=3, encoding=Encoding{presence=REQUIRED, primitiveType=null, byteOrder=LITTLE_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='unix', timeUnit=nanosecond, semanticType='null'}}
            //Token{signal=ENCODING, name='Volumn', referencedName='null', description='null', id=-1, version=0, deprecated=0, encodedLength=4, offset=32, componentTokenCount=1, encoding=Encoding{presence=REQUIRED, primitiveType=UINT32, byteOrder=BIG_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='unix', timeUnit=nanosecond, semanticType='null'}}
            builder.append("volumn=");
            builder.append(volumn());
            builder.append('|');
            //Token{signal=BEGIN_FIELD, name='complete', referencedName='null', description='null', id=13, version=0, deprecated=0, encodedLength=0, offset=36, componentTokenCount=6, encoding=Encoding{presence=REQUIRED, primitiveType=null, byteOrder=LITTLE_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='unix', timeUnit=nanosecond, semanticType='null'}}
            //Token{signal=BEGIN_ENUM, name='BooleanType', referencedName='null', description='null', id=-1, version=0, deprecated=0, encodedLength=1, offset=36, componentTokenCount=4, encoding=Encoding{presence=REQUIRED, primitiveType=UINT8, byteOrder=BIG_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='null', timeUnit=null, semanticType='null'}}
            builder.append("complete=");
            builder.append(complete());
            builder.append('|');
            //Token{signal=BEGIN_FIELD, name='time', referencedName='null', description='null', id=14, version=0, deprecated=0, encodedLength=0, offset=37, componentTokenCount=3, encoding=Encoding{presence=REQUIRED, primitiveType=null, byteOrder=LITTLE_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='unix', timeUnit=nanosecond, semanticType='null'}}
            //Token{signal=ENCODING, name='varStringEncoding', referencedName='null', description='null', id=-1, version=0, deprecated=0, encodedLength=32, offset=37, componentTokenCount=1, encoding=Encoding{presence=REQUIRED, primitiveType=CHAR, byteOrder=BIG_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='ASCII', epoch='unix', timeUnit=nanosecond, semanticType='String'}}
            builder.append("time=");
            for (int i = 0; i < timeLength() && time(i) > 0; i++)
            {
                builder.append((char)time(i));
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
        builder.append("[CandlesData](sbeTemplateId=");
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
        //Token{signal=BEGIN_FIELD, name='instrument', referencedName='null', description='null', id=1, version=0, deprecated=0, encodedLength=0, offset=0, componentTokenCount=77, encoding=Encoding{presence=REQUIRED, primitiveType=null, byteOrder=LITTLE_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='unix', timeUnit=nanosecond, semanticType='null'}}
        //Token{signal=BEGIN_ENUM, name='Instrument', referencedName='null', description='null', id=-1, version=0, deprecated=0, encodedLength=1, offset=0, componentTokenCount=75, encoding=Encoding{presence=REQUIRED, primitiveType=UINT8, byteOrder=BIG_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='null', timeUnit=null, semanticType='null'}}
        builder.append("instrument=");
        builder.append(instrument());
        builder.append('|');
        //Token{signal=BEGIN_FIELD, name='granularity', referencedName='null', description='null', id=2, version=0, deprecated=0, encodedLength=0, offset=1, componentTokenCount=19, encoding=Encoding{presence=REQUIRED, primitiveType=null, byteOrder=LITTLE_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='unix', timeUnit=nanosecond, semanticType='null'}}
        //Token{signal=BEGIN_ENUM, name='Granularity', referencedName='null', description='null', id=-1, version=0, deprecated=0, encodedLength=1, offset=1, componentTokenCount=17, encoding=Encoding{presence=REQUIRED, primitiveType=UINT8, byteOrder=BIG_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='null', timeUnit=null, semanticType='null'}}
        builder.append("granularity=");
        builder.append(granularity());
        builder.append('|');
        //Token{signal=BEGIN_GROUP, name='candles', referencedName='null', description='null', id=3, version=0, deprecated=0, encodedLength=69, offset=2, componentTokenCount=42, encoding=Encoding{presence=REQUIRED, primitiveType=null, byteOrder=LITTLE_ENDIAN, minValue=null, maxValue=null, nullValue=null, constValue=null, characterEncoding='null', epoch='null', timeUnit=null, semanticType='null'}}
        builder.append("candles=[");
        CandlesDecoder candles = candles();
        if (candles.count() > 0)
        {
            while (candles.hasNext())
            {
                candles.next().appendTo(builder);
                builder.append(',');
            }
            builder.setLength(builder.length() - 1);
        }
        builder.append(']');

        limit(originalLimit);

        return builder;
    }
}
