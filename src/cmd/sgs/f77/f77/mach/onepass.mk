#	START NEW ARIX SCCS HEADER
#
#	@(#) onepass.mk: version 25.1 created on 12/2/91 at 17:38:51
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)onepass.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#		Copyright (c) 1985 AT&T		#
#		All Rights Reserved		#
#	onepass.mk	7.1	
#
# Makefile for four flavors of resident f77 compilers:
#	vax using pcc/f1
#	3b using pcc/f1
#	pdp11 using pcc/f1
#	pdp11 using dmr/c1
#
#	for vax "make vax"
#	for 3b "make 3b"
#	for pdp11/pcc "make pcc11"
#	for pdp11/dmr "make dmr11"
#
# This makefile will call itself recursively with appropriate parameters
# for the desired flavor of f77 compiler. 

#ident	"@(#)f77/f77/mach:onepass.mk	25.1"

# POSIX

SVID_FLAG =

DEFS =
FORTDEF = -DFONEPASS -DFORT
M = /usr/src/cmd/cc/comp2/common
P = /usr/src/cmd/cc/comp2/m68k
DIRBUF = /crp/c/cmf
DIRNUC = /crp/c/cmf
DIRMIP = .
DIRPCC = .
DIRVAX = .
DIR3B = ./pcc/3b
DIRPCC11 = ./pcc/pdp11
DIRDMR = ./dmr
CPFLAGS = $(FORTDEF) $(DEFS) -I$(DIRMIP) -I. -I$(MACHDIR) -I$(FAMDIR)
OBJECTS = main.o init.o gram.o lex.o proc.o equiv.o data.o \
	  expr.o exec.o intr.o io.o misc.o error.o put.o \
	  $(PUT).o $(MACH).o $(MACHX).o ftrees.o
SOURCE = $(DIRMIP)/main.c $(DIRMIP)/init.c gram.c $(DIRMIP)/lex.c \
	  $(DIRMIP)/proc.c $(DIRMIP)/equiv.c $(DIRMIP)/data.c \
	  $(DIRMIP)/expr.c $(DIRMIP)/exec.c $(DIRMIP)/intr.c $(DIRMIP)/io.c \
	  $(DIRMIP)/misc.c $(DIRMIP)/error.c $(DIRMIP)/put.c \
	  $(FAMDIR)/$(PUT).c $(MACHDIR)/$(MACH).c $(MACHDIR)/$(MACHX).c
INCLUDES = $(DIRMIP)/defs $(DIRMIP)/defines machdefs $(DIRMIP)/ftypes
GRAMMAR = $(DIRMIP)/gram.head $(DIRMIP)/gram.dcl $(DIRMIP)/gram.expr \
	  $(DIRMIP)/gram.exec $(DIRMIP)/gram.io

all:
	echo "Please specify flavor: vax 3b pcc11 dmr11 m68k"
	exit 1

m68k:
	make -f onepass.mk\
		"DEFS = -DHERE=VAX -DTARGET=M68K -DFAMILY=PCC -DFONEPASS\
		 -DSGS2 -DSTKCK" \
		"MACH = m68k" "MACHX = m68kx" "MACHDEFS = m68kdefs" \
		"PUT = putp2" "FAMDEFS = pccdefs" "MACHDIR = $(DIRVAX)" \
		"FAMDIR = $(DIRPCC)" compiler

vax:
	make -f onepass.mk\
		"DEFS = -DHERE=VAX -DTARGET=VAX -DFAMILY=PCC -DFONEPASS" \
		"MACH = vax" "MACHX = vaxx" "MACHDEFS = vaxdefs" \
		"PUT = putp2" "FAMDEFS = pccdefs" "MACHDIR = $(DIRVAX)" \
		"FAMDIR = $(DIRPCC)" compiler

3b:
	make -f onepass.mk\
		"DEFS = -DHERE=VAX -DTARGET=X3B -DFAMILY=PCC" \
		"MACH = 3b" "MACHX = 3bx" "MACHDEFS = 3bdefs" \
		"PUT = putpcc" "FAMDEFS = pccdefs" "MACHDIR = $(DIR3B)" \
		"FAMDIR = $(DIRPCC)" 3bxcompiler

pcc11:
	make -f onepass.mk\
		"DEFS = -DHERE=PDP11 -DTARGET=PDP11 -DFAMILY=PCC" \
		"MACH = pdp11" "MACHX = pdp11x" "MACHDEFS = pdp11defs" \
		"PUT = putpcc" "FAMDEFS = pccdefs" "MACHDIR = $(DIRPCC11)" \
		"FAMDIR = $(DIRPCC)" "LDFLAGS = -i" compiler

