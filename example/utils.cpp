#include "utils.hpp"

#include <boost/program_options.hpp>

#include <cstdlib>
#include <iostream>
#include <iterator>

boost::asio::ip::tcp::endpoint global_options::tcp_endpoint() const {
    using namespace boost::asio::ip;

    return host.empty() ?
        tcp::endpoint(address_v4::loopback(), port) :
        tcp::endpoint(address::from_string(host), port);
}

amy::auth_info global_options::auth_info() const {
    return amy::auth_info(user, password);
}

void parse_command_line_options(int argc, char* argv[]) {
    using namespace boost::program_options;

    options_description opts_desc("Available options");
    opts_desc.add_options()
        ("help,h",
         "Show this help message")

        ("host,H",
         value(&opts.host)->default_value("127.0.0.1"),
         "MySQL server host.")

        ("port,P",
         value(&opts.port)->default_value(3306u),
         "MySQL server port.")

        ("user,u",
         value(&opts.user)->default_value("amy"),
         "Login user.")

        ("password,p",
         value(&opts.password)->default_value("amy"),
         "Login password.")

        ("schema,s",
         value(&opts.schema)->default_value("test_amy"),
         "Default schema to use.")
        ;

    variables_map vars;
    store(parse_command_line(argc, argv, opts_desc), vars);
    notify(vars);

    if (vars.count("help")) {
        std::cout << opts_desc << std::endl;
        exit(EXIT_SUCCESS);
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

void check_error(boost::system::error_code const& ec) {
    if (!!ec) {
        boost::throw_exception(boost::system::system_error(ec));
    }
}

// vim:ft=cpp sw=4 ts=4 tw=80 et
