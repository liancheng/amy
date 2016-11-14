#include <gtest/gtest.h>

#include <amy/auth_info.hpp>

TEST(auth_info_test,
     should_construct_instance_with_anonymous_user)
{
    amy::auth_info auth;
    ASSERT_STREQ("", auth.user());
}

TEST(auth_info_test,
     should_construct_instance_without_password)
{
    amy::auth_info auth;
    ASSERT_EQ(nullptr, auth.password());
}

TEST(auth_info_test,
     should_construct_instance_with_user)
{
    amy::auth_info auth("user");
    ASSERT_STREQ("user", auth.user());
}

TEST(auth_info_test,
     should_construct_instance_with_user_and_nonempty_password)
{
    amy::auth_info auth("user", "secret");
    ASSERT_STREQ("user", auth.user());
    ASSERT_STREQ("secret", auth.password());
}

TEST(auth_info_test,
     should_return_previously_set_password)
{
    amy::auth_info auth;
    auth.password("secret");
    ASSERT_STREQ("secret", auth.password());
}

TEST(auth_info_test,
     should_return_null_after_clearing_password)
{
    amy::auth_info auth;
    auth.password("secret");
    auth.clear_password();
    ASSERT_EQ(nullptr, auth.password());
}

// vim:ft=cpp ts=4 sw=4 et
