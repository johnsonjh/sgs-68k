#
#	ssin.sa 3.3 7/29/91
#
#	The entry point sSIN computes the sine of an input argument
#	sCOS computes the cosine, and sSINCOS computes both. The
#	corresponding entry points with a "d" computes the same
#	corresponding function values for denormalized inputs.
#
#	Input: Double-extended number X in location pointed to
#		by address register a0.
#
#	Output: The funtion value sin(X) or cos(X) returned in Fp0 if SIN or
#		COS is requested. Otherwise, for SINCOS, sin(X) is returned
#		in Fp0, and cos(X) is returned in Fp1.
#
#	Modifies: Fp0 for SIN or COS; both Fp0 and Fp1 for SINCOS.
#
#	Accuracy and Monotonicity: The returned result is within 1 ulp in
#		64 significant bit, i.e. within 0.5001 ulp to 53 bits if the
#		result is subsequently rounded to double precision. The
#		result is provably monotonic in double precision.
#
#	Speed: The programs sSIN and sCOS take approximately 150 cycles for
#		input argument X such that |X| < 15Pi, which is the the usual
#		situation. The speed for sSINCOS is approximately 190 cycles.
#
#	Algorithm:
#
#	SIN and COS:
#	1. If SIN is invoked, set AdjN := 0; otherwise, set AdjN := 1.
#
#	2. If |X| >= 15Pi or |X| < 2**(-40), go to 7.
#
#	3. Decompose X as X = N(Pi/2) + r where |r| <= Pi/4. Let
#		k = N mod 4, so in particular, k = 0,1,2,or 3. Overwirte
#		k by k := k + AdjN.
#
#	4. If k is even, go to 6.
#
#	5. (k is odd) Set j := (k-1)/2, sgn := (-1)**j. Return sgn*cos(r)
#		where cos(r) is approximated by an even polynomial in r,
#		1 + r*r*(B1+s*(B2+ ... + s*B8)),	s = r*r.
#		Exit.
#
#	6. (k is even) Set j := k/2, sgn := (-1)**j. Return sgn*sin(r)
#		where sin(r) is approximated by an odd polynomial in r
#		r + r*s*(A1+s*(A2+ ... + s*A7)),	s = r*r.
#		Exit.
#
#	7. If |X| > 1, go to 9.
#
#	8. (|X|<2**(-40)) If SIN is invoked, return X; otherwise return 1.
#
#	9. Overwrite X by X := X rem 2Pi. Now that |X| <= Pi, go back to 3.
#
#	SINCOS:
#	1. If |X| >= 15Pi or |X| < 2**(-40), go to 6.
#
#	2. Decompose X as X = N(Pi/2) + r where |r| <= Pi/4. Let
#		k = N mod 4, so in particular, k = 0,1,2,or 3.
#
#	3. If k is even, go to 5.
#
#	4. (k is odd) Set j1 := (k-1)/2, j2 := j1 (EOR) (k mod 2), i.e.
#		j1 exclusive or with the l.s.b. of k.
#		sgn1 := (-1)**j1, sgn2 := (-1)**j2.
#		SIN(X) = sgn1 * cos(r) and COS(X) = sgn2*sin(r) where
#		sin(r) and cos(r) are computed as odd and even polynomials
#		in r, respectively. Exit
#
#	5. (k is even) Set j1 := k/2, sgn1 := (-1)**j1.
#		SIN(X) = sgn1 * sin(r) and COS(X) = sgn1*cos(r) where
#		sin(r) and cos(r) are computed as odd and even polynomials
#		in r, respectively. Exit
#
#	6. If |X| > 1, go to 8.
#
#	7. (|X|<2**(-40)) SIN(X) = X and COS(X) = 1. Exit.
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

#	include	l_fpsp.h	

BOUNDS1:
	long		0x3FD78000,0x4004BC7E
TWOBYPI:
	long		0x3FE45F30,0x6DC9C883

SINA7:
	long		0xBD6AAA77,0xCCC994F5
SINA6:
	long		0x3DE61209,0x7AAE8DA1

SINA5:
	long		0xBE5AE645,0x2A118AE4
SINA4:
	long		0x3EC71DE3,0xA5341531

SINA3:
	long		0xBF2A01A0,0x1A018B59,0x00000000,0x00000000

