# Overview

Amy is a C++11 compliant header-only **A**synchronous **My**SQL client library based on [Asio][asio]. It enables you to work with MySQL in both asynchronous and blocking ways. Amy had been tested using and Clang++ 3.8 under Ubuntu 16.04, FreeBSD 11.0, and Mac OS X 10.10. It should also works under other similar UNIX-like systems with minimum efforts.

# Getting Started

## Dependencies

### Using vanilla Asio

By default, Amy compiles against [vanilla Asio][vanilla-asio] to minimize dependencies. You may also claim this explicitly by defining macro `USE_BOOST_ASIO` to `0`.

- [Asio][vanilla-asio] 1.10.6 or newer
- [Boost][boost] 1.58 or newer for the following dependencies:

  - [Boost.Date_time][boost-date-time] for processing MySQL date and time data types.
  - [Boost.Iterator][boost-iterator] for implementing the result set iterator.

- [MySQL C client library][mysql-c-connector] 5.6 or newer

### Using Boost.Asio

Amy also allows you to compile against [Boost.Asio][boost-asio] by defineing `USE_BOOST_ASIO` to `1`.

- [Boost][boost] 1.58 or newer for the following dependencies:

  - [Boost.Asio][boost-asio]
  - [Boost.Date_time][boost-date-time] for processing MySQL date and time data types
  - [Boost.Iterator][boost-iterator] for implementing the result set iterator
  - [Boost.System][boost-system] as a transient dependency of [Boost.Asio][boost-asio]

- [MySQL C client library][mysql-c-connector] 5.6 or newer

## Installing dependencies

You can probably obtain all the dependencies pretty easily using the package manager on your favorite operating system. For example:

- Ubuntu 16.04:

  ```
  $ sudo apt-get install libboost-all-dev libmysqlclient-dev libasio-dev
  ```

- FreeBSD 11.0:

  ```
  $ sudo pkg instal boost-all mysql-connector-c asio
  ```

- Mac OS X 10.10:

  ```
  $ brew install boost mysql-connector-c asio
  ```

Optionally, you may also want to install [SCons][scons] to build examples and tests:

```
$ sudo pip install scons
```

## Compile your program with Amy

Get Amy:

```
$ git clone https://github.com/liancheng/amy.git amy
$ cd amy
$ export AMY_HOME=$(pwd)
```

Add `$AMY_HOME/include` to your header search path and make sure to link your program against the following libraries:

- `libmysqlclient`
- `pthread`
- `libboost_system` (only required when using Boost.Asio)

Most of the time, the following compiler options should be sufficient:

- Building against vanilla Asio

  ```
  clang++ -DUSE_BOOST_ASIO=0  \
          -I$AMY_HOME/include \
          -L/usr/lib          \
          -L/usr/local/lib    \
          -lmysqlclient       \
          -lpthread           \
          ...
  ```

- Building against Boost.Asio

  ```
  clang++ -DUSE_BOOST_ASIO=1  \
          -I$AMY_HOME/include \
          -L/usr/lib          \
          -L/usr/local/lib    \
          -lmysqlclient       \
          -lpthread           \
          -lboost_system      \
          ...
  ```

You may need an extra library path `/usr/local/lib/mysql` under FreeBSD since that's where the `pkg` package manager installs the MySQL client library:

# Examples

Please check the following examples for a taste of Amy:

- How to establish a MySQL connection?

  - [example/async_connect.cpp](example/async_connect.cpp)
  - [example/blocking_connect.cpp](example/blocking_connect.cpp)

- How to perform a single SQL query and retrieve its result set?

  - [example/async_single_query.cpp](example/async_single_query.cpp)
  - [example/blocking_single_query.cpp](example/blocking_single_query.cpp)

- How to perform multiple SQL queries at once and retrieve their result sets?

  - [example/async_multi_query.cpp](example/async_multi_query.cpp)
  - [example/blocking_multi_query.cpp](example/blocking_multi_query.cpp)

- How to perform a DDL/DML statement?

  - [example/async_execute.cpp](example/async_execute.cpp)
  - [example/blocking_execute.cpp](example/blocking_execute.cpp)

Note that an extra dependency, [Boost.Program_options][boost-program-options], is used to build and run the examples. Usually, it should have already been installed together with Boost.

To build the examples, run:

```
$ cd $AMY_ROOT
$ scons USE_BOOST_ASIO=0 example # Using vanilla Asio
$ scons USE_BOOST_ASIO=1 example # Using Boost.Asio
```

Built executables can be found under `$AMY_ROOT/build/example`. Run each example with `--help` to find out how to use them.

Run the example programs with `-h` command line option to find out how to use them.

# Tests

Some (integration) test cases need to establish actual connections to an existing MySQL server. To make sure these tests succeed, you need to start a MySQL server listening on localhost:3306 and setup the following testing database and user account:

```sql
CREATE DATABASE test_amy;
CREATE USER 'amy'@'localhost' IDENTIFIED BY 'amy';
GRANT ALL PRIVILEGES ON test_amy.* TO 'amy'@'localhost';
FLUSH PRIVILEGES;
```

To test Amy, run:

```
$ cd $AMY_ROOT
$ scons USE_BOOST_ASIO=0 test # Using vanilla Asio
$ scons USE_BOOST_ASIO=1 test # Using Boost.Asio
```

[asio]: http://think-async.com/Asio
[boost]: http://www.boost.org/
[boost-asio]: http://www.boost.org/doc/libs/1_58_0/doc/html/boost_asio.html
[boost-date-time]: http://www.boost.org/doc/libs/1_58_0/doc/html/date_time.html
[boost-iterator]: http://www.boost.org/doc/libs/1_58_0/libs/iterator/doc/index.html
[boost-program-options]: http://www.boost.org/doc/libs/1_58_0/doc/html/program_options.html
[boost-system]: http://www.boost.org/doc/libs/1_58_0/libs/system/doc/index.html
[scons]: http://scons.org/
[vanilla-asio]: https://github.com/chriskohlhoff/asio
