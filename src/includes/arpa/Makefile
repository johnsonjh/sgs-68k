#
# SCCS_ID - "@(#)Makefile (TWG)       1.2     89/08/31 "
#
# @(#) Copyright 1986.  The Wollongong Group, Inc.  All Rights Reserved.
#

SOURCES	= ftp.h inet.h nameser.h telnet.h tftp.h
OBJECTS	= 

BINDIST	= ../../bindist
SRCDIST	= ../../srcdist
INSTALL =

include	../../make.pre

SHELL	= /bin/sh

OPTS	= 
DEFINES = 

$(DRIVER) all: $(SOURCES)

bindist : $(SOURCES)
	cp $(SOURCES) $(BINDIST)/user/usr/include/tcp/arpa

install : $(SOURCES)
	-mkdir $(INSTALL)/usr/include/tcp/arpa
	cp $(SOURCES) $(INSTALL)/usr/include/tcp/arpa

srcdist : 
	cp $(SOURCES) Makefile $(SRCDIST)/user/include/arpa

clean :
	rm -f *.o core a.out

clobber : clean
