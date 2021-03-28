#ident	"@(#)system.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
PROF=
NONPROF=

include $(IROOT)/macdefs
include $(IROOT)/sgsdefs

SYSTRAP_H=$(IROOT)/systrap

#POSIX
SVID_FLAG=

M4=m4 $(M4FLAGS) ../m4.def

DEFLIST= -DM32
#SDEFLIST=

.SUFFIXES: .p

OBJECTS= \
access.o   execve.o   fstat.o    kill.o     open.o     unlink.o \
cerror.o   _exit.o    getpid.o   link.o     read.o     wait.o \
close.o    fcntl.o    ioctl.o    lseek.o    sbrk.o     write.o \
execl.o    fork.o     isatty.o   mcount.o   stat.o

all:	$(OBJECTS)

.c.o .c.p:
	$(PROF)@echo $*.c:
	$(PROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p $*.c  \
		&& mv $(*F).o $*.p
	$(NONPROF)@echo $*.c:
	$(NONPROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c $*.c

.s.o .s.p:
	$(NONPROF)@echo $*.s:
	$(NONPROF)$(M4) $(SDEFLIST)  -DMCOUNT=# $*.s   > $*.m4.s   
	$(NONPROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c $*.m4.s \
		&& mv $*.m4.o $*.o
	$(PROF)@echo $*.s:
	$(PROF)$(M4) ../mcount.def  $(SDEFLIST) $*.s   > $*.m4.s 
	$(PROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p $*.m4.s  \
		&& mv $*.m4.o $*.p
	-rm $*.m4.s

clean:
	-rm -f *.o
	-rm -f *.p

clobber: clean
