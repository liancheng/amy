#include <gtest/gtest.h>

#include <amysql/connector.hpp>

TEST(connector_test,
     should_construct_a_connector_instance)
{
    boost::asio::io_service io_service;
    amysql::connector connector(io_service);
}

// vim:ft=cpp ts=4 sw=4 et
