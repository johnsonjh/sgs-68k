#	START NEW ARIX SCCS HEADER
#
#	@(#) sccs.mk: version 25.1 created on 12/2/91 at 17:13:06
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)sccs.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	ATT: ident	"sccs:sccs.mk	6.3"

#ident	"@(#)sccs:sccs.mk	25.1"

IROOT =
include $(IROOT)/macdefs

BIN = $(ROOT)/usr/bin
HELPLIB = $(ROOT)/usr/lib/help
ARGS =

all: libs cmds helplib
	@echo "SCCS is built"

libs:
	cd lib; $(MAKE) -e -f lib.mk

cmds:
	cd cmd; $(MAKE) -e -f cmd.mk $(ARGS)

helplib:
	cd help.d; $(MAKE) -e -f help.mk

install:
	cd lib; $(MAKE) -e -f lib.mk install
	cd cmd; $(MAKE) -e -f cmd.mk $(ARGS) BIN=$(BIN) install
	cd help.d; $(MAKE) -e -f help.mk HELPLIB=$(HELPLIB) install

clean:
	cd lib; $(MAKE) -e -f lib.mk clean
	cd cmd; $(MAKE) -e -f cmd.mk clean
	cd help.d; $(MAKE) -e -f help.mk clean

clobber:
	cd lib; $(MAKE) -e -f lib.mk clobber
	cd cmd; $(MAKE) -e -f cmd.mk clobber BIN=$(BIN)
	cd help.d; $(MAKE) -e -f help.mk clobber HELPLIB=$(HELPLIB)