SINA2:
	long		0x3FF80000,0x88888888,0x888859AF,0x00000000

SINA1:
	long		0xBFFC0000,0xAAAAAAAA,0xAAAAAA99,0x00000000

COSB8:
	long		0x3D2AC4D0,0xD6011EE3
COSB7:
	long		0xBDA9396F,0x9F45AC19

COSB6:
	long		0x3E21EED9,0x0612C972
COSB5:
	long		0xBE927E4F,0xB79D9FCF

COSB4:
	long		0x3EFA01A0,0x1A01D423,0x00000000,0x00000000

COSB3:
	long		0xBFF50000,0xB60B60B6,0x0B61D438,0x00000000

COSB2:
	long		0x3FFA0000,0xAAAAAAAA,0xAAAAAB5E
COSB1:
	long		0xBF000000

INVTWOPI:
	long		0x3FFC0000,0xA2F9836E,0x4E44152A

TWOPI1:
	long		0x40010000,0xC90FDAA2,0x00000000,0x00000000
TWOPI2:
	long		0x3FDF0000,0x85A308D4,0x00000000,0x00000000

#	xref	PITBL	

	set		INARG,FP_SCR4

	set		X,FP_SCR5
	set		XDCARE,X+2
	set		XFRAC,X+4

	set		RPRIME,FP_SCR1
	set		SPRIME,FP_SCR2

	set		POSNEG1,L_SCR1
	set		TWOTO63,L_SCR1

	set		ENDFLAG,L_SCR2
	set		N,L_SCR2

	set		ADJN,L_SCR3

#	xref	t_frcinx	
#	xref	t_extdnrm	
#	xref	sto_cos	

	global		ssind
ssind:
#--SIN(X) = X FOR DENORMALIZED X
	bra		t_extdnrm

	global		scosd
scosd:
#--COS(X) = 1 FOR DENORMALIZED X

	fmov.s		&0x3F800000,%fp0
#
#	9D25B Fix: Sometimes the previous fmove.s sets fpsr bits
#
	fmov.l		&0,%status
#
	bra		t_frcinx

	global		ssin
ssin:
#--SET ADJN TO 0
	mov.l		&0,ADJN(%a6)
	bra.b		SINBGN

	global		scos
scos:
#--SET ADJN TO 1
	mov.l		&1,ADJN(%a6)

SINBGN:
#--SAVE FPCR, FP1. CHECK IF |X| IS TOO SMALL OR LARGE

	fmov.x		(%a0),%fp0	# LOAD INPUT

	mov.l		(%a0),%d0
	mov.w		4(%a0),%d0
	fmov.x		%fp0,X(%a6)
	and.l		&0x7FFFFFFF,%d0	# COMPACTIFY X

	cmp.l		%d0,&0x3FD78000	# |X| >= 2**(-40)?
	bge.b		SOK1
	bra.w		SINSM

SOK1:
	cmp.l		%d0,&0x4004BC7E	# |X| < 15 PI?
	blt.b		SINMAIN
	bra.w		REDUCEX

SINMAIN:
#--THIS IS THE USUAL CASE, |X| <= 15 PI.
#--THE ARGUMENT REDUCTION IS DONE BY TABLE LOOK UP.
	fmov.x		%fp0,%fp1
	fmul.d		TWOBYPI,%fp1	# X*2/PI

#--HIDE THE NEXT THREE INSTRUCTIONS
	lea		PITBL+0x200,%a1	# TABLE OF N*PI/2, N = -32,...,32

#--FP1 IS NOW READY
	fmov.l		%fp1,N(%a6)	# CONVERT TO INTEGER

	mov.l		N(%a6),%d0
	asl.l		&4,%d0
	add.l		%d0,%a1		# A1 IS THE ADDRESS OF N*PIBY2
#				...WHICH IS IN TWO PIECES Y1 & Y2

	fsub.x		(%a1)+,%fp0	# X-Y1
#--HIDE THE NEXT ONE
	fsub.s		(%a1),%fp0	# FP0 IS R = (X-Y1)-Y2

SINCONT:
#--continuation from REDUCEX

#--GET N+ADJN AND SEE IF SIN(R) OR COS(R) IS NEEDED
	mov.l		N(%a6),%d0
	add.l		ADJN(%a6),%d0	# SEE IF D0 IS ODD OR EVEN
	ror.l		&1,%d0		# D0 WAS ODD IFF D0 IS NEGATIVE
	cmp.l		%d0,&0
	blt.w		COSPOLY

SINPOLY:
#--LET J BE THE LEAST SIG. BIT OF D0, LET SGN := (-1)**J.
#--THEN WE RETURN	SGN*SIN(R). SGN*SIN(R) IS COMPUTED BY
#--R' + R'*S*(A1 + S(A2 + S(A3 + S(A4 + ... + SA7)))), WHERE
#--R' = SGN*R, S=R*R. THIS CAN BE REWRITTEN AS
#--R' + R'*S*( [A1+T(A3+T(A5+TA7))] + [S(A2+T(A4+TA6))])
#--WHERE T=S*S.
#--NOTE THAT A3 THROUGH A7 ARE STORED IN DOUBLE PRECISION
#--WHILE A1 AND A2 ARE IN DOUBLE-EXTENDED FORMAT.
	fmov.x		%fp0,X(%a6)	# X IS R
	fmul.x		%fp0,%fp0	# FP0 IS S
#---HIDE THE NEXT TWO WHILE WAITING FOR FP0
	fmov.d		SINA7,%fp3
	fmov.d		SINA6,%fp2
#--FP0 IS NOW READY
	fmov.x		%fp0,%fp1
	fmul.x		%fp1,%fp1	# FP1 IS T
#--HIDE THE NEXT TWO WHILE WAITING FOR FP1

	ror.l		&1,%d0
	and.l		&0x80000000,%d0
#				...LEAST SIG. BIT OF D0 IN SIGN POSITION
	eor.l		%d0,X(%a6)	# X IS NOW R'= SGN*R

	fmul.x		%fp1,%fp3	# TA7
	fmul.x		%fp1,%fp2	# TA6

	fadd.d		SINA5,%fp3	# A5+TA7
	fadd.d		SINA4,%fp2	# A4+TA6

	fmul.x		%fp1,%fp3	# T(A5+TA7)
	fmul.x		%fp1,%fp2	# T(A4+TA6)

	fadd.d		SINA3,%fp3	# A3+T(A5+TA7)
	fadd.x		SINA2,%fp2	# A2+T(A4+TA6)

	fmul.x		%fp3,%fp1	# T(A3+T(A5+TA7))

	fmul.x		%fp0,%fp2	# S(A2+T(A4+TA6))
	fadd.x		SINA1,%fp1	# A1+T(A3+T(A5+TA7))
	fmul.x		X(%a6),%fp0	# R'*S

	fadd.x		%fp2,%fp1	# [A1+T(A3+T(A5+TA7))]+[S(A2+T(A4+TA6))]
#--FP3 RELEASED, RESTORE NOW AND TAKE SOME ADVANTAGE OF HIDING
#--FP2 RELEASED, RESTORE NOW AND TAKE FULL ADVANTAGE OF HIDING

	fmul.x		%fp1,%fp0	# SIN(R')-R'
#--FP1 RELEASED.

	fmov.l		%d1,%control	# restore users exceptions
	fadd.x		X(%a6),%fp0	# last inst - possible exception set
	bra		t_frcinx


COSPOLY:
#--LET J BE THE LEAST SIG. BIT OF D0, LET SGN := (-1)**J.
#--THEN WE RETURN	SGN*COS(R). SGN*COS(R) IS COMPUTED BY
#--SGN + S'*(B1 + S(B2 + S(B3 + S(B4 + ... + SB8)))), WHERE
#--S=R*R AND S'=SGN*S. THIS CAN BE REWRITTEN AS
#--SGN + S'*([B1+T(B3+T(B5+TB7))] + [S(B2+T(B4+T(B6+TB8)))])
#--WHERE T=S*S.
#--NOTE THAT B4 THROUGH B8 ARE STORED IN DOUBLE PRECISION
#--WHILE B2 AND B3 ARE IN DOUBLE-EXTENDED FORMAT, B1 IS -1/2
#--AND IS THEREFORE STORED AS SINGLE PRECISION.

	fmul.x		%fp0,%fp0	# FP0 IS S
#---HIDE THE NEXT TWO WHILE WAITING FOR FP0
	fmov.d		COSB8,%fp2
	fmov.d		COSB7,%fp3
#--FP0 IS NOW READY
	fmov.x		%fp0,%fp1
	fmul.x		%fp1,%fp1	# FP1 IS T
#--HIDE THE NEXT TWO WHILE WAITING FOR FP1
	fmov.x		%fp0,X(%a6)	# X IS S
	ror.l		&1,%d0
	and.l		&0x80000000,%d0
#			...LEAST SIG. BIT OF D0 IN SIGN POSITION

	fmul.x		%fp1,%fp2	# TB8
#--HIDE THE NEXT TWO WHILE WAITING FOR THE XU
	eor.l		%d0,X(%a6)	# X IS NOW S'= SGN*S
	and.l		&0x80000000,%d0

	fmul.x		%fp1,%fp3	# TB7
#--HIDE THE NEXT TWO WHILE WAITING FOR THE XU
	or.l		&0x3F800000,%d0	# D0 IS SGN IN SINGLE
	mov.l		%d0,POSNEG1(%a6)

	fadd.d		COSB6,%fp2	# B6+TB8
	fadd.d		COSB5,%fp3	# B5+TB7

	fmul.x		%fp1,%fp2	# T(B6+TB8)
	fmul.x		%fp1,%fp3	# T(B5+TB7)

	fadd.d		COSB4,%fp2	# B4+T(B6+TB8)
	fadd.x		COSB3,%fp3	# B3+T(B5+TB7)

	fmul.x		%fp1,%fp2	# T(B4+T(B6+TB8))
	fmul.x		%fp3,%fp1	# T(B3+T(B5+TB7))

	fadd.x		COSB2,%fp2	# B2+T(B4+T(B6+TB8))
	fadd.s		COSB1,%fp1	# B1+T(B3+T(B5+TB7))

	fmul.x		%fp2,%fp0	# S(B2+T(B4+T(B6+TB8)))
#--FP3 RELEASED, RESTORE NOW AND TAKE SOME ADVANTAGE OF HIDING
#--FP2 RELEASED.

	fadd.x		%fp1,%fp0
#--FP1 RELEASED

	fmul.x		X(%a6),%fp0

	fmov.l		%d1,%control	# restore users exceptions
	fadd.s		POSNEG1(%a6),%fp0	# last inst - possible exception set
	bra		t_frcinx


SINBORS:
#--IF |X| > 15PI, WE USE THE GENERAL ARGUMENT REDUCTION.
#--IF |X| < 2**(-40), RETURN X OR 1.
	cmp.l		%d0,&0x3FFF8000
	bgt.b		REDUCEX

SINSM:
	mov.l		ADJN(%a6),%d0
	cmp.l		%d0,&0
	bgt.b		COSTINY

SINTINY:
	mov.w		&0x0000,XDCARE(%a6)	# JUST IN CASE
	fmov.l		%d1,%control	# restore users exceptions
	fmov.x		X(%a6),%fp0	# last inst - possible exception set
	bra		t_frcinx


COSTINY:
	fmov.s		&0x3F800000,%fp0

	fmov.l		%d1,%control	# restore users exceptions
	fsub.s		&0x00800000,%fp0	# last inst - possible exception set
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

	mov.l		ADJN(%a6),%d0
	cmp.l		%d0,&4

	blt.w		SINCONT
	bra.b		SCCONT

	global		ssincosd
ssincosd:
#--SIN AND COS OF X FOR DENORMALIZED X

	fmov.s		&0x3F800000,%fp1
	bsr		sto_cos		# store cosine result
	bra		t_extdnrm

	global		ssincos
ssincos:
#--SET ADJN TO 4
	mov.l		&4,ADJN(%a6)

	fmov.x		(%a0),%fp0	# LOAD INPUT

	mov.l		(%a0),%d0
	mov.w		4(%a0),%d0
	fmov.x		%fp0,X(%a6)
	and.l		&0x7FFFFFFF,%d0	# COMPACTIFY X

	cmp.l		%d0,&0x3FD78000	# |X| >= 2**(-40)?
	bge.b		SCOK1
	bra.w		SCSM

SCOK1:
	cmp.l		%d0,&0x4004BC7E	# |X| < 15 PI?
	blt.b		SCMAIN
	bra.w		REDUCEX


