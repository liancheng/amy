#ifndef __AMY_CONNECTOR_HPP__
#define __AMY_CONNECTOR_HPP__

#include <amy/basic_connector.hpp>
#include <amy/mysql_service.hpp>

namespace amy {

typedef
    basic_connector<mysql_service>
    connector;

}   //  namespace amy

#endif  //  __AMY_CONNECTOR_HPP__

// vim:ft=cpp ts=4 sw=4 et
