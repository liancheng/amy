#include <gtest/gtest.h>

#include <amy/connect.hpp>
#include <amy/connector.hpp>
#include <amy/placeholders.hpp>

#include <boost/bind.hpp>

class async_connect_test : public testing::Test {
protected:
    bool handler_invoked;

    virtual void SetUp() {
        handler_invoked = false;
    } 

public:
    void handle_connect(boost::system::error_code const& ec) {
        handler_invoked = true;
    }

};  //  class async_connect_test

TEST_F(async_connect_test,
       should_connect_to_localhost_with_given_user)
{
    boost::asio::io_service io_service;
    amy::connector c(io_service);

    amy::async_connect(c,
                       amy::null_endpoint(),
                       amy::auth_info("amy", "amy"),
                       boost::bind(&async_connect_test::handle_connect,
                                   this,
                                   amy::placeholders::error));

    io_service.run();

    ASSERT_TRUE(handler_invoked);
}
