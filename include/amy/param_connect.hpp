#ifndef __AMY_PARAM_CONNECT_HPP__
#define __AMY_PARAM_CONNECT_HPP__

#include <amy/connect.hpp>

#include <boost/parameter.hpp>

namespace amy {
namespace keyword {

BOOST_PARAMETER_NAME(connector);
BOOST_PARAMETER_NAME(endpoint);
BOOST_PARAMETER_NAME(auth_info);
BOOST_PARAMETER_NAME(database);
BOOST_PARAMETER_NAME(flags);

}   //  namespace keyword

BOOST_PARAMETER_FUNCTION(
        (void),
        pconnect,
        keyword::tag,

        (required
            (in_out(connector), *))

        (optional
            (endpoint,    *, amy::null_endpoint())
            (auth_info,   *, amy::auth_info())
            (database,    *, std::string())
            (flags,       *, amy::default_flags)))
{
    return connect(connector, endpoint, auth_info, database, flags);
}

}   //  namespace amy

#endif  //  __AMY_PARAM_CONNECT_HPP__
