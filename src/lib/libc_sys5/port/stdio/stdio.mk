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

OBJECTS=\
clrerr.o    ctermid.o	\
cuserid.o   data.o	doscan.o    fdopen.o	\
fgetc.o     fgets.o     filbuf.o    findiop.o	\
flsbuf.o    fopen.o     fputc.o     fputs.o	\
fread.o     fseek.o     ftell.o     fwrite.o	\
getchar.o   getpass.o   gets.o      getw.o	\
popen.o     putchar.o   puts.o      putw.o	\
rew.o       scanf.o     setbuf.o    setvbuf.o	\
stdio_def.o system.o	\
tempnam.o   tmpfile.o   tmpnam.o    ungetc.o

POBJECTS=\
clrerr.p    ctermid.p	\
cuserid.p   data.p      doscan.p    fdopen.p	\
fgetc.p     fgets.p     filbuf.p    findiop.p	\
flsbuf.p    fopen.p     fputc.p     fputs.p	\
fread.p     fseek.p     ftell.p     fwrite.p	\
getchar.p   getpass.p   gets.p      getw.p	\
popen.p     putchar.p   puts.p      putw.p	\
rew.p       scanf.p     setbuf.p    setvbuf.p	\
system.p    \
tempnam.p   tmpfile.p   tmpnam.p    ungetc.p

nonprof: iob $(OBJECTS)

prof: iob $(POBJECTS)

iob:
	/bin/sh iob.sh > iob.c
	$(PROF)@echo iob.c:
	$(PROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p iob.c && mv iob.o iob.p
	$(NONPROF)@echo iob.c:
	$(NONPROF)$(CC) $(DEFLIST) $(NDYNFLAGS) $(INCLIST) $(CFLAGS) -c iob.c && mv iob.o iob.n
	$(NONPROF)@echo iob.c:
	$(NONPROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c iob.c

.c.o .c.p:
	$(PROF)@echo $*.c:
	$(PROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p $*.c && mv $(*F).o $*.p
	$(NONPROF)@echo $*.c:
	$(NONPROF)$(CC) $(DEFLIST) $(NDYNFLAGS) $(INCLIST) $(CFLAGS) -c $*.c && mv $(*F).o $*.n
	$(NONPROF)@echo $*.c:
	$(NONPROF)$(CC) $(DEFLIST) $(SDEFLIST) $(INCLIST) $(CFLAGS) -c $*.c

clean:
	-rm -f *.o
	-rm -f *.n

clobber:	clean
	-rm -f greplist tags

SOURCES=\
	fdopen.c     fputc.c      getpass.c    putw.c       system.c	\
	clrerr.c     fgetc.c      fputs.c      gets.c       rew.c        \
	tempnam.c    ctermid.c    fgets.c      fread.c      getw.c       \
	scanf.c      tmpfile.c    cuserid.c    filbuf.c     fseek.c      \
	setbuf.c     tmpnam.c     data.c       findiop.c    ftell.c      \
	popen.c      setvbuf.c    ungetc.c     flsbuf.c     fwrite.c     \
	putchar.c    stdio_def.c  doscan.c     fopen.c      getchar.c    \
	puts.c       stdiom.h

ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
