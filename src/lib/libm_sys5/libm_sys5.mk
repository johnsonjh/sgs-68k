#ident	"@(#)libm_sys5.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT: ident	"libm:libm.mk	1.17"

#
# makefile for libm
#
# Note that the object list given in FILES is in the order in which the files
# should be added to the archive when it is created.  If the files are not
# ordered correctly, linking of libm fails on the pdp11 and is slowed down
# on other machines.

IROOT =
PROF  =
include $(IROOT)/macdefs
include $(IROOT)/sgsdefs
INC=$(INCRT)
LIB=$(ROOT)/$(SGS)lib
LIBP=$(ROOT)/$(SGS)lib/libp
USRLIB=$(ROOT)/usr/$(SGS)lib
VARIANT = 
LIBNAME = lib$(VARIANT)m.a
LIBNAME2 = lib$(VARIANT)me.a


M4= m4 $(M4FLAGS) ./m4.def

FILES =\
	$(LIBNAME)(asin.o)\
	$(LIBNAME)(atan.o)\
	$(LIBNAME)(erf.o)\
	$(LIBNAME)(fabs.o)\
	$(LIBNAME)(floor.o)\
	$(LIBNAME)(fmod.o)\
	$(LIBNAME)(gamma.o)\
	$(LIBNAME)(hypot.o)\
	$(LIBNAME)(jn.o)\
	$(LIBNAME)(j0.o)\
	$(LIBNAME)(j1.o)\
	$(LIBNAME)(pow.o)\
	$(LIBNAME)(log.o)\
	$(LIBNAME)(sin.o)\
	$(LIBNAME)(sinh.o)\
	$(LIBNAME)(sqrt.o)\
	$(LIBNAME)(tan.o)\
	$(LIBNAME)(tanh.o)\
	$(LIBNAME)(exp.o)\
	$(LIBNAME)(asin881.o)\
	$(LIBNAME)(atan881.o)\
	$(LIBNAME)(etox881.o)\
	$(LIBNAME)(log881.o)\
	$(LIBNAME)(mul881.o)\
	$(LIBNAME)(nan881.o)\
	$(LIBNAME)(sin881.o)\
	$(LIBNAME)(sinh881.o)\
	$(LIBNAME)(sqrt881.o)\
	$(LIBNAME)(tan881.o)\
	$(LIBNAME)(tanh881.o)\
	$(LIBNAME)(matherr.o)

all: $(LIBNAME)

$(LIBNAME): $(FILES)

.s.a:
	$(M4) -UEMU $*.s > $*.m4.s
	$(CC) $(CFLAGS) -c $*.m4.s \
		&& mv $*.m4.o $*.o
	$(AR) rv $@ $*.o
	$(PROF)$(M4) -UEMU $*.s > $*.m4.s
	$(PROF)$(CC) -p $(CFLAGS) -c $*.m4.s && mv $*.m4.o $*.o
	$(PROF)$(AR) rv libp.$@ $*.o
	rm $*.o
	$(M4) -DEMU $*.s > $*.m4.s
	$(CC) $(CFLAGS) -c $*.m4.s && mv $*.m4.o $*.o
	$(AR) rv $(LIBNAME2) $*.o
	$(PROF)$(M4) -DEMU $*.s > $*.m4.s
	$(PROF)$(CC) -p $(CFLAGS) -c $*.m4.s && mv $*.m4.o $*.o
	$(PROF)$(AR) rv libp.$(LIBNAME2) $*.o
	rm $*.o $*.m4.s

.c.a:
	$(CC) -I. $(CFLAGS) -c $*.c
	$(AR) rv $@ $*.o
	$(AR) rv $(LIBNAME2) $*.o
	$(PROF)$(CC) -p $(CFLAGS) -c $*.c
	$(PROF)$(AR) rv libp.$@ $*.o
	$(PROF)$(AR) rv libp.$(LIBNAME2) $*.o
	rm $*.o

$(FILES): $(INC)/math.h $(INC)/values.h $(INC)/errno.h \
		$(INC)/sys/errno.h

install: all
	$(INS) $(LIBNAME) $(LIB)/$(LIBNAME)
	$(AR) ts $(LIB)/$(LIBNAME) > /dev/null
	chmod 644 $(LIB)/$(LIBNAME)
	chgrp bin $(LIB)/$(LIBNAME)
	chown bin $(LIB)/$(LIBNAME)
	$(PROF)$(INS) libp.$(LIBNAME) $(LIBP)/$(LIBNAME)
	$(PROF)$(AR) ts $(LIBP)/$(LIBNAME) > /dev/null
	$(PROF)chmod 644 $(LIBP)/$(LIBNAME)
	$(PROF)chgrp bin $(LIBP)/$(LIBNAME)
	$(PROF)chown bin $(LIBP)/$(LIBNAME)
	$(INS) $(LIBNAME2) $(LIB)/$(LIBNAME2)
	$(AR) ts $(LIB)/$(LIBNAME2) > /dev/null
	chmod 644 $(LIB)/$(LIBNAME2)
	chgrp bin $(LIB)/$(LIBNAME2)
	chown bin $(LIB)/$(LIBNAME2)
	$(PROF)$(INS) libp.$(LIBNAME2) $(LIBP)/$(LIBNAME2)
	$(PROF)$(AR) ts $(LIBP)/$(LIBNAME2) > /dev/null
	$(PROF)chmod 644 $(LIBP)/$(LIBNAME2)
	$(PROF)chgrp bin $(LIBP)/$(LIBNAME2)
	$(PROF)chown bin $(LIBP)/$(LIBNAME2)

clean:
	-rm -f *.o

clobber:	clean
	-rm -f $(LIBNAME) libp.$(LIBNAME) $(LIBNAME2) libp.$(LIBNAME2) greplist tags

SOURCES= \
exp.c       gamma.c     jn.c        matherr.c   sinh.c		\
asin.c      fabs.c      hypot.c     sqrt.c		\
atan.c      floor.c     j0.c        pow.c       tan.c		\
erf.c       fmod.c      j1.c        log.c       sin.c       tanh.c

ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
