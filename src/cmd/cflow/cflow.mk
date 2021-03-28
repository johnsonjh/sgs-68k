#	START NEW ARIX SCCS HEADER
#
#	@(#) cflow.mk: version 25.1 created on 12/2/91 at 13:55:38
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)cflow.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT: ident	"cflow:cflow.mk	1.12"

#ident	"@(#)clfow:cflow.mk	25.1"

include	$(IROOT)/macdefs

FLG=
TESTDIR = .
FILES = Makefile README cflow.sh dag.c lpfx.c nmf.c flip.c
ALL = dag lpfx nmf flip
LINT = .
MIP = .
BIN = $(ROOT)/usr/bin
LIB = $(ROOT)/usr/lib
SOURCES= \
	dag.c      flip.c     lerror.h   lpass2.h   lpfx.c     nmf.c

all:	chk_pdp $(ALL)

chk_pdp:
	if pdp11 && [ "$(FLG)" != "x" ]; then \
		$(MAKE) -$(MAKEFLAGS) FLG=x ALL="$(ALL)" LINT="$(LINT)" \
			MIP="$(MIP)" CFLAGS="$(CFLAGS) -UFLEXNAME" \
			-f cflow.mk all; \
	fi

dag:	dag.c
	$(CC) $(CFLAGS)  dag.c -o $(TESTDIR)/dag $(LDFLAGS)

lpfx:	lpfx.c $(LINT)/lerror.h $(LINT)/lmanifest $(LINT)/lpass2.h \
		$(MIP)/manifest
	$(CC) -DFLEXNAMES $(CFLAGS) -I$(LINT) -I$(MIP)  lpfx.c -o $(TESTDIR)/lpfx -lmalloc $(LDFLAGS)

nmf:	nmf.c
	$(CC) $(CFLAGS) nmf.c  -o $(TESTDIR)/nmf $(LDFLAGS)

flip:	flip.c
	$(CC) $(CFLAGS) flip.c  -o $(TESTDIR)/flip $(LDFLAGS)

install:	chk_pdp $(ALL)
	$(INS) cflow.sh $(BIN)/cflow
	$(INS) dag $(LIB)/dag
	$(INS) lpfx $(LIB)/lpfx
	$(INS) nmf $(LIB)/nmf
	$(INS) flip $(LIB)/flip
	$(CH) chgrp bin $(BIN)/cflow $(LIB)/dag $(LIB)/lpfx $(LIB)/nmf $(LIB)/flip
	$(CH) chown bin $(BIN)/cflow $(LIB)/dag $(LIB)/lpfx $(LIB)/nmf $(LIB)/flip
	$(CH) chmod 755 $(BIN)/cflow $(LIB)/dag $(LIB)/lpfx $(LIB)/nmf $(LIB)/flip

clean:
	-rm -f *.o a.out make.out core

clobber:	clean
	-rm -f $(ALL)
	-rm -f greplist tags
ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
