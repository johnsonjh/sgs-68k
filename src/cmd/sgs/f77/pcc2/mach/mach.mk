#	START NEW ARIX SCCS HEADER
#
#	@(#) mach.mk: version 25.1 created on 12/2/91 at 17:44:18
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)mach.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#

#ident	"@(#)sgs/comp2/mach:mach.mk	1.1"

#	OLD MOT: makefile	7.2		85/09/18
#
#	MC68000 PCC2 TWOPASS MAKEFILE
#		PROCESSOR must be set to M68000, M68010, M68020, or M68040
#
#
SROOT =
IROOT =
include $(IROOT)/macdefs
include $(IROOT)/sgsdefs

# POSIX

SVID_FLAG =

BINDIR=	$(ROOT)/bin
LIBDIR=	$(ROOT)/lib

PCC2COM=	../common
# Directory for sdb.h
SDBINC=	.

FLEX=	-DFLEXNAMES

LFLAGS=	-p
YFLAGS =
#
#	To compile with enhanced asm capability, compile with
#		ASM	= -DIN_LINE
#		INLINE.C= $(PCC2COM)/inline.c
#		INLINE.O= inline.o
#	Otherwise, set all to null
ASM	= -DIN_LINE
INLINE0.C= $(PCC2COM)/inline.c
INLINE0.O= inline0.o
INLINE1.C= $(PCC2COM)/inline.c
INLINE1.O= inline1.o
#
INCLIST=	-I. -I $(PCC2COM) -I$(SDBINC) -I$(MINCRT)
#
#	conditional compilation variables are declared 
#	in the DEFLIST and are passed to cpp.
#
#	variables:
#		STKCK	- causes inclusion of code to generate
#			  stack fault preclusion code at function
#			  entry.  targeted for MC68000.
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

ODEFLIST=	-DSTKCK -DPROFILE -DCLOCAL
DEFLIST=	$(FLEX) $(ASM) -DSTINCC 
TWOPASS=	-DTWOPASS -DITREES
FPU=M68881
MINCRT= ../../inc/mach
PROCESSOR=$(MACH)

CC_CMD=	$(CC) -c $(CFLAGS) $(INCLIST) $(DEFLIST) $(ODEFLIST) $(TWOPASS) -D$(FPU)
YACC_CMD=	$(YACC) $(YFLAGS)

# use M as a short-hand for the awkward PCC2COM
# (M for Machine independent part)

M=	$(PCC2COM)


# OFILES= cgram.o xdefs.o scan.o pftn.o trees.o optim.o local.o reader.o\
	local2.o debug.o match.o allo.o comm1.o tablea.o tableb.o cost.o cgen.o\
	wordlist.o
# CFILES=	$M/cgram.c $M/xdefs.c $M/scan.c $M/pftn.c $M/cgen.c $M/cost.c \
	$M/trees.c $M/optim.c local.c $M/reader.c \
	$(FAMILY)/local2.c debug.c $M/match.c $M/allo.c $M/comm1.c \
	$(FAMILY)/tablea.c $(FAMILY)/tableb.c wordlist.c

C0_OFILES= trees.o scan.o cgram.o xdefs.o pftn.o optim.o local.o comm1.o \
		debug.o xdefs.o $(INLINE0.O)

C0_CFILES= $M/trees.c $M/scan.c $M/cgram.c $M/xdefs.c $M/pftn.c $M/optim.c\
	$M/comm1.c $M/xdefs.c debug.c local.c $(INLINE0.C)

C1_OFILES= reader.o local2.o match.o allo.o comm2.o tablea.o tableb.o\
	cgen.o cost.o xdefs.o wordlist.o $(INLINE1.O)

C1_CFILES= $M/reader.c $M/match.c $M/allo.c $M/comm2.c $M/cgen.c\
	$M/cost.c $M/xdefs.c $(FAMILY)/local2.c $(FAMILY)/tablea.c\
	$(FAMILY)/tableb.c wordlist.c $(INLINE1.C)

all build:	c0 c1
#-------------------------

