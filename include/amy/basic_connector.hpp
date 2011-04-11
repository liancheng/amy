#ifndef __AMY_BASIC_CONNECTOR_HPP__
#define __AMY_BASIC_CONNECTOR_HPP__

#include <amy/auth_info.hpp>
#include <amy/detail/throw_error.hpp>

#include <boost/config.hpp>
#include <boost/asio/basic_io_object.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/system/system_error.hpp>

namespace amy {

template<typename MySQLService>
class basic_connector : public boost::asio::basic_io_object<MySQLService> {
public:
    typedef MySQLService service_type;

    typedef typename service_type::native_type native_type;

    explicit basic_connector(boost::asio::io_service& io_service) :
        boost::asio::basic_io_object<MySQLService>(io_service)
    {}

    native_type native() {
        return this->service.native(this->implementation);
    }

private:

};  //  class basic_connector

}   //  namespace amy

#endif  //  __AMY_BASIC_CONNECTOR_HPP__

// vim:ft=cpp ts=4 sw=4 et
