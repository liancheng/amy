#ifndef __AMYSQL_AUTH_INFO_HPP__
#define __AMYSQL_AUTH_INFO_HPP__

#include <boost/optional.hpp>

#include <string>

namespace amysql {

class auth_info {
public:
    auth_info() :
        user_(), password_()
    {}

    auth_info(std::string const& user) :
        user_(user)
    {}

    auth_info(std::string const& user, std::string const& password) :
        user_(user),
        password_(password)
    {}

    void user(std::string const& user) {
        user_ = user;
    }

    char const* user() const {
        return user_.c_str();
    }

    void password(std::string const& password) {
        password_ = password
    }

    char const* password() const {
        return !!password ? password_->c_str() : 0;
    }

    void clear_password() {
        password_.reset();
    }

private:
    std::string user_;
    boost::optional<std::string> password_;

};  //  class auth_info

}   //  namespace amysql

#endif  //  __AMYSQL_AUTH_INFO_HPP__

// vim:ft=cpp ts=4 sw=4 et
