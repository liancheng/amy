#ifndef __AMY_DETAIL_EXECUTE_HANDLER_HPP__
#define __AMY_DETAIL_EXECUTE_HANDLER_HPP__

#include <amy/basic_connector.hpp>

#include <boost/system/error_code.hpp>

namespace amy {
namespace detail {

template<
    typename MySQLService,
    typename ExecuteHandler
>
class execute_handler {
public:
    typedef void result_type;

    execute_handler(basic_connector<MySQLService>& connector,
                    ExecuteHandler handler)
      : connector(connector),
        handler(handler)
    {}

    void operator()(boost::system::error_code const& ec) {
        handler(ec, !!ec ? connector.affected_rows() : 0u);
    }

private:
    basic_connector<MySQLService>& connector;
    ExecuteHandler handler;

};  //  class execute_handler

}   //  namespace detail
}   //  namespace amy

#endif  //  __AMY_DETAIL_EXECUTE_HANDLER_HPP__
