#
#	res_func.sa 3.9 7/29/91
#
# Normalizes denormalized numbers if necessary and updates the
# stack frame.  The function is then restored back into the
# machine and the 040 completes the operation.  This routine
# is only used by the unsupported data type/format handler.
# (Exception vector 55).
#
# For packed move out (fmove.p fpm,<ea>) the operation is
# completed here; data is packed and moved to user memory. 
# The stack is restored to the 040 only in the case of a
# reportable exception in the conversion.
#
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

sp_bnds:
	short		0x3f81,0x407e
	short		0x3f6a,0x0000
dp_bnds:
	short		0x3c01,0x43fe
	short		0x3bcd,0x0000

#	xref	mem_write	
#	xref	bindec	
#	xref	get_fline	
#	xref	round	
#	xref	denorm	
#	xref	dest_ext	
#	xref	dest_dbl	
#	xref	dest_sgl	
#	xref	unf_sub	
#	xref	nrm_set	
#	xref	dnrm_lp	
#	xref	ovf_res	
#	xref	reg_dest	
#	xref	t_ovfl	
#	xref	t_unfl	

	global		res_func
	global		p_move

res_func:
	clr.b		DNRM_FLG(%a6)
	clr.b		RES_FLG(%a6)
	clr.b		CU_ONLY(%a6)
	tst.b		DY_MO_FLG(%a6)
	beq.b		monadic
dyadic:
	btst		&7,DTAG(%a6)	# if dop = norm=000, zero=001,
#				;inf=010 or nan=011
	beq.b		monadic		# then branch
#				;else denorm
# HANDLE DESTINATION DENORM HERE
#				;set dtag to norm
#				;write the tag & fpte15 to the fstack
	lea.l		FPTEMP(%a6),%a0

	bclr		&sign_bit,LOCAL_EX(%a0)
	sne		LOCAL_SGN(%a0)

	bsr		nrm_set		# normalize number (exp will go negative)
	bclr		&sign_bit,LOCAL_EX(%a0)	# get rid of false sign
	bfclr		LOCAL_SGN(%a0){&0:&8}	# change back to IEEE ext format
	beq.b		dpos
	bset		&sign_bit,LOCAL_EX(%a0)
dpos:
	bfclr		DTAG(%a6){&0:&4}	# set tag to normalized, FPTE15 = 0
	bset		&4,DTAG(%a6)	# set FPTE15
	or.b		&0x0f,DNRM_FLG(%a6)
monadic:
	lea.l		ETEMP(%a6),%a0
	btst		&direction_bit,CMDREG1B(%a6)	# check direction
	bne.w		opclass3	# it is a mv out
#
# At this point, only oplcass 0 and 2 possible
#
	btst		&7,STAG(%a6)	# if sop = norm=000, zero=001,
#				;inf=010 or nan=011
	bne.w		mon_dnrm	# else denorm
	tst.b		DY_MO_FLG(%a6)	# all cases of dyadic instructions would
	bne.w		normal		# require normalization of denorm

# At this point:
#	monadic instructions:	fabs  = $18  fneg   = $1a  ftst   = $3a
#				fmove = $00  fsmove = $40  fdmove = $44
#				fsqrt = $05* fssqrt = $41  fdsqrt = $45
#				(*fsqrt reencoded to $05)
#
	mov.w		CMDREG1B(%a6),%d0	# get command register
	and.l		&0x7f,%d0	# strip to only command word
#
# At this point, fabs, fneg, fsmove, fdmove, ftst, fsqrt, fssqrt, and 
# fdsqrt are possible.
# For cases fabs, fneg, fsmove, and fdmove goto spos (do not normalize)
# For cases fsqrt, fssqrt, and fdsqrt goto nrm_src (do normalize)
#
	btst		&0,%d0
	bne.w		normal		# weed out fsqrt instructions
#
# cu_norm handles fmove in instructions with normalized inputs.
# The routine round is used to correctly round the input for the
# destination precision and mode.
#
cu_norm:
	st		CU_ONLY(%a6)	# set cu-only inst flag
	mov.w		CMDREG1B(%a6),%d0
	and.b		&0x3b,%d0	# isolate bits to select inst
	tst.b		%d0
	beq.l		cu_nmove	# if zero, it is an fmove
	cmp.b		%d0,&0x18
	beq.l		cu_nabs		# if $18, it is fabs
	cmp.b		%d0,&0x1a
	beq.l		cu_nneg		# if $1a, it is fneg
#
# Inst is ftst.  Check the source operand and set the cc's accordingly.
# No write is done, so simply rts.
#
cu_ntst:
	mov.w		LOCAL_EX(%a0),%d0
	bclr		&15,%d0
	sne		LOCAL_SGN(%a0)
	beq.b		cu_ntpo
	or.l		&neg_mask,USER_FPSR(%a6)	# set N
cu_ntpo:
	cmp.w		%d0,&0x7fff	# test for inf/nan
	bne.b		cu_ntcz
	tst.l		LOCAL_HI(%a0)
	bne.b		cu_ntn
	tst.l		LOCAL_LO(%a0)
	bne.b		cu_ntn
	or.l		&inf_mask,USER_FPSR(%a6)
	rts
cu_ntn:
	or.l		&nan_mask,USER_FPSR(%a6)
	mov.l		ETEMP_EX(%a6),FPTEMP_EX(%a6)	# set up fptemp sign for 
#						;snan handler

	rts
cu_ntcz:
	tst.l		LOCAL_HI(%a0)
	bne.l		cu_ntsx
	tst.l		LOCAL_LO(%a0)
	bne.l		cu_ntsx
	or.l		&z_mask,USER_FPSR(%a6)
cu_ntsx:
	rts
#
# Inst is fabs.  Execute the absolute value function on the input.
# Branch to the fmove code.  If the operand is NaN, do nothing.
#
cu_nabs:
	mov.b		STAG(%a6),%d0
	btst		&5,%d0		# test for NaN or zero
	bne		wr_etemp	# if either, simply write it
	bclr		&7,LOCAL_EX(%a0)	# do abs
	bra.b		cu_nmove	# fmove code will finish
#
# Inst is fneg.  Execute the negate value function on the input.
# Fall though to the fmove code.  If the operand is NaN, do nothing.
#
cu_nneg:
	mov.b		STAG(%a6),%d0
	btst		&5,%d0		# test for NaN or zero
	bne		wr_etemp	# if either, simply write it
	bchg		&7,LOCAL_EX(%a0)	# do neg
#
# Inst is fmove.  This code also handles all result writes.
# If bit 2 is set, round is forced to double.  If it is clear,
# and bit 6 is set, round is forced to single.  If both are clear,
# the round precision is found in the fpcr.  If the rounding precision
# is double or single, round the result before the write.
#
cu_nmove:
	mov.b		STAG(%a6),%d0
	and.b		&0xe0,%d0	# isolate stag bits
	bne		wr_etemp	# if not norm, simply write it
	btst		&2,CMDREG1B+1(%a6)	# check for rd
	bne		cu_nmrd
	btst		&6,CMDREG1B+1(%a6)	# check for rs
	bne		cu_nmrs
#
# The move or operation is not with forced precision.  Test for
# nan or inf as the input; if so, simply write it to FPn.  Use the
# FPCR_MODE byte to get rounding on norms and zeros.
#
cu_nmnr:
	bfextu		FPCR_MODE(%a6){&0:&2},%d0
	tst.b		%d0		# check for extended
	beq		cu_wrexn	# if so, just write result
	cmp.b		%d0,&1		# check for single
	beq		cu_nmrs		# fall through to double
#
# The move is fdmove or round precision is double.
#
cu_nmrd:
	mov.l		&2,%d0		# set up the size for denorm
	mov.w		LOCAL_EX(%a0),%d1	# compare exponent to double threshold
	and.w		&0x7fff,%d1
	cmp.w		%d1,&0x3c01
	bls		cu_nunfl
	bfextu		FPCR_MODE(%a6){&2:&2},%d1	# get rmode
	or.l		&0x00020000,%d1	# or in rprec (double)
	clr.l		%d0		# clear g,r,s for round
	bclr		&sign_bit,LOCAL_EX(%a0)	# convert to internal format
	sne		LOCAL_SGN(%a0)
	bsr.l		round
	bfclr		LOCAL_SGN(%a0){&0:&8}
	beq.b		cu_nmrdc
	bset		&sign_bit,LOCAL_EX(%a0)
cu_nmrdc:
	mov.w		LOCAL_EX(%a0),%d1	# check for overflow
	and.w		&0x7fff,%d1
	cmp.w		%d1,&0x43ff
	bge		cu_novfl	# take care of overflow case
	bra.w		cu_wrexn
#
# The move is fsmove or round precision is single.
#
cu_nmrs:
	mov.l		&1,%d0
	mov.w		LOCAL_EX(%a0),%d1
	and.w		&0x7fff,%d1
	cmp.w		%d1,&0x3f81
	bls		cu_nunfl
	bfextu		FPCR_MODE(%a6){&2:&2},%d1
	or.l		&0x00010000,%d1
	clr.l		%d0
	bclr		&sign_bit,LOCAL_EX(%a0)
	sne		LOCAL_SGN(%a0)
	bsr.l		round
	bfclr		LOCAL_SGN(%a0){&0:&8}
	beq.b		cu_nmrsc
	bset		&sign_bit,LOCAL_EX(%a0)
cu_nmrsc:
	mov.w		LOCAL_EX(%a0),%d1
	and.w		&0x7FFF,%d1
	cmp.w		%d1,&0x407f
	blt		cu_wrexn
#
# The operand is above precision boundaries.  Use t_ovfl to
# generate the correct value.
#
cu_novfl:
	bsr		t_ovfl
	bra		cu_wrexn
#
# The operand is below precision boundaries.  Use denorm to
# generate the correct value.
#
cu_nunfl:
	bclr		&sign_bit,LOCAL_EX(%a0)
	sne		LOCAL_SGN(%a0)
	bsr		denorm
	bfclr		LOCAL_SGN(%a0){&0:&8}	# change back to IEEE ext format
	beq.b		cu_nucont
	bset		&sign_bit,LOCAL_EX(%a0)
cu_nucont:
	bfextu		FPCR_MODE(%a6){&2:&2},%d1
	btst		&2,CMDREG1B+1(%a6)	# check for rd
	bne		inst_d
	btst		&6,CMDREG1B+1(%a6)	# check for rs
	bne		inst_s
	swap		%d1
	mov.b		FPCR_MODE(%a6),%d1
	lsr.b		&6,%d1
	swap		%d1
	bra		inst_sd
inst_d:
	or.l		&0x00020000,%d1
	bra		inst_sd
inst_s:
	or.l		&0x00010000,%d1
inst_sd:
	bclr		&sign_bit,LOCAL_EX(%a0)
	sne		LOCAL_SGN(%a0)
	bsr.l		round
	bfclr		LOCAL_SGN(%a0){&0:&8}
	beq.b		cu_nuflp
	bset		&sign_bit,LOCAL_EX(%a0)
cu_nuflp:
	btst		&inex2_bit,FPSR_EXCEPT(%a6)
	beq.b		cu_nuninx
	or.l		&aunfl_mask,USER_FPSR(%a6)	# if the round was inex, set AUNFL
cu_nuninx:
	tst.l		LOCAL_HI(%a0)	# test for zero
	bne.b		cu_nunzro
	tst.l		LOCAL_LO(%a0)
	bne.b		cu_nunzro
#
# The mantissa is zero from the denorm loop.  Check sign and rmode
# to see if rounding should have occured which would leave the lsb.
#
	mov.l		USER_FPCR(%a6),%d0
	and.l		&0x30,%d0	# isolate rmode
	cmp.l		%d0,&0x20
	blt.b		cu_nzro
	bne.b		cu_nrp
cu_nrm:
	tst.w		LOCAL_EX(%a0)	# if positive, set lsb
	bge.b		cu_nzro
	btst		&7,FPCR_MODE(%a6)	# check for double
	beq.b		cu_nincs
	bra.b		cu_nincd
cu_nrp:
	tst.w		LOCAL_EX(%a0)	# if positive, set lsb
	blt.b		cu_nzro
	btst		&7,FPCR_MODE(%a6)	# check for double
	beq.b		cu_nincs
cu_nincd:
	or.l		&0x800,LOCAL_LO(%a0)	# inc for double
	bra		cu_nunzro
cu_nincs:
	or.l		&0x100,LOCAL_HI(%a0)	# inc for single
	bra		cu_nunzro
cu_nzro:
	or.l		&z_mask,USER_FPSR(%a6)
	mov.b		STAG(%a6),%d0
	and.b		&0xe0,%d0
	cmp.b		%d0,&0x40	# check if input was tagged zero
	beq.b		cu_numv
cu_nunzro:
	or.l		&unfl_mask,USER_FPSR(%a6)	# set unfl
cu_numv:
	mov.l		(%a0),ETEMP(%a6)
	mov.l		4(%a0),ETEMP_HI(%a6)
	mov.l		8(%a0),ETEMP_LO(%a6)
#
# Write the result to memory, setting the fpsr cc bits.  NaN and Inf
# bypass cu_wrexn.
#
cu_wrexn:
	tst.w		LOCAL_EX(%a0)	# test for zero
	beq.b		cu_wrzero
	cmp.w		LOCAL_EX(%a0),&0x8000	# test for zero
	bne.b		cu_wreon
cu_wrzero:
	or.l		&z_mask,USER_FPSR(%a6)	# set Z bit
cu_wreon:
	tst.w		LOCAL_EX(%a0)
	bpl		wr_etemp
	or.l		&neg_mask,USER_FPSR(%a6)
	bra		wr_etemp

#
# HANDLE SOURCE DENORM HERE
#
#				;clear denorm stag to norm
#				;write the new tag & ete15 to the fstack
mon_dnrm:
#
# At this point, check for the cases in which normalizing the 
# denorm produces incorrect results.
#
	tst.b		DY_MO_FLG(%a6)	# all cases of dyadic instructions would
	bne.b		nrm_src		# require normalization of denorm

# At this point:
#	monadic instructions:	fabs  = $18  fneg   = $1a  ftst   = $3a
#				fmove = $00  fsmove = $40  fdmove = $44
#				fsqrt = $05* fssqrt = $41  fdsqrt = $45
#				(*fsqrt reencoded to $05)
#
	mov.w		CMDREG1B(%a6),%d0	# get command register
	and.l		&0x7f,%d0	# strip to only command word
#
# At this point, fabs, fneg, fsmove, fdmove, ftst, fsqrt, fssqrt, and 
# fdsqrt are possible.
# For cases fabs, fneg, fsmove, and fdmove goto spos (do not normalize)
# For cases fsqrt, fssqrt, and fdsqrt goto nrm_src (do normalize)
#
	btst		&0,%d0
	bne.b		nrm_src		# weed out fsqrt instructions
	st		CU_ONLY(%a6)	# set cu-only inst flag
	bra		cu_dnrm		# fmove, fabs, fneg, ftst 
#				;cases go to cu_dnrm
nrm_src:
	bclr		&sign_bit,LOCAL_EX(%a0)
	sne		LOCAL_SGN(%a0)
	bsr		nrm_set		# normalize number (exponent will go 
#				; negative)
	bclr		&sign_bit,LOCAL_EX(%a0)	# get rid of false sign

	bfclr		LOCAL_SGN(%a0){&0:&8}	# change back to IEEE ext format
	beq.b		spos
	bset		&sign_bit,LOCAL_EX(%a0)
spos:
	bfclr		STAG(%a6){&0:&4}	# set tag to normalized, FPTE15 = 0
	bset		&4,STAG(%a6)	# set ETE15
	or.b		&0xf0,DNRM_FLG(%a6)
normal:
	tst.b		DNRM_FLG(%a6)	# check if any of the ops were denorms
	bne		ck_wrap		# if so, check if it is a potential
#				;wrap-around case
fix_stk:
	mov.b		&0xfe,CU_SAVEPC(%a6)
	bclr		&E1,E_BYTE(%a6)

	clr.w		NMNEXC(%a6)

	st.b		RES_FLG(%a6)	# indicate that a restore is needed
	rts

#
# cu_dnrm handles all cu-only instructions (fmove, fabs, fneg, and
# ftst) completly in software without an frestore to the 040. 
#
cu_dnrm:
	st.b		CU_ONLY(%a6)
	mov.w		CMDREG1B(%a6),%d0
	and.b		&0x3b,%d0	# isolate bits to select inst
	tst.b		%d0
	beq.l		cu_dmove	# if zero, it is an fmove
	cmp.b		%d0,&0x18
	beq.l		cu_dabs		# if $18, it is fabs
	cmp.b		%d0,&0x1a
	beq.l		cu_dneg		# if $1a, it is fneg
#
# Inst is ftst.  Check the source operand and set the cc's accordingly.
# No write is done, so simply rts.
#
cu_dtst:
	mov.w		LOCAL_EX(%a0),%d0
	bclr		&15,%d0
	sne		LOCAL_SGN(%a0)
	beq.b		cu_dtpo
	or.l		&neg_mask,USER_FPSR(%a6)	# set N
cu_dtpo:
	cmp.w		%d0,&0x7fff	# test for inf/nan
	bne.b		cu_dtcz
	tst.l		LOCAL_HI(%a0)
	bne.b		cu_dtn
	tst.l		LOCAL_LO(%a0)
	bne.b		cu_dtn
	or.l		&inf_mask,USER_FPSR(%a6)
	rts
cu_dtn:
	or.l		&nan_mask,USER_FPSR(%a6)
	mov.l		ETEMP_EX(%a6),FPTEMP_EX(%a6)	# set up fptemp sign for 
#						;snan handler
	rts
cu_dtcz:
	tst.l		LOCAL_HI(%a0)
	bne.l		cu_dtsx
	tst.l		LOCAL_LO(%a0)
	bne.l		cu_dtsx
	or.l		&z_mask,USER_FPSR(%a6)
cu_dtsx:
	rts
#
# Inst is fabs.  Execute the absolute value function on the input.
# Branch to the fmove code.
#
cu_dabs:
	bclr		&7,LOCAL_EX(%a0)	# do abs
	bra.b		cu_dmove	# fmove code will finish
#
# Inst is fneg.  Execute the negate value function on the input.
# Fall though to the fmove code.
#
cu_dneg:
	bchg		&7,LOCAL_EX(%a0)	# do neg
#
# Inst is fmove.  This code also handles all result writes.
# If bit 2 is set, round is forced to double.  If it is clear,
# and bit 6 is set, round is forced to single.  If both are clear,
# the round precision is found in the fpcr.  If the rounding precision
# is double or single, the result is zero, and the mode is checked
# to determine if the lsb of the result should be set.
#
cu_dmove:
	btst		&2,CMDREG1B+1(%a6)	# check for rd
	bne		cu_dmrd
	btst		&6,CMDREG1B+1(%a6)	# check for rs
	bne		cu_dmrs
#
# The move or operation is not with forced precision.  Use the
# FPCR_MODE byte to get rounding.
#
cu_dmnr:
	bfextu		FPCR_MODE(%a6){&0:&2},%d0
	tst.b		%d0		# check for extended
	beq		cu_wrexd	# if so, just write result
	cmp.b		%d0,&1		# check for single
	beq		cu_dmrs		# fall through to double
#
# The move is fdmove or round precision is double.  Result is zero.
# Check rmode for rp or rm and set lsb accordingly.
#
cu_dmrd:
	bfextu		FPCR_MODE(%a6){&2:&2},%d1	# get rmode
	tst.w		LOCAL_EX(%a0)	# check sign
	blt.b		cu_dmdn
	cmp.b		%d1,&3		# check for rp
	bne		cu_dpd		# load double pos zero
	bra		cu_dpdr		# load double pos zero w/lsb
cu_dmdn:
	cmp.b		%d1,&2		# check for rm
	bne		cu_dnd		# load double neg zero
	bra		cu_dndr		# load double neg zero w/lsb
#
# The move is fsmove or round precision is single.  Result is zero.
# Check for rp or rm and set lsb accordingly.
#
cu_dmrs:
	bfextu		FPCR_MODE(%a6){&2:&2},%d1	# get rmode
	tst.w		LOCAL_EX(%a0)	# check sign
	blt.b		cu_dmsn
	cmp.b		%d1,&3		# check for rp
	bne		cu_spd		# load single pos zero
	bra		cu_spdr		# load single pos zero w/lsb
cu_dmsn:
	cmp.b		%d1,&2		# check for rm
	bne		cu_snd		# load single neg zero
	bra		cu_sndr		# load single neg zero w/lsb
