#include <boost/test/unit_test.hpp>

#include <amy/connector.hpp>

struct blocking_query_fixture {
    AMY_ASIO_NS::io_service io_service;
    amy::connector c;

    blocking_query_fixture() : c(io_service) {
        c.connect(amy::null_endpoint(),
                  amy::auth_info("amy", "amy"),
                  "test_amy",
                  amy::default_flags);
    }

}; // struct blocking_query_fixture

BOOST_FIXTURE_TEST_CASE(blocking_query, blocking_query_fixture) {
    c.query("SELECT * FROM t0");

    auto rs = c.store_result();

    BOOST_CHECK_EQUAL(rs.affected_rows(), 3u);
    BOOST_CHECK_EQUAL(rs.field_count(), 1u);
    BOOST_CHECK_EQUAL(rs.size(), 3u);

    BOOST_CHECK_EQUAL(rs[0][0].as<amy::sql_int>(), 0);
    BOOST_CHECK_EQUAL(rs[1][0].as<amy::sql_int>(), 1);
    BOOST_CHECK_EQUAL(rs[2][0].as<amy::sql_int>(), 2);
}

// vim:ft=cpp sw=4 ts=4 tw=80 et
