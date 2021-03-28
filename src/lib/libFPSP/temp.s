#
#	l_fpsp.h 1.2 5/1/91
#

#		Copyright (C) Motorola, Inc. 1991
#			All Rights Reserved
#
#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF MOTOROLA 
#	The copyright notice above does not evidence any  
#	actual or intended publication of such source code.

#	l_fpsp.h --- stack frame offsets for library version of FPSP
#
#	This file is derived from fpsp.h.  All equates that refer
#	to the fsave frame and it's bits are removed with the
#	exception of ETEMP, WBTEMP, DTAG and STAG which are simulated
#	in the library version.  Equates for the exception frame are
#	also not needed.  Some of the equates that are only used in
#	the kernel version of the FPSP are left in to minimize the
#	differences between this file and the original.
#
#	The library routines use the same source files as the regular
#	kernel mode code so they expect the same setup.  That is, you
#	must create enough space on the stack for all save areas and
#	work variables that are needed, and save any registers that
#	your compiler does not treat as scratch registers on return
#	from function calls.
#	
#	The worst case setup is:
#
#		link	a6,#-LOCAL_SIZE
#		movem.l	d0-d1/a0-a1,USER_DA(a6)
#		fmovem.x fp0-fp3,USER_FP0(a6)
#		fmovem.l fpsr/fpcr,USER_FPSR(a6)
#
#	After initialization, the stack looks like this:
#
#	A7 --->	+-------------------------------+
#		|				|
#		|	FPSP Local Variables	|
#		|	     including		|
#		|	  saved registers	|
#		|				|
#		+-------------------------------+
#	A6 --->	|	Saved A6		|
#		+-------------------------------+
#		|	Return PC		|
#		+-------------------------------+
#		|	Arguments to 		|
#		|	an FPSP library		|
#		|	package			|
#		|				|
#
#	Positive offsets from A6 refer to the input arguments.  Negative
#	offsets refer to the Local Variable area.
#
#	On exit, execute:
#
#		movem.l	USER_DA(a6),d0-d1/a0-a1
#		fmovem.x USER_FP0(a6),fp0-fp3
#		fmove.l	USER_FPSR(a6),fpsr/fpcr
#		unlk	a6
#		rts
#
#	Many 68K C compilers treat a0/a1/d0/d1/fp0/fp1 as scratch so
#	a simplified setup/exit is possible:
#
#		link	a6,#-LOCAL_SIZE
#		fmovem.x fp2-fp3,USER_FP2(a6)
#		fmove.l	fpsr/fpcr,USER_FPSR(a6)
#
#		[call appropriate emulation routine]
#
#		fmovem.x USER_FP2(a6),fp2-fp3
#		fmove.l	USER_FPSR(a6),fpsr/fpcr
#		unlk	a6
#		rts
#
#	Note that you must still save fp2/fp3 because the FPSP emulation
#	routines expect fp0-fp3 as scratch registers.  For all monadic
#	entry points, the caller should save the fpcr in d1 and zero the
#	real fpcr before calling the emulation routine.  On return, the
#	monadic emulation code will place the value supplied in d1 back
#	into the fpcr and do a single floating point operation so that
#	the final result will be correctly rounded and any specified
#	exceptions will be generated.
#
#----------------------------------------------------------------------
#
#	Local Variables on the stack
#
	set		LOCAL_SIZE,228	# bytes needed for local variables
	set		LV,-LOCAL_SIZE	# convenient base value
