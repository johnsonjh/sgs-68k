#
#	fpsp.mk 3.2 3/27/91
#
#		Copyright (C) Motorola, Inc. 1990
#			All Rights Reserved
#
#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF MOTOROLA 
#	The copyright notice above does not evidence any  
#	actual or intended publication of such source code.
#

IROOT =
PROF  =
include $(IROOT)/macdefs
include $(IROOT)/sgsdefs

LIB=$(ROOT)/$(SGS)lib
LIBP=$(ROOT)/$(SGS)lib/libp

OS_TARGET  = $(LIBDIR)/lib.fpsp
LIB_TARGET = libFPSP.a

.s.o:
	cat fpsp.defs $*.s >temp.s
	$(AS) temp.s
	mv temp.o $*.o

OBJS =	bindec.o binstr.o decbin.o do_func.o gen_except.o get_op.o \
	kernel_ex.o res_func.o round.o sacos.o sasin.o satan.o satanh.o \
	scosh.o setox.o sgetem.o sint.o unix.o slog2.o slogn.o \
	smovecr.o srem_mod.o scale.o \
	ssin.o ssinh.o stan.o stanh.o sto_res.o stwotox.o tbldo.o util.o \
	x_bsun.o x_fline.o x_operr.o x_ovfl.o x_snan.o x_store.o \
	x_unfl.o x_unimp.o x_unsupp.o bugfix.o

LIB_OBJS = l_entry.o l_do_func.o l_round.o l_sacos.o l_sasin.o l_satan.o \
	l_satanh.o l_scale.o l_scosh.o l_setox.o l_sgetem.o l_sint.o \
	l_slog2.o l_slogn.o l_srem_mod.o l_ssin.o l_ssinh.o l_stan.o \
	l_stanh.o l_stwotox.o l_support.o

all:	$(LIB_TARGET)

fpsp.o:	$(OBJS)
	$(LD) -r -o fpsp.o $(OBJS)

$(LIB_TARGET):	$(LIB_OBJS)
	$(AR) ruv $(LIB_TARGET) $(LIB_OBJS)

$(OS_TARGET):	fpsp.o
	$(AR) ruv $(OS_TARGET) fpsp.o

install:	all
	$(INS) libFPSP.a $(LIB)/libFPSP.a
	$(AR) ts $(LIB)/libFPSP.a >/dev/null
	chmod 644 $(LIB)/libFPSP.a
	chgrp bin $(LIB)/libFPSP.a
	chown bin $(LIB)/libFPSP.a
	$(PROF)$(INS) libFPSP.a $(LIBP)/libFPSP.a
	$(PROF)$(AR) ts $(LIBP)/libFPSP.a >/dev/null
	$(PROF)chmod 644 $(LIBP)/libFPSP.a
	$(PROF)chgrp bin $(LIBP)/libFPSP.a
	$(PROF)chown bin $(LIBP)/libFPSP.a

#
# Just about every file needs fpsp.defs, so...
#
$(OBJS): fpsp.defs

#
# Library versions need l_fpsp.defs, not fpsp.defs so they each
# need a separate rule.
#
l_entry.o:	l_entry.s l_fpsp.defs
	cat l_fpsp.defs $*.s >temp.s; $(AS) temp.s; mv temp.o $*.o

l_do_func.o:	l_do_func.s l_fpsp.defs
	cat l_fpsp.defs $*.s >temp.s; $(AS) temp.s; mv temp.o $*.o

l_round.o:	l_round.s l_fpsp.defs
	cat l_fpsp.defs $*.s >temp.s; $(AS) temp.s; mv temp.o $*.o

l_sacos.o:	l_sacos.s l_fpsp.defs
	cat l_fpsp.defs $*.s >temp.s; $(AS) temp.s; mv temp.o $*.o

l_sasin.o:	l_sasin.s l_fpsp.defs
	cat l_fpsp.defs $*.s >temp.s; $(AS) temp.s; mv temp.o $*.o

l_satan.o:	l_satan.s l_fpsp.defs
	cat l_fpsp.defs $*.s >temp.s; $(AS) temp.s; mv temp.o $*.o

l_satanh.o:	l_satanh.s l_fpsp.defs
	cat l_fpsp.defs $*.s >temp.s; $(AS) temp.s; mv temp.o $*.o

l_scale.o:	l_scale.s l_fpsp.defs
	cat l_fpsp.defs $*.s >temp.s; $(AS) temp.s; mv temp.o $*.o

l_scosh.o:	l_scosh.s l_fpsp.defs
	cat l_fpsp.defs $*.s >temp.s; $(AS) temp.s; mv temp.o $*.o

l_setox.o:	l_setox.s l_fpsp.defs
	cat l_fpsp.defs $*.s >temp.s; $(AS) temp.s; mv temp.o $*.o

l_sgetem.o:	l_sgetem.s l_fpsp.defs
	cat l_fpsp.defs $*.s >temp.s; $(AS) temp.s; mv temp.o $*.o

l_sint.o:	l_sint.s l_fpsp.defs
	cat l_fpsp.defs $*.s >temp.s; $(AS) temp.s; mv temp.o $*.o

l_slog2.o:	l_slog2.s l_fpsp.defs
	cat l_fpsp.defs $*.s >temp.s; $(AS) temp.s; mv temp.o $*.o

l_slogn.o:	l_slogn.s l_fpsp.defs
	cat l_fpsp.defs $*.s >temp.s; $(AS) temp.s; mv temp.o $*.o

l_srem_mod.o:	l_srem_mod.s l_fpsp.defs
	cat l_fpsp.defs $*.s >temp.s; $(AS) temp.s; mv temp.o $*.o

l_ssin.o:	l_ssin.s l_fpsp.defs
	cat l_fpsp.defs $*.s >temp.s; $(AS) temp.s; mv temp.o $*.o

l_ssinh.o:	l_ssinh.s l_fpsp.defs
	cat l_fpsp.defs $*.s >temp.s; $(AS) temp.s; mv temp.o $*.o

l_stan.o:	l_stan.s  l_fpsp.defs
	cat l_fpsp.defs $*.s >temp.s; $(AS) temp.s; mv temp.o $*.o

l_stanh.o:	l_stanh.s l_fpsp.defs
	cat l_fpsp.defs $*.s >temp.s; $(AS) temp.s; mv temp.o $*.o

l_stwotox.o:	l_stwotox.s l_fpsp.defs
	cat l_fpsp.defs $*.s >temp.s; $(AS) temp.s; mv temp.o $*.o

l_support.o:	l_support.s l_fpsp.defs
	cat l_fpsp.defs $*.s >temp.s; $(AS) temp.s; mv temp.o $*.o

clean:
	-rm -f $(OBJS) fpsp.o temp.s
	-rm -f $(LIB_OBJS)

clobber:	clean
	-rm -f $(OS_TARGET)
	-rm -f $(LIB_TARGET)

