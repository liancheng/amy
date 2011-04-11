#ifndef __AMY_DETAIL_MYSQL_LIB_INIT_HPP__
#define __AMY_DETAIL_MYSQL_LIB_INIT_HPP__

#include <amy/detail/mysql.hpp>
#include <amy/error.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/system/system_error.hpp>
#include <boost/throw_exception.hpp>

namespace amy {
namespace detail {

class mysql_lib_init {
private:
    struct do_init {
        do_init() {
            result_ = ::mysql_library_init(-1, 0, 0);
        }

        ~do_init() {
            ::mysql_library_end();
        }

        int result() const {
            return result_;
        }

        static boost::shared_ptr<do_init> instance() {
            static boost::shared_ptr<do_init> init(new do_init());

            if (0 != init->result()) {
                boost::system::system_error e(init->result(),
                                              boost::system::system_category(),
                                              "mysql_library_init()");
                boost::throw_exception(e);
            }

            return init;
        }

    private:
        int result_;

    };  //  struct do_init

public:
    explicit mysql_lib_init() :
        ref_(do_init::instance())
    {}

    ~mysql_lib_init() {
    }

private:
    boost::shared_ptr<do_init> ref_;

};  //  class mysql_lib_init

}   //  namespace detail
}   //  namespace amy

#endif  //  __AMY_DETAIL_MYSQL_LIB_INIT_HPP__

// vim:ft=cpp ts=4 sw=4 et