c0:	$(C0_OFILES)
		$(CC) $(CFLAGS) $(LDFLAGS) -o c0 $(C0_OFILES)

c1:	$(C1_OFILES)
		$(CC) $(CFLAGS) $(LDFLAGS) -o c1 $(C1_OFILES)

sty:		$M/manifest.h $M/sty.y macdefs.h $M/mfile2.h $M/common
		-@echo "Expect 3 reduce/reduce conflicts"
		$(YACC_CMD) $M/sty.y
	#
	# always use the resident compiler to generate sty.
	# whether generating a cross-compiler or a resident
	# on a non-target machine.
	#
	#	/bin/cc -I. -I$M -o sty y.tab.c $M/shptbl.c
	       /bin/cc -I$(LOCAL_INC) $(ASM) -I. -I$M -o sty y.tab.c $M/shptbl.c
		rm y.tab.c

trees.o:	$M/manifest.h macdefs.h $M/mfile1.h $(SDBINC)/sdb.h $M/trees.c
		$(CC_CMD) $M/trees.c

optim.o:	$M/manifest.h macdefs.h $M/mfile1.h $(SDBINC)/sdb.h $M/optim.c
		$(CC_CMD) $M/optim.c

pftn.o:		$M/manifest.h macdefs.h $M/mfile1.h $(SDBINC)/sdb.h $M/pftn.c
		$(CC_CMD) $M/pftn.c

local.o:	$M/manifest.h macdefs.h $M/mfile1.h $(SDBINC)/sdb.h $(MINCRT)/paths.h
		$(CC_CMD) local.c

scan.o:		$M/manifest.h macdefs.h $M/mfile1.h $(SDBINC)/sdb.h $M/scan.c
		$(CC_CMD) $M/scan.c

xdefs.o:	$M/manifest.h $M/mfile1.h $(SDBINC)/sdb.h macdefs.h $M/xdefs.c
		$(CC_CMD) $M/xdefs.c

cgram.o:	$M/manifest.h $M/mfile1.h $(SDBINC)/sdb.h macdefs.h $M/cgram.c
		$(CC_CMD) $M/cgram.c

$M/cgram.c:	$M/cgram.y
		-@echo "Expect 6 shift/reduce conflicts"
		$(YACC_CMD) $M/cgram.y
		mv y.tab.c $M/cgram.c

comm1.o:	$M/manifest.h $M/mfile1.h $(SDBINC)/sdb.h $M/common macdefs.h \
			$M/comm1.c
		$(CC_CMD) $M/comm1.c

comm2.o:	$M/manifest.h $M/mfile2.h $(SDBINC)/sdb.h $M/common macdefs.h \
			$M/comm2.c
		$(CC_CMD) $M/comm2.c

tablea.o:	$M/manifest.h $M/mfile2.h macdefs.h $(FAMILY)/tablea.c
		$(CC_CMD) -$(FAMILY)/tablea.c

tableb.o:	tablea.o
		$(CC_CMD) $(FAMILY)/tableb.c

$(FAMILY)/tablea.c $(FAMILY)/tableb.c:	sty $(FAMILY)/stin
		-@echo "Expect several \"... may be covered by ...\" messages"
		./sty <$(FAMILY)/stin
		mv tablea.c $(FAMILY)/tablea.c
		mv tableb.c $(FAMILY)/tableb.c

cost.o:		$M/manifest.h $M/mfile2.h macdefs.h $M/cost.c
		$(CC_CMD) $M/cost.c

cgen.o:		$M/manifest.h $M/mfile2.h macdefs.h $M/cgen.c
		$(CC_CMD) $M/cgen.c

