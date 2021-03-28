#ident	"@(#)ansi.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	 Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	On commandline, specify 
#			IROOT for macdefs


include $(IROOT)/macdefs
include $(IROOT)/sgsdefs
VARIANT=$(SGS)

LORDER=$(PFX)lorder
LIB=$(ROOT)/$(SGS)lib
LIBP=$(ROOT)/$(SGS)lib/libp
PROF=

DIRECTS=	assert ctype errno locale setjmp signal \
			stdio stdlib string time startup system
NO_CP_DIRECTS=	math

CRTS= crt1.o mcrt1.o crt2.o mcrt2.o
PCRTS=crt1.p mcrt1.p crt2.p mcrt2.p

all:
	for i in $(DIRECTS) $(NO_CP_DIRECTS); \
	do \
		cd $$i; $(MAKE) -$(MAKEFLAGS)f $$i.mk all; \
		cd ..; \
	done
	-mkdir -p object
	cp */*.n object
	for i in $(DIRECTS); \
	do \
		echo "cp $$i/*.o object;"; \
		cp $$i/*.o object; \
	done
	if [ "$(PROF)" != "@#" ]; \
	then \
		for i in $(DIRECTS); \
		do \
			echo "$(PROF)cp $$i/*.p object"; \
			$(PROF)cp $$i/*.p object; \
		done	\
	fi
	#
	# delete temporary libraries
	-rm -f lib.libca
	-rm -f lib.libcan
	$(PROF)-rm -f libp.libca
	#
	# set aside run-time modules, which don't go in library archive!
	cd object; \
	for i in $(CRTS) $(PCRTS); \
	do \
		mv $$i .. ;\
	done
	#
	# build archive out of the remaining modules, care is taken
	# to exclude exit and fakcu from objlist since they
	# must explicitly be placed at the end of the archive.
	#
	# mcount should not be in non-profiled library
	#
	cd object; \
	mv fakcu.o fakcu.x; \
	mv exit.o exit.x; \
	rm mcount.o; $(LORDER) *.o | tsort >objlist; \
	mv fakcu.x fakcu.o; \
	mv exit.x exit.o
	#
	# build the archive with the modules in correct order.
	cd object; $(AR) q ../lib.libca `cat objlist` exit.o fakcu.o
	$(PROF)#
	$(PROF)# build the profiled library archive, first renaming the
	$(PROF)#	.p (profiled object) modules to .o
	if [ "$(PROF)" != "@#" ]; \
	then	\
		cd object;	\
		for i in *.p ; \
		do \
			mv $$i `basename $$i .p`.o ; \
		done	\
	fi
	if [ "$(PROF)" != "@#" ]; \
	then \
		cd object; $(PROF)$(AR) q ../libp.libca `cat objlist` \
			mcount.o exit.o fakcu.o; \
	fi
	# Now do the NONDYNAMIC lib
	cd object;				\
	for i in *.n ;				\
	do					\
		mv $$i `basename $$i .n`.o ;	\
	done
	cd object;$(AR) q ../lib.libcan `cat objlist` exit.o fakcu.o
	-rm -rf object

install:	all
#
	# move the library or libraries into the correct directory
	for i in $(CRTS); \
	do \
		$(INS) $$i $(LIB)/$$i ;\
		chmod 644 $(LIB)/$$i ;\
		chgrp bin $(LIB)/$$i ;\
		chown bin $(LIB)/$$i ;\
	done
	$(INS) lib.libca $(LIB)/lib$(VARIANT)ca.a
	$(AR) ts $(LIB)/lib$(VARIANT)ca.a  >/dev/null
	rm -f lib.libca
	chmod 644 $(LIB)/lib$(VARIANT)ca.a
	chgrp bin $(LIB)/lib$(VARIANT)ca.a
	chown bin $(LIB)/lib$(VARIANT)ca.a
	$(INS) lib.libcan $(LIB)/lib$(VARIANT)can.a
	$(AR) ts $(LIB)/lib$(VARIANT)can.a  >/dev/null
	rm -f lib.libcan
	chmod 644 $(LIB)/lib$(VARIANT)can.a
	chgrp bin $(LIB)/lib$(VARIANT)can.a
	chown bin $(LIB)/lib$(VARIANT)can.a
	-if [ "$(PROF)" != "@#" ]; \
	then \
		if [ ! -d $(LIBP) ]; \
		then \
			mkdir $(LIBP); \
		fi; \
	fi
	$(PROF)$(INS) libp.libca $(LIBP)/lib$(VARIANT)ca.a ; rm -f libp.libca
	$(PROF) $(AR) ts $(LIBP)/lib$(VARIANT)ca.a >/dev/null
	$(PROF)chmod 644 $(LIBP)/lib$(VARIANT)ca.a
	$(PROF)chgrp bin $(LIBP)/lib$(VARIANT)ca.a
	$(PROF)chown bin $(LIBP)/lib$(VARIANT)ca.a

clean:
	# remove intermediate files except object modules and temp library
	rm -rf lib*.contents obj*
	rm -rf $(CRTS) $(PCRTS)
	for i in $(DIRECTS); \
	do \
		cd $$i; $(MAKE) -$(MAKEFLAGS)f $$i.mk clean ; \
		cd ..; \
	done

clobber:
	#
	# remove intermediate files
	rm -rf *.n *.o lib*.libca lib*.contents obj*
	rm -rf $(CRTS) $(PCRTS)
	for i in $(DIRECTS) $(NO_CP_DIRECTS); \
	do \
		cd $$i; $(MAKE) -$(MAKEFLAGS)f $$i.mk clobber ; \
		cd ..; \
	done
