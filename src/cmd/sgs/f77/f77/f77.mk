#	START NEW ARIX SCCS HEADER
#
#	@(#) f77.mk: version 25.1 created on 12/2/91 at 17:35:38
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)f77.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#ident	"@(#)f77/f77:f77.mk	25.1"

IROOT =
include $(IROOT)/macdefs
include $(IROOT)/sgsdefs

# POSIX

SVID_FLAG =

all install: mach
		cd mach; $(MAKE) -$(MAKEFLAGS)f f77.mk $@

clean clobber: mach
		cd mach; $(MAKE) -$(MAKEFLAGS)f f77.mk $@

mach:
	@echo CANNOT FIND DIRECTORY $@
