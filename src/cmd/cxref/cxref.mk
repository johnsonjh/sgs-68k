#	START NEW ARIX SCCS HEADER
#
#	@(#) cxref.mk: version 25.1 created on 12/2/91 at 14:11:53
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)cxref.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#ident	"@(#)cxref:cxref.mk	25.1"

include	$(IROOT)/macdefs
FLG=
OWNER=$(ROOT)/usr/lib
# comp and other contain files shared with C preprocessor, lint and comp
M=./comp
L=./other
CPP=./other
INCS=-I. -I$M -I$L
DEFS=-DCXREF -DFLEXNAMES
LINTF = -p
LINT = lint
OPRL = oprl
YACCF = -d
LFLAG1 =
LFLAG2 =
CFLAG = -O -c $(DEFS) $(INCS) $(CFLAGS)
CPASS1 =	cgram.c $(M)/comm1.c $(M)/optim.c $(M)/pftn.c $(M)/scan.c $(M)/trees.c \
		$(M)/xdefs.c xlocal.c lint.c $(M)/messages.c
OPASS1 =	cgram.o comm1.o optim.o pftn.o scan.o trees.o \
		xdefs.o xlocal.o lint.o messages.o

XPASS =	$(M)/cgram.y $(M)/comm1.c $(M)/common lint.c $(L)/lmanifest macdefs \
	$(M)/manifest $(M)/mfile1 $(M)/optim.c $(M)/pftn.c $(M)/scan.c $(M)/trees.c \
	$(M)/xdefs.c xlocal.c $(M)/messages.c $(M)/messages.h

all :	chk_pdp cxref xpass xcpp

chk_pdp :
	if pdp11 && [ "$(FLG)" != "x" ]; then \
		$(MAKE) -$(MAKEFLAGS) FLG=x DEFS="$(DEFS) -UFLEXNAME" \
			M="$(M)" L="$(L)" \
			CPP="$(CPP)" CC="$(CC)" \
			INCS="$(INCS)" -f cxref.mk all; \
	fi

# CXREF

cxref :	cxr.o
	$(CC) $(FFLAG)  cxr.o -o cxref $(LDFLAGS)

cxr.o:	cxr.c owner.h
	$(CC) $(CFLAG) cxr.c

# XPASS

xpass:	$(OPASS1)
	$(CC) $(FFLAG) $(LFLAG1)  $(OPASS1) -o xpass $(LDFLAGS)

$(OPASS1): $(M)/manifest macdefs $(M)/mfile1

cgram.c: $(M)/cgram.y
	sed -e 's/\/\*CXREF\(.*\)\*\//\1/' $(M)/cgram.y > gram.y
	$(YACC) gram.y
	mv y.tab.c cgram.c
	-rm -f gram.y

cgram.o:	cgram.c
	$(CC) -DBUG4 $(FFLAG) $(CFLAG) cgram.c

comm1.o: $(M)/common
	$(CC) -DBUG4 $(FFLAG) $(CFLAG) $(M)/comm1.c

lint.o:	$(L)/lmanifest lint.c
	$(CC) $(FFLAG) $(CFLAG) lint.c
	
optim.o:	$(M)/optim.c
	$(CC) -DBUG4 $(FFLAG) $(CFLAG) $(M)/optim.c
	
pftn.o:		$(M)/pftn.c
	$(CC) -DBUG4 $(FFLAG) $(CFLAG) $(M)/pftn.c
	
scan.o: $(M)/scan.c
	$(CC) -DBUG4 $(FFLAG) $(CFLAG) $(M)/scan.c

trees.o:	$(M)/trees.c
	$(CC) -DBUG4 $(FFLAG) $(CFLAG) $(M)/trees.c

xdefs.o: $(M)/xdefs.c
	$(CC) -DBUG4 $(FFLAG) $(CFLAG) $(M)/xdefs.c
	
xlocal.o:	xlocal.c $(L)/lmanifest
	$(CC) $(FFLAG) $(CFLAG) xlocal.c

messages.o:	$(M)/messages.c $(M)/messages.h
	$(CC) $(FFLAG) $(CFLAG) $(M)/messages.c

# XCPP

xcpp:	cpp.o cpy.o yylex.o
	$(CC) $(FFLAG) $(LFLAG2)  -o xcpp cpp.o cpy.o yylex.o $(LDFLAGS)

cpp.o:	$(CPP)/cpp.c
	$(CC) $(FFLAG) $(CFLAG) -I$(CPP) -DPD_MY_MACH=\"$(MACH)\" \
		-DPD_MACH=D_$(MACH) -DPD_MY_FAM=\"$(FAMILY)\" \
		-DPD_FAM=D_$(FAMILY) -DPD_SYS=D_unix $(CPP)/cpp.c

cpy.o:	cpy.c $(CPP)/yylex.c
	$(CC) $(FFLAG) $(CFLAG) -I$(CPP) -Dunix=1 cpy.c

yylex.o:	$(CPP)/yylex.c y.tab.h
	$(CC) $(CFLAG) -I$(CPP) $(CPP)/yylex.c

cpy.c:	$(CPP)/cpy.y
	$(YACC) $(YACCF) $(CPP)/cpy.y
	mv y.tab.c cpy.c

# UTILITIES

#
install :	all
	$(INS) cxref $(ROOT)/usr/bin/cxref
	chmod 755 $(ROOT)/usr/bin/cxref
	chgrp bin $(ROOT)/usr/bin/cxref
	chown bin $(ROOT)/usr/bin/cxref
	$(INS) xpass $(OWNER)/xpass
	chmod 755 $(OWNER)/xpass
	chgrp bin $(OWNER)/xpass
	chown bin $(OWNER)/xpass
	$(INS) xcpp $(OWNER)/xcpp
	chmod 755 $(OWNER)/xcpp
	chgrp bin $(OWNER)/xcpp
	chown bin $(OWNER)/xcpp

clean:
	-rm -f *.o  

clobber:	clean
	-rm -f xpass cgram.c cxref xcpp cpy.c y.tab.h
	-rm -f greplist tags

lint:
	$(LINT) $(LINTF) cxr.c
	$(LINT) $(LINTF) -DBUG4 $(CPASS1)
	$(LINT) $(LINTF) -Dunix=1 $(CPP)/cpp.c cpy.c
SOURCES = $(CPASS1) $(XPASS)
ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
