#	START NEW ARIX SCCS HEADER
#
#	@(#) makefile: version 25.1 created on 12/2/91 at 17:40:53
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)makefile	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#
#	MOT:makefile	7.1		9/18/85

#ident	"@(#)f77/f77optim/mach/m68k:makefile	25.1"
IROOT =
include $(IROOT)/macdefs

#	                    
#	MC68000 OPTIM MAKEFILE
#
OWN=	bin		#file owner
GRP=	bin		#file group
PROT=	775		#protections

BINDIR=	$(ROOT)/bin
LIBDIR=	$(ROOT)/lib
COMDIR= ../common

LINT=	lint
LFLAGS=	-p
DEFLIST= -D$(MACH)
INCLIST=	-I. -I$(COMDIR)

CC_CMD=	$(CC) -c $(CFLAGS) $(DEFLIST) $(INCLIST)

AWKLIST= p=$(MACH) f=$(FAMILY)

OFILES=		scan.o parse.o optab.o rand.o mipsup.o vtrace.o \
		optim.o inter.o
CFILES=		scan.c parse.c optab.c rand.c mipsup.c vtrace.c \
		$(COMDIR)/optim.c $(COMDIR)/inter.c

OPTIM_H=	$(COMDIR)/optim.h defs rand.h ops.h

build:	optim

#-------------------------
CTRACE=/work/daves/ctrace/ctrace
otrace:
		$(CTRACE) $(DEFLIST) -f sdisp -P scan.c >tr.scan.c
		$(CC_CMD) tr.scan.c
		rm scan.o
		mv tr.scan.o scan.o
		$(CC) $(CFLAGS) $(LDFLAGS) -o optimtrace $(OFILES)
#-------------------------
optim:	$(OFILES)
		$(CC) $(CFLAGS) $(LDFLAGS) -o optim $(OFILES)

optim.o:	$(OPTIM_H) $(COMDIR)/optim.c
		$(CC_CMD) $(COMDIR)/optim.c

inter.o:	$(OPTIM_H) $(COMDIR)/inter.c
		$(CC_CMD) $(COMDIR)/inter.c

scan.o:		$(OPTIM_H) scan.c
		$(CC_CMD) scan.c

parse.o:	$(OPTIM_H) parse.c
		$(CC_CMD) parse.c

rand.o:		$(OPTIM_H) rand.c
		$(CC_CMD) rand.c

mipsup.o:	$(OPTIM_H) mipsup.c
		$(CC_CMD) mipsup.c

ops.h:		../m68k/ops.head ops.tail
		cat ../m68k/ops.head ops.tail >ops.h

ops.tail:	optab.base ../m68k/optab.awk
		awk  -f ../m68k/optab.awk $(AWKLIST) optab.base >optab.tail
		#
		# NOTE: awk also builds a new "ops.tail" file, as well.
		#

optab.tail:	optab.base ../m68k/optab.awk
		awk -f ../m68k/optab.awk $(AWKLIST) optab.base >optab.tail
		#
		# NOTE: awk also builds a new "ops.tail" file, as well.
		#

optab.o:	optab.c ops.h
		$(CC_CMD) optab.c

optab.c:	../m68k/optab.head optab.tail
		cat    ../m68k/optab.head optab.tail >optab.c

vtrace.o:	vtrace.c peep.c $(OPTIM_H)
		$(CC_CMD) vtrace.c

#-------------------------

install:	optim
		-rm -f $(LIBDIR)/optim
		cp optim $(LIBDIR)
		strip $(LIBDIR)/optim
		chmod $(PROT) $(LIBDIR)/optim
		chgrp $(GRP) $(LIBDIR)/optim
		chown $(OWN) $(LIBDIR)/optim

#--------------------------

save:	$(LIBDIR)/optim
	-rm -f $(LIBDIR)/optim.back
	cp $(LIBDIR)/optim $(LIBDIR)/optim.back

#--------------------------

uninstall:	$(LIBDIR)/optim.back
		-rm -f $(LIBDIR)/optim
		cp $(LIBDIR)/optim.back $(LIBDIR)/optim

#--------------------------

clean:
	-rm -f $(OFILES)

#--------------------------

clobber:	clean
		-rm -f optab.c optim
		-rm -f greplist tags

#--------------------------

lint:	$(CFILES)
	$(LINT) $(LFLAGS) $(INCLIST) $(CFILES) >lint.out

#--------------------------

cxref:	$(CFILES)
	cxref -c -I../common -I. $(CFILES) -o optim.xref

#--------------------------

cflow:	$(CFILES)
	cflow -r -I../common -I. $(CFILES) >optim.cflow
SOURCES = $(CFILES)
ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
