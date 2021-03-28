#ident	"@(#)libF77.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT: ident	"libF77:libF77.mk	1.10.1.1"

# f77(1) FORTRAN INTRINSIC FUNCTIONS library makefile

IROOT =
PROF=
include $(IROOT)/macdefs

INSDIR = $(ROOT)/usr/$(SGS)lib
STRIP = strip
LIB=$(ROOT)/$(SGS)lib
RM=/bin/rm
LIBP=$(ROOT)/$(SGS)lib/libp
VARIANT = F77
LIBNAME = lib$(VARIANT).a

OBJ =	$(LIBNAME)(Version.o) \
	$(LIBNAME)(main.o) \
	$(LIBNAME)(s_rnge.o) \
	$(LIBNAME)(abort_.o) \
	$(LIBNAME)(getarg_.o) \
	$(LIBNAME)(iargc_.o) \
	$(LIBNAME)(getenv_.o) \
	$(LIBNAME)(signal_.o) \
	$(LIBNAME)(s_stop.o) \
	$(LIBNAME)(s_paus.o) \
	$(LIBNAME)(system_.o) \
	$(LIBNAME)(pow_ci.o) \
	$(LIBNAME)(pow_dd.o) \
	$(LIBNAME)(pow_di.o) \
	$(LIBNAME)(pow_hh.o) \
	$(LIBNAME)(pow_ii.o) \
	$(LIBNAME)(pow_ri.o) \
	$(LIBNAME)(pow_zi.o) \
	$(LIBNAME)(pow_zz.o) \
	$(LIBNAME)(c_abs.o) \
	$(LIBNAME)(c_cos.o) \
	$(LIBNAME)(c_div.o) \
	$(LIBNAME)(c_exp.o) \
	$(LIBNAME)(c_log.o) \
	$(LIBNAME)(c_sin.o) \
	$(LIBNAME)(c_sqrt.o) \
	$(LIBNAME)(z_abs.o) \
	$(LIBNAME)(z_cos.o) \
	$(LIBNAME)(z_div.o) \
	$(LIBNAME)(z_exp.o) \
	$(LIBNAME)(z_log.o) \
	$(LIBNAME)(z_sin.o) \
	$(LIBNAME)(z_sqrt.o) \
	$(LIBNAME)(r_abs.o) \
	$(LIBNAME)(r_acos.o) \
	$(LIBNAME)(r_asin.o) \
	$(LIBNAME)(r_atan.o) \
	$(LIBNAME)(r_atn2.o) \
	$(LIBNAME)(r_cnjg.o) \
	$(LIBNAME)(r_cos.o) \
	$(LIBNAME)(r_cosh.o) \
	$(LIBNAME)(r_dim.o) \
	$(LIBNAME)(r_exp.o) \
	$(LIBNAME)(r_imag.o) \
	$(LIBNAME)(r_int.o) \
	$(LIBNAME)(r_lg10.o) \
	$(LIBNAME)(r_log.o) \
	$(LIBNAME)(r_mod.o) \
	$(LIBNAME)(r_nint.o) \
	$(LIBNAME)(r_sign.o) \
	$(LIBNAME)(r_sin.o) \
	$(LIBNAME)(r_sinh.o) \
	$(LIBNAME)(r_sqrt.o) \
	$(LIBNAME)(r_tan.o) \
	$(LIBNAME)(r_tanh.o) \
	$(LIBNAME)(rand_.o) \
	$(LIBNAME)(d_abs.o) \
	$(LIBNAME)(d_acos.o) \
	$(LIBNAME)(d_asin.o) \
	$(LIBNAME)(d_atan.o) \
	$(LIBNAME)(d_atn2.o) \
	$(LIBNAME)(d_cnjg.o) \
	$(LIBNAME)(d_cos.o) \
	$(LIBNAME)(d_cosh.o) \
	$(LIBNAME)(d_dim.o) \
	$(LIBNAME)(d_exp.o) \
	$(LIBNAME)(d_imag.o) \
	$(LIBNAME)(d_int.o) \
	$(LIBNAME)(d_lg10.o) \
	$(LIBNAME)(d_log.o) \
	$(LIBNAME)(d_mod.o) \
	$(LIBNAME)(d_nint.o) \
	$(LIBNAME)(d_prod.o) \
	$(LIBNAME)(d_sign.o) \
	$(LIBNAME)(d_sin.o) \
	$(LIBNAME)(d_sinh.o) \
	$(LIBNAME)(d_sqrt.o) \
	$(LIBNAME)(d_tan.o) \
	$(LIBNAME)(d_tanh.o) \
	$(LIBNAME)(i_abs.o) \
	$(LIBNAME)(i_dim.o) \
	$(LIBNAME)(i_dnnt.o) \
	$(LIBNAME)(i_indx.o) \
	$(LIBNAME)(i_len.o) \
	$(LIBNAME)(i_mod.o) \
	$(LIBNAME)(i_nint.o) \
	$(LIBNAME)(i_sign.o) \
	$(LIBNAME)(h_abs.o) \
	$(LIBNAME)(h_dim.o) \
	$(LIBNAME)(h_dnnt.o) \
	$(LIBNAME)(h_indx.o) \
	$(LIBNAME)(h_len.o) \
	$(LIBNAME)(h_mod.o) \
	$(LIBNAME)(h_nint.o) \
	$(LIBNAME)(h_sign.o) \
	$(LIBNAME)(l_ge.o) \
	$(LIBNAME)(l_gt.o) \
	$(LIBNAME)(l_le.o) \
	$(LIBNAME)(l_lt.o) \
	$(LIBNAME)(hl_ge.o) \
	$(LIBNAME)(hl_gt.o) \
	$(LIBNAME)(hl_le.o) \
	$(LIBNAME)(hl_lt.o) \
	$(LIBNAME)(ef1asc_.o) \
	$(LIBNAME)(ef1cmc_.o) \
	$(LIBNAME)(s_cat.o) \
	$(LIBNAME)(s_cmp.o) \
	$(LIBNAME)(s_copy.o) \
	$(LIBNAME)(erf_.o) \
	$(LIBNAME)(erfc_.o) \
	$(LIBNAME)(derf_.o) \
	$(LIBNAME)(derfc_.o) \
	$(LIBNAME)(cabs.o) \
	$(LIBNAME)(tanh.o) \
	$(LIBNAME)(sinh.o) \
	$(LIBNAME)(mclock_.o) \
	$(LIBNAME)(outstr_.o) \
	$(LIBNAME)(subout.o) \
	$(LIBNAME)(and_l.o) \
	$(LIBNAME)(or_l.o) \
	$(LIBNAME)(xor_l.o) \
	$(LIBNAME)(not_l.o) \
	$(LIBNAME)(shift_l.o) \
	$(LIBNAME)(shiftc_l.o) \
	$(LIBNAME)(bclr_l.o) \
	$(LIBNAME)(bext_l.o) \
	$(LIBNAME)(bset_l.o) \
	$(LIBNAME)(btest_l.o) \
	$(LIBNAME)(mvbits.o) \
	$(LIBNAME)(bittab.o) \
	$(LIBNAME)(and_h.o) \
	$(LIBNAME)(or_h.o) \
	$(LIBNAME)(xor_h.o) \
	$(LIBNAME)(not_h.o) \
	$(LIBNAME)(shift_h.o) \
	$(LIBNAME)(shiftc_h.o) \
	$(LIBNAME)(bclr_h.o) \
	$(LIBNAME)(bext_h.o) \
	$(LIBNAME)(bset_h.o) \
	$(LIBNAME)(btest_h.o)
