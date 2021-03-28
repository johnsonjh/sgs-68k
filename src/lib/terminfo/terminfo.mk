#	START NEW ARIX SCCS HEADER
#
#	@(#) terminfo.mk: version 25.1 created on 12/2/91 at 20:21:38
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)terminfo.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT: ident	"terminfo:terminfo.mk	1.2"
#
#	terminfo makefile
#


#ident	"@(#)terminfo:terminfo.mk	25.1"

include	$(IROOT)/macdefs

TERMDIR=./terminfo
INSDIR=${ROOT}/usr/lib
TABDIR=${ROOT}/usr/lib/tabset
PARTS=	header *.ti trailer
TERMINFODIR=$(INSDIR)/terminfo
INSDIRSRC=$(TERMINFODIR)/src


all:	instsrc ckdir terminfo.src
#	the libcurses make puts a copy of the latest tic in /tmp.
#	if it exists, use it. Otherwise, use default (normally in /usr/bin).
	-@if [ -r /tmp/tic ] 					 ;\
	then 							  \
	echo USING /tmp/tic					 ;\
	TERMINFO=./terminfo 2>&1 /tmp/tic -v terminfo.src > errs ;\
	else 							  \
	echo USING default tic					 ;\
	TERMINFO=./terminfo 2>&1 tic -v terminfo.src > errs 	 ;\
	fi
	#@touch install
	@echo
	@sh ./ckout
	@echo
	@echo
	@echo

install: all
	find ./terminfo -print|cpio -pdmu $(INSDIR)
	-@find $(INSDIR)/terminfo -print|while read i	;\
	do						\
		echo $$i				;\
		if [ -d $$i ]				;\
		then chmod 755 $$i			;\
		else chmod 644 $$i			;\
		fi					;\
		chgrp bin $$i				;\
		chown bin $$i				;\
		touch $(NEWDATE) $$i			;\
	done
	-@cd tabset 					;\
	for i in *					;\
	do						\
		echo "$(INS) $$i $(TABDIR)/$$i"		;\
		$(INS) $$i $(TABDIR)/$$i		;\
		chmod 644 $(TABDIR)/$$i			;\
		chgrp bin $(TABDIR)/$$i			;\
		chown bin $(TABDIR)/$$i			;\
	done

terminfo.src:	$(PARTS)
	@cat $(PARTS) > terminfo.src

clean:
	rm -f terminfo.src install errs

clobber: clean
	rm -rf ./terminfo

ckdir:
	@echo "terminfo database will be built in $(TERMDIR)."
	@echo "checking for the existence of $(TERMDIR):"
	@echo
	@if [ -d $(TERMDIR) ]; \
	then \
		echo "\t$(TERMDIR) exists"; \
	else  \
		echo "\tbuilding $(TERMDIR)"; \
		mkdir $(TERMDIR); \
	fi
		chown bin $(TERMDIR)
		chgrp bin $(TERMDIR)
		chmod 755 $(TERMDIR)
	@echo
	@echo
	@echo
	@echo "terminfo database will reference file in $(TABDIR)."
	@echo "checking for the existence of $(TABDIR):"
	@echo
	@if [ -d $(TABDIR) ]; \
	then \
		echo "\t$(TABDIR) exists"; \
	else  \
		echo "\tbuilding $(TABDIR)"; \
		mkdir $(TABDIR); \
	fi
		chmod 755 $(TABDIR)
		chown bin $(TABDIR)
		chgrp bin $(TABDIR)
	@echo
	@echo It will take quite some time to generate $(TERMDIR)/*/*.
	@echo
 
instsrc: $(INSDIRSRC) terminfo.src cvt.ex cvt.h cvt.sed
	@-for i in terminfo.src README cvt.ex cvt.h cvt.sed ;\
	do \
	echo "$(INS) $$i $(INSDIRSRC)/$$i" ;\
	$(INS) $$i $(INSDIRSRC)/$$i ;\
	chmod 644 $(INSDIRSRC)/$$i ; \
	chgrp bin $(INSDIRSRC)/$$i ; \
	chown bin $(INSDIRSRC)/$$i ; \
	done

$(INSDIRSRC): $(TERMINFODIR)
	@if [ ! -d $(INSDIRSRC) ]; \
	then \
		mkdir $(INSDIRSRC); \
	fi
	chmod 755 $(INSDIRSRC)
	chown bin $(INSDIRSRC)
	chgrp bin $(INSDIRSRC)

$(TERMINFODIR):
	@if [ ! -d $(TERMINFODIR) ]; \
	then \
		mkdir $(TERMINFODIR); \
	fi
	chmod 755 $(TERMINFODIR)
	chown bin $(TERMINFODIR)
	chgrp bin $(TERMINFODIR)

