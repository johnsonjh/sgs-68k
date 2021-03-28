#ident	"@(#)mach.mk	23.3	2/26/91 Copyright (c) 1991 by Arix Corp."

#	OLD MOT:makefile	7.1	
#
#	m68k AS MAKEFILE
#	     generic makefile for mc68000 family
#	     The environment/makefile variable PROCESSOR
#	     should be set to indicate which one.
#	     Current values are M68000, M68010, M68020
#
#	

include $(IROOT)/macdefs
include $(IROOT)/sgsdefs

# POSIX
SVID_FLAG=

OWN=	bin		#file owner
GRP=	bin		#file group
PROT=	755		#protections

BINDIR=	$(ROOT)/bin
BASE = $(PWD)/../..
M68INC=	$(BASE)/inc/mach
COMINC=	$(INCRT)
COMAS=	$(BASE)/as/common

LINT=	lint
LFLAGS=	-p
DEFLIST=	-DINT32BIT
ODEFLIST=	-DONEPROC
# LDFLAGS=
YFLAGS=		-dl

# use M as a short-hand for the awkward COMAS
# (M for Machine independent part)
# (D for Machine dependent part)
# (P for Processor dependent part)

M=      $(COMAS)
D=	$(PWD)
F=	$(PWD)/$(FAMILY)
O=$(PWD)/$(OUT)


CC_CMD=	cd $O; $(CC) -c $(DEFLIST) $(ODEFLIST) $(CFLAGS) 
YACC_CMD=	$(YACC) $(YFLAGS)


OFILES=	$O/pass0.o $O/instab.o $O/parse.o $O/gencode.o $O/pass1.o $O/symbols.o \
	$O/code.o $O/expand1.o $O/expand2.o $O/errors.o $O/addr2.o $O/pass2.o  \
	$O/addr1.o $O/codeout.o $O/getstab.o $O/obj.o $O/symlist.o $O/dofpnum.o \
	$O/spitcode.o $O/radfix.o $O/unused.o $O/stamptime.o

CFILES=	$M/pass0.c $M/instab.c $D/parse.c 	     \
	$F/gencode.c $F/addr2.c	     \
	$F/expand2.c 			     \
	$M/pass1.c $M/symbols.c $M/code.c  \
	$M/expand1.c $M/errors.c		     \
	$M/pass2.c $M/addr1.c		     \
	$M/codeout.c $M/getstab.c $M/obj.c \
	$M/symlist.c \
	$M/unused.c \
	$D/dofpnum.c \
	$D/spitcode.c $D/radfix.c $D/stamptime.c

HFILES=	$M/codeout.h gendefs.h 			     \
	$F/instab.h $F/ops.out	     \
	$M/symbols.h $(M68INC)/paths.h		     \
	$M/expand.h $D/expand2.h $D/parse.h		     \
	$(COMINC)/reloc.h $(COMINC)/syms.h		     \
	$(COMINC)/storclass.h $(COMINC)/filehdr.h	     \
	$(COMINC)/scnhdr.h $(COMINC)/linenum.h		     \
	$(M68INC)/sgs.h	\
	$D/fpnum.h $D/fpinit.h

all build:	$O/as3 $O/as2 $O/as4
	if [ $(MACH) = "M68020" ]; \
	then \
		ln $O/as2 $O/as; \
	elif [ $(MACH) = "M68030" ]; \
	then \
		ln $O/as3 $O/as; \
	else \
		ln $O/as4 $O/as; \
	fi

$O/as2:
	$(MAKE) -$(MAKEFLAGS)f mach.mk MACH=M68020 NOTMACH="M68030 \
		-UM68040" clean $O/as
	mv $O/as $O/as2

$O/as3:
	$(MAKE) -$(MAKEFLAGS)f mach.mk MACH=M68030 NOTMACH="M68020 \
		-UM68040" clean $O/as
	mv $O/as $O/as3

$O/as4:
	$(MAKE) -$(MAKEFLAGS)f mach.mk MACH=M68040 NOTMACH="M68020 \
		-UM68030" clean $O/as
	mv $O/as $O/as4


#----------------------- FUNCTION PROTOTYPING -------------
.SUFFIXES:	 .p
.c.p:
		$(CC_CMD) -G $<
		mv $(OUT)/$(*F).p $(<D)

