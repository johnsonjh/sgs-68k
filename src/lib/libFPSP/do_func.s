#
#	do_func.sa 3.4 2/18/91
#
# Do_func performs the unimplemented operation.  The operation
# to be performed is determined from the lower 7 bits of the
# extension word (except in the case of fmovecr and fsincos).
# The opcode and tag bits form an index into a jump table in 
# tbldo.sa.  Cases of zero, infinity and NaN are handled in 
# do_func by forcing the default result.  Normalized and
# denormalized (there are no unnormalized numbers at this
# point) are passed onto the emulation code.  
#
# CMDREG1B and STAG are extracted from the fsave frame
# and combined to form the table index.  The function called
# will start with a0 pointing to the ETEMP operand.  Dyadic
# functions can find FPTEMP at -12(a0).
#
# Called functions return their result in fp0.  Sincos returns
# sin(x) in fp0 and cos(x) in fp1.
#

#		Copyright (C) Motorola, Inc. 1990
#			All Rights Reserved
#
#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF MOTOROLA 
#	The copyright notice above does not evidence any  
#	actual or intended publication of such source code.

#	idnt	2,1	Motorola 040 Floating Point Software Package

	text

#	include	fpsp.h	

#	xref	t_dz2	
#	xref	t_operr	
#	xref	t_inx2	
#	xref	t_resdnrm	
#	xref	dst_nan	
#	xref	src_nan	
#	xref	nrm_set	
#	xref	sto_cos	

#	xref	tblpre	
#	xref	slognp1,slogn,slog10,slog2	
#	xref	slognd,slog10d,slog2d	
#	xref	smod,srem	
#	xref	sscale	
#	xref	smovcr	

PONE:
	long		0x3fff0000,0x80000000,0x00000000	# +1
MONE:
	long		0xbfff0000,0x80000000,0x00000000	# -1
PZERO:
	long		0x00000000,0x00000000,0x00000000	# +0
MZERO:
	long		0x80000000,0x00000000,0x00000000	# -0
PINF:
	long		0x7fff0000,0x00000000,0x00000000	# +inf
MINF:
	long		0xffff0000,0x00000000,0x00000000	# -inf
QNAN:
	long		0x7fff0000,0xffffffff,0xffffffff	# non-signaling nan
PPIBY2:
	long		0x3FFF0000,0xC90FDAA2,0x2168C235	# +PI/2
MPIBY2:
	long		0xbFFF0000,0xC90FDAA2,0x2168C235	# -PI/2

	global		do_func
do_func:
	clr.b		CU_ONLY(%a6)
#
# Check for fmovecr.  It does not follow the format of fp gen
# unimplemented instructions.  The test is on the upper 6 bits;
# if they are $17, the inst is fmovecr.  Call entry smovcr
# directly.
#
	bfextu		CMDREG1B(%a6){&0:&6},%d0	# get opclass and src fields
	cmp.l		%d0,&0x17	# if op class and size fields are $17, 
#				;it is FMOVECR; if not, continue
	bne.b		not_fmovecr
	jmp		smovcr		# fmovecr; jmp directly to emulation

not_fmovecr:
	mov.w		CMDREG1B(%a6),%d0
	and.l		&0x7F,%d0
	cmp.l		%d0,&0x38	# if the extension is >= $38, 
	bge.b		serror		# it is illegal
	bfextu		STAG(%a6){&0:&3},%d1
	lsl.l		&3,%d0		# make room for STAG
	add.l		%d1,%d0		# combine for final index into table
	lea.l		tblpre,%a1	# start of monster jump table
	mov.l		(%a1,%d0.w*4),%a1	# real target address
	lea.l		ETEMP(%a6),%a0	# a0 is pointer to src op
	mov.l		USER_FPCR(%a6),%d1
	and.l		&0xFF,%d1	# discard all but rounding mode/prec
	fmov.l		&0,%control
	jmp		(%a1)
#
#	ERROR
#
	global		serror
serror:
	st.b		STORE_FLG(%a6)
	rts
#
# These routines load forced values into fp0.  They are called
# by index into tbldo.
#
# Load a signed zero to fp0 and set inex2/ainex
#
	global		snzrinx
