#include "utils.hpp"

#include <amy/asio.hpp>
#include <amy/connector.hpp>
#include <amy/placeholders.hpp>

#include <boost/format.hpp>

#include <functional>
#include <iostream>

global_options opts;

void handle_connect(AMY_SYSTEM_NS::error_code const& ec,
                    amy::connector& connector)
{
    check_error(ec);
    std::cout << "Connected." << std::endl;
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
