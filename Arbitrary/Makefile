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

all: Arbitrary${EXEEXT}

Arbitrary${EXEEXT}: main.o Arbitrary.o
	${BUILDCMD} -o Arbitrary${EXEEXT} Arbitrary.o main.o

main.o: test.cxx Arbitrary.hxx ../Exception.hxx ../ReferencePointer.hxx ../toString.hxx
	${BUILDCMD} -c -o main.o test.cxx

Arbitrary.o: Arbitrary.cxx Arbitrary.hxx ../Exception.hxx ../ReferencePointer.hxx ../toString.hxx ../rppower.hxx
	${BUILDCMD} -c -o Arbitrary.o Arbitrary.cxx

clean:
	${DELCMD} Arbitrary${EXEEXT}
	${DELCMD} main.o
	${DELCMD} Arbitrary.o

love:
	@echo "Not war."
