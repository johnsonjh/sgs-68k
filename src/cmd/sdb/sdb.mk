#	START NEW ARIX SCCS HEADER
#
#	@(#) sdb.mk: version 25.1 created on 12/2/91 at 17:15:26
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)sdb.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#

#ident	"@(#)sdb:sdb.mk	25.1"

IROOT =
include $(IROOT)/macdefs


all install clean clobber:
	@-echo "cd "`pwd`"/mach; $(MAKE) -$(MAKEFLAGS)f mach.mk FLAVOR=M68020 UFLAVOR=M68040 $@" 
	-cd mach; $(MAKE) -$(MAKEFLAGS)f mach.mk FLAVOR=M68020 UFLAVOR=M68040 $@ 
	@-echo "cd "`pwd`"/mach; $(MAKE) -$(MAKEFLAGS)f mach.mk FLAVOR=M68040 UFLAVOR=M68020 $@" 
	-cd mach; $(MAKE) -$(MAKEFLAGS)f mach.mk FLAVOR=M68040 UFLAVOR=M68020 $@ 
