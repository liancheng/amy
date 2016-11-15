#include <boost/test/unit_test.hpp>

#include <amy/auth_info.hpp>

BOOST_AUTO_TEST_CASE(should_construct_instance_with_anonymous_user) {
    amy::auth_info auth;
    BOOST_TEST("" == auth.user());
}

BOOST_AUTO_TEST_CASE(should_construct_instance_without_password) {
    amy::auth_info auth;
    BOOST_TEST(static_cast<const char*>(nullptr) == auth.password());
}

BOOST_AUTO_TEST_CASE(should_construct_instance_with_user) {
    amy::auth_info auth("user");
    BOOST_TEST("user" == auth.user());
}

BOOST_AUTO_TEST_CASE(
    should_construct_instance_with_user_and_nonempty_password
) {
    amy::auth_info auth("user", "secret");
    BOOST_TEST("user", auth.user());
    BOOST_TEST("secret" == auth.password());
}

BOOST_AUTO_TEST_CASE(should_return_previously_set_password) {
    amy::auth_info auth;
    auth.password("secret");
    BOOST_TEST("secret" == auth.password());
}

BOOST_AUTO_TEST_CASE(should_return_null_after_clearing_password) {
    amy::auth_info auth;
    auth.password("secret");
    auth.clear_password();
    BOOST_TEST(static_cast<const char*>(nullptr) == auth.password());
}

// vim:ft=cpp sw=4 ts=4 tw=80 et
