#	START NEW ARIX SCCS HEADER
#
#	@(#) libsec.mk: version 25.1 created on 12/2/91 at 20:14:32
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	ident	"@(#)libsec.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
#
#	END NEW ARIX SCCS HEADER
#
# libsec makefile

include $(IROOT)/macdefs

OBJS =     \
libsec.a(dev_check.o)	\
libsec.a(audlib.o)	\
libsec.a(getspent.o)	\
libsec.a(getsgent.o)	\
libsec.a(gettaent.o)	\
libsec.a(getuaent.o)	\
libsec.a(getgaent.o)	\
libsec.a(lckpwdf.o)	\
libsec.a(lckgrpf.o)	\
libsec.a(mlslib.o)		\
libsec.a(putgrent.o)	\
libsec.a(privlib.o)	\
libsec.a(runlevel.o)	\
libsec.a(secintf.o)	\
libsec.a(lcksec.o)	\
libsec.a(net.o)		\
libsec.a(getrolent.o)	\
libsec.a(rolelib.o)

SOURCES= \
dev_check.c		\
audlib.c		\
getspent.c		\
getsgent.c		\
gettaent.c		\
getuaent.c 		\
getgaent.c		\
lckpwdf.c		\
lckgrpf.c		\
mlslib.c			\
putgrent.c		\
privlib.c		\
runlevel.c		\
secintf.s		\
lcksec.c		\
net.c			\
getrolent.c		\
rolelib.c

SVID_FLAG=
INSDIR=$(ROOT)/lib
ARGFLAGS = -r

.c.a:
	$(CC) -c $(CFLAGS) $<
	$(AR) $(ARGFLAGS) $@ $*.o
	rm -f $*.o

.s.a:
	$(CC) -c $(CFLAGS) $<
	$(AR) $(ARGFLAGS) $@ $*.o
	rm -f $*.o

libsec.a: $(OBJS)

all: libsec.a

install:	all
	$(INS) libsec.a $(INSDIR)/libsec.a
	$(AR) ts $(INSDIR)/libsec.a > /dev/null
	chmod 644 $(INSDIR)/libsec.a
	chgrp bin $(INSDIR)/libsec.a
	chown bin $(INSDIR)/libsec.a

clean:
	rm -f *.o

clobber: clean
	rm -f  libsec.a tags greplist

ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist

