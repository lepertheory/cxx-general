# Version of the library.
cxxgeneral_maj_version = '0'
cxxgeneral_min_version = '1'

# Imports.
import os

# Create options.
opts = Options('custom.py')
opts.Add(BoolOption('RELEASE', 'Set for a release build.', default = 0))

# Create the environment.
env = Environment(
  options    = opts,
  ENV        = os.environ,
  CPPPATH    = ['.'],
  INSTALLDIR = '/usr',
)

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
  env.Append(CCFLAGS    = '-g -O0 -ansi -pedantic-errors -Wall -Wextra -Wshadow -Wpointer-arith -Wredundant-decls -Wunreachable-code -Winline -Werror -pipe')
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

# Make a backup of env so SConscript files do not modify it.
tmpenv = env.Copy()

cArbInt    = SConscript(['ArbInt/SConscript'   ], exports = 'env'        ) ; env = tmpenv.Copy()
cArb       = SConscript(['Arb/SConscript'      ], exports = 'env cArbInt') ; env = tmpenv.Copy()
cTimestamp = SConscript(['Timestamp/SConscript'], exports = 'env cArb'   ) ; env = tmpenv.Copy()
cPOSIXFile = SConscript(['POSIXFile/SConscript'], exports = 'env'        ) ; env = tmpenv.Copy()

SConscript(['Tests/SConscript'], exports = 'env cArbInt cArb cTimestamp cPOSIXFile') ; env = tmpenv.Copy()

# Determine the soname.
cxxgeneral_name   = env['LIBPREFIX'] + 'cxx-general' + env['SHLIBSUFFIX']
cxxgeneral_soname = cxxgeneral_name   + '.' + cxxgeneral_maj_version
cxxgeneral_rname  = cxxgeneral_soname + '.' + cxxgeneral_min_version

# The library
cxxgeneral_objs = []
for obj in cArbInt['own_sobj'] :
  cxxgeneral_objs += obj
for obj in cArb['own_sobj'] :
  cxxgeneral_objs += obj
for obj in cTimestamp['own_sobj'] :
  cxxgeneral_objs += obj
for obj in cPOSIXFile['own_sobj'] :
  cxxgeneral_objs += obj
cxxgeneral = env.SharedLibrary(target = cxxgeneral_rname, source = cxxgeneral_objs, SHLINKFLAGS = '-shared -Wl,-soname,' + cxxgeneral_soname, SHLIBSUFFIX = '')

# Make symlinks to the libray.
try :
  os.unlink(cxxgeneral_name)
except : pass
try :
  os.unlink(cxxgeneral_soname)
except : pass
if not env.GetOption('clean') :
  os.symlink(cxxgeneral_rname, cxxgeneral_name  )
  os.symlink(cxxgeneral_rname, cxxgeneral_soname)

# Build the library by default.
if env.GetOption('clean') :
  env.Default('.')
else :
  env.Default(cxxgeneral)

# Top level header files.
headers = list()
h_AutoArray        = env.File('AutoArray.h++'       ) ; headers += [h_AutoArray       ]
h_Exception        = env.File('Exception.h++'       ) ; headers += [h_Exception       ]
h_ReferencePointer = env.File('ReferencePointer.h++') ; headers += [h_ReferencePointer]
h_SafeInt          = env.File('SafeInt.h++'         ) ; headers += [h_SafeInt         ]
h_abs              = env.File('abs.h++'             ) ; headers += [h_abs             ]
h_demangle         = env.File('demangle.h++'        ) ; headers += [h_demangle        ]
h_gcd              = env.File('gcd.h++'             ) ; headers += [h_gcd             ]
h_lcm              = env.File('lcm.h++'             ) ; headers += [h_lcm             ]
h_logBase          = env.File('logBase.h++'         ) ; headers += [h_logBase         ]
h_reduce           = env.File('reduce.h++'          ) ; headers += [h_reduce          ]
h_rppower          = env.File('rppower.h++'         ) ; headers += [h_rppower         ]
h_toString         = env.File('toString.h++'        ) ; headers += [h_toString        ]

# Alias for install location.
includedir = env['INSTALLDIR'] + '/include/cxx-general'
libdir     = env['INSTALLDIR'] + '/lib'
installdirs = [includedir, libdir]
installdirs.append(includedir + '/' + cArbInt   ['own_include'])
installdirs.append(includedir + '/' + cArb      ['own_include'])
installdirs.append(includedir + '/' + cTimestamp['own_include'])
installdirs.append(includedir + '/' + cPOSIXFile['own_include'])
env.Alias('install', installdirs)

# Installation.
if BUILD_TARGETS[0] == 'install' :
  for dir in installdirs :
    try :
      os.mkdir(dir)
    except : pass
Install(includedir                                  , headers                  )
Install(includedir + '/' + cArbInt   ['own_include'], cArbInt   ['own_headers'])
Install(includedir + '/' + cArb      ['own_include'], cArb      ['own_headers'])
Install(includedir + '/' + cTimestamp['own_include'], cTimestamp['own_headers'])
Install(includedir + '/' + cPOSIXFile['own_include'], cPOSIXFile['own_headers'])
Install(libdir                                      , cxxgeneral               )
Install(libdir                                      , cxxgeneral_name          )
Install(libdir                                      , cxxgeneral_soname        )
