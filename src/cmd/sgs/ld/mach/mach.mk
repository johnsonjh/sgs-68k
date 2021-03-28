#ident	"@(#)mach.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT:ident	"ld:m32/makefile	1.25"


include	$(IROOT)/macdefs
include	$(IROOT)/sgsdefs

# POSIX
SVID_FLAG=

#
#	BELLMAC-ACH LD MASTER MAKEFILE
#
#
# Architectures:
#
#	AR16WR = DEC 11/45-70
#	ARACHWR = VAX 11/780
#	ARACHW  = 3B family, IBM 370
#
# Specify the version of UN*X beng used:
#
#	TS  :	BTL UN*X TS
#
OWN=bin
GRP=bin
B10=
SYS=TS
FFLAG=
#	Set ARFORMAT to PORTAR for portable archive format (post 5.0)
#	Set ARFORMAT to PORT5AR for 5.0 archive format
#	Set ARFORMAT to OLDAR for pre-5.0 archive format
ARFORMAT=PORTAR
DBO=FBO
ENV=
DEFLIST= -D$(ARFORMAT) -D$(DBO)
INCLIST= -I$(ICOMINC)
CC_CMD= cd $(OUT); $(CC) -c $(CFLAGS) $(DEFLIST) $(INCLIST)
#
LDLIBS=
LD_CMD=$(CC)
#
YFLAGS=-d
YACC_CMD=$(YACC) $(YFLAGS)
#
STRIP=strip
SPFLAGS=
STRIP_CMD=$(STRIP) $(SPFLAGS)
#
LINT=lint
LINTFLAGS=-p
LINT_CMD=$(LINT) $(LINTFLAGS)
#
#	Internal Directories
#
BASE= $(PWD)/../..
BINDIR= $(ROOT)/bin
COMLD= $(BASE)/ld/common
ICOMINC= $(BASE)/inc/common
COMINC= $(INCRT)
N3BINC= $(BASE)/inc/mach
#
#	External Directories
#
USRINC= $(INCRT)
#
LIBES= -ll
LIBS=
#
# use M as a short-hand for the awkward COMLD
# (M for Machine independent part)
# (D for Machine dependent part)
# (P for Processor dependent part)

M=      $(COMLD)
D=	$(PWD)
P=	$(PWD)/$(MACH)
O=$(PWD)/$(OUT)
#
#
#
# Specify the *.c files making up ld:
#
CFILES=$M/alloc.c $M/expr0.c $M/expr1.c \
	$M/globs.c $M/instr.c \
	$M/ld00.c $M/ld01.c $M/ld1.c $M/ld2.c \
	$M/lists.c $M/output.c $M/syms.c \
	$M/slotvec.c $M/tv.c $M/util.c \
	$M/version.c $M/gmatch.c \
	$D/lex.yy.c $M/maps.c $D/y.tab.c $D/sglobs.c $D/special1.c $D/special2.c \
	$D/specsyms.c $D/stamptime.c $M/namespace.c
#
#
# Specify the *.h files making up ld:
#
HFILES1=$D/y.tab.h $D/params.h $D/system.h \
	$M/ldmacros.h $M/attributes.h $M/list.h \
	$M/structs.h  $M/ldtv.h \
	$(COMINC)/aouthdr.h $M/slotvec.h \
	$M/instr.h
HFILES2=$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
	$(COMINC)/storclass.h $(ICOMINC)/bool.h \
	$(ICOMINC)/sgsmacros.h $(COMINC)/ldfcn.h $(COMINC)/reloc.h \
	$(COMINC)/linenum.h $(ICOMINC)/patch.h
