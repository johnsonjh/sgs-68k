#ident	"@(#)print.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT: ident	"libc-port:print/makefile	1.12"
#
# makefile for libc/port/print
#
#

.SUFFIXES: .p .n
IROOT =
include $(IROOT)/macdefs
include $(IROOT)/sgsdefs
PROF=
NONPROF=
INC=$(INCRT)
INCLIBC=../../inc
SINC=$(ROOT)/usr/include
DEFLIST=
SDEFLIST=
INCLIST=-I$(INCLIBC)

OBJECTS=\
zprnt_def.o doprnt.o    fprintf.o   printf.o    sprintf.o  vfprintf.o  \
	vprintf.o  vsprintf.o

POBJECTS=\
doprnt.p    fprintf.p   printf.p    sprintf.p  vfprintf.p  vprintf.p  \
	vsprintf.p

nonprof: $(OBJECTS)

prof: $(POBJECTS)

.c.o .c.p .c.n:
	$(NONPROF)@echo $*.c:
	$(NONPROF)$(CC) $(DEFLIST) $(NDYNFLAGS) $(SDEFLIST) $(INCLIST) $(CFLAGS) -c $*.c && mv $(*F).o $*.n
	$(NONPROF)$(CC) $(DEFLIST) $(SDEFLIST) $(INCLIST) $(CFLAGS) -c $*.c
	$(PROF)@echo $*.c:
	$(PROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p $*.c && mv $(*F).o $*.p

clean:
	-rm -f *.o
	-rm -f *.n

clobber:	clean
	-rm -f greplist tags
SOURCES=\
	zprnt_def.c fprintf.c   printf.c    vprintf.c    doprnt.c    print.h     \
	sprintf.c   vfprintf.c  vsprintf.c
ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
