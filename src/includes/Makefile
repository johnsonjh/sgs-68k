#
# SCCS_ID - "@(#)Makefile (TWG)       1.4     89/08/31 "
# @(#) Copyright 1986.  The Wollongong Group, Inc.  All Rights Reserved.
#

SOURCES	= lastlog.h netdb.h resolv.h routed.h signal.h snmp.h syslog.h \
	  win_hook.h
BSOURCES = lastlog.h netdb.h resolv.h routed.h signal.h syslog.h win_hook.h
OBJECTS	= 

BINDIST	= ../bindist
SRCDIST	= ../srcdist
INSTALL	= 

include	../make.pre

SHELL	= /bin/sh

OPTS	=  -O
DEFINES = 

$(DRIVER) all: $(SOURCES)
	cd arpa ; $(MAKE) 
	cd sys ; $(MAKE) 

bindist : $(BSOURCES)
	cp $(BSOURCES) $(BINDIST)/user/usr/include/tcp
	cd arpa ; $(MAKE) bindist "OPTS=$(OPTS)" "DEFINES=$(DEFINES)"
	cd sys ;  $(MAKE) bindist "OPTS=$(OPTS)" "DEFINES=$(DEFINES)"
	cd netimp ;  $(MAKE) bindist "OPTS=$(OPTS)" "DEFINES=$(DEFINES)"

install : $(BSOURCES)
	-mkdir $(INSTALL)/usr/include/tcp
	cp $(BSOURCES) $(INSTALL)/usr/include/tcp
	cd arpa ; $(MAKE) install "OPTS=$(OPTS)" "DEFINES=$(DEFINES)"
	cd sys ;  $(MAKE) install "OPTS=$(OPTS)" "DEFINES=$(DEFINES)"
	cd netimp ;  $(MAKE) install "OPTS=$(OPTS)" "DEFINES=$(DEFINES)"

srcdist : 
	cp $(SOURCES) Makefile $(SRCDIST)/user/include
	cd arpa ; $(MAKE) srcdist
	cd sys ;  $(MAKE) srcdist
	cd netimp ;  $(MAKE) srcdist

clean :
	rm -f *.o core a.out
	cd arpa ; $(MAKE) clean
	cd sys ;  $(MAKE) clean
	cd netimp ;  $(MAKE) clean

clobber : clean
	cd sys ;  $(MAKE) clobber
	cd netimp ;  $(MAKE) clobber
	cd arpa ; $(MAKE) clobber
