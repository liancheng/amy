#include "utils.hpp"

#include <amy/connector.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/format.hpp>
#include <boost/system/system_error.hpp>

#include <algorithm>
#include <iostream>
#include <iterator>

global_options opts;

int main(int argc, char* argv[]) try {
    parse_command_line_options(argc, argv);

    boost::asio::io_service io_service;
    amy::connector connector(io_service);

    connector.connect(opts.tcp_endpoint(),
                      opts.auth_info(),
                      opts.schema,
                      amy::client_multi_statements);

    connector.query(read_from_stdin());

    auto first = amy::results_iterator(connector);
    auto last = amy::results_iterator();

    std::for_each(first, last, [](amy::result_set& rs) {
        if (rs.empty()) {
            std::cout << "Affected rows: " << rs.affected_rows() << std::endl;
        } else {
            std::cout
                << boost::format("Field count: %1%, result set size: %2%")
                   % rs.field_count() % rs.size()
                << std::endl;

            auto out = std::ostream_iterator<amy::row>(std::cout, "\n");
            std::copy(rs.begin(), rs.end(), out);
        }
    });

    return 0;
} catch (boost::system::system_error const& e) {
    std::cerr
        << boost::format("System error: %1%: %2%")
           % e.code().value() % e.what()
        << std::endl;
} catch (std::exception const& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
}