snzrinx:
	btst		&sign_bit,LOCAL_EX(%a0)	# get sign of source operand
	bne.b		ld_mzinx	# if negative, branch
	bsr		ld_pzero	# bsr so we can return and set inx
	bra		t_inx2		# now, set the inx for the next inst
ld_mzinx:
	bsr		ld_mzero	# if neg, load neg zero, return here
	bra		t_inx2		# now, set the inx for the next inst
#
# Load a signed zero to fp0; do not set inex2/ainex 
#
	global		szero
szero:
	btst		&sign_bit,LOCAL_EX(%a0)	# get sign of source operand
	bne		ld_mzero	# if neg, load neg zero
	bra		ld_pzero	# load positive zero
#
# Load a signed infinity to fp0; do not set inex2/ainex 
#
	global		sinf
sinf:
	btst		&sign_bit,LOCAL_EX(%a0)	# get sign of source operand
	bne		ld_minf		# if negative branch
	bra		ld_pinf
#
# Load a signed one to fp0; do not set inex2/ainex 
#
	global		sone
sone:
	btst		&sign_bit,LOCAL_EX(%a0)	# check sign of source
	bne		ld_mone
	bra		ld_pone
#
# Load a signed pi/2 to fp0; do not set inex2/ainex 
#
	global		spi_2
spi_2:
	btst		&sign_bit,LOCAL_EX(%a0)	# check sign of source
	bne		ld_mpi2
	bra		ld_ppi2
#
# Load either a +0 or +inf for plus/minus operand
#
	global		szr_inf
szr_inf:
	btst		&sign_bit,LOCAL_EX(%a0)	# check sign of source
	bne		ld_pzero
	bra		ld_pinf
#
# Result is either an operr or +inf for plus/minus operand
# [Used by slogn, slognp1, slog10, and slog2]
#
	global		sopr_inf
sopr_inf:
	btst		&sign_bit,LOCAL_EX(%a0)	# check sign of source
	bne		t_operr
	bra		ld_pinf
#
#	FLOGNP1 
#
	global		sslognp1
sslognp1:
	fmovm.x		(%a0),&0x80	#  {%fp0}
	fcmp.b		%fp0,&-1
	fbgt		slognp1
	fbeq		t_dz2		# if = -1, divide by zero exception
	fmov.l		&0,%status	# clr N flag
	bra		t_operr		# take care of operands < -1
#
#	FETOXM1
#
	global		setoxm1i
setoxm1i:
	btst		&sign_bit,LOCAL_EX(%a0)	# check sign of source
	bne		ld_mone
	bra		ld_pinf
#
#	FLOGN
#
# Test for 1.0 as an input argument, returning +zero.  Also check
# the sign and return operr if negative.
#
	global		sslogn
sslogn:
	btst		&sign_bit,LOCAL_EX(%a0)
	bne		t_operr		# take care of operands < 0
	cmp.w		LOCAL_EX(%a0),&0x3fff	# test for 1.0 input
	bne		slogn
	cmp.l		LOCAL_HI(%a0),&0x80000000
	bne		slogn
	tst.l		LOCAL_LO(%a0)
	bne		slogn
	fmov.x		PZERO,%fp0
	rts

	global		sslognd
sslognd:
	btst		&sign_bit,LOCAL_EX(%a0)
	beq		slognd
	bra		t_operr		# take care of operands < 0

#
#	FLOG10
#
	global		sslog10
sslog10:
	btst		&sign_bit,LOCAL_EX(%a0)
	bne		t_operr		# take care of operands < 0
	cmp.w		LOCAL_EX(%a0),&0x3fff	# test for 1.0 input
	bne		slog10
	cmp.l		LOCAL_HI(%a0),&0x80000000
	bne		slog10
	tst.l		LOCAL_LO(%a0)
	bne		slog10
	fmov.x		PZERO,%fp0
	rts

	global		sslog10d
sslog10d:
	btst		&sign_bit,LOCAL_EX(%a0)
	beq		slog10d
	bra		t_operr		# take care of operands < 0

#
#	FLOG2
#
	global		sslog2
sslog2:
	btst		&sign_bit,LOCAL_EX(%a0)
	bne		t_operr		# take care of operands < 0
	cmp.w		LOCAL_EX(%a0),&0x3fff	# test for 1.0 input
	bne		slog2
	cmp.l		LOCAL_HI(%a0),&0x80000000
	bne		slog2
	tst.l		LOCAL_LO(%a0)
	bne		slog2
	fmov.x		PZERO,%fp0
	rts

	global		sslog2d