HFILES3=$(MCOMINC)/sgs.h $(MCOMINC)/paths.h $(MCOMINC)/tv.h $(N3BINC)/magic.h
#
# Specify the *.o files making up ld:
#
OBJECTS=$O/expr0.o $O/globs.o $O/sglobs.o $O/ld00.o $O/lex.yy.o $O/lists.o \
	$O/util.o $O/version.o $O/y.tab.o $O/special1.o $O/special2.o \
	$O/alloc.o $O/specsyms.o $O/expr1.o $O/ld01.o $O/ld1.o $O/ld2.o $O/maps.o \
	$O/output.o $O/slotvec.o $O/syms.o $O/tv.o $O/instr.o \
	$O/gmatch.o $O/stamptime.o $O/namespace.o
#
#
# Make ld:
#
all:		$O/ld
#----------------------- FUNCTION PROTOTYPING -------------
.SUFFIXES:	 .p
.c.p:
		$(CC_CMD) -G $<
		mv $(OUT)/$(*F).p $(<D)

PFILES=$M/alloc.p $M/expr0.p $M/expr1.p \
	$M/globs.p $M/instr.p \
	$M/ld00.p $M/ld01.p $M/ld1.p $M/ld2.p \
	$M/lists.p $M/output.p $M/syms.p \
	$M/slotvec.p $M/tv.p $M/util.p \
	$M/version.p $M/gmatch.p \
	$D/lex.yy.p $M/maps.p $D/y.tab.p $D/sglobs.p $D/special1.p $D/special2.p \
	$D/specsyms.p $D/stamptime.p $M/namespace.p

protos:		$(PFILES)
			cat $(PFILES) > $M/protos.h
#------------------------------------------------------------
#

$O/ld:		$(OBJECTS) 
	$(LD_CMD) -o $O/ld $(OBJECTS) $(LIBES) $(LIBS) $(LDLIBS) -lld $(LDFLAGS)
#
#
# Make the *.c:
#
$D/y.tab.c $D/y.tab.h:	$M/ld.yac
	@echo "Expect to see message 'conflicts: 7 shift/reduce'";
	$(YACC_CMD) $M/ld.yac
#
$O/y.tab.o:	$M/attributes.h $M/list.h $M/structs.h  \
		$M/ldmacros.h  $M/ldtv.h $D/system.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/storclass.h $(ICOMINC)/bool.h $(ICOMINC)/sgsmacros.h \
		$(MCOMINC)/sgs.h $(MCOMINC)/tv.h \
		$D/params.h \
		$D/y.tab.c
	$(CC_CMD) $D/y.tab.c
#
$D/lex.yy.c:	$M/ld.lex 
	$(LEX) $M/ld.lex
#
$O/lex.yy.o:	$M/structs.h  $D/system.h  \
		$M/ldtv.h \
		$M/instr.h $(COMINC)/aouthdr.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/storclass.h $(ICOMINC)/bool.h \
		$(MCOMINC)/paths.h $(MCOMINC)/sgs.h $(MCOMINC)/tv.h \
		$D/y.tab.h \
		$D/lex.yy.c
	$(CC_CMD) $D/lex.yy.c
#
$O/expr0.o:	 $M/structs.h $D/system.h  \
		$M/ldmacros.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/storclass.h $(ICOMINC)/sgsmacros.h \
		$D/y.tab.h \
		$M/expr0.c
	$(CC_CMD) $M/expr0.c
#
$O/expr1.o:	 $M/structs.h $D/system.h  \
		$M/ldmacros.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/storclass.h $(ICOMINC)/sgsmacros.h \
		$D/y.tab.h \
		$M/expr1.c
	$(CC_CMD) $M/expr1.c
#
$O/ld00.o:	 $M/list.h $M/structs.h  \
		system.h $(COMINC)/ldfcn.h $M/ldmacros.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/storclass.h $(ICOMINC)/sgsmacros.h \
		$(MCOMINC)/paths.h $(MCOMINC)/sgs.h \
		$D/params.h $D/y.tab.h \
		$M/ld00.c
	$(CC_CMD) $M/ld00.c
