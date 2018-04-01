#ifndef __AMY_OPTIONS_HPP__
#define __AMY_OPTIONS_HPP__

#include <amy/detail/mysql_option.hpp>

namespace amy {
namespace options {

using init_command = detail::mysql_option::char_sequence<detail::init_command>;
using compress     = detail::mysql_option::switcher<detail::compress>;
using connect_timeout =
    detail::mysql_option::unsigned_integer<detail::connect_timeout>;
using guess_connection =
    detail::mysql_option::switcher<detail::guess_connection>;
using local_infile =
    detail::mysql_option::unsigned_integer<detail::local_infile>;
using named_pipe = detail::mysql_option::switcher<detail::named_pipe>;
using protocol   = detail::mysql_option::unsigned_integer<detail::protocol>;
using read_timeout =
    detail::mysql_option::unsigned_integer<detail::read_timeout>;
using reconnect = detail::mysql_option::boolean<detail::reconnect>;
using set_client_ip =
    detail::mysql_option::char_sequence<detail::set_client_ip>;
using ssl_verify_server_cert =
    detail::mysql_option::boolean<detail::ssl_verify_server_cert>;
using use_embedded_connection =
    detail::mysql_option::switcher<detail::use_embedded_connection>;
using use_remote_connection =
    detail::mysql_option::switcher<detail::use_remote_connection>;
using use_result = detail::mysql_option::switcher<detail::use_result>;
using write_timeout =
    detail::mysql_option::unsigned_integer<detail::write_timeout>;
using read_default_file =
    detail::mysql_option::char_sequence<detail::read_default_file>;
using read_default_group =
    detail::mysql_option::char_sequence<detail::read_default_group>;
using report_data_truncation =
    detail::mysql_option::boolean<detail::report_data_truncation>;
using secure_auth = detail::mysql_option::boolean<detail::secure_auth>;
using set_charset_dir =
    detail::mysql_option::char_sequence<detail::set_charset_dir>;
using set_charset_name =
    detail::mysql_option::char_sequence<detail::set_charset_name>;
using shared_memory_base_name =
    detail::mysql_option::char_sequence<detail::shared_memory_base_name>;

} // namespace options
} // namespace amy

#endif /* __AMY_OPTIONS_HPP__ */
