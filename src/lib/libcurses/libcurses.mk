#	START NEW ARIX SCCS HEADER
#
#	@(#) libcurses.mk: version 25.1 created on 12/2/91 at 19:09:50
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)libcurses.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT: ident	"curses:libcurses.mk	1.11"
#ident	"@(#)curses:libcurses.mk	25.1"
#
#	Curses Library High Level Makefile.
#
#	To INSTALL libcurses.a, the tic compiler and the tools type:
#
#		"make install"
#
#
#	To COMPILE libcurses.a, the tic compiler and the tools, type:
#
#		"make all"
#
#
#	To compile a particular file with normal compilation type:
#
#		"make FILES='<particular .o files>"
#
#
#	If debugging is desired then type:
#
#		"make O=debug FILES='<particular .o files>"
#
#
#	If tracing is desired then type:
#
#		"make O=trace FILES='<particular .o files>"
#
#
#	If profiling is desired then type:
#
#		"make O=profile FILES='<particular .o files>"
#
#
#	To compile only the tic compiler type:
#
#		"make tic"
#
#
#	To create cpio files for all directories type:
#		"make cpio"
#
#
SHELL=/bin/sh

IROOT =
include $(IROOT)/macdefs
USRLIB = $(ROOT)/usr/$(SGS)lib

all:	libcurses.a tools

libcurses.a:
	@cd ./screen ; $(MAKE) -$(MAKEFLAGS) rmhdrs
	@cd ./screen ; $(MAKE) -$(MAKEFLAGS) cktmp
	@cd ./screen ; $(MAKE) -$(MAKEFLAGS) libcurses.a
	@echo
	@echo "Libcurses.a has been made."
	@echo

tools:
	@cd ./screen ; $(MAKE) -$(MAKEFLAGS) tools
	@cd ./screen ; $(MAKE) -$(MAKEFLAGS) llib-lcurses.l
	@echo
	@echo "Libcurses/Terminfo tools have been made."
	@echo

tic:
	@cd ./screen ; $(MAKE) -$(MAKEFLAGS) tic
	@echo
	@echo "The tic compiler has been made."
	@echo
	
dummy:
	# make and install libcurses.a and tic
	@cd ./screen ; $(MAKE) -$(MAKEFLAGS) cktmp
	@cd ./screen ; $(MAKE) -$(MAKEFLAGS) install USRLIB=$(USRLIB) CC2=$(CC) ROOT2=""
	@echo
	@echo libcurses.a, the tic compiler, and associated tools have
	@echo been installed.

install:	dummy
	echo Now make native tic for compiling core entries
	cd ./screen; $(MAKE) -$(MAKEFLAGS)e CC=/bin/cc CFLAGS=-I$(LOCAL_INC) \
        LDFLAGS= \
	ticclob localtic
	echo The tic in $(ROOT)/usr/bin is built for the current environment.
	echo The tic found in `pwd`/screen is built native for this processor.
	echo Install the native tic in /tmp , temporarily, ;\
	echo where the terminfo make can find it. ;\
	cp screen/tic /tmp/tic
	chmod 755 /tmp/tic
	chgrp bin /tmp/tic
	chown bin /tmp/tic

clean:
	@cd ./screen ; $(MAKE) -$(MAKEFLAGS) clean

clobber:
	@cd ./screen ; $(MAKE) -$(MAKEFLAGS) clobber

cpio:
	@echo
	@echo "\n\tBuilding cpio files in ${HOME}\n\n\t\c"
	@find . -print|cpio -ocud|split -10000 - ${HOME}/crs.`date +%m%d`.
	@echo "\n\tcpio files have been built\n"

bsd:
	@echo
	cd screen; mv makefile makefile.sysv; cp makefile.bsd makefile
	cd screen; $(MAKE) -$(MAKEFLAGS)e rmident
	@echo "Curses has been converted for BSD"
# this has only been tested on 4.2BSD, but we assume libc has getopt.
