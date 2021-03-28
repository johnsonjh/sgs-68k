#	START NEW ARIX SCCS HEADER
#
#	@(#) makefile: version 25.1 created on 12/2/91 at 19:08:18
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)makefile	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT: ident	"curses:demo/makefile	1.3"
#ident	"@(#)curses:makefile	25.1"
IROOT =
include $(IROOT)/macdefs
LFLAGS=	-lcurses

all: aliens rain menu worms

aliens:	aliens.c
	$(CC) $(CFLAGS) $(LDFLAGS) aliens.c -o aliens $(LFLAGS)

rain: rain.c
	$(CC) $(CFLAGS) $(LDFLAGS) rain.c -o rain $(LFLAGS)

menu: menu.c
	$(CC) $(CFLAGS) $(LDFLAGS) menu.c -o menu $(LFLAGS)
	ln menu.data .menu.data

worms: worms.c
	$(CC) $(CFLAGS) $(LDFLAGS) worms.c -o worms $(LFLAGS)

clean:
	-rm -f *.o

clobber:	clean
	-rm -f aliens rain menu worms greplist tags .menu.data

ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
