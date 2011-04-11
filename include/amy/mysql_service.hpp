#ifndef __AMY_MYSQL_SERVICE_HPP__
#define __AMY_MYSQL_SERVICE_HPP__

#include <amy/detail/mysql_lib_init.hpp>
#include <amy/detail/mysql_types.hpp>
#include <amy/detail/service_base.hpp>
#include <amy/endpoint_traits.hpp>

namespace amy {

class mysql_service : public detail::service_base<mysql_service> {
public:
    struct implementation;

    typedef implementation implementation_type;

    typedef detail::mysql_handle native_type;

    explicit mysql_service(boost::asio::io_service& io_service);

    ~mysql_service();

    void shutdown_service();

    void construct(implementation_type& impl);

    void destroy(implementation_type& impl);

    native_type native(implementation_type& impl);

private:
    detail::mysql_lib_init mysql_lib_init_;

};  //  class mysql_service

struct noop_deleter {
    void operator()(void*) {
        // no-op
    }

};  //  struct noop_deleter

struct mysql_service::implementation {
    detail::mysql_type mysql;

    detail::client_flags flags;

    bool initialized;

    bool first_result_stored;

    boost::shared_ptr<void> cancelation_token;

    explicit implementation();

    ~implementation();

    void close();

    void cancel();

};  //  struct mysql_service::implementation

}   //  namespace amy

#endif  //  __AMY_MYSQL_SERVICE_HPP__

#include <amy/impl/mysql_service.ipp>

// vim:ft=cpp ts=4 sw=4 et
