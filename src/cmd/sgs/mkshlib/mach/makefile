#ident	"@(#)makefile	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT:ident	"mkshlib:mach/makefile	1.3"


include	$(IROOT)/macdefs
include	$(IROOT)/sgsdefs

#
#	3B2/3B5 mkshlib MAKEFILE
#
#
# POSIX

SVID_FLAG =

OWN=bin
GRP=bin
DEFLIST=
INCLIST=-I$(MACHINC) -I$(COMINC) -I$(COMDIR) -I$(USRINC)
ENV=
CC_CMD=$(CC) $(CFLAGS) -c $(DEFLIST) $(INCLIST) $(ENV)
#
LD_CMD=$(CC) $(LDFLAGS)
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
#
USRINC=$(INCRT)
#
#	Internal Directories
#
BASE= ../..
BINDIR= $(ROOT)/bin
#LIBDIR= $(BASE)/xenv/m32/lib
MACHINC= $(BASE)/inc/mach
COMDIR= $(BASE)/mkshlib/common
MACHDIR= $(BASE)/mkshlib/mach
COMINC= $(INCRT)
#
OBJECTS = extn.o fcns.o main.o t_mkbt.o t_btassem.o \
	  t_mktarget.o rdspec.o \
	  h_assem.o h_create.o h_fcns.o h_mkhost.o h_pobjs.o t_mklib.o

#
all:	mkshlib
#

mkshlib:	$(OBJECTS) 
	$(CC) -o mkshlib $(OBJECTS) -lld $(LDLIBS) $(LDFLAGS)

#
main.o:	$(COMDIR)/main.c $(COMINC)/filehdr.h $(COMINC)/ldfcn.h $(MACHINC)/paths.h \
	$(COMINC)/syms.h $(COMINC)/scnhdr.h $(COMDIR)/shlib.h $(COMDIR)/hst.h \
	$(COMDIR)/trg.h
	$(CC_CMD) $(COMDIR)/main.c
#
extn.o:	$(COMDIR)/extn.c $(COMINC)/filehdr.h $(COMINC)/ldfcn.h \
	$(COMINC)/storclass.h $(COMINC)/syms.h $(COMINC)/scnhdr.h \
	$(COMDIR)/shlib.h $(COMDIR)/hst.h $(COMDIR)/trg.h
	$(CC_CMD) $(COMDIR)/extn.c
#
fcns.o:	$(COMDIR)/fcns.c $(COMINC)/filehdr.h $(COMINC)/ldfcn.h \
	$(COMDIR)/shlib.h $(MACHINC)/paths.h
	$(CC_CMD) $(COMDIR)/fcns.c
#
h_assem.o:	$(MACHDIR)/h_assem.c $(COMINC)/filehdr.h $(COMINC)/ldfcn.h \
	$(COMINC)/syms.h $(COMINC)/scnhdr.h \
        $(COMDIR)/shlib.h $(COMDIR)/hst.h
	$(CC_CMD) $(MACHDIR)/h_assem.c
#
h_create.o:	$(COMDIR)/h_create.c $(COMINC)/filehdr.h $(COMINC)/ldfcn.h \
	$(COMINC)/storclass.h $(COMINC)/syms.h $(COMINC)/scnhdr.h \
        $(COMINC)/reloc.h $(COMDIR)/shlib.h $(COMDIR)/hst.h
	$(CC_CMD) $(COMDIR)/h_create.c
#
h_fcns.o:	$(COMDIR)/h_fcns.c $(COMINC)/filehdr.h $(COMINC)/ldfcn.h \
	$(COMINC)/syms.h $(COMINC)/scnhdr.h \
        $(COMDIR)/shlib.h $(COMDIR)/hst.h
	$(CC_CMD) $(COMDIR)/h_fcns.c
#
h_mkhost.o:	$(COMDIR)/h_mkhost.c $(COMINC)/filehdr.h $(COMINC)/ldfcn.h \
	$(COMINC)/syms.h $(COMINC)/scnhdr.h \
	$(COMDIR)/shlib.h $(COMDIR)/hst.h
	$(CC_CMD) $(COMDIR)/h_mkhost.c
