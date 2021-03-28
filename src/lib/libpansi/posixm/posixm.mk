#ident	"@(#)posixm.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#
# makefile for libansim
#
# Note that the object list given in FILES is in the order in which the files
# should be added to the archive when it is created.  If the files are not
# ordered correctly, linking of libm fails on the pdp11 and is slowed down
# on other machines.

IROOT =
PROF=

include $(IROOT)/macdefs
include $(IROOT)/sgsdefs

# POSIX binaries
SVID_FLAG=

INC=$(INCRT)
LIB=$(ROOT)/$(SGS)lib
LIBP=$(ROOT)/$(SGS)lib/libp

VARIANT = 
LIBNAME = lib$(VARIANT)m.a

SOURCES= \
gamma.c     jn.c        matherr.c hypot.c j0.c erf.c       j1.c

FILES =\
	$(LIBNAME)(erf.o)\
	$(LIBNAME)(gamma.o)\
	$(LIBNAME)(hypot.o)\
	$(LIBNAME)(jn.o)\
	$(LIBNAME)(j0.o)\
	$(LIBNAME)(j1.o)\
	$(LIBNAME)(matherr.o)

# all: $(LIBNAME) libp.$(LIBNAME)
all: $(LIBNAME)

$(LIBNAME): $(FILES)
		
.s.a:
	$(PROF) $(CC) -p $(CFLAGS) -c $*.s
	$(PROF) $(AR) rv libp.$(LIBNAME) $*.o
	$(PROF) mv $*.o $*.p
	$(CC) $(CFLAGS) -c $*.s
	$(AR) rv $@ $*.o
	# rm $*.o

.c.a:
	$(PROF) $(CC) -p $(CFLAGS) -c $*.c
	$(PROF) $(AR) rv libp.$(LIBNAME) $*.o
	$(PROF) mv $*.o $*.p
	$(CC) -I. $(CFLAGS) -c $*.c
	$(AR) rv $@ $*.o
	# rm $*.o

$(FILES): $(INC)/math.h $(INC)/values.h $(INC)/errno.h \
		$(INC)/sys/errno.h

install:	all
	@echo THIS LIB NOT TO BE INSTALLED
#	$(INS) $(LIBNAME) $(LIB)/$(LIBNAME)
#	$(AR) ts  $(LIB)/$(LIBNAME) >/dev/null
#	chmod 644 $(LIB)/$(LIBNAME)
#	chgrp bin $(LIB)/$(LIBNAME)
#	chown bin $(LIB)/$(LIBNAME)
#	$(PROF)if [ ! -d $(LIBP) ] ;\
#	then \
#		$(PROF) mkdir -p $(LIBP); \
#	$(PROF)fi
#	$(PROF)$(INS) libp.$(LIBNAME) $(LIBP)/$(LIBNAME)
#	$(AR) ts $(LIBP)/$(LIBNAME) >/dev/null
#	chmod 644 $(LIBP)/$(LIBNAME)
#	chgrp bin $(LIBP)/$(LIBNAME)
#	chown bin $(LIBP)/$(LIBNAME)

clean:
	-rm -f *.o
	-rm -f *.p

clobber:	clean
	-rm -f $(LIBNAME) libp.$(LIBNAME) greplist tags

ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