#
$O/ld01.o:	 $M/structs.h $M/list.h  \
		$M/ldmacros.h $D/system.h $M/ldtv.h $M/instr.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/storclass.h $(ICOMINC)/sgsmacros.h \
		$(MCOMINC)/tv.h \
		$M/ld01.c
	$(CC_CMD) $M/ld01.c
#
$O/ld1.o:		 $M/list.h $M/ldtv.h  \
		$M/ldmacros.h \
		$M/structs.h $D/system.h $(COMINC)/aouthdr.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/storclass.h $(COMINC)/ldfcn.h $(COMINC)/reloc.h \
		$(ICOMINC)/sgsmacros.h \
		$(MCOMINC)/sgs.h $(MCOMINC)/tv.h $(N3BINC)/magic.h \
		$D/params.h \
		$M/instr.h \
		$M/ld1.c
	$(CC_CMD) $M/ld1.c
#
$O/ld2.o:		$M/ldtv.h  $M/list.h  \
		$M/structs.h $D/system.h $M/ldmacros.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(ICOMINC)/sgsmacros.h $(COMINC)/storclass.h \
		$(COMINC)/reloc.h $(COMINC)/linenum.h $(ICOMINC)/patch.h \
		$(MCOMINC)/tv.h \
		$D/y.tab.h $D/params.h \
		$M/instr.h \
		$M/ld2.c
	$(CC_CMD) $M/ld2.c
#
$O/alloc.o:	 $M/list.h $M/structs.h  \
		$M/ldmacros.h $D/system.h $M/ldtv.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/storclass.h $(ICOMINC)/sgsmacros.h \
		$(MCOMINC)/tv.h \
		$D/params.h \
		$M/alloc.c
	$(CC_CMD) $M/alloc.c
#
$O/output.o:	$M/structs.h  $D/system.h  \
		$M/ldtv.h $M/slotvec.h $(COMINC)/aouthdr.h \
		$M/ldmacros.h $M/instr.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(ICOMINC)/sgsmacros.h $(COMINC)/storclass.h \
		$(COMINC)/reloc.h $(COMINC)/linenum.h $(COMINC)/ldfcn.h \
		$(ICOMINC)/patch.h \
		$(MCOMINC)/sgs.h $(MCOMINC)/tv.h \
		$D/params.h \
		$M/output.c
	$(CC_CMD) $M/output.c
#
$O/maps.o:	 $M/structs.h $M/attributes.h  \
		system.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/storclass.h $(ICOMINC)/sgsmacros.h \
		$M/maps.c
	$(CC_CMD) $M/maps.c
#
$O/util.o:	$M/structs.h  $D/system.h  \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/storclass.h $(ICOMINC)/sgsmacros.h \
		$M/slotvec.h $(COMINC)/reloc.h $(MCOMINC)/sgs.h \
		$D/y.tab.h \
		$M/util.c
	$(CC_CMD) $M/util.c
#
$O/lists.o: 	$M/list.h $M/structs.h $D/system.h  \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(ICOMINC)/sgsmacros.h $(COMINC)/storclass.h \
		$M/lists.c
	$(CC_CMD) $M/lists.c
#
$O/syms.o:	$M/structs.h  $M/ldtv.h  \
		system.h $M/instr.h \
		$M/ldmacros.h $(COMINC)/ldfcn.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/storclass.h $(ICOMINC)/sgsmacros.h \
		$(MCOMINC)/paths.h $(MCOMINC)/tv.h \
		$M/syms.c
	$(CC_CMD) $M/syms.c
#
$O/tv.o:		 $M/structs.h $M/ldtv.h  \
		$M/ldmacros.h $D/system.h $M/list.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(ICOMINC)/bool.h $(COMINC)/storclass.h \
		$(COMINC)/reloc.h $(COMINC)/ldfcn.h $(ICOMINC)/sgsmacros.h \
		$(MCOMINC)/tv.h \
		$M/tv.c
	$(CC_CMD) $M/tv.c
