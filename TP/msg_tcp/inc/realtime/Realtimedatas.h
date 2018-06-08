/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _REALTIME_REALTIMEDATAS_H_
#define _REALTIME_REALTIMEDATAS_H_

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
#include "GroupSizeEncoding.h"

using namespace sbe;

namespace realtime {

class Realtimedatas
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

    inline void reset(const Realtimedatas& codec)
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

    Realtimedatas() : m_buffer(nullptr), m_bufferLength(0), m_offset(0) {}

    Realtimedatas(char *buffer, const std::uint64_t bufferLength)
    {
        reset(buffer, 0, bufferLength, sbeBlockLength(), sbeSchemaVersion());
    }

    Realtimedatas(char *buffer, const std::uint64_t bufferLength, const std::uint64_t actingBlockLength, const std::uint64_t actingVersion)
    {
        reset(buffer, 0, bufferLength, actingBlockLength, actingVersion);
    }

    Realtimedatas(const Realtimedatas& codec)
    {
        reset(codec);
    }

#if __cplusplus >= 201103L
    Realtimedatas(Realtimedatas&& codec)
    {
        reset(codec);
    }

    Realtimedatas& operator=(Realtimedatas&& codec)
    {
        reset(codec);
        return *this;
    }

#endif

    Realtimedatas& operator=(const Realtimedatas& codec)
    {
        reset(codec);
        return *this;
    }

    static SBE_CONSTEXPR std::uint16_t sbeBlockLength()
    {
        return (std::uint16_t)0;
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

    Realtimedatas &wrapForEncode(char *buffer, const std::uint64_t offset, const std::uint64_t bufferLength)
    {
        reset(buffer, offset, bufferLength, sbeBlockLength(), sbeSchemaVersion());
        return *this;
    }

    Realtimedatas &wrapForDecode(
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

    class RealtimeDataList
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
            m_dimensions.blockLength((std::uint16_t)28);
            m_dimensions.numInGroup((std::uint16_t)count);
            m_index = -1;
            m_count = count;
            m_blockLength = 28;
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
            return 28;
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

        inline RealtimeDataList &next()
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


        static SBE_CONSTEXPR std::uint16_t statusId()
        {
            return 2;
        }

        static SBE_CONSTEXPR std::uint64_t statusSinceVersion()
        {
             return 0;
        }

        bool statusInActingVersion()
        {
    #if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wtautological-compare"
    #endif
            return m_actingVersion >= statusSinceVersion();
    #if defined(__clang__)
    #pragma clang diagnostic pop
    #endif
        }

        static SBE_CONSTEXPR std::size_t statusEncodingOffset()
        {
             return 0;
        }


        static const char *statusMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "";
            }

            return "";
        }

        static SBE_CONSTEXPR std::uint32_t statusNullValue()
        {
            return SBE_NULLVALUE_UINT32;
        }

        static SBE_CONSTEXPR std::uint32_t statusMinValue()
        {
            return 0;
        }

        static SBE_CONSTEXPR std::uint32_t statusMaxValue()
        {
            return 4294967294;
        }

        static SBE_CONSTEXPR std::size_t statusEncodingLength()
        {
            return 4;
        }

