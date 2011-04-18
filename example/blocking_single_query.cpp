#include "utils.hpp"

#include <amy/connect.hpp>
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

    amy::connect(connector,
                 opts.tcp_endpoint(),
                 opts.auth_info(),
                 opts.schema);

    std::cout << "Connected." << std::endl;

    std::string statement =
        "SELECT * FROM "
        "information_schema.character_sets "
        "WHERE "
        "CHARACTER_SET_NAME LIKE 'latin%'";

    std::cout << "SQL statement: " << statement << std::endl;
    connector.query(statement);
    std::cout << "Query ok.\n" << std::endl;

    amy::result_set result_set = connector.store_result();
    std::cout
        << boost::format("Field count: %1%, "
                         "result set size: %2%, "
                         "affected rows: %3%, contents:\n")
           % result_set.field_count()
           % result_set.size()
           % result_set.affected_rows()
        << std::endl;

    std::copy(result_set.begin(),
              result_set.end(),
              std::ostream_iterator<amy::row>(std::cout, "\n"));

    return 0;
}
catch(boost::system::system_error const& e) {
    std::cerr
        << boost::format("System error: %1%: %2%")
           % e.code().value() % e.what()
        << std::endl;
}
catch(std::exception const& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
}
