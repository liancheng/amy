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

    auto statement =
        "SELECT character_set_name, maxlen\n"
        "FROM information_schema.character_sets\n"
        "WHERE character_set_name LIKE 'latin%'";

    connector.async_query(statement,
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
                            amy::default_flags,
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
