#ident	"@(#)onepass.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	AT&T: ident	"stincc:m32/makefile	1.15"
#
SROOT =
IROOT =
include $(IROOT)/macdefs

# POSIX
SVID_FLAG=

#
VERSION =

# compiler version string to be output to assembler
COMPVERS= "\"02.01\""
BINDIR = $(ROOT)/bin
LIBDIR = $(ROOT)/lib
PCC2COM = ../common
STINCCOM = ../scommon
STYFLAGS = 

# Directory for sdb.h
SDBINC = .
# Directory for paths.h and sgs.h
MINCRT = ../../inc/mach
FLEX= -DFLEXNAMES
LINT= lint
LFLAGS = -p
INCLIST = -I. -I $(PCC2COM) -I$(SDBINC) -I$(MINCRT)

#
#	To compile with enhanced asm capability, compile with
#		ASM	= -DIN_LINE
#		INLINE.C= $(PCC2COM)/inline.c
#		INLINE.O= inline.o
#	Otherwise, set all to null
ASM	= -DIN_LINE
INLINE.C = $(PCC2COM)/inline.c
INLINE.O = inline.o

#
FPU = M68881
PROCESSOR = $(MACH)
#TWOPASS = -DTWOPASS -DITREES
TWOPASS =
DEFLIST = -DM32B -DNOSIMPSTR $(ASM) -DSTKCK -DPROFILE # -DSTINCC
ODEFLIST = $(FLEX) -D$(FPU) -D$(PROCESSOR) -DSPECALIGN  
YFLAGS=	-l
#
STRIP = strip
SPFLAGS =
STRIP_CMD = $(STRIP) $(SPFLAGS)
CC_CMD = $(CC) -c $(CFLAGS) $(INCLIST) $(DEFLIST) $(ODEFLIST) $(TWOPASS)
YACC_CMD = $(YACC) $(YFLAGS)

# use M as a short-hand for the awkward PCC2COM, S for STINCCOM
# (M for Machine independent part, S for Stincc code)

M =	$(PCC2COM)
S =	$(STINCCOM)


OFILES = cgram.o xdefs.o scan.o pftn.o trees.o optim.o local.o \
	reader.o local2.o debug.o match.o allo.o comm1.o \
	tablea.o tableb.o cost.o cgen.o optdir.o $(INLINE.O)

CFILES = cgram.c $M/xdefs.c $M/scan.c $M/pftn.c $M/trees.c $M/optim.c local.c \
	$M/reader.c local2.c $M/debug.c $S/match.c $M/allo.c $M/comm1.c \
	tablea.c tableb.c $M/cgen.c $S/cost.c optdir.c $(INLINE.C)

all build:	comp

#-------------------------

comp:	$(OFILES)
		$(CC) $(CFLAGS) $(LDFLAGS) -o comp $(OFILES)
		@echo $@ done

sty:		$M/manifest.h macdefs.h $M/mfile2.h $M/dope.h $S/shptbl.c sty.c
		cc $(ASM) -I. -I$M -o sty $S/shptbl.c sty.c  #native cc !!!
sty.c:		$S/sty.y
		-@echo "Expect 3 reduce/reduce conflicts"
		$(YACC_CMD) $S/sty.y
		mv y.tab.c sty.c

trees.o:	$M/manifest.h macdefs.h $M/mfile1.h $(SDBINC)/sdb.h $M/trees.c
		$(CC_CMD) $M/trees.c

optim.o:	$M/manifest.h macdefs.h $M/mfile1.h $(SDBINC)/sdb.h $M/optim.c
		$(CC_CMD) $M/optim.c

pftn.o:		$M/manifest.h macdefs.h $M/mfile1.h $(SDBINC)/sdb.h $M/pftn.c
		$(CC_CMD) $M/pftn.c

local.o:	$M/manifest.h macdefs.h $M/mfile1.h \
			$(SDBINC)/sdb.h $(MINCRT)/paths.h $(MINCRT)/sgs.h
		$(CC_CMD) -DCOMPVERS=$(COMPVERS) local.c

scan.o:		$M/manifest.h macdefs.h $M/mfile1.h $(SDBINC)/sdb.h $M/scan.c
		$(CC_CMD) $M/scan.c

