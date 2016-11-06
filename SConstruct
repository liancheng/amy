import os
import platform

system = platform.system()
mysqlclient_lib = 'mysqlclient_r'
boost_thread_lib = 'boost_thread'
extra_libpaths = []

if system in ['Darwin', 'Linux']:
    # `mysqlclient_r' is not available under Mac OS X
    mysqlclient_lib = 'mysqlclient'

if system == 'Darwin':
    boost_thread_lib = 'boost_thread-mt'

if system == 'FreeBSD':
    # Default path where `pkg` puts `mysql-connector-c'
    extra_libpaths = [Dir('/usr/local/lib/mysql')]

libpath = [Dir('/usr/lib'), Dir('/usr/local/lib')] + extra_libpaths

env = Environment(ENV=os.environ,
                  CXX=os.environ.get('CXX', 'clang++'),
                  CCFLAGS='-ggdb -std=c++11',
                  CPPPATH=[Dir('include'),
                           Dir('/usr/local/include')],
                  CPPDEFINES=[('BOOST_PARAMETER_MAX_ARITY', 6)],
                  LIBPATH=libpath)

libs = ['boost_system',
        boost_thread_lib,
        mysqlclient_lib,
        'pthread',
        'z']

env.Execute('@doxygen Doxyfile')

env.SConscript(dirs=['test'],
               exports=['env', 'libs'],
               variant_dir='build/test',
               duplicate=0)

env.SConscript(dirs=['example'],
               exports=['env', 'libs'],
               variant_dir='build/example',
               duplicate=0)

# vim:ft=python ts=4 sw=4 et
