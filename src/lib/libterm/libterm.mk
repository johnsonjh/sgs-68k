#	START NEW ARIX SCCS HEADER
#
#	@(#) libterm.mk: version 25.1 created on 12/2/91 at 20:15:33
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)libterm.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#

#	@(#)libterm.mk	1.2	

SROOT =
IROOT =
include $(IROOT)/macdefs

LIBDIR = lib

all install clean clobber: $(LIBDIR)
		cd $(LIBDIR); $(MAKE) -$(MAKEFLAGS)f $(LIBDIR).mk $@ 

$(LIBDIR):
	@echo CANNOT FIND LIBRARY SOURCE DIRECTORY `pwd`/$@