#
# The precision is extended, so the result in etemp is correct.
# Simply set unfl (not inex2 or aunfl) and write the result to 
# the correct fp register.
cu_wrexd:
	or.l		&unfl_mask,USER_FPSR(%a6)
	tst.w		LOCAL_EX(%a0)
	beq		wr_etemp
	or.l		&neg_mask,USER_FPSR(%a6)
	bra		wr_etemp
#
# These routines write +/- zero in double format.  The routines
# cu_dpdr and cu_dndr set the double lsb.
#
cu_dpd:
	mov.l		&0x3c010000,LOCAL_EX(%a0)	# force pos double zero
	clr.l		LOCAL_HI(%a0)
	clr.l		LOCAL_LO(%a0)
	or.l		&z_mask,USER_FPSR(%a6)
	or.l		&unfinx_mask,USER_FPSR(%a6)
	bra		wr_etemp
cu_dpdr:
	mov.l		&0x3c010000,LOCAL_EX(%a0)	# force pos double zero
	clr.l		LOCAL_HI(%a0)
	mov.l		&0x800,LOCAL_LO(%a0)	# with lsb set
	or.l		&unfinx_mask,USER_FPSR(%a6)
	bra		wr_etemp
cu_dnd:
	mov.l		&0xbc010000,LOCAL_EX(%a0)	# force pos double zero
	clr.l		LOCAL_HI(%a0)
	clr.l		LOCAL_LO(%a0)
	or.l		&z_mask,USER_FPSR(%a6)
	or.l		&neg_mask,USER_FPSR(%a6)
	or.l		&unfinx_mask,USER_FPSR(%a6)
	bra		wr_etemp
cu_dndr:
	mov.l		&0xbc010000,LOCAL_EX(%a0)	# force pos double zero
	clr.l		LOCAL_HI(%a0)
	mov.l		&0x800,LOCAL_LO(%a0)	# with lsb set
	or.l		&neg_mask,USER_FPSR(%a6)
	or.l		&unfinx_mask,USER_FPSR(%a6)
	bra		wr_etemp
#
# These routines write +/- zero in single format.  The routines
# cu_dpdr and cu_dndr set the single lsb.
#
cu_spd:
	mov.l		&0x3f810000,LOCAL_EX(%a0)	# force pos single zero
	clr.l		LOCAL_HI(%a0)
	clr.l		LOCAL_LO(%a0)
	or.l		&z_mask,USER_FPSR(%a6)
	or.l		&unfinx_mask,USER_FPSR(%a6)
	bra		wr_etemp
cu_spdr:
	mov.l		&0x3f810000,LOCAL_EX(%a0)	# force pos single zero
	mov.l		&0x100,LOCAL_HI(%a0)	# with lsb set
	clr.l		LOCAL_LO(%a0)
	or.l		&unfinx_mask,USER_FPSR(%a6)
	bra		wr_etemp
cu_snd:
	mov.l		&0xbf810000,LOCAL_EX(%a0)	# force pos single zero
	clr.l		LOCAL_HI(%a0)
	clr.l		LOCAL_LO(%a0)
	or.l		&z_mask,USER_FPSR(%a6)
	or.l		&neg_mask,USER_FPSR(%a6)
	or.l		&unfinx_mask,USER_FPSR(%a6)
	bra		wr_etemp
cu_sndr:
	mov.l		&0xbf810000,LOCAL_EX(%a0)	# force pos single zero
	mov.l		&0x100,LOCAL_HI(%a0)	# with lsb set
	clr.l		LOCAL_LO(%a0)
	or.l		&neg_mask,USER_FPSR(%a6)
	or.l		&unfinx_mask,USER_FPSR(%a6)
	bra		wr_etemp
#
# This code checks for 16-bit overflow conditions on dyadic
# operations which are not restorable into the floating-point
# unit and must be completed in software.  Basically, this
# condition exists with a very large norm and a denorm.  One
# of the operands must be denormalized to enter this code.
#
# Flags used:
#	DY_MO_FLG contains 0 for monadic op, $ff for dyadic
#	DNRM_FLG contains $00 for neither op denormalized
#	                  $0f for the destination op denormalized
#	                  $f0 for the source op denormalized
#	                  $ff for both ops denormalzed
#
# The wrap-around condition occurs for add, sub, div, and cmp
# when 
#
#	abs(dest_exp - src_exp) >= $8000
#
# and for mul when
#
#	(dest_exp + src_exp) < $0
#
# we must process the operation here if this case is true.
#
# The rts following the frcfpn routine is the exit from res_func
# for this condition.  The restore flag (RES_FLG) is left clear.
# No frestore is done unless an exception is to be reported.
#
# For fadd: 
#	if(sign_of(dest) != sign_of(src))
#		replace exponent of src with $3fff (keep sign)
#		use fpu to perform dest+new_src (user's rmode and X)
#		clr sticky
#	else
#		set sticky
#	call round with user's precision and mode
#	move result to fpn and wbtemp
#
# For fsub:
#	if(sign_of(dest) == sign_of(src))
#		replace exponent of src with $3fff (keep sign)
#		use fpu to perform dest+new_src (user's rmode and X)
#		clr sticky
#	else
#		set sticky
#	call round with user's precision and mode
#	move result to fpn and wbtemp
#
# For fdiv/fsgldiv:
#	if(both operands are denorm)
#		restore_to_fpu;
#	if(dest is norm)
#		force_ovf;
#	else(dest is denorm)
#		force_unf:
#
# For fcmp:
#	if(dest is norm)
#		N = sign_of(dest);
#	else(dest is denorm)
#		N = sign_of(src);
#
# For fmul:
#	if(both operands are denorm)
#		force_unf;
#	if((dest_exp + src_exp) < 0)
#		force_unf:
#	else
#		restore_to_fpu;
#
# local equates:
	set		addcode,0x22
	set		subcode,0x28
	set		mulcode,0x23
	set		divcode,0x20
	set		cmpcode,0x38
ck_wrap:
	tst.b		DY_MO_FLG(%a6)	# check for fsqrt
	beq		fix_stk		# if zero, it is fsqrt
	mov.w		CMDREG1B(%a6),%d0
	and.w		&0x3b,%d0	# strip to command bits
	cmp.w		%d0,&addcode
	beq		wrap_add
	cmp.w		%d0,&subcode
	beq		wrap_sub
	cmp.w		%d0,&mulcode
	beq		wrap_mul
	cmp.w		%d0,&cmpcode
	beq		wrap_cmp
#
# Inst is fdiv.  
#
wrap_div:
	cmp.b		DNRM_FLG(%a6),&0xff	# if both ops denorm, 
	beq		fix_stk		# restore to fpu
#
# One of the ops is denormalized.  Test for wrap condition
# and force the result.
#
	cmp.b		DNRM_FLG(%a6),&0x0f	# check for dest denorm
	bne.b		div_srcd
div_destd:
	bsr.l		ckinf_ns
	bne		fix_stk
	bfextu		ETEMP_EX(%a6){&1:&15},%d0	# get src exp (always pos)
	bfexts		FPTEMP_EX(%a6){&1:&15},%d1	# get dest exp (always neg)
	sub.l		%d1,%d0		# subtract dest from src
	cmp.l		%d0,&0x7fff
	blt		fix_stk		# if less, not wrap case
	clr.b		WBTEMP_SGN(%a6)
	mov.w		ETEMP_EX(%a6),%d0	# find the sign of the result
	mov.w		FPTEMP_EX(%a6),%d1
	eor.w		%d1,%d0
	and.w		&0x8000,%d0
	beq		force_unf
	st.b		WBTEMP_SGN(%a6)
	bra		force_unf

ckinf_ns:
	mov.b		STAG(%a6),%d0	# check source tag for inf or nan
	bra		ck_in_com
ckinf_nd:
	mov.b		DTAG(%a6),%d0	# check destination tag for inf or nan
ck_in_com:
	and.b		&0x60,%d0	# isolate tag bits
	cmp.b		%d0,&0x40	# is it inf?
	beq		nan_or_inf	# not wrap case
	cmp.b		%d0,&0x60	# is it nan?
	beq		nan_or_inf	# yes, not wrap case?
	cmp.b		%d0,&0x20	# is it a zero?
	beq		nan_or_inf	# yes
	clr.l		%d0
	rts				# then it is either a zero of norm,
#					;check wrap case
nan_or_inf:
	mov.l		&-1,%d0
	rts



div_srcd:
	bsr.l		ckinf_nd
	bne		fix_stk
	bfextu		FPTEMP_EX(%a6){&1:&15},%d0	# get dest exp (always pos)
	bfexts		ETEMP_EX(%a6){&1:&15},%d1	# get src exp (always neg)
	sub.l		%d1,%d0		# subtract src from dest
	cmp.l		%d0,&0x8000
	blt		fix_stk		# if less, not wrap case
	clr.b		WBTEMP_SGN(%a6)
	mov.w		ETEMP_EX(%a6),%d0	# find the sign of the result
	mov.w		FPTEMP_EX(%a6),%d1
	eor.w		%d1,%d0
	and.w		&0x8000,%d0
	beq.b		force_ovf
	st.b		WBTEMP_SGN(%a6)
#
# This code handles the case of the instruction resulting in 
# an overflow condition.
#
force_ovf:
	bclr		&E1,E_BYTE(%a6)
	or.l		&ovfl_inx_mask,USER_FPSR(%a6)
	clr.w		NMNEXC(%a6)
	lea.l		WBTEMP(%a6),%a0	# point a0 to memory location
	mov.w		CMDREG1B(%a6),%d0
	btst		&6,%d0		# test for forced precision
	beq.b		frcovf_fpcr
	btst		&2,%d0		# check for double
	bne.b		frcovf_dbl
	mov.l		&0x1,%d0	# inst is forced single
	bra.b		frcovf_rnd
frcovf_dbl:
	mov.l		&0x2,%d0	# inst is forced double
	bra.b		frcovf_rnd
frcovf_fpcr:
	bfextu		FPCR_MODE(%a6){&0:&2},%d0	# inst not forced - use fpcr prec
frcovf_rnd:

# The 881/882 does not set inex2 for the following case, so the 
# line is commented out to be compatible with 881/882
#	tst.b	d0
#	beq.b	frcovf_x
#	or.l	#inex2_mask,USER_FPSR(a6) ;if prec is s or d, set inex2

#frcovf_x:
	bsr.l		ovf_res		# get correct result based on
#					;round precision/mode.  This 
#					;sets FPSR_CC correctly
#					;returns in external format
	bfclr		WBTEMP_SGN(%a6){&0:&8}
	beq		frcfpn
	bset		&sign_bit,WBTEMP_EX(%a6)
	bra		frcfpn
#
# Inst is fadd.
#
wrap_add:
	cmp.b		DNRM_FLG(%a6),&0xff	# if both ops denorm, 
	beq		fix_stk		# restore to fpu
#
# One of the ops is denormalized.  Test for wrap condition
# and complete the instruction.
#
	cmp.b		DNRM_FLG(%a6),&0x0f	# check for dest denorm
	bne.b		add_srcd
add_destd:
	bsr.l		ckinf_ns
	bne		fix_stk
	bfextu		ETEMP_EX(%a6){&1:&15},%d0	# get src exp (always pos)
	bfexts		FPTEMP_EX(%a6){&1:&15},%d1	# get dest exp (always neg)
	sub.l		%d1,%d0		# subtract dest from src
	cmp.l		%d0,&0x8000
	blt		fix_stk		# if less, not wrap case
	bra		add_wrap
add_srcd:
	bsr.l		ckinf_nd
	bne		fix_stk
	bfextu		FPTEMP_EX(%a6){&1:&15},%d0	# get dest exp (always pos)
	bfexts		ETEMP_EX(%a6){&1:&15},%d1	# get src exp (always neg)
	sub.l		%d1,%d0		# subtract src from dest
	cmp.l		%d0,&0x8000
	blt		fix_stk		# if less, not wrap case
#
# Check the signs of the operands.  If they are unlike, the fpu
# can be used to add the norm and 1.0 with the sign of the
# denorm and it will correctly generate the result in extended
# precision.  We can then call round with no sticky and the result
# will be correct for the user's rounding mode and precision.  If
# the signs are the same, we call round with the sticky bit set
# and the result will be correctfor the user's rounding mode and
# precision.
#
add_wrap:
	mov.w		ETEMP_EX(%a6),%d0
	mov.w		FPTEMP_EX(%a6),%d1
	eor.w		%d1,%d0
	and.w		&0x8000,%d0
	beq		add_same
#
# The signs are unlike.
#
	cmp.b		DNRM_FLG(%a6),&0x0f	# is dest the denorm?
	bne.b		add_u_srcd
	mov.w		FPTEMP_EX(%a6),%d0
	and.w		&0x8000,%d0
	or.w		&0x3fff,%d0	# force the exponent to +/- 1
	mov.w		%d0,FPTEMP_EX(%a6)	# in the denorm
	mov.l		USER_FPCR(%a6),%d0
	and.l		&0x30,%d0
	fmov.l		%d0,%control	# set up users rmode and X
	fmov.x		ETEMP(%a6),%fp0
	fadd.x		FPTEMP(%a6),%fp0
	lea.l		WBTEMP(%a6),%a0	# point a0 to wbtemp in frame
	fmov.l		%status,%d1
	or.l		%d1,USER_FPSR(%a6)	# capture cc's and inex from fadd
	fmov.x		%fp0,WBTEMP(%a6)	# write result to memory
	lsr.l		&4,%d0		# put rmode in lower 2 bits
	mov.l		USER_FPCR(%a6),%d1
	and.l		&0xc0,%d1
	lsr.l		&6,%d1		# put precision in upper word
	swap		%d1
	or.l		%d0,%d1		# set up for round call
	clr.l		%d0		# force sticky to zero
	bclr		&sign_bit,WBTEMP_EX(%a6)
	sne		WBTEMP_SGN(%a6)
	bsr.l		round		# round result to users rmode & prec
	bfclr		WBTEMP_SGN(%a6){&0:&8}	# convert back to IEEE ext format
	beq		frcfpnr
	bset		&sign_bit,WBTEMP_EX(%a6)
	bra		frcfpnr
add_u_srcd:
	mov.w		ETEMP_EX(%a6),%d0
	and.w		&0x8000,%d0
	or.w		&0x3fff,%d0	# force the exponent to +/- 1
	mov.w		%d0,ETEMP_EX(%a6)	# in the denorm
	mov.l		USER_FPCR(%a6),%d0
	and.l		&0x30,%d0
	fmov.l		%d0,%control	# set up users rmode and X
	fmov.x		ETEMP(%a6),%fp0
	fadd.x		FPTEMP(%a6),%fp0
	fmov.l		%status,%d1
	or.l		%d1,USER_FPSR(%a6)	# capture cc's and inex from fadd
	lea.l		WBTEMP(%a6),%a0	# point a0 to wbtemp in frame
	fmov.x		%fp0,WBTEMP(%a6)	# write result to memory
	lsr.l		&4,%d0		# put rmode in lower 2 bits
	mov.l		USER_FPCR(%a6),%d1
	and.l		&0xc0,%d1
	lsr.l		&6,%d1		# put precision in upper word
	swap		%d1
	or.l		%d0,%d1		# set up for round call
	clr.l		%d0		# force sticky to zero
	bclr		&sign_bit,WBTEMP_EX(%a6)
	sne		WBTEMP_SGN(%a6)	# use internal format for round
	bsr.l		round		# round result to users rmode & prec
	bfclr		WBTEMP_SGN(%a6){&0:&8}	# convert back to IEEE ext format
	beq		frcfpnr
	bset		&sign_bit,WBTEMP_EX(%a6)
	bra		frcfpnr
#
# Signs are alike:
#
add_same:
	cmp.b		DNRM_FLG(%a6),&0x0f	# is dest the denorm?
	bne.b		add_s_srcd
add_s_destd:
	lea.l		ETEMP(%a6),%a0
	mov.l		USER_FPCR(%a6),%d0
	and.l		&0x30,%d0
	lsr.l		&4,%d0		# put rmode in lower 2 bits
	mov.l		USER_FPCR(%a6),%d1
	and.l		&0xc0,%d1
	lsr.l		&6,%d1		# put precision in upper word
	swap		%d1
	or.l		%d0,%d1		# set up for round call
	mov.l		&0x20000000,%d0	# set sticky for round
	bclr		&sign_bit,ETEMP_EX(%a6)
	sne		ETEMP_SGN(%a6)
	bsr.l		round		# round result to users rmode & prec
	bfclr		ETEMP_SGN(%a6){&0:&8}	# convert back to IEEE ext format
	beq.b		add_s_dclr
	bset		&sign_bit,ETEMP_EX(%a6)
add_s_dclr:
	lea.l		WBTEMP(%a6),%a0
	mov.l		ETEMP(%a6),(%a0)	# write result to wbtemp
	mov.l		ETEMP_HI(%a6),4(%a0)
	mov.l		ETEMP_LO(%a6),8(%a0)
	tst.w		ETEMP_EX(%a6)
	bgt		add_ckovf
	or.l		&neg_mask,USER_FPSR(%a6)
	bra		add_ckovf
add_s_srcd:
	lea.l		FPTEMP(%a6),%a0
	mov.l		USER_FPCR(%a6),%d0
	and.l		&0x30,%d0
	lsr.l		&4,%d0		# put rmode in lower 2 bits
	mov.l		USER_FPCR(%a6),%d1
	and.l		&0xc0,%d1
	lsr.l		&6,%d1		# put precision in upper word
	swap		%d1
	or.l		%d0,%d1		# set up for round call
	mov.l		&0x20000000,%d0	# set sticky for round
	bclr		&sign_bit,FPTEMP_EX(%a6)
	sne		FPTEMP_SGN(%a6)
	bsr.l		round		# round result to users rmode & prec
	bfclr		FPTEMP_SGN(%a6){&0:&8}	# convert back to IEEE ext format
	beq.b		add_s_sclr
	bset		&sign_bit,FPTEMP_EX(%a6)
add_s_sclr:
	lea.l		WBTEMP(%a6),%a0
	mov.l		FPTEMP(%a6),(%a0)	# write result to wbtemp
	mov.l		FPTEMP_HI(%a6),4(%a0)
	mov.l		FPTEMP_LO(%a6),8(%a0)
	tst.w		FPTEMP_EX(%a6)
	bgt		add_ckovf
	or.l		&neg_mask,USER_FPSR(%a6)
add_ckovf:
	mov.w		WBTEMP_EX(%a6),%d0
	and.w		&0x7fff,%d0
	cmp.w		%d0,&0x7fff
	bne		frcfpnr
#
# The result has overflowed to $7fff exponent.  Set I, ovfl,
# and aovfl, and clr the mantissa (incorrectly set by the
# round routine.)
#
	or.l		&inf_mask+ovfl_inx_mask,USER_FPSR(%a6)
	clr.l		4(%a0)
	bra		frcfpnr
#
# Inst is fsub.
#
wrap_sub:
	cmp.b		DNRM_FLG(%a6),&0xff	# if both ops denorm, 
	beq		fix_stk		# restore to fpu
#
# One of the ops is denormalized.  Test for wrap condition
# and complete the instruction.
#
	cmp.b		DNRM_FLG(%a6),&0x0f	# check for dest denorm
	bne.b		sub_srcd
sub_destd:
	bsr.l		ckinf_ns
	bne		fix_stk
	bfextu		ETEMP_EX(%a6){&1:&15},%d0	# get src exp (always pos)
	bfexts		FPTEMP_EX(%a6){&1:&15},%d1	# get dest exp (always neg)
	sub.l		%d1,%d0		# subtract src from dest
	cmp.l		%d0,&0x8000
	blt		fix_stk		# if less, not wrap case
	bra		sub_wrap
sub_srcd:
	bsr.l		ckinf_nd
	bne		fix_stk
	bfextu		FPTEMP_EX(%a6){&1:&15},%d0	# get dest exp (always pos)
	bfexts		ETEMP_EX(%a6){&1:&15},%d1	# get src exp (always neg)
	sub.l		%d1,%d0		# subtract dest from src
	cmp.l		%d0,&0x8000
	blt		fix_stk		# if less, not wrap case
#
# Check the signs of the operands.  If they are alike, the fpu
# can be used to subtract from the norm 1.0 with the sign of the
# denorm and it will correctly generate the result in extended
# precision.  We can then call round with no sticky and the result
# will be correct for the user's rounding mode and precision.  If
# the signs are unlike, we call round with the sticky bit set
# and the result will be correctfor the user's rounding mode and
# precision.
#
sub_wrap:
	mov.w		ETEMP_EX(%a6),%d0
	mov.w		FPTEMP_EX(%a6),%d1
	eor.w		%d1,%d0
	and.w		&0x8000,%d0
	bne		sub_diff
#
# The signs are alike.
#
	cmp.b		DNRM_FLG(%a6),&0x0f	# is dest the denorm?
	bne.b		sub_u_srcd
	mov.w		FPTEMP_EX(%a6),%d0
	and.w		&0x8000,%d0
	or.w		&0x3fff,%d0	# force the exponent to +/- 1
	mov.w		%d0,FPTEMP_EX(%a6)	# in the denorm
	mov.l		USER_FPCR(%a6),%d0
	and.l		&0x30,%d0
	fmov.l		%d0,%control	# set up users rmode and X
	fmov.x		FPTEMP(%a6),%fp0
	fsub.x		ETEMP(%a6),%fp0
	fmov.l		%status,%d1
	or.l		%d1,USER_FPSR(%a6)	# capture cc's and inex from fadd
	lea.l		WBTEMP(%a6),%a0	# point a0 to wbtemp in frame
	fmov.x		%fp0,WBTEMP(%a6)	# write result to memory
	lsr.l		&4,%d0		# put rmode in lower 2 bits
	mov.l		USER_FPCR(%a6),%d1
	and.l		&0xc0,%d1
	lsr.l		&6,%d1		# put precision in upper word
	swap		%d1
	or.l		%d0,%d1		# set up for round call
	clr.l		%d0		# force sticky to zero
	bclr		&sign_bit,WBTEMP_EX(%a6)
	sne		WBTEMP_SGN(%a6)
	bsr.l		round		# round result to users rmode & prec
	bfclr		WBTEMP_SGN(%a6){&0:&8}	# convert back to IEEE ext format
	beq		frcfpnr
	bset		&sign_bit,WBTEMP_EX(%a6)
	bra		frcfpnr
sub_u_srcd:
	mov.w		ETEMP_EX(%a6),%d0
	and.w		&0x8000,%d0
	or.w		&0x3fff,%d0	# force the exponent to +/- 1
	mov.w		%d0,ETEMP_EX(%a6)	# in the denorm
	mov.l		USER_FPCR(%a6),%d0
	and.l		&0x30,%d0
	fmov.l		%d0,%control	# set up users rmode and X
	fmov.x		FPTEMP(%a6),%fp0
	fsub.x		ETEMP(%a6),%fp0
	fmov.l		%status,%d1
	or.l		%d1,USER_FPSR(%a6)	# capture cc's and inex from fadd
	lea.l		WBTEMP(%a6),%a0	# point a0 to wbtemp in frame
	fmov.x		%fp0,WBTEMP(%a6)	# write result to memory
	lsr.l		&4,%d0		# put rmode in lower 2 bits
	mov.l		USER_FPCR(%a6),%d1
	and.l		&0xc0,%d1
	lsr.l		&6,%d1		# put precision in upper word
	swap		%d1
	or.l		%d0,%d1		# set up for round call
	clr.l		%d0		# force sticky to zero
	bclr		&sign_bit,WBTEMP_EX(%a6)
	sne		WBTEMP_SGN(%a6)
	bsr.l		round		# round result to users rmode & prec
	bfclr		WBTEMP_SGN(%a6){&0:&8}	# convert back to IEEE ext format
	beq		frcfpnr
	bset		&sign_bit,WBTEMP_EX(%a6)
	bra		frcfpnr
#
# Signs are unlike:
#
sub_diff:
	cmp.b		DNRM_FLG(%a6),&0x0f	# is dest the denorm?
	bne.b		sub_s_srcd
sub_s_destd:
	lea.l		ETEMP(%a6),%a0
	mov.l		USER_FPCR(%a6),%d0
	and.l		&0x30,%d0
	lsr.l		&4,%d0		# put rmode in lower 2 bits
	mov.l		USER_FPCR(%a6),%d1
	and.l		&0xc0,%d1
	lsr.l		&6,%d1		# put precision in upper word
	swap		%d1
	or.l		%d0,%d1		# set up for round call
	mov.l		&0x20000000,%d0	# set sticky for round
#
# Since the dest is the denorm, the sign is the opposite of the
# norm sign.
#
	eor.w		&0x8000,ETEMP_EX(%a6)	# flip sign on result
	tst.w		ETEMP_EX(%a6)
	bgt.b		sub_s_dwr
	or.l		&neg_mask,USER_FPSR(%a6)
sub_s_dwr:
	bclr		&sign_bit,ETEMP_EX(%a6)
	sne		ETEMP_SGN(%a6)
	bsr.l		round		# round result to users rmode & prec
	bfclr		ETEMP_SGN(%a6){&0:&8}	# convert back to IEEE ext format
	beq.b		sub_s_dclr
	bset		&sign_bit,ETEMP_EX(%a6)
sub_s_dclr:
	lea.l		WBTEMP(%a6),%a0
	mov.l		ETEMP(%a6),(%a0)	# write result to wbtemp
	mov.l		ETEMP_HI(%a6),4(%a0)
	mov.l		ETEMP_LO(%a6),8(%a0)
	bra		sub_ckovf
sub_s_srcd:
	lea.l		FPTEMP(%a6),%a0
	mov.l		USER_FPCR(%a6),%d0
	and.l		&0x30,%d0
	lsr.l		&4,%d0		# put rmode in lower 2 bits
	mov.l		USER_FPCR(%a6),%d1
	and.l		&0xc0,%d1
	lsr.l		&6,%d1		# put precision in upper word
	swap		%d1
	or.l		%d0,%d1		# set up for round call
	mov.l		&0x20000000,%d0	# set sticky for round
	bclr		&sign_bit,FPTEMP_EX(%a6)
	sne		FPTEMP_SGN(%a6)
	bsr.l		round		# round result to users rmode & prec
	bfclr		FPTEMP_SGN(%a6){&0:&8}	# convert back to IEEE ext format
	beq.b		sub_s_sclr
	bset		&sign_bit,FPTEMP_EX(%a6)
sub_s_sclr:
	lea.l		WBTEMP(%a6),%a0
	mov.l		FPTEMP(%a6),(%a0)	# write result to wbtemp
	mov.l		FPTEMP_HI(%a6),4(%a0)
	mov.l		FPTEMP_LO(%a6),8(%a0)
	tst.w		FPTEMP_EX(%a6)
	bgt		sub_ckovf
	or.l		&neg_mask,USER_FPSR(%a6)
sub_ckovf:
	mov.w		WBTEMP_EX(%a6),%d0
	and.w		&0x7fff,%d0
	cmp.w		%d0,&0x7fff
	bne		frcfpnr
#
# The result has overflowed to $7fff exponent.  Set I, ovfl,
# and aovfl, and clr the mantissa (incorrectly set by the
# round routine.)
#
	or.l		&inf_mask+ovfl_inx_mask,USER_FPSR(%a6)
	clr.l		4(%a0)
	bra		frcfpnr
#
# Inst is fcmp.
#
wrap_cmp:
	cmp.b		DNRM_FLG(%a6),&0xff	# if both ops denorm, 
	beq		fix_stk		# restore to fpu
#
# One of the ops is denormalized.  Test for wrap condition
# and complete the instruction.
#
	cmp.b		DNRM_FLG(%a6),&0x0f	# check for dest denorm
	bne.b		cmp_srcd
cmp_destd:
	bsr.l		ckinf_ns
	bne		fix_stk
	bfextu		ETEMP_EX(%a6){&1:&15},%d0	# get src exp (always pos)
	bfexts		FPTEMP_EX(%a6){&1:&15},%d1	# get dest exp (always neg)
	sub.l		%d1,%d0		# subtract dest from src
	cmp.l		%d0,&0x8000
	blt		fix_stk		# if less, not wrap case
	tst.w		ETEMP_EX(%a6)	# set N to ~sign_of(src)
	bge		cmp_setn
	rts
cmp_srcd:
	bsr.l		ckinf_nd
	bne		fix_stk
	bfextu		FPTEMP_EX(%a6){&1:&15},%d0	# get dest exp (always pos)
	bfexts		ETEMP_EX(%a6){&1:&15},%d1	# get src exp (always neg)
	sub.l		%d1,%d0		# subtract src from dest
	cmp.l		%d0,&0x8000
	blt		fix_stk		# if less, not wrap case
	tst.w		FPTEMP_EX(%a6)	# set N to sign_of(dest)
	blt		cmp_setn
	rts
cmp_setn:
	or.l		&neg_mask,USER_FPSR(%a6)
	rts

#
# Inst is fmul.
#
wrap_mul:
	cmp.b		DNRM_FLG(%a6),&0xff	# if both ops denorm, 
	beq		force_unf	# force an underflow (really!)
#
# One of the ops is denormalized.  Test for wrap condition
# and complete the instruction.
#
	cmp.b		DNRM_FLG(%a6),&0x0f	# check for dest denorm
	bne.b		mul_srcd
mul_destd:
	bsr.l		ckinf_ns
	bne		fix_stk
	bfextu		ETEMP_EX(%a6){&1:&15},%d0	# get src exp (always pos)
	bfexts		FPTEMP_EX(%a6){&1:&15},%d1	# get dest exp (always neg)
	add.l		%d1,%d0		# subtract dest from src
	bgt		fix_stk
	bra		force_unf
mul_srcd:
	bsr.l		ckinf_nd
	bne		fix_stk
	bfextu		FPTEMP_EX(%a6){&1:&15},%d0	# get dest exp (always pos)
	bfexts		ETEMP_EX(%a6){&1:&15},%d1	# get src exp (always neg)
	add.l		%d1,%d0		# subtract src from dest
	bgt		fix_stk
#
# This code handles the case of the instruction resulting in 
# an underflow condition.
#
force_unf:
	bclr		&E1,E_BYTE(%a6)
	or.l		&unfinx_mask,USER_FPSR(%a6)
	clr.w		NMNEXC(%a6)
	clr.b		WBTEMP_SGN(%a6)
	mov.w		ETEMP_EX(%a6),%d0	# find the sign of the result
	mov.w		FPTEMP_EX(%a6),%d1
	eor.w		%d1,%d0
	and.w		&0x8000,%d0
	beq.b		frcunfcont
	st.b		WBTEMP_SGN(%a6)
frcunfcont:
	lea		WBTEMP(%a6),%a0	# point a0 to memory location
	mov.w		CMDREG1B(%a6),%d0
	btst		&6,%d0		# test for forced precision
	beq.b		frcunf_fpcr
	btst		&2,%d0		# check for double
	bne.b		frcunf_dbl
	mov.l		&0x1,%d0	# inst is forced single
	bra.b		frcunf_rnd
frcunf_dbl:
	mov.l		&0x2,%d0	# inst is forced double
	bra.b		frcunf_rnd
frcunf_fpcr:
	bfextu		FPCR_MODE(%a6){&0:&2},%d0	# inst not forced - use fpcr prec
frcunf_rnd:
	bsr.l		unf_sub		# get correct result based on
#					;round precision/mode.  This 
#					;sets FPSR_CC correctly
	bfclr		WBTEMP_SGN(%a6){&0:&8}	# convert back to IEEE ext format
	beq.b		frcfpn
	bset		&sign_bit,WBTEMP_EX(%a6)
	bra		frcfpn

#
# Write the result to the user's fpn.  All results must be HUGE to be
# written; otherwise the results would have overflowed or underflowed.
# If the rounding precision is single or double, the ovf_res routine
# is needed to correctly supply the max value.
#
frcfpnr:
	mov.w		CMDREG1B(%a6),%d0
	btst		&6,%d0		# test for forced precision
	beq.b		frcfpn_fpcr
	btst		&2,%d0		# check for double
	bne.b		frcfpn_dbl
	mov.l		&0x1,%d0	# inst is forced single
	bra.b		frcfpn_rnd
frcfpn_dbl:
	mov.l		&0x2,%d0	# inst is forced double
	bra.b		frcfpn_rnd
frcfpn_fpcr:
	bfextu		FPCR_MODE(%a6){&0:&2},%d0	# inst not forced - use fpcr prec
	tst.b		%d0
	beq.b		frcfpn		# if extended, write what you got
frcfpn_rnd:
	bclr		&sign_bit,WBTEMP_EX(%a6)
	sne		WBTEMP_SGN(%a6)
	bsr.l		ovf_res		# get correct result based on
#					;round precision/mode.  This 
#					;sets FPSR_CC correctly
	bfclr		WBTEMP_SGN(%a6){&0:&8}	# convert back to IEEE ext format
	beq.b		frcfpn_clr
	bset		&sign_bit,WBTEMP_EX(%a6)
frcfpn_clr:
	or.l		&ovfinx_mask,USER_FPSR(%a6)
# 
# Perform the write.
#
frcfpn:
	bfextu		CMDREG1B(%a6){&6:&3},%d0	# extract fp destination register
	cmp.b		%d0,&3
	ble.b		frc0123		# check if dest is fp0-fp3
	mov.l		&7,%d1
	sub.l		%d0,%d1
	clr.l		%d0
	bset		%d1,%d0
	fmovm.x		WBTEMP(%a6),%d0
	rts
frc0123:
	cmp.b		%d0,&0
	beq.b		frc0_dst
	cmp.b		%d0,&1
	beq.b		frc1_dst
	cmp.b		%d0,&2
	beq.b		frc2_dst
frc3_dst:
	mov.l		WBTEMP_EX(%a6),USER_FP3(%a6)
	mov.l		WBTEMP_HI(%a6),USER_FP3+4(%a6)
	mov.l		WBTEMP_LO(%a6),USER_FP3+8(%a6)
	rts
frc2_dst:
	mov.l		WBTEMP_EX(%a6),USER_FP2(%a6)
	mov.l		WBTEMP_HI(%a6),USER_FP2+4(%a6)
	mov.l		WBTEMP_LO(%a6),USER_FP2+8(%a6)
	rts
frc1_dst:
	mov.l		WBTEMP_EX(%a6),USER_FP1(%a6)
	mov.l		WBTEMP_HI(%a6),USER_FP1+4(%a6)
	mov.l		WBTEMP_LO(%a6),USER_FP1+8(%a6)
	rts
frc0_dst:
	mov.l		WBTEMP_EX(%a6),USER_FP0(%a6)
	mov.l		WBTEMP_HI(%a6),USER_FP0+4(%a6)
	mov.l		WBTEMP_LO(%a6),USER_FP0+8(%a6)
	rts

#
# Write etemp to fpn.
# A check is made on enabled and signalled snan exceptions,
# and the destination is not overwritten if this condition exists.
# This code is designed to make fmoveins of unsupported data types
# faster.
#
wr_etemp:
	btst		&snan_bit,FPSR_EXCEPT(%a6)	# if snan is set, and
	beq.b		fmoveinc	# enabled, force restore
	btst		&snan_bit,FPCR_ENABLE(%a6)	# and don't overwrite
	beq.b		fmoveinc	# the dest
	mov.l		ETEMP_EX(%a6),FPTEMP_EX(%a6)	# set up fptemp sign for 
#						;snan handler
	tst.b		ETEMP(%a6)	# check for negative
	blt.b		snan_neg
	rts
snan_neg:
	or.l		&neg_bit,USER_FPSR(%a6)	# snan is negative; set N
	rts
fmoveinc:
	clr.w		NMNEXC(%a6)
	bclr		&E1,E_BYTE(%a6)
	mov.b		STAG(%a6),%d0	# check if stag is inf
	and.b		&0xe0,%d0
	cmp.b		%d0,&0x40
	bne.b		fminc_cnan
	or.l		&inf_mask,USER_FPSR(%a6)	# if inf, nothing yet has set I
	tst.w		LOCAL_EX(%a0)	# check sign
	bge.b		fminc_con
	or.l		&neg_mask,USER_FPSR(%a6)
	bra		fminc_con
fminc_cnan:
	cmp.b		%d0,&0x60	# check if stag is NaN
	bne.b		fminc_czero
	or.l		&nan_mask,USER_FPSR(%a6)	# if nan, nothing yet has set NaN
	mov.l		ETEMP_EX(%a6),FPTEMP_EX(%a6)	# set up fptemp sign for 
#						;snan handler
	tst.w		LOCAL_EX(%a0)	# check sign
	bge.b		fminc_con
	or.l		&neg_mask,USER_FPSR(%a6)
	bra		fminc_con
fminc_czero:
	cmp.b		%d0,&0x20	# check if zero
	bne.b		fminc_con
	or.l		&z_mask,USER_FPSR(%a6)	# if zero, set Z
	tst.w		LOCAL_EX(%a0)	# check sign
	bge.b		fminc_con
	or.l		&neg_mask,USER_FPSR(%a6)
fminc_con:
	bfextu		CMDREG1B(%a6){&6:&3},%d0	# extract fp destination register
	cmp.b		%d0,&3
	ble.b		fp0123		# check if dest is fp0-fp3
	mov.l		&7,%d1
	sub.l		%d0,%d1
	clr.l		%d0
	bset		%d1,%d0
	fmovm.x		ETEMP(%a6),%d0
	rts

fp0123:
	cmp.b		%d0,&0
	beq.b		fp0_dst
	cmp.b		%d0,&1
	beq.b		fp1_dst
	cmp.b		%d0,&2
	beq.b		fp2_dst
fp3_dst:
	mov.l		ETEMP_EX(%a6),USER_FP3(%a6)
	mov.l		ETEMP_HI(%a6),USER_FP3+4(%a6)
	mov.l		ETEMP_LO(%a6),USER_FP3+8(%a6)
	rts
fp2_dst:
	mov.l		ETEMP_EX(%a6),USER_FP2(%a6)
	mov.l		ETEMP_HI(%a6),USER_FP2+4(%a6)
	mov.l		ETEMP_LO(%a6),USER_FP2+8(%a6)
	rts
fp1_dst:
	mov.l		ETEMP_EX(%a6),USER_FP1(%a6)
	mov.l		ETEMP_HI(%a6),USER_FP1+4(%a6)
	mov.l		ETEMP_LO(%a6),USER_FP1+8(%a6)
	rts
fp0_dst:
	mov.l		ETEMP_EX(%a6),USER_FP0(%a6)
	mov.l		ETEMP_HI(%a6),USER_FP0+4(%a6)
	mov.l		ETEMP_LO(%a6),USER_FP0+8(%a6)
	rts

opclass3:
	st.b		CU_ONLY(%a6)
	mov.w		CMDREG1B(%a6),%d0	# check if packed moveout
	and.w		&0x0c00,%d0	# isolate last 2 bits of size field
	cmp.w		%d0,&0x0c00	# if size is 011 or 111, it is packed
	beq.w		pack_out	# else it is norm or denorm
	bra.w		mv_out

#
#	MOVE OUT
#

mv_tbl:
	long		li
	long		sgp
	long		xp
	long		mvout_end	# should never be taken
	long		wi
	long		dp
	long		bi
	long		mvout_end	# should never be taken
mv_out:
	bfextu		CMDREG1B(%a6){&3:&3},%d1	# put source specifier in d1
	lea.l		mv_tbl,%a0
	mov.l		(%a0,%d1*4),%a0
	jmp		(%a0)

#
# This exit is for move-out to memory.  The aunfl bit is 
# set if the result is inex and unfl is signalled.
#
mvout_end:
	btst		&inex2_bit,FPSR_EXCEPT(%a6)
	beq.b		no_aufl
	btst		&unfl_bit,FPSR_EXCEPT(%a6)
	beq.b		no_aufl
	bset		&aunfl_bit,FPSR_AEXCEPT(%a6)
no_aufl:
	clr.w		NMNEXC(%a6)
	bclr		&E1,E_BYTE(%a6)
	fmov.l		&0,%status	# clear any cc bits from res_func
#
# Return ETEMP to extended format from internal extended format so
# that gen_except will have a correctly signed value for ovfl/unfl
# handlers.
#
	bfclr		ETEMP_SGN(%a6){&0:&8}
	beq.b		mvout_con
	bset		&sign_bit,ETEMP_EX(%a6)
mvout_con:
	rts
#
# This exit is for move-out to int register.  The aunfl bit is 
# not set in any case for this move.
#
mvouti_end:
	clr.w		NMNEXC(%a6)
	bclr		&E1,E_BYTE(%a6)
	fmov.l		&0,%status	# clear any cc bits from res_func
#
# Return ETEMP to extended format from internal extended format so
# that gen_except will have a correctly signed value for ovfl/unfl
# handlers.
#
	bfclr		ETEMP_SGN(%a6){&0:&8}
	beq.b		mvouti_con
	bset		&sign_bit,ETEMP_EX(%a6)
mvouti_con:
	rts
#
# li is used to handle a long integer source specifier
#

li:
	mov.l		&4,%d0		# set byte count

	btst		&7,STAG(%a6)	# check for extended denorm
	bne.w		int_dnrm	# if so, branch

	fmovm.x		ETEMP(%a6),&0x80	#  {%fp0}
	fcmp.d		%fp0,&0x41dfffffffc00000
