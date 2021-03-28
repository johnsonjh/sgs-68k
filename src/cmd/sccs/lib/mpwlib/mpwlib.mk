#	START NEW ARIX SCCS HEADER
#
#	@(#) mpwlib.mk: version 25.1 created on 12/2/91 at 17:12:09
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)mpwlib.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	ATT: ident	"sccs:lib/mpwlib/mpwlib.mk	6.13"
#
#ident	"@(#)sccs:mpwlib.mk	25.1"

include	$(IROOT)/macdefs

AR = ar
LORDER = lorder

LIB = ../mpwlib.a

PRODUCTS = $(LIB)

SOURCE = abspath.c any.c cat.c clean.c dname.c fatal.c fdfopen.c \
	fmalloc.c imatch.c index.c lockit.c logname.c mach/move.c patoi.c \
	rename.c repl.c satoi.c setsig.c sname.c strend.c trnslat.c userexit.c \
	xcreat.c xlink.c xmsg.c xopen.c xpipe.c xunlink.c zero.c

FILES =	abspath.o	\
	alloca.o	\
	any.o	\
	cat.o	\
	clean.o	\
	dname.o	\
	fatal.o	\
	fdfopen.o	\
	fmalloc.o	\
	imatch.o	\
	index.o	\
	lockit.o	\
	logname.o	\
	move.o	\
	patoi.o	\
	rename.o	\
	repl.o	\
	satoi.o	\
	setsig.o	\
	sname.o	\
	strend.o	\
	trnslat.o	\
	userexit.o	\
	xcreat.o	\
	xlink.o	\
	xopen.o	\
	xpipe.o	\
	xunlink.o	\
	xmsg.o	\
	zero.o

all: $(LIB)
	@echo "Library $(PRODUCTS) is up to date"

$(LIB):	$(FILES)
	$(AR) cr $(LIB) `$(LORDER) *.o | tsort`
	chmod 664 $(LIB)

move.o:	
		$(CC) -c $(CFLAGS) mach/move.c

alloca.o:
		$(CC)  -c mach/alloca.s

install:	$(LIB)

clean:
	-rm -f *.o

clobber:	clean
	-rm -f $(PRODUCTS) greplist tags

.PRECIOUS:	$(PRODUCTS)

ctags:	
	ctags	$(SOURCE)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCE)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist

