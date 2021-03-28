#	START NEW ARIX SCCS HEADER
#
#	@(#) efl.mk: version 25.1 created on 12/2/91 at 14:55:29
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)efl.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	ATT:ident	"efl:efl.mk	1.10"

#ident	"@(#)efl:efl.mk	25.1"

include	$(IROOT)/macdefs
INSDIR = $(ROOT)/usr/bin

FILES =  Makefile efl.1 defs tokens main.c init.c tailor.c\
		gram.head gram.dcl gram.expr gram.exec\
		lex.l fixuplex symtab.c\
		dcl.c addr.c struct.c field.c misc.c alloc.c mk.c\
		exec.c blklab.c simple.c print.c temp.c io.c error.c\
		pass2.c icfile.c free.c dclgen.c namgen.c
SOURCE =  main.c init.c tailor.c symtab.c dcl.c addr.c struct.c field.c \
		misc.c alloc.c mk.c exec.c blklab.c simple.c print.c temp.c \
		io.c error.c pass2.c icfile.c free.c dclgen.c namgen.c
OFILES = main.o init.o tailor.o gram.o lex.o symtab.o\
	dcl.o addr.o struct.o field.o blklab.o\
	mk.o misc.o alloc.o print.o simple.o exec.o temp.o io.o error.o\
	pass2.o icfile.o free.o dclgen.o namgen.o

all:	efl

efl:	$(OFILES)
	$(CC) -o efl $(FFLAG) $(CFLAGS)  $(OFILES) $(LDFLAGS)

$(OFILES):	defs
lex.o init.o:	tokdefs

gram.c:	gram.head gram.dcl gram.expr gram.exec tokdefs
	(sed <tokdefs "s/#define/%token/" ;\
		cat gram.head gram.dcl gram.expr gram.exec) >gram.in;\
		$(YACC) $(YFLAGS) gram.in;  mv y.tab.c gram.c;  rm gram.in;

lex.c:	fixuplex lex.l
	$(LEX) lex.l
	sh ./fixuplex
	mv lex.yy.c lex.c

tokdefs: tokens
	grep -n "^[^#]" <tokens | sed "s/\([^:]*\):\(.*\)/#define \2 \1/" >tokdefs

.c.o:
	$(CC) -c $(CFLAGS) $< 2>&1 | sed "/arning/d"

clean:
	rm -f *.o efl[cde].* gram.c lex.c tokdefs

clobber:	clean
	rm -f efl greplist tags

install :	all
	$(INS) efl $(INSDIR)/efl
	chmod 755 $(INSDIR)/efl
	chgrp bin $(INSDIR)/efl
	chown bin $(INSDIR)/efl

ctags:	
	ctags	$(SOURCE)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCE)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist

