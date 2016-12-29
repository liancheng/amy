#ifndef __AMY_ASIO_HPP__
#define __AMY_ASIO_HPP__

#if defined(USE_BOOST_ASIO)

#include <boost/asio/basic_io_object.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/local/stream_protocol.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/system/system_error.hpp>

#define AMY_ASIO_NS ::boost::asio
#define AMY_SYSTEM_NS ::boost::system

#else

#include <asio/basic_io_object.hpp>
#include <asio/io_service.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/local/stream_protocol.hpp>
#include <asio/placeholders.hpp>

#include <system_error>

#define AMY_ASIO_NS ::asio
#define AMY_SYSTEM_NS ::std

#endif

#endif // __AMY_ASIO_HPP__
