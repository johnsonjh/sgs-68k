#	START NEW ARIX SCCS HEADER
#
#	@(#) ratfor.mk: version 25.1 created on 12/2/91 at 16:51:35
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)ratfor.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	ATT: ident	"ratfor:ratfor.mk	1.6"

#ident	"@(#)ratfor:ratfor.mk	25.1"

include	$(IROOT)/macdefs

INSDIR = $(ROOT)/usr/bin
YACCRM=-rm
OFILES = r0.o r1.o r2.o rio.o rlook.o rlex.o
SOURCE = r0.c r1.c r2.c rio.c rlex.c rlook.c

all: ratfor

ratfor:    $(OFILES) r.g.o
	   $(CC) $(OFILES) r.g.o $(LDFLAGS) -o ratfor -ly

$(OFILES): r.h r.g.h
r.g.c r.g.h:   r.g.y
	   $(YACC) -d r.g.y
	mv y.tab.c r.g.c
	mv y.tab.h r.g.h

test:
	   rtest ratfor

install:	all
	$(INS) ratfor $(INSDIR)/ratfor
	chmod 755 $(INSDIR)/ratfor
	chgrp bin $(INSDIR)/ratfor
	chown bin $(INSDIR)/ratfor

clean:
	   -rm -f *.o
	   $(YACCRM) -f r.g.c r.g.h

clobber:  clean
	   -rm -f ratfor greplist tags

ctags:	
	ctags	$(SOURCE)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCE)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist

