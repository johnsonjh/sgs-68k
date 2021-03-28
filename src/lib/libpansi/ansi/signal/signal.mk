#ident	"@(#)signal.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
include $(IROOT)/macdefs
include $(IROOT)/sgsdefs

SYSTRAP_H=$(IROOT)/systrap

SVID_FLAG=
PROF=
NONPROF=
M4=m4 $(M4FLAGS) ../m4.def

DEFLIST= -DM32
#SDEFLIST=

.SUFFIXES: .p .P .O

OBJECTS= \
raise.o    signal.o

all:	$(OBJECTS)

.c.o .c.p .c.O .c.P:
	$(PROF)@echo $*.c:
	$(PROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p $*.c \
		&& mv $(*F).o $*.p
	$(PROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p $*.c \
		&& mv $(*F).o $*.P
	$(NONPROF)@echo $*.c:
	$(NONPROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c $*.c \
		&& mv $(*F).o $*.O
	$(NONPROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c $*.c

.s.o .s.p .s.O .s.P:
	$(NONPROF)@echo $*.s:
	$(NONPROF)$(M4) -D_ANSI $(SDEFLIST) -DMCOUNT=# $*.s > $*.m4.s   
	$(NONPROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c $*.m4.s \
		&& mv $*.m4.o $*.o
	$(NONPROF)$(M4) $(SDEFLIST) -DMCOUNT=# $*.s > $*.m4.s  
	$(NONPROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c $*.m4.s \
		&& mv $*.m4.o $*.O
	$(PROF)@echo $*.s:
	$(PROF)$(M4) -D_ANSI ../mcount.def $(SDEFLIST) $*.s > $*.m4.s 
	$(PROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p $*.m4.s \
		&& mv $*.m4.o $*.p
	$(PROF)$(M4) ../mcount.def $(SDEFLIST) $*.s > $*.m4.s 
	$(PROF)$(CC)  $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p $*.m4.s \
		&& mv $*.m4.o $*.P
	-rm $*.m4.s

clean:
	-rm -f *.o *.O
	-rm -f *.p *.P

clobber: clean
