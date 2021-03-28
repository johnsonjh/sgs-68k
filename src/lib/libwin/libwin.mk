#	START NEW ARIX SCCS HEADER
#
#	@(#) libwin.mk: version 25.1 created on 12/2/91 at 20:15:43
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)libwin.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT: ident	"attwin:lib/libagent/makefile	1.8"
#ident	"@(#)attwin:libwin.mk	25.1"
#
#		Copyright 1985 AT&T
#

IROOT =
include $(IROOT)/macdefs
LIB = $(ROOT)/usr/$(SGS)lib
INC = $(INCRT)

all:	libwindows.a

libwindows.a:	libwindows.o
	$(AR) r libwindows.a libwindows.o

libwindows.o:	libwindows.c
libwindows.o:	$(INCRT)/stdio.h
libwindows.o:	$(INCRT)/fcntl.h
libwindows.o:	$(INCRT)/windows.h
libwindows.o:	$(INCRT)/sys/types.h
libwindows.o:	$(INCRT)/sys/tty.h
libwindows.o:	$(INCRT)/sys/jioctl.h
libwindows.o:	$(INCRT)/sys/xtproto.h
	$(CC) $(CFLAGS) -c libwindows.c

install:	all
	$(INS) libwindows.a $(LIB)/libwindows.a
	$(AR) ts $(LIB)/libwindows.a > /dev/null
	chmod 644 $(LIB)/libwindows.a
	chgrp bin $(LIB)/libwindows.a
	chown bin $(LIB)/libwindows.a

clean:
	rm -f libwindows.o

clobber: clean
	rm -f libwindows.a tags greplist

SOURCES = libwindows.c    
ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
