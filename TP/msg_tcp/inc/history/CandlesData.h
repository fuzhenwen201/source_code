/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _HISTORYDATA_CANDLESDATA_H_
#define _HISTORYDATA_CANDLESDATA_H_

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

#include "MessageHeader.h"
#include "Instrument.h"
#include "BooleanType.h"
#include "Granularity.h"
#include "GroupSizeEncoding.h"

using namespace sbe;

namespace historydata {

class CandlesData
{
private:
    char *m_buffer;
    std::uint64_t m_bufferLength;
    std::uint64_t *m_positionPtr;
    std::uint64_t m_offset;
    std::uint64_t m_position;
    std::uint64_t m_actingBlockLength;
    std::uint64_t m_actingVersion;

    inline void reset(
        char *buffer, const std::uint64_t offset, const std::uint64_t bufferLength,
        const std::uint64_t actingBlockLength, const std::uint64_t actingVersion)
    {
        m_buffer = buffer;
        m_offset = offset;
        m_bufferLength = bufferLength;
        m_actingBlockLength = actingBlockLength;
        m_actingVersion = actingVersion;
        m_positionPtr = &m_position;
        position(offset + m_actingBlockLength);
    }

    inline void reset(const CandlesData& codec)
    {
        m_buffer = codec.m_buffer;
        m_offset = codec.m_offset;
        m_bufferLength = codec.m_bufferLength;
        m_actingBlockLength = codec.m_actingBlockLength;
        m_actingVersion = codec.m_actingVersion;
        m_positionPtr = &m_position;
        m_position = codec.m_position;
    }

public:

    CandlesData() : m_buffer(nullptr), m_bufferLength(0), m_offset(0) {}

    CandlesData(char *buffer, const std::uint64_t bufferLength)
    {
        reset(buffer, 0, bufferLength, sbeBlockLength(), sbeSchemaVersion());
    }

    CandlesData(char *buffer, const std::uint64_t bufferLength, const std::uint64_t actingBlockLength, const std::uint64_t actingVersion)
    {
        reset(buffer, 0, bufferLength, actingBlockLength, actingVersion);
    }

    CandlesData(const CandlesData& codec)
    {
        reset(codec);
    }

#if __cplusplus >= 201103L
    CandlesData(CandlesData&& codec)
    {
        reset(codec);
    }

    CandlesData& operator=(CandlesData&& codec)
    {
        reset(codec);
        return *this;
    }

#endif

    CandlesData& operator=(const CandlesData& codec)
    {
        reset(codec);
        return *this;
    }

    static SBE_CONSTEXPR std::uint16_t sbeBlockLength()
    {
        return (std::uint16_t)2;
    }

    static SBE_CONSTEXPR std::uint16_t sbeTemplateId()
    {
        return (std::uint16_t)1;
    }

    static SBE_CONSTEXPR std::uint16_t sbeSchemaId()
    {
        return (std::uint16_t)1;
    }

    static SBE_CONSTEXPR std::uint16_t sbeSchemaVersion()
    {
        return (std::uint16_t)0;
    }

    static SBE_CONSTEXPR const char * sbeSemanticType()
    {
        return "";
    }

    std::uint64_t offset() const
    {
        return m_offset;
    }

    CandlesData &wrapForEncode(char *buffer, const std::uint64_t offset, const std::uint64_t bufferLength)
    {
        reset(buffer, offset, bufferLength, sbeBlockLength(), sbeSchemaVersion());
        return *this;
    }

    CandlesData &wrapForDecode(
         char *buffer, const std::uint64_t offset, const std::uint64_t actingBlockLength,
         const std::uint64_t actingVersion, const std::uint64_t bufferLength)
    {
        reset(buffer, offset, bufferLength, actingBlockLength, actingVersion);
        return *this;
    }

    std::uint64_t position() const
    {
        return m_position;
    }

    void position(const std::uint64_t position)
    {
        if (SBE_BOUNDS_CHECK_EXPECT((position > m_bufferLength), false))
        {
            throw std::runtime_error("buffer too short [E100]");
        }
        m_position = position;
    }

    std::uint64_t encodedLength() const
    {
        return position() - m_offset;
    }

    char *buffer()
    {
        return m_buffer;
    }

    std::uint64_t actingVersion() const
    {
        return m_actingVersion;
    }

    static SBE_CONSTEXPR std::uint16_t instrumentId()
    {
        return 1;
    }

