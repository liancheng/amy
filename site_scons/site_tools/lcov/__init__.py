import SCons

from SCons.Builder import Builder
from SCons.Script import Dir, Flatten, Mkdir

from os import path


class ToolLCovWarning(SCons.Warnings.Warning):
    pass


class LCovExecutableNotFound(ToolLCovWarning):
    pass


def lcov_generator(source, target, env, for_signature):
    if 'LCOVRESET' in env:
        cmd = ['lcov --zerocounters']
    else:
        cmd = ['lcov --capture']
        cmd += ['--output-file', target[0].abspath]

    if 'LCOVDIR' in env:
        cmd += ['--directory', str(Dir(env['LCOVDIR']))]

    if 'LCOVBASEDIR' in env:
        cmd += ['--base-directory', str(Dir(env['LCOVBASEDIR']))]

    return ' '.join(Flatten(cmd))


_lcov_builder = Builder(generator=lcov_generator)


def generate(env):
    env['LCov'] = _detect(env)
    env['BUILDERS']['LCov'] = _lcov_builder

def _detect(env):
    try:
        return env['LCov']
    except KeyError:
        pass

    lcov = env.WhereIs('lcov')
    if lcov:
        return lcov

    raise SCons.Errors.StopError(LCovExecutableNotFound,
                                 'Cound not detect lcov executable')

    return None


def exists(env):
    return _detect(env)
