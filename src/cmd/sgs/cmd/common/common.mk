#ident	"@(#)common.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

include	$(IROOT)/macdefs

# POSIX
SVID_FLAG=

SGS=

#
#	BELLMAC-32 CC MAKEFILE
#
#
OWN=bin
GRP=bin
BINDIR= $(ROOT)/bin


all:		gencc
#
install:	gencc
	-rm -f $(BINDIR)/$(SGS)gencc
	$(INS) gencc $(BINDIR)/$(SGS)gencc
	chmod 755 $(BINDIR)/$(SGS)gencc
	chgrp bin $(BINDIR)/$(SGS)gencc
	chown bin $(BINDIR)/$(SGS)gencc

#
clean:

clobber: clean
#