    static SBE_CONSTEXPR std::uint64_t instrumentSinceVersion()
    {
         return 0;
    }

    bool instrumentInActingVersion()
    {
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-compare"
#endif
        return m_actingVersion >= instrumentSinceVersion();
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
    }

    static SBE_CONSTEXPR std::size_t instrumentEncodingOffset()
    {
         return 0;
    }


    static const char *instrumentMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    Instrument::Value instrument() const
    {
        return Instrument::get((*((std::uint8_t *)(m_buffer + m_offset + 0))));
    }

    CandlesData &instrument(const Instrument::Value value)
    {
        *((std::uint8_t *)(m_buffer + m_offset + 0)) = (value);
        return *this;
    }
    static SBE_CONSTEXPR std::size_t instrumentEncodingLength()
    {
        return 1;
    }

    static SBE_CONSTEXPR std::uint16_t granularityId()
    {
        return 2;
    }

    static SBE_CONSTEXPR std::uint64_t granularitySinceVersion()
    {
         return 0;
    }

    bool granularityInActingVersion()
    {
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-compare"
#endif
        return m_actingVersion >= granularitySinceVersion();
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
    }

    static SBE_CONSTEXPR std::size_t granularityEncodingOffset()
    {
         return 1;
    }


    static const char *granularityMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    Granularity::Value granularity() const
    {
        return Granularity::get((*((std::uint8_t *)(m_buffer + m_offset + 1))));
    }

    CandlesData &granularity(const Granularity::Value value)
    {
        *((std::uint8_t *)(m_buffer + m_offset + 1)) = (value);
        return *this;
    }
    static SBE_CONSTEXPR std::size_t granularityEncodingLength()
    {
        return 1;
    }

    class Candles
    {
    private:
        char *m_buffer;
        std::uint64_t m_bufferLength;
        std::uint64_t *m_positionPtr;
        std::uint64_t m_blockLength;
        std::uint64_t m_count;
        std::uint64_t m_index;
        std::uint64_t m_offset;
        std::uint64_t m_actingVersion;
        GroupSizeEncoding m_dimensions;

    public:

        inline void wrapForDecode(char *buffer, std::uint64_t *pos, const std::uint64_t actingVersion, const std::uint64_t bufferLength)
        {
            m_buffer = buffer;
            m_bufferLength = bufferLength;
            m_dimensions.wrap(m_buffer, *pos, actingVersion, bufferLength);
            m_blockLength = m_dimensions.blockLength();
            m_count = m_dimensions.numInGroup();
            m_index = -1;
            m_actingVersion = actingVersion;
            m_positionPtr = pos;
            *m_positionPtr = *m_positionPtr + 4;
        }

        inline void wrapForEncode(char *buffer, const std::uint16_t count, std::uint64_t *pos, const std::uint64_t actingVersion, const std::uint64_t bufferLength)
        {
    #if defined(__GNUG__) && !defined(__clang__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wtype-limits"
    #endif
            if (count < 0 || count > 65534)
            {
                throw std::runtime_error("count outside of allowed range [E110]");
            }
    #if defined(__GNUG__) && !defined(__clang__)
    #pragma GCC diagnostic pop
    #endif
            m_buffer = buffer;
            m_bufferLength = bufferLength;
            m_dimensions.wrap(m_buffer, *pos, actingVersion, bufferLength);
            m_dimensions.blockLength((std::uint16_t)69);
            m_dimensions.numInGroup((std::uint16_t)count);
            m_index = -1;
            m_count = count;
            m_blockLength = 69;
            m_actingVersion = actingVersion;
            m_positionPtr = pos;
            *m_positionPtr = *m_positionPtr + 4;
        }

        static SBE_CONSTEXPR std::uint64_t sbeHeaderSize()
        {
            return 4;
        }

        static SBE_CONSTEXPR std::uint64_t sbeBlockLength()
        {
            return 69;
        }

        std::uint64_t position() const
        {
            return *m_positionPtr;
        }

        void position(const std::uint64_t position)
        {
            if (SBE_BOUNDS_CHECK_EXPECT((position > m_bufferLength), false))
            {
                 throw std::runtime_error("buffer too short [E100]");
            }
            *m_positionPtr = position;
        }

        inline std::uint64_t count() const
        {
            return m_count;
        }

        inline bool hasNext() const
        {
            return m_index + 1 < m_count;
        }

