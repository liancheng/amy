#ifndef __AMY_CONNECT_HPP__
#define __AMY_CONNECT_HPP__

#include <amy/auth_info.hpp>
#include <amy/client_flags.hpp>
#include <amy/endpoint_traits.hpp>

#include <boost/bind.hpp>
#include <boost/system/error_code.hpp>

namespace amy {

template<typename MySQLConnector>
void connect(MySQLConnector& connector,
             client_flags flags = default_flags)
{
    connector.connect(null_endpoint(), auth_info(), "", flags);
}

template<
    typename MySQLConnector,
    typename Endpoint
>
void connect(MySQLConnector& connector,
             Endpoint const& endpoint,
             client_flags flags = default_flags)
{
    connector.connect(endpoint, auth_info(), "", flags);
}

template<
    typename MySQLConnector,
    typename Endpoint
>
void connect(MySQLConnector& connector,
             Endpoint const& endpoint,
             auth_info const& auth,
             client_flags flags = default_flags)
{
    connector.connect(endpoint, auth, "", flags);
}

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
                                  boost::system::error_code& ec)
{
    return connector.connect(endpoint, auth_info(), "", default_flags, ec);
}

template<
    typename MySQLConnector,
    typename Endpoint
>
boost::system::error_code connect(MySQLConnector& connector,
                                  Endpoint const& endpoint,
                                  auth_info const& auth,
                                  boost::system::error_code& ec)
{
    return connector.connect(endpoint, auth, "", default_flags, ec);
}

template<
    typename MySQLConnector,
    typename Endpoint
>
boost::system::error_code connect(MySQLConnector& connector,
                                  Endpoint const& endpoint,
                                  auth_info const& auth,
                                  std::string const& database,
                                  boost::system::error_code& ec)
{
    return connector.connect(endpoint, auth, database, default_flags, ec);
}

template<
    typename MySQLConnector,
    typename Endpoint
>
boost::system::error_code connect(MySQLConnector& connector,
                                  Endpoint const& endpoint,
                                  auth_info const& auth,
                                  client_flags flags,
                                  boost::system::error_code& ec)
{
    return connector.connect(endpoint, auth, "", flags, ec);
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
    typename ConnectHandler
>
void async_connect(MySQLConnector& connector, ConnectHandler handler) {
    async_connect(connector, null_endpoint(),
                  auth_info(), "", default_flags, handler);
}

template<
    typename MySQLConnector,
    typename Endpoint,
    typename ConnectHandler
>
void async_connect(MySQLConnector& connector,
                   Endpoint const& endpoint,
                   ConnectHandler handler)
{
    async_connect(connector, endpoint, auth_info(), "", default_flags, handler);
}

template<
    typename MySQLConnector,
    typename Endpoint,
    typename ConnectHandler
>
void async_connect(MySQLConnector& connector,
                   Endpoint const& endpoint,
                   auth_info const& auth,
                   ConnectHandler handler)
{
    async_connect(connector, endpoint, auth, "", default_flags, handler);
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
                   ConnectHandler handler)
{
    async_connect(connector, endpoint, auth, database, default_flags, handler);
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

}   //  namespace amy

#endif  //  __AMY_CONNECT_HPP__

// vim:ft=cpp ts=4 sw=4 et
