#	START NEW ARIX SCCS HEADER
#
#	@(#) f77.mk: version 25.1 created on 12/2/91 at 17:36:43
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)f77.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#		Copyright (c) 1985 AT&T		#
#		All Rights Reserved		#
#	f77.mk	7.2	
#
# Makefile for the two pass M68K f77 compiler
#
# This makefile will call itself recursively with appropriate parameters
#

#ident	"@(#)f77/f77/mach:f77.mk	25.1"

include $(IROOT)/macdefs
include $(IROOT)/sgsdefs

# POSIX

SVID_FLAG =

BINDIR=	$(ROOT)/usr/bin
LIBDIR=	$(ROOT)/usr/lib

INCLIST = -I. -I$(DIRMIP) -I$(MACHDIR) $(SGS2INC)
SGS2INC = -I$M -I$P

YFLAGS=
YACC_CMD=	$(YACC) $(YFLAGS)

FPU = M68881
PROCESSOR=$(MACH)
FORTDEF = -DFORT -DSGS2 -D$(PROCESSOR) -D$(FPU) $(DEFS)
SGS2DEFS= -DFLEXNAMES -DSTKCK -DPROFILE -D$(FAMILY) -DTWOPASS -DITREES \
		$(FORTDEF)

#M = $(SROOT)/cmd/sgs/src/pcc2/common
#P = $(SROOT)/cmd/sgs/src/pcc2/mach
M = ../../pcc2/common
P = ../../pcc2/mach
DIRMIP = .
DIRM68K = .
DIRPCC = .
SDBINC = $P
CPFLAGS =
#CFLAGS = -g
#LDFLAGS =-g -lcdb

OBJECTS1 = main.o init.o gram.o lex.o proc.o equiv.o data.o \
	  expr.o exec.o intr.o io.o misc.o error.o put.o nfort.o \
	  $(PUT).o $(MACH).o $(MACHX).o ftrees.o
SOURCE1 = $(DIRMIP)/main.c $(DIRMIP)/init.c gram.c $(DIRMIP)/lex.c \
	  $(DIRMIP)/proc.c $(DIRMIP)/equiv.c $(DIRMIP)/data.c \
	  $(DIRMIP)/expr.c $(DIRMIP)/exec.c $(DIRMIP)/intr.c $(DIRMIP)/io.c \
	  $(DIRMIP)/misc.c $(DIRMIP)/error.c $(DIRMIP)/put.c \
	  $(FAMDIR)/$(PUT).c $(MACHDIR)/$(MACH).c $(MACHDIR)/$(MACHX).c

OBJECTS2= reader.o local2.o match.o allo.o comm2.o tablea.o tableb.o\
	  cgen.o cost.o xdefs.o

SOURCE2= $M/reader.c $M/match.c $M/allo.c $M/comm2.c $M/cgen.c \
	 $M/cost.c $(P)/$(PROCESSOR)/local2.c $M/xdefs.c\
	 $P/$(PROCESSOR)/tablea.c $P/$(PROCESSOR)/tableb.c

INCLUDES = $(DIRMIP)/defs $(DIRMIP)/defines machdefs $(DIRMIP)/ftypes
GRAMMAR = $(DIRMIP)/gram.head $(DIRMIP)/gram.dcl $(DIRMIP)/gram.expr \
	  $(DIRMIP)/gram.exec $(DIRMIP)/gram.io

all: 
	$(MAKE) -$(MAKEFLAGS)f f77.mk  \
		"DEFS = -DHERE=VAX -DTARGET=M68K -DFAMILY=PCC" \
		"MACH = m68k" "MACHX = m68kx" "MACHDEFS = m68kdefs" \
		"PUT = putp2" "FAMDEFS = pccdefs" "MACHDIR = $(DIRM68K)" \
		"FAMDIR = $(DIRPCC)" compiler


compiler:	f77 f77pass1 f77pass2

f77:	driver.o $(MACHX).o
	$(CC) driver.o $(MACHX).o -o f77 $(LDFLAGS)
	@size f77

driver.o:	$(DIRMIP)/drivedefs $(INCLUDES) $(DIRMIP)/driver.c
	$(CC) -c $(CFLAGS) $(FORTDEF) $(INCLIST) $(DIRMIP)/driver.c

$(MACHX).o:	$(MACHDIR)/$(MACHX).c $(INCLUDES) $(FAMDIR)/$(FAMDEFS)
	$(CC) -c $(CFLAGS) $(FORTDEF) $(INCLIST) $(MACHDIR)/$(MACHX).c

f77pass1:	$(OBJECTS1) 
	echo "f77pass1"
	@echo LOAD
	$(CC)  $(OBJECTS1) -o f77pass1 $(LDFLAGS)
	@size f77pass1

