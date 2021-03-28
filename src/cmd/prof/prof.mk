#	START NEW ARIX SCCS HEADER
#
#	@(#) prof.mk: version 25.1 created on 12/2/91 at 16:48:03
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)prof.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT:#ident	"prof:prof.mk	1.8"

#ident	"@(#)prof:prof.mk	25.1"

IROOT	=
SROOT	=
include $(IROOT)/macdefs

VERSION	=
UBINDIR	= $(ROOT)/usr/bin
USRINC	= $(ROOT)/usr/$(VERSION)/include
SOURCE = prof.c
# LDFLAGS = -s

all:	prof

prof:	prof.c $(INCRT)/stdio.h $(INCRT)/a.out.h $(INCRT)/sys/types.h \
	$(INCRT)/sys/param.h $(INCRT)/mon.h
	$(CC) $(CFLAGS)  prof.c -lld -o prof $(LDFLAGS)

install:	prof
	$(INS) prof $(UBINDIR)/prof
	chmod 775 $(UBINDIR)/prof
	chgrp bin $(UBINDIR)/prof
	chown bin $(UBINDIR)/prof

ctags:
	ctags	$(SOURCE)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCE)		;\
	do echo "$$i \\" >> greplist	;\
	done
	echo /dev/null >> greplist
	chmod +x greplist

clean:
	rm -f prof.o

clobber: clean
	rm -f prof tags greplist
