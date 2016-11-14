#ifndef __AMY_CONNECT_HPP__
#define __AMY_CONNECT_HPP__

#include <amy/auth_info.hpp>
#include <amy/client_flags.hpp>
#include <amy/endpoint_traits.hpp>

#include <boost/bind.hpp>
#include <boost/parameter.hpp>
#include <boost/system/error_code.hpp>

namespace amy {
namespace keyword {

BOOST_PARAMETER_NAME(connector)
BOOST_PARAMETER_NAME(endpoint)
BOOST_PARAMETER_NAME(auth)
BOOST_PARAMETER_NAME(database)
BOOST_PARAMETER_NAME(flags)
BOOST_PARAMETER_NAME(handler)

} // namespace keyword

BOOST_PARAMETER_FUNCTION(
        (void),
        connect,
        keyword::tag,

        (required
            (in_out(connector), *))

        (optional
            (endpoint, *, amy::null_endpoint())
            (auth,     *, amy::auth_info())
            (database, *, std::string())
            (flags,    *, amy::default_flags)))
{
    connector.connect(endpoint, auth, database, flags);
}

inline void ignore_connect_event(boost::system::error_code const&) {
}

BOOST_PARAMETER_FUNCTION(
        (void),
        async_connect,
        keyword::tag,

        (required
            (in_out(connector), *))

        (optional
            (endpoint, *, amy::null_endpoint())
            (auth,     *, amy::auth_info())
            (database, *, std::string())
            (flags,    *, amy::default_flags)
            (handler,  *, &ignore_connect_event)))
{
    connector.async_connect(endpoint, auth, database, flags, handler);
}

} // namespace amy

#endif // __AMY_CONNECT_HPP__

// vim:ft=cpp ts=4 sw=4 et
