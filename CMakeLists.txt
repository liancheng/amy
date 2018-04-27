cmake_minimum_required(VERSION 3.0)

project(amy CXX)

include_directories(include)

option(USE_BOOST_ASIO "USE_BOOST_ASIO" ON)
if(USE_BOOST_ASIO)
  set(USE_BOOST_ASIO "1")
endif()
configure_file(include/amy/asio.hpp.in ${PROJECT_SOURCE_DIR}/include/amy/asio.hpp @ONLY)

option(build_tests "build tests" ON)
if(build_tests)
    enable_testing()
    aux_source_directory(test test_src)
    add_executable(tests ${test_src})
    target_link_libraries(tests boost_unit_test_framework boost_system mysqlclient pthread)
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
    add_library(example_utils STATIC example/utils.cpp)
    foreach(example ${examples})
        add_executable(${example} example/${example}.cpp)
        target_link_libraries(${example} example_utils boost_system mysqlclient pthread)
    endforeach()
endif()

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX} PATTERN include/amy/asio.hpp.in EXCLUDE)