sslog2d:
	btst		&sign_bit,LOCAL_EX(%a0)
	beq		slog2d
	bra		t_operr		# take care of operands < 0

#
#	FMOD
#
pmodt:
#				;$21 fmod
#				;dtag,stag
	long		smod		# 00,00  norm,norm = normal
	long		smod_oper	# 00,01  norm,zero = nan with operr
	long		smod_fpn	# 00,10  norm,inf  = fpn
	long		smod_snan	# 00,11  norm,nan  = nan
	long		smod_zro	# 01,00  zero,norm = +-zero
	long		smod_oper	# 01,01  zero,zero = nan with operr
	long		smod_zro	# 01,10  zero,inf  = +-zero
	long		smod_snan	# 01,11  zero,nan  = nan
	long		smod_oper	# 10,00  inf,norm  = nan with operr
	long		smod_oper	# 10,01  inf,zero  = nan with operr
	long		smod_oper	# 10,10  inf,inf   = nan with operr
	long		smod_snan	# 10,11  inf,nan   = nan
	long		smod_dnan	# 11,00  nan,norm  = nan
	long		smod_dnan	# 11,01  nan,zero  = nan
	long		smod_dnan	# 11,10  nan,inf   = nan
	long		smod_dnan	# 11,11  nan,nan   = nan

	global		pmod
pmod:
	bfextu		STAG(%a6){&0:&3},%d0	# stag = d0
	bfextu		DTAG(%a6){&0:&3},%d1	# dtag = d1

#
# Alias extended denorms to norms for the jump table.
#
	bclr		&2,%d0
	bclr		&2,%d1

	lsl.b		&2,%d1
	or.b		%d0,%d1		# d1{3:2} = dtag, d1{1:0} = stag
#				;Tag values:
#				;00 = norm or denorm
#				;01 = zero
#				;10 = inf
#				;11 = nan
	lea		pmodt,%a1
	mov.l		(%a1,%d1.w*4),%a1
	jmp		(%a1)

smod_snan:
	bra		src_nan
smod_dnan:
	bra		dst_nan
smod_oper:
	bra		t_operr
smod_zro:
	mov.b		ETEMP(%a6),%d1	# get sign of src op
	mov.b		FPTEMP(%a6),%d0	# get sign of dst op
	eor.b		%d0,%d1		# get exor of sign bits
	btst		&7,%d1		# test for sign
	beq.b		smod_zsn	# if clr, do not set sign big
	bset		&q_sn_bit,FPSR_QBYTE(%a6)	# set q-byte sign bit
smod_zsn:
	btst		&7,%d0		# test if + or -
	beq		ld_pzero	# if pos then load +0
	bra		ld_mzero	# else neg load -0
smod_fpn:
	mov.b		ETEMP(%a6),%d1	# get sign of src op
	mov.b		FPTEMP(%a6),%d0	# get sign of dst op
	eor.b		%d0,%d1		# get exor of sign bits
	btst		&7,%d1		# test for sign
	beq.b		smod_fsn	# if clr, do not set sign big
	bset		&q_sn_bit,FPSR_QBYTE(%a6)	# set q-byte sign bit
smod_fsn:
	tst.b		DTAG(%a6)	# filter out denormal destination case
	bpl.b		smod_nrm
	lea.l		FPTEMP(%a6),%a0	# a0<- addr(FPTEMP)
	bra		t_resdnrm	# force UNFL(but exact) result
smod_nrm:
	fmov.l		USER_FPCR(%a6),%control	# use user's rmode and precision
	fmov.x		FPTEMP(%a6),%fp0	# return dest to fp0
	rts