#
$O/globs.o:	$M/structs.h $M/ldtv.h $D/system.h  \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/storclass.h \
		$(MCOMINC)/paths.h $(MCOMINC)/tv.h \
		$D/params.h \
		$M/globs.c
	$(CC_CMD) $M/globs.c
#
$O/instr.o:	$M/instr.c 
	$(CC_CMD) $M/instr.c
#
$O/slotvec.o:	$M/slotvec.h $M/structs.h   \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/storclass.h \
       		system.h $(MCOMINC)/paths.h \
		$M/slotvec.c
	$(CC_CMD) $M/slotvec.c
#
$O/version.o:	$M/version.c 
	$(CC_CMD) $M/version.c
#
$O/sglobs.o:	$M/structs.h $(COMINC)/filehdr.h  \
		$(COMINC)/scnhdr.h $(COMINC)/syms.h $(COMINC)/storclass.h \
		$(MCOMINC)/sgs.h $(MCOMINC)/tv.h $M/ldtv.h \
		system.h $D/sglobs.c
	$(CC_CMD) $D/sglobs.c
#
$O/special1.o:	$D/system.h $D/special1.c $M/structs.h    \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h $(COMINC)/storclass.h
	$(CC_CMD) $D/special1.c
#
$O/special2.o:	$(COMINC)/aouthdr.h $M/structs.h $(COMINC)/filehdr.h   \
		$(COMINC)/scnhdr.h $(COMINC)/syms.h $(COMINC)/storclass.h $(COMINC)/ldfcn.h \
		$(MCOMINC)/tv.h $M/ldtv.h $M/slotvec.h \
		$(COMINC)/reloc.h  $M/list.h \
		$(MCOMINC)/sgs.h $(N3BINC)/magic.h  $(ICOMINC)/sgsmacros.h \
		$D/system.h $D/special2.c
	$(CC_CMD) $D/special2.c
#
$O/specsyms.o:	system.h  $(ICOMINC)/sgsmacros.h   \
		$M/structs.h $(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h $(COMINC)/storclass.h \
		$(MCOMINC)/tv.h $M/ldtv.h \
		$D/specsyms.c
	$(CC_CMD) $D/specsyms.c
#
$O/stamptime.o:	$D/stamptime.c
	$(CC_CMD) $D/stamptime.c
	
$O/namespace.o:	$M/namespace.c $M/namespace.h
	$(CC_CMD) $M/namespace.c
	
$O/gmatch.o:	$M/gmatch.c
	$(CC_CMD) $M/gmatch.c
#
#
# File and administrative commands:
#
install:	$O/ld
	-rm -f $(BINDIR)/$(SGS)ld
	$(INS) $O/ld $(BINDIR)/$(SGS)ld
	chmod 755 $(BINDIR)/$(SGS)ld
	chgrp bin $(BINDIR)/$(SGS)ld
	chown bin $(BINDIR)/$(SGS)ld

#
save:	$(BINDIR)/$(SGS)ld
	-rm -f $(BINDIR)/$(SGS)ld.back
	cp $(BINDIR)/$(SGS)ld   $(BINDIR)/$(SGS)ld.back
#
uninstall:	$(BINDIR)/$(SGS)ld.back
	-rm -f $(BINDIR)/$(SGS)ld
	cp  $(BINDIR)/$(SGS)ld.back    $(BINDIR)/$(SGS)ld
#
clean:
	rm -f $(OBJECTS)

clobber: clean
	-rm -f $D/y.tab.c $O/lex.yy.c $D/y.tab.h $O/ld tags greplist

#
# Run lint on the *.c files:
#
lint:		$(CFILES) $(HFILES1) $(HFILES2) $(HFILES3)
	$(LINT_CMD) $(DEFLIST) $(INCLIST) $(CFILES)
#
# Force recompilation
#
#FRC:


SOURCES= $(CFILES) $(HFILES1) $(HFILES2) $(HFILES3) 
ctags:
	ctags $(SOURCES)
	echo 'grep $$* \\' >greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
