#ifndef __AMY_DETAIL_SERVICE_BASE_HPP__
#define __AMY_DETAIL_SERVICE_BASE_HPP__

#include <boost/asio/io_service.hpp>

namespace amy {
namespace detail {

// TODO Implements our own `service_base` class.
//
// Everything inside the `detail` namespace is not part of the public API.
using boost::asio::detail::service_base;

} // namespace detail
} // namespace amy

#endif // __AMY_DETAIL_SERVICE_BASE_HPP__

// vim:ft=cpp sw=4 ts=4 tw=80 et
