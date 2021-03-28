#ident	"@(#)makefile	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."


include	$(IROOT)/macdefs
include	$(IROOT)/sgsdefs

#******************************************************************************
#									      *
#		UNIX Symbol Table Examination (nm) Makefile		      *
#									      *
#******************************************************************************
#									      *
#		Header/Data Files					      *
#									      *
#		com/decotype.h						      *
#		com/defs.h						      *
#		com/prosym.h						      *
#									      *
#******************************************************************************
#									      *
#		Source Code Files					      *
#									      *
#		com/decotype.c						      *
#		com/fcns.c						      *
#		com/main.c						      *
#		com/process.c						      *
#		com/prosym.c						      *
#		com/setflags.c						      *
#									      *
#******************************************************************************
#									      *
#		Library Files						      *
#									      *
#		/lib/libld.a						      *
#									      *
#******************************************************************************

# POSIX

SVID_FLAG =

BASE = ../..
BINDIR = $(ROOT)/bin
ARCH = AR32WR
SGS =
#COMI = $(BASE)/inc/common
#SGSI = $(BASE)/inc/mach
COMI = $(INCRT)
SGSI = $(BASE)/inc/mach
FLEX = -DFLEXNAMES

CDIR = $(BASE)/nm/common

LPT = dpr -mx

OUTFIL = output.tmp

PPFLAGS = $(FLEX) -DPORTAR -DUNIX -D$(ARCH) -I$(SGSI) -I$(COMI)

CFILES = $(CDIR)/decotype.c $(CDIR)/fcns.c $(CDIR)/main.c \
	 $(CDIR)/process.c $(CDIR)/prosym.c $(CDIR)/setflags.c

HFILES = $(CDIR)/decotype.h $(CDIR)/defs.h $(CDIR)/prosym.h \
	 $(COMI)/syms.h $(COMI)/storclass.h \
	 $(COMI)/filehdr.h $(COMI)/ldfcn.h $(SGSI)/sgs.h $(SGSI)/paths.h

OFILES = main.o setflags.o process.o prosym.o decotype.o fcns.o

all:	nm


nm:	$(OFILES) $(HFILES) 
	$(CC) $(CFLAGS) -o nm $(OFILES) -lld $(LDFLAGS)


install:	nm
	$(INS) nm $(BINDIR)/nm
	chmod 755 $(BINDIR)/nm
	chgrp bin $(BINDIR)/nm
	chown bin $(BINDIR)/nm

main.o:  $(CDIR)/main.c $(COMI)/filehdr.h $(COMI)/ldfcn.h $(SGSI)/sgs.h \
	 $(CDIR)/defs.h
	$(CC) $(PPFLAGS) $(CFLAGS) -c $(CDIR)/main.c

setflags.o:  $(CDIR)/setflags.c $(SGSI)/sgs.h $(SGSI)/paths.h $(CDIR)/defs.h
	$(CC) $(PPFLAGS) $(CFLAGS) -c $(CDIR)/setflags.c

process.o:  $(CDIR)/process.c $(COMI)/filehdr.h $(COMI)/ldfcn.h \
	    $(COMI)/scnhdr.h $(CDIR)/defs.h
	$(CC) $(PPFLAGS) $(CFLAGS) -c $(CDIR)/process.c

prosym.o:  $(CDIR)/prosym.c $(COMI)/filehdr.h $(COMI)/ldfcn.h $(COMI)/syms.h \
	   $(COMI)/storclass.h $(CDIR)/defs.h $(CDIR)/prosym.h
	$(CC) $(PPFLAGS) $(CFLAGS) -c $(CDIR)/prosym.c

decotype.o:  $(CDIR)/decotype.c $(COMI)/filehdr.h $(COMI)/ldfcn.h \
	     $(COMI)/syms.h $(COMI)/storclass.h $(CDIR)/decotype.h \
	     $(CDIR)/defs.h
	$(CC) $(PPFLAGS) $(CFLAGS) -c $(CDIR)/decotype.c

fcns.o:  $(CDIR)/fcns.c $(COMI)/filehdr.h $(COMI)/ldfcn.h $(SGSI)/sgs.h \
	 $(CDIR)/defs.h
	$(CC) $(PPFLAGS) $(CFLAGS) -c $(CDIR)/fcns.c


clean:	
	-rm -f $(OFILES)

clobber:	clean
	-rm -f nm ../common/greplist ../common/tags
listing:
	pr -n makefile $(HFILES) $(CFILES) | $(LPT)
	@echo "files sent to lineprinter!"

flow:
	cflow $(PPFLAGS) $(CFILES) > $(OUTFIL)

flowr:
	cflow -r $(PPFLAGS) $(CFILES) > $(OUTFIL)

SOURCES=$(CFILES) $(HFILES)
ctags:
	ctags $(SOURCES);mv tags ../common/tags
	echo 'grep $$* \\' >../common/greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> ../common/greplist
	echo /dev/null >> ../common/greplist
	chmod +x ../common/greplist
