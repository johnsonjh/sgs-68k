#	START NEW ARIX SCCS HEADER
#
#	@(#) libPW.mk: version 25.1 created on 12/2/91 at 18:44:34
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)libPW.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT: ident	"libPW:libPW.mk	1.17"
#ident	"@(#)libPW:libPW.mk	25.1"

IROOT =
include $(IROOT)/macdefs

LIB=$(ROOT)/$(SGS)lib
LNAME = libPW.a
STRIP = strip

OBJECTS =	abspath.o anystr.o bal.o curdir.o fdfopen.o giveup.o \
	imatch.o index.o lockit.o logname.o move.o patoi.o \
	patol.o regcmp.o regex.o rename.o repeat.o repl.o satoi.o \
	setsig.o sname.o strend.o substr.o trnslat.o userdir.o \
	username.o verify.o any.o xalloc.o xlink.o \
	xopen.o xpipe.o xunlink.o xwrite.o alloca.o \
	cat.o dname.o fatal.o clean.o userexit.o zero.o zeropad.o
NOBJECTS =	xmsg.o xcreat.o
LINKS =		Xmsg Xcreat

all:	init objects $(LNAME)

init:
	:

objects:	$(OBJECTS) $(LINKS)
alloca.o:	alloca.s
	$(CC) -c alloca.s

$(LNAME):
	-rm -f $(LNAME)
	$(AR) r $(LNAME) $(OBJECTS) $(NOBJECTS)
	$(STRIP) -r $(LNAME) 
	$(AR) ts $(LNAME)  >/dev/null

install:	all      
		$(INS) $(LNAME) $(LIB)/$(LNAME) 
		$(AR) ts $(LIB)/$(LNAME)  >/dev/null
		chmod 644 $(LIB)/$(LNAME)
		chgrp bin $(LIB)/$(LNAME)
		chown bin $(LIB)/$(LNAME)

clean:
	-rm -f $(OBJECTS) $(LINKS) $(NOBJECTS)

clobber:	clean
	-rm -f  greplist tags $(LNAME)

SOURCES= \
abspath.c   dname.c     logname.c   repl.c      userdir.c   xmsg.c \
any.c       fatal.c     patoi.c     satoi.c     userexit.c  xopen.c \
anystr.c    fdfopen.c   patol.c     setsig.c    username.c  xpipe.c \
bal.c       giveup.c    regcmp.c    sname.c     verify.c    xunlink.c \
cat.c       imatch.c    regex.c     strend.c    xalloc.c    xwrite.c \
clean.c     index.c     rename.c    substr.c    xcreat.c \
curdir.c    lockit.c    repeat.c    trnslat.c   xlink.c

# DO NOT Optimize out the  link/ulnk in routines which call alloca
Xmsg:	xmsg.c
	rm -f xmsg.o
	$(MAKE) -ief libPW.mk OPTFLAG="" xmsg.o
	touch Xmsg

Xcreat:	xcreat.c
	rm -f xcreat.o
	$(MAKE) -ief libPW.mk OPTFLAG="" xcreat.o
	touch Xcreat

ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
