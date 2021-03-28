#
#	sto_res.sa 3.1 12/10/90
#
#	Takes the result and puts it in where the user expects it.
#	Library functions return result in fp0.	If fp0 is not the
#	users destination register then fp0 is moved to the the
#	correct floating-point destination register.  fp0 and fp1
#	are then restored to the original contents. 
#
#	Input:	result in fp0,fp1 
#
#		d2 & a0 should be kept unmodified
#
#	Output:	moves the result to the true destination reg or mem
#
#	Modifies: destination floating point register
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

	global		sto_cos
sto_cos:
	bfextu		CMDREG1B(%a6){&13:&3},%d0	# extract cos destination
	cmp.b		%d0,&3		# check for fp0/fp1 cases
	ble.b		c_fp0123
	fmovm.x		&0x2,-(%sp)	#  {%fp1}
	mov.l		&7,%d1
	sub.l		%d0,%d1		# d1 = 7- (dest. reg. no.)
	clr.l		%d0
	bset		%d1,%d0		# d0 is dynamic register mask
	fmovm.x		(%sp)+,%d0
	rts
c_fp0123:
	cmp.b		%d0,&0
	beq.b		c_is_fp0
	cmp.b		%d0,&1
	beq.b		c_is_fp1
	cmp.b		%d0,&2
	beq.b		c_is_fp2
c_is_fp3:
	fmovm.x		&0x40,USER_FP3(%a6)	#  {%fp1}
	rts
c_is_fp2:
	fmovm.x		&0x40,USER_FP2(%a6)	#  {%fp1}
	rts
c_is_fp1:
	fmovm.x		&0x40,USER_FP1(%a6)	#  {%fp1}
	rts
c_is_fp0:
	fmovm.x		&0x40,USER_FP0(%a6)	#  {%fp1}
	rts


	global		sto_res
sto_res:
	bfextu		CMDREG1B(%a6){&6:&3},%d0	# extract destination register
	cmp.b		%d0,&3		# check for fp0/fp1 cases
	ble.b		fp0123
	fmovm.x		&0x1,-(%sp)	#  {%fp0}
	mov.l		&7,%d1
	sub.l		%d0,%d1		# d1 = 7- (dest. reg. no.)
	clr.l		%d0
	bset		%d1,%d0		# d0 is dynamic register mask
	fmovm.x		(%sp)+,%d0
	rts
fp0123:
	cmp.b		%d0,&0
	beq.b		is_fp0
	cmp.b		%d0,&1
	beq.b		is_fp1
	cmp.b		%d0,&2
	beq.b		is_fp2
is_fp3:
	fmovm.x		&0x80,USER_FP3(%a6)	#  {%fp0}
	rts
is_fp2:
	fmovm.x		&0x80,USER_FP2(%a6)	#  {%fp0}
	rts
is_fp1:
	fmovm.x		&0x80,USER_FP1(%a6)	#  {%fp0}
	rts
is_fp0:
	fmovm.x		&0x80,USER_FP0(%a6)	#  {%fp0}
	rts

#	end		
