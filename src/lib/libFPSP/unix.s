#
#	unix.s 3.2 4/26/91
#
#	Unix System V version of skeleton.sa

#		Copyright (C) Motorola, Inc. 1990
#			All Rights Reserved
#
#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF MOTOROLA 
#	The copyright notice above does not evidence any  
#	actual or intended publication of such source code.

#	idnt	2,1	Motorola 040 Floating Point Software Package


	text

#	include	fpsp.h	

#	xref	b1238_fix	

#
#	Divide by Zero exception
#
#	All dz exceptions are 'real', hence no fpsp_dz entry point.
#
	global		fpsp_dz
fpsp_dz:
	global		real_dz
real_dz:
	link		%a6,&-LOCAL_SIZE
	fsave		-(%sp)
	bclr		&E1,E_BYTE(%a6)
	frestore	(%sp)+
	unlk		%a6

	jmp		Xtrap
#
#	Inexact exception
#
#	All inexact exceptions are real, but the 'real' handler
#	will probably want to clear the pending exception.
#	The provided code will clear the E3 exception (if pending), 
#	otherwise clear the E1 exception.  The frestore is not really
#	necessary for E1 exceptions.
#
# Code following the 'inex' label is to handle bug #1232.  In this
# bug, if an E1 snan, ovfl, or unfl occured, and the process was
# swapped out before taking the exception, the exception taken on
# return was inex, rather than the correct exception.  The snan, ovfl,
# and unfl exception to be taken must not have been enabled.  The
# fix is to check for E1, and the existence of one of snan, ovfl,
# or unfl bits set in the fpsr.  If any of these are set, branch
# to the appropriate  handler for the exception in the fpsr.  Note
# that this fix is only for d43b parts, and is skipped if the
# version number is not $40.
# 
	global		fpsp_inex
fpsp_inex:
	link		%a6,&-LOCAL_SIZE
	fsave		-(%sp)
	cmp.b		(%sp),&VER_40	# test version number
	bne.b		not_fmt40
	fmov.l		%status,-(%sp)
	btst		&E1,E_BYTE(%a6)	# test for E1 set
	beq.b		not_b1232
	btst		&snan_bit,2(%sp)	# test for snan
	beq		inex_ckofl
	add.l		&4,%sp
	frestore	(%sp)+
	unlk		%a6
	bra		fpsp_snan
inex_ckofl:
	btst		&ovfl_bit,2(%sp)	# test for ovfl
	beq		inex_ckufl
	add.l		&4,%sp
	frestore	(%sp)+
	unlk		%a6
	bra		fpsp_ovfl
inex_ckufl:
	btst		&unfl_bit,2(%sp)	# test for unfl
	beq		not_b1232
	add.l		&4,%sp
	frestore	(%sp)+
	unlk		%a6
	bra		fpsp_unfl

#
# We do not have the bug 1232 case.  Clean up the stack and call
# real_inex.
#
not_b1232:
	add.l		&4,%sp
	frestore	(%sp)+
	unlk		%a6

	global		real_inex
real_inex:

	link		%a6,&-LOCAL_SIZE
	fsave		-(%sp)
not_fmt40:
	bclr		&E3,E_BYTE(%a6)	# clear and test E3 flag
	beq.b		inex_cke1
#
# Clear dirty bit on dest resister in the frame before branching
# to b1238_fix.
#
	movm.l		&0x3,USER_DA(%a6)	#  {%d0/%d1}
	bfextu		CMDREG1B(%a6){&6:&3},%d0	# get dest reg no
	bclr		%d0,FPR_DIRTY_BITS(%a6)	# clr dest dirty bit
	bsr		b1238_fix	# test for bug1238 case
	movm.l		USER_DA(%a6),&0x3	#  {%d0/%d1}
	bra.b		inex_done
inex_cke1:
	bclr		&E1,E_BYTE(%a6)
inex_done:
	frestore	(%sp)+
	unlk		%a6
	jmp		Xtrap
#
#	Overflow exception
#
#	xref	fpsp_ovfl	
	global		real_ovfl
real_ovfl:

	link		%a6,&-LOCAL_SIZE
	fsave		-(%sp)
	bclr		&E3,E_BYTE(%a6)	# clear and test E3 flag
	bne.b		ovfl_done
	bclr		&E1,E_BYTE(%a6)
ovfl_done:
	frestore	(%sp)+
	unlk		%a6
	jmp		Xtrap
#
#	Underflow exception
#
#	xref	fpsp_unfl	
	global		real_unfl
real_unfl:

	link		%a6,&-LOCAL_SIZE
	fsave		-(%sp)
	bclr		&E3,E_BYTE(%a6)	# clear and test E3 flag
	bne.b		unfl_done
	bclr		&E1,E_BYTE(%a6)
unfl_done:
	frestore	(%sp)+
	unlk		%a6
	jmp		Xtrap
#
#	Signalling NAN exception
#
#	xref	fpsp_snan	
	global		real_snan
real_snan:
	link		%a6,&-LOCAL_SIZE
	fsave		-(%sp)
	bclr		&E1,E_BYTE(%a6)	# snan is always an E1 exception
	frestore	(%sp)+
	unlk		%a6

	jmp		Xtrap
