#include "utils.hpp"

#include <amy/connector.hpp>
#include <amy/execute.hpp>
#include <amy/placeholders.hpp>

#include <boost/format.hpp>

#include <functional>
#include <iostream>

global_options opts;

void handle_execute(AMY_SYSTEM_NS::error_code const& ec,
                    uint64_t affected_rows,
                    amy::connector& connector)
{
    check_error(ec);
    std::cout << "Affected rows: " << affected_rows << std::endl;
    return;
}

void handle_connect(AMY_SYSTEM_NS::error_code const& ec,
                    amy::connector& connector)
{
    check_error(ec);

    // Executes an arbitrary SQL statement read from stdin.
    amy::async_execute(connector,
                       read_from_stdin(),
                       std::bind(handle_execute,
                                 amy::placeholders::error,
                                 amy::placeholders::affected_rows,
                                 std::ref(connector)));
}

int main(int argc, char* argv[]) {
    parse_command_line_options(argc, argv);

    AMY_ASIO_NS::io_service io_service;
    amy::connector connector(io_service);

    connector.async_connect(opts.tcp_endpoint(),
                            opts.auth_info(),
                            opts.schema,
                            amy::default_flags,
                            std::bind(handle_connect,
                                      amy::placeholders::error,
                                      std::ref(connector)));

    io_service.run();

    return 0;
}

// vim:ft=cpp sw=4 ts=4 tw=80 et
