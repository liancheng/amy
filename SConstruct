import os

env = Environment(ENV=os.environ,
                  CFLAGS='-ggdb',
                  CPPPATH=Dir('include'))

env.Execute('@doxygen Doxyfile')

env.SConscript(dirs=['test'],
               exports='env',
               variant_dir='build/test',
               duplicate=0)

# vim:ft=python ts=4 sw=4 et
