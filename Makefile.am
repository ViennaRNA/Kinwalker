SUBDIRS = man

bin_PROGRAMS = kinwalker
kinwalker_SOURCES = main.cpp \
                    Energy.cpp Energy.h \
                    MorganHiggs.cpp MorganHiggs.h \
                    Node.cpp Node.h \
                    options.c options.h \
                    Util.cpp Util.h 

LDADD = $(RNALIB_LIBS)

AM_CPPFLAGS = $(RNALIB_CFLAGS)  

EXTRA_DIST = INSTALL template_utils.c kinfold_test.seq
