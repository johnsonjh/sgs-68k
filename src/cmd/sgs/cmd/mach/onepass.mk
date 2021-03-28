#ident	"@(#)onepass.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT:ident	"sgs-cmd:m32/makefile	1.12"


include	$(IROOT)/macdefs

# POSIX
SVID_FLAG=

#
#	BELLMAC-32 CC MAKEFILE
#
#
BINDIR= $(ROOT)/bin
LDLIBS =
ARCH=AR32WR
ASM	= -DIN_LINE
TWOPASS=
DEFLIST=-D$(ARCH) -DM32 $(ASM)
INCLIST=-I$(MACHINC) -I$(USRINC)
ENV=
FFLAG=
CC_CMD=$(CC) $(CFLAGS) $(TWOPASS) $(DEFLIST) $(INCLIST) $(ENV) 
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
BASE= ../..

LIBDIR= $(ROOT)lib
MACHINC= $(BASE)/inc/mach
COMCMD= $(BASE)/cmd/common
#
all:		$(SGS)cc
#
$(SGS)cc:	$(MACHINC)/paths.h $(MACHINC)/sgs.h $(COMCMD)/cc.c
	$(CC_CMD) -i -o $(SGS)cc $(COMCMD)/cc.c $(LDLIBS) $(LDFLAGS)
#
save:		$(BINDIR)/$(SGS)cc
	-rm -f $(BINDIR)/$(SGS)cc.back
	cp $(BINDIR)/$(SGS)cc $(BINDIR)/$(SGS)cc.back
#
install:	$(SGS)cc
	$(INS) $(SGS)cc $(BINDIR)/$(SGS)cc
	chmod 755 $(BINDIR)/$(SGS)cc
	chgrp bin $(BINDIR)/$(SGS)cc
	chown bin $(BINDIR)/$(SGS)cc

#
uninstall:	$(BINDIR)/$(SGS)cc.back
	-rm -f $(BINDIR)/$(SGS)cc
	cp $(BINDIR)/$(SGS)cc.back $(BINDIR)/$(SGS)cc
#
clean:
	-rm -f *.o

clobber: clean
	-rm -f $(SGS)cc tags greplist
#
lint:		$(MACHINC)/paths.h $(MACHINC)/sgs.h $(COMCMD)/cc.c
	$(LINT_CMD) $(DEFLIST) $(INCLIST) $(COMCMD)/cc.c

SOURCES=$(COMCMD)/cc.c

ctags:
	ctags $(SOURCES)
	echo 'grep $$* \\' >greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