reader.o:	$M/manifest.h $M/mfile2.h macdefs.h $M/reader.c $(MINCRT)/paths.h
		$(CC_CMD
		$(CC_CMD) $M/reader.c

local2.o:	$M/manifest.h $M/mfile2.h macdefs.h $(FAMILY)/local2.c
		$(CC_CMD) $(FAMILY)/local2.c

debug.o:	$M/mfile1.h macdefs.h $(INCRT)/storclass.h
		@-if [ "$(FAMILY)" = "m68k" ] ;\
		then $(CC_CMD) -DCC20 debug.c ;\
		echo " $(CC_CMD) -DCC20 debug.c" ;\
		else $(CC_CMD) -UCC20 debug.c ;\
		echo " $(CC_CMD) -UCC20 debug.c" ;\
		fi 
match.o:	$M/manifest.h $M/mfile2.h macdefs.h $M/match.c $(SDBINC)/sdb.h
		$(CC_CMD) $M/match.c

allo.o:		$M/manifest.h $M/mfile2.h macdefs.h $M/allo.c
		$(CC_CMD) $M/allo.c

inline0.o:	$M/mfile2.h macdefs.h $M/manifest.h $M/inline.c
		$(CC_CMD) -DPASSONE $M/inline.c
		/bin/mv inline.o inline0.o

inline1.o:	$M/mfile2.h macdefs.h $M/manifest.h $M/inline.c
		$(CC_CMD) -DPASSTWO $M/inline.c
		/bin/mv inline.o inline1.o
#-------------------------

install:	all
		-rm -f $(LIBDIR)/$(SGS)c0 $(LIBDIR)/$(SGS)c1
		$(INS) c0 $(LIBDIR)/$(SGS)c0
		$(INS) c1 $(LIBDIR)/$(SGS)c1
		chmod 755 $(LIBDIR)/$(SGS)c0 $(LIBDIR)/$(SGS)c1
		chgrp bin $(LIBDIR)/$(SGS)c0 $(LIBDIR)/$(SGS)c1
		chown bin $(LIBDIR)/$(SGS)c0 $(LIBDIR)/$(SGS)c1

#--------------------------

clean:
	-rm -f $(C0_OFILES) $(C1_OFILES)

#--------------------------

clobber:	clean
		-rm -f c0 c1 $(FAMILY)/table*.c sty shptbl.o y.tab.o $M/cgram.c
		-rm -f tags greplist*

#--------------------------

lint:	$(C0_CFILES) $(C1_CFILES)
	$(LINT) $(LFLAGS) -I. -I$M -I$(SDBINC) $(C0_CFILES) $(C1_CFILES) >lint.out

#--------------------------
DEFS=$(DEFLIST) $(ODEFLIST) $(TWOPASS) -D$(FAMILY) -D$(PROCESSOR) -D$(FPU)

cxref0: $(C0_CFILES)
	cxref -c -I. -I$M -I$(SDBINC) $(DEFS) $(C0_CFILES) -o c0.xref

cxref1: $(C1_CFILES)
	cxref -c -I. -I$M -I$(SDBINC) $(DEFS) $(C1_CFILES) -o c1.xref

cxref: cxref0 cxref1

#--------------------------
C0_CFILES2= $M/trees.c $M/scan.c $M/cgram.y $M/xdefs.c $M/pftn.c $M/optim.c\
	$M/comm1.c $M/xdefs.c debug.c local.c

cflow0:	$(C0_CFILES2)
	cflow -r -I. -I$M -I$(SDBINC) $(DEFS) $(C0_CFILES) >c0.cflow

cflow1:	$(C1_CFILES)
	cflow -r -I. -I$M -I$(SDBINC) $(DEFS) $(C1_CFILES) >c1.cflow

cflow: cflow0 cflow1

HFILES= \
	../common/common ../common/manifest.h ../common/mfile1.h	\
	../common/mfile2.h macdefs.h sdb.h

ctags:
	ctags $(C0_CFILES) $(C1_CFILES)  ../common/cgram.y $(HFILES)
	echo 'grep $$* \\' >greplist0
	for i in $(C0_CFILES) ../common/cgram.y	$(HFILES);\
	do echo "$$i \\"	;\
	done >> greplist0
	echo /dev/null >> greplist0
	chmod +x greplist0
	echo 'grep $$* \\' >greplist1
	for i in $(C1_CFILES)	$(HFILES);\
	do echo "$$i \\"	;\
	done >> greplist1
	echo /dev/null >> greplist1
	chmod +x greplist1
