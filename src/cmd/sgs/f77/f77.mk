#	START NEW ARIX SCCS HEADER
#
#	@(#) f77.mk: version 25.1 created on 12/2/91 at 17:40:02
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)f77.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#ident	"@(#)f77:f77.mk	25.1"

IROOT =
include $(IROOT)/macdefs

# POSIX

SVID_FLAG =

DIRECTS= f77 f77optim
all clean clobber: $(DIRECTS)
	@-for i in $(DIRECTS); \
	do \
		echo "cd $$i; $(MAKE) -$(MAKEFLAGS)f $$i.mk $@"; \
		cd $$i; $(MAKE) -$(MAKEFLAGS)f $$i.mk $@; \
		cd ..; \
	done

install: $(DIRECTS)
	@-for i in $(DIRECTS); \
	do \
		echo "cd $$i; $(MAKE) -$(MAKEFLAGS)f $$i.mk install"; \
		cd $$i; $(MAKE) -$(MAKEFLAGS)f $$i.mk install; \
		cd ..; \
	done
$(DIRECTS):
	@echo CANNOT FIND DIRECTORY $@
