#	START NEW ARIX SCCS HEADER
#
#	@(#) libnls.mk: version 25.1 created on 12/2/91 at 19:41:07
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)libnls.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	AT&T: #ident	"listen:listen.mk	1.14.2.1"

#ident	"@(#)listen:listen.mk	23.1"

#
# listen.mk:
# makefile for network listener - System V release 3.1
#

IROOT =
SROOT =
include $(IROOT)/macdefs

# INC	= $(ROOT)/usr/include
# OPT	= -O
# CFLAGS	= -Um68k -Um68 -Umc68k -US4 -Uvax  -UMICROOMS -Updp11 -Uu3b -Du3b2 -I$(INC) ${OPT}
# LDFLAGS	= $(LLDFLAGS)
LIBNLS = libnls.a
LDLIBS = -lnsl_s
LSTINC	= .

# change the next two lines to compile with -g
# OPT	= -g
# LLDFLAGS= -s
OPTION	= -DDEBUGMODE

LIBDIR = $(ROOT)/usr/lib

LSUID	= 81
LSGID	= adm
LIBID	= bin

# LLDLIBS	=  $(LIBNLS)


# The DEBUG module can always be included...
# if DEBUGMODE is undefined, no code gets compiled.
# doprnt.o is from system V rel 5.0.5.

LIBSRC = \
	nlsenv.c \
	nlsrequest.c

NLSOBJS = \
	$(LIBNLS)(nlsenv.o) \
	$(LIBNLS)(nlsrequest.o)

all:	libnls

#
# SHAREDLIB version
#

libnls:	$(LIBNLS)

$(LIBNLS):	$(NLSOBJS)

.PRECIOUS:	$(LIBNLS)

# listener library routines and /usr/include headers:
# putenv.o should only be included on an S4. (not included in s4's libc.a)
# In addition, nlsconnect.o, nlsestab.o, and nlsname.o are only on the S4.

$(LIBNLS)(nlsenv.o): nlsenv.c
$(LIBNLS)(nlsrequest.o): $(LSTINC)/listen.h nlsrequest.c

install:	all
		-rm -f $(LIBDIR)/$(LIBNLS)
		$(INS) $(LIBNLS) $(LIBDIR)/$(LIBNLS)
		$(AR) ts $(LIBDIR)/$(LIBNLS) > /dev/null
		chmod 644 $(LIBDIR)/$(LIBNLS)
		chgrp $(LSGID) $(LIBDIR)/$(LIBNLS)
		chown $(LSUID) $(LIBDIR)/$(LIBNLS)

ctags:
	ctags	$(SOURCE)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCE)		;\
	do echo "$$i \\" >> greplist	;\
	done
	echo /dev/null >> greplist
	chmod +x greplist

clean:
	-rm -f *.o

clobber: clean
	-rm -f libnls.a

FRC:
