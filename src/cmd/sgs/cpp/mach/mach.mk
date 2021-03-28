#ident	"@(#)mach.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

include	$(IROOT)/macdefs
include	$(IROOT)/sgsdefs

# POSIX
SVID_FLAG=


#	Cpp Makefile
#
#
#
# the two variables NEW_MACH and NEW_SYS are used to create new instances
# of cpp by giving new machine and/or system names to cpp.c.  These
# should be used with PD_MACH=D_newmach and PD_SYS=D_newsys.
# NEW_MACH and NEW_SYS are used (just as SGSINC is) as a string in cpp.c
#
PD_MACH	= MAX_PD_MACH
NEW_MACH =$(MACH)
NEW_SYS	=unix
#
#	External Directories
#
SGSBASE	= $(PWD)/../..
LIBDIR	= $(ROOT)/lib
INCDIR	= 
USRINC	= /usr/include
#
#	Internal Directories
#
CPPCOM	= $(SGSBASE)/cpp/common
COMINC  = $(SGSBASE)/inc/mach
# use M as a short-hand for the awkward CPPCOM
# (M for Machine independent part)
# (D for Machine dependent part)
M=      $(CPPCOM)
D=	$(PWD)
O=$(PWD)/$(OUT)
#
#
CC_CMD = cd $(OUT); $(CC) $(CFLAGS) $(DEFLIST) $(ENVPARMS) -I$(COMINC) -c -DNODBG
#
#	Compilation Parameters for cpp.o
#
ENVPARMS = -DSGSINC=\"$(INCDIR)\" -DUSRINC=\"$(USRINC)\" \
			-DPD_MACH=$(PD_MACH) -DPD_SYS=D_newsys	\
			-DPD_MY_SYS=\"$(NEW_SYS)\"  \
			-DPD_FAM=0  \
			-DPD_MY_MACH=\"$(NEW_MACH)\"
#
LIBES	=
#
#	Lint Parameters
#
LINT	= lint
LINTFLAGS = -p
#
#	Other Commands
#
SH	= sh
YFLAGS	= -d -l
STRIP	= strip
SPFLAGS	=
CP	= cp
RM	= rm
PRINT	= pr
PRFLAGS	= -n
LP	= lp
#
MODE	= 0755
GRP	= bin
OWN	= bin
#
#
######################## FUNCTION PROTOTYPING ##############
.SUFFIXES:	 .p
.c.p:
		$(CC_CMD) -G $<
		mv $(OUT)/$(*F).p $(<D)
PFILES	=	$M/cpp.p $O/cpy.p $M/yylex.p $M/mac.p
protos:		$(PFILES)
			echo "#if defined __STDC__" >> $M/ftnproto.h
			cat $(PFILES) >> $M/ftnproto.h
			echo "#endif /* defined __STDC__ */" >> $M/ftnproto.h
#			rm -f $(PFILES)
############################################################
#
#
#	Files making up cpp
#
OBJECTS	= $O/cpp.o $O/cpy.o $O/rodata.o $O/yylex.o $O/mac.o
SOURCE	= $M/cpy.y $M/yylex.c $M/cpp.c $M/mac.c
#
build:	all
#
all:	$O/cpp2  clean $O/cpp4
	if [ "$(MACH)" = "M68020" ];	\
	then				\
	     ln $O/cpp2 $O/cpp;		\
	else				\
	     ln $O/cpp4 $O/cpp;		\
	fi

#

$O/cpp:	$(OBJECTS)
	$(CC) $(CFLAGS) -o $O/cpp $(OBJECTS) $(LIBES) $(LDFLAGS)

$O/cpp2:
	 $(MAKE) -$(MAKEFLAGS)f mach.mk MACH=M68020 NOTMACH=M68040 $(O)/cpp
	 mv $O/cpp $O/cpp2

$O/cpp4:
	 $(MAKE) -$(MAKEFLAGS)f mach.mk MACH=M68040 NOTMACH=M68020 $(O)/cpp
	 mv $O/cpp $O/cpp4
#
$D/y.tab.h $O/cpy.c : $M/cpy.y
	$(YACC) $(YFLAGS) $M/cpy.y
	$(SH) $M/:yyfix >$O/rodata.c; 
	$(CC_CMD) -c $O/rodata.c
	 mv y.tab.c $O/cpy.c
#
$O/cpy.o:	$O/cpy.c
	$(CC_CMD) -c $O/cpy.c
#
$O/yylex.o: $M/yylex.c $O/cpy.o 
	$(CC_CMD) $M/yylex.c
	
#
$O/cpp.o: $M/cpp.c $M/mac.h
	$(CC_CMD) $M/cpp.c
#
$O/mac.o: $M/mac.c $M/mac.h
	$(CC_CMD) $M/mac.c
#
#$O/rodata.o: $O/cpy.o
#	 $(CC_CMD) -c $O/rodata.c

$O/rodata.o:

#
clean :
	$(RM) -f $O/cpp $O/cpy.c $D/y.tab.h $O/rodata.c $O/rodata.s $(OBJECTS)
#
clobber: clean
	$(RM) -f $O/cpp $O/cpp2 $O/cpp4 tags greplist
#
install : all
	-rm -f $(LIBDIR)/$(SGS)cpp $(LIBDIR)/$(SGS)cpp2 $(LIBDIR)/$(SGS)cpp4 
	$(INS) $O/cpp $(LIBDIR)/$(SGS)cpp
	chmod $(MODE) $(LIBDIR)/$(SGS)cpp
	chgrp $(GRP) $(LIBDIR)/$(SGS)cpp
	chown $(OWN) $(LIBDIR)/$(SGS)cpp
	$(INS) $O/cpp2 $(LIBDIR)/$(SGS)cpp2
	chmod $(MODE) $(LIBDIR)/$(SGS)cpp2
	chgrp $(GRP) $(LIBDIR)/$(SGS)cpp2
	chown $(OWN) $(LIBDIR)/$(SGS)cpp2
	$(INS) $O/cpp4 $(LIBDIR)/$(SGS)cpp4
	chmod $(MODE) $(LIBDIR)/$(SGS)cpp4
	chgrp $(GRP) $(LIBDIR)/$(SGS)cpp4
	chown $(OWN) $(LIBDIR)/$(SGS)cpp
#
save:
	$(RM) -f $(LIBDIR)/$(SGS)cpp.back
	$(CP) $(LIBDIR)/$(SGS)cpp $(LIBDIR)/$(SGS)cpp.back
#
uninstall:	$(LIBDIR)/$(SGS)cpp.back
	$(RM) -f $(LIBDIR)/$(SGS)cpp
	$(CP) $(LIBDIR)/$(SGS)cpp.back $(LIBDIR)/$(SGS)cpp
#
lint:	$(SOURCE)
	$(LINT) $(LINTFLAGS) $(DEFLIST) $(SOURCE)
#
listing:	$(SOURCE) 
	$(PRINT) $(PRFLAGS) $(SOURCE) $M/:yyfix $M/:rofix | $(LP)
#

SOURCES=$(SOURCE)
ctags:
	ctags $(SOURCES)
	echo 'grep $$* \\' >greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
