import os

env = Environment(ENV=os.environ,
                  CCFLAGS='-ggdb',
                  CPPPATH=Dir('include'))

libs = ['boost_filesystem',
        'boost_system',
        'boost_thread',
        'mysqlclient_r',
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