.c.o:;

all: $(LIBNAME)

$(LIBNAME):	$(OBJ)
		$(CC) -c $(CFLAGS) $(?:.o=.c)
		$(AR) r $(LIBNAME) $?
		$(PROF)$(CC) -p -c $(CFLAGS) $(?:.o=.c)
		$(PROF)$(AR) r libp.$(LIBNAME) $?
		$(RM) $?

$(OBJ): complex bit.h

.c.a:;

install: all
	$(INS) $(LIBNAME) $(LIB)/$(LIBNAME)
	$(AR) ts $(LIB)/$(LIBNAME) >/dev/null
	chmod	644 $(LIB)/$(LIBNAME)
	chgrp	bin $(LIB)/$(LIBNAME)
	chown	bin $(LIB)/$(LIBNAME)
	$(PROF)$(INS) libp.$(LIBNAME) $(LIBP)/$(LIBNAME)
	$(PROF)$(AR) ts $(LIBP)/$(LIBNAME) >/dev/null
	$(PROF)chmod	644 $(LIBP)/$(LIBNAME)
	$(PROF)chgrp	bin $(LIBP)/$(LIBNAME)
	$(PROF)chown	bin $(LIBP)/$(LIBNAME)
	
clean:
	-rm -f *.o

clobber:	clean
	-rm -f $(LIBNAME) libp.$(LIBNAME) greplist tags

