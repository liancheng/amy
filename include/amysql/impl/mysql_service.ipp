#ifndef __AMYSQL_IMPL_MYSQL_SERVICE_IPP__
#define __AMYSQL_IMPL_MYSQL_SERVICE_IPP__

namespace amysql {

inline mysql_service::implementation::implementation() :
    flags(0),
    initialized(false),
    first_result_stored(false),
    cancelation_token(static_cast<void*>(0), noop_deleter())
{}

inline mysql_service::implementation::~implementation() {
    close();
}

inline void mysql_service::implementation::close() {
}

inline void mysql_service::implementation::cancel() {
    this->cancelation_token.reset(static_cast<void*>(0), noop_deleter());
}

inline mysql_service::mysql_service(boost::asio::io_service& io_service) :
    detail::service_base<mysql_service>(io_service)
{}

inline mysql_service::~mysql_service() {
    shutdown_service();
}

inline void mysql_service::shutdown_service() {
}

inline void mysql_service::construct(implementation_type& impl) {
}

inline void mysql_service::destroy(implementation_type& impl) {
}

inline mysql_service::native_type
    mysql_service::native(implementation_type& impl)
{
    return &impl.mysql;
}

}   //  namespace amysql

#endif  //  __AMYSQL_IMPL_MYSQL_SERVICE_IPP__

// vim:ft=cpp ts=4 sw=4 et
