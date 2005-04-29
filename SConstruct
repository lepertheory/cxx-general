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
  env.Append(CCFLAGS = '-g -O0 -ansi -w -pipe')

# Create an environment for platform-dependant modules.
env_platd = env.Copy()

# Determine the platform.
if (env_platd['PLATFORM'] == 'win32') :
  env_platd.Append(CPPDEFINES = [ 'PLAT_WIN32' ])
elif (env_platd['PLATFORM'] == 'posix') :
  env_platd.Append(CPPDEFINES = [ 'PLAT_POSIX' ])

# Create a special environment for Timestamp.
env_Timestamp = env_platd.Copy()

# Configure Timestamp's environment.
if doconfig :
  conf = Configure(env_Timestamp)
  if (env_Timestamp['PLATFORM'] == 'win32') :
    if conf.CheckHeader(header = 'windows.h', language = 'C++') :
      env_Timestamp.Append(CPPDEFINES = [ 'HAS_WINDOWS_H' ])
      if conf.CheckType(type_name = '_SYSTEMTIME', includes = '#include <windows.h>', language = 'C++') :
        env_Timestamp.Append(CPPDEFINES = [ 'HAS__SYSTEMTIME' ])
  elif (env_Timestamp['PLATFORM'] == 'posix') :
    if conf.CheckHeader(header = 'time.h',     language = 'C++') :
      env_Timestamp.Append(CPPDEFINES = [ 'HAS_TIME_H' ])
      if conf.CheckFunc(function_name = 'time',     language = 'C++') :
        env_Timestamp.Append(CPPDEFINES = [ 'HAS_TIME' ])
      if conf.CheckFunc(function_name = 'gmtime',   language = 'C++') :
        env_Timestamp.Append(CPPDEFINES = [ 'HAS_GMTIME' ])
      if conf.CheckFunc(function_name = 'gmtime_r', language = 'C++') :
        env_Timestamp.Append(CPPDEFINES = [ 'HAS_GMTIME_R' ])
      if conf.CheckType(type_name = 'time_t', includes = '#include <time.h>', language = 'C++') :
        env_Timestamp.Append(CPPDEFINES = [ 'HAS_TIME_T' ])
      if conf.CheckType(type_name = 'tm',     includes = '#include <time.h>', language = 'C++') :
        env_Timestamp.Append(CPPDEFINES = [ 'HAS_TM' ])
    if conf.CheckHeader(header = 'sys/time.h', language = 'C++') :
      env_Timestamp.Append(CPPDEFINES = [ 'HAS_SYS_TIME_H' ])
      if conf.CheckFunc(function_name = 'gettimeofday', language = 'C++') :
        env_Timestamp.Append(CPPDEFINES = [ 'HAS_GETTIMEOFDAY' ])
      if conf.CheckType(type_name = 'timeval',     includes = '#include <sys/time.h>', language = 'C++') :
        env_Timestamp.Append(CPPDEFINES = [ 'HAS_TIMEVAL' ])
      if conf.CheckType(type_name = 'timezone',    includes = '#include <sys/time.h>', language = 'C++') :
        env_Timestamp.Append(CPPDEFINES = [ 'HAS_TIMEZONE' ])
  env_Timestamp = conf.Finish()

# Object files.
SafeInteger_test =           env.Object(target = 'SafeInteger-test', source = ['SafeInteger-test.cxx'])
ArbInt_test      =           env.Object(target = 'ArbInt-test',      source = ['ArbInt-test.cxx']     )
Arb              =           env.Object(target = 'Arb',              source = ['Arb.cxx']             )
Arb_test         =           env.Object(target = 'Arb-test',         source = ['Arb-test.cxx']        )
Timestamp        = env_Timestamp.Object(target = 'Timestamp',        source = ['Timestamp.cxx']       )
Timestamp_test   =           env.Object(target = 'Timestamp-test',   source = ['Timestamp-test.cxx']  )
ibc              =           env.Object(target = 'ibc',              source = ['ibc.cxx']             )

# Program files.
env.Program(target = 'SafeInteger-test', source = [SafeInteger_test]              )
env.Program(target = 'ArbInt-test',      source = [ArbInt_test]                   )
env.Program(target = 'Arb-test',         source = [Arb_test, Arb]                 )
env.Program(target = 'Timestamp-test',   source = [Arb, Timestamp, Timestamp_test])
env.Program(target = 'ibc',              source = [ibc]                           )

# Do not build everything by default, do clean everything.
if not env.GetOption('clean') :
  env.Default(None)
