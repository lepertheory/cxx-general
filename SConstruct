import os

env = Environment(
	ENV     = os.environ,
	CPPPATH = ['.'],
	tools   = ['mingw', 'gcc']
)

if (env['CC'] == 'gcc') :
	env.Append(CCFLAGS = '-g -O0 -ansi -w -pipe')

SafeInteger_test = env.Object(target = 'SafeInteger-test', source = ['SafeInteger-test.cxx'])
ArbInt_test      = env.Object(target = 'ArbInt-test',      source = ['ArbInt-test.cxx']     )
Arb              = env.Object(target = 'Arb',              source = ['Arb.cxx']             )
Arb_test         = env.Object(target = 'Arb-test',         source = ['Arb-test.cxx']        )

env.Program(target = 'SafeInteger-test', source = [SafeInteger_test])
env.Program(target = 'ArbInt-test',      source = [ArbInt_test])
env.Program(target = 'Arb-test',         source = [Arb_test, Arb])