xdefs.o:	$M/manifest.h $M/mfile1.h $(SDBINC)/sdb.h macdefs.h $M/xdefs.c
		$(CC_CMD) $M/xdefs.c

cgram.o:	$M/manifest.h $M/mfile1.h $(SDBINC)/sdb.h macdefs.h cgram.c
		$(CC_CMD) -DYYDEBUG=0 cgram.c

cgram.c:	$M/cgram.y
		-@echo "Expect 9 shift/reduce conflicts"
		$(YACC_CMD) $M/cgram.y
		mv y.tab.c cgram.c

COMMON=		$M/common $M/dope.h

comm1.o:	$M/manifest.h $M/mfile1.h $(SDBINC)/sdb.h $(COMMON) macdefs.h \
			$M/comm1.c
		$(CC_CMD) $M/comm1.c

comm2.o:	$M/manifest.h $M/mfile2.h $(SDBINC)/sdb.h $(COMMON) macdefs.h \
			$M/comm2.c
		$(CC_CMD) $M/comm2.c

tablea.o:	$M/manifest.h $M/mfile2.h macdefs.h tablea.c
		$(CC_CMD) tablea.c

tableb.o:	$M/manifest.h $M/mfile2.h macdefs.h tableb.c
		$(CC_CMD) tableb.c

tablea.c tableb.c:	sty stin
		-@echo "Expect several \"... may be covered by ...\" messages"
		./sty $(STYFLAGS) < stin

cost.o:		$M/manifest.h $M/mfile2.h macdefs.h $S/cost.c
		$(CC_CMD) $S/cost.c

cgen.o:		$M/manifest.h $M/mfile2.h macdefs.h $M/cgen.c
		$(CC_CMD) $M/cgen.c

reader.o:	$M/manifest.h $M/mfile2.h macdefs.h $M/reader.c
		$(CC_CMD) $M/reader.c

local2.o:	$M/manifest.h $M/mfile2.h macdefs.h
		$(CC_CMD) local2.c

debug.o:	$M/mfile1.h macdefs.h $(INCRT)/storclass.h $M/debug.c
		$(CC_CMD) $M/debug.c

match.o:	$M/manifest.h $M/mfile2.h macdefs.h $S/match.c $(SDBINC)/sdb.h
		$(CC_CMD) $S/match.c

allo.o:		$M/manifest.h $M/mfile2.h macdefs.h $M/allo.c
		$(CC_CMD) $M/allo.c

optdir.o:	$M/mfile1.h macdefs.h $M/manifest.h $(INCRT)/storclass.h
		$(CC_CMD) optdir.c

inline.o:	$M/mfile2.h macdefs.h $M/manifest.h $M/inline.c
		$(CC_CMD) $M/inline.c

install:	all
		-rm -f $(LIBDIR)/$(SGS)comp
		$(INS) comp $(LIBDIR)/$(SGS)comp
		chmod 755 $(LIBDIR)/$(SGS)comp
		chgrp bin $(LIBDIR)/$(SGS)comp
		chown bin $(LIBDIR)/$(SGS)comp

#--------------------------

clean:
	-rm -f $(OFILES) sty.o shptbl.o

#--------------------------

clobber:	clean
		-rm -f comp tablea.c tableb.c sty cgram.c sty.c
		-rm -f tags greplist*

#--------------------------

lint:	$(CFILES)
	$(LINT) $(LFLAGS) -I. -I$M -I$(SDBINC) $(CFILES) >lint.out

#--------------------------
DEFS=$(DEFLIST) $(ODEFLIST) $(TWOPASS) -D$(PROCESSOR) -D$(FPU)

compxref: $(CFILES)
	cxref -c -I. -I$M -I$(SDBINC) $(DEFS) $(CFILES) -o comp.xref

cxref: compxref

#--------------------------

compflow:	$(CFILES2)
	cflow -r -I. -I$M -I$(SDBINC) $(DEFS) $(CFILES) >comp.cflow

cflow: compflow

HFILES= \
	../common/common ../common/manifest.h ../common/mfile1.h	\
	../common/mfile2.h macdefs.h sdb.h 

ctags:
	ctags $(CFILES) ../common/cgram.y $(HFILES)
	echo 'grep $$* \\' >greplist0
	for i in $(CFILES) ../common/cgram.y	$(HFILES);\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
