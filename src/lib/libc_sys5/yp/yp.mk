#ident	"@(#)yp.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#
#	Copyright 1986 Lachman Associates Inc. and The Instruction Set Ltd.
#
#      @(#)makefile	2.2 LAI KNFS for ARETE  source
#      @(#)makefile	2.7 System V NFS  source
include $(IROOT)/macdefs
include $(IROOT)/sgsdefs
FILES = yp_bind.o \
	ypprot_err.o \
	yperr_str.o \
	yp_enum.o \
	yp_match.o \
	ypxdr.o \
	yp_all.o \
	yp_master.o \
	yp_order.o \
	ypv1_xdr.o

SOURCE = yp_bind.c \
	ypprot_err.c \
	yperr_str.c \
	yp_enum.c \
	yp_match.c \
	ypxdr.c \
	yp_all.c \
	yp_master.c \
	yp_order.c \
	ypv1_xdr.c

all: $(FILES) 

yp_bind.o:	yp_bind.c 
	$(CC) $(CFLAGS) -c yp_bind.c
ypprot_err.o:	 ypprot_err.c 
	$(CC) $(CFLAGS) -c ypprot_err.c
yperr_str.o :	yperr_str.c 
	$(CC) $(CFLAGS) -c yperr_str.c
yp_enum.o: 	yp_enum.c 
	$(CC) $(CFLAGS) -c yp_enum.c
yp_match.o:	 yp_match.c 
	$(CC) $(CFLAGS) -c yp_match.c
ypxdr.o:	 ypxdr.c 
	$(CC) $(CFLAGS) -c ypxdr.c
yp_all.o:	yp_all.c 
	$(CC) $(CFLAGS) -c yp_all.c
yp_master.o:	yp_master.c 
	$(CC) $(CFLAGS) -c yp_master.c
yp_order.o:	yp_order.c 
	$(CC) $(CFLAGS) -c yp_order.c
ypv1_xdr.o:	ypv1_xdr.c
	$(CC) $(CFLAGS) -c ypv1_xdr.c

install: all


clean:
	-rm -f *.o

clobber:	clean
