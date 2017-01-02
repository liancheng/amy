#ifndef __AMY_TEST_GLOBAL_FIXTURE_HPP__
#define __AMY_TEST_GLOBAL_FIXTURE_HPP__

#include <amy/client_flags.hpp>
#include <amy/connector.hpp>
#include <amy/execute.hpp>

#include <algorithm>
#include <fstream>
#include <iterator>

struct global_fixture {
    AMY_ASIO_NS::io_service io_service;
    amy::connector connector;

    global_fixture() : connector(io_service) {
        connector.connect(
                amy::null_endpoint(),
                amy::auth_info("amy", "amy"),
                "test_amy",
                amy::client_multi_statements);

        amy::forgetful_execute(connector, read_from_file("test/setup.sql"));
    }

    ~global_fixture() {
        amy::forgetful_execute(connector, read_from_file("test/teardown.sql"));
    }

    std::string read_from_file(char const* path) {
        std::ifstream in(path);
        std::istream_iterator<char> begin(in);
        std::istream_iterator<char> end;

        in.unsetf(std::ios::skipws);
        std::string str(begin, end);
        in.setf(std::ios::skipws);

        return str;
    }

}; // struct global_fixture

#endif // __AMY_TEST_GLOBAL_FIXTURE_HPP__

// vim:ft=cpp sw=4 ts=4 tw=80 et
