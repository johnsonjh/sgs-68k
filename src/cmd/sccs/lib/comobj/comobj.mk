#	START NEW ARIX SCCS HEADER
#
#	@(#) comobj.mk: version 25.1 created on 12/2/91 at 17:09:49
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)comobj.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	ATT: ident	"sccs:lib/comobj/comobj.mk	1.8"
#
#

#ident	"@(#)sccs:comobj.mk	25.1"

include	$(IROOT)/macdefs
LORDER = lorder

LIB = ../comobj.a

PRODUCTS = $(LIB)

SOURCE = auxf.c chkid.c chksid.c date_ab.c date_ba.c del_ab.c del_ba.c \
	dodelt.c dofile.c dohist.c doie.c dolist.c eqsid.c flushto.c \
	fmterr.c getline.c logname.c newstats.c permiss.c pf_ab.c putline.c \
	rdmod.c setup.c sid_ab.c sid_ba.c sidtoser.c sinit.c stats_ab.c

FILES = auxf.o		\
		chkid.o		\
		chksid.o	\
		date_ab.o	\
		date_ba.o	\
		del_ab.o	\
		del_ba.o	\
		dodelt.o	\
		dofile.o	\
		dohist.o	\
		doie.o		\
		dolist.o	\
		eqsid.o		\
		flushto.o	\
		fmterr.o	\
		getline.o	\
		logname.o	\
		newstats.o	\
		permiss.o	\
		pf_ab.o		\
		putline.o	\
		rdmod.o		\
		setup.o		\
		sid_ab.o	\
		sid_ba.o	\
		sidtoser.o	\
		sinit.o		\
		stats_ab.o

all: $(PRODUCTS)
	@echo "Library $(PRODUCTS) is up to date\n"

$(LIB): $(FILES)
	$(AR) cr $(LIB) `$(LORDER) *.o | tsort`
	chmod 664 $(LIB)

auxf.o: ../../hdr/defines.h
chkid.o: ../../hdr/defines.h
chksid.o: ../../hdr/defines.h
date_ab.o:
date_ba.o: ../../hdr/defines.h
del_ab.o: ../../hdr/defines.h
del_ba.o: ../../hdr/defines.h
dodelt.o: ../../hdr/defines.h
dofile.o: ../../hdr/defines.h
dohist.o: ../../hdr/defines.h ../../hdr/had.h
doie.o: ../../hdr/defines.h
dolist.o: ../../hdr/defines.h
eqsid.o: ../../hdr/defines.h
flushto.o: ../../hdr/defines.h
fmterr.o: ../../hdr/defines.h
getline.o: ../../hdr/defines.h
logname.o:
newstats.o: ../../hdr/defines.h
permiss.o: ../../hdr/defines.h
pf_ab.o: ../../hdr/defines.h
putline.o: ../../hdr/defines.h
rdmod.o: ../../hdr/defines.h
setup.o: ../../hdr/defines.h
sid_ab.o: ../../hdr/defines.h
sid_ba.o: ../../hdr/defines.h
sidtoser.o: ../../hdr/defines.h
sinit.o: ../../hdr/defines.h
stats_ab.o: ../../hdr/defines.h

install:	$(LIB)

clean:
	-rm -f *.o

clobber: clean
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

