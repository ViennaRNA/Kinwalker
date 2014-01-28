# $Id: Makefile,v 1.10 2007/10/01 08:29:47 Kinwalker Exp $
# vars
#debug variables
#make DEFS="-D_DEBUG_MH_ -D_DEBUG_MH_GETCOMBINATION_ -D_DEBUG_MH_REGROUP_"
ISODATE := $(shell date +%g%m%d-%H%M%S)
BACKDIR  = $(strip Kinwalker-$(ISODATE))
CC       = gcc
#CXX      = g++-3.4
#LL       = g++-3.4
CXX      = g++
LL       = g++
DEFS     =
VRNADIR  = ${HOME}/C/ViennaRNA
INCL     = -I. -I$(VRNADIR)/H
WARN     = -Wall -ansi -pedantic
OPTI     = -O3
#OPTI     = -O3 -p -g3
CFLAGS   = $(WARN) $(OPTI) $(DEFS) $(INCL)
LIBS     = -L$(VRNADIR)/lib -lRNA  -lm -lstdc++
CSRC     = options.c findpath.c
COBJ     = $(foreach file, $(CSRC:.c=.o), $(file))
CXXSRC   = Energy.cpp Node.cpp Util.cpp MorganHiggs.cpp main.cpp
CXXOBJ   = $(foreach file, $(CXXSRC:.cpp=.o), $(file))
HFILES   = Energy.h MorganHiggs.h Node.h options.h Util.h
XTRA     = Makefile $(HFILES) \
           doc/*.sty \
           doc/Makefile \
           doc/*.bib \
           doc/*.tex
EXE      = kinwalker
# rules
all: $(EXE)
$(CXXOBJ): %.o: %.cpp
	$(CXX) -c $(CFLAGS) $< -o $@
$(COBJ): %.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@
$(EXE): $(COBJ) $(CXXOBJ)
	$(LL) $(CFLAGS) -o $(EXE) $(COBJ) $(CXXOBJ) $(LIBS)
clean:
	rm -f $(COBJ) $(CXXOBJ) $(EXE)
backup: clean
	mkdir $(BACKDIR); mkdir $(BACKDIR)/test; mkdir $(BACKDIR)/doc;
	cp Makefile template_utils.c $(HFILES) $(CSRC) $(CXXSRC) $(BACKDIR)
	cp test/*.cpp test/*.mk test/Makefile test/*.seq $(BACKDIR)/test
	cp doc/*.sty doc/*.bib doc/*.tex doc/Makefile $(BACKDIR)/doc
#	find . -maxdepth 1 -name \*.c -or -name \*.cpp -or -name \*.h \
#	| xargs -t cp --target-directory=$(BACKDIR)
	tar cf - $(BACKDIR) | gzip --best -> $(BACKDIR).tar.gz
#	mv $(BACKDIR).tar.gz Arch
	rm -rf $(BACKDIR)
PHONY: clean
# End of file