        std::uint32_t status() const
        {
            return SBE_BIG_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset + 0)));
        }

        RealtimeDataList &status(const std::uint32_t value)
        {
            *((std::uint32_t *)(m_buffer + m_offset + 0)) = SBE_BIG_ENDIAN_ENCODE_32(value);
            return *this;
        }

        static SBE_CONSTEXPR std::uint16_t bidId()
        {
            return 3;
        }

        static SBE_CONSTEXPR std::uint64_t bidSinceVersion()
        {
             return 0;
        }

        bool bidInActingVersion()
        {
    #if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wtautological-compare"
    #endif
            return m_actingVersion >= bidSinceVersion();
    #if defined(__clang__)
    #pragma clang diagnostic pop
    #endif
        }

        static SBE_CONSTEXPR std::size_t bidEncodingOffset()
        {
             return 4;
        }


        static const char *bidMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "";
            }

            return "";
        }

        static SBE_CONSTEXPR std::int32_t bidNullValue()
        {
            return SBE_NULLVALUE_INT32;
        }

        static SBE_CONSTEXPR std::int32_t bidMinValue()
        {
            return -2147483647;
        }

        static SBE_CONSTEXPR std::int32_t bidMaxValue()
        {
            return 2147483647;
        }

        static SBE_CONSTEXPR std::size_t bidEncodingLength()
        {
            return 4;
        }

        std::int32_t bid() const
        {
            return SBE_BIG_ENDIAN_ENCODE_32(*((std::int32_t *)(m_buffer + m_offset + 4)));
        }

        RealtimeDataList &bid(const std::int32_t value)
        {
            *((std::int32_t *)(m_buffer + m_offset + 4)) = SBE_BIG_ENDIAN_ENCODE_32(value);
            return *this;
        }

        static SBE_CONSTEXPR std::uint16_t askId()
        {
            return 4;
        }

        static SBE_CONSTEXPR std::uint64_t askSinceVersion()
        {
             return 0;
        }

        bool askInActingVersion()
        {
    #if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wtautological-compare"
    #endif
            return m_actingVersion >= askSinceVersion();
    #if defined(__clang__)
    #pragma clang diagnostic pop
    #endif
        }

        static SBE_CONSTEXPR std::size_t askEncodingOffset()
        {
             return 8;
        }


        static const char *askMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "";
            }

            return "";
        }

        static SBE_CONSTEXPR std::int32_t askNullValue()
        {
            return SBE_NULLVALUE_INT32;
        }

        static SBE_CONSTEXPR std::int32_t askMinValue()
        {
            return -2147483647;
        }

        static SBE_CONSTEXPR std::int32_t askMaxValue()
        {
            return 2147483647;
        }

        static SBE_CONSTEXPR std::size_t askEncodingLength()
        {
            return 4;
        }

        std::int32_t ask() const
        {
            return SBE_BIG_ENDIAN_ENCODE_32(*((std::int32_t *)(m_buffer + m_offset + 8)));
        }

        RealtimeDataList &ask(const std::int32_t value)
        {
            *((std::int32_t *)(m_buffer + m_offset + 8)) = SBE_BIG_ENDIAN_ENCODE_32(value);
            return *this;
        }

        static SBE_CONSTEXPR std::uint16_t timestampId()
        {
            return 5;
        }

        static SBE_CONSTEXPR std::uint64_t timestampSinceVersion()
        {
             return 0;
        }

        bool timestampInActingVersion()
        {
    #if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wtautological-compare"
    #endif
            return m_actingVersion >= timestampSinceVersion();
    #if defined(__clang__)
    #pragma clang diagnostic pop
    #endif
        }

        static SBE_CONSTEXPR std::size_t timestampEncodingOffset()
        {
             return 12;
        }


        static const char *timestampMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "";
            }

            return "";
        }

        static SBE_CONSTEXPR std::uint64_t timestampNullValue()
        {
            return SBE_NULLVALUE_UINT64;
        }

        static SBE_CONSTEXPR std::uint64_t timestampMinValue()
        {
            return 0x0L;
        }

        static SBE_CONSTEXPR std::uint64_t timestampMaxValue()
        {
            return 0xfffffffffffffffeL;
        }

        static SBE_CONSTEXPR std::size_t timestampEncodingLength()
        {
            return 8;
        }

        std::uint64_t timestamp() const
        {
            return SBE_BIG_ENDIAN_ENCODE_64(*((std::uint64_t *)(m_buffer + m_offset + 12)));
        }

        RealtimeDataList &timestamp(const std::uint64_t value)
        {
            *((std::uint64_t *)(m_buffer + m_offset + 12)) = SBE_BIG_ENDIAN_ENCODE_64(value);
            return *this;
        }

        static SBE_CONSTEXPR std::uint16_t instrumentId()
        {
            return 6;
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
             return 20;
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

        static SBE_CONSTEXPR char instrumentNullValue()
        {
            return (char)0;
        }

        static SBE_CONSTEXPR char instrumentMinValue()
        {
            return (char)32;
        }

        static SBE_CONSTEXPR char instrumentMaxValue()
        {
            return (char)126;
        }

        static SBE_CONSTEXPR std::size_t instrumentEncodingLength()
        {
            return 1;
        }

        static SBE_CONSTEXPR std::uint64_t instrumentLength()
        {
            return 8;
        }

        const char *instrument() const
        {
            return (m_buffer + m_offset + 20);
        }

        char instrument(const std::uint64_t index) const
        {
            if (index >= 8)
            {
                throw std::runtime_error("index out of range for instrument [E104]");
            }

            return (*((char *)(m_buffer + m_offset + 20 + (index * 1))));
        }

        void instrument(const std::uint64_t index, const char value)
        {
            if (index >= 8)
            {
                throw std::runtime_error("index out of range for instrument [E105]");
            }

            *((char *)(m_buffer + m_offset + 20 + (index * 1))) = (value);
        }

        std::uint64_t getInstrument(char *dst, const std::uint64_t length) const
        {
            if (length > 8)
            {
                 throw std::runtime_error("length too large for getInstrument [E106]");
            }

            std::memcpy(dst, m_buffer + m_offset + 20, sizeof(char) * length);
            return length;
        }

        RealtimeDataList &putInstrument(const char *src)
        {
            std::memcpy(m_buffer + m_offset + 20, src, sizeof(char) * 8);
            return *this;
        }

        std::string getInstrumentAsString() const
        {
            std::string result(m_buffer + m_offset + 20, 8);
            return result;
        }

        RealtimeDataList &putInstrument(const std::string& str)
        {
            std::memcpy(m_buffer + m_offset + 20, str.c_str(), 8);
            return *this;
        }

    };

private:
    RealtimeDataList m_realtimeDataList;

public:

    static SBE_CONSTEXPR std::uint16_t RealtimeDataListId()
    {
        return 1;
    }


    inline RealtimeDataList &realtimeDataList()
    {
        m_realtimeDataList.wrapForDecode(m_buffer, m_positionPtr, m_actingVersion, m_bufferLength);
        return m_realtimeDataList;
    }

    RealtimeDataList &realtimeDataListCount(const std::uint16_t count)
    {
        m_realtimeDataList.wrapForEncode(m_buffer, count, m_positionPtr, m_actingVersion, m_bufferLength);
        return m_realtimeDataList;
    }

    static SBE_CONSTEXPR std::uint64_t realtimeDataListSinceVersion()
    {
         return 0;
    }

    bool realtimeDataListInActingVersion()
    {
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-compare"
#endif
        return m_actingVersion >= realtimeDataListSinceVersion();
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
    }
};
}
#endif
