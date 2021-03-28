#ident	"@(#)math.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
PROF=
NONPROF=

include $(IROOT)/macdefs
include $(IROOT)/sgsdefs

SYSTRAP_H=$(IROOT)/systrap

#POSIX
SVID_FLAG=

M4=m4 $(M4FLAGS) ../m4.def

DEFLIST= -DM32
SDEFLIST= -DMCOUNT=#

LORDER=$(PFX)lorder
LIB=$(ROOT)/$(SGS)lib
LIBP=$(ROOT)/$(SGS)lib/libp

.SUFFIXES: .p .O .P

OBJECTS= \
_asin881.o   _sin881.o    asin.o       fmod.o       pow.o        tanh.o \
_atan881.o   _sinh881.o   atan.o       frexp.o      sin.o \
_etox881.o   _sqrt881.o   exp.o        ldexp.o      sinh.o \
_log881.o    _tan881.o    fabs.o       log.o        sqrt.o \
_matherr.o   _tanh881.o   floor.o      modf.o       tan.o

all:	$(OBJECTS) build_lib 

.c.o .c.p .c.O .c.P .c.e .c.f .c.E .c.F:
	$(PROF)@echo $*.c:
	$(PROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p -M2 $*.c \
		&& mv $(*F).o $*.p && cp $*.p $*.f
	$(PROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p $*.c \
		&& mv $(*F).o $*.P && cp $*.P $*.F
	$(NONPROF)@echo $*.c:
	$(NONPROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c $*.c \
		&& mv $(*F).o $*.O && cp $*.O $*.E
	$(NONPROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -M2 $*.c \
		&& cp $(*F).o $*.e


.s.o .s.p .s.O .s.P .s.e .s.f .s.E .s.F:
	$(NONPROF)@echo $*.s:
	# non-emulated
	$(NONPROF)$(M4) -D__STRICT_STDC__ $(SDEFLIST) $*.s   > $*.m4.s   
	$(NONPROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -M2 $*.m4.s \
		&& mv $*.m4.o $*.o
	$(NONPROF)$(M4) $(SDEFLIST) $*.s   > $*.m4.s   
	$(NONPROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c $*.m4.s \
		&& mv $*.m4.o $*.O
	# emulated
	$(NONPROF)$(M4) -D__STRICT_STDC__ -DEMU $(SDEFLIST) $*.s   > $*.m4.s   
	$(NONPROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -M2 $*.m4.s \
		&& mv $*.m4.o $*.e
	$(NONPROF)$(M4) -DEMU $(SDEFLIST) $*.s   > $*.m4.s   
	$(NONPROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c $*.m4.s \
		&& mv $*.m4.o $*.E
	$(PROF)@echo $*.s:
	#non-emulated
	$(PROF)$(M4) -D__STRICT_STDC__ ../mcount.def  $*.s   > $*.m4.s 
	$(PROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p -M2 $*.m4.s  \
		&& mv $*.m4.o $*.p
	$(PROF)$(M4) ../mcount.def  $*.s   > $*.m4.s 
	$(PROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p $*.m4.s  \
		&& mv $*.m4.o $*.P
	#emulated
	$(PROF)$(M4) -D__STRICT_STDC__ -DEMU ../mcount.def  $*.s   > $*.m4.s 
	$(PROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p -M2 $*.m4.s  \
		&& mv $*.m4.o $*.f
	$(PROF)$(M4) -DEMU ../mcount.def  $*.s   > $*.m4.s 
	$(PROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p $*.m4.s  \
		&& mv $*.m4.o $*.F
	-rm $*.m4.s

build_lib:
#Ansi non-emulated
	$(NONPROF)$(LORDER) *.o | tsort >objlist;
	$(NONPROF)$(AR) q libcam.a `cat objlist`
	$(PROF)$(LORDER) *.p | tsort >objlist;
	$(PROF)$(AR) q libp.libcam.a `cat objlist`
	#Ansi emulated
	$(NONPROF)$(LORDER) *.e | tsort >objlist;
	$(NONPROF)$(AR) q libcame.a `cat objlist`
	$(PROF)$(LORDER) *.f | tsort >objlist;
	$(PROF)$(AR) q libp.libcame.a `cat objlist`
	#POSIX non-emulated
	$(NONPROF)$(LORDER) *.O | tsort >objlist;
	$(NONPROF)$(AR) q libcm.a `cat objlist`
	$(PROF)$(LORDER) *.P | tsort >objlist;
	$(PROF)$(AR) q libp.libcm.a `cat objlist`
	#POSIX emulated
	$(NONPROF)$(LORDER) *.E | tsort >objlist;
	$(NONPROF)$(AR) q libcme.a `cat objlist`
	$(PROF)$(LORDER) *.F | tsort >objlist;
	$(PROF)$(AR) q libp.libcme.a `cat objlist`
	-rm -f objlist
	# Ansi
	$(NONPROF)$(INS) libcam.a $(LIB)/libcam.a
	$(NONPROF)$(AR) ts $(LIB)/libcam.a  >/dev/null
	$(NONPROF)chmod 644 $(LIB)/libcam.a
	$(NONPROF)chgrp bin $(LIB)/libcam.a
	$(NONPROF)chown bin $(LIB)/libcam.a
	$(NONPROF)$(INS) libcame.a $(LIB)/libcame.a
	$(NONPROF)$(AR) ts $(LIB)/libcame.a  >/dev/null
	$(NONPROF)chmod 644 $(LIB)/libcame.a
	$(NONPROF)chgrp bin $(LIB)/libcame.a
	$(NONPROF)chown bin $(LIB)/libcame.a
	$(PROF)$(INS) libp.libcam.a $(LIBP)/libcam.a
	$(PROF)$(AR) ts $(LIBP)/libcam.a  >/dev/null
	$(PROF)chmod 644 $(LIBP)/libcam.a
	$(PROF)chgrp bin $(LIBP)/libcam.a
	$(PROF)chown bin $(LIBP)/libcam.a
	$(PROF)$(INS) libp.libcame.a $(LIBP)/libcame.a
	$(PROF)$(AR) ts $(LIBP)/libcame.a  >/dev/null
	$(PROF)chmod 644 $(LIBP)/libcame.a
	$(PROF)chgrp bin $(LIBP)/libcame.a
	$(PROF)chown bin $(LIBP)/libcame.a
	$(NONPROF)$(INS) libcm.a $(LIB)/libcm.a
	$(NONPROF)$(AR) ts $(LIB)/libcm.a  >/dev/null
	$(NONPROF)chmod 644 $(LIB)/libcm.a
	$(NONPROF)chgrp bin $(LIB)/libcm.a
	$(NONPROF)chown bin $(LIB)/libcm.a
	$(NONPROF)$(INS) libcme.a $(LIB)/libcme.a
	$(NONPROF)$(AR) ts $(LIB)/libcme.a  >/dev/null
	$(NONPROF)chmod 644 $(LIB)/libcme.a
	$(NONPROF)chgrp bin $(LIB)/libcme.a
	$(NONPROF)chown bin $(LIB)/libcme.a
	$(PROF)$(INS) libp.libcm.a $(LIBP)/libcm.a
	$(PROF)$(AR) ts $(LIBP)/libcm.a  >/dev/null
	$(PROF)chmod 644 $(LIBP)/libcm.a
	$(PROF)chgrp bin $(LIBP)/libcm.a
	$(PROF)chown bin $(LIBP)/libcm.a
	$(PROF)$(INS) libp.libcme.a $(LIBP)/libcme.a
	$(PROF)$(AR) ts $(LIBP)/libcme.a  >/dev/null
	$(PROF)chmod 644 $(LIBP)/libcme.a
	$(PROF)chgrp bin $(LIBP)/libcme.a
	$(PROF)chown bin $(LIBP)/libcme.a
	-rm -f *.o *.O
	-rm -f *.p *.P
	-rm -f *.e *.E
	-rm -f *.f *.F
	-rm -f libcam.a libcame.a libcm.a libcme.a
	-rm -f libp.libcam.a libp.libcame.a libp.libcm.a libp.libcme.a

	
clean:
	-rm -f *.o *.O
	-rm -f *.p *.P
	-rm -f *.e *.E
	-rm -f *.f *.F
	-rm -f libcam.a libcame.a libcm.a libcme.a
	-rm -f libp.libcam.a libp.libcame.a libp.libcm.a libp.libcme.a

clobber: clean
