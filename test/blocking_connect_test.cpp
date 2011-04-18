#include <gtest/gtest.h>

#include <amy/connect.hpp>
#include <amy/connector.hpp>

TEST(blocking_connect_test,
     should_connect_to_localhost_with_given_user)
{
    boost::asio::io_service io_service;
    amy::connector c(io_service);
    amy::connect(c, amy::null_endpoint(), amy::auth_info("amy", "amy"));
}
