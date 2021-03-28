#ident	"@(#)mach.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright 1985 Motorola Inc.
#	MAKEFILE for intermediate optimizer
#
#
SROOT =
IROOT =
include $(IROOT)/macdefs
include $(IROOT)/sgsdefs

# POSIX
SVID_FLAG=

#
#
LIBDIR= $(ROOT)/lib
PROT = 755
BASE = $(PWD)/../..
COMP2=	$(BASE)/comp2
COMS = $(PWD)/../common

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

CC_CMD= cd $(OUT); $(CC) -c $(INCLIST) $(CFLAGS) $(TWOPASS) $(HLOPTIM) 

# (D for Machine dependent part)
# (P for Processor dependent part)
M=      $(COMS)
D=	$(PWD)
O=$(PWD)/$(OUT)

HLOPTIM_OFILES= $O/addvalue.o $O/adjframe.o $O/bb.o $O/bitset.o $O/gather.o\
	$O/hash.o $O/hloptim.o $O/localopt.o $O/main.o $O/misc.o\
	$O/reader.o $O/regasg.o $O/symtab.o $O/storage.o $O/write.o

HLOPTIM_CFILES= $M/addvalue.c $M/adjframe.c $M/bb.c $M/bitset.c $M/gather.c\
	$M/hash.c $M/hloptim.c $D/localopt.c $M/main.c $M/misc.c\
	$(COMP2)/common/reader.c $M/regasg.c $M/symtab.c $D/storage.c $M/write.c

HLOPTIM_H= $(COMP2)/common/manifest.h $M/symtab.h \
	$(COMP2)/mach/macdefs.h $M/hash.h $M/declar.h

all:  $O/coptim
#----------------------- FUNCTION PROTOTYPING -------------
.SUFFIXES:	 .p
.c.p:
		$(CC_CMD) -G $<
		mv $(OUT)/$(*F).p $(<D)

HLOPTIM_PFILES= $M/addvalue.p $M/adjframe.p $M/bb.p $M/bitset.p $M/gather.p\
	$M/hash.p $M/hloptim.p $D/localopt.p $M/main.p $M/misc.p\
	$(COMP2)/common/reader.p $M/regasg.p $M/symtab.p $D/storage.p $M/write.p

protos:		$(HLOPTIM_PFILES)
			cat $(HLOPTIM_PFILES) > $M/protos.h
#------------------------------------------------------------

$O/coptim:	$(HLOPTIM_OFILES)
		$(CC) $(CFLAGS) $(LDFLAGS) -o $O/coptim \
		$(HLOPTIM_OFILES) $(LIBV) $(LDLIBS)
# COPTIM has been built

$O/addvalue.o:     $M/addvalue.c $(HLOPTIM_H)
		$(CC_CMD) $M/addvalue.c

$O/adjframe.o:     $M/adjframe.c $(HLOPTIM_H)
		$(CC_CMD) $M/adjframe.c

$O/bb.o:           $M/bb.c $(HLOPTIM_H)
		$(CC_CMD) $M/bb.c

$O/bitset.o:       $M/bitset.h $M/bitset.c
		$(CC_CMD) $M/bitset.c

$O/gather.o:       $M/gather.c $(HLOPTIM_H)
		$(CC_CMD) $M/gather.c

$O/hash.o:         $M/hash.c $(HLOPTIM_H)
		$(CC_CMD) -DSTART $M/hash.c

$O/hloptim.o:      $M/hloptim.c $(HLOPTIM_H)
		$(CC_CMD) $M/hloptim.c

$O/localopt.o:	$D/localopt.c $(HLOPTIM_H)
		$(CC_CMD) $D/localopt.c

$O/main.o:         $M/main.c $(HLOPTIM_H)
		$(CC_CMD) $M/main.c

$O/misc.o:         $M/misc.c $(HLOPTIM_H)
		$(CC_CMD) $M/misc.c

$O/regasg.o:       $M/regasg.c $(HLOPTIM_H)
		$(CC_CMD) $M/regasg.c

$O/reader.o:       $(COMP2)/common/manifest.h $(COMP2)/common/mfile2.h \
				$(COMP2)/mach/macdefs.h $(COMP2)/common/reader.c $M/hash.h
		$(CC_CMD) $(COMP2)/common/reader.c

$O/storage.o:	$D/storage.c $(HLOPTIM_H)
		$(CC_CMD) $D/storage.c

$O/symtab.o:       $M/symtab.c $(HLOPTIM_H)
		$(CC_CMD) $M/symtab.c

$O/write.o:        $M/write.c $(HLOPTIM_H)
		$(CC_CMD) $M/write.c

#-------------------------

install: $O/coptim
	-rm -f $(LIBDIR)/$(SGS)coptim
	$(INS) $O/coptim $(LIBDIR)/$(SGS)coptim
	chmod 755 $(LIBDIR)/$(SGS)coptim
	chgrp bin $(LIBDIR)/$(SGS)coptim
	chown bin $(LIBDIR)/$(SGS)coptim

#-------------------------

clean clobber:
		-rm -f $O/coptim $(HLOPTIM_OFILES)

shrink:	clean

