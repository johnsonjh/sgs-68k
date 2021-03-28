#ident	"@(#)makefile	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.


include	$(IROOT)/macdefs
include	$(IROOT)/sgsdefs


#
#	BELLMAC-32 DUMP MAKEFILE
#
#
# POSIX

SVID_FLAG =

OWN=bin
GRP=bin
ARCH=AR32WR
#	Set ARFORMAT to "PORTAR" for portable archive format (post 5.0)
ARFORMAT=PORTAR
#	Set FLEX to -DFLEXNAMES for flexnames (post 6.0)
FLEX= -DFLEXNAMES
DEFLIST=-D$(ARCH) -D$(ARFORMAT) $(FLEX)
INCLIST=-I$(COMINC) -I$(COMDUMP) -I$(MACHINC) -I$(USRINC)
ENV=
CC_CMD=$(CC) -c $(CFLAGS) $(DEFLIST) $(INCLIST) $(ENV)
FFLAG=
#
#LDFLAGS=-i $(FFLAG)
#
LDLIBS=
#
STRIP=strip
SPFLAGS=
STRIP_CMD=$(STRIP) $(SPFLAGS)
#
LINT=lint
LINTFLAGS=-p
LINT_CMD=$(LINT) $(LINTFLAGS)
#
#	External Directories
VERSION=
#USRINC= $(ROOT)/usr/$(VERSION)include
USRINC= $(INCRT)
#
#
#	Internal Directories
BASE= ../..
BINDIR= $(ROOT)/bin
COMDUMP= $(BASE)/dump/common
COMINC= $(USRINC)
#COMINC= $(BASE)/inc/common
MACHINC= $(BASE)/inc/mach
#
OBJECTS= main.o setflags.o openobject.o process.o fcns.o
#
all:	dump
#

dump:	$(OBJECTS) 
	$(CC) -o dump $(OBJECTS) -lld $(LDLIBS) $(LDFLAGS)
#
main.o:	$(COMDUMP)/main.c $(USRINC)/filehdr.h $(COMINC)/ldfcn.h $(COMDUMP)/filedefs.h \
	 $(COMDUMP)/dumpmap.h $(MACHINC)/sgs.h
	$(CC_CMD) $(COMDUMP)/main.c
#
setflags.o:	$(COMDUMP)/setflags.c $(COMDUMP)/flagdefs.h $(MACHINC)/sgs.h
	$(CC_CMD) $(COMDUMP)/setflags.c
#
openobject.o:	$(COMDUMP)/openobject.c $(USRINC)/filehdr.h $(COMINC)/ldfcn.h \
	       $(MACHINC)/sgs.h $(COMDUMP)/filedefs.h
	$(CC_CMD) $(COMDUMP)/openobject.c
#
process.o:	$(COMDUMP)/process.c $(USRINC)/filehdr.h $(COMINC)/ldfcn.h $(MACHINC)/sgs.h\
	    $(COMDUMP)/filedefs.h $(COMDUMP)/process.h $(COMDUMP)/flagdefs.h
	$(CC_CMD) $(COMDUMP)/process.c
#
fcns.o:	$(COMDUMP)/fcns.c $(USRINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/reloc.h \
	 $(COMINC)/linenum.h $(COMINC)/syms.h $(COMINC)/storclass.h $(COMINC)/ldfcn.h \
	 $(MACHINC)/sgs.h
	$(CC_CMD) $(COMDUMP)/fcns.c
#
save:	$(BINDIR)/$(SGS)dump
	-rm -f $(BINDIR)/$(SGS)dump.back
	cp $(BINDIR)/$(SGS)dump $(BINDIR)/$(SGS)dump.back
#
install:	dump
	$(INS) dump $(BINDIR)/dump
	chmod 755 $(BINDIR)/dump
	chgrp $(GRP) $(BINDIR)/dump
	chown $(OWN) $(BINDIR)/dump

clean:
	rm -f *.o

clobber: clean
	rm -f dump tags greplist
#
uninstall:	$(BINDIR)/$(SGS)dump.back
	-rm -f $(BINDIR)/$(SGS)dump
	cp $(BINDIR)/$(SGS)dump.back $(BINDIR)/$(SGS)dump
#
lint:	$(COMDUMP)/main.c $(COMDUMP)/setflags.c $(COMDUMP)/openobject.c \
	$(COMDUMP)/process.c $(COMDUMP)/fcns.c \
	$(COMDUMP)/dumpmap.h \
	$(USRINC)/filehdr.h $(COMINC)/ldfcn.h $(COMDUMP)/filedefs.h \
	$(MACHINC)/sgs.h $(COMDUMP)/flagdefs.h $(MACHINC)/paths.h \
	$(COMINC)/syms.h $(COMINC)/storclass.h $(COMINC)/linenum.h \
	$(COMINC)/scnhdr.h $(COMINC)/reloc.h
	$(LINT_CMD) $(DEFLIST) $(INCLIST) $(COMDUMP)/*.c
#

SOURCES=$(COMDUMP)/main.c $(COMDUMP)/setflags.c $(COMDUMP)/openobject.c \
	$(COMDUMP)/process.c $(COMDUMP)/fcns.c \
	$(COMDUMP)/dumpmap.h \
	$(USRINC)/filehdr.h $(COMINC)/ldfcn.h $(COMDUMP)/filedefs.h \
	$(MACHINC)/sgs.h $(COMDUMP)/flagdefs.h $(MACHINC)/paths.h \
	$(COMINC)/syms.h $(COMINC)/storclass.h $(COMINC)/linenum.h \
	$(COMINC)/scnhdr.h $(COMINC)/reloc.h

ctags:
	ctags $(SOURCES)
	echo 'grep $$* \\' >greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
shrink:
	-rm -f $(OBJECTS) dump
