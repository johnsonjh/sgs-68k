#ident	"@(#)mach.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#
#	MC68000 PCC2 TWOPASS MAKEFILE
#		MACH must be set to M68000, M68010, M68020, or M68040
#               FPU must be set to M68881
#
#
SROOT =
IROOT =
include $(IROOT)/macdefs
include $(IROOT)/sgsdefs

# POSIX
SVID_FLAG=

SGS=

LIBDIR=	$(ROOT)/lib

COMS=    $(PWD)/../common
# Directory for sdb.h
SDBINC = $(PWD)/.
BASE = $(PWD)/../..

# Compiler version id (Used for .version asm directive)
# Format must be VERSION= -DCOMPVERS=idstring
# (Don't use until assembler knows about .version)
#VERSION =

#
#	conditional compilation variables are declared 
#	in the DEFLIST and are passed to cpp.
#
#	variables:
#               DFLTDBLALIGN - default environment flag DBLALIGN to YES
#                         instead of NO. This same flag exists in the high
#                         level optimizer's make file and must be identical.
#               FORCE881 - builds a compiler which always generates '881 code.
#			  This is for Unix systems which will always have an 
#			  '881 in the system or when not will support software 
#			  and the F-LINE emulator trap to do the '881 funcs.
#		PROFILE - causes inclusion of profiling code -
#			  conditional compilation can be removed if desired.
#
#		TWOPASS - causes inclusion of code when defined
#			  to generate two pass compiler.  this "#undef"s
#			  ONEPASS in macdefs.h.  if not defined, a one
#			  pass compiler is created.
#
#		ITREES  - Use icon based passing to pass all information
#			  to the back end.
#
#        Note 1: ITREES and TWOPASS must be set for use of coptim.
#        Note 2: ITREES requires TWOPASS (#ifdef dependencies as written)



DEFLIST=         -Dm68k  -DPROFILE -DFORCE881 $(ODEFLIST)
TWOPASS=	-DTWOPASS -DITREES
#LDFLAGS=        -s
YFLAGS= -l

CC_CMD=	cd $O; $(CC) -c $(CFLAGS) $(DEFLIST) $(TWOPASS)
YACC_CMD=	$(YACC) $(YFLAGS)

# use M as a short-hand for the awkward COMS
# (M for Machine independent part)
# (D for Machine dependent part)
# (P for Processor dependent part)

M=      $(COMS)
D=	$(PWD)
P=	$(PWD)/$(MACH)
F=	$(PWD)/$(FAMILY)
O=$(PWD)/$(OUT)


C0_OFILES= $O/trees.o $O/scan.o $O/cgram.o $O/xdefs.o \
			$O/pftn.o $O/optim.o $O/local.o $O/comm1.o \
			$O/debug.o $O/inline1.o $O/proto.o $O/grey.o

C0_CFILES= $M/trees.c $M/scan.c $M/cgram.c $M/xdefs.c $M/pftn.c $M/optim.c\
	$M/comm1.c $M/xdefs.c $D/debug.c $D/local.c $M/inline1.c $M/proto.c	\
	$M/grey.c

C1_OFILES= $O/tablea.o $O/reader.o $O/local2.o $O/match.o \
			$O/allo.o $O/comm2.o $O/tablea2.o  $O/tableb.o\
			$O/cgen.o $O/cost.o $O/xdefs.o $O/inline2.o $O/iconmul.o

C1_CFILES= $M/reader.c $M/match.c $M/allo.c $M/comm2.c $M/cgen.c\
	$M/cost.c $M/xdefs.c $F/local2.c $F/tablea.c\
	$F/tablea2.c $F/tableb.c $M/inline2.c $M/iconmul.c

#-----------------------
all build:	$O/c0 $O/c1
#----------------------- FUNCTION PROTOTYPING -------------
.SUFFIXES:	 .p
.c.p:
		$(CC_CMD) -G $<
		mv $(OUT)/$(*F).p $(<D)

