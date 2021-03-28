#ident	"@(#)stdio.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
include $(IROOT)/macdefs
include $(IROOT)/sgsdefs

SYSTRAP_H=$(IROOT)/systrap

#POSIX binary
SVID_FLAG=

PROF=
NONPROF=
M4=m4 $(M4FLAGS) ../m4.def

DEFLIST= -DM32
#SDEFLIST=

.SUFFIXES: .p .P .O .n

OBJECTS= \
_access81.o    _mktemp.o      fgetc.o        fwrite.o       setbuf.o \
_bufsync.o     _number.o      fgetpos.o      getc.o         setvbuf.o \
_cvt.o         _setup.o       fgets.o        getchar.o      sprintf.o \
_doprnt.o      _stdio.var.o   fopen.o        gets.o         sscanf.o \
_doscan.o      _string.o      fprintf.o      perror.o       tmpfile.o \
_dowrite.o     fputc.o        printf.o       tmpnam.o \
_endopen.o     _wrtchk.o      fputs.o        putc.o         ungetc.o \
_filbuf.o      _xflsbuf.o     fread.o        putchar.o      vfprintf.o \
_findbuf.o     clearerr.o     freopen.o      puts.o         vprintf.o \
_findiop.o     fclose.o       fscanf.o       remove.o       vsprintf.o \
_flsbuf.o      feof.o         fseek.o        \
_lowdigit.o    ferror.o       fsetpos.o      rewind.o \
_memccpy.o     fflush.o       ftell.o        scanf.o 	rename.o \
_sortfree.o

all:	rename_fix iob $(OBJECTS)

rename_fix:
	if [ -z "$(A1000)" ]; \
	then \
		rm -f rename.s; cp rename.as rename.s; rm -f rename.c;	\
		else rm -f rename.c; cp rename.comp rename.c; fi

perror.o perror.n perror.p perror.O perror.P:
	$(PROF)@echo $*.c:
	$(PROF)$(CC) -D_ANSI -D__STRICT_STDC__ $(DEFLIST) $(INCLIST)	\
		$(CFLAGS) -c -p $*.c && mv $*.o $*.p
	$(PROF)$(CC)  $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p $*.c	\
		&& mv $*.o $*.P
	$(NONPROF)@echo $*.c:
	$(NONPROF)$(CC)  $(DEFLIST) $(INCLIST) $(CFLAGS) -c $*.c	\
		&& mv $*.o $*.O
	$(CC) $(DEFLIST) -D__STRICT_STDC__ -D_ANSI $(NDYNFLAGS)		\
		$(INCLIST) $(CFLAGS) -c $*.c && mv $*.o $*.n
	$(NONPROF)$(CC) -D__STRICT_STDC__ -D_ANSI $(DEFLIST)		\
		$(INCLIST) $(CFLAGS) -c $*.c
iob:
	/bin/sh iob.sh > iob.c
	$(PROF)@echo iob.c:
	$(PROF)$(CC) -D_ANSI $(DEFLIST) $(INCLIST)			\
		$(CFLAGS) -c -p iob.c && mv iob.o iob.p
	$(PROF)$(CC)  $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p iob.c	\
		&& mv iob.o iob.P
	$(NONPROF)@echo iob.c:
	$(NONPROF)$(CC)  $(DEFLIST) $(INCLIST) $(CFLAGS) -c iob.c	\
		&& mv iob.o iob.O
	$(CC) $(DEFLIST)  -D_ANSI $(NDYNFLAGS)				\
		$(INCLIST) $(CFLAGS) -c iob.c && mv iob.o iob.n
	$(NONPROF)$(CC) -D_ANSI $(DEFLIST)				\
		$(INCLIST) $(CFLAGS) -c iob.c
	touch iob

.c.o .c.p .c.n:
	$(PROF)@echo $*.c:
	$(PROF)$(CC) -D_ANSI $(DEFLIST) $(INCLIST)	\
		$(CFLAGS) -c -p  $*.c && mv $(*F).o $*.p
	$(PROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p $*.c		\
		&& mv $(*F).o $*.P
	$(NONPROF)@echo $*.c:
	$(NONPROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c $*.c		\
		&& mv $(*F).o $*.O
	$(CC) $(DEFLIST) -D_ANSI $(NDYNFLAGS)		\
		$(INCLIST) $(CFLAGS) -c  $*.c && mv $(*F).o $*.n
	$(NONPROF)$(CC) -D_ANSI $(DEFLIST)		\
		$(INCLIST) $(CFLAGS) -c  $*.c

.s.o .s.p .s.O .s.P:
	$(NONPROF)@echo $*.s:
	$(NONPROF)$(M4) -D_ANSI $(SDEFLIST) -DMCOUNT=# $*.s > $*.m4.s   
	$(NONPROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c  $*.m4.s \
		&& mv $*.m4.o $*.o
	$(NONPROF)$(M4) $(SDEFLIST) -DMCOUNT=# $*.s > $*.m4.s   
	$(NONPROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c $*.m4.s \
		&& mv $*.m4.o $*.O
	$(NONPROF)@echo $*.s:
	$(NONPROF)$(M4) -D_ANSI $(SDEFLIST) -DMCOUNT=# $*.s > $*.m4.s   
	$(NONPROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c  $*.m4.s \
		&& mv $*.m4.o $*.n
	$(PROF)@echo $*.s:
	$(PROF)$(M4) -D_ANSI ../mcount.def $(SDEFLIST) $*.s > $*.m4.s 
	$(PROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p  $*.m4.s \
		&& mv $*.m4.o $*.p
	$(PROF)$(M4) ../mcount.def $(SDEFLIST) $*.s > $*.m4.s 
	$(PROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p $*.m4.s \
		&& mv $*.m4.o $*.P
	-rm $*.m4.s

clean:
	-rm -f *.o *.O
	-rm -f *.p *.P
	-rm -f *.n
	-rm -f iob

clobber: clean
