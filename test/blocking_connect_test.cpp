#include <boost/test/unit_test.hpp>

#include <amy/connector.hpp>

BOOST_AUTO_TEST_CASE(connect_ok) {
    AMY_ASIO_NS::io_service io_service;
    amy::connector c(io_service);

    c.connect(amy::null_endpoint(),
              amy::auth_info("amy", "amy"),
              "test_amy",
              amy::default_flags);
}

BOOST_AUTO_TEST_CASE(connect_failure) {
    AMY_ASIO_NS::io_service io_service;
    amy::connector c(io_service);

    try {
        c.connect(amy::null_endpoint(),
                  amy::auth_info("bad", "credential"),
                  "test_amy",
                  amy::default_flags);

        BOOST_ERROR("Expecting an exception to be thrown");
    } catch (amy::system_error const& e) {
        BOOST_CHECK_EQUAL(std::string(e.what()).find("Access denied"), 0u);
    }
}

// vim:ft=cpp sw=4 ts=4 tw=80 et