# 41dfffffffc00000 in dbl prec = 401d0000fffffffe00000000 in ext prec
	fbge.w		lo_plrg
	fcmp.d		%fp0,&0xc1e0000000000000
# c1e0000000000000 in dbl prec = c01e00008000000000000000 in ext prec
	fble.w		lo_nlrg
#
# at this point, the answer is between the largest pos and neg values
#
	mov.l		USER_FPCR(%a6),%d1	# use user's rounding mode
	and.l		&0x30,%d1
	fmov.l		%d1,%control
	fmov.l		%fp0,L_SCR1(%a6)	# let the 040 perform conversion
	fmov.l		%status,%d1
	or.l		%d1,USER_FPSR(%a6)	# capture inex2/ainex if set
	bra.w		int_wrt


lo_plrg:
	mov.l		&0x7fffffff,L_SCR1(%a6)	# answer is largest positive int
	fbeq.w		int_wrt		# exact answer
	fcmp.d		%fp0,&0x41dfffffffe00000
# 41dfffffffe00000 in dbl prec = 401d0000ffffffff00000000 in ext prec
	fbge.w		int_operr	# set operr
	bra.w		int_inx		# set inexact

lo_nlrg:
	mov.l		&0x80000000,L_SCR1(%a6)
	fbeq.w		int_wrt		# exact answer
	fcmp.d		%fp0,&0xc1e0000000100000
# c1e0000000100000 in dbl prec = c01e00008000000080000000 in ext prec
	fblt.w		int_operr	# set operr
	bra.w		int_inx		# set inexact

#
# wi is used to handle a word integer source specifier
#

wi:
	mov.l		&2,%d0		# set byte count

	btst		&7,STAG(%a6)	# check for extended denorm
	bne.w		int_dnrm	# branch if so

	fmovm.x		ETEMP(%a6),&0x80	#  {%fp0}
	fcmp.s		%fp0,&0x46fffe00
# 46fffe00 in sgl prec = 400d0000fffe000000000000 in ext prec
	fbge.w		wo_plrg
	fcmp.s		%fp0,&0xc7000000
# c7000000 in sgl prec = c00e00008000000000000000 in ext prec
	fble.w		wo_nlrg

#
# at this point, the answer is between the largest pos and neg values
#
	mov.l		USER_FPCR(%a6),%d1	# use user's rounding mode
	and.l		&0x30,%d1
	fmov.l		%d1,%control
	fmov.w		%fp0,L_SCR1(%a6)	# let the 040 perform conversion
	fmov.l		%status,%d1
	or.l		%d1,USER_FPSR(%a6)	# capture inex2/ainex if set
	bra.w		int_wrt

wo_plrg:
	mov.w		&0x7fff,L_SCR1(%a6)	# answer is largest positive int
	fbeq.w		int_wrt		# exact answer
	fcmp.s		%fp0,&0x46ffff00
# 46ffff00 in sgl prec = 400d0000ffff000000000000 in ext prec
	fbge.w		int_operr	# set operr
	bra.w		int_inx		# set inexact

wo_nlrg:
	mov.w		&0x8000,L_SCR1(%a6)
	fbeq.w		int_wrt		# exact answer
	fcmp.s		%fp0,&0xc7000080
# c7000080 in sgl prec = c00e00008000800000000000 in ext prec
	fblt.w		int_operr	# set operr
	bra.w		int_inx		# set inexact

#
# bi is used to handle a byte integer source specifier
#

bi:
	mov.l		&1,%d0		# set byte count

	btst		&7,STAG(%a6)	# check for extended denorm
	bne.w		int_dnrm	# branch if so

	fmovm.x		ETEMP(%a6),&0x80	#  {%fp0}
	fcmp.s		%fp0,&0x42fe0000
# 42fe0000 in sgl prec = 40050000fe00000000000000 in ext prec
	fbge.w		by_plrg
	fcmp.s		%fp0,&0xc3000000
# c3000000 in sgl prec = c00600008000000000000000 in ext prec
	fble.w		by_nlrg

#
# at this point, the answer is between the largest pos and neg values
#
	mov.l		USER_FPCR(%a6),%d1	# use user's rounding mode
	and.l		&0x30,%d1
	fmov.l		%d1,%control
	fmov.b		%fp0,L_SCR1(%a6)	# let the 040 perform conversion
	fmov.l		%status,%d1
	or.l		%d1,USER_FPSR(%a6)	# capture inex2/ainex if set
	bra.w		int_wrt

by_plrg:
	mov.b		&0x7f,L_SCR1(%a6)	# answer is largest positive int
	fbeq.w		int_wrt		# exact answer
	fcmp.s		%fp0,&0x42ff0000
# 42ff0000 in sgl prec = 40050000ff00000000000000 in ext prec
	fbge.w		int_operr	# set operr
	bra.w		int_inx		# set inexact

by_nlrg:
	mov.b		&0x80,L_SCR1(%a6)
	fbeq.w		int_wrt		# exact answer
	fcmp.s		%fp0,&0xc3008000
# c3008000 in sgl prec = c00600008080000000000000 in ext prec
	fblt.w		int_operr	# set operr
	bra.w		int_inx		# set inexact

#
# Common integer routines
#
# int_drnrm---account for possible nonzero result for round up with positive
# operand and round down for negative answer.  In the first case (result = 1)
# byte-width (store in d0) of result must be honored.  In the second case,
# -1 in L_SCR1(a6) will cover all contingencies (FMOVE.B/W/L out).

int_dnrm:
	mov.l		&0,L_SCR1(%a6)	# initialize result to 0
	bfextu		FPCR_MODE(%a6){&2:&2},%d1	# d1 is the rounding mode
	cmp.b		%d1,&2
	bmi.b		int_inx		# if RN or RZ, done
	bne.b		int_rp		# if RP, continue below
	tst.w		ETEMP(%a6)	# RM: store -1 in L_SCR1 if src is negative
	bpl.b		int_inx		# otherwise result is 0
	mov.l		&-1,L_SCR1(%a6)
	bra.b		int_inx
int_rp:
	tst.w		ETEMP(%a6)	# RP: store +1 of proper width in L_SCR1 if
#				; source is greater than 0
	bmi.b		int_inx		# otherwise, result is 0
	lea		L_SCR1(%a6),%a1	# a1 is address of L_SCR1
	add.l		%d0,%a1		# offset by destination width -1
	sub.l		&1,%a1
	bset		&0,(%a1)	# set low bit at a1 address
int_inx:
	or.l		&inx2a_mask,USER_FPSR(%a6)
	bra.b		int_wrt
int_operr:
	fmovm.x		&0x80,FPTEMP(%a6)	# FPTEMP must contain the extended {%fp0}
#				;precision source that needs to be
#				;converted to integer this is required
#				;if the operr exception is enabled.
#				;set operr/aiop (no inex2 on int ovfl)

	or.l		&opaop_mask,USER_FPSR(%a6)
#				;fall through to perform int_wrt
int_wrt:
	mov.l		EXC_EA(%a6),%a1	# load destination address
	tst.l		%a1		# check to see if it is a dest register
	beq.b		wrt_dn		# write data register 
	lea		L_SCR1(%a6),%a0	# point to supervisor source address
	bsr.l		mem_write
	bra.w		mvouti_end

wrt_dn:
	mov.l		%d0,-(%sp)	# d0 currently contains the size to write
	bsr.l		get_fline	# get_fline returns Dn in d0
	and.w		&0x7,%d0	# isolate register
	mov.l		(%sp)+,%d1	# get size
	cmp.l		%d1,&4		# most frequent case
	beq.b		sz_long
	cmp.l		%d1,&2
	bne.b		sz_con
	or.l		&8,%d0		# add 'word' size to register#
	bra.b		sz_con
sz_long:
	or.l		&0x10,%d0	# add 'long' size to register#
sz_con:
	mov.l		%d0,%d1		# reg_dest expects size:reg in d1
	bsr.l		reg_dest	# load proper data register
	bra.w		mvouti_end
xp:
	lea		ETEMP(%a6),%a0
	bclr		&sign_bit,LOCAL_EX(%a0)
	sne		LOCAL_SGN(%a0)
	btst		&7,STAG(%a6)	# check for extended denorm
	bne.w		xdnrm
	clr.l		%d0
	bra.b		do_fp		# do normal case
sgp:
	lea		ETEMP(%a6),%a0
	bclr		&sign_bit,LOCAL_EX(%a0)
	sne		LOCAL_SGN(%a0)
	btst		&7,STAG(%a6)	# check for extended denorm
	bne.w		sp_catas	# branch if so
	mov.w		LOCAL_EX(%a0),%d0
	lea		sp_bnds,%a1
	cmp.w		%d0,(%a1)
	blt.w		sp_under
	cmp.w		%d0,2(%a1)
	bgt.w		sp_over
	mov.l		&1,%d0		# set destination format to single
	bra.b		do_fp		# do normal case
dp:
	lea		ETEMP(%a6),%a0
	bclr		&sign_bit,LOCAL_EX(%a0)
	sne		LOCAL_SGN(%a0)

	btst		&7,STAG(%a6)	# check for extended denorm
	bne.w		dp_catas	# branch if so

	mov.w		LOCAL_EX(%a0),%d0
	lea		dp_bnds,%a1

	cmp.w		%d0,(%a1)
	blt.w		dp_under
	cmp.w		%d0,2(%a1)
	bgt.w		dp_over
	mov.l		&2,%d0		# set destination format to double
#				;fall through to do_fp
#
do_fp:
	bfextu		FPCR_MODE(%a6){&2:&2},%d1	# rnd mode in d1
	swap		%d0		# rnd prec in upper word
	add.l		%d0,%d1		# d1 has PREC/MODE info
	clr.l		%d0		# clear g,r,s 

	bsr.l		round		# round 

	mov.l		%a0,%a1
	mov.l		EXC_EA(%a6),%a0

	bfextu		CMDREG1B(%a6){&3:&3},%d1	# extract destination format
#					;at this point only the dest
#					;formats sgl, dbl, ext are
#					;possible
	cmp.b		%d1,&2
	bgt.b		ddbl		# double=5, extended=2, single=1
	bne.b		dsgl
#					;fall through to dext
dext:
	bsr.l		dest_ext
	bra.w		mvout_end
dsgl:
	bsr.l		dest_sgl
	bra.w		mvout_end
ddbl:
	bsr.l		dest_dbl
	bra.w		mvout_end

