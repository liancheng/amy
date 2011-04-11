#ifndef __AMY_SYSTEM_ERROR_HPP__
#define __AMY_SYSTEM_ERROR_HPP__

#include <boost/system/system_error.hpp>

namespace amy {

class system_error : public boost::system::system_error {
public:
    explicit system_error(boost::system::error_code const& ec) :
        boost::system::system_error(ec)
    {}

    explicit system_error(boost::system::error_code const& ec,
                          std::string const& message)
      : boost::system::system_error(ec),
        message_(message)
    {}

    virtual ~system_error() throw() {
    }

    virtual char const* what() const throw() {
        return message_.c_str();
    }

private:
    std::string message_;

};  //  class system_error

}   //  namespace amy

#endif  //  __AMY_SYSTEM_ERROR_HPP__

// vim:ft=cpp ts=4 sw=4 et