dmr11:
	make -f onepass.mk\
		"DEFS = -DHERE=PDP11 -DTARGET=PDP11 -DFAMILY=DMR -DNEWDMR" \
		"MACH = pdp11" "MACHX = pdp11x" "MACHDEFS = pdp11defs" \
		"PUT = putdmr" "FAMDEFS = dmrdefs" "MACHDIR = $(DIRDMR)" \
		"FAMDIR = $(DIRDMR)" "LDFLAGS = -i" compiler

compiler:	f77 f77comp

3bxcompiler:	3bxf77 3bxf77comp

f77:	driver.o $(DIRMIP)/$(MACHX).o
	$(CC)  driver.o $(MACHX).o -o f77 $(LDFLAGS)
	@size f77

fcost.o: $(M)/cost.c
	$(CC) -c $(CFLAGS) $(FORTDEF) -I$(M) -I$(P) $(DIRMIP)/fcost.c

fxdefs.o: $(M)/xdefs.c
	$(CC) -c $(CFLAGS) $(FORTDEF) -I$(M) -I$(P) $(DIRMIP)/fxdefs.c

fcgen.o: $(M)/cgen.c
	$(CC) -c $(CFLAGS) $(FORTDEF) -I$(M) -I$(P) $(DIRMIP)/fcgen.c

nfort.o: $(DIRMIP)/fort.h $(DIRMIP)/nfort.c
	$(CC) -c $(CFLAGS) $(FORTDEF) -I$(M) -I$(P) $(DIRMIP)/nfort.c

freader.o: $(M)/reader.c
	$(CC) -c -O $(CFLAGS) $(FORTDEF) -I$(M) -I$(P) $(DIRMIP)/freader.c

fallo.o: $(M)/allo.c
	$(CC) -c -O $(CFLAGS) $(FORTDEF) -I$(M) -I$(P) $(DIRMIP)/fallo.c

fmatch.o: $(M)/match.c
	$(CC) -c -O $(CFLAGS) $(FORTDEF) -I$(M) -I$(P) $(DIRMIP)/fmatch.c

ftable.o: $(DIRMIP)/table.c
	$(CC) -S $(CFLAGS) $(FORTDEF) -I. -I$(M) -I$(P) $(DIRMIP)/ftable.c
	as -o ftable.o ftable.s
	rm -f ftable.s

flocal2.o: $(P)/$(PROCESSOR)/local2.c
	$(CC) -c -O $(CFLAGS) $(FORTDEF) -I$(M) -I$(P) -I$(P)/$(PROCESSOR) $(DIRMIP)/flocal2.c

fcomm2.o: $(M)/common
	$(CC) -c -O $(CFLAGS) $(FORTDEF) -I$(M) -I$(P) $(DIRMIP)/fcomm2.c

f77comp:	$(OBJECTS) nfort.o freader.o fallo.o fmatch.o ftable.o \
		flocal2.o fcomm2.o fcgen.o fxdefs.o fcost.o
	@echo LOAD
	$(CC) -O $(OBJECTS) nfort.o freader.o fallo.o fmatch.o \
	ftable.o flocal2.o fcomm2.o fcgen.o fxdefs.o fcost.o -o f77comp \
	$(LDFLAGS)
	@size f77comp 

lint:
	make -f onepass.mk \
		"DEFS = -DHERE=VAX -DTARGET=VAX -DFAMILY=PCC -DFONEPASS" \
		"MACH = m68k" "MACHX = m68kx" "MACHDEFS = m68kdefs" \
		"PUT = putpcc" "FAMDEFS = pccdefs" "MACHDIR = $(DIRVAX)" \
		"FAMDIR = $(DIRPCC)" lintem

lintem:	machdefs gram.c
	lint $(CFLAGS)  -I$(M) -I$(P) -abhvxn  \
	$(SOURCE) $(DIRMIP)/nfort.c $(DIRMIP)/freader.c $(DIRMIP)/fallo.c \
	$(DIRMIP)/fmatch.c $(DIRMIP)/ftable.c $(DIRMIP)/flocal2.c \
	$(DIRMIP)/fcomm2.c $(LDFLAGS)

3bxf77:	3bxdriver.o $(MACHX).o
	$(CC) driver.o $(MACHX).o -o 3bxf77

