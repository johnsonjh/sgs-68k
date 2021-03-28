#ident	"@(#)net.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#
#	Copyright 1986 Lachman Associates Inc. and The Instruction Set Ltd.
#
#      @(#)makefile	2.2 LAI KNFS for ARETE  source
#      @(#)makefile	2.7 System V NFS  source
include $(IROOT)/macdefs
include $(IROOT)/sgsdefs
FILES = getnetent.o gethostent.o getrpcent.o innetgr.o\
	gethostnm.o  getntgrent.o getprtoent.o getservent.o 
SOURCE = getnetent.c gethostent.c getrpcent.c innetgr.c \
	gethostnm.c  getntgrent.c getprtoent.c getservent.c 
#	inet_netwk.o \
#	inet_lnaof.o \
#	inet_mkadr.o

all: $(FILES) 

getnetent.o:	getnetent.c 
	$(CC) $(CFLAGS) -c 	getnetent.c 
gethostent.o:	gethostent.c 
	$(CC) $(CFLAGS) -c	gethostent.c 
getrpcent.o:	getrpcent.c 
	$(CC) $(CFLAGS) -c	getrpcent.c 
innetgr.o:	innetgr.c 
	$(CC) $(CFLAGS) -c	innetgr.c 
gethostnm.o:	gethostnm.c  
	$(CC) $(CFLAGS) -c	gethostnm.c 
getntgrent.o:	getntgrent.c 
	$(CC) $(CFLAGS) -c	getntgrent.c 
getprtoent.o:	getprtoent.c 
	$(CC) $(CFLAGS) -c	getprtoent.c 
getservent.o:	getservent.c 
	$(CC) $(CFLAGS) -c	getservent.c 

install: all 

clean:
	-rm -f *.o

clobber:	clean
