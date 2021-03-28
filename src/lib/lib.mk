include	$(IROOT)/macdefs

LIB=
LIBP=

LIBS = \
	libpansi libPW libplot \
	libmalloc libld libns \
	libc_sys5 libm_sys5 libF77 \
	libsec libcrypt liby libnls \
	libcurses terminfo libI77 \
	libg libgen libl nsl libFPSP

LIBS1 = \
	libpansi libcurses terminfo 

LIBS2 = \
	libc_sys5 libm_sys5 libF77 libplot\
	libsec libcrypt liby libnls libgen

LIBS3 = \
	libI77 libPW libg libl nsl libmalloc \
	libld libns libFPSP

#	libwin  -- not supported

RFSLIBS =	libns

all:
	(												  \
	nice --19 $(MAKE) -$(MAKEFLAGS)f lib.mk libs1     \
		> ../nohup.libs1$(NOHUP) 2>&1 & WAIT_LIBS1=$$! ; \
	nice --19 $(MAKE) -$(MAKEFLAGS)f lib.mk libs2     \
		> ../nohup.libs2$(NOHUP) 2>&1 & WAIT_LIBS2=$$! ; \
	nice --19 $(MAKE) -$(MAKEFLAGS)f lib.mk libs3     \
		> ../nohup.libs3$(NOHUP) 2>&1 & WAIT_LIBS3=$$! ; \
	echo "\n\tWAITING FOR $$WAIT_LIBS1 at `date`" ;   \
	wait $$WAIT_LIBS1;								  \
	echo "\n\tWAITING FOR $$WAIT_LIBS2 at `date`" ;   \
	wait $$WAIT_LIBS2;								  \
	echo "\n\tWAITING FOR $$WAIT_LIBS3 at `date`" ;   \
	wait $$WAIT_LIBS3; )
	@-echo "\n\t============== libs DONE: `date` ======================"

libs1: $(LIBS1)
	-for i in $(LIBS1); \
	do \
		cd $$i; \
		echo "Making:$(MAKE) -$(MAKEFLAGS)f $$i.mk all" ; \
		$(MAKE) -$(MAKEFLAGS)f $$i.mk all ; \
		cd ..; \
	done

libs2: $(LIBS2)
	-for i in $(LIBS2); \
	do \
		cd $$i; \
		echo "Making:$(MAKE) -$(MAKEFLAGS)f $$i.mk all" ; \
		$(MAKE) -$(MAKEFLAGS)f $$i.mk all ; \
		cd ..; \
	done

libs3: $(LIBS3)
	-for i in $(LIBS3); \
	do \
		cd $$i; \
		echo "Making:$(MAKE) -$(MAKEFLAGS)f $$i.mk all" ; \
		$(MAKE) -$(MAKEFLAGS)f $$i.mk all ; \
		cd ..; \
	done

rfs:	
	-for i in $(RFSLIBS) ; \
	do \
		cd $$i; \
		echo "$(MAKE) -$(MAKEFLAGS)f $$i.mk all" ; \
		$(MAKE) -$(MAKEFLAGS)f $$i.mk all ; \
		cd ..; \
	done

clean clobber: 
	for i in $(LIBS); \
	do \
		cd $$i; \
		echo "$(MAKE) -$(MAKEFLAGS)f $$i.mk $@" ; \
		$(MAKE) -$(MAKEFLAGS)f $$i.mk $@ ; \
		cd ..; \
	done

insrfs:	
	cp libns/libns.a $(IROOT)/libns.a

# Important: inslibs must explicitly do an install/clobber, if
# it does a '$@' it will end up doing a 'inslibs clobber'
# which no library make understands, mer99.

install: dirs
	(												  	 \
	nice --19 $(MAKE) -$(MAKEFLAGS)f lib.mk inslibs1     \
		> ../nohup.libs1$(NOHUP) 2>&1 & WAIT_LIBS1=$$! ;    \
	nice --19 $(MAKE) -$(MAKEFLAGS)f lib.mk inslibs2     \
		> ../nohup.libs2$(NOHUP) 2>&1 & WAIT_LIBS2=$$! ;    \
	nice --19 $(MAKE) -$(MAKEFLAGS)f lib.mk inslibs3     \
		> ../nohup.libs3$(NOHUP) 2>&1 & WAIT_LIBS3=$$! ;    \
	echo "\n\tWAITING FOR $$WAIT_LIBS1 at `date`" ;      \
	wait $$WAIT_LIBS1;								     \
	echo "\n\tWAITING FOR $$WAIT_LIBS2 at `date`" ;      \
	wait $$WAIT_LIBS2;								     \
	echo "\n\tWAITING FOR $$WAIT_LIBS3 at `date`" ;      \
	wait $$WAIT_LIBS3; )
	@-echo "\n\t============== libs DONE: `date` ======================"

inslibs1: $(LIBS1)
	@-for i in $(LIBS1); \
	do \
		cd $$i; \
		echo "$(MAKE) -$(MAKEFLAGS)f $$i.mk install" ; \
		$(MAKE) -$(MAKEFLAGS)f $$i.mk install; \
		cd ..; \
	done
			
inslibs2: $(LIBS2)
	@-for i in $(LIBS2); \
	do \
		cd $$i; \
		echo "$(MAKE) -$(MAKEFLAGS)f $$i.mk install" ; \
		$(MAKE) -$(MAKEFLAGS)f $$i.mk install; \
		cd ..; \
	done

inslibs3: $(LIBS3)
	@-for i in $(LIBS3); \
	do \
		cd $$i; \
		echo "$(MAKE) -$(MAKEFLAGS)f $$i.mk install" ; \
		$(MAKE) -$(MAKEFLAGS)f $$i.mk install; \
		cd ..; \
	done

$(LIBS1):
	@echo CANNOT FIND LIBRARY SOURCE DIRECTORY `pwd`/$@

$(LIBS2):
	@echo CANNOT FIND LIBRARY SOURCE DIRECTORY `pwd`/$@

$(LIBS3):
	@echo CANNOT FIND LIBRARY SOURCE DIRECTORY `pwd`/$@

dirs:
	@-for i in /lib lib/libp /$(SGS)lib /$(SGS)lib/libp 		   \
		/usr/lib /usr/$(SGS)lib ;\
	do								   \
		if [ ! -d $(ROOT)/$$i ]					  ;\
			then mkdir $(ROOT)/$$i				  ;\
			echo mkdir $(ROOT)/$$i				  ;\
		fi					  		  ;\
		chmod 755 $(ROOT)/$$i					  ;\
		chgrp bin $(ROOT)/$$i					  ;\
		chown bin $(ROOT)/$$i					  ;\
	done
