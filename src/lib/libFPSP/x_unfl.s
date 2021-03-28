#
#	x_unfl.sa 3.4 7/1/91
#
#	fpsp_unfl --- FPSP handler for underflow exception
#
# Trap disabled results
#	For 881/2 compatibility, sw must denormalize the intermediate 
# result, then store the result.  Denormalization is accomplished 
# by taking the intermediate result (which is always normalized) and 
# shifting the mantissa right while incrementing the exponent until 
# it is equal to the denormalized exponent for the destination 
# format.  After denormalizatoin, the result is rounded to the 
# destination format.
#		
# Trap enabled results
# 	All trap disabled code applies.	In addition the exceptional 
# operand needs to made available to the user with a bias of $6000 
# added to the exponent.
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

#	xref	denorm	
#	xref	round	
#	xref	store	
#	xref	g_rndpr	
#	xref	g_opcls	
#	xref	g_dfmtou	
#	xref	real_unfl	
#	xref	real_inex	
#	xref	fpsp_done	
#	xref	b1238_fix	

	global		fpsp_unfl
fpsp_unfl:
	link		%a6,&-LOCAL_SIZE
	fsave		-(%sp)
	movm.l		&0x303,USER_DA(%a6)	#  {%d0-%d1/%a0-%a1}
	fmovm.x		&0xf0,USER_FP0(%a6)	#  {%fp0-%fp3}
	fmovm.l		%control,%status,%iaddr,USER_FPCR(%a6)

#
	bsr.l		unf_res		# denormalize, round & store interm op
#
# If underflow exceptions are not enabled, check for inexact
# exception
#
	btst		&unfl_bit,FPCR_ENABLE(%a6)
	beq.b		ck_inex

	btst		&E3,E_BYTE(%a6)
	beq.b		no_e3_1
#
# Clear dirty bit on dest resister in the frame before branching
# to b1238_fix.
#
	bfextu		CMDREG3B(%a6){&6:&3},%d0	# get dest reg no
	bclr		%d0,FPR_DIRTY_BITS(%a6)	# clr dest dirty bit
	bsr.l		b1238_fix	# test for bug1238 case
	mov.l		USER_FPSR(%a6),FPSR_SHADOW(%a6)
	or.l		&sx_mask,E_BYTE(%a6)
no_e3_1:
	movm.l		USER_DA(%a6),&0x303	#  {%d0-%d1/%a0-%a1}
	fmovm.x		USER_FP0(%a6),&0xf0	#  {%fp0-%fp3}
	fmovm.l		USER_FPCR(%a6),%control,%status,%iaddr
	frestore	(%sp)+
	unlk		%a6
	bra.l		real_unfl
#
# It is possible to have either inex2 or inex1 exceptions with the
# unfl.  If the inex enable bit is set in the FPCR, and either
# inex2 or inex1 occured, we must clean up and branch to the
# real inex handler.
#
ck_inex:
	mov.b		FPCR_ENABLE(%a6),%d0
	and.b		FPSR_EXCEPT(%a6),%d0
	and.b		&0x3,%d0
	beq.b		unfl_done

#
# Inexact enabled and reported, and we must take an inexact exception
#	
take_inex:
	btst		&E3,E_BYTE(%a6)
	beq.b		no_e3_2
#
# Clear dirty bit on dest resister in the frame before branching
# to b1238_fix.
#
	bfextu		CMDREG3B(%a6){&6:&3},%d0	# get dest reg no
	bclr		%d0,FPR_DIRTY_BITS(%a6)	# clr dest dirty bit
	bsr.l		b1238_fix	# test for bug1238 case
	mov.l		USER_FPSR(%a6),FPSR_SHADOW(%a6)
	or.l		&sx_mask,E_BYTE(%a6)
no_e3_2:
	mov.b		&INEX_VEC,EXC_VEC+1(%a6)
	movm.l		USER_DA(%a6),&0x303	#  {%d0-%d1/%a0-%a1}
	fmovm.x		USER_FP0(%a6),&0xf0	#  {%fp0-%fp3}
	fmovm.l		USER_FPCR(%a6),%control,%status,%iaddr
	frestore	(%sp)+
	unlk		%a6
	bra.l		real_inex

unfl_done:
	bclr		&E3,E_BYTE(%a6)
	beq.b		e1_set		# if set then branch
#
# Clear dirty bit on dest resister in the frame before branching
# to b1238_fix.
#
	bfextu		CMDREG3B(%a6){&6:&3},%d0	# get dest reg no
	bclr		%d0,FPR_DIRTY_BITS(%a6)	# clr dest dirty bit
	bsr.l		b1238_fix	# test for bug1238 case
	mov.l		USER_FPSR(%a6),FPSR_SHADOW(%a6)
	or.l		&sx_mask,E_BYTE(%a6)
	movm.l		USER_DA(%a6),&0x303	#  {%d0-%d1/%a0-%a1}
	fmovm.x		USER_FP0(%a6),&0xf0	#  {%fp0-%fp3}
	fmovm.l		USER_FPCR(%a6),%control,%status,%iaddr
	frestore	(%sp)+
	unlk		%a6
	bra.l		fpsp_done
e1_set:
	movm.l		USER_DA(%a6),&0x303	#  {%d0-%d1/%a0-%a1}
	fmovm.x		USER_FP0(%a6),&0xf0	#  {%fp0-%fp3}
	fmovm.l		USER_FPCR(%a6),%control,%status,%iaddr
	unlk		%a6
	bra.l		fpsp_done
#
#	unf_res --- underflow result calculation
#
unf_res:
	bsr.l		g_rndpr		# returns RND_PREC in d0 0=ext,
#					;1=sgl, 2=dbl
#					;we need the RND_PREC in the
#					;upper word for round
	mov.w		&0,-(%sp)
	mov.w		%d0,-(%sp)	# copy RND_PREC to stack
#
#
# If the exception bit set is E3, the exceptional operand from the
# fpu is in WBTEMP; else it is in FPTEMP.
#
	btst		&E3,E_BYTE(%a6)
	beq.b		unf_E1
unf_E3:
	lea		WBTEMP(%a6),%a0	# a0 now points to operand
#
# Test for fsgldiv and fsglmul.  If the inst was one of these, then
# force the precision to extended for the denorm routine.  Use
# the user's precision for the round routine.
#
	mov.w		CMDREG3B(%a6),%d1	# check for fsgldiv or fsglmul
	and.w		&0x7f,%d1
	cmp.w		%d1,&0x30	# check for sgldiv
	beq.b		unf_sgl
	cmp.w		%d1,&0x33	# check for sglmul
	bne.b		unf_cont	# if not, use fpcr prec in round
unf_sgl:
	clr.l		%d0
	mov.w		&0x1,(%sp)	# override g_rndpr precision
#					;force single
	bra.b		unf_cont
unf_E1:
	lea		FPTEMP(%a6),%a0	# a0 now points to operand
unf_cont:
	bclr		&sign_bit,LOCAL_EX(%a0)	# clear sign bit
	sne		LOCAL_SGN(%a0)	# store sign

	bsr.l		denorm		# returns denorm, a0 points to it
#
# WARNING:
#				;d0 has guard,round sticky bit
#				;make sure that it is not corrupted
#				;before it reaches the round subroutine
#				;also ensure that a0 isn't corrupted

#
# Set up d1 for round subroutine d1 contains the PREC/MODE
# information respectively on upper/lower register halves.
#
	bfextu		FPCR_MODE(%a6){&2:&2},%d1	# get mode from FPCR
#						;mode in lower d1
	add.l		(%sp)+,%d1	# merge PREC/MODE
#
# WARNING: a0 and d0 are assumed to be intact between the denorm and
# round subroutines. All code between these two subroutines
# must not corrupt a0 and d0.
#
#
# Perform Round	
#	Input:		a0 points to input operand
#			d0{31:29} has guard, round, sticky
#			d1{01:00} has rounding mode
#			d1{17:16} has rounding precision
#	Output:		a0 points to rounded operand
#

	bsr.l		round		# returns rounded denorm at (a0)
#
# Differentiate between store to memory vs. store to register
#
unf_store:
	bsr.l		g_opcls		# returns opclass in d0{2:0}
	cmp.b		%d0,&0x3
	bne.b		not_opc011
#
# At this point, a store to memory is pending
#
opc011:
	bsr.l		g_dfmtou
	tst.b		%d0
	beq.b		ext_opc011	# If extended, do not subtract
# 				;If destination format is sgl/dbl, 
	tst.b		LOCAL_HI(%a0)	# If rounded result is normal,don't
#					;subtract
	bmi.b		ext_opc011
	subq.w		&1,LOCAL_EX(%a0)	# account for denorm bias vs.
#				;normalized bias
#				;          normalized   denormalized
#				;single       $7f           $7e
#				;double       $3ff          $3fe
#
ext_opc011:
	bsr.l		store		# stores to memory
	bra.b		unf_done	# finish up

#
# At this point, a store to a float register is pending
#
not_opc011:
	bsr.l		store		# stores to float register
#				;a0 is not corrupted on a store to a
#				;float register.
#
# Set the condition codes according to result
#
	tst.l		LOCAL_HI(%a0)	# check upper mantissa
	bne.b		ck_sgn
	tst.l		LOCAL_LO(%a0)	# check lower mantissa
	bne.b		ck_sgn
	bset		&z_bit,FPSR_CC(%a6)	# set condition codes if zero
ck_sgn:
	btst		&sign_bit,LOCAL_EX(%a0)	# check the sign bit
	beq.b		unf_done
	bset		&neg_bit,FPSR_CC(%a6)

#
# Finish.  
#
unf_done:
	btst		&inex2_bit,FPSR_EXCEPT(%a6)
	beq.b		no_aunfl
	bset		&aunfl_bit,FPSR_AEXCEPT(%a6)
no_aunfl:
	rts

#	end		
