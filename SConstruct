import os
import platform

system = platform.system()
mysqlclient_lib = 'mysqlclient_r'
extra_libpaths = []

if system == 'Darwin':
    # `mysqlclient_r' is not available under Mac OS X
    mysqlclient_lib = 'mysqlclient'

if system == 'FreeBSD':
    # Default path where `pkg` puts `mysql-connector-c'
    extra_libpaths = [Dir('/usr/local/lib/mysql')]

env = Environment(ENV=os.environ,
                  CCFLAGS='-ggdb',
                  CPPPATH=[Dir('include'),
                           Dir('/usr/local/include')],
                  CPPDEFINES=[('BOOST_PARAMETER_MAX_ARITY', 6)],
                  LIBPATH=[Dir('/usr/lib')] + extra_libpaths)

libs = ['boost_filesystem',
        'boost_system',
        'boost_thread-mt',
        'pthread',
        'z',
        mysqlclient_lib]

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
