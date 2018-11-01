#ifndef __AMY_DETAIL_VALUE_CAST_HPP__
#define __AMY_DETAIL_VALUE_CAST_HPP__

#include <amy/sql_types.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <iomanip>
#include <string>
#include <sstream>

namespace amy {
namespace detail {

template<typename ValueType>
ValueType value_cast(const char* s, unsigned long l) {
    ValueType v{};
    std::istringstream is({s, l});
    is >> v;
    return v;
}

template<>
inline std::string value_cast(const char* s, unsigned long l) {
    return {s, l};
}

template<>
inline long long value_cast(const char* s, unsigned long) {
    return std::strtoll(s, NULL, 10);
}

template<>
inline long value_cast(const char* s, unsigned long) {
    return std::strtol(s, NULL, 10);
}

template<>
inline int32_t value_cast(const char* s, unsigned long) {
    return static_cast<int32_t>(std::strtol(s, NULL, 10));
}

template<>
inline int16_t value_cast(const char* s, unsigned long) {
    return static_cast<int16_t>(std::strtol(s, NULL, 10));
}

template<>
inline int8_t value_cast(const char* s, unsigned long) {
    return static_cast<int8_t>(std::strtol(s, NULL, 10));
}

template<>
inline bool value_cast(const char* s, unsigned long) {
    return static_cast<bool>(std::strtol(s, NULL, 10));
}

template<>
inline unsigned long long value_cast(const char* s, unsigned long) {
    return std::strtoull(s, NULL, 10);
}

template<>
inline unsigned long value_cast(const char* s, unsigned long) {
    return std::strtoul(s, NULL, 10);
}

template<>
inline uint32_t value_cast(const char* s, unsigned long) {
    return static_cast<uint32_t>(std::strtoul(s, NULL, 10));
}

template<>
inline uint16_t value_cast(const char* s, unsigned long) {
    return static_cast<int16_t>(std::strtoul(s, NULL, 10));
}

template<>
inline uint8_t value_cast(const char* s, unsigned long) {
    return static_cast<uint8_t>(std::strtoul(s, NULL, 10));
}

template<>
inline double value_cast(const char* s, unsigned long) {
    return std::strtod(s, NULL);
}

template<>
inline float value_cast(const char* s, unsigned long) {
    return std::strtof(s, NULL);
}

template<>
inline sql_datetime value_cast(const char* s, unsigned long l) {
    using namespace boost::posix_time;

    std::istringstream in({s, l});
    in.unsetf(std::ios::skipws);
    in.imbue(std::locale(std::locale::classic(),
                         new time_input_facet("%Y-%m-%d %H:%M:%S%F")));

    sql_datetime v;
    in >> v;
    return v;
}

template<>
inline sql_time value_cast(const char* str, unsigned long len) {
    using namespace boost::posix_time;

    // It's a pity that Boost.Date_Time cannot correctly parse negative time
    // durations, so we have to deal with the sign here.
    assert(len > 0);
    bool negative = (str[0] == '-') ? true : false;
    if (negative) {
        ++str;
        --len;
    }

    time_input_facet* input_facet = new time_input_facet();
    input_facet->time_duration_format("%H:%M:%S:%F");

    std::istringstream in({str, len});
    in.unsetf(std::ios::skipws);
    in.imbue(std::locale(std::locale::classic(), input_facet));

    sql_time v;
    in >> v;

    if (negative) {
        v = hours(0) - v;
    }

    return v;
}

#ifdef WIN32

#include <stdio.h> // For ::_strtoui64

/// \c value_cast<sql_bigint_unsigned> specialization for win32 platform only.
/**
 * \c std::istringstream cannot deal with unsigned 64-bit integer under the
 * win32 platform.  And \c _strtoui64 is used instead.
 */
template<>
inline sql_bigint_unsigned value_cast(const char* s, unsigned long) {
    sql_bigint_unsigned v = ::_strtoui64(s, NULL, 10);
    return v;
}

#endif // ifdef WIN32

} // namespace detail
} // namespace amy

#endif // __AMY_DETAIL_VALUE_CAST_HPP__

// vim:ft=cpp sw=4 ts=4 tw=80 et
