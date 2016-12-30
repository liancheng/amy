# Overview

Amy is a C++11 compliant header-only **A**synchronous **My**SQL client library based on [Asio][asio]. It enables you to work with MySQL in both asynchronous and blocking ways.

# Getting Started

Amy had been tested using and Clang++ 3.8 under Ubuntu 16.04, FreeBSD 11.0, and Mac OS X 10.10. However, it probably also works under other similar UNIX-like systems with minimum efforts.

## Dependencies

Amy allows users to choose between [Boost.Asio][boost-asio] and vanilla Asio by defining the `USE_BOOST_ASIO` compilation flag or not. By default, Amy compiles against vanilla Asio to minimize dependencies.

The following dependencies are required to use Amy:

- [Boost][boost]: 1.55 or newer

  - [Boost.Date_time][boost-date-time]

    Used for processing MySQL date and time data types.

  - [Boost.Iterator][boost-iterator]

    Used for help implementing the result set iterator.

  When compiled against Boost.Asio, [Boost.System][boost-system] is required as a transitive dependency.

- MySQL client library: 5.6 or newer

Most of the time, you can obtain them pretty easily using package managers of your favorite operating systems. For example:

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

Also, Amy uses [SCons][scons] to build examples and tests. You may either install it using your favorite package manager or using Python `pip`:

```
$ sudo pip install scons
```

## Compile Your Program with Amy

Get Amy:

```
$ git clone https://github.com/liancheng/amy.git amy
$ cd amy
$ git submodule update --init --recursive
```

Add `amy/include` into your header search path and make sure to link your program against the following libraries:

- `libmysqlclient`
- `pthread`
- `libboost_system` (if you are using Boost.Asio)

Most of the time, the following compiler options should be sufficient:

```
clang++ -L/usr/lib       \
        -L/usr/local/lib \
        -lmysqlclient    \
        -lpthread        \
        [-lboost_system] \
        ...
```

You may need an extra library path under FreeBSD since that's where the `pkg` package manager installs the MySQL client library:

```
-L/usr/local/lib/mysql
```

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

To build the examples, simply run `scons` under the root directory of the source tree of Amy.

Note that an extra dependency, Boost.Program_options, is needed to build and run the examples. Usually, it's already properly installed together with Boost.

Run the example programs with `-h` command line option to find out how to use them.

# Tests

Some (integration) test cases try to establish actual connections to an existing MySQL server. To make sure these tests succeed, you need to start a MySQL server listening on localhost:3306 and setup the following testing database and user account:

```sql
CREATE DATABASE test_amy;
CREATE USER 'amy'@'localhost' IDENTIFIED BY 'amy';
GRANT ALL PRIVILEGES ON test_amy.* TO 'amy'@'localhost';
FLUSH PRIVILEGES;
```

To run the tests, simply type `scons` under the root directory of Amy's source tree.

[asio]: http://think-async.com/Asio
[boost-asio]: http://www.boost.org/doc/libs/1_63_0/doc/html/boost_asio.html
[boost-date-time]: http://www.boost.org/doc/libs/1_63_0/doc/html/date_time.html
[boost-iterator]: http://www.boost.org/doc/libs/1_63_0/libs/iterator/doc/index.html
[boost-system]: http://www.boost.org/doc/libs/1_63_0/libs/system/doc/index.html
[boost]: http://www.boost.org/
[scons]: http://scons.org/
