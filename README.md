# Overview

Amy is a C++11 compliant header-only **A**synchronous **My**SQL client library based on Boost.Asio. It enables you to work with MySQL in both asynchronous and blocking ways.

# Getting Started

Amy had been tested using and Clang++ 3.8 under Ubuntu 16.04, FreeBSD 11.0, and Mac OS X 10.10. However, it probably also works under other similar UNIX-like systems with minimum efforts.

## Dependencies

The following dependencies are required to use Amy:

- Boost: 1.55 or newer

  Amy uses various header-only Boost libraries as well as the following compiled Boost libraries:

  - Boost.System

    Same as Boost.Asio, Amy uses Boost.System for error handling.

- MySQL client library: 5.6 or newer

Most of the time, you can obtain them pretty easily using package managers of your favorite operating systems. For example:

- Ubuntu 16.04:

  ```
  $ sudo apt-get install libboost-all-dev libmysqlclient-dev
  ```

- FreeBSD 11.0:

  ```
  $ sudo pkg instal boost-all mysql-connector-c
  ```

- Mac OS X 10.10:

  ```
  $ brew install boost mysql-connector-c
  ```

Also, Amy uses [SCons](http://scons.org/) to build examples and tests. You may either install it using your favorite package manager or using Python `pip`:

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
- `libboost_system`

Most of the time, the following compiler options should be sufficient:

```
clang++ -L/usr/lib       \
        -L/usr/local/lib \
        -lboost_system   \
        -lmysqlclient    \
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