lint:
	make -f onepass.mk \
		"DEFS = -DHERE=VAX -DTARGET=VAX -DFAMILY=PCC -DFONEPASS" \
		"MACH = m68k" "MACHX = m68kx" "MACHDEFS = m68kdefs" \
		"PUT = putpcc" "FAMDEFS = pccdefs" "MACHDIR = $(DIRVAX)" \
		"FAMDIR = $(DIRPCC)" lintem

lintem:	machdefs gram.c
	lint $(CFLAGS) $(FORTDEF) -I$(M) -I$(P) -abhvxn $(SOURCE1) $(LDFLAGS)


main.o:	$(DIRMIP)/main.c $(INCLUDES)
	$(CC) -c $(CFLAGS)  $(FORTDEF) $(INCLIST) $(DIRMIP)/main.c

init.o: $(DIRMIP)/init.c $(INCLUDES) $M/mfile1.h $P/macdefs.h
	$(CC) -c $(CFLAGS)  $(FORTDEF) $(INCLIST) $(DIRMIP)/init.c

gram.o: gram.c $(INCLUDES)
	$(CC) -c $(CFLAGS) $(FORTDEF) $(INCLIST) gram.c

lex.o:	$(DIRMIP)/lex.c $(INCLUDES) tokdefs
	$(CC) -c $(CFLAGS) $(FORTDEF) $(INCLIST) $(DIRMIP)/lex.c

proc.o:	$(DIRMIP)/proc.c $(INCLUDES)
	$(CC) -c $(CFLAGS) $(FORTDEF) $(INCLIST) $(DIRMIP)/proc.c

equiv.o:	$(DIRMIP)/equiv.c $(INCLUDES)
	$(CC) -c $(CFLAGS) $(FORTDEF) $(INCLIST) $(DIRMIP)/equiv.c

data.o:	$(DIRMIP)/data.c $(INCLUDES)
	$(CC) -c $(CFLAGS) $(FORTDEF) $(INCLIST) $(DIRMIP)/data.c

expr.o:	$(DIRMIP)/expr.c $(INCLUDES)
	$(CC) -c $(CFLAGS) $(FORTDEF) $(INCLIST) $(DIRMIP)/expr.c

exec.o:	$(DIRMIP)/exec.c $(INCLUDES)
	$(CC) -c $(CFLAGS) $(FORTDEF) $(INCLIST) $(DIRMIP)/exec.c

intr.o:	$(DIRMIP)/intr.c $(INCLUDES)
	$(CC) -c $(CFLAGS) $(FORTDEF) $(INCLIST) $(DIRMIP)/intr.c

io.o:	$(DIRMIP)/io.c $(INCLUDES) $(DIRMIP)/fio.h
	$(CC) -c $(CFLAGS) $(FORTDEF) $(INCLIST) $(DIRMIP)/io.c

misc.o:	$(DIRMIP)/misc.c $(INCLUDES)
	$(CC) -c $(CFLAGS) $(FORTDEF) $(INCLIST) $(DIRMIP)/misc.c

error.o:	$(DIRMIP)/error.c $(INCLUDES)
	$(CC) -c $(CFLAGS) $(FORTDEF) $(INCLIST) $(DIRMIP)/error.c

put.o:	$(DIRMIP)/put.c $(INCLUDES)
	$(CC) -c $(CFLAGS) $(FORTDEF) $(INCLIST) $(DIRMIP)/put.c

$(PUT).o:	$(FAMDIR)/$(PUT).c $(INCLUDES) $(FAMDIR)/$(FAMDEFS) $M/mfile2.h
	$(CC) -c $(CFLAGS) $(FORTDEF) $(INCLIST) $(FAMDIR)/$(PUT).c

$(MACH).o:	$(MACHDIR)/$(MACH).c $(INCLUDES) $(FAMDIR)/$(FAMDEFS)
	$(CC) -c $(CFLAGS) $(FORTDEF)  $(INCLIST) $(MACHDIR)/$(MACH).c

ftrees.o:	$(FAMDIR)/ftrees.c $(INCLUDES) $(FAMDIR)/$(FAMDEFS) \
		$M/mfile1.h $M/common $M/manifest.h $M/mfile1.h
	$(CC) -c $(CFLAGS) $(FORTDEF) $(INCLIST) $(FAMDIR)/ftrees.c

nfort.o: $(DIRMIP)/fort.h $(DIRMIP)/nfort.c $M/mfile2.h
	$(CC) -c $(CFLAGS) $(FORTDEF) $(INCLIST) $(DIRMIP)/nfort.c

