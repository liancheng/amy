import os
import platform

system = platform.system()
libpath = [Dir('/usr/lib'), Dir('/usr/local/lib')] +\
          [Dir('/usr/local/lib/mysql')] if system == 'FreeBSD' else []

use_boost_asio = ARGUMENTS.get('USE_BOOST_ASIO', 0)

env = Environment(tools=['default', 'lcov', 'genhtml'],
                  ENV=os.environ,
                  CXX=os.environ.get('CXX', 'clang++'),
                  CCFLAGS=['-ggdb', '-O0', '-std=c++11'],
                  CPPPATH=[Dir('include'),
                           Dir('/usr/local/include')],
                  CPPDEFINES=[('USE_BOOST_ASIO', use_boost_asio)],
                  LIBPATH=libpath,
                  LIBS=['mysqlclient', 'pthread'])

if use_boost_asio:
    libs += ['boost_system']

env.SConscript(dirs=['test'],
               exports='env',
               variant_dir='build/test',
               duplicate=0)

env.SConscript(dirs=['example'],
               exports='env',
               variant_dir='build/example',
               duplicate=0)

# vim:ft=python sw=4 ts=4 tw=80 et
