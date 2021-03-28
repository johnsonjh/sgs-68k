#	START NEW ARIX SCCS HEADER
#
#	@(#) f77optim.mk: version 25.1 created on 12/2/91 at 17:40:22
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)f77optim.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#ident	"@(#)f77/f77optim:f77optim.mk	25.1"

IROOT =
include $(IROOT)/macdefs
include $(IROOT)/sgsdefs

DIRECTS=mach

all install clean clobber: $(DIRECTS)
	@-for i in $(DIRECTS); \
	do \
		echo "cd $$i; $(MAKE) -$(MAKEFLAGS)f $$i.mk $@"; \
		cd $$i; $(MAKE) -$(MAKEFLAGS)f $$i.mk $@; \
		cd ..; \
	done

$(DIRECTS):
	@echo CANNOT FIND DIRECTORY $@
