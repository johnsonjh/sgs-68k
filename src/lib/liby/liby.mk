#ident	"@(#)liby.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT: ident	"liby:liby.mk	1.6"

IROOT =
include $(IROOT)/macdefs

LIB=$(ROOT)/$(SGS)lib
USRLIB=$(ROOT)/usr/$(SGS)lib
LORDER=lorder


LIBOBJS=libmai.o libzer.o

all:     $(USRLIB)/liby.a

$(USRLIB)/liby.a: $(LIBOBJS)
	ar rv tmplib.a `$(LORDER) *.o | tsort`;

libmai.o:	libmai.c
		$(CC) -c $(CFLAGS) libmai.c

libzer.o:	libzer.c
		$(CC) -c $(CFLAGS) libzer.c

install:  all
	$(INS) tmplib.a $(USRLIB)/liby.a;
	$(AR) ts $(USRLIB)/liby.a > /dev/null ;
	chmod 644 $(USRLIB)/liby.a;
	chgrp bin $(USRLIB)/liby.a;
	chown bin $(USRLIB)/liby.a;

clean:
	-rm -f *.o

clobber:	clean
	-rm -f tmplib.a  tags greplist

SOURCES = libmai.c    libzer.c    
ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