PFILES=	$M/pass0.p $M/instab.p $D/parse.p 	     \
	$F/gencode.p $F/addr2.p	     \
	$F/expand2.p 			     \
	$M/pass1.p $M/symbols.p $M/code.p  \
	$M/expand1.p $M/errors.p		     \
	$M/pass2.p $M/addr1.p		     \
	$M/codeout.p $M/getstab.p $M/obj.p \
	$M/symlist.p \
	$M/unused.p \
	$D/dofpnum.p \
	$D/spitcode.p $D/radfix.p $D/stamptime.p

protos:		$(PFILES)
			cat $(PFILES) > $M/protos.h
#------------------------------------------------------------

$O/as:	$(OFILES)
		$(CC) $(CFLAGS) -o $O/as $(OFILES) $(LDFLAGS)

$O/pass0.o:	$M/pass0.c $D/pass0.h $(M68INC)/paths.h   \
		$(M68INC)/sgs.h $D/systems.h
		$(CC_CMD) $M/pass0.c

# The baroque code here tries to keep file times in sync to
# avoid unnecessary re-compilations.

$O/parse.o:	$O/parse.c $M/symbols.h $F/instab.h gendefs.h
		$(CC_CMD) parse.c

$O/instab.o:	$M/instab.c $F/instab.h $F/ops.out $D/parse.h \
		$M/symbols.h $D/systems.h
		$(CC_CMD) $M/instab.c

$O/parse.c:	$D/y.tab.c
		if cmp -s $D/y.tab.c $O/parse.c;then :;else cp $D/y.tab.c $O/parse.c;fi
$D/parse.h:	$D/y.tab.h
		if cmp -s $D/y.tab.h $D/parse.h;then :;else cp $D/y.tab.h $D/parse.h;fi

$D/y.tab.c $D/y.tab.h:	$F/parse.y
		$(YACC_CMD) $F/parse.y

# end baroque code

$O/code.o:		$M/code.c $M/symbols.h	     \
		$M/codeout.h gendefs.h systems.h
		$(CC_CMD) $M/code.c

$O/errors.o:	$M/errors.c $D/gendefs.h $D/systems.h
		$(CC_CMD) $M/errors.c

$O/pass1.o:	$M/pass1.c $(M68INC)/paths.h $D/gendefs.h \
		$M/symbols.h $D/systems.h
		$(CC_CMD) $M/pass1.c

$O/gencode.o:	$F/gencode.c $M/symbols.h $F/instab.h\
		gendefs.h $M/expand.h $D/expand2.h
		$(CC_CMD) $F/gencode.c

$O/expand1.o:	$M/expand1.c $M/expand.h	     \
		$M/symbols.h $D/gendefs.h $D/systems.h
		$(CC_CMD) $M/expand1.c

$O/expand2.o:	$F/expand2.c $M/expand.h $D/expand2.h	     \
		$M/symbols.h
		$(CC_CMD) $F/expand2.c

$O/addr1.o:	$M/addr1.c $(COMINC)/reloc.h $D/gendefs.h \
		$(COMINC)/syms.h $(COMINC)/storclass.h	     \
		$(COMINC)/linenum.h $(COMINC)/filehdr.h	     \
		$M/symbols.h $M/codeout.h	     \
		$D/systems.h
		$(CC_CMD) $M/addr1.c

$O/addr2.o:	$F/addr2.c $(COMINC)/reloc.h $M/symbols.h \
		$F/instab.h $D/gendefs.h $M/codeout.h
		$(CC_CMD) $F/addr2.c

$O/codeout.o:	$M/codeout.c $M/symbols.h	     \
		$M/codeout.h $D/gendefs.h $D/systems.h
		$(CC_CMD) $M/codeout.c

$O/getstab.o:	$M/getstab.c $M/symbols.h	     \
		$D/gendefs.h $D/systems.h
		$(CC_CMD) $M/getstab.c

$O/pass2.o:	$M/pass2.c $D/gendefs.h $D/systems.h	     \
		$M/symbols.h
		$(CC_CMD) $M/pass2.c

$O/unused.o:	$M/unused.c $D/symbols2.h
		$(CC_CMD) $M/unused.c

$O/obj.o:		$M/obj.c $(COMINC)/filehdr.h	     \
		$(COMINC)/linenum.h $(COMINC)/reloc.h	     \
		$(COMINC)/scnhdr.h $(COMINC)/syms.h	     \
		$(COMINC)/storclass.h $M/symbols.h     \
		$M/codeout.h $D/gendefs.h $D/systems.h	     \
		$(M68INC)/sgs.h
		$(CC_CMD) $M/obj.c

$O/symlist.o:	$M/symlist.c $M/symbols.h	     \
		$(COMINC)/syms.h $(COMINC)/storclass.h	     \
		$D/gendefs.h $D/systems.h
		$(CC_CMD) $M/symlist.c

$O/symbols.o:	$M/symbols.c $M/symbols.h	     \
		$D/symbols2.h $D/systems.h
		$(CC_CMD) $M/symbols.c

$O/spitcode.o:	$D/spitcode.c 
		$(CC_CMD) $D/spitcode.c

$O/radfix.o:	$D/radfix.c 
		$(CC_CMD) $D/radfix.c

$O/stamptime.o:	$D/stamptime.c 
		$(CC_CMD) $D/stamptime.c

$O/dofpnum.o:	$D/dofpnum.c $D/fpnum.h $D/fpinit.h
		$(CC_CMD) $D/dofpnum.c

#-------------------------

install:	all
		-rm -f $(BINDIR)/$(SGS)as
		-rm -f $(BINDIR)/$(SGS)as2
		-rm -f $(BINDIR)/$(SGS)as3
		-rm -f $(BINDIR)/$(SGS)as4
		$(INS) $O/$(SGS)as $(BINDIR)/$(SGS)as
		chmod $(PROT) $(BINDIR)/$(SGS)as
		chgrp $(GRP) $(BINDIR)/$(SGS)as
		chown $(OWN) $(BINDIR)/$(SGS)as
		$(INS) $O/$(SGS)as2 $(BINDIR)/$(SGS)as2
		chmod $(PROT) $(BINDIR)/$(SGS)as2
		chgrp $(GRP) $(BINDIR)/$(SGS)as2
		chown $(OWN) $(BINDIR)/$(SGS)as2
		$(INS) $O/$(SGS)as3 $(BINDIR)/$(SGS)as3
		chmod $(PROT) $(BINDIR)/$(SGS)as3
		chgrp $(GRP) $(BINDIR)/$(SGS)as3
		chown $(OWN) $(BINDIR)/$(SGS)as3
		$(INS) $O/$(SGS)as4 $(BINDIR)/$(SGS)as4
		chmod $(PROT) $(BINDIR)/$(SGS)as4
		chgrp $(GRP) $(BINDIR)/$(SGS)as4
		chown $(OWN) $(BINDIR)/$(SGS)as4
		touch $(ROOT)/usr/lib/cm4defs
		chmod 444 $(ROOT)/usr/lib/cm4defs
		chgrp $(GRP) $(ROOT)/usr/lib/cm4defs
		chown $(OWN) $(ROOT)/usr/lib/cm4defs
		touch $(ROOT)/usr/lib/cm4tvdefs
		chmod 444 $(ROOT)/usr/lib/cm4tvdefs
		chgrp $(GRP) $(ROOT)/usr/lib/cm4tvdefs
		chown $(OWN) $(ROOT)/usr/lib/cm4tvdefs

#--------------------------

save:	$(BINDIR)/$(SGS)as
	-rm -f $(BINDIR)/$(SGS)as.back
	cp $(BINDIR)/$(SGS)as $(BINDIR)/$(SGS)as.back

#--------------------------

uninstall:	$(BINDIR)/$(SGS)as.back
		-rm -f $(BINDIR)/$(SGS)as
		cp $(BINDIR)/$(SGS)as.back $(BINDIR)/$(SGS)as

#--------------------------

clean:
	-rm -f $(OFILES)

#--------------------------

clobber:	clean
		-rm -f $O/as $O/as2 $O/as3 $O/as4 $(SGS)as $(SGS)as2 \
		$(SGS)as3 $(SGS)as4 $O/parse.c $D/parse.h \
		$D/y.tab.c $D/y.tab.h tags greplist

#--------------------------

lint:	$(COMINC)/a.out.h $(COMINC)/ar.h \
	$(CFILES)
	$(LINT) $(LFLAGS) -I$(M68INC) -I$(COMINC) $(CFILES) >lint.out


SOURCES= $F/parse.y $(CFILES) $(HFILES)
ctags:
	ctags $(SOURCES)
	echo 'grep $$* \\' >greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
