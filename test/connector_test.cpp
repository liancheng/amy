#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <amy/connector.hpp>

BOOST_AUTO_TEST_CASE(construction) {
    AMY_ASIO_NS::io_service io_service;
    amy::connector connector(io_service);
}

BOOST_AUTO_TEST_CASE(open_after_construction) {
    AMY_ASIO_NS::io_service io_service;
    amy::connector connector(io_service);

    connector.open();
    BOOST_CHECK(connector.is_open());
}

BOOST_AUTO_TEST_CASE(open_and_close) {
    AMY_ASIO_NS::io_service io_service;
    amy::connector connector(io_service);

    connector.open();
    BOOST_CHECK(connector.is_open());

    connector.close();
    BOOST_CHECK(!connector.is_open());
}

// vim:ft=cpp sw=4 ts=4 tw=80 et
