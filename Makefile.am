SUBDIRS = man
bin_PROGRAMS = Kinwalker
Kinwalker_SOURCES = Energy.cpp Energy.h main.cpp MorganHiggs.cpp MorganHiggs.h Node.cpp Node.h options.c options.h Util.cpp Util.h 

LDADD = $(RNALIB_LIBS)
 #search for header files in ViennaRNA2 libraries, comes from the configure.ac file
AM_CPPFLAGS = $(RNALIB_CFLAGS)  

docu_DATA= INSTALL 
docudir= $(pkgdatadir)
	    
EXTRA_DIST = INSTALL template_utils.c kinfold_test.seq
	    
