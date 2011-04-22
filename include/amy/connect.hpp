#ifndef __AMY_CONNECT_HPP__
#define __AMY_CONNECT_HPP__

#include <amy/auth_info.hpp>
#include <amy/client_flags.hpp>
#include <amy/endpoint_traits.hpp>

#include <boost/bind.hpp>
#include <boost/parameter.hpp>
#include <boost/system/error_code.hpp>

namespace amy {
namespace detail {

template<
    typename MySQLConnector,
    typename Endpoint
>
void connect(MySQLConnector& connector,
             Endpoint const& endpoint,
             auth_info const& auth,
             std::string const& database,
             client_flags flags = default_flags)
{
    connector.connect(endpoint, auth, database, flags);
}

template<
    typename MySQLConnector,
    typename Endpoint
>
boost::system::error_code connect(MySQLConnector& connector,
                                  Endpoint const& endpoint,
                                  auth_info const& auth,
                                  std::string const& database,
                                  client_flags flags,
                                  boost::system::error_code& ec)
{
    return connector.connect(endpoint, auth, database, flags, ec);
}

template<
    typename MySQLConnector,
    typename Endpoint,
    typename ConnectHandler
>
void async_connect(MySQLConnector& connector,
                   Endpoint const& endpoint,
                   auth_info const& auth,
                   std::string const& database,
                   client_flags flags,
                   ConnectHandler handler)
{
    connector.async_connect(endpoint, auth, database, flags, handler);
}

}   //  namespace detail

namespace keyword {

BOOST_PARAMETER_NAME(connector)
BOOST_PARAMETER_NAME(endpoint)
BOOST_PARAMETER_NAME(auth)
BOOST_PARAMETER_NAME(database)
BOOST_PARAMETER_NAME(flags)
BOOST_PARAMETER_NAME(handler)

}   //  namespace keyword

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
    detail::connect(connector, endpoint, auth, database, flags);
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
    detail::async_connect(
            connector, endpoint, auth, database, flags, handler);
}

}   //  namespace amy

#endif  //  __AMY_CONNECT_HPP__

// vim:ft=cpp ts=4 sw=4 et
