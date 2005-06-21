# Imports.
import os

# Create options.
opts = Options('custom.py')
opts.Add(BoolOption('RELEASE', 'Set for a release build.', default = 0))
opts.Add(EnumOption('TOOLSET', 'Select the toolkit used to build.', allowed_values = ('mingw', 'gcc', 'msvc', 'default'), default = 'default'))

# Create the environment.
env = Environment(
  options = opts,
  ENV     = os.environ,
  CPPPATH = ['.'],
  tools   = []
)

# Set the toolset to use.
if env['TOOLSET'] == 'default' :
  env.Tool('mingw')
  env.Tool('gcc')
# env.Tool('msvc') -- This just isn't ready yet.
else :
  env.Tool(env['TOOLSET'])

# Generate help text.
Help(opts.GenerateHelpText(env))

# Set compiler options.
if env['CC'] == 'gcc' :
  # -g enables debug mode.
  # -O0 disables optimization.
  # -ansi ensures this is strict ANSI code.
  # -pedantic-errors enables additional error checking.
  # -Wall enables all warnings.
  # -Wextra enables even more warnings.
  # -Wfloat-equal warns if the == operator is used on a float.
  # -Wshadow warns when something is shadowed.
  # -Wpointer-arith warns when using size of a function or void.
  # -Wcast-qual warns when const is cast away.
  # -Wwrite-strings protects strings.
  # -Wconversion warns about unexpected conversions.
  # -Waggregate-return warns if trying to return a non-integral type.
  # -Wredundant-decls warns about redundant declarations.
  # -Wunreachable-code warns if unreachable code exists.
  # -Winline warns if a function declared inline cannot be inlined.
  # -Werror turns all warnings into errors.
  # -pipe communicates between CPP and compiler with a pipe instead of tmp.
  env.Append(CCFLAGS    = '-g -O0 -ansi -pedantic-errors -Wall -Wextra -Wfloat-equal -Wshadow -Wpointer-arith -Wredundant-decls -Wunreachable-code -Winline -Werror -pipe')
  env.Append(CPPDEFINES = 'CC_GCC')
if env['CC'] == 'cl' :
  # /GR enables RTTI.
  # /EHsc enables synchronous exceptions, assumes that extern C functions
  #   never throw.
  # /Od disables optimization.
  # /Wp64 checks for 64-bit portability problems.
  # /Za disables Microsoft extensions.
  # /Wall enables all warnings. -- turned off because Microsoft's own headers
  #   trigger tons of warnings.
  env.Append(CCFLAGS    = '/GR /EHsc /Od /Wp64 /Za')
  env.Append(CPPDEFINES = 'CC_CL')

# Do not build everything by default, do clean everything.
if not env.GetOption('clean') :
  env.Default(None)

# Determine the platform.
if (env['PLATFORM'] == 'win32') :
  env.Append(CPPDEFINES = [ 'PLAT_WIN32' ])
elif (env['PLATFORM'] == 'posix') :
  env.Append(CPPDEFINES = [ 'PLAT_POSIX' ])

# Make a backup of env so SConscript files do not modify it.
tmpenv = env.Copy()

cArbInt    = SConscript(['ArbInt/SConscript'],    exports = 'env')         ; env = tmpenv.Copy()
cArb       = SConscript(['Arb/SConscript'],       exports = 'env cArbInt') ; env = tmpenv.Copy()
cTimestamp = SConscript(['Timestamp/SConscript'], exports = 'env cArb')    ; env = tmpenv.Copy()

SConscript(['Tests/SConscript'],     exports = 'env cArbInt cArb cTimestamp') ; env = tmpenv.Copy()