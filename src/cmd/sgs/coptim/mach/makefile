#ident	"@(#)makefile	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	@(#)makefile	7.1	
#	Copyright 1985 Motorola Inc.
#	MAKEFILE for intermediate optimizer
#
#
SROOT =
IROOT =
include $(IROOT)/macdefs
#
PROT = 755
BASE = ../../..
COMP2=	$(BASE)/comp2
COMS = ../common

#       PROCESSOR must be one of M68000, M68010, M68020, or M68040
PROCESSOR = $(MACH)

INCLIST=        -I$(COMP2)/common -I$(COMP2)/mach

#
#	conditional compilation variables are declared 
#	in the DEFLIST and are passed to cpp.
#
#	variables:
#
#               DFLTDBLALIGN - default environment variable DBLALIGN to YES
#                         instead of NO.  This must match compilers Make.
#		FORCE881 - Always assume '881 registers available.
#			  (Same as always seeing FP=M68881 environment set.)
#		TWOPASS - causes inclusion of code when defined
#			  to generate two pass compiler.  this "#undef"s
#			  ONEPASS in macdefs.h.  if not defined, the code 
#			  for a one pass compiler is used.
#
#		ITREES  - Use icon based passing to pass all information
#			  to the back end.
#
#		HLOPTIM   - cause inclusion/exclusion of code specific to the
#			  high level optimizer. Should be set to -DHLOPTIM
#			  for the optimizer.
#
#  NOTE: coptim requires that all three of the above parameters be set

TWOPASS=	-DTWOPASS -DITREES 
HLOPTIM=	-DHLOPTIM

CC_CMD= $(CC) -c $(CFLAGS) $(INCLIST) $(TWOPASS) $(HLOPTIM)


HLOPTIM_OFILES= addvalue.o adjframe.o bb.o bitset.o gather.o\
	hash.o hloptim.o localopt.o main.o misc.o\
	reader.o regasg.o symtab.o storage.o write.o

HLOPTIM_CFILES= $(COMS)/addvalue.c $(COMS)/adjframe.c $(COMS)/bb.c $(COMS)/bitset.c $(COMS)/gather.c\
	$(COMS)/hash.c $(COMS)/hloptim.c localopt.c $(COMS)/main.c $(COMS)/misc.c\
	$(COMP2)/common/reader.c $(COMS)/regasg.c $(COMS)/symtab.c storage.c $(COMS)/write.c

HLOPTIM_H= $(COMP2)/common/manifest.h $(COMS)/symtab.h \
	$(COMP2)/mach/macdefs.h $(COMS)/hash.h $(COMS)/declar.h

all:  coptim

#-------------------------

coptim:	$(HLOPTIM_OFILES)
		$(CC) $(CFLAGS) $(LDFLAGS) -o coptim $(HLOPTIM_OFILES) $(LIBV) $(LDLIBS)
# COPTIM has been built

addvalue.o:     $(COMS)/addvalue.c $(HLOPTIM_H)
		$(CC_CMD) $(COMS)/addvalue.c

adjframe.o:     $(COMS)/adjframe.c $(HLOPTIM_H)
		$(CC_CMD) $(COMS)/adjframe.c

bb.o:           $(COMS)/bb.c $(HLOPTIM_H)
		$(CC_CMD) $(COMS)/bb.c

bitset.o:       $(COMS)/bitset.h $(COMS)/bitset.c
		$(CC_CMD) $(COMS)/bitset.c

gather.o:       $(COMS)/gather.c $(HLOPTIM_H)
		$(CC_CMD) $(COMS)/gather.c

hash.o:         $(COMS)/hash.c $(HLOPTIM_H)
		$(CC_CMD) -DSTART $(COMS)/hash.c

hloptim.o:      $(COMS)/hloptim.c $(HLOPTIM_H)
		$(CC_CMD) $(COMS)/hloptim.c

localopt.o:	localopt.c $(HLOPTIM_H)
		$(CC_CMD) localopt.c

main.o:         $(COMS)/main.c $(HLOPTIM_H)
		$(CC_CMD) $(COMS)/main.c

misc.o:         $(COMS)/misc.c $(HLOPTIM_H)
		$(CC_CMD) $(COMS)/misc.c

regasg.o:       $(COMS)/regasg.c $(HLOPTIM_H)
		$(CC_CMD) $(COMS)/regasg.c

reader.o:       $(COMP2)/common/manifest.h $(COMP2)/common/mfile2.h $(COMP2)/mach/macdefs.h $(COMP2)/common/reader.c $(COMS)/hash.h
		$(CC_CMD) -DFLEXNAMES $(COMP2)/common/reader.c

storage.o:	storage.c $(HLOPTIM_H)
		$(CC_CMD) storage.c

symtab.o:       $(COMS)/symtab.c $(HLOPTIM_H)
		$(CC_CMD) $(COMS)/symtab.c

write.o:        $(COMS)/write.c $(HLOPTIM_H)
		$(CC_CMD) $(COMS)/write.c

#-------------------------

install: coptim
	-rm -f $(LIBDIR)/coptim
	sh $(BASE)/sgs.install $(PROT) $(OWN) $(GRP) $(LIBDIR)/coptim coptim
	strip $(LIBDIR)/coptim

#-------------------------

clean:
		-rm -f coptim $(HLOPTIM_OFILES)

shrink:	clean

