#include "utils.hpp"

#include <amy/mariadb_connector.hpp>
#include <amy/placeholders.hpp>

#include <functional>
#include <iostream>

global_options opts;

void handle_store_result(AMY_SYSTEM_NS::error_code const& ec,
                         amy::result_set rs,
                         amy::mariadb_connector& connector)
{
    check_error(ec);

    std::cout
        << "Affected rows: " << rs.affected_rows()
        << ", field count: " << rs.field_count()
        << ", result set size: " << rs.size()
        << std::endl;

    auto out = std::ostream_iterator<amy::row>(std::cout, "\n");
    std::copy(rs.begin(), rs.end(), out);

    if (connector.has_more_results()) {
        connector.async_store_result(
                std::bind(handle_store_result,
                          amy::placeholders::error,
                          amy::placeholders::result_set,
                          std::ref(connector)));
    }
}

void handle_query(AMY_SYSTEM_NS::error_code const& ec,
                  amy::mariadb_connector& connector)
{
    check_error(ec);

    connector.async_store_result(
            std::bind(handle_store_result,
                      amy::placeholders::error,
                      amy::placeholders::result_set,
                      std::ref(connector)));
}

void handle_connect(AMY_SYSTEM_NS::error_code const& ec,
                    amy::mariadb_connector& connector)
{
    check_error(ec);

    // Executes multiple ';'-separated SQL queries read from stdin.
    connector.async_query(read_from_stdin(),
                          std::bind(handle_query,
                                    amy::placeholders::error,
                                    std::ref(connector)));
}

int main(int argc, char* argv[]) {
    parse_command_line_options(argc, argv);

    AMY_ASIO_NS::io_service io_service;
    amy::mariadb_connector connector(io_service);

    connector.async_connect(opts.tcp_endpoint(),
                            opts.auth_info(),
                            opts.schema,
                            amy::client_multi_statements,
                            std::bind(handle_connect,
                                      amy::placeholders::error,
                                      std::ref(connector)));

    try {
        io_service.run();
    } catch (AMY_SYSTEM_NS::system_error const& e) {
        report_system_error(e);
    } catch (std::exception const& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}

// vim:ft=cpp sw=4 ts=4 tw=80 et
