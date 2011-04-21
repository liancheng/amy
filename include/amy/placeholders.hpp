#ifndef __AMY_PLACEHOLDERS_HPP__
#define __AMY_PLACEHOLDERS_HPP__

#include <boost/asio/placeholders.hpp>

namespace amy {
namespace placeholders {

inline boost::arg<1> error() {
    return boost::arg<1>();
}

inline boost::arg<2> result_set() {
    return boost::arg<2>();
}

inline boost::arg<2> affected_rows() {
    return boost::arg<2>();
}

}   //  namespace placeholders
}   //  namespace amy

#endif  //  __AMY_PLACEHOLDERS_HPP__

// vim:ft=cpp ts=4 sw=4 et
