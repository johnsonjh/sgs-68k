#ident	"@(#)libpansi.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#
# makefile for libansic
#
#
# The variable PROF is null by default, causing both the standard C library
# and a profiled library to be maintained.  If profiled object is not 
# desired, the reassignment PROF=@# should appear in the make command line.
#
# The variable IGN may be set to -i by the assignment IGN=-i in order to
# allow a make to complete even if there are compile errors in individual
# modules.
#
# See also the comments in the lower-level machine-dependent makefiles.
#

IROOT =
PROF=
include $(IROOT)/macdefs
include $(IROOT)/sgsdefs
VARIANT=$(SGS)

# POSIX
SVID_FLAG=

PCFLAGS=
PFX=

LIB=$(ROOT)/$(SGS)lib
LIBP=$(ROOT)/$(SGS)lib/libp

DONE=
NONPROF=
DEFLIST=

all:	ansi posixm objects 

ansi:	FRC
	cd ansi; $(MAKE) -ief ansi.mk all

posixm:	FRC
	cd posixm; $(MAKE) -ief posixm.mk all

installall:	objects 

objects:
	#
	# compile portable library modules
	cd port; $(MAKE) -$(MAKEFLAGS)ef port.mk
	#
	# compile machine-dependent library modules
	cd mach; $(MAKE) -$(MAKEFLAGS)ef mach.mk
	#
	# place portable modules in "object" directory, then overlay
	# 	the machine-dependent modules.
	-rm -rf object
	mkdir object
	cp port/*/*.o object
	cp port/*/*.n object
	cd ansi; cp */*.o ../object
	cd ansi; cp */*.O ../object
	cd ansi; cp */*.n ../object
	cp posixm/*.o object
	$(PROF)cp port/*/*.p object
	$(PROF)cd ansi; cp */*.p ../object
	$(PROF)cd ansi; cp */*.P ../object
	cp mach/*/*.o object
	$(PROF)cp mach/*/*.p object
	$(PROF)cp posixm/*.p object
	#
	#
	# delete temporary libraries
	-rm -f lib.libc
	-rm -f lib.libcn
	$(PROF)-rm -f libp.libc
	#
	# build archive out of the remaining modules.
	cd object; $(MAKE) -$(MAKEFLAGS)ef ../mach/mach.mk archive \
		AR=$(AR)  LORDER=$(LORDER) PROF=$(PROF)
	-rm -rf object

move:
	#
	# move the library or libraries into the correct directory
	$(INS) lib.libc $(LIB)/lib$(VARIANT)c.a 
	$(AR) ts $(LIB)/lib$(VARIANT)c.a  >/dev/null
	chmod 644 $(LIB)/lib$(VARIANT)c.a
	chgrp bin $(LIB)/lib$(VARIANT)c.a
	chown bin $(LIB)/lib$(VARIANT)c.a
	$(INS) lib.libcn $(LIB)/lib$(VARIANT)cn.a 
	$(AR) ts $(LIB)/lib$(VARIANT)cn.a  >/dev/null
	chmod 644 $(LIB)/lib$(VARIANT)cn.a
	chgrp bin $(LIB)/lib$(VARIANT)cn.a
	chown bin $(LIB)/lib$(VARIANT)cn.a
	-if [ "$(PROF)" != "@#" ]; \
	then \
		if [ ! -d $(LIBP) ]; \
		then \
			mkdir $(LIBP); \
		fi; \
	fi
	$(PROF)$(INS) libp.libc $(LIBP)/lib$(VARIANT)c.a
	$(AR) ts $(LIBP)/lib$(VARIANT)c.a >/dev/null
	chmod 644 $(LIBP)/lib$(VARIANT)c.a
	chgrp bin $(LIBP)/lib$(VARIANT)c.a
	chown bin $(LIBP)/lib$(VARIANT)c.a

install: FRC
	cd ansi; $(MAKE) -ief ansi.mk install
	cd posixm; $(MAKE) -ief posixm.mk install
	$(MAKE) -ief libpansi.mk installall
	$(MAKE) -ief libpansi.mk move

clean:
	#
	# remove intermediate files except object modules and temp library
	-rm -rf lib*.contents obj*
	cd port ;  $(MAKE) -$(MAKEFLAGS)ef port.mk clean
	cd mach ;  $(MAKE) -$(MAKEFLAGS)ef mach.mk clean 
	cd ansi ;  $(MAKE) -$(MAKEFLAGS)ef ansi.mk clean

clobber:
	#
	# remove intermediate files
	-rm -rf *.n *.o *.p lib*.libc lib*.libcn lib*.contents obj*
	cd port ;  $(MAKE) -$(MAKEFLAGS)ef port.mk clobber
	cd mach ;  $(MAKE) -$(MAKEFLAGS)ef mach.mk clobber 
	cd ansi ;  $(MAKE) -$(MAKEFLAGS)ef ansi.mk clobber
	cd posixm ; $(MAKE) -$(MAKEFLAGS)ef posixm.mk clobber

FRC:
