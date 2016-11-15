#include <boost/test/unit_test.hpp>

#include <amy/connect.hpp>
#include <amy/connector.hpp>

BOOST_AUTO_TEST_CASE(should_connect_to_localhost_with_given_auth_info) {
    using namespace amy::keyword;

    boost::asio::io_service io_service;
    amy::connector c(io_service);
    amy::connect(c, _auth = amy::auth_info("amy", "amy"));
}

// vim:ft=cpp sw=4 ts=4 tw=80 et
