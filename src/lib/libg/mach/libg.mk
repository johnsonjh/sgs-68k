#	START NEW ARIX SCCS HEADER
#
#	@(#) libg.mk: version 25.1 created on 12/2/91 at 19:31:20
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)libg.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT: ident	"libg:m32/libg.mk	1.12"
#ident	"@(#)libg:libg.mk	25.1"
IROOT =
include $(IROOT)/macdefs
LIB=$(ROOT)/$(SGS)lib
USRLIB=$(ROOT)/usr/$(SGS)lib
all:	libg.a

libg.a: libg.s
	$(AS) -m libg.s
	mv libg.o libg.a

install: all
	$(INS) libg.a $(USRLIB)/libg.a
	chmod 644 $(USRLIB)/libg.a
	chgrp bin $(USRLIB)/libg.a
	chown bin $(USRLIB)/libg.a

clean:
	rm -f *.o

clobber: clean
	rm -f libg.a greplist tags

SOURCES=libg.s
ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