#
	set		USER_DA,LV+0	# save space for D0-D1,A0-A1
	set		USER_D0,LV+0	# saved user D0
	set		USER_D1,LV+4	# saved user D1
	set		USER_A0,LV+8	# saved user A0
	set		USER_A1,LV+12	# saved user A1
	set		USER_FP0,LV+16	# saved user FP0
	set		USER_FP1,LV+28	# saved user FP1
	set		USER_FP2,LV+40	# saved user FP2
	set		USER_FP3,LV+52	# saved user FP3
	set		USER_FPCR,LV+64	# saved user FPCR
	set		FPCR_ENABLE,USER_FPCR+2	# FPCR exception enable 
	set		FPCR_MODE,USER_FPCR+3	# FPCR rounding mode control
	set		USER_FPSR,LV+68	# saved user FPSR
	set		FPSR_CC,USER_FPSR+0	# FPSR condition code
	set		FPSR_QBYTE,USER_FPSR+1	# FPSR quotient
	set		FPSR_EXCEPT,USER_FPSR+2	# FPSR exception
	set		FPSR_AEXCEPT,USER_FPSR+3	# FPSR accrued exception
	set		USER_FPIAR,LV+72	# saved user FPIAR
	set		FP_SCR1,LV+76	# room for a temporary float value
	set		FP_SCR2,LV+92	# room for a temporary float value
	set		L_SCR1,LV+108	# room for a temporary long value
	set		L_SCR2,LV+112	# room for a temporary long value
	set		STORE_FLG,LV+116
	set		BINDEC_FLG,LV+117	# used in bindec
	set		DNRM_FLG,LV+118	# used in res_func
	set		RES_FLG,LV+119	# used in res_func
	set		DY_MO_FLG,LV+120	# dyadic/monadic flag
	set		UFLG_TMP,LV+121	# temporary for uflag errata
	set		CU_ONLY,LV+122	# cu-only flag
	set		VER_TMP,LV+123	# temp holding for version number
	set		L_SCR3,LV+124	# room for a temporary long value
	set		FP_SCR3,LV+128	# room for a temporary float value
	set		FP_SCR4,LV+144	# room for a temporary float value
	set		FP_SCR5,LV+160	# room for a temporary float value
	set		FP_SCR6,LV+176
#
#--------------------------------------------------------------------------
#
	set		STAG,LV+192	# source tag (1 byte)
#
	set		DTAG,LV+193	# dest tag (1 byte)
#
	set		FPTEMP,LV+196	# fptemp (12 bytes)
	set		FPTEMP_EX,FPTEMP	# fptemp sign and exponent (2 bytes)
	set		FPTEMP_HI,FPTEMP+4	# fptemp mantissa [63:32] (4 bytes)
	set		FPTEMP_LO,FPTEMP+8	# fptemp mantissa [31:00] (4 bytes)
#
	set		FPTEMP_SGN,FPTEMP+2	# used to store sign
#
	set		ETEMP,LV+208	# etemp (12 bytes)
	set		ETEMP_EX,ETEMP	# etemp sign and exponent (2 bytes)
	set		ETEMP_HI,ETEMP+4	# etemp mantissa [63:32] (4 bytes)
	set		ETEMP_LO,ETEMP+8	# etemp mantissa [31:00] (4 bytes)
#
	set		ETEMP_SGN,ETEMP+2	# used to store sign
#
#--------------------------------------------------------------------------
#
#	FPSR/FPCR bits
#
	set		neg_bit,3	# negative result
	set		z_bit,2		# zero result
	set		inf_bit,1	# infinity result
	set		nan_bit,0	# not-a-number result
#
	set		q_sn_bit,7	# sign bit of quotient byte
#
	set		bsun_bit,7	# branch on unordered
	set		snan_bit,6	# signalling nan
	set		operr_bit,5	# operand error
	set		ovfl_bit,4	# overflow
	set		unfl_bit,3	# underflow
	set		dz_bit,2	# divide by zero
	set		inex2_bit,1	# inexact result 2
	set		inex1_bit,0	# inexact result 1
#
	set		aiop_bit,7	# accrued illegal operation
	set		aovfl_bit,6	# accrued overflow
	set		aunfl_bit,5	# accrued underflow
	set		adz_bit,4	# accrued divide by zero
	set		ainex_bit,3	# accrued inexact
#
#	FPSR individual bit masks
#
	set		neg_mask,0x08000000
	set		z_mask,0x04000000
	set		inf_mask,0x02000000
	set		nan_mask,0x01000000
#
	set		bsun_mask,0x00008000
	set		snan_mask,0x00004000
	set		operr_mask,0x00002000
	set		ovfl_mask,0x00001000
	set		unfl_mask,0x00000800
	set		dz_mask,0x00000400
	set		inex2_mask,0x00000200
	set		inex1_mask,0x00000100
#
	set		aiop_mask,0x00000080	# accrued illegal operation
	set		aovfl_mask,0x00000040	# accrued overflow
	set		aunfl_mask,0x00000020	# accrued underflow
	set		adz_mask,0x00000010	# accrued divide by zero
	set		ainex_mask,0x00000008	# accrued inexact
