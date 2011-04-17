#ifndef __AMY_MYSQL_SERVICE_HPP__
#define __AMY_MYSQL_SERVICE_HPP__

#include <amy/detail/mysql_lib_init.hpp>
#include <amy/detail/mysql_types.hpp>
#include <amy/detail/service_base.hpp>
#include <amy/endpoint_traits.hpp>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

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

    boost::system::error_code open(implementation_type& impl,
                                   boost::system::error_code& ec);

    bool is_open(implementation_type const& impl) const;

    void close(implementation_type& impl);

    template<typename Endpoint>
    boost::system::error_code connect(implementation_type& impl,
                                      Endpoint const& endpoint,
                                      auth_info const& auth,
                                      std::string const& database,
                                      client_flags client_flag,
                                      boost::system::error_code& ec);

    boost::system::error_code query(implementation_type& impl,
                                    std::string const& stmt,
                                    boost::system::error_code& ec);

private:
    struct result_set_deleter;

    detail::mysql_lib_init mysql_lib_init_;
    boost::mutex work_mutex_;
    boost::scoped_ptr<boost::asio::io_service> work_io_service_;
    boost::scoped_ptr<boost::asio::io_service::work> work_;
    boost::shared_ptr<boost::thread> work_thread_;

    void start_work_thread();

};  //  class mysql_service

/// The underlying MySQL client connector implementation.
struct mysql_service::implementation {
    /// The native MySQL connection handle.
    detail::mysql_type mysql;

    /// Client flags.
    client_flags flags;

    /// Indicates whether the connection handle is initialized.
    bool initialized;

    /// Indicates whether the first result set of the last query is already
    /// stored.
    bool first_result_stored;

    /// The last stored result set of the last query.
    boost::shared_ptr<detail::result_set_type> last_result;

    /// Token used to cancel unfinished asynchronous operations.
    boost::shared_ptr<void> cancelation_token;

    /// Constructor.
    /**
     * The native connection handle is neither opened nor initialized within
     * constructor.
     */
    explicit implementation();

    /// Destructor.
    /**
     * Simply call \c close.
     */
    ~implementation();

    /// Closes the connection and revokes result set resource if any.
    void close();

    /// Frees the result set resource if any.
    void free_result();

    /// Cancels unfinished asynchronous operations.
    void cancel();

};  //  struct mysql_service::implementation

struct mysql_service::result_set_deleter {
    void operator()(void* p);

};  //  struct mysql_service::result_set_deleter

}   //  namespace amy

#endif  //  __AMY_MYSQL_SERVICE_HPP__

#include <amy/impl/mysql_service.ipp>

// vim:ft=cpp ts=4 sw=4 et
