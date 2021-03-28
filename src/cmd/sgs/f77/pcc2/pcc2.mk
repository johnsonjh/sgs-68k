#	START NEW ARIX SCCS HEADER
#
#	@(#) pcc2.mk: version 25.1 created on 12/2/91 at 17:44:28
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)pcc2.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#

#ident	"@(#)sgs/comp2:pcc2.mk	1.1"


SROOT =
IROOT =
include $(IROOT)/macdefs
include $(IROOT)/sgsdefs

# POSIX

SVID_FLAG =

DIRS=mach

all install clean clobber: $(DIRS)
	@-for i in $(DIRS); \
	do \
		echo "cd $$i; $(MAKE) -$(MAKEFLAGS)f $$i.mk $@"; \
		cd $$i; $(MAKE) -$(MAKEFLAGS)f $$i.mk $@; \
		cd ..; \
	done

$(DIRS):
	@echo CANNOT FIND DIRECTORY $@
