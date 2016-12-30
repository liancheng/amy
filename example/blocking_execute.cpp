#include "utils.hpp"

#include <amy/asio.hpp>
#include <amy/connector.hpp>
#include <amy/execute.hpp>

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
                      amy::default_flags);

    // Executes an arbitrary SQL statement read from stdin.
    auto affected_rows = amy::execute(connector, read_from_stdin());
    std::cout << "Affected rows: " << affected_rows << std::endl;

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
