SUFFIXES = .bmp

ICONS  = \
        small_calc.bmp   \
        big_calc.bmp

all: ../icons.h ../icons.cpp

../icons.h: $(ICONS)
	$(RESWRAP) -i -o ../icons.h $^

../icons.cpp: $(ICONS)
	$(RESWRAP) -e -o ../icons.cpp $^

CLEANFILES = ../icons.h ../icons.cpp

EXTRA_DIST = $(ICONS)

