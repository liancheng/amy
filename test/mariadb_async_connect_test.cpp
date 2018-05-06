#include <boost/test/unit_test.hpp>

#include <amy/mariadb_connector.hpp>
#include <amy/placeholders.hpp>

struct maria_async_connect_test {
  bool handler_invoked;

  maria_async_connect_test() { handler_invoked = false; }

  void handle_connect(AMY_SYSTEM_NS::error_code const& ec) {
    handler_invoked = true;
  }

}; // struct maria_async_connect_test

BOOST_AUTO_TEST_CASE(
    should_maria_async_connect_to_localhost_with_given_auth_info) {
  maria_async_connect_test fixture;

  AMY_ASIO_NS::io_service io_service;

  amy::mariadb_connector c(io_service);

  c.async_connect(amy::null_endpoint(), amy::auth_info("amy", "amy"),
      "test_amy", amy::default_flags,
      std::bind(&maria_async_connect_test::handle_connect, &fixture,
          amy::placeholders::error));

  BOOST_CHECK(!fixture.handler_invoked);

  io_service.run();

  BOOST_CHECK(fixture.handler_invoked);
}

// vim:ft=cpp sw=4 ts=4 tw=80 et
