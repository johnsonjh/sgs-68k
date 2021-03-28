#ident	"@(#)lex.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.



SROOT =
IROOT =
include $(IROOT)/macdefs
include $(IROOT)/sgsdefs

# POSIX
SVID_FLAG=

INSDIR=$(ROOT)/usr/bin
FILES = main.o sub1.o sub2.o header.o
SOURCE = header.c ldefs.c main.c once.c sub1.c sub2.c

all: lex

lex:	$(FILES) y.tab.o
	$(CC)  $(FILES) y.tab.o -ly -o lex $(LDFLAGS)

$(FILES): ldefs.c $(FRC)
main.o:	  once.c $(FRC)
y.tab.c:  parser.y $(FRC)
	  $(YACC) parser.y

install: all
	$(INS) lex $(INSDIR)/lex
	chmod 755 $(INSDIR)/lex
	chgrp bin $(INSDIR)/lex
	chown bin $(INSDIR)/lex

ctags:
	ctags	$(SOURCE)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCE)		;\
	do echo "$$i \\" >> greplist	;\
	done
	echo /dev/null >> greplist
	chmod +x greplist

clean:
	 -rm -f *.o y.tab.c

clobber: clean
	 -rm -f lex tags greplist

lint:	main.c sub1.c sub2.c header.c y.tab.c once.c ldefs.c
	lint -p main.c sub1.c sub2.c header.c y.tab.c once.c ldefs.c