#
#	Operand Error exception
#
#	xref	fpsp_operr	
	global		real_operr
real_operr:
	link		%a6,&-LOCAL_SIZE
	fsave		-(%sp)
	bclr		&E1,E_BYTE(%a6)	# operr is always an E1 exception
	frestore	(%sp)+
	unlk		%a6

	jmp		Xtrap
#
#	BSUN exception
#
#	This sample handler simply clears the nan bit in the FPSR.
#
#	xref	fpsp_bsun	
	global		real_bsun
real_bsun:
	link		%a6,&-LOCAL_SIZE
	fsave		-(%sp)
	bclr		&E1,E_BYTE(%a6)	# bsun is always an E1 exception
	fmov.l		%status,-(%sp)
	bclr		&nan_bit,(%sp)
	fmov.l		(%sp)+,%status
	frestore	(%sp)+
	unlk		%a6

	jmp		Xtrap

#
#	F-line exception
#
#	A 'real' F-line exception is one that the FPSP isn't supposed to 
#	handle. E.g. an instruction with a co-processor ID that is not 1.
#
#
#	xref	fpsp_fline	
	global		real_fline
real_fline:

	jmp		Xtrap

#
#	Trace exception
#
	global		real_trace
real_trace:
	jmp		Xtrap

#
#	fpsp_fmt_error --- exit point for frame format error
#
#	The fpu stack frame does not match the frames existing
#	or planned at the time of this writing.  The fpsp is
#	unable to handle frame sizes not in the following
#	version:size pairs:
#
#	{4060, 4160} - busy frame
#	{4028, 4130} - unimp frame
#	{4000, 4100} - idle frame
#
#	This entry point simply holds an f-line illegal value.  
#	Replace this with a call to your kernel panic code or
#	code to handle future revisions of the fpu.
#
	global		fpsp_fmt_error
fpsp_fmt_error:

	long		0xf27f0000	# f-line illegal 
	bra.b		fpsp_fmt_error

#
#	fpsp_done --- FPSP exit point
#
#	The exception has been handled by the package and we are ready
#	to return to user mode, but there may be OS specific code
#	to execute before we do.  If there is, do it now.
#
#
	global		fpsp_done
fpsp_done:
	rte

#
#	mem_write --- write to user or supervisor address space
#
# Writes to memory while in supervisor mode.  copyout accomplishes
# this via a 'moves' instruction.  copyout is a UNIX SVR3 (and later) function.
# If you don't have copyout, use the local copy of the function below.
#
#	a0 - supervisor source address
#	a1 - user destination address
#	d0 - number of bytes to write (maximum count is 12)
#
# The supervisor source address is guaranteed to point into the supervisor
# stack.  The result is that a UNIX
# process is allowed to sleep as a consequence of a page fault during
# copyout.  The probability of a page fault is exceedingly small because
# the 68040 always reads the destination address and thus the page
# faults should have already been handled.
#
# If the EXC_SR shows that the exception was from supervisor space,
# then just do a dumb (and slow) memory move.  In a UNIX environment
# there shouldn't be any supervisor mode floating point exceptions.
#
	global		mem_write
mem_write:
	btst		&5,EXC_SR(%a6)	# check for supervisor state
	beq.b		user_write
super_write:
	mov.b		(%a0)+,(%a1)+
	subq.l		&1,%d0
	bne.b		super_write
	rts
user_write:
	mov.l		%d1,-(%sp)
	mov.l		%d0,-(%sp)
	mov.l		%a1,-(%sp)
	mov.l		%a0,-(%sp)
	jsr		copyout
	add.w		&12,%sp
	mov.l		(%sp)+,%d1
	rts
#
#	mem_read --- read from user or supervisor address space
#
# Reads from memory while in supervisor mode.  copyin accomplishes
# this via a 'moves' instruction.  copyin is a UNIX SVR3 (and later) function.
# If you don't have copyin, use the local copy of the function below.
#
# The FPSP calls mem_read to read the original F-line instruction in order
# to extract the data register number when the 'Dn' addressing mode is
# used.
#
#Input:
#	a0 - user source address
#	a1 - supervisor destination address
#	d0 - number of bytes to read (maximum count is 12)
#
# Like mem_write, mem_read always reads with a supervisor 
# destination address on the supervisor stack.  Also like mem_write,
# the EXC_SR is checked and a simple memory copy is done if reading
# from supervisor space is indicated.
#
	global		mem_read
mem_read:
	btst		&5,EXC_SR(%a6)	# check for supervisor state
	beq.b		user_read
super_read:
	mov.b		(%a0)+,(%a1)+
	subq.l		&1,%d0
	bne.b		super_read
	rts
user_read:
	mov.l		%d1,-(%sp)
	mov.l		%d0,-(%sp)
	mov.l		%a1,-(%sp)
	mov.l		%a0,-(%sp)
	jsr		copyin
	add.w		&12,%sp
	mov.l		(%sp)+,%d1
	rts


#	end		
