#ifndef __AMY_DETAIL_THROW_ERROR_HPP__
#define __AMY_DETAIL_THROW_ERROR_HPP__

#include <amy/detail/mysql_types.hpp>
#include <amy/error.hpp>
#include <amy/system_error.hpp>

#include <boost/system/system_error.hpp>
#include <boost/throw_exception.hpp>

namespace amy {
namespace detail {

inline void throw_error(boost::system::error_code const& ec,
                        detail::mysql_handle m)
{
    if (ec) {
        if (ec.category() == amy::error::get_client_category()) {
            amy::system_error e(ec, ::mysql_error(m));
            boost::throw_exception(e);
        }
        else {
            amy::system_error e(ec);
            boost::throw_exception(e);
        }
    }
}

}   //  namespace detail
}   //  namespace amy

#endif  //  __AMY_DETAIL_THROW_ERROR_HPP__

// vim:ft=cpp ts=4 sw=4 et