#
#	FREM
#
premt:
#				;$25 frem
#				;dtag,stag
	long		srem		# 00,00  norm,norm = normal
	long		srem_oper	# 00,01  norm,zero = nan with operr
	long		srem_fpn	# 00,10  norm,inf  = fpn
	long		srem_snan	# 00,11  norm,nan  = nan
	long		srem_zro	# 01,00  zero,norm = +-zero
	long		srem_oper	# 01,01  zero,zero = nan with operr
	long		srem_zro	# 01,10  zero,inf  = +-zero
	long		srem_snan	# 01,11  zero,nan  = nan
	long		srem_oper	# 10,00  inf,norm  = nan with operr
	long		srem_oper	# 10,01  inf,zero  = nan with operr
	long		srem_oper	# 10,10  inf,inf   = nan with operr
	long		srem_snan	# 10,11  inf,nan   = nan
	long		srem_dnan	# 11,00  nan,norm  = nan
	long		srem_dnan	# 11,01  nan,zero  = nan
	long		srem_dnan	# 11,10  nan,inf   = nan
	long		srem_dnan	# 11,11  nan,nan   = nan

	global		prem
prem:

	bfextu		STAG(%a6){&0:&3},%d0	# stag = d0
	bfextu		DTAG(%a6){&0:&3},%d1	# dtag = d1
#
# Alias extended denorms to norms for the jump table.
#
	bclr		&2,%d0
	bclr		&2,%d1

	lsl.b		&2,%d1
	or.b		%d0,%d1		# d1{3:2} = dtag, d1{1:0} = stag
#				;Tag values:
#				;00 = norm or denorm
#				;01 = zero
#				;10 = inf
#				;11 = nan
	lea		premt,%a1
	mov.l		(%a1,%d1.w*4),%a1
	jmp		(%a1)
srem_snan:
	bra		src_nan
srem_dnan:
	bra		dst_nan
srem_oper:
	bra		t_operr
srem_zro:
	mov.b		ETEMP(%a6),%d1	# get sign of src op
	mov.b		FPTEMP(%a6),%d0	# get sign of dst op
	eor.b		%d0,%d1		# get exor of sign bits
	btst		&7,%d1		# test for sign
	beq.b		srem_zsn	# if clr, do not set sign big
	bset		&q_sn_bit,FPSR_QBYTE(%a6)	# set q-byte sign bit
srem_zsn:
	btst		&7,%d0		# test if + or -
	beq		ld_pzero	# if pos then load +0
	bra		ld_mzero	# else neg load -0
srem_fpn:
	mov.b		ETEMP(%a6),%d1	# get sign of src op
	mov.b		FPTEMP(%a6),%d0	# get sign of dst op
	eor.b		%d0,%d1		# get exor of sign bits
	btst		&7,%d1		# test for sign
	beq.b		srem_fsn	# if clr, do not set sign big
	bset		&q_sn_bit,FPSR_QBYTE(%a6)	# set q-byte sign bit
srem_fsn:
	tst.b		DTAG(%a6)	# filter out denormal destination case
	bpl.b		srem_nrm
	lea.l		FPTEMP(%a6),%a0	# a0<- addr(FPTEMP)
	bra		t_resdnrm	# force UNFL(but exact) result
srem_nrm:
	fmov.l		USER_FPCR(%a6),%control	# use user's rmode and precision
	fmov.x		FPTEMP(%a6),%fp0	# return dest to fp0
	rts
#
#	FSCALE
#
pscalet:
#				;$26 fscale
#				;dtag,stag
	long		sscale		# 00,00  norm,norm = result
	long		sscale		# 00,01  norm,zero = fpn
	long		scl_opr		# 00,10  norm,inf  = nan with operr
	long		scl_snan	# 00,11  norm,nan  = nan
	long		scl_zro		# 01,00  zero,norm = +-zero
	long		scl_zro		# 01,01  zero,zero = +-zero
	long		scl_opr		# 01,10  zero,inf  = nan with operr
	long		scl_snan	# 01,11  zero,nan  = nan
	long		scl_inf		# 10,00  inf,norm  = +-inf
	long		scl_inf		# 10,01  inf,zero  = +-inf
	long		scl_opr		# 10,10  inf,inf   = nan with operr
	long		scl_snan	# 10,11  inf,nan   = nan
	long		scl_dnan	# 11,00  nan,norm  = nan
	long		scl_dnan	# 11,01  nan,zero  = nan
	long		scl_dnan	# 11,10  nan,inf   = nan
	long		scl_dnan	# 11,11  nan,nan   = nan

	global		pscale
