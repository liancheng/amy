#ifndef __AMYSQL_DETAIL_THROW_ERROR_HPP__
#define __AMYSQL_DETAIL_THROW_ERROR_HPP__

#include <amysql/detail/mysql_types.hpp>
#include <amysql/error.hpp>
#include <amysql/system_error.hpp>

#include <boost/system/system_error.hpp>
#include <boost/throw_exception.hpp>

namespace amysql {
namespace detail {

inline void throw_error(boost::system::error_code const& ec,
                        detail::mysql_handle m)
{
    if (ec) {
        if (ec.category() == amysql::error::get_client_category()) {
            amysql::system_error e(ec, ::mysql_error(m));
            boost::throw_exception(e);
        }
        else {
            amysql::system_error e(ec);
            boost::throw_exception(e);
        }
    }
}

}   //  namespace detail
}   //  namespace amysql

#endif  //  __AMYSQL_DETAIL_THROW_ERROR_HPP__

// vim:ft=cpp ts=4 sw=4 et
