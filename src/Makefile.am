INCLUDES = -I$(top_srcdir)/include

SUFFIXES = .bmp

bin_PROGRAMS = fxcalc

ICONS  = \
        $(top_srcdir)/icons/small_calc.bmp   \
        $(top_srcdir)/icons/big_calc.bmp

fxcalc_SOURCES = \
                $(top_srcdir)/include/icons.h \
		icons.cpp        \
		CALCDigitBox.cpp \
		CALCList.cpp     \
		CALCWindow.cpp   \
		CALCLabel.cpp    \
		CALCStatBox.cpp  \
                calc.cpp

CALCWindow.cpp: $(top_srcdir)/include/icons.h icons.cpp


$(top_srcdir)/include/icons.h: $(ICONS)
	$(RESWRAP) -i -o $(top_srcdir)/include/icons.h $^

icons.cpp: $(ICONS)
	$(RESWRAP) -e -o icons.cpp $^


CLEANFILES = $(top_srcdir)/include/icons.h icons.cpp

EXTRA_DIST = $(ICONS)
