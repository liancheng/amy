cmake_minimum_required(VERSION 3.0)

project(amy CXX)

include_directories(include)

option(USE_BOOST_ASIO "USE_BOOST_ASIO" ON)
if(USE_BOOST_ASIO)
  set(USE_BOOST_ASIO "1")
endif()
configure_file(include/amy/asio.hpp.in ${PROJECT_SOURCE_DIR}/include/amy/asio.hpp @ONLY)

option(USE_MARIADB "USE_MARIADB" ON)
if(USE_MARIADB)
  set(MYSQL_LIB mariadbclient)
else()
  set(MYSQL_LIB mysqlclient)
endif()

option(build_tests "build tests" ON)
if(build_tests)
    enable_testing()
    set(test_src
        test/async_connect_test.cpp
        test/auth_info_test.cpp
        test/blocking_connect_test.cpp
        test/connector_test.cpp
        test/init.sql
        test/main.cpp)
    if(USE_MARIADB)
        set(test_src ${test_src} test/mariadb_async_connect_test.cpp)
    endif()
    add_executable(tests ${test_src})
    target_link_libraries(tests boost_unit_test_framework boost_system ${MYSQL_LIB} pthread)
    add_test(tests tests)
endif()

option(build_examples "build examples" ON)
if(build_examples)
    set(examples
        blocking_execute
        async_execute
        blocking_multi_query
        async_multi_query
        blocking_single_query
        async_single_query
        blocking_connect
        async_connect
    )
    if(USE_MARIADB)
        set(examples
            ${examples}
            mariadb_async_multi_query
            mariadb_async_single_query)
    endif()
    add_library(example_utils STATIC example/utils.cpp)
    foreach(example ${examples})
        add_executable(${example} example/${example}.cpp)
        target_link_libraries(${example} example_utils boost_system ${MYSQL_LIB} pthread)
    endforeach()
endif()

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX} PATTERN include/amy/asio.hpp.in EXCLUDE)