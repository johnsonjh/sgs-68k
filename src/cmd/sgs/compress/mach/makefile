#ident	"@(#)makefile	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
include	$(IROOT)/macdefs
include	$(IROOT)/sgsdefs

#	COMPRESS MAKEFILE
#
#
# POSIX

SVID_FLAG =

OWN = bin
GRP = bin
BASE = ../..
#
INCCOM = $(INCRT)
COMDIR = $(BASE)/compress/common
INCLST = -I$(COMDIR) -I$(INCCOM)
BINDIR = $(ROOT)/bin
#
STRIP=strip
SPFLAGS=
STRIP_CMD=$(STRIP) $(SPFLAGS)
#
LINT = lint
LINTFLAGS=-p
LINT_CMD=$(LINT) $(LINTFLAGS)
#
OBJECTS = pass1.o pass2.o
LOADS = $(OBJECTS) 
LDLIBS=
#
#
all:	cprs
#


cprs:  $(LOADS) 
	$(CC) $(LOADS) -o  cprs  $(LDFLAGS) $(LDLIBS) -lld
#
pass1.o:  $(INCCOM)/filehdr.h $(INCCOM)/storclass.h $(INCCOM)/syms.h \
	$(INCCOM)/ldfcn.h $(COMDIR)/tagitem.h $(COMDIR)/pass1.c
	$(CC) $(CFLAGS) -c $(INCLST) $(COMDIR)/pass1.c
#
pass2.o:  $(INCCOM)/filehdr.h $(INCCOM)/syms.h \
	$(INCCOM)/scnhdr.h $(INCCOM)/linenum.h $(INCCOM)/reloc.h \
	$(INCCOM)/ldfcn.h $(COMDIR)/tagitem.h  $(COMDIR)/pass2.c
	$(CC) $(CFLAGS) -c $(INCLST) $(COMDIR)/pass2.c
#
save:	$(BINDIR)/$(SGS)cprs
	-rm -f $(BINDIR)/$(SGS)cprs.back
	cp $(BINDIR)/$(SGS)cprs $(BINDIR)/$(SGS)cprs.back
#
install: cprs
	$(INS) cprs $(BINDIR)/cprs
	chmod 0755 $(BINDIR)/cprs
	chgrp bin $(BINDIR)/cprs
	chown bin $(BINDIR)/cprs

#
uninstall:	$(BINDIR)/$(SGS)cprs.back
	-rm -f $(BINDIR)/$(SGS)cprs
	cp $(BINDIR)/$(SGS)cprs.back $(BINDIR)/$(SGS)cprs
#
clean:
	-rm -f $(OBJECTS) 

clobber: clean
	-rm -f cprs tags greplist
#
lint:	$(INCCOM)/filehdr.h $(INCCOM)/ldfcn.h $(INCCOM)/linenum.h\
	$(INCCOM)/reloc.h $(INCCOM)/scnhdr.h $(INCCOM)/storclass.h\
	$(INCCOM)/syms.h $(COMDIR)/tagitem.h $(COMDIR)/pass1.c $(COMDIR)/pass2.c
	$(LINT_CMD) -I$(INCCOM) -I$(COMDIR) $(COMDIR)/*.c


SOURCES=$(COMDIR)/pass1.c $(COMDIR)/pass2.c $(COMDIR)/tagitem.h
ctags:
	ctags $(SOURCES)
	echo 'grep $$* \\' >greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
