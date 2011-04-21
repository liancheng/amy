#include "utils.hpp"

#include <amy/connect.hpp>
#include <amy/connector.hpp>
#include <amy/placeholders.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/system/system_error.hpp>

#include <iostream>

global_options opts;

void handle_store_result(boost::system::error_code const& ec,
                         amy::result_set result_set,
                         amy::connector& connector)
{
    if (!!ec) {
        std::cerr
            << boost::format("Failed to store result: %1% - %2%")
               % ec.value() % connector.error_message(ec)
            << std::endl;
    }

    std::copy(result_set.begin(),
              result_set.end(),
              std::ostream_iterator<amy::row>(std::cout, "\n"));
}

void handle_query(boost::system::error_code const& ec,
                  amy::connector& connector)
{
    if (!!ec) {
        std::cerr
            << boost::format("Query error: %1% - %2%")
               % ec.value() % connector.error_message(ec)
            << std::endl;
    }

    connector.async_store_result(
            boost::bind(handle_store_result,
                        amy::placeholders::error,
                        amy::placeholders::result_set,
                        boost::ref(connector)));
}

void handle_connect(boost::system::error_code const& ec,
                    amy::connector& connector)
{
    if (!!ec) {
        std::cerr
            << boost::format("Connection error: %1% - %2%")
               % ec.value() % connector.error_message(ec)
            << std::endl;
        return;
    }

    std::cout << "Connected." << std::endl;

    std::string statement = "SHOW DATABASES;";
    connector.async_query(statement,
                          boost::bind(handle_query,
                                      amy::placeholders::error,
                                      boost::ref(connector)));
}

int main(int argc, char* argv[]) try {
    parse_command_line_options(argc, argv);

    boost::asio::io_service io_service;
    amy::connector connector(io_service);

    amy::async_connect(connector,
                       opts.tcp_endpoint(),
                       opts.auth_info(),
                       boost::bind(handle_connect,
                                   amy::placeholders::error,
                                   boost::ref(connector)));

    io_service.run();

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