C0_PFILES= $M/trees.p $M/scan.p $M/cgram.p $M/xdefs.p $M/pftn.p $M/optim.p\
	$M/comm1.p $M/xdefs.p $D/debug.p $D/local.p $M/inline1.p $M/proto.p	\
	$M/grey.p
C1_PFILES=	$M/reader.p $M/match.p $M/allo.p $M/comm2.p $M/cgen.p\
	$M/cost.p $M/xdefs.p $F/local2.p $F/tablea.p\
	$F/tablea2.p $F/tableb.p $M/inline2.p $M/iconmul.p

protos:		c0_protos c1_protos
c0_protos:		$(C0_PFILES)
				cat $(C0_PFILES) > $M/c0proto.h

c1_protos:		$(C1_PFILES)
				cat $(C1_PFILES) > $M/c1proto.h
#------------------------------------------------------------

$O/c0:	$(C0_OFILES)
		$(CC) $(CFLAGS) -o $O/c0 $(C0_OFILES) $(LDLB) $(LDFLAGS)

$O/c1:	$(C1_OFILES)
		$(CC) $(CFLAGS) -o $O/c1 $(C1_OFILES) $(LDLB) $(LDFLAGS)

sty:		$M/manifest.h $M/sty.y macdefs.h $M/mfile2.h $M/common
		-@echo "Expect 3 reduce/reduce conflicts"
		$(YACC_CMD) $M/sty.y
#
# always use the resident compiler to generate sty.
# whether generating a cross-compiler or a resident
# on a non-target machine. Also sty.y needs TWOPASS and FPU flags.
#
#		/bin/cc $(CFLAGS) -I$(LOCAL_INC) $(TWOPASS) -D$(FPU) $(DEFLIST) -I. -I$M -o sty y.tab.c $M/shptbl.c $(LDFLAGS)
		/bin/cc $(TWOPASS) -I$(LOCAL_INC) $(COPROC) $(DEFLIST) -I. -I$M -I../../inc/mach -o sty y.tab.c $M/shptbl.c
		rm y.tab.c

$O/trees.o:	$M/manifest.h macdefs.h $M/mfile1.h $M/modes.h $(SDBINC)/sdb.h \
			$M/trees.c $M/proto.h
		$(CC_CMD) $M/trees.c

$O/optim.o:	$M/manifest.h macdefs.h $M/mfile1.h $(SDBINC)/sdb.h $M/optim.c
		$(CC_CMD) $M/optim.c

$O/pftn.o:		$M/manifest.h macdefs.h $M/mfile1.h $M/modes.h $(SDBINC)/sdb.h \
			$M/pftn.c $M/proto.h
		$(CC_CMD) $M/pftn.c

$O/local.o:	$M/manifest.h macdefs.h $M/mfile1.h $(SDBINC)/sdb.h
		$(CC_CMD) $D/local.c

$O/scan.o:		$M/manifest.h macdefs.h $M/mfile1.h $M/modes.h \
			$(SDBINC)/sdb.h $M/scan.c
		$(CC_CMD) $M/scan.c

$O/xdefs.o:	$M/manifest.h $M/mfile1.h $(SDBINC)/sdb.h macdefs.h $M/xdefs.c
		$(CC_CMD) $M/xdefs.c

$O/cgram.o:	$M/manifest.h $M/mfile1.h $(SDBINC)/sdb.h macdefs.h $M/cgram.c
		$(CC_CMD) $M/cgram.c

$M/cgram.c:	$M/cgram.y
		-@echo "Expect 17 shift/reduce conflicts"
		$(YACC_CMD) $M/cgram.y
		mv y.tab.c $M/cgram.c

$O/proto.o:	$M/manifest.h $M/mfile1.h macdefs.h $M/proto.c $M/proto.h
		$(CC_CMD) $M/proto.c

$O/grey.o:	$M/manifest.h $M/mfile1.h macdefs.h $M/grey.c
		$(CC_CMD) $M/grey.c

