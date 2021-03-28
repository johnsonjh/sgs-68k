#	START NEW ARIX SCCS HEADER
#
#	@(#) libns.mk: version 25.1 created on 12/2/91 at 19:41:53
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)libns.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT: ident	"libns:libns.mk	1.10"
#ident	"@(#)libns:libns.mk	25.1"

# This makefile makes libns.a, which is the library for
# the name server library.
# NOTE: this library is not for general use.  It is put
# 	in /usr/lib ONLY for the convenience of the
#	commands that use it.
#
IROOT =
include $(IROOT)/macdefs
include $(IROOT)/sgsdefs
LIBDIR = .
INCDIR = $(INCRT)
# install this for access by the other utility makes
# rel.mk must remember to remove it.
# no installation in $(ROOT) and no distribution
# to customers
USRLIB = $(ROOT)/usr/$(SGS)lib
LIB = $(ROOT)/$(SGS)lib
#USRLIB = $(SROOT)
LIBNAME = libns.a
LLIB = ns
LINTLIB = llib-l$(LLIB).ln
LOG=-DLOGGING -DLOGMALLOC
PROFILE=
DEBUG=
SRC= ind_data.c nsblock.c nsports.c nsrports.c \
	rtoken.c astoa.c stoa.c ns_comm.c nslog.c canon.c spipe.c \
	logmalloc.c ns_findp.c ns_getaddr.c ns_getblock.c ns_initaddr.c \
	ns_verify.c ns_error.c ns_errlist.c ns_info.c ns_sendpass.c \
	attconnect.c rfrequest.c negotiate.c getoken.c netname.c \
	swtab.c uidmap.c ns_syntax.c rfs_up.c rfrcv.c

FILES =\
	$(LIBNAME)(ind_data.o)\
	$(LIBNAME)(nsblock.o)\
	$(LIBNAME)(nsports.o)\
	$(LIBNAME)(nsrports.o)\
	$(LIBNAME)(rtoken.o)\
	$(LIBNAME)(stoa.o)\
	$(LIBNAME)(astoa.o)\
	$(LIBNAME)(ns_comm.o) \
	$(LIBNAME)(nslog.o) \
	$(LIBNAME)(canon.o) \
	$(LIBNAME)(spipe.o) \
	$(LIBNAME)(logmalloc.o)\
	$(LIBNAME)(ns_getaddr.o)\
	$(LIBNAME)(ns_findp.o)\
	$(LIBNAME)(ns_getblock.o)\
	$(LIBNAME)(ns_initaddr.o)\
	$(LIBNAME)(ns_verify.o)\
	$(LIBNAME)(ns_error.o)\
	$(LIBNAME)(ns_errlist.o)\
	$(LIBNAME)(ns_info.o)\
	$(LIBNAME)(ns_sendpass.o)\
	$(LIBNAME)(attconnect.o)\
	$(LIBNAME)(rfrequest.o)\
	$(LIBNAME)(negotiate.o)\
	$(LIBNAME)(getoken.o)\
	$(LIBNAME)(netname.o)\
	$(LIBNAME)(uidmap.o)\
	$(LIBNAME)(rfs_up.o)\
	$(LIBNAME)(ns_syntax.o)\
	$(LIBNAME)(swtab.o)\
	$(LIBNAME)(rfrcv.o)

lib all:	$(LIBNAME) 
ns_getblock.c: nsgetblock.c
	ln nsgetblock.c ns_getblock.c

ns_initaddr.c: nsinitaddr.c
	ln nsinitaddr.c ns_initaddr.c
	
ns_sendpass.c: nssendpass.c
	ln nssendpass.c ns_sendpass.c

debug:
	$(MAKE) -$(MAKEFLAGS)f libns.mk \
	LIBNAME=libnsdb.a DEBUG="-g -DDEBUG -DLOGGING -DLOGMALLOC" lib
lint:
	lint -uax -DLOGGING -o $(LLIB) $(SRC)

