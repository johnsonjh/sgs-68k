#ident	"@(#)sys.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT: ident	"libc-port:sys/makefile	1.5.1.7"
#
# makefile for libc/port/sys
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
INCSYS=$(INCRT)/sys
INCLIBC=../../inc
DEFLIST=
SDEFLIST=
INCLIST=-I$(INCLIBC)

OBJECTS=\
execle.o      execv.o	lockf.o    \
msgsys.o      semsys.o      shmsys.o  

POBJECTS=\
execle.p      execv.p	lockf.p    \
msgsys.p      semsys.p      shmsys.p  

nonprof: $(OBJECTS)

prof: $(POBJECTS)

msgsys.o msgsys.p semsys.o semsys.p \
	shmsys.o shmsys.p: $(INCSYS)/ipc.h
msgsys.o msgsys.p: $(INCSYS)/msg.h
semsys.o semsys.p: $(INCSYS)/sem.h
shmsys.o shmsys.p: $(INCSYS)/shm.h
msgsys.o msgsys.p semsys.o semsys.p \
	shmsys.o shmsys.p: $(INCSYS)/types.h
lockf.o lockf.p: $(INC)/errno.h 
lockf.o lockf.p: $(INC)/fcntl.h 
lockf.o lockf.p: $(INC)/unistd.h

.c.o .c.p:
	$(PROF)@echo $*.c:
	$(PROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p $*.c  && mv $(*F).o $*.p
	$(NONPROF)@echo $*.c:
	$(NONPROF)$(CC) $(DEFLIST) $(SDEFLIST) $(INCLIST) $(CFLAGS) -c $*.c
clean:
	-rm -f *.o

clobber:	clean
	-rm -f greplist tags

SOURCES=\ execle.c execv.c lockf.c msgsys.c semsys.c shmsys.c

ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
