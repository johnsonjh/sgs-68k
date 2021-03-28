#	START NEW ARIX SCCS HEADER
#
#	@(#) help.mk: version 25.1 created on 12/2/91 at 17:08:55
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)help.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	ATT:ident	"sccs:help.d/help.mk	6.6"
#
#

#ident	"@(#)help.d:help.mk	25.1"

include	$(IROOT)/macdefs

HELPLIB = $(ROOT)/usr/lib/help

FILES1 = ad bd cb cm cmds co de default
FILES2 = ge he prs rc un ut vc

all:

install: all
	if [ ! -d $(HELPLIB) ] ;\
	then mkdir $(HELPLIB) ;\
	fi ;
	-chmod 775 $(HELPLIB)
	-chgrp bin $(HELPLIB)
	-chown bin $(HELPLIB)
	-cd $(HELPLIB); rm -f $(FILES1) $(FILES2)
	@for i in $(FILES1) $(FILES2); \
	do \
		echo "$(INS) $$i $(HELPLIB)/$$i"; \
		$(INS) $$i $(HELPLIB)/$$i; \
		chmod 644 $(HELPLIB)/$$i; \
		chown bin $(HELPLIB)/$$i; \
		chgrp bin $(HELPLIB)/$$i; \
	done
clean:

clobber:
