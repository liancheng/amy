#include <boost/test/unit_test.hpp>

#include <amy/connect.hpp>
#include <amy/connector.hpp>
#include <amy/placeholders.hpp>

struct async_connect_test {
    bool handler_invoked;

    async_connect_test() {
        handler_invoked = false;
    } 

    void handle_connect(boost::system::error_code const& ec) {
        handler_invoked = true;
    }

}; // struct async_connect_test

BOOST_AUTO_TEST_CASE(should_async_connect_to_localhost_with_given_auth_info) {
    async_connect_test fixture;

    boost::asio::io_service io_service;
    amy::connector c(io_service);

    using namespace amy::keyword;

    amy::async_connect(_connector = c,
                       _auth      = amy::auth_info("amy", "amy"),
                       _handler   = boost::bind(
                                        &async_connect_test::handle_connect,
                                        &fixture,
                                        amy::placeholders::error));

    io_service.run();

    BOOST_CHECK(fixture.handler_invoked);
}

// vim:ft=cpp ts=4 sw=4 tw=80 et
