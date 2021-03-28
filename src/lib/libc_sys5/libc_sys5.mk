#ident	"@(#)libc_sys5.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#
#	 Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT: ident	"libc:libc.mk	1.26"
#
# makefile for libc_sys5
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
NONPROF=

include $(IROOT)/macdefs
include $(IROOT)/sgsdefs

VARIANT=$(SGS)
OSGS=
PFX=
LORDER=$(PFX)lorder

LIB=$(ROOT)/lib
LIBP=$(ROOT)/lib/libp

DONE=
DEFLIST=

all:	objects 

nfsall:	objects nfs

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
	cp port/gen/*.o object
	cp port/print/*.o object
	cp port/stdio/*.o object
	cp port/sys/*.o object
	cp */*/*.n object
	$(PROF)cp port/gen/*.p object
	$(PROF)cp port/print/*.p object
	$(PROF)cp port/stdio/*.p object
	$(PROF)cp port/sys/*.p object
	cp mach/*/*.o object
	$(PROF)cp mach/*/*.p object
	#
	# delete temporary libraries
	-rm -f lib.libc
	-rm -f lib.libcn
	$(PROF)-rm -f libp.libc
	#
	# set aside run-time modules, which don't go in library archive!
	cd object ;\
	for i in crt0.o crt5.o crtn.o mcrt0.o mcrt5.o ; \
	do \
		mv $$i .. ;\
	done
	#
	# build archive out of the remaining modules.
	cd object; $(MAKE) -$(MAKEFLAGS)ef ../mach/mach.mk archive \
		AR=$(AR)  LORDER=$(LORDER) PROF=$(PROF)
	-rm -rf object

nfs:
	mkdir object
	cd net; $(MAKE) -$(MAKEFLAGS)ef net.mk
	cd inet; $(MAKE) -$(MAKEFLAGS)ef inet.mk
	cd rpc; $(MAKE) -$(MAKEFLAGS)ef rpc.mk
	cd yp; $(MAKE) -$(MAKEFLAGS)ef yp.mk
	cp net/*.o object
	cp inet/*.o object
	cp rpc/*.o object
	cp yp/*.o object
	cd object; ar rv ../lib.libc *.o
	rm -rf object

move:
	#
	# move the library or libraries into the correct directory
	# copy crt5.0 to /lib because that's where the compiler
	# will look for them (under $PREFIX ).
	for i in crt0.o crtn.o crt5.o mcrt0.o mcrt5.o ; \
	do \
		$(INS) $$i $(LIB)/$(OSGS)$$i ;\
		chmod 644 $(LIB)/$(OSGS)$$i ;\
		chgrp bin $(LIB)/$(OSGS)$$i ;\
		chown bin $(LIB)/$(OSGS)$$i ;\
		rm -f $$i ;\
	done
	$(INS) lib.libc $(LIB)/libc5.a 
	$(AR) ts $(LIB)/libc5.a  >/dev/null
	rm -f lib.libc
	$(INS) lib.libcn $(LIB)/libc5n.a 
	$(AR) ts $(LIB)/libc5n.a  >/dev/null
	rm -f lib.libcn
	$(PROF)if [ ! -d $(LIBP) ] ;\
	then \
		$(PROF)	mkdir $(LIBP); \
	$(PROF)fi
	$(PROF)$(INS) libp.libc $(LIBP)/libc5.a ; rm -f libp.libc
	$(AR) ts $(LIBP)/libc5.a >/dev/null
	chmod 644 $(LIBP)/libc5.a
	chgrp bin $(LIBP)/libc5.a
	chown bin $(LIBP)/libc5.a

install: all move

clean:
	#
	# remove intermediate files except object modules and temp library
	-rm -rf lib*.contents obj*
	cd port ;  $(MAKE) -$(MAKEFLAGS)ef port.mk clean
	cd mach ;  $(MAKE) -$(MAKEFLAGS)ef mach.mk clean 
	if [ -d yp  ] ; \
	then \
		cd yp ;\
		$(MAKE) -$(MAKEFLAGS)ef yp.mk clean ;\
	fi
	if [ -d rpc  ] ; \
	then \
		cd rpc ;\
		$(MAKE) -$(MAKEFLAGS)ef rpc.mk clean ;\
	fi
	if [ -d net  ] ; \
	then \
		cd net ;\
		$(MAKE) -$(MAKEFLAGS)ef net.mk clean ;\
	fi
	if [ -d inet  ] ; \
	then \
		cd inet ;\
		$(MAKE) -$(MAKEFLAGS)ef inet.mk clean ;\
	fi
	if [ -d machmau ] ;\
	then \
		cd machmau ;\
		$(MAKE) -$(MAKEFLAGS)ef machmau.mk clean ;\
	fi

clobber:
	#
	# remove intermediate files
	-rm -rf *.o lib*.libc lib*.contents obj*
	cd port ;  $(MAKE) -$(MAKEFLAGS)ef port.mk clobber
	cd mach ;  $(MAKE) -$(MAKEFLAGS)ef mach.mk clobber 
	if [ -d machmau ] ;\
	then \
		cd machmau ;\
		$(MAKE) -$(MAKEFLAGS)ef machmau.mk clean ;\
	fi
