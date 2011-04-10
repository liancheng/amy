#ifndef __AMYSQL_CONNECTOR_HPP__
#define __AMYSQL_CONNECTOR_HPP__

#include <amysql/basic_connector.hpp>
#include <amysql/mysql_service.hpp>

namespace amysql {

typedef
    basic_connector<mysql_service>
    connector;

}   //  namespace amysql

#endif  //  __AMYSQL_CONNECTOR_HPP__

// vim:ft=cpp ts=4 sw=4 et
