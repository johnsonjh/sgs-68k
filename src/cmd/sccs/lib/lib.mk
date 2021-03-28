#	START NEW ARIX SCCS HEADER
#
#	@(#) lib.mk: version 25.1 created on 12/2/91 at 17:11:17
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)lib.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT: ident	"sccs:lib/lib.mk	6.3"
#
#

#ident	"@(#)sccs/lib:lib.mk	25.1"

include	$(IROOT)/macdefs


LIBS=	cassi.a		\
		comobj.a	\
		mpwlib.a

PRODUCTS = $(LIBS)

INSTR=

all: $(LIBS)
	@echo "SCCS Libraries are up to date."

cassi.a::
	cd cassi; $(MAKE) -e -f cassi.mk all

comobj.a::
	cd comobj; $(MAKE) -e -f comobj.mk all

mpwlib.a::
	cd mpwlib; $(MAKE) -e -f mpwlib.mk all


install:	$(LIBS)
	cd cassi; $(MAKE) -e -f cassi.mk install
	cd comobj; $(MAKE) -e -f comobj.mk install
	cd mpwlib; $(MAKE) -e -f mpwlib.mk install

clean:
	cd cassi; $(MAKE) -e -f cassi.mk clean
	cd comobj; $(MAKE) -e -f comobj.mk clean
	cd mpwlib; $(MAKE) -e -f mpwlib.mk clean

clobber:
	cd cassi; $(MAKE) -e -f cassi.mk clobber
	cd comobj; $(MAKE) -e -f comobj.mk clobber
	cd mpwlib; $(MAKE) -e -f mpwlib.mk clobber

.PRECIOUS:	$(PRODUCTS)
