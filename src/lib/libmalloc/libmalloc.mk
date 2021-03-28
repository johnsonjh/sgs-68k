#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT: ident	"libmalloc:libmalloc.mk	1.18"
#
# makefile for libmalloc
#

.SUFFIXES: .p
IROOT =
PROF  =
include $(IROOT)/macdefs

ENV=

INC=$(INCRT)
LIB=$(ROOT)/$(SGS)lib
USRLIB= $(ROOT)/usr/$(SGS)lib
LIBP= $(ROOT)/$(SGS)lib/libp
LINT=lint
LINTFLAGS=-p

ARFLAGS=r

CP=cp
MV=mv
RM=rm

OBJECTS=malloc.o
POBJECTS=malloc.p

#variables used to build lint library in 5.0 compatible mode
LINT1=/usr/lib/lint1
LINT2=/usr/lib/lint2

TMPDIR=/usr/tmp
FRC=

all: noprof prof llib-lmalloc.l

noprof: $(OBJECTS)
	$(RM) -rf objects
	mkdir objects
	$(CP) $(OBJECTS) objects
	cd objects; $(AR) $(ARFLAGS) libmalloc.a *

prof:	$(POBJECTS)
	$(PROF)$(RM) -rf pobjects
	$(PROF)mkdir pobjects
	$(PROF)for i in $(POBJECTS);\
	do\
		$(PROF)$(CP) $$i pobjects/`basename $$i .p`.o;\
	$(PROF)done
	$(PROF)cd pobjects; $(AR) $(ARFLAGS) libmalloc.a *

malloc.o malloc.p: $(INC)/malloc.h mallint.h

.c.o:
	@echo $*.c:
	$(CC) $(CFLAGS) -c $*.c

.c.p:
	@echo $*.c:
	$(PROF)$(CC) -p $(CFLAGS) -c $*.c    && $(MV) $(*F).o $*.p

#*****************************************************************************
#           code as in lint.sh to implement -c option
#		on pre SVR2 systems
#*****************************************************************************
llib-lmalloc.l:  llib-lmall.c $(LINT1) $(FRC) $(LINT2)
	cp llib-lmall.c llib-lmalloc.c
	$(CC) -E -C -Dlint llib-lmalloc.c | \
		$(LINT1) -H$(TMPDIR)/hlint > llib-lmalloc.l
	$(LINT2) -Tllib-lmalloc.l -H$(TMPDIR)/hlint
	rm -f llib-lmalloc.c

install: all
	$(PROF)if [ ! -d $(LIBP) ];\
	then\
		$(PROF)mkdir $(LIBP);\
	$(PROF)fi
	$(INS) objects/libmalloc.a $(USRLIB)/libmalloc.a
	$(AR) ts $(USRLIB)/libmalloc.a > /dev/null
	$(PROF)$(INS) pobjects/libmalloc.a $(LIBP)/libmalloc.a
	$(PROF)$(AR) ts $(LIBP)/libmalloc.a > /dev/null
	$(INS) llib-lmalloc.l $(USRLIB)/llib-lmalloc.l
	chmod 644 $(USRLIB)/libmalloc.a $(USRLIB)/llib-lmalloc.l
	chgrp bin $(USRLIB)/libmalloc.a $(USRLIB)/llib-lmalloc.l
	chown bin $(USRLIB)/libmalloc.a $(USRLIB)/llib-lmalloc.l
	$(PROF)chmod 644 $(LIBP)/libmalloc.a
	$(PROF)chgrp bin $(LIBP)/libmalloc.a
	$(PROF)chown bin $(LIBP)/libmalloc.a

clean: 
	$(RM) -fr pobjects objects

clobber: clean
	$(RM) -f $(POBJECTS) $(OBJECTS) llib-lmalloc.l greplist tags

SOURCES= mallint.h       llib-lmall.c    malloc.c        
ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
