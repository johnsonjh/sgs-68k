#	START NEW ARIX SCCS HEADER
#
#	@(#) sgs.mk: version 25.1 created on 12/2/91 at 18:00:58
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)sgs.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#


#ident	"@(#)cmd:sgs.mk	25.1"


SROOT =
IROOT =
include $(IROOT)/macdefs

DIRS	=	sgs

all clean clobber:
	@for i in $(DIRS) ;\
	do \
	if [ -d $$i ] ;\
	then \
		echo "	(cd $$i/src; $(MAKE) -$(MAKEFLAGS)f $$i.mk $@ )"; \
		(cd $$i/src; $(MAKE) -$(MAKEFLAGS)f $$i.mk $@ ); \
	fi ;\
	done

nfs nfsa1000 nfs90 :	all

install:
	@for i in $(DIRS) ; \
	do \
	if [ -d $$i ] ;\
	then \
		echo "	(cd $$i/src; $(MAKE) -$(MAKEFLAGS)f $$i.mk install)"; \
		(cd $$i/src; $(MAKE) -$(MAKEFLAGS)f $$i.mk install); \
	fi ;\
	done

stripbins:
	@-echo "Stripping binaries \c"; \
	awk '{print $$1 " " $$2}' spec_file | grep -v '^\.' | \
	while read a b ; \
	do \
		echo ".\c"; \
		$(IROOT)/stripmap $$a $$b $(MAPDIR); \
	done 
	@echo " done"

