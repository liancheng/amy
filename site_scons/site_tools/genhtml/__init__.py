import SCons

from SCons.Builder import Builder
from SCons.Script import Dir, Flatten, Mkdir

from os import path


class ToolGenHtmlWarning(SCons.Warnings.Warning):
    pass


class GenHtmlExecutableNotFound(ToolGenHtmlWarning):
    pass


def genhtml_generator(source, target, env, for_signature):
    cmd = ['genhtml']
    cmd += [str(source[0])]
    cmd += ['--output-directory', str(target[0])]

    return ' '.join(Flatten(cmd))


_genhtml_builder = Builder(generator=genhtml_generator)


def generate(env):
    env['GenHtml'] = _detect(env)
    env['BUILDERS']['GenHtml'] = _genhtml_builder

def _detect(env):
    try:
        return env['GenHtml']
    except KeyError:
        pass

    genhtml = env.WhereIs('genhtml')
    if genhtml:
        return genhtml 

    raise SCons.Errors.StopError(GenHtmlExecutableNotFound,
                                 'Cound not detect genhtml executable')

    return None


def exists(env):
    return _detect(env)
