#include "utils.hpp"

#include <amy/connect.hpp>
#include <amy/connector.hpp>
#include <amy/placeholders.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/system/system_error.hpp>

#include <iostream>

global_options opts;

void handle_connect(boost::system::error_code const& ec,
                    amy::connector& connector)
{
    check_error(ec);
    std::cout << "Connected." << std::endl;
}

int main(int argc, char* argv[]) try {
    parse_command_line_options(argc, argv);

    boost::asio::io_service io_service;
    amy::connector connector(io_service);

    using namespace amy::keyword;

    amy::async_connect(_connector = connector,
                       _endpoint  = opts.tcp_endpoint(),
                       _auth      = opts.auth_info(),
                       _database  = opts.schema,
                       _handler   = boost::bind(handle_connect,
                                                amy::placeholders::error,
                                                boost::ref(connector)));

    io_service.run();

    return 0;
} catch (boost::system::system_error const& e) {
    std::cerr
        << boost::format("System error: %1%: %2%")
           % e.code().value() % e.what()
        << std::endl;
} catch (std::exception const& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
}

// vim:ft=cpp sw=4 ts=4 tw=80 et
