SUBDIRS = icons

bin_PROGRAMS = fxcalc
fxcalc_SOURCES = \
		icons.h          \
		icons.cpp        \
		CALCdefs.h       \
		CALCHelp.h       \
		CALCDigitBox.h   \
		CALCDigitBox.cpp \
		CALCList.h       \
		CALCList.cpp     \
		CALCWindow.h     \
		CALCWindow.cpp   \
		CALCLabel.h      \
		CALCLabel.cpp    \
		CALCStatBox.h    \
		CALCStatBox.cpp  \
                calc.cpp

dist-hook:
	mkdir $(distdir)/windows
	cp $(srcdir)/windows/fxcalc.ds? $(srcdir)/windows/fxcalc.res $(srcdir)/windows/fxcalc.ico $(srcdir)/windows/reswrap.* $(distdir)/windows
	cp -dpR doubledouble $(distdir)/doubledouble

