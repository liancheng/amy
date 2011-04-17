import os

env = Environment(ENV=os.environ,
                  CCFLAGS='-ggdb',
                  CPPPATH=Dir('include'))

env.Execute('@doxygen Doxyfile')

env.SConscript(dirs=['test'],
               exports='env',
               variant_dir='build/test',
               duplicate=0)

env.SConscript(dirs=['example'],
               exports='env',
               variant_dir='build/example',
               duplicate=0)

# vim:ft=python ts=4 sw=4 et
