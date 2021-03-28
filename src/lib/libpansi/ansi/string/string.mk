#ident	"@(#)string.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
PROF=
NONPROF=

include $(IROOT)/macdefs
include $(IROOT)/sgsdefs

SYSTRAP_H=$(IROOT)/systrap

#POSIX
SVID_FLAG=

M4=m4 $(M4FLAGS) ../m4.def

DEFLIST= -DM32
SDEFLIST= -DMCOUNT=#

.SUFFIXES: .p .O .P

OBJECTS= \
memchr.o     memset.o     strcpy.o     strncat.o    strrchr.o    strxfrm.o \
memcmp.o     strcat.o     strcspn.o    strncmp.o    strspn.o \
memcpy.o     strchr.o     strerror.o   strncpy.o    strstr.o \
memmove.o    strcmp.o     strlen.o     strpbrk.o    strtok.o

all:	$(OBJECTS)

.c.o .c.p .c.O .c.P:
	$(PROF)@echo $*.c:
	$(PROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p -M2 $*.c \
		&& mv $(*F).o $*.p
	$(PROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p $*.c \
		&& mv $(*F).o $*.P
	$(NONPROF)@echo $*.c:
	$(NONPROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c $*.c \
		&& mv $(*F).o $*.O
	$(NONPROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -M2 $*.c

.s.o .s.p .s.O .s.P:
	$(NONPROF)@echo $*.s:
	$(NONPROF)$(M4) -D__STRICT_STDC__ $(SDEFLIST) $*.s > $*.m4.s   
	$(NONPROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -M2 $*.m4.s \
		&& mv $*.m4.o $*.o
	$(NONPROF)$(M4) $(SDEFLIST) $*.s > $*.m4.s   
	$(NONPROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c $*.m4.s \
		&& mv $*.m4.o $*.O
	$(PROF)@echo $*.s:
	$(PROF)$(M4) -D_STRICT_STDC__ ../mcount.def $*.s > $*.m4.s 
	$(PROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p -M2 $*.m4.s  \
		&& mv $*.m4.o $*.p
	$(PROF)$(M4) ../mcount.def $*.s > $*.m4.s 
	$(PROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p $*.m4.s  \
		&& mv $*.m4.o $*.P
	-rm $*.m4.s

clean:
	-rm -f *.o *.O
	-rm -f *.p *.P

clobber: clean