SCMAIN:
#--THIS IS THE USUAL CASE, |X| <= 15 PI.
#--THE ARGUMENT REDUCTION IS DONE BY TABLE LOOK UP.
	fmov.x		%fp0,%fp1
	fmul.d		TWOBYPI,%fp1	# X*2/PI

#--HIDE THE NEXT THREE INSTRUCTIONS
	lea		PITBL+0x200,%a1	# TABLE OF N*PI/2, N = -32,...,32

#--FP1 IS NOW READY
	fmov.l		%fp1,N(%a6)	# CONVERT TO INTEGER

	mov.l		N(%a6),%d0
	asl.l		&4,%d0
	add.l		%d0,%a1		# ADDRESS OF N*PIBY2, IN Y1, Y2

	fsub.x		(%a1)+,%fp0	# X-Y1
	fsub.s		(%a1),%fp0	# FP0 IS R = (X-Y1)-Y2

SCCONT:
#--continuation point from REDUCEX

#--HIDE THE NEXT TWO
	mov.l		N(%a6),%d0
	ror.l		&1,%d0
	cmp.l		%d0,&0		# D0 < 0 IFF N IS ODD
	bge.w		NEVEN

NODD:
#--REGISTERS SAVED SO FAR: D0, A0, FP2.

	fmov.x		%fp0,RPRIME(%a6)
	fmul.x		%fp0,%fp0	# FP0 IS S = R*R
	fmov.d		SINA7,%fp1	# A7
	fmov.d		COSB8,%fp2	# B8
	fmul.x		%fp0,%fp1	# SA7
	mov.l		%d2,-(%sp)
	mov.l		%d0,%d2
	fmul.x		%fp0,%fp2	# SB8
	ror.l		&1,%d2
	and.l		&0x80000000,%d2

	fadd.d		SINA6,%fp1	# A6+SA7
	eor.l		%d0,%d2
	and.l		&0x80000000,%d2
	fadd.d		COSB7,%fp2	# B7+SB8

	fmul.x		%fp0,%fp1	# S(A6+SA7)
	eor.l		%d2,RPRIME(%a6)
	mov.l		(%sp)+,%d2
	fmul.x		%fp0,%fp2	# S(B7+SB8)
	ror.l		&1,%d0
	and.l		&0x80000000,%d0

	fadd.d		SINA5,%fp1	# A5+S(A6+SA7)
	mov.l		&0x3F800000,POSNEG1(%a6)
	eor.l		%d0,POSNEG1(%a6)
	fadd.d		COSB6,%fp2	# B6+S(B7+SB8)

	fmul.x		%fp0,%fp1	# S(A5+S(A6+SA7))
	fmul.x		%fp0,%fp2	# S(B6+S(B7+SB8))
	fmov.x		%fp0,SPRIME(%a6)

	fadd.d		SINA4,%fp1	# A4+S(A5+S(A6+SA7))
	eor.l		%d0,SPRIME(%a6)
	fadd.d		COSB5,%fp2	# B5+S(B6+S(B7+SB8))

	fmul.x		%fp0,%fp1	# S(A4+...)
	fmul.x		%fp0,%fp2	# S(B5+...)

	fadd.d		SINA3,%fp1	# A3+S(A4+...)
	fadd.d		COSB4,%fp2	# B4+S(B5+...)

	fmul.x		%fp0,%fp1	# S(A3+...)
	fmul.x		%fp0,%fp2	# S(B4+...)

	fadd.x		SINA2,%fp1	# A2+S(A3+...)
	fadd.x		COSB3,%fp2	# B3+S(B4+...)

	fmul.x		%fp0,%fp1	# S(A2+...)
	fmul.x		%fp0,%fp2	# S(B3+...)

	fadd.x		SINA1,%fp1	# A1+S(A2+...)
	fadd.x		COSB2,%fp2	# B2+S(B3+...)

	fmul.x		%fp0,%fp1	# S(A1+...)
	fmul.x		%fp2,%fp0	# S(B2+...)


	fmul.x		RPRIME(%a6),%fp1	# R'S(A1+...)
	fadd.s		COSB1,%fp0	# B1+S(B2...)
	fmul.x		SPRIME(%a6),%fp0	# S'(B1+S(B2+...))

	mov.l		%d1,-(%sp)	# restore users mode & precision
	and.l		&0xff,%d1	# mask off all exceptions
	fmov.l		%d1,%control
	fadd.x		RPRIME(%a6),%fp1	# COS(X)
	bsr		sto_cos		# store cosine result
	fmov.l		(%sp)+,%control	# restore users exceptions
	fadd.s		POSNEG1(%a6),%fp0	# SIN(X)

	bra		t_frcinx


NEVEN:
#--REGISTERS SAVED SO FAR: FP2.

	fmov.x		%fp0,RPRIME(%a6)
	fmul.x		%fp0,%fp0	# FP0 IS S = R*R
	fmov.d		COSB8,%fp1	# B8
	fmov.d		SINA7,%fp2	# A7
	fmul.x		%fp0,%fp1	# SB8
	fmov.x		%fp0,SPRIME(%a6)
	fmul.x		%fp0,%fp2	# SA7
	ror.l		&1,%d0
	and.l		&0x80000000,%d0
	fadd.d		COSB7,%fp1	# B7+SB8
	fadd.d		SINA6,%fp2	# A6+SA7
	eor.l		%d0,RPRIME(%a6)
	eor.l		%d0,SPRIME(%a6)
	fmul.x		%fp0,%fp1	# S(B7+SB8)
	or.l		&0x3F800000,%d0
	mov.l		%d0,POSNEG1(%a6)
	fmul.x		%fp0,%fp2	# S(A6+SA7)

	fadd.d		COSB6,%fp1	# B6+S(B7+SB8)
	fadd.d		SINA5,%fp2	# A5+S(A6+SA7)

	fmul.x		%fp0,%fp1	# S(B6+S(B7+SB8))
	fmul.x		%fp0,%fp2	# S(A5+S(A6+SA7))

	fadd.d		COSB5,%fp1	# B5+S(B6+S(B7+SB8))
	fadd.d		SINA4,%fp2	# A4+S(A5+S(A6+SA7))

	fmul.x		%fp0,%fp1	# S(B5+...)
	fmul.x		%fp0,%fp2	# S(A4+...)

	fadd.d		COSB4,%fp1	# B4+S(B5+...)
	fadd.d		SINA3,%fp2	# A3+S(A4+...)

	fmul.x		%fp0,%fp1	# S(B4+...)
	fmul.x		%fp0,%fp2	# S(A3+...)

	fadd.x		COSB3,%fp1	# B3+S(B4+...)
	fadd.x		SINA2,%fp2	# A2+S(A3+...)

	fmul.x		%fp0,%fp1	# S(B3+...)
	fmul.x		%fp0,%fp2	# S(A2+...)

	fadd.x		COSB2,%fp1	# B2+S(B3+...)
	fadd.x		SINA1,%fp2	# A1+S(A2+...)

	fmul.x		%fp0,%fp1	# S(B2+...)
	fmul.x		%fp2,%fp0	# s(a1+...)


	fadd.s		COSB1,%fp1	# B1+S(B2...)
	fmul.x		RPRIME(%a6),%fp0	# R'S(A1+...)
	fmul.x		SPRIME(%a6),%fp1	# S'(B1+S(B2+...))

	mov.l		%d1,-(%sp)	# save users mode & precision
	and.l		&0xff,%d1	# mask off all exceptions
	fmov.l		%d1,%control
	fadd.s		POSNEG1(%a6),%fp1	# COS(X)
	bsr		sto_cos		# store cosine result
	fmov.l		(%sp)+,%control	# restore users exceptions
	fadd.x		RPRIME(%a6),%fp0	# SIN(X)

	bra		t_frcinx

SCBORS:
	cmp.l		%d0,&0x3FFF8000
	bgt.w		REDUCEX

SCSM:
	mov.w		&0x0000,XDCARE(%a6)
	fmov.s		&0x3F800000,%fp1

	mov.l		%d1,-(%sp)	# save users mode & precision
	and.l		&0xff,%d1	# mask off all exceptions
	fmov.l		%d1,%control
	fsub.s		&0x00800000,%fp1
	bsr		sto_cos		# store cosine result
	fmov.l		(%sp)+,%control	# restore users exceptions
	fmov.x		X(%a6),%fp0
	bra		t_frcinx

#	end		
