# Imports.
import os

# Create the environment.
env = Environment(
  ENV     = os.environ,
  CPPPATH = ['.'],
  tools   = ['mingw', 'gcc']
)

# Should we configure?
doconfig = BUILD_TARGETS != []

# Set compiler options.
if (env['CC'] == 'gcc') :
  env.Append(CCFLAGS = '-g -O0 -ansi -pedantic-errors -Wall -Wno-long-long -pipe')

# Create an environment for platform-dependant modules.
env_platd = env.Copy()

# Determine the platform.
if (env_platd['PLATFORM'] == 'win32') :
  env_platd.Append(CPPDEFINES = [ 'PLAT_WIN32' ])
elif (env_platd['PLATFORM'] == 'posix') :
  env_platd.Append(CPPDEFINES = [ 'PLAT_POSIX' ])

# Do not build everything by default, do clean everything.
if not env.GetOption('clean') :
  env.Default(None)

SConscript(['Tests/SConscript'], exports = 'env')