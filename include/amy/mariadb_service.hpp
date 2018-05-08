#ifndef __AMY_MARIADB_SERVICE_HPP__
#define __AMY_MARIADB_SERVICE_HPP__

#include <amy/detail/mysql_lib_init.hpp>
#include <amy/detail/mysql_types.hpp>
#include <amy/detail/service_base.hpp>

#include <amy/auth_info.hpp>
#include <amy/endpoint_traits.hpp>
#include <amy/result_set.hpp>

#include <boost/asio/posix/stream_descriptor.hpp>

#include <memory>

namespace amy {

class mariadb_service : public detail::service_base<mariadb_service> {
public:
  struct implementation;

  template <typename Handler>
  class handler_base;

  template <typename Endpoint, typename ConnectHandler>
  class connect_handler;

  template <typename QueryHandler>
  class query_handler;

  template <typename NextResultHandler>
  class next_result_handler;

  template <typename StoreResultHandler>
  class store_result_handler;

  typedef implementation implementation_type;

  typedef detail::mysql_handle native_type;

  explicit mariadb_service(AMY_ASIO_NS::io_service& io_service);

  ~mariadb_service();

  void shutdown_service();

  void construct(implementation_type& impl);

  void destroy(implementation_type& impl);

  native_type native(implementation_type& impl);

  std::string error_message(
      implementation_type& impl, AMY_SYSTEM_NS::error_code const& ec);

  AMY_SYSTEM_NS::error_code open(
      implementation_type& impl, AMY_SYSTEM_NS::error_code& ec);

  bool is_open(implementation_type const& impl) const;

  void close(implementation_type& impl);

  template <typename Option>
  AMY_SYSTEM_NS::error_code set_option(implementation_type& impl,
      Option const& option, AMY_SYSTEM_NS::error_code& ec);

  void cancel(implementation_type& impl);

  template <typename Endpoint>
  AMY_SYSTEM_NS::error_code connect(implementation_type& impl,
      Endpoint const& endpoint, auth_info const& auth,
      std::string const& database, client_flags client_flag,
      AMY_SYSTEM_NS::error_code& ec);

  template <typename Endpoint, typename ConnectHandler>
  void async_connect(implementation_type& impl, Endpoint const& endpoint,
      auth_info const& auth, std::string const& database, client_flags flags,
      ConnectHandler handler);

  AMY_SYSTEM_NS::error_code query(implementation_type& impl,
      std::string const& stmt, AMY_SYSTEM_NS::error_code& ec);

  template <typename QueryHandler>
  void async_query(
      implementation_type& impl, std::string const& stmt, QueryHandler handler);

  bool has_more_results(implementation_type const& impl) const;

  result_set store_result(
      implementation_type& impl, AMY_SYSTEM_NS::error_code& ec);

  template <typename StoreResultHandler>
  void async_store_result(
      implementation_type& impl, StoreResultHandler handler);

  AMY_SYSTEM_NS::error_code autocommit(
      implementation_type& impl, bool mode, AMY_SYSTEM_NS::error_code& ec);

  AMY_SYSTEM_NS::error_code commit(
      implementation_type& impl, AMY_SYSTEM_NS::error_code& ec);

  AMY_SYSTEM_NS::error_code rollback(
      implementation_type& impl, AMY_SYSTEM_NS::error_code& ec);

  uint64_t affected_rows(implementation_type& impl);

private:
  struct result_set_deleter;

  detail::mysql_lib_init mysql_lib_init_;
}; // class mariadb_service

/// The underlying MySQL client connector implementation.
struct mariadb_service::implementation {
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
  std::shared_ptr<detail::result_set_type> last_result;

  /// Token used to cancel unfinished asynchronous operations.
  std::shared_ptr<void> cancelation_token;

  std::unique_ptr<AMY_ASIO_NS::posix::stream_descriptor> ev_;
  std::unique_ptr<AMY_ASIO_NS::steady_timer> timer_;

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

}; // struct mariadb_service::implementation

template <typename Handler>
class mariadb_service::handler_base
    : public std::enable_shared_from_this<
          mariadb_service::handler_base<Handler>> {
public:
  explicit handler_base(implementation_type& impl,
      AMY_ASIO_NS::io_service& io_service, Handler handler);

private:
  std::function<int(int, AMY_SYSTEM_NS::error_code&)> mysql_continue_;
  void continue_(AMY_SYSTEM_NS::error_code, int);

protected:
  implementation_type& impl_;
  std::weak_ptr<void> cancelation_token_;
  AMY_ASIO_NS::io_service& io_service_;
  AMY_ASIO_NS::io_service::work work_;
  Handler handler_;

  void await(AMY_SYSTEM_NS::error_code ec, int status,
      std::function<int(int, AMY_SYSTEM_NS::error_code&)> mysql_continue) {
    mysql_continue_ = std::move(mysql_continue);
    continue_(ec, status);
  }

  void await(int status,
      std::function<void(AMY_SYSTEM_NS::error_code, int)> continue_fun_);

}; // class mariadb_service::handler_base

template <typename Endpoint, typename ConnectHandler>
class mariadb_service::connect_handler : public handler_base<ConnectHandler> {
public:
  explicit connect_handler(implementation_type& impl, Endpoint const& endpoint,
      amy::auth_info const& auth, std::string const& database,
      client_flags flags, AMY_ASIO_NS::io_service& io_service,
      ConnectHandler handler);

  void operator()();

private:
  Endpoint endpoint_;
  amy::auth_info auth_;
  std::string database_;
  client_flags flags_;
  detail::mysql_type* result_ = nullptr;

}; // class mariadb_service::connect_handler

template <typename QueryHandler>
class mariadb_service::query_handler : public handler_base<QueryHandler> {
public:
  explicit query_handler(implementation_type& impl, std::string const& stmt,
      AMY_ASIO_NS::io_service& io_service, QueryHandler handler);

  void operator()();

private:
  std::string stmt_;
  int result_ = -1;

}; // class mariadb_service::query_handler

template <typename NextResultHandler>
class mariadb_service::next_result_handler
    : public handler_base<NextResultHandler> {
public:
  explicit next_result_handler(implementation_type& impl,
      AMY_ASIO_NS::io_service& io_service, NextResultHandler handler);

  void operator()();

private:
  int result_ = -1;

}; // class mariadb_service::next_result_handler

template <typename StoreResultHandler>
class mariadb_service::store_result_handler
    : public handler_base<StoreResultHandler> {
public:
  explicit store_result_handler(implementation_type& impl,
      AMY_ASIO_NS::io_service& io_service, StoreResultHandler handler);

  void operator()();

private:
  void continue_(AMY_SYSTEM_NS::error_code ec, int status);
  detail::result_set_type* result_ = nullptr;

}; // class mariadb_service::store_result_handler

struct mariadb_service::result_set_deleter {
  void operator()(void* p);

}; // struct mariadb_service::result_set_deleter

} // namespace amy

#endif // __AMY_MYSQL_SERVICE_HPP__

#include <amy/impl/mariadb_service.ipp>

// vim:ft=cpp sw=4 ts=4 tw=80 et