3bxf77comp:	$(OBJECTS)
	@echo LOAD
	@$(CC)  $(OBJECTS) -o 3bxf77comp $(LDFLAGS)
	@size 3bxf77comp

3bxdriver.o:	$(DIRMIP)/3bdrivedefs $(INCLUDES)
	mv $(DIRMIP)/drivedefs $(DIRMIP)/tmpdrivedefs
	cp $(DIRMIP)/3bdrivedefs $(DIRMIP)/drivedefs
	$(CC) -c $(CFLAGS) $(DIRMIP)/driver.c
	mv $(DIRMIP)/tmpdrivedefs $(DIRMIP)/drivedefs
	touch driver.o 3bxdriver.o

driver.o:	$(DIRMIP)/drivedefs $(INCLUDES) $(DIRMIP)/driver.c
	$(CC) -c $(CFLAGS) $(DIRMIP)/driver.c

main.o:	$(DIRMIP)/main.c $(INCLUDES)
	$(CC) -c $(CFLAGS) $(DIRMIP)/main.c

init.o: $(DIRMIP)/init.c $(INCLUDES)
	$(CC) -c $(CFLAGS) -I$(M) -I$(P) $(DIRMIP)/init.c

gram.o: gram.c $(INCLUDES)
	$(CC) -c $(CFLAGS) gram.c

lex.o:	$(DIRMIP)/lex.c $(INCLUDES) tokdefs
	$(CC) -c $(CFLAGS) $(DIRMIP)/lex.c

proc.o:	$(DIRMIP)/proc.c $(INCLUDES)
	$(CC) -c $(CFLAGS) $(DIRMIP)/proc.c

equiv.o:	$(DIRMIP)/equiv.c $(INCLUDES)
	$(CC) -c $(CFLAGS) $(DIRMIP)/equiv.c

data.o:	$(DIRMIP)/data.c $(INCLUDES)
	$(CC) -c $(CFLAGS) $(DIRMIP)/data.c

expr.o:	$(DIRMIP)/expr.c $(INCLUDES)
	$(CC) -c $(CFLAGS) $(DIRMIP)/expr.c

exec.o:	$(DIRMIP)/exec.c $(INCLUDES)
	$(CC) -c $(CFLAGS) $(DIRMIP)/exec.c

intr.o:	$(DIRMIP)/intr.c $(INCLUDES)
	$(CC) -c $(CFLAGS) $(DIRMIP)/intr.c

io.o:	$(DIRMIP)/io.c $(INCLUDES) $(DIRMIP)/fio.h
	$(CC) -c $(CFLAGS) $(DIRMIP)/io.c

misc.o:	$(DIRMIP)/misc.c $(INCLUDES)
	$(CC) -c $(CFLAGS) $(DIRMIP)/misc.c

error.o:	$(DIRMIP)/error.c $(INCLUDES)
	$(CC) -c $(CFLAGS) $(DIRMIP)/error.c

put.o:	$(DIRMIP)/put.c $(INCLUDES)
	$(CC) -c $(CFLAGS) $(DIRMIP)/put.c

$(PUT).o:	$(FAMDIR)/$(PUT).c $(INCLUDES) $(FAMDIR)/$(FAMDEFS)
	$(CC) -c $(CFLAGS) -I$(M) -I$(P) $(FAMDIR)/$(PUT).c

$(MACH).o:	$(MACHDIR)/$(MACH).c $(INCLUDES) $(FAMDIR)/$(FAMDEFS)
	$(CC) -c $(CFLAGS)  -I$(M) -I$(P) $(MACHDIR)/$(MACH).c

$(MACHX).o:	$(MACHDIR)/$(MACHX).c $(INCLUDES) $(FAMDIR)/$(FAMDEFS)
	$(CC) -c $(CFLAGS)  -I$(M) -I$(P) $(MACHDIR)/$(MACHX).c

ftrees.o:	$(FAMDIR)/ftrees.c $(INCLUDES) $(FAMDIR)/$(FAMDEFS)
	$(CC) -c $(CFLAGS) -I$(M) -I$(P) $(FAMDIR)/ftrees.c

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

install:
	@if [ `ls f77 f77comp | wc -l` != 2 ] ; then echo Make compiler flavor first ; exit 1 ; else exit 0 ; fi
	@strip f77
	@strip f77comp
	/etc/install -n /usr/bin f77
	/etc/install -n /usr/lib f77comp

clean:
	-rm -f gram.c *.o tokdefs machdefs

clobber:	clean
	-rm -f f77 f77comp
