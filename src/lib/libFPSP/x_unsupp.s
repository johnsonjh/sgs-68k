#
#	x_unsupp.sa 3.3 7/1/91
#
#	fpsp_unsupp --- FPSP handler for unsupported data type exception
#
# Trap vector #55	(See table 8-1 Mc68030 User's manual).	
# Invoked when the user program encounters a data format (packed) that
# hardware does not support or a data type (denormalized numbers or un-
# normalized numbers).
# Normalizes denorms and unnorms, unpacks packed numbers then stores 
# them back into the machine to let the 040 finish the operation.  
#
# Unsupp calls two routines:
# 	1. get_op -  gets the operand(s)
# 	2. res_func - restore the function back into the 040 or
# 			if fmove.p fpm,<ea> then pack source (fpm)
# 			and store in users memory <ea>.
#
#  Input: Long fsave stack frame
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
#	xref	res_func	
#	xref	gen_except	
#	xref	fpsp_fmt_error	

	global		fpsp_unsupp
fpsp_unsupp:
#
	link		%a6,&-LOCAL_SIZE
	fsave		-(%sp)
	movm.l		&0x303,USER_DA(%a6)	#  {%d0-%d1/%a0-%a1}
	fmovm.x		&0xf0,USER_FP0(%a6)	#  {%fp0-%fp3}
	fmovm.l		%control,%status,%iaddr,USER_FPCR(%a6)


	mov.b		(%sp),VER_TMP(%a6)	# save version number
	mov.b		(%sp),%d0	# test for valid version num
	and.b		&0xf0,%d0	# test for $4x
	cmp.b		%d0,&VER_4	# must be $4x or exit
	bne.l		fpsp_fmt_error

	fmov.l		&0,%status	# clear all user status bits
	fmov.l		&0,%control	# clear all user control bits
#
#	The following lines are used to ensure that the FPSR
#	exception byte and condition codes are clear before proceeding,
#	except in the case of fmove, which leaves the cc's intact.
#
unsupp_con:
	mov.l		USER_FPSR(%a6),%d1
	btst		&5,CMDREG1B(%a6)	# looking for fmove out
	bne		fmove_con
	and.l		&0xFF00FF,%d1	# clear all but aexcs and qbyte
	bra.b		end_fix
fmove_con:
	and.l		&0x0FFF40FF,%d1	# clear all but cc's, snan bit, aexcs, and qbyte
end_fix:
	mov.l		%d1,USER_FPSR(%a6)

	st		UFLG_TMP(%a6)	# set flag for unsupp data

	bsr.l		get_op		# everything okay, go get operand(s)
	bsr.l		res_func	# fix up stack frame so can restore it
	clr.l		-(%sp)
	mov.b		VER_TMP(%a6),(%sp)	# move idle fmt word to top of stack
	bra.l		gen_except
#
#	end		
