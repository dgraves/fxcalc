# Makefile for doubledouble library.  K M Briggs 98 Jan 19
# To configure:
# 1. Select one of the sets of definitions of CPP, OPTS, INC, DEF, RANLIB below
#    The symbols have these meanings...
#      CPP:  C++ compiler name
#      OPTS: C++ compiler options
#      INC:  C++ include path
#      DEF:  preprocessor defines
#      RANLIB: ranlib program, if present
# 2. If you would like to be added to the doubledouble email list,
#    remove the # in the next line.    Your email address will not be passed
#    to anyone else, and will only be used for infrequent upgrade notices.
MAIL=#@echo `whoami`@`hostname` has tested doubledouble 2.2 | mail kmb28@cam.ac.uk

# linux-x86, standard setup...
CPP=g++
OPTS=-m486 -O3
INC=
DEF=-DDD_INLINE -Dx86
RANLIB=ranlib

# Generic Unix...
#CPP=g++
#OPTS=-O3
#INC=
#DEF=-DDD_INLINE
#RANLIB=ranlib

# linux-x86, special KMB setup...
#CPP=g++-old
#OPTS=-m486 -O3 -Wall
#INC=-I/usr/include/g++ -I/usr/lib/gcc-lib/i386-linux/2.7.2.1/include/
#DEF=-DDD_INLINE -Dx86
#RANLIB=ranlib

# mips-sgi-irix with CC...
#CPP=CC
#OPTS=-O
#INC=
#DEF=-DDD_INLINE -DSGI_CC
#RANLIB=ls # nop

# mips-sgi-irix with egcs...
#CPP=g++ # this must be g++ >=2.8, or better egcs-1.0
#OPTS=-O
#INC=
#DEF=-DDD_INLINE
#RANLIB=ls # nop

# Sun sparc solaris...
#CPP=g++
#OPTS=-O
#INC=
#DEF=-DDD_INLINE -DSUN
#RANLIB=ls # nop

####################### Do not edit below here ##############################

PKG=README INSTALL ANNOUNCE COPYLEFT doubledouble.html Makefile doubledouble.h inline.h doubledouble.cpp math.cpp trydd.cpp

trydd: trydd.o libdoubledouble.a
	$(CPP) $(OPTS) -L. trydd.o -o trydd -ldoubledouble -lm
	time ./trydd
	$(MAIL)

trydd.o: trydd.cpp doubledouble.h inline.h
	$(CPP) $(INC) $(OPTS) $(DEF) -c trydd.cpp

doubledouble.o: doubledouble.cpp doubledouble.h inline.h
	$(CPP) $(INC) $(OPTS) $(DEF) -c doubledouble.cpp

math.o: math.cpp doubledouble.h inline.h
	$(CPP) $(INC) $(OPTS) $(DEF) -c math.cpp

libdoubledouble.a: math.o doubledouble.o
	ar ru libdoubledouble.a math.o doubledouble.o
	$(RANLIB) libdoubledouble.a
tgz:
	tar zcf doubledouble.tgz $(PKG)
shar:
	shar -V -c $(PKG) > doubledouble.shar
html:
	c2html <doubledouble.h >doubledouble.h.html
	c2html <doubledouble.cpp >doubledouble.cpp.html
	c2html <inline.h >inline.h.html
	c2html <math.cpp >math.cpp.html

