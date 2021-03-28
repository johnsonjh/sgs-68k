#	START NEW ARIX SCCS HEADER
#
#	@(#) ctrace.mk: version 25.1 created on 12/2/91 at 14:09:42
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)ctrace.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT:#ident	"ctrace:ctrace.mk	1.6"

#ident	"@(#)ctrace:ctrace.mk	25.1"

SROOT =
IROOT =
include $(IROOT)/macdefs

BIN = $(ROOT)/usr/bin
LIB = /usr/lib/ctrace
OLIB = $(ROOT)$(LIB)

CFLGS =  -DLIB=\"$(LIB) 

YFLAGS = -d

SOURCE	 = constants.h global.h main.c parser.y scanner.l lookup.c trace.c \
	   runtime.c ctcr
OTHER	 = ctrace.mk
OBJECTS	 = main.o parser.o scanner.o lookup.o trace.o

ctrace: $(OBJECTS)
	$(CC)  $(OBJECTS) -o $@ $(LDFLAGS)

.c.o:
	$(CC) $(CFLGS) $(CFLAGS) -c $*.c

all: ctrace

install: all
	-rm -f $(BIN)/ctc $(BIN)/ctcr $(BIN)/ctrace
	$(INS) ctcr $(BIN)/ctcr
	chmod 755 $(BIN)/ctcr 
	chgrp bin $(BIN)/ctcr
	chown bin $(BIN)/ctcr
	$(INS) ctrace $(BIN)/ctrace 
	chmod 755 $(BIN)/ctrace
	chgrp bin $(BIN)/ctrace
	chown bin $(BIN)/ctrace
	ln $(BIN)/ctcr $(BIN)/ctc
	if [ ! -d $(OLIB) ];\
		then mkdir $(OLIB);\
		     chmod 755 $(OLIB);\
		     chown bin $(OLIB);\
		     chgrp bin $(OLIB);\
	fi
	$(INS) runtime.c $(OLIB)/runtime.c 
	chmod 644 $(OLIB)/runtime.c
	chgrp bin $(OLIB)/runtime.c
	chown bin $(OLIB)/runtime.c

ctags:
	ctags	$(SOURCE)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCE)		;\
	do echo "$$i \\" >> greplist	;\
	done
	echo /dev/null >> greplist
	chmod +x greplist

clean:
	rm -f *.o y.tab.h y.output

clobber: clean
	rm -f ctrace tags greplist
