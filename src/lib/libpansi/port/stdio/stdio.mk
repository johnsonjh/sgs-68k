#ident	"@(#)stdio.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT: ident	"libc-port:stdio/makefile	1.14"
#
# makefile for libc/port/stdio
#
#


include	$(IROOT)/macdefs
include	$(IROOT)/sgsdefs

.SUFFIXES: .p .n
PROF=
NONPROF=
INC=$(INCRT)
INCSYS=$(INCRT)/sys
INCLIBC=../../inc
SINC=$(ROOT)/usr/include
DEFLIST=
SDEFLIST=
INCLIST=-I$(INCLIBC)

# POSIX
SVID_FLAG=

OBJECTS=\
ctermid.o	\
cuserid.o   fdopen.o	\
getpass.o   getw.o	\
popen.o     putw.o	\
stdio_def.o tcsetpgrp.o \
tempnam.o

POBJECTS=\
ctermid.p	\
cuserid.p   fdopen.p	\
getpass.p   getw.p	\
popen.p     putw.p	\
tcsetpgrp.p \
tempnam.p

nonprof: $(OBJECTS)

prof: $(POBJECTS)


.c.o .c.p:
	$(PROF)@echo $*.c:
	$(PROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p $*.c && mv $(*F).o $*.p
	$(NONPROF)@echo $*.c:
	$(NONPROF)$(CC) $(DEFLIST) $(NDYNFLAGS) $(INCLIST) $(CFLAGS) -c $*.c && mv $(*F).o $*.n
	$(NONPROF)@echo $*.c:
	$(NONPROF)$(CC) $(DEFLIST) $(SDEFLIST) $(INCLIST) $(CFLAGS) -c $*.c

clean:
	-rm -f *.o

clobber:	clean
	-rm -f greplist tags

SOURCES=\
	fdopen.c     getpass.c    putw.c       \
	tempnam.c    ctermid.c    getw.c       \
	cuserid.c \
	popen.c \
	stdio_def.c \
	stdiom.h	  tcsetpgrp.c

ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
