#	START NEW ARIX SCCS HEADER
#
#	@(#) libg.mk: version 25.1 created on 12/2/91 at 19:31:13
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

#	ATT: ident	"libg:libg.mk	1.13"
#ident	"@(#)libg:libg.mk	25.1"
# machine independent libg.mk

IROOT =
include $(IROOT)/macdefs
LIB=$(ROOT)/lib
USRLIB=$(ROOT)/usr/lib

all:
		cd mach; $(MAKE) -$(MAKEFLAGS)f libg.mk all ROOT=$(ROOT) $@

install:	all
		cd mach; $(MAKE) -$(MAKEFLAGS)f libg.mk LIB=$(LIB) USRLIB=$(USRLIB) install ROOT=$(ROOT) $@

clean:
		cd mach; $(MAKE) -$(MAKEFLAGS)f libg.mk clean ROOT=$(ROOT) $@

clobber:
		cd mach; $(MAKE) -$(MAKEFLAGS)f libg.mk clobber ROOT=$(ROOT) $@
