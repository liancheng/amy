#include <boost/test/unit_test.hpp>

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

    c.async_connect(amy::null_endpoint(),
                    amy::auth_info("amy", "amy"),
                    "test_amy",
                    amy::default_flags,
                    boost::bind(&async_connect_test::handle_connect,
                                &fixture,
                                amy::placeholders::error));

    io_service.run();

    BOOST_CHECK(fixture.handler_invoked);
}

// vim:ft=cpp sw=4 ts=4 tw=80 et
