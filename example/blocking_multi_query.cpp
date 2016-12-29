#include "utils.hpp"

#include <amy/asio.hpp>
#include <amy/connector.hpp>

#include <boost/format.hpp>

#include <algorithm>
#include <iostream>
#include <iterator>

global_options opts;

int main(int argc, char* argv[]) try {
    parse_command_line_options(argc, argv);

    AMY_ASIO_NS::io_service io_service;
    amy::connector connector(io_service);

    connector.connect(opts.tcp_endpoint(),
                      opts.auth_info(),
                      opts.schema,
                      amy::client_multi_statements);

    // Executes multiple ';'-separated SQL queries read from stdin.
    connector.query(read_from_stdin());

    auto first = amy::results_iterator(connector);
    auto last = amy::results_iterator();

    // Prints result sets of each executed query.
    std::for_each(first, last, [](const amy::result_set& rs) {
        std::cout
            << boost::format("Affected rows: %1%, "
                             "field count: %2%, "
                             "result set size %3%")
               % rs.affected_rows() % rs.field_count() % rs.size()
            << std::endl;

        auto out = std::ostream_iterator<amy::row>(std::cout, "\n");
        std::copy(rs.begin(), rs.end(), out);
    });

    return 0;
} catch (AMY_SYSTEM_NS::system_error const& e) {
    std::cerr
        << boost::format("System error: %1%: %2%")
           % e.code().value() % e.what()
        << std::endl;
} catch (std::exception const& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
}

// vim:ft=cpp sw=4 ts=4 tw=80 et
