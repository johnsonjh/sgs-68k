#ident	"@(#)libcrypt.mk	23.2	1/8/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT: ident	"libcrypt.mk	1.9"

#	Makefile for libcrypt

IROOT =
include $(IROOT)/macdefs

USRLIB = $(ROOT)/usr/$(SGS)lib

INC = $(INCRT)

STRIP = strip

SIZE = size

#top#
# Generated by makefile 1.47

MAKEFILE = libcrypt.mk

LIBRARY = libcrypt.a

OBJECTS =  crypt.o cryptio.o des_crypt.o des_decrypt.o des_encrypt.o \
	p2open.o

SOURCES =  crypt.c cryptio.c des_crypt.c des_decrypt.c des_encrypt.c \
	p2open.c

ALL :
	@echo "	Making UN-SECURE DOMESTIC version of libcrypt.a\n"
	make -$(MAKEFLAGS)f $(MAKEFILE) DOMWARN $(LIBRARY) DECRYPT=d_decrypt.c
	mv $(LIBRARY) d$(LIBRARY)
	rm des_decrypt.c
	@echo "	Making SECURE INTERNATIONAL version of libcrypt.a\n"
	make -$(MAKEFLAGS)f $(MAKEFILE) INTWARN $(LIBRARY) \
		INT=-DINTERNATIONAL DECRYPT=/dev/null

DOMWARN:
	#
	# This makefile compiles the UN-SECURE, or DOMESTIC libcrypt.a
	# As we understand it, this MAY NOT be sent to other countries.
	#

INTWARN:
	#
	# This makefile compiles the SECURE, or INTERNATIONAL libcrypt.a
	# As we understand it, this MAY NOT be sent to other countries.
	#

$(LIBRARY):	$(LIBRARY)(p2open.o) $(LIBRARY)(des_encrypt.o) \
		$(LIBRARY)(des_decrypt.o) $(LIBRARY)(des_crypt.o) \
		$(LIBRARY)(cryptio.o) $(LIBRARY)(crypt.o) 

$(LIBRARY)(crypt.o):	 

$(LIBRARY)(cryptio.o):	 $(INCRT)/stdio.h \
		 $(INCRT)/signal.h	\
		 $(INCRT)/sys/signal.h 

des_decrypt.c: d_decrypt.c
	cp	$(DECRYPT) des_decrypt.c

des_encrypt.c: d_encrypt.c
	ln	d_encrypt.c des_encrypt.c

$(LIBRARY)(des_decrypt.o):	 des_decrypt.c

$(LIBRARY)(des_encrypt.o):	 des_encrypt.c

$(LIBRARY)(p2open.o):	 $(INCRT)/stdio.h \
		 $(INCRT)/signal.h	\
		 $(INCRT)/sys/signal.h \
		 $(INCRT)/fcntl.h \
		 $(INCRT)/sys/fcntl.h 

GLOBALINCRTS = $(INCRT)/fcntl.h \
	$(INCRT)/signal.h \
	$(INCRT)/stdio.h \
	$(INCRT)/sys/fcntl.h \
	$(INCRT)/sys/signal.h 

clean:
	rm -f $(OBJECTS)

clobber:
	rm -f $(OBJECTS) $(LIBRARY) d$(LIBRARY) tags greplist des_encrypt.c des_decrypt.c

newmakefile:
	makefile -m -$(MAKEFLAGS)f $(MAKEFILE) -L $(LIBRARY)  -s INCRT=$(INCRT)

#bottom#

$(LIBRARY)(des_crypt.o):	 
	#
	# WARNING: COMPILATION FOR DOMESTIC/UN-SECURE crypt
	#
	$(CC) -c $(CFLAGS) $(INT) des_crypt.c 
	$(AR) rv $(LIBRARY) des_crypt.o 
	rm -f des_crypt.o

all : ALL

install: ALL
	$(INS) $(LIBRARY) $(USRLIB)/$(LIBRARY)
	$(INS) d$(LIBRARY) $(USRLIB)/d$(LIBRARY)
	$(AR) ts $(USRLIB)/$(LIBRARY) > /dev/null
	$(AR) ts $(USRLIB)/d$(LIBRARY) > /dev/null
	chmod 644 $(USRLIB)/$(LIBRARY)
	chgrp bin $(USRLIB)/$(LIBRARY)
	chown bin $(USRLIB)/$(LIBRARY)
	chmod 644 $(USRLIB)/d$(LIBRARY)
	chgrp bin $(USRLIB)/d$(LIBRARY)
	chown bin $(USRLIB)/d$(LIBRARY)

size: ALL
	$(SIZE) $(LIBRARY)

strip: ALL

#	These targets are useful but optional

partslist:
	@echo $(MAKEFILE) $(SOURCES) $(LOCALINCRTS)  |  tr ' ' '\012'  |  sort

productdir:
	@echo $(USRLIB) | tr ' ' '\012' | sort

product:
	@echo $(LIBRARY)  |  tr ' ' '\012'  | \
	sed 's;^;$(USRLIB)/;'

srcaudit:
	@fileaudit $(MAKEFILE) $(LOCALINCRTS) $(SOURCES) -o $(OBJECTS) \
		$(LIBRARY)

ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
