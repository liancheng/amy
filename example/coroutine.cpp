#include "utils.hpp"

#include <amy/connector.hpp>
#include <amy/placeholders.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/system/system_error.hpp>

#include <iostream>

global_options opts;

void coroutine(amy::connector& connector, boost::asio::yield_context yield) {
    connector.async_connect(
            opts.tcp_endpoint(), opts.auth_info(), opts.schema,
            amy::client_multi_statements, yield);

    connector.async_query(read_from_stdin(), yield);

    do {
        auto rs = connector.async_store_result(yield);
        auto out = std::ostream_iterator<amy::row>(std::cout, "\n");
        std::copy(rs.begin(), rs.end(), out);
    } while(connector.has_more_results());
}

int main(int argc, char* argv[]) try {
    parse_command_line_options(argc, argv);

    boost::asio::io_service io_service;
    amy::connector connector(io_service);

    boost::asio::spawn(
            io_service,
            boost::bind(coroutine,
                        boost::ref(connector),
                        amy::placeholders::yield()));

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
