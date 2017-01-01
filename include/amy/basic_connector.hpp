#ifndef __AMY_BASIC_CONNECTOR_HPP__
#define __AMY_BASIC_CONNECTOR_HPP__

#include <amy/detail/throw_error.hpp>

#include <amy/asio.hpp>
#include <amy/auth_info.hpp>
#include <amy/client_flags.hpp>
#include <amy/result_set.hpp>

namespace amy {

/// Provides MySQL client functionalities.
template<typename MySQLService>
class basic_connector : public AMY_ASIO_NS::basic_io_object<MySQLService> {
public:
    /// The type of the service that provides actual MySQL client
    /// functionalities.
    typedef MySQLService service_type;

    /// The native MySQL connection handle type.
    typedef typename service_type::native_type native_type;

    /// Constructs a \c basic_connector without opening it.
    explicit basic_connector(AMY_ASIO_NS::io_service& io_service) :
        AMY_ASIO_NS::basic_io_object<MySQLService>(io_service)
    {}

    native_type native() {
        return this->service.native(this->implementation);
    }

    std::string error_message(AMY_SYSTEM_NS::error_code const& ec) {
        return this->service.error_message(this->implementation, ec);
    }

    void open() {
        AMY_SYSTEM_NS::error_code ec;
        detail::throw_error(open(ec), &(this->implementation.mysql));
    }

    AMY_SYSTEM_NS::error_code open(AMY_SYSTEM_NS::error_code& ec) {
        return this->service.open(this->implementation, ec);
    }

    bool is_open() const {
        return this->service.is_open(this->implementation);
    }

    void close() {
        return this->service.close(this->implementation);
    }

    template<typename Option>
    void set_option(Option const& option) {
        AMY_SYSTEM_NS::error_code ec;
        detail::throw_error(set_option(this->implementation, option, ec),
                            &(this->implementation.mysql));
    }

    template<typename Option>
    AMY_SYSTEM_NS::error_code set_option(Option const& option,
                                         AMY_SYSTEM_NS::error_code& ec)
    {
        this->service.set_option(this->implementation, option, ec);
        detail::throw_error(ec, &(this->implementation.mysql));
    }

    void cancel() {
        this->service.cancel(this->implementation);
    }

    template<typename Endpoint>
    void connect(Endpoint const& endpoint,
                 auth_info const& auth,
                 std::string const& database,
                 client_flags flags)
    {
        AMY_SYSTEM_NS::error_code ec;
        detail::throw_error(connect(endpoint, auth, database, flags, ec),
                            &(this->implementation.mysql));
    }

    template<typename Endpoint>
    AMY_SYSTEM_NS::error_code connect(Endpoint const& endpoint,
                                      auth_info const& auth,
                                      std::string const& database,
                                      client_flags flags,
                                      AMY_SYSTEM_NS::error_code& ec)
    {
        return this->service.connect(this->implementation,
                                     endpoint, auth, database, flags, ec);
    }

    template<typename Endpoint, typename ConnectHandler>
    void async_connect(Endpoint const& endpoint,
                       auth_info const& auth,
                       std::string const& database,
                       client_flags flags,
                       ConnectHandler handler)
    {
        return this->service.async_connect(
                this->implementation, endpoint, auth, database, flags, handler);
    }

    void query(std::string const& stmt) {
        AMY_SYSTEM_NS::error_code ec;
        detail::throw_error(query(stmt, ec), &(this->implementation.mysql));
    }

    AMY_SYSTEM_NS::error_code query(std::string const& stmt,
                                    AMY_SYSTEM_NS::error_code& ec)
    {
        return this->service.query(this->implementation, stmt, ec);
    }

    template<typename QueryHandler>
    void async_query(std::string const& stmt, QueryHandler handler) {
        this->service.async_query(this->implementation, stmt, handler);
    }

    bool has_more_results() const {
        return this->service.has_more_results(this->implementation);
    }

    result_set store_result() {
        AMY_SYSTEM_NS::error_code ec;
        result_set rs = store_result(ec);
        detail::throw_error(ec, &(this->implementation.mysql));
        return rs;
    }

    result_set store_result(AMY_SYSTEM_NS::error_code& ec) {
        return this->service.store_result(this->implementation, ec);
    }

    template<typename StoreResultHandler>
    void async_store_result(StoreResultHandler handler) {
        this->service.async_store_result(this->implementation, handler);
    }

    void autocommit(bool mode) {
        AMY_SYSTEM_NS::error_code ec;
        detail::throw_error(autocommit(mode, ec),
                            &(this->implementation.mysql));
    }

    AMY_SYSTEM_NS::error_code autocommit(bool mode,
                                         AMY_SYSTEM_NS::error_code& ec)
    {
        this->service.autocommit(this->implementation, mode, ec);
        return ec;
    }

    void commit() {
        AMY_SYSTEM_NS::error_code ec;
        detail::throw_error(commit(ec), &(this->implementation.mysql));
    }

    AMY_SYSTEM_NS::error_code commit(AMY_SYSTEM_NS::error_code& ec) {
        this->service.commit(this->implementation, ec);
        return ec;
    }

    void rollback() {
        AMY_SYSTEM_NS::error_code ec;
        detail::throw_error(rollback(ec), &(this->implementation.mysql));
    }

    AMY_SYSTEM_NS::error_code rollback(AMY_SYSTEM_NS::error_code& ec) {
        this->service.rollback(this->implementation, ec);
        return ec;
    }

    uint64_t affected_rows() {
        return this->service.affected_rows(this->implementation);
    }

}; // class basic_connector

} // namespace amy

#endif // __AMY_BASIC_CONNECTOR_HPP__

// vim:ft=cpp sw=4 ts=4 tw=80 et
