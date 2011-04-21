#include "utils.hpp"

#include <amy/connect.hpp>
#include <amy/connector.hpp>
#include <amy/param_connect.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/format.hpp>
#include <boost/system/system_error.hpp>

#include <iostream>

global_options opts;

int main(int argc, char* argv[]) try {
    parse_command_line_options(argc, argv);

    boost::asio::io_service io_service;
    amy::connector connector(io_service);

    using namespace amy::keyword;

    amy::pconnect(connector,
                  _endpoint  = opts.tcp_endpoint(),
                  _auth_info = opts.auth_info(),
                  _database  = opts.schema);

    std::cout << "Connected." << std::endl;

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
