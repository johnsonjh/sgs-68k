#	START NEW ARIX SCCS HEADER
#
#	@(#) pcc.mk: version 25.1 created on 12/2/91 at 17:38:55
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)pcc.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#		Copyright (c) 1985 AT&T		#
#		All Rights Reserved		#
#	pcc.mk	7.1	

#ident	"@(#)f77/f77/mach:pcc.mk	25.1"

# POSIX

SVID_FLAG =

TESTDIR = .
INS = /etc/install
INSDIR =
FRC =
FORTDEF=-DSPFORT
INSTALL=nccom
FINSTALL=f1
M=/usr/src/cmd/cc/pcc/mip
P=/usr/src/cmd/cc/pcc/vax
all: comp fort
comp: rodata.o cgram.o xdefs.o scan.o pftn.o trees.o optim.o code.o local.o \
	reader.o local2.o order.o match.o allo.o comm1.o table.o messages.o
	$(CC) -i rodata.o cgram.o xdefs.o scan.o pftn.o trees.o \
		optim.o code.o local.o reader.o local2.o order.o match.o \
		allo.o comm1.o table.o messages.o -o $(TESTDIR)/comp $(LDFLAGS)
trees.o: $M/manifest $P/macdefs $M/mfile1 $M/messages.h $M/trees.c
	$(CC) -c $(CFLAGS) -I$M -I.  $M/trees.c
optim.o: $M/manifest $P/macdefs $M/mfile1 $M/optim.c
	$(CC) -c $(CFLAGS) -I$M -I. $M/optim.c
pftn.o: $M/manifest $P/macdefs $M/mfile1 $M/messages.h $M/pftn.c
	$(CC) -c $(CFLAGS) -I$M -I. $M/pftn.c
code.o: $M/manifest $P/macdefs $M/mfile1
	$(CC) -c $(CFLAGS) -I$M -I. code.c
local.o: $M/manifest $P/macdefs $M/mfile1
	$(CC) -c $(CFLAGS) -I$M -I. local.c
scan.o: $M/manifest $P/macdefs $M/mfile1 $M/messages.h $M/scan.c
	$(CC) -c $(CFLAGS) -I$M -I. $M/scan.c
xdefs.o: $M/manifest $M/mfile1 $P/macdefs $M/xdefs.c
	$(CC) -c $(CFLAGS) -I$M -I. $M/xdefs.c
cgram.o: $M/manifest $M/mfile1 $P/macdefs $M/messages.h $M/cgram.c
#	$(CC) -c $(CFLAGS) -I. -I$M $M/cgram.c
	$(CC) -c $(CFLAGS) -DYYMAXDEPTH=300 -I. -I$M $M/cgram.c
rodata.o $M/cgram.c: $M/cgram.y
	$(YACC) $M/cgram.y
	rm -f rodata.c
	./:yyfix yyexca yyact yypact yypgo yyr1 yyr2 yychk yydef
	mv y.tab.c $M/cgram.c
	$(CC) $(CFLAGS) -S rodata.c
	./:rofix rodata.s
	as -o rodata.o rodata.s
	rm -f rodata.s rodata.c
comm1.o: $M/manifest $M/mfile1 $M/common $P/macdefs $M/comm1.c
	$(CC) -c $(CFLAGS) -I. -I$M $M/comm1.c
table.o: $M/manifest $M/mfile2 $P/mac2defs $P/macdefs $P/table.c
	$(CC) $(CFLAGS) -S -I$M -I. table.c
	chmod +x ./:rofix
	./:rofix table.s
	as -o table.o table.s
	rm -f table.s
messages.o: $M/messages.c $M/messages.h
	$(CC) $(CFLAGS) -S -I$M -I. $M/messages.c
	./:rofix messages.s
	as -o messages.o messages.s
	rm -f messages.s
reader.o: $M/manifest $M/mfile2 $P/mac2defs $P/macdefs $M/messages.h $M/reader.c
	$(CC) -c $(CFLAGS) -I$M -I. $M/reader.c
local2.o: $M/manifest $M/mfile2 $P/mac2defs $P/macdefs $P/local2.c
	$(CC) -c $(CFLAGS) -I$M -I. $P/local2.c
order.o: $M/manifest $M/mfile2 $P/mac2defs $P/macdefs $P/order.c
	$(CC) -c $(CFLAGS) -I$M -I. $P/order.c
match.o: $M/manifest $M/mfile2 $P/mac2defs $P/macdefs $M/match.c
	$(CC) -c $(CFLAGS) -I$M -I. $M/match.c
allo.o: $M/manifest $M/mfile2 $P/mac2defs $P/macdefs $M/allo.c
	$(CC) -c $(CFLAGS) -I$M -I. $M/allo.c
shrink:
	rm *.o comp
clean:
	rm -f *.o
clobber: clean
	rm -f $(TESTDIR)/fort $(TESTDIR)/comp $(TESTDIR)/f1
lintall:
	lint -hpv -I. -I$M  $M/cgram.c $M/xdefs.c $M/scan.c $M/pftn.c \
		$M/trees.c $M/optim.c code.c local.c $M/reader \
		local2.c order.c $M/match.c $M/allo.c $M/comm1.c table.c
fort: fort.o freader.o fallo.o fmatch.o ftable.o forder.o flocal2.o \
	fcomm2.o
	$(CC) -i $(CFLAGS) fort.o freader.o fallo.o fmatch.o ftable.o \
		forder.o flocal2.o fcomm2.o -o $(TESTDIR)/fort
fort.o: fort.h fort.c
	$(CC) -c $(CFLAGS) $(FORTDEF) -I$(M) -I. -I$(P) fort.c
freader.o: freader.c
	$(CC) -c $(CFLAGS) $(FORTDEF) -I$(M) -I. -I$(P) freader.c
fallo.o: fallo.c
	$(CC) -c $(CFLAGS) $(FORTDEF) -I$(M) -I. -I$(P) fallo.c
fmatch.o: fmatch.c
	$(CC) -c $(CFLAGS) $(FORTDEF) -I$(M) -I. -I$(P) fmatch.c
ftable.o: ftable.c
	$(CC) -S $(CFLAGS) $(FORTDEF) -I$(M) -I. -I$(P) ftable.c
	./:rofix ftable.s
	$(CC) -c ftable.s
	rm -f ftable.s
forder.o: forder.c
	$(CC) -c $(CFLAGS) $(FORTDEF) -I$(M) -I. -I$(P) forder.c
flocal2.o: flocal2.c
	$(CC) -c $(CFLAGS) $(FORTDEF) -I$(M) -I. -I$(P) flocal2.c
fcomm2.o: $(M)/common
	$(CC) -c $(CFLAGS) $(FORTDEF) -I$(M) -I. -I$(P) fcomm2.c
fort.o freader.o fallo.o fmatch.o ftable.o forder.o flocal2.o fcomm2.o: \
	$M/mfile2 $M/manifest $P/macdefs $P/mac2defs
install:	all
	ln $(TESTDIR)/comp $(INSTALL)
	rm -f /lib/nccom
	cp /lib/ccom /lib/nccom
	$(INS) -i $(INSTALL) $(DESTDIR)/lib
	rm $(INSTALL)
	ln $(TESTDIR)/fort $(FINSTALL)
	rm -f /lib/nf1
	cp /lib/f1 /lib/nf1
	$(INS) -i $(FINSTALL) $(DESTDIR)/lib
	rm $(FINSTALL)

finstall:	fort
	mv $(TESTDIR)/fort $(TESTDIR)/f1
	$(INS) -f /lib $(TESTDIR)/f1
