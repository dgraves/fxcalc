SUFFIXES = .bmp

ICONS  = \
        small_calc.bmp   \
        big_calc.bmp

all: $(top_srcdir)/include/icons.h $(top_srcdir)/src/icons.cpp

$(top_srcdir)/include/icons.h: $(ICONS)
	$(RESWRAP) -i -o $(top_srcdir)/include/icons.h $^

$(top_srcdir)/src/icons.cpp: $(ICONS)
	$(RESWRAP) -e -o $(top_srcdir)/src/icons.cpp $^

CLEANFILES = $(top_srcdir)/include/icons.h $(top_srcdir)/src/icons.cpp

EXTRA_DIST = $(ICONS)