SOURCES= \
d_cosh.c        h_sign.c        outstr_.c       rand_.c \
Version.c       d_dim.c         hl_ge.c         pow_ci.c        s_cat.c \
abort_.c        d_exp.c         hl_gt.c         pow_dd.c        s_cmp.c \
and_h.c         d_imag.c        hl_le.c         pow_di.c        s_copy.c \
and_l.c         d_int.c         hl_lt.c         pow_hh.c        s_paus.c \
bclr_h.c        d_lg10.c        i_abs.c         pow_ii.c        s_rnge.c \
bclr_l.c        d_log.c         i_dim.c         pow_ri.c        s_stop.c \
bext_h.c        d_mod.c         i_dnnt.c        pow_zi.c        shift_h.c \
bext_l.c        d_nint.c        i_indx.c        pow_zz.c        shift_l.c \
bit.h           d_prod.c        i_len.c         r_abs.c         shiftc_h.c \
bittab.c        d_sign.c        i_mod.c         r_acos.c        shiftc_l.c \
bset_h.c        d_sin.c         i_nint.c        r_asin.c        signal_.c \
bset_l.c        d_sinh.c        i_sign.c        r_atan.c        sinh.c \
btest_h.c       d_sqrt.c        iargc_.c        r_atn2.c        subout.c \
btest_l.c       d_tan.c         l_ge.c          r_cnjg.c        system_.c \
c_abs.c         d_tanh.c        l_gt.c          r_cos.c         tanh.c \
c_cos.c         derf_.c         l_le.c          r_cosh.c         \
c_div.c         derfc_.c        l_lt.c          r_dim.c         xor_h.c \
c_exp.c         ef1asc_.c       r_exp.c         xor_l.c \
c_log.c         ef1cmc_.c       r_imag.c         \
c_sin.c         erf_.c          r_int.c         z_abs.c \
c_sqrt.c        erfc_.c         r_lg10.c        z_cos.c \
cabs.c          getarg_.c       r_log.c         z_div.c \
complex         getenv_.c       main.c          r_mod.c         z_exp.c \
d_abs.c         h_abs.c         mclock_.c       r_nint.c        z_log.c \
d_acos.c        h_dim.c         mvbits.c        r_sign.c        z_sin.c \
d_asin.c        h_dnnt.c        r_sin.c         z_sqrt.c \
d_atan.c        h_indx.c        not_h.c         r_sinh.c \
d_atn2.c        h_len.c         not_l.c         r_sqrt.c \
d_cnjg.c        h_mod.c         or_h.c          r_tan.c \
d_cos.c         h_nint.c        or_l.c          r_tanh.c 

ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
