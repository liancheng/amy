#ifndef __AMY_EXAMPLE_UTILS_HPP__
#define __AMY_EXAMPLE_UTILS_HPP__

#include <amy/auth_info.hpp>

#include <boost/asio/ip/tcp.hpp>
#include <boost/optional.hpp>

/// Global options.
struct global_options {
    /// MySQL server host.
    std::string host;

    /// MySQL server port.
    unsigned int port;

    /// Login user.
    std::string user;

    /// Login password.
    std::string password;

    /// Default schema to use.
    std::string schema;

    boost::asio::ip::tcp::endpoint tcp_endpoint() const;

    amy::auth_info auth_info() const;

}; // struct global_options

extern global_options opts;

void parse_command_line_options(int argc, char* argv[]);

std::string read_from_stdin();

#endif // __AMY_EXAMPLE_UTILS_HPP__
