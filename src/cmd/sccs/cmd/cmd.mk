#	START NEW ARIX SCCS HEADER
#
#	@(#) cmd.mk: version 25.1 created on 12/2/91 at 17:07:15
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)cmd.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	ATT: ident	"sccs:cmd/cmd.mk	6.14"
#
#

#ident	"@(#)sccs:cmd.mk	25.1"

include	$(IROOT)/macdefs

HDR = ../hdr

HELPLOC = $(ROOT)/usr/lib/help

LIBS = ../lib/comobj.a	\
	../lib/cassi.a	\
	../lib/mpwlib.a	

BIN = $(ROOT)/usr/bin

SOURCE = admin.c comb.c delta.c get.c help.c help2.c prs.c rmchg.c \
	 unget.c val.c vc.c what.c

ARGS =

C_CMDS = admin	\
	cdc	\
	comb	\
	delta	\
	get	\
	prs	\
	rmdel	\
	sact	\
	unget	\
	val	\
	vc	\
	what

CMDS = $(C_CMDS)	\
	sccsdiff

all:	$(LIBS) $(CMDS) help help2

admin:	admin.o	$(LIBS)
	$(CC)  admin.o $(LIBS) -o admin $(LDFLAGS)

admin.o:	admin.c
	$(CC) -c $(CFLAGS)  admin.c

cdc:	rmchg
	-ln	rmchg cdc

comb:	comb.o	$(LIBS)
	$(CC)  comb.o $(LIBS) -o comb $(LDFLAGS)

comb.o:	comb.c
	$(CC) -c $(CFLAGS)  comb.c

delta:	delta.o	$(LIBS)
	$(CC)  delta.o $(LIBS) -o delta $(LDFLAGS)

delta.o:	delta.c
	$(CC) -c $(CFLAGS)  delta.c

get:	get.o	$(LIBS)
	$(CC)  get.o $(LIBS) -o get $(LDFLAGS)

get.o:	get.c
	$(CC) -c $(CFLAGS)  get.c

help:	help.o
	$(CC)  help.o -o help $(LDFLAGS)

help.o:	help.c
	$(CC) -c $(CFLAGS) help.c

help2:	help2.o	$(LIBS)
	$(CC)  help2.o $(LIBS) -o help2 $(LDFLAGS)

help2.o: help2.c
	$(CC) -c $(CFLAGS) help2.c

prs:	prs.o	$(LIBS)
	$(CC)  prs.o $(LIBS) -o prs $(LDFLAGS)

prs.o:	prs.c
	$(CC) -c $(CFLAGS)  prs.c
	
rmdel:	rmchg $(LIBS)
	-ln rmchg rmdel

rmchg:	rmchg.o $(LIBS)
	$(CC)  rmchg.o $(LIBS) -o rmchg $(LDFLAGS)

rmchg.o:	rmchg.c
	$(CC) -c $(CFLAGS)  rmchg.c

sact:	unget
	-ln unget sact

sccsdiff:	sccsdiff.sh
	-cp sccsdiff.sh sccsdiff

unget:	unget.o	$(LIBS)
	$(CC)  unget.o $(LIBS) -o unget $(LDFLAGS)

unget.o:	unget.c
	$(CC) -c $(CFLAGS)  unget.c

val:	val.o	$(LIBS)
	$(CC)  val.o $(LIBS) -o val $(LDFLAGS)

val.o:	val.c
	$(CC) -c $(CFLAGS) val.c

vc:	vc.o	$(LIBS)
	$(CC)  vc.o $(LIBS) -o vc $(LDFLAGS)

vc.o:	vc.c
	$(CC) -c $(CFLAGS) vc.c

what:	what.o	$(LIBS)
	$(CC)  what.o $(LIBS) -o what $(LDFLAGS)

what.o:	what.c
	$(CC) -c $(CFLAGS) what.c

$(LIBS):
	cd ../lib; $(MAKE) -f lib.mk

install:	all
	@for i in $(CMDS); \
	do \
		echo "$(INS) $$i $(BIN)/$$i"; \
		$(INS) $$i $(BIN)/$$i; \
		chmod 755 $(BIN)/$$i; \
		chown bin $(BIN)/$$i; \
		chgrp bin $(BIN)/$$i; \
	done
	-if [ ! -d $(HELPLOC) ]; \
	then \
		mkdir $(HELPLOC); \
	fi
	-if [ ! -d $(HELPLOC)/lib ]; \
	then \
		mkdir $(HELPLOC)/lib; \
	fi
	chmod 755 $(HELPLOC) $(HELPLOC)/lib
	chown bin $(HELPLOC) $(HELPLOC)/lib
	chgrp bin $(HELPLOC) $(HELPLOC)/lib
	$(INS) help2 $(HELPLOC)/lib/help2 
	chmod 755 $(HELPLOC)/lib/help2 
	chown bin $(HELPLOC)/lib/help2 
	chgrp bin $(HELPLOC)/lib/help2 

clean:
	-rm -f *.o
	-rm -f rmchg

clobber:	clean
	-rm -f $(CMDS) help help2 greplist tags

ctags:	
	ctags	$(SOURCE)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCE)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist

