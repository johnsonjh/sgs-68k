#
#	sasin.sa 3.3 12/19/90
#
#	Description: The entry point sAsin computes the inverse sine of
#		an input argument; sAsind does the same except for denormalized
#		input.
#
#	Input: Double-extended number X in location pointed to
#		by address register a0.
#
#	Output: The value arcsin(X) returned in floating-point register Fp0.
#
#	Accuracy and Monotonicity: The returned result is within 3 ulps in
#		64 significant bit, i.e. within 0.5001 ulp to 53 bits if the
#		result is subsequently rounded to double precision. The 
#		result is provably monotonic in double precision.
#
#	Speed: The program sASIN takes approximately 310 cycles.
#
#	Algorithm:
#
#	ASIN
#	1. If |X| >= 1, go to 3.
#
#	2. (|X| < 1) Calculate asin(X) by
#		z := sqrt( [1-X][1+X] )
#		asin(X) = atan( x / z ).
#		Exit.
#
#	3. If |X| > 1, go to 5.
#
#	4. (|X| = 1) sgn := sign(X), return asin(X) := sgn * Pi/2. Exit.
#
#	5. (|X| > 1) Generate an invalid operation by 0 * infinity.
#		Exit.
#

#		Copyright (C) Motorola, Inc. 1990
#			All Rights Reserved
#
#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF MOTOROLA 
#	The copyright notice above does not evidence any  
#	actual or intended publication of such source code.

#	idnt	2,1	Motorola 040 Floating Point Software Package

	text

PIBY2:
	long		0x3FFF0000,0xC90FDAA2,0x2168C235,0x00000000

#	xref	t_operr	
#	xref	t_frcinx	
#	xref	t_extdnrm	
#	xref	satan	

	global		sasind
sasind:
#--ASIN(X) = X FOR DENORMALIZED X

	bra		t_extdnrm

	global		sasin
sasin:
	fmov.x		(%a0),%fp0	# LOAD INPUT

	mov.l		(%a0),%d0
	mov.w		4(%a0),%d0
	and.l		&0x7FFFFFFF,%d0
	cmp.l		%d0,&0x3FFF8000
	bge.b		asinbig

#--THIS IS THE USUAL CASE, |X| < 1
#--ASIN(X) = ATAN( X / SQRT( (1-X)(1+X) ) )

	fmov.s		&0x3F800000,%fp1
	fsub.x		%fp0,%fp1	# 1-X
	fmovm.x		&0x4,-(%sp)	#  {%fp2}
	fmov.s		&0x3F800000,%fp2
	fadd.x		%fp0,%fp2	# 1+X
	fmul.x		%fp2,%fp1	# (1+X)(1-X)
	fmovm.x		(%sp)+,&0x20	#  {%fp2}
	fsqrt.x		%fp1		# SQRT([1-X][1+X])
	fdiv.x		%fp1,%fp0	# X/SQRT([1-X][1+X])
	fmovm.x		&0x80,(%a0)	#  {%fp0}
	bsr		satan
	bra		t_frcinx

asinbig:
	fabs.x		%fp0		# |X|
	fcmp.s		%fp0,&0x3F800000
	fbgt		t_operr		# cause an operr exception

#--|X| = 1, ASIN(X) = +- PI/2.

	fmov.x		PIBY2,%fp0
	mov.l		(%a0),%d0
	and.l		&0x80000000,%d0	# SIGN BIT OF X
	or.l		&0x3F800000,%d0	# +-1 IN SGL FORMAT
	mov.l		%d0,-(%sp)	# push SIGN(X) IN SGL-FMT
	fmov.l		%d1,%control
	fmul.s		(%sp)+,%fp0
	bra		t_frcinx

#	end		
