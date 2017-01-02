#include <boost/test/unit_test.hpp>

#include <amy/connector.hpp>
#include <amy/placeholders.hpp>

struct async_connect_test {
    bool handler_invoked;
    bool error_detected;

    async_connect_test() {
        handler_invoked = false;
        error_detected = false;
    }

    void handle_connect(AMY_SYSTEM_NS::error_code const& ec) {
        handler_invoked = true;

        if (ec) {
            error_detected = true;
        }
    }

}; // struct async_connect_test

BOOST_AUTO_TEST_CASE(async_connect_ok) {
    async_connect_test fixture;

    AMY_ASIO_NS::io_service io_service;
    amy::connector c(io_service);

    c.async_connect(amy::null_endpoint(),
                    amy::auth_info("amy", "amy"),
                    "test_amy",
                    amy::default_flags,
                    std::bind(&async_connect_test::handle_connect,
                              &fixture,
                              amy::placeholders::error));

    io_service.run();

    BOOST_CHECK(fixture.handler_invoked);
    BOOST_CHECK(!fixture.error_detected);
}

BOOST_AUTO_TEST_CASE(async_connect_failure) {
    async_connect_test fixture;

    AMY_ASIO_NS::io_service io_service;
    amy::connector c(io_service);

    c.async_connect(amy::null_endpoint(),
                    amy::auth_info("bad", "credential"),
                    "test_amy",
                    amy::default_flags,
                    std::bind(&async_connect_test::handle_connect,
                              &fixture,
                              amy::placeholders::error));

    io_service.run();

    BOOST_CHECK(fixture.handler_invoked);
    BOOST_CHECK(fixture.error_detected);
}

// vim:ft=cpp sw=4 ts=4 tw=80 et
