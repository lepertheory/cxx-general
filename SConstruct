# Imports.
import os

# Create the environment.
env = Environment(
  ENV     = os.environ,
  CPPPATH = ['.'],
  tools   = ['mingw', 'gcc', 'msvc']
)

# Should we configure?
doconfig = BUILD_TARGETS != []

# Set compiler options.
if (env['CC'] == 'gcc') :
  # -g enables debug mode.
  # -O0 disables optimization.
  # -ansi ensures this is strict ANSI code.
  # -pedantic-errors enables additional error checking.
  # -Wall enables all warnings.
  # -pipe communicates between CPP and compiler with a pipe instead of tmp.
  env.Append(CCFLAGS = '-g -O0 -ansi -pedantic-errors -Wall -pipe')
if (env['CC'] == 'cl') :
  # /GR enables RTTI.
  # /EHsc enables synchronous exceptions, assumes that extern C functions
  #   never throw.
  # /Od disables optimization.
  # /Wp64 checks for 64-bit portability problems.
  # /Za disables Microsoft extensions.
  # /Wall enables all warnings. -- turned off because Microsoft's own headers
  #   trigger tons of warnings.
  env.Append(CCFLAGS = '/GR /EHsc /Od /Wp64 /Za')

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