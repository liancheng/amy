#include <gtest/gtest.h>

#include <amysql/auth_info.hpp>

TEST(auth_info_test,
     should_construct_instance_with_anonymous_user)
{
    amysql::auth_info auth;
    ASSERT_STREQ("", auth.user());
}

TEST(auth_info_test,
     should_construct_instance_without_password)
{
    amysql::auth_info auth;
    ASSERT_EQ(0, auth.password());
}

TEST(auth_info_test,
     should_construct_instance_with_user)
{
    amysql::auth_info auth("user");
    ASSERT_STREQ("user", auth.user());
}

TEST(auth_info_test,
     should_construct_instance_with_user_and_nonempty_password)
{
    amysql::auth_info auth("user", "secret");
    ASSERT_STREQ("user", auth.user());
    ASSERT_STREQ("secret", auth.password());
}

TEST(auth_info_test,
     should_return_previously_set_password)
{
    amysql::auth_info auth;
    auth.password("secret");
    ASSERT_STREQ("secret", auth.password());
}

TEST(auth_info_test,
     should_return_null_after_clearing_password)
{
    amysql::auth_info auth;
    auth.password("secret");
    auth.clear_password();
    ASSERT_EQ(0, auth.password());
}

// vim:ft=cpp ts=4 sw=4 et