#
h_pobjs.o:	$(COMDIR)/h_pobjs.c $(COMINC)/filehdr.h $(COMINC)/ldfcn.h \
	$(COMINC)/syms.h $(COMINC)/scnhdr.h $(MACHINC)/sgs.h $(COMINC)/reloc.h \
	$(COMINC)/aouthdr.h $(COMDIR)/shlib.h $(COMDIR)/hst.h
	$(CC_CMD) $(COMDIR)/h_pobjs.c
#
rdspec.o:	$(COMDIR)/rdspec.c $(COMINC)/filehdr.h $(COMINC)/ldfcn.h \
	$(MACHINC)/paths.h $(COMINC)/syms.h $(COMINC)/scnhdr.h \
	$(COMDIR)/shlib.h $(COMDIR)/hst.h $(COMDIR)/trg.h
	$(CC_CMD) $(COMDIR)/rdspec.c
#
t_btassem.o:	$(MACHDIR)/t_btassem.c $(COMINC)/filehdr.h $(COMINC)/ldfcn.h \
	$(COMDIR)/shlib.h $(COMDIR)/trg.h
	$(CC_CMD) $(MACHDIR)/t_btassem.c
#
t_mkbt.o:	$(COMDIR)/t_mkbt.c $(COMINC)/filehdr.h $(COMINC)/ldfcn.h \
	$(COMINC)/storclass.h $(COMINC)/syms.h $(COMINC)/scnhdr.h \
        $(COMDIR)/shlib.h $(COMDIR)/trg.h
	$(CC_CMD) $(COMDIR)/t_mkbt.c
#
t_mktarget.o:	$(COMDIR)/t_mktarget.c $(COMINC)/filehdr.h $(COMINC)/ldfcn.h \
	$(MACHINC)/paths.h $(COMDIR)/shlib.h $(COMDIR)/trg.h
	$(CC_CMD) $(COMDIR)/t_mktarget.c
t_mklib.o:	$(COMDIR)/t_mklib.c 
	$(CC_CMD) $(COMDIR)/t_mklib.c
#
save:	$(BINDIR)/$(SGS)mkshlib
	-rm -f $(BINDIR)/$(SGS)mkshlib.back
	cp $(BINDIR)/$(SGS)mkshlib $(BINDIR)/$(SGS)mkshlib.back
#
install:	mkshlib
	$(INS)	mkshlib	$(BINDIR)/mkshlib
	chmod 755 $(BINDIR)/mkshlib
	chgrp bin $(BINDIR)/mkshlib
	chown bin $(BINDIR)/mkshlib
#
uninstall:	$(BINDIR)/$(SGS)mkshlib.back
	-rm -f $(BINDIR)/$(SGS)mkshlib
	cp $(BINDIR)/$(SGS)mkshlib.back $(BINDIR)/$(SGS)mkshlib
#
clean:
	-rm -f $(OBJECTS) 

clobber: clean
	-rm -f mkshlib tags greplist
#
lint:	$(COMDIR)/extn.c $(COMDIR)/fcns.c $(COMDIR)/hst.h \
	$(COMDIR)/h_create.c $(COMDIR)/h_fcns.c $(COMDIR)/h_mkhost.c \
	$(COMDIR)/h_pobjs.c $(COMDIR)/main.c $(COMDIR)/rdspec.c \
	$(COMDIR)/shlib.h $(COMDIR)/trg.h $(COMDIR)/t_mkbt.c \
	$(COMDIR)/mktarget.c $(MACHDIR)/h_assem.c $(MACHDIR)/t_btassem.c \
	$(COMINC)/aouthdr.h $(COMINC)/filehdr.h $(COMINC)/ldfcn.h $(COMINC)/syms.h \
	$(COMINC)/storclass.h $(MACHINC)/paths.h $(MACHINC)/sgs.h \
	$(COMINC)/scnhdr.h
	$(LINT_CMD) $(DEFLIST) $(INCLIST) $(COMDIR)/*.c

SOURCES=\
	../common/extn.c ../common/fcns.c ../common/h_create.c		\
	../common/h_fcns.c ../common/h_mkhost.c ../common/h_pobjs.c	\
	../common/hst.h ../common/main.c ../common/rdspec.c		\
	../common/shlib.h ../common/t_mkbt.c ../common/t_mktarget.c	\
	../common/trg.h h_assem.c t_btassem.c
ctags:
	ctags $(SOURCES)
	echo 'grep $$* \\' >greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
