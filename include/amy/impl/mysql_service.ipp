#ifndef __AMY_IMPL_MYSQL_SERVICE_IPP__
#define __AMY_IMPL_MYSQL_SERVICE_IPP__

#include <amy/detail/mysql_ops.hpp>
#include <amy/endpoint_traits.hpp>

namespace amy {

inline mysql_service::mysql_service(boost::asio::io_service& io_service) :
    detail::service_base<mysql_service>(io_service),
    work_mutex_(),
    work_io_service_(new boost::asio::io_service),
    work_(new boost::asio::io_service::work(*work_io_service_)),
    work_thread_()
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
mysql_service::native(implementation_type& impl) {
    return &impl.mysql;
}

inline boost::system::error_code
mysql_service::open(implementation_type& impl,
                    boost::system::error_code& ec)
{
    namespace ops = amy::detail::mysql_ops;

    ops::clear_error(ec);

    if (!is_open(impl)) {
        impl.initialized = !!ops::mysql_init(&impl.mysql, ec);
    }

    return ec;
}

inline bool mysql_service::is_open(implementation_type const& impl) const {
    return impl.initialized;
}

inline void mysql_service::close(implementation_type& impl) {
    if (is_open(impl)) {
        impl.close();
    }
}

inline void mysql_service::start_work_thread() {
    boost::mutex::scoped_lock lock(work_mutex_);

    if (!!work_thread_) {
        work_thread_.reset(
                new boost::thread(
                    boost::bind(&boost::asio::io_service::run,
                                work_io_service_.get())));
    }
}

template<typename Endpoint>
boost::system::error_code
mysql_service::connect(implementation_type& impl,
                       Endpoint const& endpoint,
                       auth_info const& auth,
                       std::string const& database,
                       client_flags client_flag,
                       boost::system::error_code& ec)
{
    if (!is_open(impl)) {
        if (open(impl, ec)) {
            return ec;
        }
    }

    amy::endpoint_traits<Endpoint> traits(endpoint);

    namespace ops = amy::detail::mysql_ops;

    ops::mysql_real_connect(&impl.mysql, traits.host(), auth.user(),
                            auth.password(), database.c_str(), traits.port(),
                            traits.unix_socket(), client_flag, ec);

    impl.flags = client_flag;

    return ec;
}

inline boost::system::error_code
mysql_service::query(implementation_type& impl,
                     std::string const& stmt,
                     boost::system::error_code& ec)
{
    if (!is_open(impl)) {
        ec = amy::error::not_initialized;
        return ec;
    }

    impl.free_result();
    impl.first_result_stored = false;

    namespace ops = detail::mysql_ops;

    ops::mysql_real_query(&impl.mysql, stmt.c_str(), stmt.length(), ec);
    return ec;
}

struct noop_deleter {
    void operator()(void*) {
        // no-op
    }

};  //  struct noop_deleter

void mysql_service::result_set_deleter::operator()(void* p) {
    namespace ops = detail::mysql_ops;

    if (!!p) {
        ops::mysql_free_result(static_cast<detail::result_set_handle>(p));
    }
}

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
    if (this->initialized) {
        amy::detail::mysql_ops::mysql_close(&this->mysql);
        this->initialized = false;
    }

    // TODO release the result set

    cancel();
}

inline void mysql_service::implementation::free_result() {
    // TODO
}

inline void mysql_service::implementation::cancel() {
    this->cancelation_token.reset(static_cast<void*>(0), noop_deleter());
}

}   //  namespace amy

#endif  //  __AMY_IMPL_MYSQL_SERVICE_IPP__

// vim:ft=cpp ts=4 sw=4 et
