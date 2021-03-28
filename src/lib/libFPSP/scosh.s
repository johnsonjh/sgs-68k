#
#	scosh.sa 3.1 12/10/90
#
#	The entry point sCosh computes the hyperbolic cosine of
#	an input argument; sCoshd does the same except for denormalized
#	input.
#
#	Input: Double-extended number X in location pointed to
#		by address register a0.
#
#	Output: The value cosh(X) returned in floating-point register Fp0.
#
#	Accuracy and Monotonicity: The returned result is within 3 ulps in
#		64 significant bit, i.e. within 0.5001 ulp to 53 bits if the
#		result is subsequently rounded to double precision. The
#		result is provably monotonic in double precision.
#
#	Speed: The program sCOSH takes approximately 250 cycles.
#
#	Algorithm:
#
#	COSH
#	1. If |X| > 16380 log2, go to 3.
#
#	2. (|X| <= 16380 log2) Cosh(X) is obtained by the formulae
#		y = |X|, z = exp(Y), and
#		cosh(X) = (1/2)*( z + 1/z ).
#		Exit.
#
#	3. (|X| > 16380 log2). If |X| > 16480 log2, go to 5.
#
#	4. (16380 log2 < |X| <= 16480 log2)
#		cosh(X) = sign(X) * exp(|X|)/2.
#		However, invoking exp(|X|) may cause premature overflow.
#		Thus, we calculate sinh(X) as follows:
#		Y	:= |X|
#		Fact	:=	2**(16380)
#		Y'	:= Y - 16381 log2
#		cosh(X) := Fact * exp(Y').
#		Exit.
#
#	5. (|X| > 16480 log2) sinh(X) must overflow. Return
#		Huge*Huge to generate overflow and an infinity with
#		the appropriate sign. Huge is the largest finite number in
#		extended format. Exit.
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

#	xref	t_ovfl	
#	xref	t_frcinx	
#	xref	setox	

T1:
	long		0x40C62D38,0xD3D64634	# 16381 LOG2 LEAD
T2:
	long		0x3D6F90AE,0xB1E75CC7	# 16381 LOG2 TRAIL

TWO16380:
	long		0x7FFB0000,0x80000000,0x00000000,0x00000000

	global		scoshd
scoshd:
#--COSH(X) = 1 FOR DENORMALIZED X

	fmov.s		&0x3F800000,%fp0

	fmov.l		%d1,%control
	fadd.s		&0x00800000,%fp0
	bra		t_frcinx

	global		scosh
scosh:
	fmov.x		(%a0),%fp0	# LOAD INPUT

	mov.l		(%a0),%d0
	mov.w		4(%a0),%d0
	and.l		&0x7FFFFFFF,%d0
	cmp.l		%d0,&0x400CB167
	bgt.b		COSHBIG

#--THIS IS THE USUAL CASE, |X| < 16380 LOG2
#--COSH(X) = (1/2) * ( EXP(X) + 1/EXP(X) )

	fabs.x		%fp0		# |X|

	mov.l		%d1,-(%sp)
	clr.l		%d1
	fmovm.x		&0x80,(%a0)	# pass parameter to setox {%fp0}
	bsr		setox		# FP0 IS EXP(|X|)
	fmul.s		&0x3F000000,%fp0	# (1/2)EXP(|X|)
	mov.l		(%sp)+,%d1

	fmov.s		&0x3E800000,%fp1	# (1/4)
	fdiv.x		%fp0,%fp1	# 1/(2 EXP(|X|))

	fmov.l		%d1,%control
	fadd.x		%fp1,%fp0

	bra		t_frcinx

COSHBIG:
	cmp.l		%d0,&0x400CB2B3
	bgt.b		COSHHUGE

	fabs.x		%fp0
	fsub.d		T1(%pc),%fp0	# (|X|-16381LOG2_LEAD)
	fsub.d		T2(%pc),%fp0	# |X| - 16381 LOG2, ACCURATE

	mov.l		%d1,-(%sp)
	clr.l		%d1
	fmovm.x		&0x80,(%a0)	#  {%fp0}
	bsr		setox
	fmov.l		(%sp)+,%control

	fmul.x		TWO16380(%pc),%fp0
	bra		t_frcinx

COSHHUGE:
	fmov.l		&0,%status	# clr N bit if set by source
	bclr		&7,(%a0)	# always return positive value
	fmovm.x		(%a0),&0x80	#  {%fp0}
	bra		t_ovfl

#	end		
