#	START NEW ARIX SCCS HEADER
#
#	@(#) error.mk: version 25.1 created on 12/2/91 at 17:35:04
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)error.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#
# Copyright (c) 1980 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#
#	@(#)Makefile	5.1 (Berkeley) 5/31/85
#
#
#	error.h			error header file
#	errorinput.c		does all of the input, and canonicalizes errors
#	errorpi.c		canonicalizes pi errors
#	errormain.c		main
#	errorsubr.c		does grotty subroutine work
#	errorfltr.c		"greps" out unwanted messages
#	errortouch.c		touches all mentioned files
#
include $(IROOT)/macdefs
include $(IROOT)/sgsdefs

BINDIR = $(ROOT)/bin
HDRS = error.h
PHDRS = $(HDRS)
SRCS = 	errormain.c errorinput.c errorpi.c errorsubr.c \
	errorfltr.c errortouch.c
PSRCS = $(SRCS)
OBJS = 	errormain.o errorinput.o errorpi.o errorsubr.o \
	errorfltr.o errortouch.o

DFLAGS = -DDEBUG

LINTFLAGS = $(DFLAGS)

all:	error

error:	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o error

$(OBJS): $(HDRS)

install: all
	-rm -f $(BINDIR)/error
	$(INS) error $(BINDIR)/error
	chmod 755 $(BINDIR)/error
	chown bin $(BINDIR)/error
	chgrp bin $(BINDIR)/error

clean:
	rm -f error $(OBJS)

clobber:	clean

lint:	
	lint $(LINTFLAGS) $(SRCS)

psrcs:
	echo $(PHDRS) $(PSRCS)

print:
	print $(PHDRS) $(PSRCS)
