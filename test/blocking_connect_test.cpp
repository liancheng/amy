#include <boost/test/unit_test.hpp>

#include <amy/connector.hpp>

BOOST_AUTO_TEST_CASE(should_connect_to_localhost_with_given_auth_info) {
    AMY_ASIO_NS::io_service io_service;
    amy::connector c(io_service);

    c.connect(amy::null_endpoint(),
              amy::auth_info("amy", "amy"),
              "test_amy",
              amy::default_flags);
}

// vim:ft=cpp sw=4 ts=4 tw=80 et
