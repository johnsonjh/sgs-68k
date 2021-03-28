#ident	"@(#)errno.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
PROF=
NONPROF=

include $(IROOT)/macdefs
include $(IROOT)/sgsdefs

SYSTRAP_H=$(IROOT)/systrap

# POSIX
SVID_FLAG=

M4=m4 $(M4FLAGS) ../m4.def

DEFLIST= -DM32
SDEFLIST= -DMCOUNT=#

.SUFFIXES: .p .O .P

OBJECTS= _errno.var.o errno.var.o

all:	$(OBJECTS)

.c.o .c.p .c.O .c.P:
	$(PROF)@echo $*.c:
	$(PROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -M2 -p $*.c \
		&& mv $(*F).o $*.p
	$(PROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p $*.c \
		&& mv $(*F).o $*.P
	$(NONPROF)@echo $*.c:
	$(NONPROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c $*.c \
		&& mv $(*F).o $*.O
	$(NONPROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -M2 $*.c

clean:
	-rm -f *.o *.O
	-rm -f *.p *.P

clobber: clean