        inline Candles &next()
        {
            m_offset = *m_positionPtr;
            if (SBE_BOUNDS_CHECK_EXPECT(( (m_offset + m_blockLength) > m_bufferLength ), false))
            {
                throw std::runtime_error("buffer too short to support next group index [E108]");
            }
            *m_positionPtr = m_offset + m_blockLength;
            ++m_index;

            return *this;
        }

    #if __cplusplus < 201103L
        template<class Func> inline void forEach(Func& func)
        {
            while (hasNext())
            {
                next(); func(*this);
            }
        }

    #else
        template<class Func> inline void forEach(Func&& func)
        {
            while (hasNext())
            {
                next(); func(*this);
            }
        }

    #endif


        static SBE_CONSTEXPR std::uint16_t openBidId()
        {
            return 4;
        }

        static SBE_CONSTEXPR std::uint64_t openBidSinceVersion()
        {
             return 0;
        }

        bool openBidInActingVersion()
        {
    #if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wtautological-compare"
    #endif
            return m_actingVersion >= openBidSinceVersion();
    #if defined(__clang__)
    #pragma clang diagnostic pop
    #endif
        }

        static SBE_CONSTEXPR std::size_t openBidEncodingOffset()
        {
             return 0;
        }


        static const char *openBidMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "";
            }

            return "";
        }

        static SBE_CONSTEXPR std::uint32_t openBidNullValue()
        {
            return SBE_NULLVALUE_UINT32;
        }

        static SBE_CONSTEXPR std::uint32_t openBidMinValue()
        {
            return 0;
        }

        static SBE_CONSTEXPR std::uint32_t openBidMaxValue()
        {
            return 4294967294;
        }

        static SBE_CONSTEXPR std::size_t openBidEncodingLength()
        {
            return 4;
        }

