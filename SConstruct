import os

env = Environment(
	ENV     = os.environ,
	CPPPATH = ['.'],
	tools   = ['mingw', 'gcc']
)

if (env['CC'] == 'gcc') :
	env.Append(CCFLAGS = '-g -O0 -ansi -w -pipe')

if (env['PLATFORM'] == 'win32') :
	env.Append(CPPDEFINES = [ 'PLAT_WIN32' ])
elif (env['PLATFORM'] == 'posix') :
	env.Append(CPPDEFINES = [ 'PLAT_POSIX' ])

SafeInteger_test = env.Object(target = 'SafeInteger-test', source = ['SafeInteger-test.cxx'])
ArbInt_test      = env.Object(target = 'ArbInt-test',      source = ['ArbInt-test.cxx']     )
Arb              = env.Object(target = 'Arb',              source = ['Arb.cxx']             )
Arb_test         = env.Object(target = 'Arb-test',         source = ['Arb-test.cxx']        )
Timestamp        = env.Object(target = 'Timestamp',        source = ['Timestamp.cxx']       )
Timestamp_test   = env.Object(target = 'Timestamp-test',   source = ['Timestamp-test.cxx']  )
ibc              = env.Object(target = 'ibc',              source = ['ibc.cxx']             )

env.Program(target = 'SafeInteger-test', source = [SafeInteger_test]              )
env.Program(target = 'ArbInt-test',      source = [ArbInt_test]                   )
env.Program(target = 'Arb-test',         source = [Arb_test, Arb]                 )
env.Program(target = 'Timestamp-test',   source = [Arb, Timestamp, Timestamp_test])
env.Program(target = 'ibc',              source = [ibc]                           )
