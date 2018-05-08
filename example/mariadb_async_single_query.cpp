#include "utils.hpp"

#include <amy/mariadb_connector.hpp>
#include <amy/placeholders.hpp>

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

    const auto& fields_info = rs.fields_info();

    for (const auto& row : rs) {
        std::cout
            << fields_info[0].name() << ": " << row[0].as<std::string>() << ", "
            << fields_info[1].name() << ": " << row[1].as<amy::sql_bigint>()
            << std::endl;
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

    auto statement =
        "SELECT character_set_name, maxlen\n"
        "FROM information_schema.character_sets\n"
        "WHERE character_set_name LIKE 'latin%'";

    connector.async_query(statement,
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
                            amy::default_flags,
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
