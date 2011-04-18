#ifndef __AMY_SQL_TYPES_HPP__
#define __AMY_SQL_TYPES_HPP__

#include <amy/detail/mysql.hpp>
#include <amy/detail/mysql_types.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

namespace amy {

typedef int8_t   sql_tinyint;
typedef uint8_t  sql_tinyint_unsigned;
typedef int16_t  sql_smallint;
typedef uint16_t sql_smallint_unsigned;
typedef int32_t  sql_int;
typedef uint32_t sql_int_unsigned;
typedef int32_t  sql_mediumint;
typedef uint32_t sql_mediumint_unsigned;
typedef int64_t  sql_bigint;
typedef uint64_t sql_bigint_unsigned;
typedef float    sql_float;
typedef double   sql_double;
typedef double   sql_decimal;

typedef std::string sql_blob;
typedef std::string sql_enum;
typedef std::string sql_char;
typedef std::string sql_tinytext;
typedef std::string sql_mediumtext;
typedef std::string sql_longtext;
typedef std::string sql_varchar;
typedef std::string sql_text;

typedef bool sql_bool;
typedef bool sql_boolean;

typedef boost::posix_time::ptime         sql_datetime;
typedef boost::posix_time::time_duration sql_time;

}   //  namespace amy

#endif  //  __AMY_SQL_TYPES_HPP__

// vim:ft=cpp ts=4 sw=4 et
