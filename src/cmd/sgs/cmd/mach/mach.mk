#ident	"@(#)mach.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

include	$(IROOT)/macdefs
include	$(IROOT)/sgsdefs

# POSIX
SVID_FLAG=


#
#	BELLMAC-32 CC MAKEFILE
#
#
BINDIR= $(ROOT)/bin
LDLIBS =
#
CC_CMD= cd $(OUT); $(CC) $(CFLAGS)
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
VERSION=
USRINC= $(INCRT)
#
#	Internal Directories
#
BASE= $(PWD)/../..
LIBDIR= $(ROOT)lib
MACHINC= $(BASE)/inc/mach
COMCMD= $(BASE)/cmd/common
#
# use M as a short-hand for the awkward COMCMD
# (M for Machine independent part)
# (D for Machine dependent part)
M=      $(COMCMD)
D=	$(PWD)
O=$(PWD)/$(OUT)
#
#

########################
PFILES	=	$M/cc.p
#
protos:		$(PFILES)
			cat $(PFILES) > $M/protos.h
			rm -f $(PFILES)
########################
all:		$O/cc
#
$O/cc:	$(MACHINC)/paths.h $(MACHINC)/sgs.h $M/cc.c
	$(CC_CMD) -o $O/cc $M/cc.c $(LDLIBS) $(LDFLAGS)
#
save:		$(BINDIR)/$(SGS)cc
	-rm -f $(BINDIR)/$(SGS)cc.back
	cp $(BINDIR)/$(SGS)cc $(BINDIR)/$(SGS)cc.back
#
install:	$O/cc
	-rm -f $(BINDIR)/$(SGS)cc
	$(INS) $O/cc $(BINDIR)/$(SGS)cc
	chmod 755 $(BINDIR)/$(SGS)cc
	chgrp bin $(BINDIR)/$(SGS)cc
	chown bin $(BINDIR)/$(SGS)cc

#
uninstall:	$(BINDIR)/$(SGS)cc.back
	-rm -f $(BINDIR)/$(SGS)cc
	cp $(BINDIR)/$(SGS)cc.back $(BINDIR)/$(SGS)cc
#
clean:
	-rm -f $(OUT)/*.o

clobber: clean
	-rm -f $O/cc tags greplist
#
lint:		$(MACHINC)/paths.h $(MACHINC)/sgs.h $M/cc.c
	$(LINT_CMD) $(INCLIST) $M/cc.c

SOURCES=$M/cc.c

ctags:
	ctags $(SOURCES)
	echo 'grep $$* \\' >greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
