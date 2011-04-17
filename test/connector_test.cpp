#include <gtest/gtest.h>

#include <amy/connector.hpp>

TEST(connector_test,
     should_construct_a_connector_instance)
{
    boost::asio::io_service io_service;
    amy::connector connector(io_service);
}

TEST(connector_test,
     should_be_opened_successfully)
{
    boost::asio::io_service io_service;
    amy::connector connector(io_service);

    connector.open();
    ASSERT_TRUE(connector.is_open());
}

TEST(connector_test,
     should_be_closed_successfully)
{
    boost::asio::io_service io_service;
    amy::connector connector(io_service);

    connector.open();
    ASSERT_TRUE(connector.is_open());

    connector.close();
    ASSERT_FALSE(connector.is_open());
}

// vim:ft=cpp ts=4 sw=4 et
