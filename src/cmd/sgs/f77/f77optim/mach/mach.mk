#	START NEW ARIX SCCS HEADER
#
#	@(#) mach.mk: version 25.1 created on 12/2/91 at 17:41:25
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)mach.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#ident	"@(#)f77/f77optim/mach:mach.mk	25.1"

#	MC68000 OPTIM MAKEFILE
#

include $(IROOT)/macdefs
include $(IROOT)/sgsdefs

OWN=	bin		#file owner
GRP=	bin		#file group
PROT=	755		#protections

BINDIR=	$(ROOT)/bin
LIBDIR=	$(ROOT)/lib
COMDIR= ../common

LINT=	lint
LFLAGS=	-p
#FPU= NOFPU
PROCESSOR=$(MACH)
FPU= M68881
DEFLIST=	-D$(PROCESSOR) -D$(FAMILY) -D$(FPU)
AWKLIST=	p=$(PROCESSOR) f=$(FAMILY)
INCLIST=	-I./$(FAMILY) -I. -I$(COMDIR)

CC_CMD=	$(CC) -c  $(DEFLIST) $(INCLIST) $(CFLAGS)


OFILES=		scan.o parse.o optab.o rand.o mipsup.o vtrace.o \
		optim.o inter.o
CFILES=		$(FAMILY)/scan.c $(FAMILY)/mipsup.c \
		$(FAMILY)/vtrace.c $(FAMILY)optab.c \
		parse.c rand.c \
		$(COMDIR)/optim.c $(COMDIR)/inter.c
OP_FILES=	$(FAMILY)/ops.tail $(FAMILY)/optab.tail $(FAMILY)/ops.h

OPTIM_H=	$(COMDIR)/optim.h defs $(FAMILY)/rand.h $(FAMILY)/ops.h

all build:	optim

#-------------------------
optim:	$(OFILES)
		$(CC) $(CFLAGS)  -o optim $(OFILES) $(LDFLAGS)

optim.o:	$(OPTIM_H) $(COMDIR)/optim.c debug.h
		$(CC_CMD) $(COMDIR)/optim.c

inter.o:	$(OPTIM_H) $(COMDIR)/inter.c debug.h
		$(CC_CMD) $(COMDIR)/inter.c

scan.o:		$(OPTIM_H) $(FAMILY)/scan.c debug.h
		$(CC_CMD) $(FAMILY)/scan.c

parse.o:	$(OPTIM_H) parse.c debug.h
		$(CC_CMD) parse.c

rand.o:		$(OPTIM_H) rand.c debug.h
		$(CC_CMD) rand.c

mipsup.o:	$(OPTIM_H) $(FAMILY)/mipsup.c debug.h
		$(CC_CMD) $(FAMILY)/mipsup.c

$(OP_FILES):	$(FAMILY)/optab.base optab.awk
		cd $(FAMILY); awk -f ../optab.awk $(AWKLIST) optab.base \
				>optab.tail;
		#
		# NOTE: awk also builds a new "$(FAMILY)/ops.tail" file.
		#
		cat ops.head $(FAMILY)/ops.tail >$(FAMILY)/ops.h
		touch $(OP_FILES)

$(FAMILY)/optab.c:	optab.head $(FAMILY)/optab.base optab.awk $(OP_FILES) debug.h
		cat optab.head $(FAMILY)/optab.tail >$(FAMILY)/optab.c

optab.o:	$(FAMILY)/optab.c $(FAMILY)/ops.h debug.h
		$(CC_CMD) $(FAMILY)/optab.c

vtrace.o:	$(FAMILY)/vtrace.c $(FAMILY)/peep.c $(OPTIM_H) debug.h
		$(CC_CMD) $(FAMILY)/vtrace.c 

#-------------------------

install:	all
		-rm -f $(LIBDIR)/f77optim
		$(INS) optim $(LIBDIR)/f77optim
		chmod $(PROT) $(LIBDIR)/f77optim
		chgrp $(GRP) $(LIBDIR)/f77optim
		chown $(OWN) $(LIBDIR)/f77optim

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
	-rm -f $(OFILES) $(OP_FILES)

#--------------------------

clobber:	clean
		-rm -f $(FAMILY)/optab.c optim */ops.h
		-rm -f greplist tags

#--------------------------

lint:	$(CFILES) $(FAMILY)/peep.c
	$(LINT) $(LFLAGS) $(INCLIST) $(CFILES) >lint.out

#--------------------------

cxref:	$(CFILES) $(FAMILY)/peep.c
	cxref -c $(INCLIST) $(CFILES) -o optim.xref

#--------------------------

cflow:	$(CFILES) $(FAMILY)/peep.c
	cflow -r $(INCLIST) $(CFILES) >optim.cflow
SOURCES= $(CFILES)
ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
