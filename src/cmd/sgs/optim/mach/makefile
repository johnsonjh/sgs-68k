#ident	"@(#)makefile	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	@(#)makefile	7.2	
#	MC68000 OPTIM MAKEFILE
#
#	@(#) makefile:	2.1 83/07/08
#
SROOT =
IROOT =
include $(IROOT)/macdefs
#
OWN=	bin		#file owner
GRP=	bin		#file group
PROT=	775		#protections

BASE = ../../..
COMS= ../common
COMP2= $(BASE)/comp2/mach

AWKLIST=	p=$(MACH)

CC_CMD=	$(CC) -c $(CFLAGS)


OFILES=		scan.o parse.o optab.o rand.o mipsup.o vtrace.o \
		optim.o inter.o motopt.o motopt2.o rmframe.o

CFILES=		$(MACH)/scan.c $(MACH)/mipsup.c \
		$(MACH)/vtrace.c $(MACH)optab.c \
		parse.c rand.c motopt.c motopt2.c rmframe.c \
		$(COMS)/optim.c $(COMS)/inter.c

OP_FILES=	$(MACH)/ops.tail $(MACH)/optab.tail $(MACH)/ops.h

OPTIM_H=        $(COMS)/optim.h defs $(MACH)/rand.h $(MACH)/ops.h

all:  optim

#-------------------------
optim:	$(OFILES)
		$(CC) $(CFLAGS) $(LDFLAGS) -o optim $(OFILES) $(LIBV) $(LDLIBS)

optim.o:        $(OPTIM_H) $(COMS)/optim.c
		$(CC_CMD) $(COMS)/optim.c

inter.o:        $(OPTIM_H) $(COMS)/inter.c
		$(CC_CMD) $(COMS)/inter.c

scan.o:         $(OPTIM_H) $(MACH)/scan.c $(OP_FILES)
		$(CC_CMD) $(MACH)/scan.c

motopt.o:        $(OPTIM_H) motopt.c $(COMP2)/macdefs.h
		$(CC_CMD) -I$(COMP2) motopt.c

motopt2.o:        $(OPTIM_H) motopt2.c $(COMP2)/macdefs.h
		$(CC_CMD) -I$(COMP2) motopt2.c

rmframe.o:        $(OPTIM_H) rmframe.c $(COMP2)/macdefs.h
		$(CC_CMD) -I$(COMP2) rmframe.c

parse.o:        $(OPTIM_H) parse.c $(COMP2)/macdefs.h
		$(CC_CMD) -I$(COMP2) parse.c

rand.o:		$(OPTIM_H) rand.c
		$(CC_CMD) rand.c

mipsup.o:	$(OPTIM_H) $(MACH)/mipsup.c
		$(CC_CMD) $(MACH)/mipsup.c

$(OP_FILES):	$(MACH)/optab.base optab.awk
		cd $(MACH); awk -f ../optab.awk $(AWKLIST) optab.base \
				>optab.tail;
#
# NOTE: awk also builds a new "$(MACH)/ops.tail" file.
#
		cat ops.head $(MACH)/ops.tail >$(MACH)/ops.h
		touch $(OP_FILES)

$(MACH)/optab.c:	optab.head $(MACH)/optab.base optab.awk $(OP_FILES)
		cat optab.head $(MACH)/optab.tail >$(MACH)/optab.c

optab.o:	$(MACH)/optab.c $(MACH)/ops.h
		$(CC_CMD) $(MACH)/optab.c

vtrace.o:	$(MACH)/vtrace.c $(MACH)/peep.c $(OPTIM_H)
		$(CC_CMD) $(MACH)/vtrace.c 

#-------------------------

install:	optim
	-rm -f $(LIBDIR)/optim
	sh $(BASE)/sgs.install $(PROT) $(OWN) $(GRP) $(LIBDIR)/optim optim
	strip $(LIBDIR)/optim

#--------------------------

clean:
	-rm -f $(OFILES) $(OP_FILES) $(MACH)/optab.c optim */ops.h

shrink: clean
