#-----------------------------------------------------------------------------#
# Configuration.
#-----------------------------------------------------------------------------#

# Version of the library.
cxxgeneral_maj_version = open('config/LIB_VER_MAJOR', 'r').read()
cxxgeneral_min_version = open('config/LIB_VER_MINOR', 'r').read()

#-----------------------------------------------------------------------------#
# End of configuration.
#-----------------------------------------------------------------------------#

# Imports.
import sconsgeneral
import os

# Create options.
opts = Options('custom.py')
opts.AddOptions(
  BoolOption('DEBUG'  , 'Set for a debug build.'                                                          , default = 0                                       ),
  PathOption('PREFIX' , 'Prefix of hardcoded paths. Set this to the prefix the program will be run from.' , default = '/usr', validator = PathOption.PathIsDir),
  PathOption('DESTDIR', 'Install destination directory. Use this to install to a place other than PREFIX.', default = '/usr', validator = PathOption.PathIsDir)
)

# Create the environment.
env = Environment(
  options    = opts,
  ENV        = os.environ,
  CPPPATH    = ['.']
)

# Apply scons general stuff.
sconsgeneral.apply(env)

# Generate help text.
Help(opts.GenerateHelpText(env))

# Set compiler options.
if env['CC'] == 'gcc' :
  env.Append(CCFLAGS    = '-g -O0 -ansi -pedantic-errors -Wall -Wextra -Wshadow -Wpointer-arith -Wredundant-decls -Wunreachable-code -Winline -Werror -pipe')
  env.Append(CPPDEFINES = 'CC_GCC')
if env['CC'] == 'cl' :
  env.Append(CCFLAGS    = '/GR /EHsc /Od /Wp64 /Za')
  env.Append(CPPDEFINES = 'CC_CL')

# Compile-time paths.
includedir   = env['PREFIX'] + '/include/cxx-general'
libdir       = env['PREFIX'] + '/lib'
pkgconfigdir = libdir + '/pkgconfig'
env['project_includedir'] = includedir

# Install paths.
install_includedir   = env['DESTDIR'] + '/include/cxx-general'
install_libdir       = env['DESTDIR'] + '/lib'
install_pkgconfigdir = install_libdir + '/pkgconfig'

# Headers.
headers = []
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
h_getErrorText     = env.File('getErrorText.h++'    ) ; headers += [h_getErrorText    ]
h_CaseConvert      = env.File('CaseConvert.h++'     ) ; headers += [h_CaseConvert     ]

# Modules.
modules = []
tmpenv = env.Copy()
cArbInt    = SConscript(['ArbInt/SConscript'   ], exports = 'env'                           ) ; env = tmpenv.Copy() ; modules.append(cArbInt   )
cArb       = SConscript(['Arb/SConscript'      ], exports = 'env cArbInt'                   ) ; env = tmpenv.Copy() ; modules.append(cArb      )
cTimestamp = SConscript(['Timestamp/SConscript'], exports = 'env cArb'                      ) ; env = tmpenv.Copy() ; modules.append(cTimestamp)
cwrapText  = SConscript(['wrapText/SConscript' ], exports = 'env'                           ) ; env = tmpenv.Copy() ; modules.append(cwrapText )
ctrim      = SConscript(['trim/SConscript'     ], exports = 'env'                           ) ; env = tmpenv.Copy() ; modules.append(ctrim     )
cValReader = SConscript(['ValReader/SConscript'], exports = 'env cArbInt cArb'              ) ; env = tmpenv.Copy() ; modules.append(cValReader)
ctokenize  = SConscript(['tokenize/SConscript' ], exports = 'env'                           ) ; env = tmpenv.Copy() ; modules.append(ctokenize )
cINIFile   = SConscript(['INIFile/SConscript'  ], exports = 'env cValReader ctokenize ctrim') ; env = tmpenv.Copy() ; modules.append(cINIFile  )

# Tests.
SConscript(['Tests/SConscript'], exports = 'env cArbInt cArb cTimestamp cINIFile cValReader ctokenize ctrim') ; env = tmpenv.Copy()

# Shared library filenames.
cxxgeneral_name   = env['LIBPREFIX'] + env['project_name'] + env['SHLIBSUFFIX']
cxxgeneral_soname = cxxgeneral_name   + '.' + cxxgeneral_maj_version
cxxgeneral_rname  = cxxgeneral_soname + '.' + cxxgeneral_min_version

# Create the shared library.
cxxgeneral_objs = []
for module in modules :
  for obj in module.own_shared_objects :
    cxxgeneral_objs += obj
cxxgeneral = env.SharedLibrary(target = cxxgeneral_rname, SHLIBSUFFIX = '', source = cxxgeneral_objs, SHLINKFLAGS = '-Wl,-soname,' + cxxgeneral_soname + env['SHLINKFLAGS'])

# Create the pkg-config file.
pcfile = env.PkgConfig(target = env['project_name'], source = env['project_name'])

# Install files.
install = []
install.append(env.Install(install_includedir, headers))
for module in modules :
  install.append(env.Install(install_includedir, module.own_headers))
install.append(env.Install(install_libdir                        , cxxgeneral))
install.append(env.Symlink(install_libdir + '/' + cxxgeneral_name, cxxgeneral))
install.append(env.Install(install_pkgconfigdir, pcfile))

# Install alias.
env.Alias('install', install)

# Build the library by default.
if env.GetOption('clean') :
  env.Default('.')
else :
  env.Default(cxxgeneral, pcfile)
