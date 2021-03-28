#	START NEW ARIX SCCS HEADER
#
#	@(#) libl.mk: version 25.1 created on 12/2/91 at 19:37:36
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)libl.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT: ident	"libl:libl.mk	1.7"
#ident	"@(#)libl:libl.mk	25.1"

LIB=$(ROOT)/$(SGS)lib
USRLIB=$(ROOT)/usr/$(SGS)lib

IROOT =
include $(IROOT)/macdefs

LORDER=lorder
LIBOBJS=allprint.o main.o reject.o yyless.o yywrap.o

all:     libl.a

libl.a: $(LIBOBJS)
	rm -f tmplib.a
	ar rv tmplib.a `$(LORDER) *.o | tsort`;

install:  ncform nrform all
	$(INS) tmplib.a $(USRLIB)/libl.a
	$(AR) ts $(USRLIB)/libl.a > /dev/null
	chmod 644 $(USRLIB)/libl.a
	chgrp bin $(USRLIB)/libl.a
	chown bin $(USRLIB)/libl.a
	if [ ! -d $(USRLIB)/lex ];\
		then mkdir $(USRLIB)/lex;\
	fi
	-rm -f $(USRLIB)/lex/ncform;
	$(INS) lib/ncform $(USRLIB)/lex/ncform
	chmod 444 $(USRLIB)/lex/ncform
	chgrp bin $(USRLIB)/lex/ncform
	chown bin $(USRLIB)/lex/ncform
	-rm -f $(USRLIB)/lex/nrform;
	$(INS) lib/nrform $(USRLIB)/lex/nrform
	chmod 444 $(USRLIB)/lex/nrform
	chgrp bin $(USRLIB)/lex/nrform
	chown bin $(USRLIB)/lex/nrform

clean:
	-rm -f *.o

clobber:	clean
	-rm -f tmplib.a greplist tags

allprint.o:	lib/allprint.c
	$(CC) -c $(CFLAGS) lib/allprint.c
main.o:		lib/main.c
	$(CC) -c $(CFLAGS) lib/main.c
reject.o:		lib/reject.c
	$(CC) -c $(CFLAGS) lib/reject.c
yyless.o:		lib/yyless.c
	$(CC) -c $(CFLAGS) lib/yyless.c
yywrap.o:		lib/yywrap.c
	$(CC) -c $(CFLAGS) lib/yywrap.c

ncform:	lib/ncform

nrform:	lib/nrform

SOURCES= lib/main.c      lib/nrform      lib/yyless.c			\
	lib/allprint.c  lib/ncform      lib/reject.c    lib/yywrap.c

ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