pscale:
	bfextu		STAG(%a6){&0:&3},%d0	# stag in d0
	bfextu		DTAG(%a6){&0:&3},%d1	# dtag in d1
	bclr		&2,%d0		# alias  denorm into norm
	bclr		&2,%d1		# alias  denorm into norm
	lsl.b		&2,%d1
	or.b		%d0,%d1		# d1{4:2} = dtag, d1{1:0} = stag
#				;dtag values     stag values:
#				;000 = norm      00 = norm
#				;001 = zero	 01 = zero
#				;010 = inf	 10 = inf
#				;011 = nan	 11 = nan
#				;100 = dnrm
#
#
	lea.l		pscalet,%a1	# load start of jump table
	mov.l		(%a1,%d1.w*4),%a1	# load a1 with label depending on tag
	jmp		(%a1)		# go to the routine

scl_opr:
	bra		t_operr

scl_dnan:
	bra		dst_nan

scl_zro:
	btst		&sign_bit,FPTEMP_EX(%a6)	# test if + or -
	beq		ld_pzero	# if pos then load +0
	bra		ld_mzero	# if neg then load -0
scl_inf:
	btst		&sign_bit,FPTEMP_EX(%a6)	# test if + or -
	beq		ld_pinf		# if pos then load +inf
	bra		ld_minf		# else neg load -inf
scl_snan:
	bra		src_nan
#
#	FSINCOS
#
	global		ssincosz
ssincosz:
	btst		&sign_bit,ETEMP(%a6)	# get sign
	beq.b		sincosp
	fmov.x		MZERO,%fp0
	bra.b		sincoscom
sincosp:
	fmov.x		PZERO,%fp0
sincoscom:
	fmovm.x		PONE,&0x40	# do not allow FPSR to be affected {%fp1}
	bra		sto_cos		# store cosine result

	global		ssincosi
ssincosi:
	fmov.x		QNAN,%fp1	# load NAN
	bsr		sto_cos		# store cosine result
	fmov.x		QNAN,%fp0	# load NAN
	bra		t_operr

	global		ssincosnan
ssincosnan:
	mov.l		ETEMP_EX(%a6),FP_SCR1(%a6)
	mov.l		ETEMP_HI(%a6),FP_SCR1+4(%a6)
	mov.l		ETEMP_LO(%a6),FP_SCR1+8(%a6)
	bset		&signan_bit,FP_SCR1+4(%a6)
	fmovm.x		FP_SCR1(%a6),&0x40	#  {%fp1}
	bsr		sto_cos
	bra		src_nan
#
# This code forces default values for the zero, inf, and nan cases 
# in the transcendentals code.  The CC bits must be set in the
# stacked FPSR to be correctly reported.
#
#**Returns +PI/2
	global		ld_ppi2
ld_ppi2:
	fmov.x		PPIBY2,%fp0	# load +pi/2
	bra		t_inx2		# set inex2 exc

#**Returns -PI/2
	global		ld_mpi2
ld_mpi2:
	fmov.x		MPIBY2,%fp0	# load -pi/2
	or.l		&neg_mask,USER_FPSR(%a6)	# set N bit
	bra		t_inx2		# set inex2 exc

#**Returns +inf
	global		ld_pinf
ld_pinf:
	fmov.x		PINF,%fp0	# load +inf
	or.l		&inf_mask,USER_FPSR(%a6)	# set I bit
	rts

#**Returns -inf
	global		ld_minf
ld_minf:
	fmov.x		MINF,%fp0	# load -inf
	or.l		&neg_mask+inf_mask,USER_FPSR(%a6)	# set N and I bits
	rts

#**Returns +1
	global		ld_pone
ld_pone:
	fmov.x		PONE,%fp0	# load +1
	rts

#**Returns -1
	global		ld_mone
ld_mone:
	fmov.x		MONE,%fp0	# load -1
	or.l		&neg_mask,USER_FPSR(%a6)	# set N bit
	rts

#**Returns +0
	global		ld_pzero
ld_pzero:
	fmov.x		PZERO,%fp0	# load +0
	or.l		&z_mask,USER_FPSR(%a6)	# set Z bit
	rts

#**Returns -0
	global		ld_mzero
ld_mzero:
	fmov.x		MZERO,%fp0	# load -0
	or.l		&neg_mask+z_mask,USER_FPSR(%a6)	# set N and Z bits
	rts

#	end		
