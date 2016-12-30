#include "utils.hpp"

#include <amy/asio.hpp>
#include <amy/connector.hpp>
#include <amy/placeholders.hpp>

#include <boost/format.hpp>

#include <iostream>

global_options opts;

void handle_store_result(AMY_SYSTEM_NS::error_code const& ec,
                         amy::result_set rs,
                         amy::connector& connector)
{
    check_error(ec);

    std::cout
        << boost::format("Field count: %1%, "
                         "result set size: %2%, "
                         "affected rows: %3%, contents:")
           % rs.field_count() % rs.size() % rs.affected_rows()
        << std::endl;

    const auto& fields_info = rs.fields_info();

    for (const auto& row : rs) {
        std::cout
            << boost::format("%1%: %2%, %3%: %4%")
               % fields_info[0].name() % row[0].as<std::string>()
               % fields_info[1].name() % row[1].as<amy::sql_bigint>()
            << std::endl;
    }
}

void handle_query(AMY_SYSTEM_NS::error_code const& ec,
                  amy::connector& connector)
{
    check_error(ec);

    connector.async_store_result(
            std::bind(handle_store_result,
                      amy::placeholders::error,
                      amy::placeholders::result_set,
                      std::ref(connector)));
}

void handle_connect(AMY_SYSTEM_NS::error_code const& ec,
                    amy::connector& connector)
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

int main(int argc, char* argv[]) try {
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
} catch (AMY_SYSTEM_NS::system_error const& e) {
    std::cerr
        << boost::format("System error: %1%: %2%")
           % e.code().value() % e.what()
        << std::endl;
} catch (std::exception const& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
}

// vim:ft=cpp sw=4 ts=4 tw=80 et
