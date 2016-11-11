#include "utils.hpp"

#include <amy/connector.hpp>
#include <amy/execute.hpp>
#include <amy/placeholders.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/system/system_error.hpp>

#include <iostream>

global_options opts;

void handle_execute(boost::system::error_code const& ec,
                    uint64_t affected_rows,
                    amy::connector& connector)
{
    if (!!ec) {
        std::cerr
            << boost::format("Statement execution failed: %1% - %2%")
               % ec.value() % ec.message()
            << std::endl;
        return;
    }

    std::cout
        << boost::format("Affected rows: %1%") % affected_rows
        << std::endl;

    return;
}

void handle_connect(boost::system::error_code const& ec,
                    amy::connector& connector)
{
    if (!!ec) {
        std::cerr
            << boost::format("Connection error: %1% - %2%")
               % ec.value() % ec.message()
            << std::endl;
        return;
    }

    std::cout << "Connected" << std::endl;

    amy::async_execute(connector,
                       read_from_stdin(),
                       boost::bind(handle_execute,
                                   amy::placeholders::error,
                                   amy::placeholders::affected_rows,
                                   boost::ref(connector)));
}

int main(int argc, char* argv[]) {
    parse_command_line_options(argc, argv);

    boost::asio::io_service io_service;
    amy::connector connector(io_service);

    connector.async_connect(opts.tcp_endpoint(),
                            opts.auth_info(),
                            opts.schema,
                            amy::default_flags,
                            boost::bind(handle_connect,
                                        amy::placeholders::error,
                                        boost::ref(connector)));

    io_service.run();

    return 0;
}
