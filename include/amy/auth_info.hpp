#ifndef __AMY_AUTH_INFO_HPP__
#define __AMY_AUTH_INFO_HPP__

#include <boost/optional.hpp>

#include <string>

namespace amy {

/// The \c auth_info class wraps MySQL authentication information.
/**
 * The authentication information includes a username and an optional password.
 * Please notice the difference between a user without password and a user with
 * an empty password (which is an emtpy string).
 */
class auth_info {
public:
    /// Default constructor.  Creates an \c auth_info instance with anonymous
    /// user and no password.
    auth_info() :
        user_(), password_()
    {}

    /// Creates an \c auth_info instance with a user and no password.
    /**
     * \param user The user name.  Empty string stands for the anonymous user.
     */
    auth_info(std::string const& user) :
        user_(user)
    {}

    /// Creates an \c auth_info instance with a user and a password.
    /**
     * \param user The user.  Empty string stands for the anonymous user.
     * \param password The password.  May be cleared by \c clear_password.
     *
     * \see \c clear_password
     */
    auth_info(std::string const& user, std::string const& password) :
        user_(user),
        password_(password)
    {}

    /// Sets the user.
    /**
     * \param user The user.  Empty string stands for the anonymous user.
     */
    void user(std::string const& user) {
        user_ = user;
    }

    /// Gets the user.
    /**
     * \return The user.  May be empty (anonymous user) but never NULL.
     */
    char const* user() const {
        return user_.c_str();
    }

    /// Sets the password.
    /**
     * \param password The password.
     */
    void password(std::string const& password) {
        password_ = password;
    }

    /// Gets the password.
    /**
     * \return Returns the password set previously.  If no password is set,
     *         returns NULL.
     */
    char const* password() const {
        return !!password_ ? password_->c_str() : 0;
    }

    /// Clears any previously set password.
    void clear_password() {
        password_.reset();
    }

private:
    std::string user_;
    boost::optional<std::string> password_;

};  //  class auth_info

}   //  namespace amy

#endif  //  __AMY_AUTH_INFO_HPP__

// vim:ft=cpp ts=4 sw=4 et
