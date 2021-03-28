#ident	"@(#)makefile	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT:ident	"list:m32/makefile	1.14"


include	$(IROOT)/macdefs
include	$(IROOT)/sgsdefs

#
#	BELLMAC-ACH LIST MAKEFILE
#
#
# POSIX

SVID_FLAG =

OWN=bin
GRP=bin
ARCH=ARACHW
#	Set FLEX to -DFLEXNAMES for flexnames
FLEX= -DFLEXNAMES
DEFLIST=-D$(ARCH) $(FLEX)
INCLIST=-I$(MACHINC) -I$(COMINC) -I$(COMLIST) -I$(USRINC)
ENV=
CC_CMD=$(CC) $(CFLAGS) -c $(DEFLIST) $(INCLIST) $(ENV)
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
#	Internal Directories
#
BASE= ../..
BINDIR= $(ROOT)/bin
LIBDIR= $(ROOT)/lib
MACHINC= $(BASE)/inc/mach
COMLIST= $(BASE)/list/common
COMINC= $(INCRT)
ICOMINC= $(BASE)/inc/common
#
#	External Directories
#
USRINC=$(INCRT)
#
OBJECTS= main.o initfcns.o process.o symfcns.o findname.o parsefcns.o \
	srciofcns.o fcns.o
#
all:	list
#

list:		$(OBJECTS)  
	$(CC) -o list $(OBJECTS) -lld $(LDLIBS) $(LDFLAGS)
#
main.o:	$(COMLIST)/main.c $(COMINC)/filehdr.h $(COMINC)/ldfcn.h $(MACHINC)/sgs.h \
	$(COMLIST)/defs.h
	$(CC_CMD) $(COMLIST)/main.c
#
initfcns.o:	$(COMLIST)/initfcns.c $(COMINC)/filehdr.h $(COMINC)/ldfcn.h \
		$(MACHINC)/paths.h $(MACHINC)/sgs.h $(COMLIST)/defs.h
	$(CC_CMD) $(COMLIST)/initfcns.c
#
process.o:	$(COMLIST)/process.c $(COMLIST)/defs.h
	$(CC_CMD) $(COMLIST)/process.c
#
symfcns.o:	$(COMLIST)/symfcns.c $(COMINC)/filehdr.h $(COMINC)/syms.h $(COMINC)/ldfcn.h \
		$(COMINC)/storclass.h $(COMLIST)/defs.h
	$(CC_CMD) $(COMLIST)/symfcns.c
#
findname.o:	$(COMLIST)/findname.c $(COMINC)/syms.h $(COMINC)/storclass.h \
		$(COMLIST)/defs.h $(COMLIST)/findname.h
	$(CC_CMD) $(COMLIST)/findname.c
#
parsefcns.o:	$(COMLIST)/parsefcns.c $(COMLIST)/defs.h
	$(CC_CMD) $(COMLIST)/parsefcns.c
#
srciofcns.o:	$(COMLIST)/srciofcns.c $(COMINC)/filehdr.h $(COMINC)/linenum.h \
		$(COMINC)/ldfcn.h $(COMLIST)/defs.h $(ICOMINC)/sgsmacros.h
	$(CC_CMD) $(COMLIST)/srciofcns.c
#
fcns.o:	$(COMLIST)/fcns.c $(COMINC)/filehdr.h $(COMINC)/ldfcn.h $(MACHINC)/sgs.h \
	$(COMLIST)/defs.h
	$(CC_CMD) $(COMLIST)/fcns.c
#
#
clean:
	-rm -f $(OBJECTS) 

clobber: clean
	-rm -f list ../common/greplist ../common/tags
#
save:	$(BINDIR)/$(SGS)list
	-rm -f $(BINDIR)/$(SGS)list.back
	cp $(BINDIR)/$(SGS)list $(BINDIR)/$(SGS)list.back
#
install: list	
#
	$(INS) list	$(BINDIR)/list
	chmod 755	$(BINDIR)/list
	chgrp bin	$(BINDIR)/list
	chown bin	$(BINDIR)/list
#
uninstall:	$(BINDIR)/$(SGS)list.back
	-rm -f $(BINDIR)/$(SGS)list
	cp $(BINDIR)/$(SGS)list.back $(BINDIR)/$(SGS)list
#
lint:	$(COMLIST)/main.c $(COMLIST)/initfcns.c $(COMLIST)/process.c \
	$(COMLIST)/symfcns.c $(COMLIST)/findname.c $(COMLIST)/parsefcns.c \
	$(COMLIST)/srciofcns.c $(COMLIST)/fcns.c \
	$(COMINC)/filehdr.h $(COMINC)/ldfcn.h $(MACHINC)/sgs.h \
	$(COMINC)/linenum.h $(COMINC)/syms.h $(COMINC)/storclass.h \
	$(COMLIST)/defs.h $(COMLIST)/findname.h \
	$(MACHINC)/paths.h
	$(LINT_CMD) $(DEFLIST) $(INCLIST) $(COMLIST)/*.c

SOURCES=\
	../common/defs.h       ../common/initfcns.c   ../common/process.c \
	../common/fcns.c       ../common/lnnofcns.c   ../common/srciofcns.c \
	../common/findname.c   ../common/main.c       ../common/symfcns.c \
	../common/findname.h   ../common/parsefcns.c
ctags:
	ctags $(SOURCES);mv tags ../common
	echo 'grep $$* \\' >../common/greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> ../common/greplist
	echo /dev/null >> ../common/greplist
	chmod +x ../common/greplist
