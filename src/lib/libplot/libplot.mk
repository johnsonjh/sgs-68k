#	START NEW ARIX SCCS HEADER
#
#	@(#) libplot.mk: version 25.1 created on 12/2/91 at 20:08:27
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)libplot.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT: ident	"libplot:libplot.mk	1.7"
#ident	"@(#)libplot:libplot.mk	25.1"

#	Makefile for graphics librarys (libplot)

IROOT =
include $(IROOT)/macdefs

DIR = $(ROOT)/usr/$(SGS)lib
USRLIB = $(DIR)

INC = $(INCRT)



STRIP = strip

SIZE = size

LIST = lp

MAKEFILE = libplot.mk

LIBVAR = ROOT=$(ROOT) LROOT=$(LROOT) DIR=$(DIR) USRLIB=$(USRLIB)

all :
	cd plot; $(MAKE) -$(MAKEFLAGS)f plot.mk $(LIBVAR)
	cd t300; $(MAKE) -$(MAKEFLAGS)f t300.mk $(LIBVAR)
	cd t300s; $(MAKE) -$(MAKEFLAGS)f t300s.mk $(LIBVAR)
	cd t4014; $(MAKE) -$(MAKEFLAGS)f t4014.mk $(LIBVAR)
	cd t450; $(MAKE) -$(MAKEFLAGS)f t450.mk $(LIBVAR)
	cd vt0; $(MAKE) -$(MAKEFLAGS)f vt0.mk $(LIBVAR)

install:
	cd plot; $(MAKE) -$(MAKEFLAGS)f plot.mk $(LIBVAR) install
	cd t300; $(MAKE) -$(MAKEFLAGS)f t300.mk $(LIBVAR) install
	cd t300s; $(MAKE) -$(MAKEFLAGS)f t300s.mk $(LIBVAR) install
	cd t4014; $(MAKE) -$(MAKEFLAGS)f t4014.mk $(LIBVAR) install
	cd t450; $(MAKE) -$(MAKEFLAGS)f t450.mk $(LIBVAR) install
	cd vt0; $(MAKE) -$(MAKEFLAGS)f vt0.mk $(LIBVAR) install

clean:
	cd plot; $(MAKE) -$(MAKEFLAGS)f plot.mk clean
	cd t300; $(MAKE) -$(MAKEFLAGS)f t300.mk clean
	cd t300s; $(MAKE) -$(MAKEFLAGS)f t300s.mk clean
	cd t4014; $(MAKE) -$(MAKEFLAGS)f t4014.mk clean
	cd t450; $(MAKE) -$(MAKEFLAGS)f t450.mk clean
	cd vt0; $(MAKE) -$(MAKEFLAGS)f vt0.mk clean

clobber:
	cd plot; $(MAKE) -$(MAKEFLAGS)f plot.mk clobber
	cd t300; $(MAKE) -$(MAKEFLAGS)f t300.mk clobber
	cd t300s; $(MAKE) -$(MAKEFLAGS)f t300s.mk clobber
	cd t4014; $(MAKE) -$(MAKEFLAGS)f t4014.mk clobber
	cd t450; $(MAKE) -$(MAKEFLAGS)f t450.mk clobber
	cd vt0; $(MAKE) -$(MAKEFLAGS)f vt0.mk clobber

size:
	cd plot; $(MAKE) -$(MAKEFLAGS)f plot.mk size
	cd t300; $(MAKE) -$(MAKEFLAGS)f t300.mk size
	cd t300s; $(MAKE) -$(MAKEFLAGS)f t300s.mk size
	cd t4014; $(MAKE) -$(MAKEFLAGS)f t4014.mk size
	cd t450; $(MAKE) -$(MAKEFLAGS)f t450.mk size
	cd vt0; $(MAKE) -$(MAKEFLAGS)f vt0.mk size

strip: 
	cd plot; $(MAKE) -$(MAKEFLAGS)f plot.mk strip
	cd t300; $(MAKE) -$(MAKEFLAGS)f t300.mk strip
	cd t300s; $(MAKE) -$(MAKEFLAGS)f t300s.mk strip
	cd t4014; $(MAKE) -$(MAKEFLAGS)f t4014.mk strip
	cd t450; $(MAKE) -$(MAKEFLAGS)f t450.mk strip
	cd vt0; $(MAKE) -$(MAKEFLAGS)f vt0.mk strip

#	These targets are useful but optional

partslist:
	cd plot; $(MAKE) -$(MAKEFLAGS)f plot.mk partslist
	cd t300; $(MAKE) -$(MAKEFLAGS)f t300.mk partslist
	cd t300s; $(MAKE) -$(MAKEFLAGS)f t300s.mk partslist
	cd t4014; $(MAKE) -$(MAKEFLAGS)f t4014.mk partslist
	cd t450; $(MAKE) -$(MAKEFLAGS)f t450.mk partslist
	cd vt0; $(MAKE) -$(MAKEFLAGS)f vt0.mk partslist

productdir:
	cd plot; $(MAKE) -$(MAKEFLAGS)f plot.mk productdir
	cd t300; $(MAKE) -$(MAKEFLAGS)f t300.mk productdir
	cd t300s; $(MAKE) -$(MAKEFLAGS)f t300s.mk productdir
	cd t4014; $(MAKE) -$(MAKEFLAGS)f t4014.mk productdir
	cd t450; $(MAKE) -$(MAKEFLAGS)f t450.mk productdir
	cd vt0; $(MAKE) -$(MAKEFLAGS)f vt0.mk productdir

product:
	cd plot; $(MAKE) -$(MAKEFLAGS)f plot.mk product
	cd t300; $(MAKE) -$(MAKEFLAGS)f t300.mk product
	cd t300s; $(MAKE) -$(MAKEFLAGS)f t300s.mk product
	cd t4014; $(MAKE) -$(MAKEFLAGS)f t4014.mk product
	cd t450; $(MAKE) -$(MAKEFLAGS)f t450.mk product
	cd vt0; $(MAKE) -$(MAKEFLAGS)f vt0.mk product

srcaudit:
	cd plot; $(MAKE) -$(MAKEFLAGS)f plot.mk srcaudit
	cd t300; $(MAKE) -$(MAKEFLAGS)f t300.mk srcaudit
	cd t300s; $(MAKE) -$(MAKEFLAGS)f t300s.mk srcaudit
	cd t4014; $(MAKE) -$(MAKEFLAGS)f t4014.mk srcaudit
	cd t450; $(MAKE) -$(MAKEFLAGS)f t450.mk srcaudit
	cd vt0; $(MAKE) -$(MAKEFLAGS)f vt0.mk srcaudit

listing:	listmk
	cd plot; $(MAKE) -$(MAKEFLAGS)f plot.mk LIST=$(LIST) listing
	cd t300; $(MAKE) -$(MAKEFLAGS)f t300.mk LIST=$(LIST) listing
	cd t300s; $(MAKE) -$(MAKEFLAGS)f t300s.mk LIST=$(LIST) listing
	cd t4014; $(MAKE) -$(MAKEFLAGS)f t4014.mk LIST=$(LIST) listing
	cd t450; $(MAKE) -$(MAKEFLAGS)f t450.mk LIST=$(LIST) listing
	cd vt0; $(MAKE) -$(MAKEFLAGS)f vt0.mk LIST=$(LIST) listing

listmk:
	pr -n $(MAKEFILE) | $(LIST)

ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
