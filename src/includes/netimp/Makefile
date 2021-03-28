#
# SCCS_ID - "@(#)Makefile (TWG)       1.2     89/08/31 "
#
# @(#) Copyright 1986.  The Wollongong Group, Inc.  All Rights Reserved.
#

SOURCES	= if_imp.h if_imphost.h
OBJECTS	= 

BINDIST	= ../../bindist
SRCDIST	= ../../srcdist
INSTALL	=

include	../../make.pre

SHELL	= /bin/sh

OPTS	= 
DEFINES = 

$(DRIVER) all: $(SOURCES)

bindist : $(SOURCES)
	cp $(SOURCES) $(BINDIST)/user/usr/include/tcp/netimp

install : $(SOURCES)
	-mkdir $(INSTALL)/usr/include/tcp/netimp
	cp $(SOURCES) $(INSTALL)/usr/include/tcp/netimp

srcdist : 
	cp $(SOURCES) Makefile $(SRCDIST)/user/include/netimp

clean :
	rm -f *.o core a.out

clobber : clean
