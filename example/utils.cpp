#include "utils.hpp"

#include <getopt.h>

#include <cstdlib>
#include <iostream>
#include <iterator>

AMY_ASIO_NS::ip::tcp::endpoint global_options::tcp_endpoint() const {
    using namespace AMY_ASIO_NS::ip;

    return host.empty() ?
        tcp::endpoint(address_v4::loopback(), port) :
        tcp::endpoint(address::from_string(host), port);
}

amy::auth_info global_options::auth_info() const {
    return amy::auth_info(user, password);
}

static std::string usage =
    "Available options:\n"
    "  -h [ --help ]                   Show this help message\n"
    "  -H [ --host ] arg (=127.0.0.1)  MySQL server host.\n"
    "  -P [ --port ] arg (=3306)       MySQL server port.\n"
    "  -u [ --user ] arg (=amy)        Login user.\n"
    "  -p [ --password ] arg (=amy)    Login password.\n"
    "  -s [ --schema ] arg (=test_amy) Default schema to use.";

static struct option options[] = {
    { "help",     no_argument,       NULL, 'h' },
    { "host",     required_argument, NULL, 'H' },
    { "port",     required_argument, NULL, 'P' },
    { "user",     required_argument, NULL, 'u' },
    { "password", required_argument, NULL, 'p' },
    { "schema",   required_argument, NULL, 's' }
};

void parse_command_line_options(int argc, char* argv[]) {
    char ch;

    while ((ch = getopt_long(argc, argv, "hH:P:u:P:s:", options, NULL)) != -1) {
        switch (ch) {
            case 'h':
                std::cout << usage << std::endl;
                exit(EXIT_SUCCESS);
                break;

            case 'H':
                opts.host = optarg;
                break;

            case 'P':
                opts.port = atoi(optarg);
                break;

            case 'u':
                opts.user = optarg;
                break;

            case 'p':
                opts.password = optarg;
                break;

            case 's':
                opts.schema = optarg;
                break;

            default:
                std::cerr << "Command option not recognized." << std::endl;
                std::cout << usage << std::endl;
                break;
        }
    }
}

std::string read_from_stdin() {
    std::istream_iterator<char> begin(std::cin);
    std::istream_iterator<char> end;

    std::cin.unsetf(std::ios::skipws);
    std::string str(begin, end);
    std::cin.setf(std::ios::skipws);

    return str;
}

void check_error(AMY_SYSTEM_NS::error_code const& ec) {
    if (ec) {
        throw AMY_SYSTEM_NS::system_error(ec);
    }
}

void report_system_error(AMY_SYSTEM_NS::system_error const& e) {
    std::cerr
        << "System error: "
        << e.code().value() << " - " << e.what()
        << std::endl;
}

// vim:ft=cpp sw=4 ts=4 tw=80 et
