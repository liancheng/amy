#include <boost/test/unit_test.hpp>

#include <amy/auth_info.hpp>

#include <cstring>

BOOST_AUTO_TEST_CASE(anonymous_user_without_password) {
    amy::auth_info auth;
    BOOST_CHECK(std::strcmp("", auth.user()) == 0);
    BOOST_CHECK(static_cast<const char*>(nullptr) == auth.password());
}

BOOST_AUTO_TEST_CASE(user_without_password) {
    amy::auth_info auth("user");
    BOOST_CHECK(std::strcmp("user", auth.user()) == 0);
}

BOOST_AUTO_TEST_CASE(user_with_password) {
    amy::auth_info auth("user", "secret");
    BOOST_CHECK(std::strcmp("user", auth.user()) == 0);
    BOOST_CHECK(std::strcmp("secret", auth.password()) == 0);
}

BOOST_AUTO_TEST_CASE(get_password) {
    amy::auth_info auth;
    auth.password("secret");
    BOOST_CHECK(std::strcmp("secret", auth.password()) == 0);
}

BOOST_AUTO_TEST_CASE(clear_password) {
    amy::auth_info auth;
    auth.password("secret");
    auth.clear_password();
    BOOST_CHECK(static_cast<const char*>(nullptr) == auth.password());
}

// vim:ft=cpp sw=4 ts=4 tw=80 et
