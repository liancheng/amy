#ifndef __AMY_DETAIL_VALUE_CAST_HPP__
#define __AMY_DETAIL_VALUE_CAST_HPP__

#include <amy/sql_types.hpp>

#include <boost/utility/string_view.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iomanip>
#include <string>
#include <sstream>

namespace amy {
namespace detail {

using string_view = boost::string_view;

template<typename ValueType>
inline ValueType value_cast(string_view str) {
    ValueType v{};
    std::istringstream is(str.to_string());
    is >> v;
    return v;
}

template<>
inline string_view value_cast(string_view str) {
    return str;
}

template<>
inline std::string value_cast(string_view str) {
    return str.to_string();
}

template<>
inline long long value_cast(string_view str) {
    return std::strtoll(str.data(), NULL, 10);
}

template<>
inline long value_cast(string_view str) {
    return std::strtol(str.data(), NULL, 10);
}

template<>
inline int32_t value_cast(string_view str) {
    return static_cast<int32_t>(std::strtol(str.data(), NULL, 10));
}

template<>
inline int16_t value_cast(string_view str) {
    return static_cast<int16_t>(std::strtol(str.data(), NULL, 10));
}

template<>
inline int8_t value_cast(string_view str) {
  return static_cast<int8_t>(std::strtol(str.data(), NULL, 10));
}

template<>
inline bool value_cast(string_view str) {
  return static_cast<bool>(std::strtol(str.data(), NULL, 10));
}

template<>
inline unsigned long long value_cast(string_view str) {
    return std::strtoull(str.data(), NULL, 10);
}

template<>
inline unsigned long value_cast(string_view str) {
    return std::strtoul(str.data(), NULL, 10);
}

template<>
inline uint32_t value_cast(string_view str) {
    return static_cast<uint32_t>(std::strtoul(str.data(), NULL, 10));
}

template<>
inline uint16_t value_cast(string_view str) {
    return static_cast<int16_t>(std::strtoul(str.data(), NULL, 10));
}

template<>
inline uint8_t value_cast(string_view str) {
  return static_cast<uint8_t>(std::strtoul(str.data(), NULL, 10));
}

template<>
inline double value_cast(string_view str) {
  return std::strtod(str.data(), NULL);
}

template<>
inline float value_cast(string_view str) {
  return std::strtof(str.data(), NULL);
}

template<>
inline sql_datetime value_cast(string_view str) {
    using namespace boost::posix_time;

    std::istringstream in(str.to_string());
    in.unsetf(std::ios::skipws);
    in.imbue(std::locale(std::locale::classic(),
                         new time_input_facet("%Y-%m-%d %H:%M:%S%F")));

    sql_datetime v;
    in >> v;
    return v;
}

template<>
inline sql_time value_cast(string_view str) {
    using namespace boost::posix_time;

    // It's a pity that Boost.Date_Time cannot correctly parse negative time
    // durations, so we have to deal with the sign here.
    bool negative = (str[0] == '-') ? true : false;
    string_view const* ptr = &str;
    string_view s;
    if (negative) {
        s = str.substr(1);
        ptr = &s;
    }

    time_input_facet* input_facet = new time_input_facet();
    input_facet->time_duration_format("%H:%M:%S:%F");

    std::istringstream in(ptr->to_string());
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
inline sql_bigint_unsigned value_cast(string_view str) {
    sql_bigint_unsigned v = ::_strtoui64(str.c_str(), NULL, 10);
    return v;
}

#endif // ifdef WIN32

} // namespace detail
} // namespace amy

#endif // __AMY_DETAIL_VALUE_CAST_HPP__

// vim:ft=cpp sw=4 ts=4 tw=80 et
