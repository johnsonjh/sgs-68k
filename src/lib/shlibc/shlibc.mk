#	START NEW ARIX SCCS HEADER
#
#	@(#) shlibc.mk: version 25.1 created on 12/2/91 at 20:18:52
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)shlibc.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#

#ident	"@(#)lib/shlibc:shlibc.mk	25.1"

include	$(IROOT)/macdefs

all: 
	cd ../libc; $(MAKE) -$(MAKEFLAGS)f shlibc.mk $@
install: all
	cd ../libc; $(MAKE) -$(MAKEFLAGS)f shlibc.mk $@
clean:
	cd ../libc; $(MAKE) -$(MAKEFLAGS)f shlibc.mk $@
clobber: clean
	cd ../libc; $(MAKE) -$(MAKEFLAGS)f shlibc.mk $@

cleanup:
	cd ../libc; $(MAKE) -$(MAKEFLAGS)f libc.mk clobber
	cd ../libc; $(MAKE) -$(MAKEFLAGS)f shlibc.mk clobber