$O/comm1.o:	$M/manifest.h $M/mfile1.h $(SDBINC)/sdb.h $M/common macdefs.h \
			$M/comm1.c
		$(CC_CMD) $M/comm1.c

$O/comm2.o:	$M/manifest.h $M/mfile2.h $(SDBINC)/sdb.h $M/common macdefs.h \
			$M/comm2.c
		$(CC_CMD) $M/comm2.c

$O/tablea.o:	$M/manifest.h $M/mfile2.h macdefs.h $F/tablea.c
		$(CC_CMD) $F/tablea.c

$O/tablea2.o:      $M/manifest.h $M/mfile2.h macdefs.h $F/tablea2.c
		$(CC_CMD) $F/tablea2.c

$O/tableb.o:	$O/tablea.o
		$(CC_CMD) $F/tableb.c

$F/tablea.c $F/tablea2.c $F/tableb.c:    sty $F/stin
		-@echo "Expect 2 \"... may be covered by ...\" messages"
		./sty <$F/stin
		mv tablea.c  $F/tablea.c
		mv tablea2.c $F/tablea2.c
		mv tableb.c  $F/tableb.c

$O/cost.o:		$M/manifest.h $M/mfile2.h macdefs.h $M/cost.c
		$(CC_CMD) $M/cost.c

$O/cgen.o:		$M/manifest.h $M/mfile2.h macdefs.h $M/cgen.c
		$(CC_CMD) $M/cgen.c

$O/reader.o:	$M/manifest.h $M/mfile2.h macdefs.h $M/reader.c
		$(CC_CMD) $M/reader.c

$O/local2.o:	$M/manifest.h $M/mfile2.h macdefs.h $F/local2.c
		$(CC_CMD) $F/local2.c

$O/debug.o:        $M/mfile1.h macdefs.h $D/debug.c
		$(CC_CMD) $D/debug.c

$O/match.o:	$M/manifest.h $M/mfile2.h macdefs.h $M/match.c $(SDBINC)/sdb.h
		$(CC_CMD) $M/match.c

$O/allo.o:		$M/manifest.h $M/mfile2.h macdefs.h $M/allo.c
		$(CC_CMD) $M/allo.c

$O/inline1.o:      $M/mfile2.h macdefs.h $M/manifest.h $M/inline1.c
		$(CC_CMD) $M/inline1.c

$O/inline2.o:      $M/mfile2.h macdefs.h $M/manifest.h $M/inline2.c
		$(CC_CMD) $M/inline2.c

$O/iconmul.o:	$M/mfile2.h macdefs.h $M/manifest.h $M/iconmul.c
		$(CC_CMD) $M/iconmul.c


#-------------------------

install:	all
		-rm -f $(LIBDIR)/$(SGS)c0 $(LIBDIR)/$(SGS)c1
		$(INS) $O/c0 $(LIBDIR)/$(SGS)c0
		$(INS) $O/c1 $(LIBDIR)/$(SGS)c1
		chmod 755 $(LIBDIR)/$(SGS)c0 $(LIBDIR)/$(SGS)c1
		chgrp bin $(LIBDIR)/$(SGS)c0 $(LIBDIR)/$(SGS)c1
		chown bin $(LIBDIR)/$(SGS)c0 $(LIBDIR)/$(SGS)c1

#--------------------------

clean:
	-rm -f $(C0_OFILES) $(C1_OFILES)
	-rm -f $F/table*.c sty shptbl.o y.tab.o $M/cgram.c

#--------------------------

clobber:
	-rm -f $(C0_OFILES) $(C1_OFILES)
	-rm -f  $O/c0 $O/c1 $F/table*.c sty shptbl.o y.tab.o $M/cgram.c
	-rm -f ../common/y.tab.c

#--------------------------
shrink:
	-rm -f $(C0_OFILES) $(C1_OFILES)
	-rm -f  $O/c0 $O/c1 $F/table*.c sty shptbl.o y.tab.o $M/cgram.c

uninstall:
	-rm -f $(LIB)/c0 $(LIB)/c1

save:
