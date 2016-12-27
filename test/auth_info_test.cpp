#include <boost/test/unit_test.hpp>

#include <amy/auth_info.hpp>

#include <cstring>

BOOST_AUTO_TEST_CASE(should_construct_instance_with_anonymous_user) {
    amy::auth_info auth;
    BOOST_CHECK(std::strcmp("", auth.user()) == 0);
}

BOOST_AUTO_TEST_CASE(should_construct_instance_without_password) {
    amy::auth_info auth;
    BOOST_CHECK(static_cast<const char*>(nullptr) == auth.password());
}

BOOST_AUTO_TEST_CASE(should_construct_instance_with_user) {
    amy::auth_info auth("user");
    BOOST_CHECK(std::strcmp("user", auth.user()) == 0);
}

BOOST_AUTO_TEST_CASE(
    should_construct_instance_with_user_and_nonempty_password
) {
    amy::auth_info auth("user", "secret");
    BOOST_CHECK(std::strcmp("user", auth.user()) == 0);
    BOOST_CHECK(std::strcmp("secret", auth.password()) == 0);
}

BOOST_AUTO_TEST_CASE(should_return_previously_set_password) {
    amy::auth_info auth;
    auth.password("secret");
    BOOST_CHECK(std::strcmp("secret", auth.password()) == 0);
}

BOOST_AUTO_TEST_CASE(should_return_null_after_clearing_password) {
    amy::auth_info auth;
    auth.password("secret");
    auth.clear_password();
    BOOST_CHECK(static_cast<const char*>(nullptr) == auth.password());
}

// vim:ft=cpp sw=4 ts=4 tw=80 et