install: lib
	$(INS) $(LIBNAME) $(USRLIB)/$(LIBNAME)
	$(AR) ts $(USRLIB)/$(LIBNAME) > /dev/null
	chmod 644 $(USRLIB)/$(LIBNAME)
	chgrp bin $(USRLIB)/$(LIBNAME)
	chown bin $(USRLIB)/$(LIBNAME)
	$(INS) $(LIBNAME) $(LIB)/$(LIBNAME)
	$(AR) ts $(LIB)/$(LIBNAME) > /dev/null
	chmod 644 $(LIB)/$(LIBNAME)
	chgrp bin $(LIB)/$(LIBNAME)
	chown bin $(LIB)/$(LIBNAME)

uninstall:
	-rm $(USRLIB)/$(LIBNAME)

.PRECIOUS:	$(LIBNAME)

$(LIBNAME):	$(FILES)

clean:
	-rm -f *.o

clobber: clean
	-rm -f $(LIBNAME) tags greplist ns_initaddr.c ns_sendpass.c ns_getblock.c

#### dependencies now follow

$(LIBNAME)(nsports.o): stdns.h nsports.h nsdb.h $(INCRT)/nsaddr.h nslog.h
$(LIBNAME)(nsrports.o): stdns.h nsports.h nsdb.h $(INCRT)/nsaddr.h nslog.h \
	$(INCRT)/pn.h
$(LIBNAME)(rtoken.o): stdns.h nsdb.h
$(LIBNAME)(ind_data.o): stdns.h nslog.h
$(LIBNAME)(nsblock.o): nslog.h nsdb.h stdns.h $(INCRT)/nserve.h
$(LIBNAME)(ns_comm.o): $(INCRT)/nserve.h $(INCRT)/nsaddr.h nslog.h stdns.h nsports.h
$(LIBNAME)(nslog.o): nslog.h
$(LIBNAME)(astoa.o): $(INCRT)/nsaddr.h
$(LIBNAME)(stoa.o): $(INCRT)/nsaddr.h
$(LIBNAME)(ns_getaddr.o): $(INCRT)/nserve.h $(INCRT)/nsaddr.h
$(LIBNAME)(ns_findp.o): $(INCRT)/nserve.h $(INCRT)/nsaddr.h
$(LIBNAME)(ns_getblock.o): $(INCRT)/nserve.h
$(LIBNAME)(ns_initaddr.o): $(INCRT)/nserve.h
$(LIBNAME)(ns_verify.o): $(INCRT)/nserve.h
$(LIBNAME)(ns_sendpass.o): $(INCRT)/nserve.h
$(LIBNAME)(attconnect.o): $(INCRT)/pn.h
$(LIBNAME)(rfrequest.o): $(INCRT)/pn.h
$(LIBNAME)(negotiate.o): $(INCRT)/pn.h
$(LIBNAME)(getoken.o): $(INCRT)/sys/nserve.h $(INCRT)/sys/cirmgr.h\
	$(INCRT)/sys/utsname.h $(INCRT)/sys/rfsys.h
$(LIBNAME)(netname.o): $(INCRT)/sys/nserve.h $(INCRT)/sys/cirmgr.h\
	$(INCRT)/sys/utsname.h $(INCRT)/sys/rfsys.h
$(LIBNAME)(swtab.o): $(INCRT)/sys/nserve.h $(INCRT)/sys/cirmgr.h\
	$(INCRT)/sys/param.h $(INCRT)/pn.h
$(LIBNAME)(uidmap.o): idload.h $(INCRT)/nserve.h $(INCRT)/sys/rfsys.h
$(LIBNAME)(rfs_up.o): $(INCRT)/nserve.h $(INCRT)/sys/rfsys.h
$(LIBNAME)(ns_syntax.o): $(INCRT)/nserve.h
$(LIBNAME)(rfrcv.o): $(INCRT)/tiuser.h

SOURCES=$(SRC)
ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
