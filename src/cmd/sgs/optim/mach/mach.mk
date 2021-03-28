#ident	"@(#)mach.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	MC68000 OPTIM MAKEFILE
#
#	@(#) makefile:	2.1 83/07/08
#
SROOT =
IROOT =
include $(IROOT)/macdefs
include $(IROOT)/sgsdefs

# POSIX
SVID_FLAG=

#
LIBDIR = $(ROOT)/lib
OWN=	bin		#file owner
GRP=	bin		#file group
PROT=	775		#protections

BASE = $(PWD)/../..
COMS=  $(PWD)/../common
COMP2= $(BASE)/comp2/mach

AWKLIST=	p=$(MACH) f=$(FAMILY)

CC_CMD=	cd $(OUT); $(CC) -c $(CFLAGS)

# (M for Machine independent part)
# (D for Machine dependent part)
# (P for Processor dependent part)
M=      $(COMS)
D=	$(PWD)
F=	$(PWD)/$(FAMILY)
O=$(PWD)/$(OUT)

OFILES=		$O/scan.o $O/parse.o $O/optab.o $O/rand.o $O/mipsup.o $O/vtrace.o \
		$O/optim.o $O/inter.o $O/motopt.o $O/motopt2.o $O/rmframe.o

CFILES=		$F/scan.c $F/mipsup.c \
		$F/vtrace.c $F/optab.c \
		$D/parse.c $D/rand.c $D/motopt.c $D/motopt2.c $D/rmframe.c \
		$M/optim.c $M/inter.c

OP_FILES=	$D/ops.tail $F/optab.tail $F/ops.h

OPTIM_H=        $M/optim.h $D/defs $F/rand.h $F/ops.h

all: $O/optim2  clean $O/optim4
	rm -f $O/optim
	if [ "$(MACH)" = "M68020" ];\
	then \
	   ln $O/optim2 $O/optim; \
	else \
	   ln $O/optim4 $O/optim; \
	fi  

#----------------------- FUNCTION PROTOTYPING -------------
.SUFFIXES:	 .p[
.c.p:
		$(CC_CMD) -I$(COMP2) -G $<
		mv $(OUT)/$(*F).p $(<D)

PFILES=		$F/scan.p $F/mipsup.p \
		$F/vtrace.p $F/optab.p \
		$D/parse.p $D/rand.p $D/motopt.p $D/motopt2.p $D/rmframe.p \
		$M/optim.p $M/inter.p

protos:		$(PFILES)
			cat $(PFILES) > $M/protos.h
#------------------------------------------------------------

$O/optim:	$(OFILES)
		$(CC) $(CFLAGS) $(LDFLAGS) -o $O/optim $(OFILES) $(LIBV) $(LDLIBS)

$O/optim2:
		$(MAKE) -$(MAKEFLAGS)f mach.mk MACH=M68020 NOTMACH=M68040 $O/optim
		mv $O/optim $O/optim2

$O/optim4:
		$(MAKE) -$(MAKEFLAGS)f mach.mk MACH=M68040 NOTMACH=M68020 $O/optim
		mv $O/optim $O/optim4

		
$O/optim.o:        $(OPTIM_H) $M/optim.c
		$(CC_CMD) $M/optim.c

$O/inter.o:        $(OPTIM_H) $M/inter.c
		$(CC_CMD) $M/inter.c

$O/scan.o:         $(OPTIM_H) $F/scan.c $(OP_FILES)
		$(CC_CMD) $F/scan.c

$O/motopt.o:        $(OPTIM_H) $D/motopt.c $(COMP2)/macdefs.h
		$(CC_CMD) -I$(COMP2) $D/motopt.c

$O/motopt2.o:        $(OPTIM_H) $D/motopt2.c $(COMP2)/macdefs.h
		$(CC_CMD) -I$(COMP2) $D/motopt2.c

$O/rmframe.o:        $(OPTIM_H) rmframe.c $(COMP2)/macdefs.h
		$(CC_CMD) -I$(COMP2) $D/rmframe.c

$O/parse.o:        $(OPTIM_H) parse.c $(COMP2)/macdefs.h
		$(CC_CMD) -I$(COMP2) $D/parse.c

$O/rand.o:		$(OPTIM_H) $D/rand.c
		$(CC_CMD) $D/rand.c

$O/mipsup.o:	$(OPTIM_H) $F/mipsup.c
		$(CC_CMD) $F/mipsup.c

$(OP_FILES):	$F/optab.base $D/optab.awk
		awk -f $D/optab.awk $(AWKLIST) $F/optab.base \
				>$F/optab.tail;
#
# NOTE: awk also builds a new "$D/ops.tail" file.
#
		cat $D/ops.head $D/ops.tail >$F/ops.h
		touch $(OP_FILES)

$F/optab.c:	$D/optab.head $F/optab.base $D/optab.awk $(OP_FILES)
		cat $D/optab.head $F/optab.tail >$F/optab.c

$O/optab.o:	$F/optab.c $F/ops.h
		$(CC_CMD) $F/optab.c

$O/vtrace.o:	$F/vtrace.c $F/peep.c $(OPTIM_H)
		$(CC_CMD) $F/vtrace.c 

#-------------------------

install:	all	
		-rm -f  $(LIBDIR)/$(SGS)optim $(LIBDIR)/$(SGS)optim2 \
  			$(LIBDIR)/$(SGS)optim4 
		$(INS) $O/optim $(LIBDIR)/$(SGS)optim
		chmod 755 $(LIBDIR)/$(SGS)optim
		chgrp bin $(LIBDIR)/$(SGS)optim
		chown bin $(LIBDIR)/$(SGS)optim
		$(INS) $O/optim2 $(LIBDIR)/$(SGS)optim2
		chmod 755 $(LIBDIR)/$(SGS)optim2
		chgrp bin $(LIBDIR)/$(SGS)optim2
		chown bin $(LIBDIR)/$(SGS)optim2
		$(INS) $O/optim4 $(LIBDIR)/$(SGS)optim4
		chmod 755 $(LIBDIR)/$(SGS)optim4
		chgrp bin $(LIBDIR)/$(SGS)optim4
		chown bin $(LIBDIR)/$(SGS)optim4

#--------------------------

clean: 
	-rm -f $(OFILES) $(OP_FILES) 

clobber: clean
	-rm -f $O/optim $O/optim2 $O/optim4 $F/optab.c */ops.h 

shrink: clean
