#include "utils.hpp"

#include <amy/connector.hpp>

#include <iostream>

global_options opts;

int main(int argc, char* argv[]) try {
    parse_command_line_options(argc, argv);

    AMY_ASIO_NS::io_service io_service;
    amy::connector connector(io_service);

    connector.connect(opts.tcp_endpoint(),
                      opts.auth_info(),
                      opts.schema,
                      amy::default_flags);

    std::cout << "Connected." << std::endl;

    return 0;
} catch (AMY_SYSTEM_NS::system_error const& e) {
    report_system_error(e);
} catch (std::exception const& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
}

// vim:ft=cpp sw=4 ts=4 tw=80 et
