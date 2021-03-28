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

.SUFFIXES: .p
IROOT =
include $(IROOT)/macdefs
include $(IROOT)/sgsdefs

#POSIX
SVID_FLAG=

PROF=
NONPROF=
INC=$(INCRT)
INCLIBC=../../inc
SINC=$(ROOT)/usr/include
DEFLIST=
SDEFLIST=
INCLIST=-I$(INCLIBC)

OBJECTS=\
zprnt_def.o 

POBJECTS=


nonprof: $(OBJECTS)

prof: $(POBJECTS)

.c.o .c.p:
	$(NONPROF)@echo $*.c:
	$(NONPROF)$(CC) $(DEFLIST) $(SDEFLIST) $(INCLIST) $(CFLAGS) -c $*.c
	$(PROF)@echo $*.c:
	$(PROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p $*.c && mv $(*F).o $*.p

clean:
	-rm -f *.o

clobber:	clean
	-rm -f greplist tags
SOURCES=\
	zprnt_def.c print.h 
ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
