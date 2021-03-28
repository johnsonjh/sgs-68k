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
