#ident	"@(#)sgs.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
include $(IROOT)/macdefs
include $(IROOT)/sgsdefs

DIRECTS=\
	mcs \
	yacc lex as cmd comp2 coptim cpp ld optim ar compress dis objconv \
	dump list lorder nm size strip mkshlib f77 error maketape

all clean clobber :
	@-for i in $(DIRECTS); \
	do \
		echo "cd $$i; $(MAKE) -$(MAKEFLAGS)f $$i.mk $@"; \
		cd $$i; $(MAKE) -$(MAKEFLAGS)f $$i.mk $@; \
		cd ..; sync; \
	done

install : 
	@-for i in $(DIRECTS); \
	do \
		echo "cd $$i; $(MAKE) -$(MAKEFLAGS)f $$i.mk install "; \
		cd $$i; $(MAKE) -$(MAKEFLAGS)f $$i.mk install ; \
		cd ..; sync; \
	done
	@echo "DONE with sgs install: ==== `date` ===="

nfs nfsa1000 nfs90:
		echo "sgs is not needed for NFS releases"

$(DIRECTS):
	@echo CANNOT FIND DIRECTORY $@
