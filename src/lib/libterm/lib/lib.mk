#	START NEW ARIX SCCS HEADER
#
#	@(#) lib.mk: version 25.1 created on 12/2/91 at 20:15:04
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)lib.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#	@(#)lib.mk	1.2


SROOT =
IROOT =
include $(IROOT)/macdefs

CFLAG= -I. -DCM_N -DCM_GT -DCM_B -DCM_D $(CFLAGS)
INSLIB=$(ROOT)/usr/$(SGS)lib
OFILES=termcap.o tgoto.o tputs.o

all:		libtermcap.a

libtermcap.a:	$(OFILES)
	$(AR) cr libtermcap.a $(OFILES)

termcap.o:	../local/uparm.h
	$(CC) $(CFLAG) -c $*.c

.c.o:
	$(CC) $(CFLAG) -c $*.c

install:	all
	$(INS) libtermcap.a $(INSLIB)/libtermcap.a
	$(AR) ts $(INSLIB)/libtermcap.a > /dev/null
	-chmod 644 $(INSLIB)/libtermcap.a
	-chown bin $(INSLIB)/libtermcap.a
	-chgrp bin $(INSLIB)/libtermcap.a
	-rm -f $(INSLIB)/libtermlib.a
	-ln $(INSLIB)/libtermcap.a $(INSLIB)/libtermlib.a

clean:
	-rm -f *.o

clobber:	clean
	-rm -f libtermcap.a