#
# Handle possible denorm or catastrophic underflow cases here
#
xdnrm:
	bsr.w		set_xop		# initialize WBTEMP
	bset		&wbtemp15_bit,WB_BYTE(%a6)	# set wbtemp15

	mov.l		%a0,%a1
	mov.l		EXC_EA(%a6),%a0	# a0 has the destination pointer
	bsr.l		dest_ext	# store to memory
	bset		&unfl_bit,FPSR_EXCEPT(%a6)
	bra.w		mvout_end
sp_under:
	bset		&etemp15_bit,STAG(%a6)

	cmp.w		%d0,4(%a1)
	blt.b		sp_catas	# catastrophic underflow case	

	mov.l		&1,%d0		# load in round precision
	mov.l		&sgl_thresh,%d1	# load in single denorm threshold
	bsr.l		dpspdnrm	# expects d1 to have the proper
#				;denorm threshold
	bsr.l		dest_sgl	# stores value to destination
	bset		&unfl_bit,FPSR_EXCEPT(%a6)
	bra.w		mvout_end	# exit

dp_under:
	bset		&etemp15_bit,STAG(%a6)

	cmp.w		%d0,4(%a1)
	blt.b		dp_catas	# catastrophic underflow case
	mov.l		&dbl_thresh,%d1	# load in double precision threshold
	mov.l		&2,%d0
	bsr.l		dpspdnrm	# expects d1 to have proper
#				;denorm threshold
#				;expects d0 to have round precision
	bsr.l		dest_dbl	# store value to destination
	bset		&unfl_bit,FPSR_EXCEPT(%a6)
	bra.w		mvout_end	# exit

#
# Handle catastrophic underflow cases here
#
sp_catas:
# Temp fix for z bit set in unf_sub
	mov.l		USER_FPSR(%a6),-(%sp)

	mov.l		&1,%d0		# set round precision to sgl

	bsr.l		unf_sub		# a0 points to result

	mov.l		(%sp)+,USER_FPSR(%a6)

	mov.l		&1,%d0
	sub.w		%d0,LOCAL_EX(%a0)	# account for difference between
#				;denorm/norm bias

	mov.l		%a0,%a1		# a1 has the operand input
	mov.l		EXC_EA(%a6),%a0	# a0 has the destination pointer
	bsr.l		dest_sgl	# store the result
	or.l		&unfinx_mask,USER_FPSR(%a6)
	bra.w		mvout_end
dp_catas:
# Temp fix for z bit set in unf_sub
	mov.l		USER_FPSR(%a6),-(%sp)

	mov.l		&2,%d0		# set round precision to dbl
	bsr.l		unf_sub		# a0 points to result

	mov.l		(%sp)+,USER_FPSR(%a6)

	mov.l		&1,%d0
	sub.w		%d0,LOCAL_EX(%a0)	# account for difference between 
#				;denorm/norm bias

	mov.l		%a0,%a1		# a1 has the operand input
	mov.l		EXC_EA(%a6),%a0	# a0 has the destination pointer
	bsr.l		dest_dbl	# store the result
	or.l		&unfinx_mask,USER_FPSR(%a6)
	bra.w		mvout_end

#
# Handle catastrophic overflow cases here
#
sp_over:
# Temp fix for z bit set in unf_sub
	mov.l		USER_FPSR(%a6),-(%sp)

	mov.l		&1,%d0
	lea.l		FP_SCR1(%a6),%a0	# use FP_SCR1 for creating result
	mov.l		ETEMP_EX(%a6),(%a0)
	mov.l		ETEMP_HI(%a6),4(%a0)
	mov.l		ETEMP_LO(%a6),8(%a0)
	bsr.l		ovf_res

	mov.l		(%sp)+,USER_FPSR(%a6)

	mov.l		%a0,%a1
	mov.l		EXC_EA(%a6),%a0
	bsr.l		dest_sgl
	or.l		&ovfinx_mask,USER_FPSR(%a6)
	bra.w		mvout_end

dp_over:
# Temp fix for z bit set in ovf_res
	mov.l		USER_FPSR(%a6),-(%sp)

	mov.l		&2,%d0
	lea.l		FP_SCR1(%a6),%a0	# use FP_SCR1 for creating result
	mov.l		ETEMP_EX(%a6),(%a0)
	mov.l		ETEMP_HI(%a6),4(%a0)
	mov.l		ETEMP_LO(%a6),8(%a0)
	bsr.l		ovf_res

	mov.l		(%sp)+,USER_FPSR(%a6)

	mov.l		%a0,%a1
	mov.l		EXC_EA(%a6),%a0
	bsr.l		dest_dbl
	or.l		&ovfinx_mask,USER_FPSR(%a6)
	bra.w		mvout_end

#
# 	DPSPDNRM
#
# This subroutine takes an extended normalized number and denormalizes
# it to the given round precision. This subroutine also decrements
# the input operand's exponent by 1 to account for the fact that
# dest_sgl or dest_dbl expects a normalized number's bias.
#
# Input: a0  points to a normalized number in internal extended format
#	 d0  is the round precision (=1 for sgl; =2 for dbl)
#	 d1  is the the single precision or double precision
#	     denorm threshold
#
# Output: (In the format for dest_sgl or dest_dbl)
#	 a0   points to the destination
#   	 a1   points to the operand
#
# Exceptions: Reports inexact 2 exception by setting USER_FPSR bits
#
dpspdnrm:
	mov.l		%d0,-(%sp)	# save round precision
	clr.l		%d0		# clear initial g,r,s
	bsr.l		dnrm_lp		# careful with d0, it's needed by round

	bfextu		FPCR_MODE(%a6){&2:&2},%d1	# get rounding mode
	swap		%d1
	mov.w		2(%sp),%d1	# set rounding precision 
	swap		%d1		# at this point d1 has PREC/MODE info
	bsr.l		round		# round result, sets the inex bit in
#				;USER_FPSR if needed

	mov.w		&1,%d0
	sub.w		%d0,LOCAL_EX(%a0)	# account for difference in denorm
#				;vs norm bias

	mov.l		%a0,%a1		# a1 has the operand input
	mov.l		EXC_EA(%a6),%a0	# a0 has the destination pointer
	add.w		&4,%sp		# pop stack
	rts
#
# SET_XOP initialized WBTEMP with the value pointed to by a0
# input: a0 points to input operand in the internal extended format
#
set_xop:
	mov.l		LOCAL_EX(%a0),WBTEMP_EX(%a6)
	mov.l		LOCAL_HI(%a0),WBTEMP_HI(%a6)
	mov.l		LOCAL_LO(%a0),WBTEMP_LO(%a6)
	bfclr		WBTEMP_SGN(%a6){&0:&8}
	beq.b		sxop
	bset		&sign_bit,WBTEMP_EX(%a6)
sxop:
	bfclr		STAG(%a6){&5:&4}	# clear wbtm66,wbtm1,wbtm0,sbit
	rts
#
#	P_MOVE
#
p_movet:
	long		p_move
	long		p_movez
	long		p_movei
	long		p_moven
	long		p_move
p_regd:
	long		p_dyd0
	long		p_dyd1
	long		p_dyd2
	long		p_dyd3
	long		p_dyd4
	long		p_dyd5
	long		p_dyd6
	long		p_dyd7

pack_out:
	lea.l		p_movet,%a0	# load jmp table address
	mov.w		STAG(%a6),%d0	# get source tag
	bfextu		%d0{&16:&3},%d0	# isolate source bits
	mov.l		(%a0,%d0.w*4),%a0	# load a0 with routine label for tag
	jmp		(%a0)		# go to the routine

p_write:
	mov.l		&0x0c,%d0	# get byte count
	mov.l		EXC_EA(%a6),%a1	# get the destination address
	bsr		mem_write	# write the user's destination
	mov.b		&0,CU_SAVEPC(%a6)	# set the cu save pc to all 0's

#
# Also note that the dtag must be set to norm here - this is because 
# the 040 uses the dtag to execute the correct microcode.
#
	bfclr		DTAG(%a6){&0:&3}	# set dtag to norm

	rts

# Notes on handling of special case (zero, inf, and nan) inputs:
#	1. Operr is not signalled if the k-factor is greater than 18.
#	2. Per the manual, status bits are not set.
#

p_move:
	mov.w		CMDREG1B(%a6),%d0
	btst		&kfact_bit,%d0	# test for dynamic k-factor
	beq.b		statick		# if clear, k-factor is static
dynamick:
	bfextu		%d0{&25:&3},%d0	# isolate register for dynamic k-factor
	lea		p_regd,%a0
	mov.l		(%a0,%d0*4),%a0
	jmp		(%a0)
statick:
	and.w		&0x007f,%d0	# get k-factor
	bfexts		%d0{&25:&7},%d0	# sign extend d0 for bindec
	lea.l		ETEMP(%a6),%a0	# a0 will point to the packed decimal
	bsr.l		bindec		# perform the convert; data at a6
	lea.l		FP_SCR1(%a6),%a0	# load a0 with result address
	bra.l		p_write
p_movez:
	lea.l		ETEMP(%a6),%a0	# a0 will point to the packed decimal
	clr.w		2(%a0)		# clear lower word of exp
	clr.l		4(%a0)		# load second lword of ZERO
	clr.l		8(%a0)		# load third lword of ZERO
	bra.w		p_write		# go write results
p_movei:
	fmov.l		&0,%status	# clear aiop
	lea.l		ETEMP(%a6),%a0	# a0 will point to the packed decimal
	clr.w		2(%a0)		# clear lower word of exp
	bra.w		p_write		# go write the result
p_moven:
	lea.l		ETEMP(%a6),%a0	# a0 will point to the packed decimal
	clr.w		2(%a0)		# clear lower word of exp
	bra.w		p_write		# go write the result

#
# Routines to read the dynamic k-factor from Dn.
#
p_dyd0:
	mov.l		USER_D0(%a6),%d0
	bra.b		statick
p_dyd1:
	mov.l		USER_D1(%a6),%d0
	bra.b		statick
p_dyd2:
	mov.l		%d2,%d0
	bra.b		statick
p_dyd3:
	mov.l		%d3,%d0
	bra.b		statick
p_dyd4:
	mov.l		%d4,%d0
	bra.b		statick
p_dyd5:
	mov.l		%d5,%d0
	bra.b		statick
p_dyd6:
	mov.l		%d6,%d0
	bra.w		statick
p_dyd7:
	mov.l		%d7,%d0
	bra.w		statick

#	end		
