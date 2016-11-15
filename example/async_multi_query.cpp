#include "utils.hpp"

#include <amy/connector.hpp>
#include <amy/placeholders.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/system/system_error.hpp>

#include <iostream>

global_options opts;

void handle_store_result(boost::system::error_code const& ec,
                         amy::result_set rs,
                         amy::connector& connector)
{
    check_error(ec);

    // Prints result sets of each executed query.
    std::cout
        << boost::format("Affected rows: %1%, "
                         "field count: %2%, "
                         "result set size %3%")
           % rs.affected_rows() % rs.field_count() % rs.size()
        << std::endl;

    auto out = std::ostream_iterator<amy::row>(std::cout, "\n");
    std::copy(rs.begin(), rs.end(), out);

    if (connector.has_more_results()) {
        connector.async_store_result(
                boost::bind(handle_store_result,
                            amy::placeholders::error,
                            amy::placeholders::result_set,
                            boost::ref(connector)));
    }
}

void handle_query(boost::system::error_code const& ec,
                  amy::connector& connector)
{
    check_error(ec);
    connector.async_store_result(
            boost::bind(handle_store_result,
                        amy::placeholders::error,
                        amy::placeholders::result_set,
                        boost::ref(connector)));
}

void handle_connect(boost::system::error_code const& ec,
                    amy::connector& connector)
{
    check_error(ec);

    // Executes multiple ';'-separated SQL queries read from stdin.
    connector.async_query(read_from_stdin(),
                          boost::bind(handle_query,
                                      amy::placeholders::error,
                                      boost::ref(connector)));
}

int main(int argc, char* argv[]) try {
    parse_command_line_options(argc, argv);

    boost::asio::io_service io_service;
    amy::connector connector(io_service);

    connector.async_connect(opts.tcp_endpoint(),
                            opts.auth_info(),
                            opts.schema,
                            amy::client_multi_statements,
                            boost::bind(handle_connect,
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
