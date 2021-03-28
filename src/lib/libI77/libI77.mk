#ident	"@(#)libI77.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT: ident	"libI77:libI77.mk	1.13"

# F77 I/O library (libI77.a) makefile

IROOT =
PROF=
include $(IROOT)/macdefs

RM=/bin/rm
STRIP = strip
LIB=$(ROOT)/$(SGS)lib
LIBP=$(ROOT)/$(SGS)lib/libp
VARIANT = I77
LIBNAME = lib$(VARIANT).a


OBJ =	$(LIBNAME)(Version.o) \
	$(LIBNAME)(backspace.o) \
	$(LIBNAME)(dfe.o) \
	$(LIBNAME)(due.o) \
	$(LIBNAME)(iio.o) \
	$(LIBNAME)(inquire.o) \
	$(LIBNAME)(rewind.o) \
	$(LIBNAME)(rsfe.o) \
	$(LIBNAME)(rdfmt.o) \
	$(LIBNAME)(sue.o) \
	$(LIBNAME)(uio.o) \
	$(LIBNAME)(wsfe.o) \
	$(LIBNAME)(sfe.o) \
	$(LIBNAME)(fmt.o) \
	$(LIBNAME)(nio.o) \
	$(LIBNAME)(lio.o) \
	$(LIBNAME)(lread.o) \
	$(LIBNAME)(open.o) \
	$(LIBNAME)(close.o) \
	$(LIBNAME)(util.o) \
	$(LIBNAME)(endfile.o) \
	$(LIBNAME)(wrtfmt.o) \
	$(LIBNAME)(err.o) \
	$(LIBNAME)(fmtlib.o) \
	$(LIBNAME)(ecvt.o) \
	$(LIBNAME)(ltostr.o)
.c.o:;

all: $(LIBNAME)

$(LIBNAME):	$(OBJ)
		$(CC) -c $(CFLAGS) $(?:.o=.c)
		$(AR) r $(LIBNAME) $?
		$(PROF)$(CC) -p -c $(CFLAGS) $(?:.o=.c)
		$(PROF)$(AR) r libp.$(LIBNAME) $?
		$(RM) $?

$(OBJ): lio.h fio.h fmt.h

.c.a:;

install: all
	$(INS) $(LIBNAME) $(LIB)/$(LIBNAME)
	$(AR) ts $(LIB)/$(LIBNAME) >/dev/null
	chmod	644 $(LIB)/$(LIBNAME)
	chgrp	bin $(LIB)/$(LIBNAME)
	chown	bin $(LIB)/$(LIBNAME)
	$(PROF)$(INS) libp.$(LIBNAME) $(LIBP)/$(LIBNAME)
	$(PROF)$(AR) ts $(LIBP)/$(LIBNAME) >/dev/null
	$(PROF)chmod	644 $(LIBP)/$(LIBNAME)
	$(PROF)chgrp	bin $(LIBP)/$(LIBNAME)
	$(PROF)chown	bin $(LIBP)/$(LIBNAME)

clean:
	-rm -f *.o

clobber:	clean
	-rm -f $(LIBNAME) libp.$(LIBNAME) greplist tags

SOURCES= \
	ecvt.c         inquire.c      ltostr.c       sue.c\
	endfile.c      lib.c          nio.c          \
	Version.c      err.c          uio.c\
	backspace.c    fio.h          open.c         util.c\
	close.c        fmt.c          rdfmt.c        wrtfmt.c\
	ctest.c        fmt.h          rewind.c       wsfe.c\
	dballoc.c      fmtlib.c       lio.c          rsfe.c\
	dfe.c          ftest.c        lio.h          sfe.c\
	due.c          iio.c          lread.c        stest.c

ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist

