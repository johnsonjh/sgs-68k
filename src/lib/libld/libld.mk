#ident	"@(#)libld.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
SROOT =
IROOT =
include $(IROOT)/macdefs
include $(IROOT)/sgsdefs

#	GLOBAL makefile for libld.a library
#	Target system processors:	m68 (motorola M68000)
#					pdp (PDP 11/70)

all localinstall clean clobber:	
	cd mach; $(MAKE) -$(MAKEFLAGS)f makefile $@  

install:	
	cd mach; $(MAKE) -$(MAKEFLAGS)f makefile $@
