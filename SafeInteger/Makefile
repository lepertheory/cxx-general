OS=windows

ifeq (${OS}, linux)
  EXEEXT=
  DELCMD=rm -f
else
  EXEEXT=.exe
  DELCMD=del /F /Q
endif

COMPILER=g++
DEBUG=-g
STANDARD=-ansi
ERRORS=-Wall
INCLUDE=-I..
OPTIMIZE=-O0

BUILDCMD=${COMPILER} ${DEBUG} ${STANDARD} ${ERRORS} ${OPTIMIZE} ${INCLUDE}

all: SafeInteger${EXEEXT}

SafeInteger${EXEEXT}: main.o
	${BUILDCMD} -o SafeInteger${EXEEXT} main.o

main.o: test.cxx SafeInteger.hxx ../Exception.hxx ../toString.hxx
	${BUILDCMD} -c -o main.o test.cxx

clean:
	${DELCMD} SafeInteger${EXEEXT}
	${DELCMD} main.o

love:
	@echo "Not war."