machdefs:	$(MACHDIR)/$(MACHDEFS)
	cp $(MACHDIR)/$(MACHDEFS) machdefs

gram.c:	$(GRAMMAR) tokdefs
	( sed < tokdefs "s/#define/%token/" ;\
		cat $(GRAMMAR) ) > gram.in
	$(YACC) $(YFLAGS) gram.in
	@echo "expect 4 shift/reduce"
	mv y.tab.c gram.c
	rm gram.in

tokdefs: $(DIRMIP)/tokens
	grep -n "^[^#]" < $(DIRMIP)/tokens | sed "s/\([^:]*\):\(.*\)/#define \2 \1/" > tokdefs


f77pass2:	$(OBJECTS2) 
	@echo LOAD
	$(CC)  $(OBJECTS2) -o f77pass2 $(LDFLAGS)
	@size f77pass1

sty:		$M/manifest.h $M/sty.y $P/macdefs.h $M/mfile2.h $M/common
		-@echo "Expect 3 reduce/reduce conflicts"
		$(YACC_CMD) $M/sty.y
	#
	# always use the resident compiler to generate sty.
	# whether generating a cross-compiler or a resident
	# on a non-target machine.
	#
		/bin/cc $(CFLAGS)  $(CFLAGS) $(SGS2INC) -o sty y.tab.c $M/shptbl.c
		rm y.tab.c


comm2.o:	$M/manifest.h $M/mfile2.h $(SDBINC)/sdb.h $M/common \
		$P/macdefs.h $M/comm2.c
		$(CC) -c  $(CFLAGS) $(SGS2DEFS) $(SGS2INC) $M/comm2.c

tablea.o:	$M/manifest.h $M/mfile2.h $P/macdefs.h tablea.c
		$(CC) -c $(CFLAGS) $(SGS2DEFS) $(SGS2INC) tablea.c

tableb.o:	tablea.o
		$(CC) -c $(CFLAGS) $(SGS2DEFS) $(SGS2INC) tableb.c

#tablea.c tableb.c:	sty $P/$(PROCESSOR)/stin
#		-@echo "Expect 2 \"... may be covered by ...\" messages"
#		./sty <$P/$(PROCESSOR)/stin

cost.o:		$M/manifest.h $M/mfile2.h $P/macdefs.h $M/cost.c
		$(CC) -c  $(CFLAGS) $(SGS2DEFS) $(SGS2INC) $M/cost.c

cgen.o:		$M/manifest.h $M/mfile2.h $P/macdefs.h $M/cgen.c
		$(CC) -c  $(CFLAGS) $(SGS2DEFS) $(SGS2INC) $M/cgen.c

reader.o:	$M/manifest.h $M/mfile2.h $P/macdefs.h $M/reader.c
		$(CC) -c  $(CFLAGS) $(SGS2DEFS) $(SGS2INC) $M/reader.c

local2.o:	$M/manifest.h $M/mfile2.h $P/macdefs.h $P/$(PROCESSOR)/local2.c
		$(CC) -c  $(CFLAGS) $(SGS2DEFS) $(SGS2INC) $P/$(PROCESSOR)/local2.c

match.o:	$M/manifest.h $M/mfile2.h $P/macdefs.h $M/match.c \
		$(SDBINC)/sdb.h
		$(CC) -c  $(CFLAGS) $(SGS2DEFS) $(SGS2INC) $M/match.c

allo.o:		$M/manifest.h $M/mfile2.h $P/macdefs.h $M/allo.c
		$(CC) -c  $(CFLAGS) $(SGS2DEFS) $(SGS2INC) $M/allo.c
 
xdefs.o:	$M/manifest.h $M/mfile1.h $(SDBINC)/sdb.h $P/macdefs.h $M/xdefs.c
		$(CC) -c  $(CFLAGS) $(SGS2DEFS) $(SGS2INC) $M/xdefs.c

#-------------------------
install: all
	$(INS) f77 $(BINDIR)/f77
	$(INS) f77pass1 $(LIBDIR)/f77pass1
	$(INS) f77pass2 $(LIBDIR)/f77pass2
	chmod 755 $(BINDIR)/f77 $(LIBDIR)/f77pass1  $(LIBDIR)/f77pass2 
	chgrp bin $(BINDIR)/f77 $(LIBDIR)/f77pass1  $(LIBDIR)/f77pass2 
	chown bin $(BINDIR)/f77 $(LIBDIR)/f77pass1  $(LIBDIR)/f77pass2 


clean:
	-rm -f gram.c *.o tokdefs machdefs

clobber:	clean
	-rm -f f77 f77pass1 f77pass2 sty
	-rm -f greplist tags
ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
