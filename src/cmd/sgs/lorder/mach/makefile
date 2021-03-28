#ident	"@(#)makefile	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."


include	$(IROOT)/macdefs
include	$(IROOT)/sgsdefs

#***************************************************************************
#                                                                          *
#			UNIX Lorder (lorder) makefile			   *
#									   *
#***************************************************************************
#                                                                          *
#		Source Code Files					   *
#								           *
#	com/lorder		lorder function shell script		   *
#									   *
#***************************************************************************

# POSIX

SVID_FLAG =

LPT = dpr -mx
BASE = ../..
CDIR = $(BASE)/lorder/common

BINDIR = $(ROOT)/bin
SGS =
ARCH = AR32WR

SFILES = $(CDIR)/lorder.sh

all:	lorder

lorder:	$(SFILES)
	sed 's/SGS//' < $(CDIR)/lorder.sh > lorder

install:	all
	$(INS) lorder $(BINDIR)/lorder
	chmod 755 $(BINDIR)/lorder
	chgrp bin $(BINDIR)/lorder
	chown bin $(BINDIR)/lorder

clean:

clobber: clean
	-rm -f lorder
