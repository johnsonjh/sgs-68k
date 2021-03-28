#	START NEW ARIX SCCS HEADER
#
#	@(#) makefile: version 25.1 created on 12/2/91 at 19:08:42
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

#	ATT: ident	"curses:demo/pacman/makefile	1.2"
#ident	"@(#)curses:makefile	25.1"
IROOT =
include $(IROOT)/macdefs
DFLAGS =
OFILES =	pacman.o monster.o util.o movie.o 

pacman:	$(OFILES)
	$(CC) $(LDFLAGS) -o pacman $(OFILES) -lcurses

pacman.o:	pacman.c pacdefs.h
	$(CC) -c $(CFLAGS) $(DFLAGS) pacman.c

monster.o:	monster.c pacdefs.h
	$(CC) -c $(CFLAGS) $(DFLAGS) monster.c

util.o:	util.c pacdefs.h
	$(CC) -c $(CFLAGS) $(DFLAGS) util.c

movie.o:	movie.c pacdefs.h
	$(CC) -c $(CFLAGS) $(DFLAGS) movie.c


install:	pacman
	cp pacman /usr/games/pacman
	$(INS) pacman $(ROOT)/usr/games/pacman
	chmod 100 $(ROOT)/usr/games/pacman
	chgrp bin $(ROOT)/usr/games/pacman
	chown bin $(ROOT)/usr/games/pacman

strip:	pacman
	strip pacman

clean:
	-rm -f *.o

clobber:	clean
	-rm -f pacman errs core a.out
	-rm -f greplist tags


lint:	$(CFILES)
	lint -pc $(CFILES)

list:	$(CFILES) pacdefs.h makefile
	oprl -x makefile $(CFILES) pacdefs.h
	oprl -x -C $(CFILES)
	touch list

SOURCES=\
	getopt.c     monster.c    pacdeffs.h   util.c \
	movie.c      pacdefs.h    pacman.c
ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
