#	START NEW ARIX SCCS HEADER
#
#	@(#) nsl.mk: version 25.1 created on 12/2/91 at 20:17:04
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)nsl.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT: ident	"libnsl:nsl/nsl.mk	1.10"
#ident	"@(#)libnsl:nsl.mk	25.1"
# 
# Network services library
#

IROOT =
include $(IROOT)/macdefs
USRLIB=$(ROOT)/usr/$(SGS)lib
LIB=$(ROOT)/shlib
PFX=$(PREFIX)

LIBOBJS= t_accept.o t_bind.o t_connect.o t_error.o t_close.o\
	 t_getinfo.o t_getstate.o t_listen.o t_look.o\
	 t_rcv.o t_rcvconnect.o t_rcvdis.o t_snd.o t_snddis.o\
	 t_unbind.o t_optmgmt.o\
	 t_rcvudata.o t_rcvuderr.o t_sndudata.o t_sndrel.o t_rcvrel.o\
	 t_alloc.o t_free.o t_open.o t_sync.o\
	 _dummy.o _errlst.o _data.o _data2.o _conn_util.o _utility.o\
	 __free_def.o __calloc_def.o __perror_def.o __strlen_def.o\
	 __write_def.o __ioctl_def.o __putmsg_def.o __getmsg_def.o\
	 __errno_def.o __memcpy_def.o __fcntl_def.o __sigset_def.o\
	 __open_def.o __close_def.o __ulimit_def.o

INCLUDES=  	$(INCRT)/sys/param.h\
		$(INCRT)/sys/types.h\
		$(INCRT)/sys/errno.h\
		$(INCRT)/sys/stream.h\
		$(INCRT)/sys/stropts.h\
		$(INCRT)/sys/tihdr.h\
		$(INCRT)/sys/timod.h\
		$(INCRT)/sys/tiuser.h\
		$(INCRT)/sys/signal.h\
		./_import.h


all:      _spec ./libnsl_s ./libnsl_s.a

__calloc_def.c :	calloc_def.c
	ln	calloc_def.c __calloc_def.c 

__close_def.c :	close_def.c
	ln	close_def.c __close_def.c 

__errno_def.c :	errno_def.c
	ln	errno_def.c __errno_def.c 

__fcntl_def.c :	fcntl_def.c
	ln	fcntl_def.c __fcntl_def.c 

__getmsg_def.c :	getmsg_def.c
	ln	getmsg_def.c __getmsg_def.c 

__ioctl_def.c :	ioctl_def.c
	ln	ioctl_def.c __ioctl_def.c 

__memcpy_def.c :	memcpy_def.c
	ln	memcpy_def.c __memcpy_def.c 

__perror_def.c :	perror_def.c
	ln	perror_def.c __perror_def.c 

__putmsg_def.c :	putmsg_def.c
	ln	putmsg_def.c __putmsg_def.c 

__sigset_def.c :	sigset_def.c
	ln	sigset_def.c __sigset_def.c 

__strlen_def.c :	strlen_def.c
	ln	strlen_def.c __strlen_def.c 

__ulimit_def.c :	ulimit_def.c
	ln	ulimit_def.c __ulimit_def.c 

__write_def.c :	write_def.c
	ln	write_def.c __write_def.c 

t_rcvconnect.c :	rcvconnect.c
	ln	rcvconnect.c t_rcvconnect.c 


./libnsl_s: _spec $(LIBOBJS)
	#-rm -f libnsl_s;
	#$(PFX)mkshlib -s _spec -t libnsl_s

./libnsl_s.a: _spec $(LIBOBJS)
	-rm -f libnsl_s.a;
	ar qv libnsl.a $(LIBOBJS)
	#$(PFX)mkshlib -s _spec -h libnsl_s.a -n -t libnsl_s

$(LIBOBJS):	$(INCLUDES)

install:  all
	$(INS) libnsl.a $(USRLIB)/libnsl.a
	@- if [ -f $(USRLIB)/libnsl_s.a ]; then rm $(USRLIB)/libnsl_s.a; fi
	ln $(USRLIB)/libnsl.a $(USRLIB)/libnsl_s.a
	$(AR) ts $(USRLIB)/libnsl.a > /dev/null
	$(CH)chmod 644 $(USRLIB)/libnsl.a
	$(CH)chgrp bin $(USRLIB)/libnsl.a
	$(CH)chown bin $(USRLIB)/libnsl.a
	#$(INS) libnsl_s.a $(USRLIB)/libnsl_s.a
	#$(AR) ts $(USRLIB)/libnsl_s.a > /dev/null
	#$(INS) libnsl_s $(LIB)/libnsl_s
	#$(CH)chmod 644 $(USRLIB)/libnsl_s.a
	#$(CH)chmod 755 $(LIB)/libnsl_s
	#$(CH)chgrp bin $(USRLIB)/libnsl_s.a
	#$(CH)chgrp bin $(LIB)/libnsl_s
	#$(CH)chown bin $(USRLIB)/libnsl_s.a
	#$(CH)chown bin $(LIB)/libnsl_s

clean:
	-rm -f *.o

clobber:	clean
	-rm -f libnsl_s.a 
	-rm -f libnsl.a 
	-rm -f libnsl_s 
	-rm -f greplist tags
	-rm -f __calloc_def.c __close_def.c __errno_def.c __fcntl_def.c
	-rm -f __getmsg_def.c __ioctl_def.c __memcpy_def.c __perror_def.c
	-rm -f __putmsg_def.c __sigset_def.c __strlen_def.c __ulimit_def.c
	-rm -f __write_def.c t_rcvconnect.c

SOURCES= \
__putmsg_def.c  _import.h       t_connect.c     t_rcvdis.c \
__calloc_def.c  __recv_def.c    _spec           t_error.c       t_rcvrel.c \
__close_def.c   __send_def.c    _utility.c      t_free.c        t_rcvudata.c \
__errno_def.c   __sigset_def.c  t_getinfo.c     t_rcvuderr.c \
__fcntl_def.c   __strlen_def.c  t_getstate.c    t_snd.c \
__free_def.c    __ulimit_def.c  t_listen.c      t_snddis.c \
__getmsg_def.c  __write_def.c   t_look.c        t_sndrel.c \
__ioctl_def.c   _conn_util.c    t_accept.c      t_open.c        t_sndudata.c \
__memcpy_def.c  _data.c         t_alloc.c       t_optmgmt.c     t_sync.c \
_data2.c  \
__open_def.c    _dummy.c        t_bind.c        t_rcv.c         t_unbind.c \
__perror_def.c  _errlst.c       t_close.c       t_rcvconnect.c

ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