#
#	FPSR combinations used in the FPSP
#
	set		dzinf_mask,inf_mask+dz_mask+adz_mask
	set		opnan_mask,nan_mask+operr_mask+aiop_mask
	set		nzi_mask,0x01ffffff	# clears N, Z, and I
	set		unfinx_mask,unfl_mask+inex2_mask+aunfl_mask+ainex_mask
	set		unf2inx_mask,unfl_mask+inex2_mask+ainex_mask
	set		ovfinx_mask,ovfl_mask+inex2_mask+aovfl_mask+ainex_mask
	set		inx1a_mask,inex1_mask+ainex_mask
	set		inx2a_mask,inex2_mask+ainex_mask
	set		snaniop_mask,nan_mask+snan_mask+aiop_mask
	set		naniop_mask,nan_mask+aiop_mask
	set		neginf_mask,neg_mask+inf_mask
	set		infaiop_mask,inf_mask+aiop_mask
	set		negz_mask,neg_mask+z_mask
	set		opaop_mask,operr_mask+aiop_mask
	set		unfl_inx_mask,unfl_mask+aunfl_mask+ainex_mask
	set		ovfl_inx_mask,ovfl_mask+aovfl_mask+ainex_mask
#
#--------------------------------------------------------------------------
#
#	FPCR rounding modes
#
	set		x_mode,0x00	# round to extended
	set		s_mode,0x40	# round to single
	set		d_mode,0x80	# round to double
#
	set		rn_mode,0x00	# round nearest
	set		rz_mode,0x10	# round to zero
	set		rm_mode,0x20	# round to minus infinity
	set		rp_mode,0x30	# round to plus infinity
#
#--------------------------------------------------------------------------
#
#	Miscellaneous equates
#
	set		signan_bit,6	# signalling nan bit in mantissa
	set		sign_bit,7
#
	set		rnd_stky_bit,29	# round/sticky bit of mantissa
#				this can only be used if in a data register
	set		LOCAL_EX,0
	set		LOCAL_SGN,2
	set		LOCAL_HI,4
	set		LOCAL_LO,8
	set		LOCAL_GRS,12	# valid ONLY for FP_SCR1, FP_SCR2
#
#
	set		norm_tag,0x00	# tag bits in {7:5} position
	set		zero_tag,0x20
	set		inf_tag,0x40
	set		nan_tag,0x60
	set		dnrm_tag,0x80
#
	set		dbl_thresh,0x3C01
	set		sgl_thresh,0x3F81
#
#
#	l_support.sa 1.2 5/1/91
#
#		Copyright (C) Motorola, Inc. 1991
#			All Rights Reserved
#
#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF MOTOROLA 
#	The copyright notice above does not evidence any  
#	actual or intended publication of such source code.

#	idnt	2,1	Motorola 040 Floating Point Software Package

	text

mns_one:
	long		0xbfff0000,0x80000000,0x00000000
pls_one:
	long		0x3fff0000,0x80000000,0x00000000
pls_inf:
	long		0x7fff0000,0x00000000,0x00000000
pls_huge:
	long		0x7ffe0000,0xffffffff,0xffffffff
mns_huge:
	long		0xfffe0000,0xffffffff,0xffffffff
pls_tiny:
	long		0x00000000,0x80000000,0x00000000
mns_tiny:
	long		0x80000000,0x80000000,0x00000000
small:
	long		0x20000000,0x80000000,0x00000000
pls_zero:
	long		0x00000000,0x00000000,0x00000000

#	include	l_fpsp.h	

#
# 	tag --- determine the type of an extended precision operand
#
#	The tag values returned match the way the 68040 would have
#	tagged them.
#
#	Input:	a0 points to operand
#
#	Output	d0.b	= $00 norm
#			  $20 zero
#			  $40 inf
#			  $60 nan
#			  $80 denorm
#		All other registers are unchanged
#
	global		tag
tag:
	mov.w		LOCAL_EX(%a0),%d0
	and.w		&0x7fff,%d0
	beq.b		chk_zro
	cmp.w		%d0,&0x7fff
	beq.b		chk_inf
tag_nrm:
	clr.b		%d0
	rts
tag_nan:
	mov.b		&0x60,%d0
	rts
tag_dnrm:
	mov.b		&0x80,%d0
	rts
chk_zro:
	btst		&7,LOCAL_HI(%a0)	# # check if J-bit is set
	bne.b		tag_nrm
	tst.l		LOCAL_HI(%a0)
	bne.b		tag_dnrm
	tst.l		LOCAL_LO(%a0)
	bne.b		tag_dnrm
tag_zero:
	mov.b		&0x20,%d0
	rts
chk_inf:
	tst.l		LOCAL_HI(%a0)
	bne.b		tag_nan
	tst.l		LOCAL_LO(%a0)
	bne.b		tag_nan
tag_inf:
	mov.b		&0x40,%d0
	rts

#
#	t_dz, t_dz2 --- divide by zero exception
#
# t_dz2 is used by monadic functions such as flogn (from do_func).
# t_dz is used by monadic functions such as satanh (from the 
# transcendental function).
#
	global		t_dz2
t_dz2:
	fmovm.x		mns_one,&0x80	#  {%fp0}
	fmov.l		%d1,%control
	fdiv.x		pls_zero,%fp0
	rts

	global		t_dz
t_dz:
	btst		&sign_bit,ETEMP_EX(%a6)	# check sign for neg or pos
	beq.b		p_inf		# branch if pos sign
m_inf:
	fmovm.x		mns_one,&0x80	#  {%fp0}
	fmov.l		%d1,%control
	fdiv.x		pls_zero,%fp0
	rts
p_inf:
	fmovm.x		pls_one,&0x80	#  {%fp0}
	fmov.l		%d1,%control
	fdiv.x		pls_zero,%fp0
	rts
#
#	t_operr --- Operand Error exception
#
	global		t_operr
t_operr:
	fmovm.x		pls_inf,&0x80	#  {%fp0}
	fmov.l		%d1,%control
	fmul.x		pls_zero,%fp0
	rts

#
#	t_unfl --- UNFL exception
#
	global		t_unfl
t_unfl:
	btst		&sign_bit,ETEMP(%a6)
	beq.b		unf_pos
unf_neg:
	fmovm.x		mns_tiny,&0x80	#  {%fp0}
	fmov.l		%d1,%control
	fmul.x		pls_tiny,%fp0
	rts
unf_pos:
	fmovm.x		pls_tiny,&0x80	#  {%fp0}
	fmov.l		%d1,%control
	fmul.x		%fp0,%fp0
	rts
#
#	t_ovfl --- OVFL exception
#
#	t_ovfl is called as an exit for monadic functions.  t_ovfl2
#	is for dyadic exits.
#
	global		t_ovfl
t_ovfl:
	global		t_ovfl2
	mov.l		%d1,USER_FPCR(%a6)	# user's control register
	mov.l		&ovfinx_mask,%d0
	bra.b		t_work
t_ovfl2:
	mov.l		&ovfl_inx_mask,%d0
t_work:
	btst		&sign_bit,ETEMP(%a6)
	beq.b		ovf_pos
ovf_neg:
	fmovm.x		mns_huge,&0x80	#  {%fp0}
	fmov.l		USER_FPCR(%a6),%control
	fmul.x		pls_huge,%fp0
	fmov.l		%status,%d1
	or.l		%d1,%d0
	fmov.l		%d0,%status
	rts
ovf_pos:
	fmovm.x		pls_huge,&0x80	#  {%fp0}
	fmov.l		USER_FPCR(%a6),%control
	fmul.x		pls_huge,%fp0
	fmov.l		%status,%d1
	or.l		%d1,%d0
	fmov.l		%d0,%status
	rts
#
#	t_inx2 --- INEX2 exception (correct fpcr is in USER_FPCR(a6))
#
	global		t_inx2
t_inx2:
	fmov.l		%status,USER_FPSR(%a6)	# capture incoming fpsr
	fmov.l		USER_FPCR(%a6),%control
#
# create an inex2 exception by adding two numbers with very different exponents
# do the add in fp1 so as to not disturb the result sitting in fp0
#
	fmov.x		pls_one,%fp1
	fadd.x		small,%fp1
#
	or.l		&inx2a_mask,USER_FPSR(%a6)	# set INEX2, AINEX
	fmov.l		USER_FPSR(%a6),%status
	rts
#
#	t_frcinx --- Force Inex2 (for monadic functions)
#
	global		t_frcinx
t_frcinx:
	fmov.l		%status,USER_FPSR(%a6)	# capture incoming fpsr
	fmov.l		%d1,%control
#
# create an inex2 exception by adding two numbers with very different exponents
# do the add in fp1 so as to not disturb the result sitting in fp0
#
	fmov.x		pls_one,%fp1
	fadd.x		small,%fp1
#
	or.l		&inx2a_mask,USER_FPSR(%a6)	# set INEX2, AINEX
	btst		&unfl_bit,FPSR_EXCEPT(%a6)	# test for unfl bit set
	beq.b		no_uacc1	# if clear, do not set aunfl
	bset		&aunfl_bit,FPSR_AEXCEPT(%a6)
no_uacc1:
	fmov.l		USER_FPSR(%a6),%status
	rts
#
#	dst_nan --- force result when destination is a NaN
#
	global		dst_nan
dst_nan:
	fmov.l		USER_FPCR(%a6),%control
	fmov.x		FPTEMP(%a6),%fp0
	rts

#
#	src_nan --- force result when source is a NaN
#
	global		src_nan
src_nan:
	fmov.l		USER_FPCR(%a6),%control
	fmov.x		ETEMP(%a6),%fp0
	rts
#
#	mon_nan --- force result when source is a NaN (monadic version)
#
#	This is the same as src_nan except that the user's fpcr comes
#	in via d1, not USER_FPCR(a6).
#
	global		mon_nan
mon_nan:
	fmov.l		%d1,%control
	fmov.x		ETEMP(%a6),%fp0
	rts
#
#	t_extdnrm, t_resdnrm --- generate results for denorm inputs
#
#	For all functions that have a denormalized input and that f(x)=x,
#	this is the entry point.
#
	global		t_extdnrm
t_extdnrm:
	fmov.l		%d1,%control
	fmov.x		LOCAL_EX(%a0),%fp0
	fmov.l		%status,%d0
	or.l		&unfinx_mask,%d0
	fmov.l		%d0,%status
	rts

	global		t_resdnrm
t_resdnrm:
	fmov.l		USER_FPCR(%a6),%control
	fmov.x		LOCAL_EX(%a0),%fp0
	fmov.l		%status,%d0
	or.l		&unfl_mask,%d0
	fmov.l		%d0,%status
	rts
#
#
#
	global		t_avoid_unsupp
t_avoid_unsupp:
	fmov.x		%fp0,%fp0
	rts

	global		sto_cos
sto_cos:
	fmovm.x		LOCAL_EX(%a0),&0x40	#  {%fp1}
	rts
#
#	Native instruction support
#
#	Some systems may need entry points even for 68040 native
#	instructions.  These routines are provided for
#	convenience.
#
	global		sadd
sadd:
	fmovm.x		FPTEMP(%a6),&0x80	#  {%fp0}
	fmov.l		USER_FPCR(%a6),%control
	fadd.x		ETEMP(%a6),%fp0
	rts

	global		ssub
ssub:
	fmovm.x		FPTEMP(%a6),&0x80	#  {%fp0}
	fmov.l		USER_FPCR(%a6),%control
	fsub.x		ETEMP(%a6),%fp0
	rts

	global		smul
smul:
	fmovm.x		FPTEMP(%a6),&0x80	#  {%fp0}
	fmov.l		USER_FPCR(%a6),%control
	fmul.x		ETEMP(%a6),%fp0
	rts

	global		sdiv
sdiv:
	fmovm.x		FPTEMP(%a6),&0x80	#  {%fp0}
	fmov.l		USER_FPCR(%a6),%control
	fdiv.x		ETEMP(%a6),%fp0
	rts

	global		sabs
sabs:
	fmovm.x		ETEMP(%a6),&0x80	#  {%fp0}
	fmov.l		%d1,%control
	fabs		%fp0
	rts

	global		sneg
sneg:
	fmovm.x		ETEMP(%a6),&0x80	#  {%fp0}
	fmov.l		%d1,%control
	fneg		%fp0
	rts

	global		ssqrt
ssqrt:
	fmovm.x		ETEMP(%a6),&0x80	#  {%fp0}
	fmov.l		%d1,%control
	fsqrt		%fp0
	rts

#
#	l_sint,l_sintrz,l_sintd --- special wrapper for fint and fintrz
#
#	On entry, move the user's FPCR to USER_FPCR.
#
#	On return from, we need to pickup the INEX2/AINEX bits
#	that are in USER_FPSR.
#
#	xref	sint	
#	xref	sintrz	
#	xref	sintd	

	global		l_sint
l_sint:
	mov.l		%d1,USER_FPCR(%a6)
	jsr		sint
	fmov.l		%status,%d0
	or.l		USER_FPSR(%a6),%d0
	fmov.l		%d0,%status
	rts

	global		l_sintrz
l_sintrz:
	mov.l		%d1,USER_FPCR(%a6)
	jsr		sintrz
	fmov.l		%status,%d0
	or.l		USER_FPSR(%a6),%d0
	fmov.l		%d0,%status
	rts

	global		l_sintd
l_sintd:
	mov.l		%d1,USER_FPCR(%a6)
	jsr		sintd
	fmov.l		%status,%d0
	or.l		USER_FPSR(%a6),%d0
	fmov.l		%d0,%status
	rts

#	end		
