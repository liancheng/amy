#ifndef __AMYSQL_DETAIL_MYSQL_HPP__
#define __AMYSQL_DETAIL_MYSQL_HPP__

#ifdef WIN32
#   include <winsock2.h>
#   include <mysql/mysql.h>
#   include <mysql/errmsg.h>
#else
#   include <mysql/mysql.h>
#   include <mysql/errmsg.h>
#endif  //  #ifdef WIN32

#endif  //  __AMYSQL_DETAIL_MYSQL_HPP__

// vim:ft=cpp ts=4 sw=4 et
