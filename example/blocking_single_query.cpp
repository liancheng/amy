#include "utils.hpp"

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
                      amy::default_flags);

    std::string statement =
        "SELECT character_set_name, maxlen\n"
        "FROM information_schema.character_sets\n"
        "WHERE character_set_name LIKE 'latin%'";

    connector.query(statement);

    amy::result_set rs = connector.store_result();

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

    return 0;
} catch (AMY_SYSTEM_NS::system_error const& e) {
    report_system_error(e);
} catch (std::exception const& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
}

// vim:ft=cpp sw=4 ts=4 tw=80 et
