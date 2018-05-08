#ifndef __AMY_IMPL_MARIADB_SERVICE_IPP__
#define __AMY_IMPL_MARIADB_SERVICE_IPP__

#include <amy/detail/mariadb_ops.hpp>

#include <amy/mariadb_options.hpp>
#include <amy/client_flags.hpp>
#include <amy/endpoint_traits.hpp>
#include <amy/noop_deleter.hpp>

#include <functional>

namespace amy {

inline mariadb_service::mariadb_service(AMY_ASIO_NS::io_service& io_service)
    : detail::service_base<mariadb_service>(io_service) {}

inline mariadb_service::~mariadb_service() { shutdown_service(); }

inline void mariadb_service::shutdown_service() {}

inline void mariadb_service::construct(implementation_type& impl) {
  impl.ev_ =
      std::make_unique<AMY_ASIO_NS::posix::stream_descriptor>(get_io_service());
  impl.timer_ = std::make_unique<AMY_ASIO_NS::steady_timer>(get_io_service());
}

inline void mariadb_service::destroy(implementation_type& impl) { close(impl); }

inline mariadb_service::native_type mariadb_service::native(
    implementation_type& impl) {
  return &impl.mysql;
}

inline std::string mariadb_service::error_message(
    implementation_type& impl, AMY_SYSTEM_NS::error_code const& ec) {
  uint32_t ev = static_cast<uint32_t>(ec.value());

  if (::mysql_errno(native(impl)) == ev) {
    return ::mysql_error(native(impl));
  } else {
    return ec.message();
  }
}

inline AMY_SYSTEM_NS::error_code mariadb_service::open(
    implementation_type& impl, AMY_SYSTEM_NS::error_code& ec) {
  namespace ops = amy::detail::mysql_ops;

  ops::clear_error(ec);

  if (!is_open(impl)) {
    impl.initialized = !!ops::mysql_init(&impl.mysql, ec);
  }

  return ec;
}

inline bool mariadb_service::is_open(implementation_type const& impl) const {
  return impl.initialized;
}

inline void mariadb_service::close(implementation_type& impl) {
  if (is_open(impl)) {
    impl.close();
  }
}

template <typename Endpoint>
AMY_SYSTEM_NS::error_code mariadb_service::connect(implementation_type& impl,
    Endpoint const& endpoint, auth_info const& auth,
    std::string const& database, client_flags client_flag,
    AMY_SYSTEM_NS::error_code& ec) {
  if (!is_open(impl)) {
    if (open(impl, ec)) {
      return ec;
    }
  }

  amy::endpoint_traits<Endpoint> traits(endpoint);

  namespace ops = amy::detail::mysql_ops;

  ops::mysql_real_connect(&impl.mysql, traits.host(), auth.user(),
      auth.password(), database.c_str(), traits.port(), traits.unix_socket(),
      client_flag, ec);

  impl.flags = client_flag;

  return ec;
}

template <typename T, typename... Args>
auto make_shared_handler(Args&&... args) {
  auto ptr = std::make_shared<T>(std::forward<Args>(args)...);
  return [ptr]() { (*ptr)(); };
}

template <typename Endpoint, typename ConnectHandler>
void mariadb_service::async_connect(implementation_type& impl,
    Endpoint const& endpoint, auth_info const& auth,
    std::string const& database, client_flags flags, ConnectHandler handler) {
  if (!is_open(impl)) {
    AMY_SYSTEM_NS::error_code ec;
    if (!!open(impl, ec)) {
      this->get_io_service().post(std::bind(handler, ec));
      return;
    }
  }

  AMY_SYSTEM_NS::error_code ec;
  set_option(impl, options::nonblock_default(), ec);
  if (ec) {
    this->get_io_service().post(std::bind(handler, ec));
    return;
  }
  this->get_io_service().post(
      make_shared_handler<connect_handler<Endpoint, ConnectHandler>>(impl,
          endpoint, auth, database, flags, this->get_io_service(), handler));
}

inline AMY_SYSTEM_NS::error_code mariadb_service::query(
    implementation_type& impl, std::string const& stmt,
    AMY_SYSTEM_NS::error_code& ec) {
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

template <typename QueryHandler>
void mariadb_service::async_query(
    implementation_type& impl, std::string const& stmt, QueryHandler handler) {
  if (!is_open(impl)) {
    this->get_io_service().post(
        std::bind(handler, amy::error::not_initialized));
  } else {
    AMY_SYSTEM_NS::error_code ec;
    if (!!open(impl, ec)) {
      this->get_io_service().post(std::bind(handler, ec));
      return;
    }

    this->get_io_service().post(
        make_shared_handler<query_handler<QueryHandler>>(
            impl, stmt, this->get_io_service(), handler));
  }
}

inline bool mariadb_service::has_more_results(
    implementation_type const& impl) const {
  namespace ops = amy::detail::mysql_ops;

  if (!is_open(impl)) {
    return false;
  }

  detail::mysql_handle m = const_cast<detail::mysql_handle>(&impl.mysql);

  bool multi_results =
      impl.flags & (amy::client_multi_results | amy::client_multi_statements);

  return multi_results ? !impl.first_result_stored || ops::mysql_more_results(m)
                       : !impl.first_result_stored && ops::mysql_field_count(m);
}

inline result_set mariadb_service::store_result(
    implementation_type& impl, AMY_SYSTEM_NS::error_code& ec) {
  namespace ops = amy::detail::mysql_ops;

  if (impl.first_result_stored) {
    // Frees the last result set.
    impl.free_result();

    if (!has_more_results(impl)) {
      ec = amy::error::no_more_results;
    } else {
      ops::mysql_next_result(&impl.mysql, ec);
    }
  } else {
    impl.first_result_stored = true;
  }

  if (ec) {
    // If anything went wrong, returns an empty result set.
    return result_set::empty_set(&impl.mysql);
  }

  // Retrieves the next result set.
  impl.last_result.reset(
      ops::mysql_store_result(&impl.mysql, ec), result_set_deleter());

  result_set rs;
  rs.assign(&impl.mysql, impl.last_result, ec);

  return rs;
}

template <typename StoreResultHandler>
void mariadb_service::async_store_result(
    implementation_type& impl, StoreResultHandler handler) {
  if (!is_open(impl)) {
    this->get_io_service().post(std::bind(handler, amy::error::not_initialized,
        result_set::empty_set(&impl.mysql)));
  } else {
    this->get_io_service().post(
        make_shared_handler<store_result_handler<StoreResultHandler>>(
            impl, this->get_io_service(), handler));
  }
}

inline AMY_SYSTEM_NS::error_code mariadb_service::autocommit(
    implementation_type& impl, bool mode, AMY_SYSTEM_NS::error_code& ec) {
  namespace ops = amy::detail::mysql_ops;
  ops::mysql_autocommit(&impl.mysql, mode, ec);
  return ec;
}

inline AMY_SYSTEM_NS::error_code mariadb_service::commit(
    implementation_type& impl, AMY_SYSTEM_NS::error_code& ec) {
  namespace ops = amy::detail::mysql_ops;

  if (!is_open(impl)) {
    ec = amy::error::not_initialized;
    return ec;
  }

  ops::mysql_commit(&impl.mysql, ec);
  return ec;
}

inline AMY_SYSTEM_NS::error_code mariadb_service::rollback(
    implementation_type& impl, AMY_SYSTEM_NS::error_code& ec) {
  namespace ops = amy::detail::mysql_ops;

  if (!is_open(impl)) {
    ec = amy::error::not_initialized;
    return ec;
  }

  ops::mysql_rollback(&impl.mysql, ec);
  return ec;
}

inline uint64_t mariadb_service::affected_rows(implementation_type& impl) {
  return amy::detail::mysql_ops::mysql_affected_rows(&impl.mysql);
}

inline mariadb_service::implementation::implementation()
    : flags(amy::default_flags), initialized(false), first_result_stored(false),
      last_result(static_cast<detail::result_set_handle>(nullptr),
          result_set_deleter()),
      cancelation_token(static_cast<void*>(nullptr), noop_deleter()) {}

inline mariadb_service::implementation::~implementation() { close(); }

inline void mariadb_service::implementation::close() {
  if (this->initialized) {
    amy::detail::mysql_ops::mysql_close(&this->mysql);
    this->initialized = false;
  }

  ev_->release();
  if (timer_) timer_->cancel();

  this->first_result_stored = false;
  free_result();
  cancel();
}

template <typename Option>
AMY_SYSTEM_NS::error_code mariadb_service::set_option(implementation_type& impl,
    Option const& option, AMY_SYSTEM_NS::error_code& ec) {
  namespace ops = detail::mysql_ops;

  if (!is_open(impl)) {
    ec = amy::error::not_initialized;
    return ec;
  }

  ops::mysql_options(native(impl), option.option(), option.data(), ec);
  return ec;
}

inline void mariadb_service::cancel(implementation_type& impl) {
  impl.cancel();
}

inline void mariadb_service::implementation::free_result() {
  this->last_result.reset();
}

inline void mariadb_service::implementation::cancel() {
  this->cancelation_token.reset(static_cast<void*>(nullptr), noop_deleter());
}

template <typename Handler>
mariadb_service::handler_base<Handler>::handler_base(implementation_type& impl,
    AMY_ASIO_NS::io_service& io_service, Handler handler)
    : impl_(impl), cancelation_token_(impl.cancelation_token),
      io_service_(io_service), work_(io_service), handler_(handler) {}

template <typename Handler>
void mariadb_service::handler_base<Handler>::continue_(
    AMY_SYSTEM_NS::error_code ec, int status) {
  if (ec) {
    if (ec != AMY_ASIO_NS::error::operation_aborted)
      this->io_service_.post(std::bind(this->handler_, ec));
    return;
  }

  if (this->cancelation_token_.expired()) {
    this->io_service_.post(
        std::bind(this->handler_, AMY_ASIO_NS::error::operation_aborted));
    return;
  }

  namespace ops = amy::detail::mysql_ops;

  assert(this->mysql_continue_);

  auto self = this->shared_from_this();

  this->await(status, [self, this](AMY_SYSTEM_NS::error_code ec, int status) {
    if (ec) {
      this->io_service_.post(std::bind(this->handler_, ec));
      return;
    }

    if (this->cancelation_token_.expired()) {
      this->io_service_.post(
          std::bind(this->handler_, AMY_ASIO_NS::error::operation_aborted));
      return;
    }

    status = this->mysql_continue_(status, ec);

    if (status == ops::wait_type::finish || ec) {
      this->io_service_.post(std::bind(this->handler_, ec));
    } else {
      this->io_service_.post(
          std::bind(&mariadb_service::handler_base<Handler>::continue_, self,
              ec, status));
    }
  });
}

template <typename Handler>
template <typename ContinueFun, typename>
void mariadb_service::handler_base<Handler>::await(
    int status, ContinueFun continue_fun) {
  namespace ops = amy::detail::mysql_ops;

  auto& ev = *this->impl_.ev_;
  int fd   = -1;
  if (status & ops::wait_type::read_or_write) {
    fd = ops::mysql_get_socket(&this->impl_.mysql);
    if (ev.native_handle() != fd) {
      ev.release();
      ev.assign(fd);
    }
  }

  if (this->impl_.timer_) this->impl_.timer_->cancel();
  if (ev.native_handle() != -1) ev.cancel();

  using boost::asio::posix::descriptor_base;
  using namespace std::placeholders;

  if (status & ops::wait_type::read)
    ev.async_wait(descriptor_base::wait_read,
        std::bind(continue_fun, _1, ops::wait_type::read));

  if (status & ops::wait_type::write)
    ev.async_wait(descriptor_base::wait_write,
        std::bind(continue_fun, _1, ops::wait_type::write));

  if (status & ops::wait_type::timeout) {
    if (!this->impl_.timer_)
      this->impl_.timer_ =
          std::make_unique<AMY_ASIO_NS::steady_timer>(this->io_service_);

    auto timeout = ops::mysql_get_timeout_value(&this->impl_.mysql);

    auto& timer = *this->impl_.timer_;
    timer.cancel();
    timer.expires_after(timeout);
    timer.async_wait(std::bind(continue_fun, _1, ops::wait_type::timeout));
  }
}

template <typename Endpoint, typename ConnectHandler>
mariadb_service::connect_handler<Endpoint, ConnectHandler>::connect_handler(
    implementation_type& impl, Endpoint const& endpoint,
    amy::auth_info const& auth, std::string const& database, client_flags flags,
    AMY_ASIO_NS::io_service& io_service, ConnectHandler handler)
    : handler_base<ConnectHandler>(impl, io_service, handler),
      endpoint_(endpoint), auth_(auth), database_(database), flags_(flags) {}

template <typename Endpoint, typename ConnectHandler>
void mariadb_service::connect_handler<Endpoint, ConnectHandler>::operator()() {
  using namespace amy::error;
  namespace ops = amy::detail::mysql_ops;

  if (this->cancelation_token_.expired()) {
    this->io_service_.post(
        std::bind(this->handler_, AMY_ASIO_NS::error::operation_aborted));
    return;
  }

  amy::endpoint_traits<Endpoint> traits(this->endpoint_);

  AMY_SYSTEM_NS::error_code ec;

  int status = ops::mysql_real_connect_start(&this->result_, &this->impl_.mysql,
      traits.host(), auth_.user(), auth_.password(), database_.c_str(),
      traits.port(), traits.unix_socket(), flags_, ec);

  this->impl_.flags = flags_;

  if (status == ops::wait_type::finish)
    this->io_service_.post(std::bind(this->handler_, ec));
  else {
    this->await(ec, status, [this](int status, AMY_SYSTEM_NS::error_code& ec) {
      return ops::mysql_real_connect_cont(
          &this->result_, &this->impl_.mysql, status, ec);
    });
  }
}

template <typename QueryHandler>
mariadb_service::query_handler<QueryHandler>::query_handler(
    implementation_type& impl, std::string const& stmt,
    AMY_ASIO_NS::io_service& io_service, QueryHandler handler)
    : handler_base<QueryHandler>(impl, io_service, handler), stmt_(stmt) {}

template <typename QueryHandler>
void mariadb_service::query_handler<QueryHandler>::operator()() {
  using namespace amy::error;
  namespace ops = amy::detail::mysql_ops;

  if (this->cancelation_token_.expired()) {
    this->io_service_.post(
        std::bind(this->handler_, AMY_ASIO_NS::error::operation_aborted));
    return;
  }

  this->impl_.free_result();
  this->impl_.first_result_stored = false;

  AMY_SYSTEM_NS::error_code ec;

  int status = ops::mysql_real_query_start(
      &this->result_, &this->impl_.mysql, stmt_.c_str(), stmt_.length(), ec);

  if (status == ops::wait_type::finish)
    this->io_service_.post(std::bind(this->handler_, ec));
  else {
    this->await(ec, status, [this](int status, AMY_SYSTEM_NS::error_code& ec) {
      return ops::mysql_real_query_cont(
          &this->result_, &this->impl_.mysql, status, ec);
    });
  }
}

template <typename NextResultHandler>
mariadb_service::next_result_handler<NextResultHandler>::next_result_handler(
    implementation_type& impl, AMY_ASIO_NS::io_service& io_service,
    NextResultHandler handler)
    : handler_base<NextResultHandler>(impl, io_service, handler) {}

template <typename NextResultHandler>
void mariadb_service::next_result_handler<NextResultHandler>::operator()() {
  namespace ops = amy::detail::mysql_ops;

  if (this->cancelation_token_.expired()) {
    this->io_service_.post(
        std::bind(this->handler_, AMY_ASIO_NS::error::operation_aborted));
    return;
  }

  this->impl_.free_result();
  this->impl_.first_result_stored = false;

  AMY_SYSTEM_NS::error_code ec;

  int status =
      ops::mysql_next_result_start(&this->result_, &this->impl_.mysql, ec);

  if (status == ops::wait_type::finish)
    this->io_service_.post(std::bind(this->handler_, ec));
  else {
    this->await(ec, status, [this](int status, AMY_SYSTEM_NS::error_code& ec) {
      return ops::mysql_next_result_cont(
          &this->result_, &this->impl_.mysql, status, ec);
    });
  }
}

template <typename StoreResultHandler>
mariadb_service::store_result_handler<StoreResultHandler>::store_result_handler(
    implementation_type& impl, AMY_ASIO_NS::io_service& io_service,
    StoreResultHandler handler)
    : handler_base<StoreResultHandler>(impl, io_service, handler) {}

template <typename StoreResultHandler>
void mariadb_service::store_result_handler<StoreResultHandler>::operator()() {
  namespace ops = amy::detail::mysql_ops;

  if (this->cancelation_token_.expired()) {
    this->io_service_.post(
        std::bind(this->handler_, AMY_ASIO_NS::error::operation_aborted,
            result_set::empty_set(&this->impl_.mysql)));
    return;
  }

  AMY_SYSTEM_NS::error_code ec;

  if (this->impl_.first_result_stored) {
    // Frees the last result set.
    this->impl_.free_result();

    mariadb_service& service =
        AMY_ASIO_NS::use_service<mariadb_service>(this->io_service_);

    if (!service.has_more_results(this->impl_)) {
      ec = amy::error::no_more_results;
    } else {
      using self_type =
          mariadb_service::store_result_handler<StoreResultHandler>;
      auto self = std::static_pointer_cast<self_type>(this->shared_from_this());

      auto next = [self, this](AMY_SYSTEM_NS::error_code ec) {
        if (ec) {
          // If anything went wrong, invokes the user-defined handler with the
          // error code and an empty result set.
          this->io_service_.post(std::bind(
              this->handler_, ec, result_set::empty_set(&this->impl_.mysql)));
          return;
        }
        (*self)();
      };

      this->io_service_.post(
          make_shared_handler<next_result_handler<decltype(next)>>(
              this->impl_, this->io_service_, next));
      return;
    }
  } else {
    this->impl_.first_result_stored = true;
  }

  if (ec) {
    // If anything went wrong, invokes the user-defined handler with the
    // error code and an empty result set.
    this->io_service_.post(std::bind(
        this->handler_, ec, result_set::empty_set(&this->impl_.mysql)));
    return;
  }

  int status =
      ops::mysql_store_result_start(&this->result_, &this->impl_.mysql, ec);

  if (ec) {
    // If anything went wrong, invokes the user-defined handler with the
    // error code and an empty result set.
    this->io_service_.post(std::bind(
        this->handler_, ec, result_set::empty_set(&this->impl_.mysql)));
    return;
  }

  if (status == ops::wait_type::finish) {
    // Retrieves the next result set.
    this->impl_.last_result.reset(this->result_, result_set_deleter());

    result_set rs;
    rs.assign(&this->impl_.mysql, this->impl_.last_result, ec);
    this->io_service_.post(std::bind(this->handler_, ec, rs));
  } else {
    this->continue_(ec, status);
  }
}

template <typename StoreResultHandler>
void mariadb_service::store_result_handler<StoreResultHandler>::continue_(
    AMY_SYSTEM_NS::error_code ec, int status) {
  if (ec) {
    if (ec != AMY_ASIO_NS::error::operation_aborted)
      this->io_service_.post(std::bind(
          this->handler_, ec, result_set::empty_set(&this->impl_.mysql)));
    return;
  }

  if (this->cancelation_token_.expired()) {
    this->io_service_.post(
        std::bind(this->handler_, AMY_ASIO_NS::error::operation_aborted,
            result_set::empty_set(&this->impl_.mysql)));
    return;
  }

  namespace ops = amy::detail::mysql_ops;

  using self_type = mariadb_service::store_result_handler<StoreResultHandler>;
  auto self = std::static_pointer_cast<self_type>(this->shared_from_this());

  this->await(status, [self, this](AMY_SYSTEM_NS::error_code ec, int status) {
    if (ec) {
      this->io_service_.post(std::bind(
          this->handler_, ec, result_set::empty_set(&this->impl_.mysql)));
      return;
    }

    if (this->cancelation_token_.expired()) {
      this->io_service_.post(
          std::bind(this->handler_, AMY_ASIO_NS::error::operation_aborted,
              result_set::empty_set(&this->impl_.mysql)));
      return;
    }

    status = ops::mysql_store_result_cont(
        &this->result_, &this->impl_.mysql, status, ec);

    if (ec) {
      // If anything went wrong, invokes the user-defined handler with the
      // error code and an empty result set.
      this->io_service_.post(std::bind(
          this->handler_, ec, result_set::empty_set(&this->impl_.mysql)));
      return;
    }

    if (status == ops::wait_type::finish) {
      // Retrieves the next result set.
      this->impl_.last_result.reset(this->result_, result_set_deleter());

      result_set rs;
      rs.assign(&this->impl_.mysql, this->impl_.last_result, ec);

      this->io_service_.post(std::bind(this->handler_, ec, rs));
    } else {
      this->io_service_.post(
          std::bind(&self_type::continue_, self, ec, status));
    }
  });
}

inline void mariadb_service::result_set_deleter::operator()(void* p) {
  namespace ops = detail::mysql_ops;

  if (!!p) {
    ops::mysql_free_result(static_cast<detail::result_set_handle>(p));
  }
}

} // namespace amy

#endif // __AMY_IMPL_MARIADB_SERVICE_IPP__

// vim:ft=cpp sw=4 ts=4 tw=80 et
