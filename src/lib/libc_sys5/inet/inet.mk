#ident	"@(#)inet.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#
#	Copyright 1986 Lachman Associates Inc. and The Instruction Set Ltd.
#
#      @(#)makefile	2.2 LAI KNFS for ARETE  source
#      @(#)makefile	2.7 System V NFS  source
include $(IROOT)/macdefs
include $(IROOT)/sgsdefs
FILES = inet_mkadr.o  inet_addr.o  inet_netof.o \
	inet_ntoa.o inet_netwk.o  inet_lnaof.o 

SOURCE = inet_mkadr.c inet_addr.c inet_netof.c \
	inet_ntoa.c inet_netwk.c  inet_lnaof.c 

all: $(FILES) 

inet_mkadr.o:	inet_mkadr.c 
	$(CC) $(CFLAGS) -c	inet_mkadr.c 
inet_addr.o:	inet_addr.c 
	$(CC) $(CFLAGS) -c	inet_addr.c 
inet_netof.o:	inet_netof.c 
	$(CC) $(CFLAGS) -c	inet_netof.c 
inet_ntoa.o:	inet_ntoa.c 
	$(CC) $(CFLAGS) -c	inet_ntoa.c 
inet_netwk.o:	inet_netwk.c  
	$(CC) $(CFLAGS) -c	inet_netwk.c 
inet_lnaof.o:	inet_lnaof.c 
	$(CC) $(CFLAGS) -c	inet_lnaof.c 

install: all 

clean:
	-rm -f *.o

clobber:	clean
