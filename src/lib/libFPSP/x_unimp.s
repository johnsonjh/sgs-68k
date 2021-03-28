#
#	x_unimp.sa 3.3 7/1/91
#
#	fpsp_unimp --- FPSP handler for unimplemented instruction	
#	exception.
#
# Invoked when the user program encounters a floating-point
# op-code that hardware does not support.  Trap vector# 11
# (See table 8-1 MC68030 User's Manual).
#
# 
# Note: An fsave for an unimplemented inst. will create a short
# fsave stack.
#
#  Input: 1. Six word stack frame for unimplemented inst, four word
#            for illegal
#            (See table 8-7 MC68030 User's Manual).
#         2. Unimp (short) fsave state frame created here by fsave
#            instruction.
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

#	xref	get_op	
#	xref	do_func	
#	xref	sto_res	
#	xref	gen_except	
#	xref	fpsp_fmt_error	

	global		fpsp_unimp
	global		uni_2
fpsp_unimp:
	link		%a6,&-LOCAL_SIZE
	fsave		-(%sp)
uni_2:
	movm.l		&0x303,USER_DA(%a6)	#  {%d0-%d1/%a0-%a1}
	fmovm.x		&0xf0,USER_FP0(%a6)	#  {%fp0-%fp3}
	fmovm.l		%control,%status,%iaddr,USER_FPCR(%a6)
	mov.b		(%sp),%d0	# test for valid version num
	and.b		&0xf0,%d0	# test for $4x
	cmp.b		%d0,&VER_4	# must be $4x or exit
	bne.l		fpsp_fmt_error
#
#	Temporary D25B Fix
#	The following lines are used to ensure that the FPSR
#	exception byte and condition codes are clear before proceeding
#
	mov.l		USER_FPSR(%a6),%d0
	and.l		&0xFF,%d0	# clear all but accrued exceptions
	mov.l		%d0,USER_FPSR(%a6)
	fmov.l		&0,%status	# clear all user bits
	fmov.l		&0,%control	# clear all user exceptions for FPSP

	clr.b		UFLG_TMP(%a6)	# clr flag for unsupp data

	bsr.l		get_op		# go get operand(s)
	clr.b		STORE_FLG(%a6)
	bsr.l		do_func		# do the function
	fsave		-(%sp)		# capture possible exc state
	tst.b		STORE_FLG(%a6)
	bne.b		no_store	# if STORE_FLG is set, no store
	bsr.l		sto_res		# store the result in user space
no_store:
	bra.l		gen_except	# post any exceptions and return

#	end		
