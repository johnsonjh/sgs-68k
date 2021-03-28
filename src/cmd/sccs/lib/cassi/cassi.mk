#	START NEW ARIX SCCS HEADER
#
#	@(#) cassi.mk: version 25.1 created on 12/2/91 at 17:09:17
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)cassi.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	ATT: ident	"sccs:lib/cassi/cassi.mk	1.7"
#
#

#ident	"@(#)sccs:cassi.mk	25.1"

include	$(IROOT)/macdefs

LORDER = lorder


LIB = ../cassi.a

PRODUCTS = $(LIB)

SOURCE = cmrcheck.c deltack.c error.c filehand.c gf.c

FILES = gf.o		\
		cmrcheck.o	\
		deltack.o	\
		error.o		\
		filehand.o

all: $(PRODUCTS)
	@echo "Library $(PRODUCTS) is up to date\n"

$(LIB): $(FILES)
	$(AR) cr $(LIB) `$(LORDER) *.o | tsort`
	$(CH) chmod 664 $(LIB)

gf.o:	gf.c	\
	 ../../hdr/filehand.h

cmrcheck.o:	cmrcheck.c	\
	 ../../hdr/filehand.h

deltack.o:	deltack.c	\
	 ../../hdr/filehand.h	\
	 ../../hdr/had.h	\
	 ../../hdr/defines.h

filehand.o:	filehand.c ../../hdr/filehand.h

install:	$(PRODUCTS)

clean:
	-rm -f *.o

clobber:	clean
	-rm -f $(PRODUCTS) greplist tags

ctags:	
	ctags	$(SOURCE)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCE)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist


