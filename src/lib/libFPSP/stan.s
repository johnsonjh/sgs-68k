#
#	stan.sa 3.3 7/29/91
#
#	The entry point stan computes the tangent of
#	an input argument;
#	stand does the same except for denormalized input.
#
#	Input: Double-extended number X in location pointed to
#		by address register a0.
#
#	Output: The value tan(X) returned in floating-point register Fp0.
#
#	Accuracy and Monotonicity: The returned result is within 3 ulp in
#		64 significant bit, i.e. within 0.5001 ulp to 53 bits if the
#		result is subsequently rounded to double precision. The
#		result is provably monotonic in double precision.
#
#	Speed: The program sTAN takes approximately 170 cycles for
#		input argument X such that |X| < 15Pi, which is the the usual
#		situation.
#
#	Algorithm:
#
#	1. If |X| >= 15Pi or |X| < 2**(-40), go to 6.
#
#	2. Decompose X as X = N(Pi/2) + r where |r| <= Pi/4. Let
#		k = N mod 2, so in particular, k = 0 or 1.
#
#	3. If k is odd, go to 5.
#
#	4. (k is even) Tan(X) = tan(r) and tan(r) is approximated by a
#		rational function U/V where
#		U = r + r*s*(P1 + s*(P2 + s*P3)), and
#		V = 1 + s*(Q1 + s*(Q2 + s*(Q3 + s*Q4))),  s = r*r.
#		Exit.
#
#	4. (k is odd) Tan(X) = -cot(r). Since tan(r) is approximated by a
#		rational function U/V where
#		U = r + r*s*(P1 + s*(P2 + s*P3)), and
#		V = 1 + s*(Q1 + s*(Q2 + s*(Q3 + s*Q4))), s = r*r,
#		-Cot(r) = -V/U. Exit.
#
#	6. If |X| > 1, go to 8.
#
#	7. (|X|<2**(-40)) Tan(X) = X. Exit.
#
#	8. Overwrite X by X := X rem 2Pi. Now that |X| <= Pi, go back to 2.
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

BOUNDS1:
	long		0x3FD78000,0x4004BC7E
TWOBYPI:
	long		0x3FE45F30,0x6DC9C883

TANQ4:
	long		0x3EA0B759,0xF50F8688
TANP3:
	long		0xBEF2BAA5,0xA8924F04

TANQ3:
	long		0xBF346F59,0xB39BA65F,0x00000000,0x00000000

TANP2:
	long		0x3FF60000,0xE073D3FC,0x199C4A00,0x00000000

TANQ2:
	long		0x3FF90000,0xD23CD684,0x15D95FA1,0x00000000

TANP1:
	long		0xBFFC0000,0x8895A6C5,0xFB423BCA,0x00000000

TANQ1:
	long		0xBFFD0000,0xEEF57E0D,0xA84BC8CE,0x00000000

INVTWOPI:
	long		0x3FFC0000,0xA2F9836E,0x4E44152A,0x00000000

TWOPI1:
	long		0x40010000,0xC90FDAA2,0x00000000,0x00000000
TWOPI2:
	long		0x3FDF0000,0x85A308D4,0x00000000,0x00000000

#--N*PI/2, -32 <= N <= 32, IN A LEADING TERM IN EXT. AND TRAILING
#--TERM IN SGL. NOTE THAT PI IS 64-BIT LONG, THUS N*PI/2 IS AT
#--MOST 69 BITS LONG.
	global		PITBL
PITBL:
	long		0xC0040000,0xC90FDAA2,0x2168C235,0x21800000
	long		0xC0040000,0xC2C75BCD,0x105D7C23,0xA0D00000
	long		0xC0040000,0xBC7EDCF7,0xFF523611,0xA1E80000
	long		0xC0040000,0xB6365E22,0xEE46F000,0x21480000
	long		0xC0040000,0xAFEDDF4D,0xDD3BA9EE,0xA1200000
	long		0xC0040000,0xA9A56078,0xCC3063DD,0x21FC0000
	long		0xC0040000,0xA35CE1A3,0xBB251DCB,0x21100000
	long		0xC0040000,0x9D1462CE,0xAA19D7B9,0xA1580000
	long		0xC0040000,0x96CBE3F9,0x990E91A8,0x21E00000
	long		0xC0040000,0x90836524,0x88034B96,0x20B00000
	long		0xC0040000,0x8A3AE64F,0x76F80584,0xA1880000
	long		0xC0040000,0x83F2677A,0x65ECBF73,0x21C40000
	long		0xC0030000,0xFB53D14A,0xA9C2F2C2,0x20000000
	long		0xC0030000,0xEEC2D3A0,0x87AC669F,0x21380000
	long		0xC0030000,0xE231D5F6,0x6595DA7B,0xA1300000
	long		0xC0030000,0xD5A0D84C,0x437F4E58,0x9FC00000
	long		0xC0030000,0xC90FDAA2,0x2168C235,0x21000000
	long		0xC0030000,0xBC7EDCF7,0xFF523611,0xA1680000
	long		0xC0030000,0xAFEDDF4D,0xDD3BA9EE,0xA0A00000
	long		0xC0030000,0xA35CE1A3,0xBB251DCB,0x20900000
	long		0xC0030000,0x96CBE3F9,0x990E91A8,0x21600000
	long		0xC0030000,0x8A3AE64F,0x76F80584,0xA1080000
	long		0xC0020000,0xFB53D14A,0xA9C2F2C2,0x1F800000
	long		0xC0020000,0xE231D5F6,0x6595DA7B,0xA0B00000
	long		0xC0020000,0xC90FDAA2,0x2168C235,0x20800000
	long		0xC0020000,0xAFEDDF4D,0xDD3BA9EE,0xA0200000
	long		0xC0020000,0x96CBE3F9,0x990E91A8,0x20E00000
	long		0xC0010000,0xFB53D14A,0xA9C2F2C2,0x1F000000
	long		0xC0010000,0xC90FDAA2,0x2168C235,0x20000000
	long		0xC0010000,0x96CBE3F9,0x990E91A8,0x20600000
	long		0xC0000000,0xC90FDAA2,0x2168C235,0x1F800000
	long		0xBFFF0000,0xC90FDAA2,0x2168C235,0x1F000000
	long		0x00000000,0x00000000,0x00000000,0x00000000
	long		0x3FFF0000,0xC90FDAA2,0x2168C235,0x9F000000
	long		0x40000000,0xC90FDAA2,0x2168C235,0x9F800000
	long		0x40010000,0x96CBE3F9,0x990E91A8,0xA0600000
	long		0x40010000,0xC90FDAA2,0x2168C235,0xA0000000
	long		0x40010000,0xFB53D14A,0xA9C2F2C2,0x9F000000
	long		0x40020000,0x96CBE3F9,0x990E91A8,0xA0E00000
	long		0x40020000,0xAFEDDF4D,0xDD3BA9EE,0x20200000
	long		0x40020000,0xC90FDAA2,0x2168C235,0xA0800000
	long		0x40020000,0xE231D5F6,0x6595DA7B,0x20B00000
	long		0x40020000,0xFB53D14A,0xA9C2F2C2,0x9F800000
	long		0x40030000,0x8A3AE64F,0x76F80584,0x21080000
	long		0x40030000,0x96CBE3F9,0x990E91A8,0xA1600000
	long		0x40030000,0xA35CE1A3,0xBB251DCB,0xA0900000
	long		0x40030000,0xAFEDDF4D,0xDD3BA9EE,0x20A00000
	long		0x40030000,0xBC7EDCF7,0xFF523611,0x21680000
	long		0x40030000,0xC90FDAA2,0x2168C235,0xA1000000
	long		0x40030000,0xD5A0D84C,0x437F4E58,0x1FC00000
	long		0x40030000,0xE231D5F6,0x6595DA7B,0x21300000
	long		0x40030000,0xEEC2D3A0,0x87AC669F,0xA1380000
	long		0x40030000,0xFB53D14A,0xA9C2F2C2,0xA0000000
	long		0x40040000,0x83F2677A,0x65ECBF73,0xA1C40000
	long		0x40040000,0x8A3AE64F,0x76F80584,0x21880000
	long		0x40040000,0x90836524,0x88034B96,0xA0B00000
	long		0x40040000,0x96CBE3F9,0x990E91A8,0xA1E00000
	long		0x40040000,0x9D1462CE,0xAA19D7B9,0x21580000
	long		0x40040000,0xA35CE1A3,0xBB251DCB,0xA1100000
	long		0x40040000,0xA9A56078,0xCC3063DD,0xA1FC0000
	long		0x40040000,0xAFEDDF4D,0xDD3BA9EE,0x21200000
	long		0x40040000,0xB6365E22,0xEE46F000,0xA1480000
	long		0x40040000,0xBC7EDCF7,0xFF523611,0x21E80000
	long		0x40040000,0xC2C75BCD,0x105D7C23,0x20D00000
	long		0x40040000,0xC90FDAA2,0x2168C235,0xA1800000

	set		INARG,FP_SCR4

	set		TWOTO63,L_SCR1
	set		ENDFLAG,L_SCR2
	set		N,L_SCR3

#	xref	t_frcinx	
#	xref	t_extdnrm	

	global		stand
stand:
#--TAN(X) = X FOR DENORMALIZED X

	bra		t_extdnrm

	global		stan
stan:
	fmov.x		(%a0),%fp0	# LOAD INPUT

	mov.l		(%a0),%d0
	mov.w		4(%a0),%d0
	and.l		&0x7FFFFFFF,%d0

	cmp.l		%d0,&0x3FD78000	# |X| >= 2**(-40)?
	bge.b		TANOK1
	bra.w		TANSM
TANOK1:
	cmp.l		%d0,&0x4004BC7E	# |X| < 15 PI?
	blt.b		TANMAIN
	bra.w		REDUCEX


TANMAIN:
#--THIS IS THE USUAL CASE, |X| <= 15 PI.
#--THE ARGUMENT REDUCTION IS DONE BY TABLE LOOK UP.
	fmov.x		%fp0,%fp1
	fmul.d		TWOBYPI,%fp1	# X*2/PI

#--HIDE THE NEXT TWO INSTRUCTIONS
	lea.l		PITBL+0x200,%a1	# TABLE OF N*PI/2, N = -32,...,32

#--FP1 IS NOW READY
	fmov.l		%fp1,%d0	# CONVERT TO INTEGER

	asl.l		&4,%d0
	add.l		%d0,%a1		# ADDRESS N*PIBY2 IN Y1, Y2

	fsub.x		(%a1)+,%fp0	# X-Y1
#--HIDE THE NEXT ONE

	fsub.s		(%a1),%fp0	# FP0 IS R = (X-Y1)-Y2

	ror.l		&5,%d0
	and.l		&0x80000000,%d0	# D0 WAS ODD IFF D0 < 0

TANCONT:

	cmp.l		%d0,&0
	blt.w		NODD

	fmov.x		%fp0,%fp1
	fmul.x		%fp1,%fp1	# S = R*R

	fmov.d		TANQ4,%fp3
	fmov.d		TANP3,%fp2

	fmul.x		%fp1,%fp3	# SQ4
	fmul.x		%fp1,%fp2	# SP3

	fadd.d		TANQ3,%fp3	# Q3+SQ4
	fadd.x		TANP2,%fp2	# P2+SP3

	fmul.x		%fp1,%fp3	# S(Q3+SQ4)
	fmul.x		%fp1,%fp2	# S(P2+SP3)

	fadd.x		TANQ2,%fp3	# Q2+S(Q3+SQ4)
	fadd.x		TANP1,%fp2	# P1+S(P2+SP3)

	fmul.x		%fp1,%fp3	# S(Q2+S(Q3+SQ4))
	fmul.x		%fp1,%fp2	# S(P1+S(P2+SP3))

	fadd.x		TANQ1,%fp3	# Q1+S(Q2+S(Q3+SQ4))
	fmul.x		%fp0,%fp2	# RS(P1+S(P2+SP3))

	fmul.x		%fp3,%fp1	# S(Q1+S(Q2+S(Q3+SQ4)))

	fadd.x		%fp2,%fp0	# R+RS(P1+S(P2+SP3))

	fadd.s		&0x3F800000,%fp1	# 1+S(Q1+...)

	fmov.l		%d1,%control	# restore users exceptions
	fdiv.x		%fp1,%fp0	# last inst - possible exception set

	bra		t_frcinx

NODD:
	fmov.x		%fp0,%fp1
	fmul.x		%fp0,%fp0	# S = R*R

	fmov.d		TANQ4,%fp3
	fmov.d		TANP3,%fp2

	fmul.x		%fp0,%fp3	# SQ4
	fmul.x		%fp0,%fp2	# SP3

	fadd.d		TANQ3,%fp3	# Q3+SQ4
	fadd.x		TANP2,%fp2	# P2+SP3

	fmul.x		%fp0,%fp3	# S(Q3+SQ4)
	fmul.x		%fp0,%fp2	# S(P2+SP3)

	fadd.x		TANQ2,%fp3	# Q2+S(Q3+SQ4)
	fadd.x		TANP1,%fp2	# P1+S(P2+SP3)

	fmul.x		%fp0,%fp3	# S(Q2+S(Q3+SQ4))
	fmul.x		%fp0,%fp2	# S(P1+S(P2+SP3))

	fadd.x		TANQ1,%fp3	# Q1+S(Q2+S(Q3+SQ4))
	fmul.x		%fp1,%fp2	# RS(P1+S(P2+SP3))

	fmul.x		%fp3,%fp0	# S(Q1+S(Q2+S(Q3+SQ4)))

	fadd.x		%fp2,%fp1	# R+RS(P1+S(P2+SP3))
	fadd.s		&0x3F800000,%fp0	# 1+S(Q1+...)

	fmov.x		%fp1,-(%sp)
	eor.l		&0x80000000,(%sp)

	fmov.l		%d1,%control	# restore users exceptions
	fdiv.x		(%sp)+,%fp0	# last inst - possible exception set

	bra		t_frcinx

TANBORS:
#--IF |X| > 15PI, WE USE THE GENERAL ARGUMENT REDUCTION.
#--IF |X| < 2**(-40), RETURN X OR 1.
	cmp.l		%d0,&0x3FFF8000
	bgt.b		REDUCEX

TANSM:

	fmov.x		%fp0,-(%sp)
	fmov.l		%d1,%control	# restore users exceptions
	fmov.x		(%sp)+,%fp0	# last inst - posibble exception set

	bra		t_frcinx


REDUCEX:
#--WHEN REDUCEX IS USED, THE CODE WILL INEVITABLY BE SLOW.
#--THIS REDUCTION METHOD, HOWEVER, IS MUCH FASTER THAN USING
#--THE REMAINDER INSTRUCTION WHICH IS NOW IN SOFTWARE.

	fmovm.x		&0x3c,-(%sp)	# save FP2 through FP5 {%fp2-%fp5}
	mov.l		%d2,-(%sp)
	fmov.s		&0x00000000,%fp1

#--If compact form of abs(arg) in d0=$7ffeffff, argument is so large that
#--there is a danger of unwanted overflow in first LOOP iteration.  In this
#--case, reduce argument by one remainder step to make subsequent reduction
#--safe.
	cmp.l		%d0,&0x7ffeffff	# is argument dangerously large?
	bne.b		LOOP
	mov.l		&0x7ffe0000,FP_SCR2(%a6)	# yes
#					;create 2**16383*PI/2
	mov.l		&0xc90fdaa2,FP_SCR2+4(%a6)
	clr.l		FP_SCR2+8(%a6)
	ftest.x		%fp0		# test sign of argument
	mov.l		&0x7fdc0000,FP_SCR3(%a6)	# create low half of 2**16383*
#					;PI/2 at FP_SCR3
	mov.l		&0x85a308d3,FP_SCR3+4(%a6)
	clr.l		FP_SCR3+8(%a6)
	fblt.w		red_neg
	or.w		&0x8000,FP_SCR2(%a6)	# positive arg
	or.w		&0x8000,FP_SCR3(%a6)
red_neg:
	fadd.x		FP_SCR2(%a6),%fp0	# high part of reduction is exact
	fmov.x		%fp0,%fp1	# save high result in fp1
	fadd.x		FP_SCR3(%a6),%fp0	# low part of reduction
	fsub.x		%fp0,%fp1	# determine low component of result
	fadd.x		FP_SCR3(%a6),%fp1	# fp0/fp1 are reduced argument.

#--ON ENTRY, FP0 IS X, ON RETURN, FP0 IS X REM PI/2, |X| <= PI/4.
#--integer quotient will be stored in N
#--Intermeditate remainder is 66-bit long; (R,r) in (FP0,FP1)

LOOP:
	fmov.x		%fp0,INARG(%a6)	# +-2**K * F, 1 <= F < 2
	mov.w		INARG(%a6),%d0
	mov.l		%d0,%a1		# save a copy of D0
	and.l		&0x00007FFF,%d0
	sub.l		&0x00003FFF,%d0	# D0 IS K
	cmp.l		%d0,&28
	ble.b		LASTLOOP
CONTLOOP:
	sub.l		&27,%d0		# D0 IS L := K-27
	mov.l		&0,ENDFLAG(%a6)
	bra.b		WORK
LASTLOOP:
	clr.l		%d0		# D0 IS L := 0
	mov.l		&1,ENDFLAG(%a6)

WORK:
#--FIND THE REMAINDER OF (R,r) W.R.T.	2**L * (PI/2). L IS SO CHOSEN
#--THAT	INT( X * (2/PI) / 2**(L) ) < 2**29.

#--CREATE 2**(-L) * (2/PI), SIGN(INARG)*2**(63),
#--2**L * (PIby2_1), 2**L * (PIby2_2)

	mov.l		&0x00003FFE,%d2	# BIASED EXPO OF 2/PI
	sub.l		%d0,%d2		# BIASED EXPO OF 2**(-L)*(2/PI)

	mov.l		&0xA2F9836E,FP_SCR1+4(%a6)
	mov.l		&0x4E44152A,FP_SCR1+8(%a6)
	mov.w		%d2,FP_SCR1(%a6)	# FP_SCR1 is 2**(-L)*(2/PI)

	fmov.x		%fp0,%fp2
	fmul.x		FP_SCR1(%a6),%fp2
#--WE MUST NOW FIND INT(FP2). SINCE WE NEED THIS VALUE IN
#--FLOATING POINT FORMAT, THE TWO FMOVE'S	FMOVE.L FP <--> N
#--WILL BE TOO INEFFICIENT. THE WAY AROUND IT IS THAT
#--(SIGN(INARG)*2**63	+	FP2) - SIGN(INARG)*2**63 WILL GIVE
#--US THE DESIRED VALUE IN FLOATING POINT.

#--HIDE SIX CYCLES OF INSTRUCTION
	mov.l		%a1,%d2
	swap		%d2
	and.l		&0x80000000,%d2
	or.l		&0x5F000000,%d2	# D2 IS SIGN(INARG)*2**63 IN SGL
	mov.l		%d2,TWOTO63(%a6)

	mov.l		%d0,%d2
	add.l		&0x00003FFF,%d2	# BIASED EXPO OF 2**L * (PI/2)

#--FP2 IS READY
	fadd.s		TWOTO63(%a6),%fp2	# THE FRACTIONAL PART OF FP1 IS ROUNDED

#--HIDE 4 CYCLES OF INSTRUCTION; creating 2**(L)*Piby2_1  and  2**(L)*Piby2_2
	mov.w		%d2,FP_SCR2(%a6)
	clr.w		FP_SCR2+2(%a6)
	mov.l		&0xC90FDAA2,FP_SCR2+4(%a6)
	clr.l		FP_SCR2+8(%a6)	# FP_SCR2 is  2**(L) * Piby2_1	

#--FP2 IS READY
	fsub.s		TWOTO63(%a6),%fp2	# FP2 is N

	add.l		&0x00003FDD,%d0
	mov.w		%d0,FP_SCR3(%a6)
	clr.w		FP_SCR3+2(%a6)
	mov.l		&0x85A308D3,FP_SCR3+4(%a6)
	clr.l		FP_SCR3+8(%a6)	# FP_SCR3 is 2**(L) * Piby2_2

	mov.l		ENDFLAG(%a6),%d0

#--We are now ready to perform (R+r) - N*P1 - N*P2, P1 = 2**(L) * Piby2_1 and
#--P2 = 2**(L) * Piby2_2
	fmov.x		%fp2,%fp4
	fmul.x		FP_SCR2(%a6),%fp4	# W = N*P1
	fmov.x		%fp2,%fp5
	fmul.x		FP_SCR3(%a6),%fp5	# w = N*P2
	fmov.x		%fp4,%fp3
#--we want P+p = W+w  but  |p| <= half ulp of P
#--Then, we need to compute  A := R-P   and  a := r-p
	fadd.x		%fp5,%fp3	# FP3 is P
	fsub.x		%fp3,%fp4	# W-P

	fsub.x		%fp3,%fp0	# FP0 is A := R - P
	fadd.x		%fp5,%fp4	# FP4 is p = (W-P)+w

	fmov.x		%fp0,%fp3	# FP3 A
	fsub.x		%fp4,%fp1	# FP1 is a := r - p

#--Now we need to normalize (A,a) to  "new (R,r)" where R+r = A+a but
#--|r| <= half ulp of R.
	fadd.x		%fp1,%fp0	# FP0 is R := A+a
#--No need to calculate r if this is the last loop
	cmp.l		%d0,&0
	bgt.w		RESTORE

#--Need to calculate r
	fsub.x		%fp0,%fp3	# A-R
	fadd.x		%fp3,%fp1	# FP1 is r := (A-R)+a
	bra.w		LOOP

RESTORE:
	fmov.l		%fp2,N(%a6)
	mov.l		(%sp)+,%d2
	fmovm.x		(%sp)+,&0x3c	#  {%fp2-%fp5}

	mov.l		N(%a6),%d0
	ror.l		&1,%d0


	bra.w		TANCONT

#	end		