        std::uint32_t openBid() const
        {
            return SBE_BIG_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset + 0)));
        }

        Candles &openBid(const std::uint32_t value)
        {
            *((std::uint32_t *)(m_buffer + m_offset + 0)) = SBE_BIG_ENDIAN_ENCODE_32(value);
            return *this;
        }

        static SBE_CONSTEXPR std::uint16_t openAskId()
        {
            return 5;
        }

        static SBE_CONSTEXPR std::uint64_t openAskSinceVersion()
        {
             return 0;
        }

        bool openAskInActingVersion()
        {
    #if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wtautological-compare"
    #endif
            return m_actingVersion >= openAskSinceVersion();
    #if defined(__clang__)
    #pragma clang diagnostic pop
    #endif
        }

        static SBE_CONSTEXPR std::size_t openAskEncodingOffset()
        {
             return 4;
        }


        static const char *openAskMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "";
            }

            return "";
        }

        static SBE_CONSTEXPR std::uint32_t openAskNullValue()
        {
            return SBE_NULLVALUE_UINT32;
        }

        static SBE_CONSTEXPR std::uint32_t openAskMinValue()
        {
            return 0;
        }

        static SBE_CONSTEXPR std::uint32_t openAskMaxValue()
        {
            return 4294967294;
        }

        static SBE_CONSTEXPR std::size_t openAskEncodingLength()
        {
            return 4;
        }

        std::uint32_t openAsk() const
        {
            return SBE_BIG_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset + 4)));
        }

        Candles &openAsk(const std::uint32_t value)
        {
            *((std::uint32_t *)(m_buffer + m_offset + 4)) = SBE_BIG_ENDIAN_ENCODE_32(value);
            return *this;
        }

        static SBE_CONSTEXPR std::uint16_t highBidId()
        {
            return 6;
        }

        static SBE_CONSTEXPR std::uint64_t highBidSinceVersion()
        {
             return 0;
        }

        bool highBidInActingVersion()
        {
    #if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wtautological-compare"
    #endif
            return m_actingVersion >= highBidSinceVersion();
    #if defined(__clang__)
    #pragma clang diagnostic pop
    #endif
        }

        static SBE_CONSTEXPR std::size_t highBidEncodingOffset()
        {
             return 8;
        }


        static const char *highBidMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "";
            }

            return "";
        }

        static SBE_CONSTEXPR std::uint32_t highBidNullValue()
        {
            return SBE_NULLVALUE_UINT32;
        }

        static SBE_CONSTEXPR std::uint32_t highBidMinValue()
        {
            return 0;
        }

        static SBE_CONSTEXPR std::uint32_t highBidMaxValue()
        {
            return 4294967294;
        }

        static SBE_CONSTEXPR std::size_t highBidEncodingLength()
        {
            return 4;
        }

        std::uint32_t highBid() const
        {
            return SBE_BIG_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset + 8)));
        }

        Candles &highBid(const std::uint32_t value)
        {
            *((std::uint32_t *)(m_buffer + m_offset + 8)) = SBE_BIG_ENDIAN_ENCODE_32(value);
            return *this;
        }

        static SBE_CONSTEXPR std::uint16_t highAskId()
        {
            return 7;
        }

        static SBE_CONSTEXPR std::uint64_t highAskSinceVersion()
        {
             return 0;
        }

        bool highAskInActingVersion()
        {
    #if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wtautological-compare"
    #endif
            return m_actingVersion >= highAskSinceVersion();
    #if defined(__clang__)
    #pragma clang diagnostic pop
    #endif
        }

        static SBE_CONSTEXPR std::size_t highAskEncodingOffset()
        {
             return 12;
        }


        static const char *highAskMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "";
            }

            return "";
        }

        static SBE_CONSTEXPR std::uint32_t highAskNullValue()
        {
            return SBE_NULLVALUE_UINT32;
        }

        static SBE_CONSTEXPR std::uint32_t highAskMinValue()
        {
            return 0;
        }

        static SBE_CONSTEXPR std::uint32_t highAskMaxValue()
        {
            return 4294967294;
        }

        static SBE_CONSTEXPR std::size_t highAskEncodingLength()
        {
            return 4;
        }

        std::uint32_t highAsk() const
        {
            return SBE_BIG_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset + 12)));
        }

        Candles &highAsk(const std::uint32_t value)
        {
            *((std::uint32_t *)(m_buffer + m_offset + 12)) = SBE_BIG_ENDIAN_ENCODE_32(value);
            return *this;
        }

        static SBE_CONSTEXPR std::uint16_t lowBidId()
        {
            return 8;
        }

        static SBE_CONSTEXPR std::uint64_t lowBidSinceVersion()
        {
             return 0;
        }

        bool lowBidInActingVersion()
        {
    #if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wtautological-compare"
    #endif
            return m_actingVersion >= lowBidSinceVersion();
    #if defined(__clang__)
    #pragma clang diagnostic pop
    #endif
        }

        static SBE_CONSTEXPR std::size_t lowBidEncodingOffset()
        {
             return 16;
        }


        static const char *lowBidMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "";
            }

            return "";
        }

        static SBE_CONSTEXPR std::uint32_t lowBidNullValue()
        {
            return SBE_NULLVALUE_UINT32;
        }

        static SBE_CONSTEXPR std::uint32_t lowBidMinValue()
        {
            return 0;
        }

        static SBE_CONSTEXPR std::uint32_t lowBidMaxValue()
        {
            return 4294967294;
        }

        static SBE_CONSTEXPR std::size_t lowBidEncodingLength()
        {
            return 4;
        }

        std::uint32_t lowBid() const
        {
            return SBE_BIG_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset + 16)));
        }

        Candles &lowBid(const std::uint32_t value)
        {
            *((std::uint32_t *)(m_buffer + m_offset + 16)) = SBE_BIG_ENDIAN_ENCODE_32(value);
            return *this;
        }

        static SBE_CONSTEXPR std::uint16_t lowAskId()
        {
            return 9;
        }

        static SBE_CONSTEXPR std::uint64_t lowAskSinceVersion()
        {
             return 0;
        }

        bool lowAskInActingVersion()
        {
    #if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wtautological-compare"
    #endif
            return m_actingVersion >= lowAskSinceVersion();
    #if defined(__clang__)
    #pragma clang diagnostic pop
    #endif
        }

        static SBE_CONSTEXPR std::size_t lowAskEncodingOffset()
        {
             return 20;
        }


        static const char *lowAskMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "";
            }

            return "";
        }

        static SBE_CONSTEXPR std::uint32_t lowAskNullValue()
        {
            return SBE_NULLVALUE_UINT32;
        }

        static SBE_CONSTEXPR std::uint32_t lowAskMinValue()
        {
            return 0;
        }

        static SBE_CONSTEXPR std::uint32_t lowAskMaxValue()
        {
            return 4294967294;
        }

        static SBE_CONSTEXPR std::size_t lowAskEncodingLength()
        {
            return 4;
        }

        std::uint32_t lowAsk() const
        {
            return SBE_BIG_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset + 20)));
        }

        Candles &lowAsk(const std::uint32_t value)
        {
            *((std::uint32_t *)(m_buffer + m_offset + 20)) = SBE_BIG_ENDIAN_ENCODE_32(value);
            return *this;
        }

        static SBE_CONSTEXPR std::uint16_t closeBidId()
        {
            return 10;
        }

        static SBE_CONSTEXPR std::uint64_t closeBidSinceVersion()
        {
             return 0;
        }

        bool closeBidInActingVersion()
        {
    #if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wtautological-compare"
    #endif
            return m_actingVersion >= closeBidSinceVersion();
    #if defined(__clang__)
    #pragma clang diagnostic pop
    #endif
        }

        static SBE_CONSTEXPR std::size_t closeBidEncodingOffset()
        {
             return 24;
        }


        static const char *closeBidMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "";
            }

            return "";
        }

        static SBE_CONSTEXPR std::uint32_t closeBidNullValue()
        {
            return SBE_NULLVALUE_UINT32;
        }

        static SBE_CONSTEXPR std::uint32_t closeBidMinValue()
        {
            return 0;
        }

        static SBE_CONSTEXPR std::uint32_t closeBidMaxValue()
        {
            return 4294967294;
        }

        static SBE_CONSTEXPR std::size_t closeBidEncodingLength()
        {
            return 4;
        }

        std::uint32_t closeBid() const
        {
            return SBE_BIG_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset + 24)));
        }

        Candles &closeBid(const std::uint32_t value)
        {
            *((std::uint32_t *)(m_buffer + m_offset + 24)) = SBE_BIG_ENDIAN_ENCODE_32(value);
            return *this;
        }

        static SBE_CONSTEXPR std::uint16_t closeAskId()
        {
            return 11;
        }

        static SBE_CONSTEXPR std::uint64_t closeAskSinceVersion()
        {
             return 0;
        }

        bool closeAskInActingVersion()
        {
    #if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wtautological-compare"
    #endif
            return m_actingVersion >= closeAskSinceVersion();
    #if defined(__clang__)
    #pragma clang diagnostic pop
    #endif
        }

        static SBE_CONSTEXPR std::size_t closeAskEncodingOffset()
        {
             return 28;
        }


        static const char *closeAskMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "";
            }

            return "";
        }

        static SBE_CONSTEXPR std::uint32_t closeAskNullValue()
        {
            return SBE_NULLVALUE_UINT32;
        }

        static SBE_CONSTEXPR std::uint32_t closeAskMinValue()
        {
            return 0;
        }

        static SBE_CONSTEXPR std::uint32_t closeAskMaxValue()
        {
            return 4294967294;
        }

        static SBE_CONSTEXPR std::size_t closeAskEncodingLength()
        {
            return 4;
        }

        std::uint32_t closeAsk() const
        {
            return SBE_BIG_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset + 28)));
        }

        Candles &closeAsk(const std::uint32_t value)
        {
            *((std::uint32_t *)(m_buffer + m_offset + 28)) = SBE_BIG_ENDIAN_ENCODE_32(value);
            return *this;
        }

        static SBE_CONSTEXPR std::uint16_t volumnId()
        {
            return 12;
        }

        static SBE_CONSTEXPR std::uint64_t volumnSinceVersion()
        {
             return 0;
        }

        bool volumnInActingVersion()
        {
    #if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wtautological-compare"
    #endif
            return m_actingVersion >= volumnSinceVersion();
    #if defined(__clang__)
    #pragma clang diagnostic pop
    #endif
        }

        static SBE_CONSTEXPR std::size_t volumnEncodingOffset()
        {
             return 32;
        }


        static const char *volumnMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "";
            }

            return "";
        }

        static SBE_CONSTEXPR std::uint32_t volumnNullValue()
        {
            return SBE_NULLVALUE_UINT32;
        }

        static SBE_CONSTEXPR std::uint32_t volumnMinValue()
        {
            return 0;
        }

        static SBE_CONSTEXPR std::uint32_t volumnMaxValue()
        {
            return 4294967294;
        }

        static SBE_CONSTEXPR std::size_t volumnEncodingLength()
        {
            return 4;
        }

        std::uint32_t volumn() const
        {
            return SBE_BIG_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset + 32)));
        }

        Candles &volumn(const std::uint32_t value)
        {
            *((std::uint32_t *)(m_buffer + m_offset + 32)) = SBE_BIG_ENDIAN_ENCODE_32(value);
            return *this;
        }

        static SBE_CONSTEXPR std::uint16_t completeId()
        {
            return 13;
        }

        static SBE_CONSTEXPR std::uint64_t completeSinceVersion()
        {
             return 0;
        }

        bool completeInActingVersion()
        {
    #if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wtautological-compare"
    #endif
            return m_actingVersion >= completeSinceVersion();
    #if defined(__clang__)
    #pragma clang diagnostic pop
    #endif
        }

        static SBE_CONSTEXPR std::size_t completeEncodingOffset()
        {
             return 36;
        }


        static const char *completeMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "";
            }

            return "";
        }

        BooleanType::Value complete() const
        {
            return BooleanType::get((*((std::uint8_t *)(m_buffer + m_offset + 36))));
        }

        Candles &complete(const BooleanType::Value value)
        {
            *((std::uint8_t *)(m_buffer + m_offset + 36)) = (value);
            return *this;
        }
        static SBE_CONSTEXPR std::size_t completeEncodingLength()
        {
            return 1;
        }

        static SBE_CONSTEXPR std::uint16_t timeId()
        {
            return 200;
        }

        static SBE_CONSTEXPR std::uint64_t timeSinceVersion()
        {
             return 0;
        }

        bool timeInActingVersion()
        {
    #if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wtautological-compare"
    #endif
            return m_actingVersion >= timeSinceVersion();
    #if defined(__clang__)
    #pragma clang diagnostic pop
    #endif
        }

        static SBE_CONSTEXPR std::size_t timeEncodingOffset()
        {
             return 37;
        }


        static const char *timeMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "";
            }

            return "";
        }

        static SBE_CONSTEXPR char timeNullValue()
        {
            return (char)0;
        }

        static SBE_CONSTEXPR char timeMinValue()
        {
            return (char)32;
        }

        static SBE_CONSTEXPR char timeMaxValue()
        {
            return (char)126;
        }

        static SBE_CONSTEXPR std::size_t timeEncodingLength()
        {
            return 1;
        }

        static SBE_CONSTEXPR std::uint64_t timeLength()
        {
            return 32;
        }

        const char *time() const
        {
            return (m_buffer + m_offset + 37);
        }

        char time(const std::uint64_t index) const
        {
            if (index >= 32)
            {
                throw std::runtime_error("index out of range for time [E104]");
            }

            return (*((char *)(m_buffer + m_offset + 37 + (index * 1))));
        }

        void time(const std::uint64_t index, const char value)
        {
            if (index >= 32)
            {
                throw std::runtime_error("index out of range for time [E105]");
            }

            *((char *)(m_buffer + m_offset + 37 + (index * 1))) = (value);
        }

        std::uint64_t getTime(char *dst, const std::uint64_t length) const
        {
            if (length > 32)
            {
                 throw std::runtime_error("length too large for getTime [E106]");
            }

            std::memcpy(dst, m_buffer + m_offset + 37, sizeof(char) * length);
            return length;
        }

        Candles &putTime(const char *src)
        {
            std::memcpy(m_buffer + m_offset + 37, src, sizeof(char) * 32);
            return *this;
        }

        std::string getTimeAsString() const
        {
            std::string result(m_buffer + m_offset + 37, 32);
            return result;
        }

        Candles &putTime(const std::string& str)
        {
            std::memcpy(m_buffer + m_offset + 37, str.c_str(), 32);
            return *this;
        }

    };

private:
    Candles m_candles;

public:

    static SBE_CONSTEXPR std::uint16_t CandlesId()
    {
        return 3;
    }


    inline Candles &candles()
    {
        m_candles.wrapForDecode(m_buffer, m_positionPtr, m_actingVersion, m_bufferLength);
        return m_candles;
    }

    Candles &candlesCount(const std::uint16_t count)
    {
        m_candles.wrapForEncode(m_buffer, count, m_positionPtr, m_actingVersion, m_bufferLength);
        return m_candles;
    }

    static SBE_CONSTEXPR std::uint64_t candlesSinceVersion()
    {
         return 0;
    }

    bool candlesInActingVersion()
    {
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-compare"
#endif
        return m_actingVersion >= candlesSinceVersion();
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
    }
};
}
#endif
