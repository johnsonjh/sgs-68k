#ident	"@(#)assert.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
include $(IROOT)/macdefs
include $(IROOT)/sgsdefs

SYSTRAP_H=$(IROOT)/systrap

# POSIX
SVID_FLAG=

PROF=
NONPROF=
M4=m4 $(M4FLAGS) ../m4.def

DEFLIST= -DM32
SDEFLIST= -DMCOUNT=#

.SUFFIXES: .p .n

OBJECTS= _assert.o _assert.p _assert.n

all:	$(OBJECTS)

.c.o .c.p .c.n:
	$(PROF)@echo $*.c:
	$(PROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p $*.c  \
		&& mv $(*F).o $*.p
	@echo $*.c:
	$(CC) $(DEFLIST) $(NDYNFLAGS) $(INCLIST) $(CFLAGS) -c $*.c \
		&& mv $(*F).o $*.n
	$(NONPROF)@echo $*.c:
	$(NONPROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c $*.c

.s.o .s.p:
	$(NONPROF)@echo $*.s:
	$(NONPROF)$(M4) $(SDEFLIST)  $*.s   > $*.m4.s   
	$(NONPROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c $*.m4.s \
		&& mv $*.m4.o $*.o
	$(PROF)@echo $*.s:
	$(PROF)$(M4) ../mcount.def  $*.s   > $*.m4.s 
	$(PROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p $*.m4.s  \
		&& mv $*.m4.o $*.p
	-rm $*.m4.s

clean:
	-rm -f *.o
	-rm -f *.n
	-rm -f *.p

clobber: clean
