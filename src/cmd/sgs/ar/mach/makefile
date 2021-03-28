#ident	"@(#)makefile	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#ATT	ident	"ar:m32/makefile	1.11"


include	$(IROOT)/macdefs
include	$(IROOT)/sgsdefs


#***************************************************************************
#                                                                          *
#			UNIX Archive (ar) makefile			   *
#									   *
#***************************************************************************

# POSIX binary

SVID_FLAG =

BINDIR	= $(ROOT)/bin
BASE	= ../..
COMI	= $(BASE)/inc/common
INCMACH	= $(BASE)/inc/mach
CDIR	= $(BASE)/ar/common
FLEX	= -DFLEXNAMES
ARFORMAT= PORTAR

OWN	= bin
GRP	= bin

#USRINC= $(ROOT)/usr/$(VERSION)include
USRINC= $(INCRT)

CFILES = $(CDIR)/ar.c

#hanna -- out for now
#HFILES = $(COMI)/ar.h $(COMI)/filehdr.h $(COMI)/syms.h $(INCMACH)/paths.h

LDLIBS =
DEFLIST	= $(FLEX) -D$(ARFORMAT)
INCLIST	= -I$(COMI) -I$(INCMACH) -I$(USRINC)


all:	ar

ar:	$(CFILES) $(HFILES) 
	$(CC) $(CFLAGS) $(DEFLIST) $(INCLIST) $(CFILES) -o ar -lld $(LDLIBS) $(LDFLAGS)

install:	ar
	$(INS)	ar	$(BINDIR)/ar
	chmod	755	$(BINDIR)/ar
	chgrp	$(GRP)	$(BINDIR)/ar
	chown	$(OWN)	$(BINDIR)/ar

listing:
	pr -n makefile $(HFILES) $(CFILES) | $(LP)
	@echo "files sent to lineprinter"

clean:

flow:
	cflow $(PPFLAGS) $(CFILES) > $(OUTFIL)

flowr:
	cflow -r $(PPFLAGS) $(CFILES) > $(OUTFIL)

shrink:		clobber

clobber:	clean
	-rm -f ar greplist tags

SOURCES=$(CFILES) $(HFILES)
ctags:
	ctags $(SOURCES)
	echo 'grep $$* \\' >greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
