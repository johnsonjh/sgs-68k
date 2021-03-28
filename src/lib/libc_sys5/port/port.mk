#ident	"@(#)port.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT: ident	"libc-port:makefile	1.17"
#
# makefile for libc/port
#
#
# The macro PROF is null by default, causing profiled object to be maintained.
# If profiled object is not desired, the reassignment PROF=@# should appear in
# the make command line.
#

.SUFFIXES: .p
IROOT =
include $(IROOT)/macdefs
include $(IROOT)/sgsdefs
PROF=
NONPROF=
DEFLIST=
SDEFLIST=

all:	prof nonprof 

nonprof:
	$(NONPROF)cd gen; \
		$(MAKE) -$(MAKEFLAGS)ef gen.mk $(IGN) nonprof PROF=@#
	$(NONPROF)cd print; \
		$(MAKE) -$(MAKEFLAGS)ef print.mk $(IGN) nonprof PROF=@#
	$(NONPROF)cd stdio; \
		$(MAKE) -$(MAKEFLAGS)ef stdio.mk $(IGN) nonprof PROF=@#
	$(NONPROF)cd sys; \
		$(MAKE) -$(MAKEFLAGS)ef sys.mk $(IGN) nonprof PROF=@#
prof:
	$(PROF)cd gen; \
		$(MAKE) -$(MAKEFLAGS)ef gen.mk $(IGN) prof NONPROF=@#
	$(PROF)cd print; \
		$(MAKE) -$(MAKEFLAGS)ef print.mk $(IGN) prof NONPROF=@#
	$(PROF)cd stdio; \
		$(MAKE) -$(MAKEFLAGS)ef stdio.mk $(IGN) prof NONPROF=@#
	$(PROF)cd sys; \
		$(MAKE) -$(MAKEFLAGS)ef sys.mk $(IGN) prof NONPROF=@#
	
clean:

clobber: clean
	-rm -f */*.o
	-rm -f */*.p
	-rm -f */*.n
	-rm -f greplist tags

ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
