#
#	gen_except.sa 3.6 7/31/91
#
#	gen_except --- FPSP routine to detect reportable exceptions
#	
#	This routine compares the exception enable byte of the
#	user_fpcr on the stack with the exception status byte
#	of the user_fpsr. 
#
#	Any routine which may report an exceptions must load
#	the stack frame in memory with the exceptional operand(s).
#
#	Priority for exceptions is:
#
#	Highest:	bsun
#			snan
#			operr
#			ovfl
#			unfl
#			dz
#			inex2
#	Lowest:		inex1
#
#	Note: The IEEE standard specifies that inex2 is to be
#	reported if ovfl occurs and the ovfl enable bit is not
#	set but the inex2 enable bit is.  
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

#	xref	real_trace	
#	xref	fpsp_done	
#	xref	fpsp_fmt_error	

exc_tbl:
	long		bsun_exc
	long		commonE1
	long		commonE1
	long		ovfl_unfl
	long		ovfl_unfl
	long		commonE1
	long		commonE3
	long		commonE3
	long		no_match

	global		gen_except
gen_except:
	cmp.b		1(%sp),&IDLE_SIZE-4	# test for idle frame
	beq.w		do_check	# go handle idle frame
	cmp.b		1(%sp),&UNIMP_40_SIZE-4	# test for orig unimp frame
	beq.b		unimp_x		# go handle unimp frame
	cmp.b		1(%sp),&UNIMP_41_SIZE-4	# test for rev unimp frame
	beq.b		unimp_x		# go handle unimp frame
	cmp.b		1(%sp),&BUSY_SIZE-4	# if size <> $60, fmt error
	bne		fpsp_fmt_error
	lea.l		BUSY_SIZE+LOCAL_SIZE(%sp),%a1	# init a1 so fpsp.h
#					;equates will work
# Fix up the new busy frame with entries from the unimp frame
#
	mov.l		ETEMP_EX(%a6),ETEMP_EX(%a1)	# copy etemp from unimp
	mov.l		ETEMP_HI(%a6),ETEMP_HI(%a1)	# frame to busy frame
	mov.l		ETEMP_LO(%a6),ETEMP_LO(%a1)
	mov.l		CMDREG1B(%a6),CMDREG1B(%a1)	# set inst in frame to unimp
	mov.l		CMDREG1B(%a6),%d0	# fix cmd1b to make it
	and.l		&0x03c30000,%d0	# work for cmd3b
	bfextu		CMDREG1B(%a6){&13:&1},%d1	# extract bit 2
	lsl.l		&5,%d1
	swap		%d1
	or.l		%d1,%d0		# put it in the right place
	bfextu		CMDREG1B(%a6){&10:&3},%d1	# extract bit 3,4,5
	lsl.l		&2,%d1
	swap		%d1
	or.l		%d1,%d0		# put them in the right place
	mov.l		%d0,CMDREG3B(%a1)	# in the busy frame
#
# Or in the FPSR from the emulation with the USER_FPSR on the stack.
#
	fmov.l		%status,%d0
	or.l		%d0,USER_FPSR(%a6)
	mov.l		USER_FPSR(%a6),FPSR_SHADOW(%a1)	# set exc bits
	or.l		&sx_mask,E_BYTE(%a1)
	bra		do_clean

#
# Frame is an unimp frame possible resulting from an fmove <ea>,fp0
# that caused an exception
#
# a1 is modified to point into the new frame allowing fpsp equates
# to be valid.
#
unimp_x:
	cmp.b		1(%sp),&UNIMP_40_SIZE-4	# test for orig unimp frame
	bne.b		test_rev
	lea.l		UNIMP_40_SIZE+LOCAL_SIZE(%sp),%a1
	bra.b		unimp_con
test_rev:
	cmp.b		1(%sp),&UNIMP_41_SIZE-4	# test for rev unimp frame
	bne		fpsp_fmt_error	# if not $28 or $30
	lea.l		UNIMP_41_SIZE+LOCAL_SIZE(%sp),%a1
unimp_con:
#
# Fix up the new unimp frame with entries from the old unimp frame
#
	mov.l		CMDREG1B(%a6),CMDREG1B(%a1)	# set inst in frame to unimp
#
# Or in the FPSR from the emulation with the USER_FPSR on the stack.
#
	fmov.l		%status,%d0
	or.l		%d0,USER_FPSR(%a6)
	bra		do_clean

#
# Frame is idle, so check for exceptions reported through
# USER_FPSR and set the unimp frame accordingly.  
# A7 must be incremented to the point before the
# idle fsave vector to the unimp vector.
#
do_check:
	add.l		&4,%sp		# point A7 back to unimp frame
#
# Or in the FPSR from the emulation with the USER_FPSR on the stack.
#
	fmov.l		%status,%d0
	or.l		%d0,USER_FPSR(%a6)
#
# On a busy frame, we must clear the nmnexc bits.
#
	cmp.b		1(%sp),&BUSY_SIZE-4	# check frame type
	bne.b		check_fr	# if busy, clr nmnexc
	clr.w		NMNEXC(%a6)	# clr nmnexc & nmcexc
	btst		&5,CMDREG1B(%a6)	# test for fmove out
	bne.b		frame_com
	mov.l		USER_FPSR(%a6),FPSR_SHADOW(%a6)	# set exc bits
	or.l		&sx_mask,E_BYTE(%a6)
	bra.b		frame_com
check_fr:
	cmp.b		1(%sp),&UNIMP_40_SIZE-4
	beq.b		frame_com
	clr.w		NMNEXC(%a6)
frame_com:
	mov.b		FPCR_ENABLE(%a6),%d0	# get fpcr enable byte
	and.b		FPSR_EXCEPT(%a6),%d0	# and in the fpsr exc byte
	bfffo		%d0{&24:&8},%d1	# test for first set bit
	lea.l		exc_tbl,%a0	# load jmp table address
	sub.b		&24,%d1		# normalize bit offset to 0-8
	mov.l		(%a0,%d1.w*4),%a0	# load routine address based
#					;based on first enabled exc
	jmp		(%a0)		# jump to routine
#
# Bsun is not possible in unimp or unsupp
#
bsun_exc:
	bra		do_clean
#
# The typical work to be done to the unimp frame to report an 
# exception is to set the E1/E3 byte and clr the U flag.
# commonE1 does this for E1 exceptions, which are snan, 
# operr, and dz.  commonE3 does this for E3 exceptions, which 
# are inex2 and inex1, and also clears the E1 exception bit
# left over from the unimp exception.
#
commonE1:
	bset		&E1,E_BYTE(%a6)	# set E1 flag
	bra.w		commonE		# go clean and exit

commonE3:
	tst.b		UFLG_TMP(%a6)	# test flag for unsup/unimp state
	bne.b		unsE3
uniE3:
	bset		&E3,E_BYTE(%a6)	# set E3 flag
	bclr		&E1,E_BYTE(%a6)	# clr E1 from unimp
	bra.w		commonE

unsE3:
	tst.b		RES_FLG(%a6)
	bne.b		unsE3_0
unsE3_1:
	bset		&E3,E_BYTE(%a6)	# set E3 flag
unsE3_0:
	bclr		&E1,E_BYTE(%a6)	# clr E1 flag
	mov.l		CMDREG1B(%a6),%d0
	and.l		&0x03c30000,%d0	# work for cmd3b
	bfextu		CMDREG1B(%a6){&13:&1},%d1	# extract bit 2
	lsl.l		&5,%d1
	swap		%d1
	or.l		%d1,%d0		# put it in the right place
	bfextu		CMDREG1B(%a6){&10:&3},%d1	# extract bit 3,4,5
	lsl.l		&2,%d1
	swap		%d1
	or.l		%d1,%d0		# put them in the right place
	mov.l		%d0,CMDREG3B(%a6)	# in the busy frame

commonE:
	bclr		&UFLAG,T_BYTE(%a6)	# clr U flag from unimp
	bra.w		do_clean	# go clean and exit
#
# No bits in the enable byte match existing exceptions.  Check for
# the case of the ovfl exc without the ovfl enabled, but with
# inex2 enabled.
#
no_match:
	btst		&inex2_bit,FPCR_ENABLE(%a6)	# check for ovfl/inex2 case
	beq.b		no_exc		# if clear, exit
	btst		&ovfl_bit,FPSR_EXCEPT(%a6)	# now check ovfl
	beq.b		no_exc		# if clear, exit
	bra.b		ovfl_unfl	# go to unfl_ovfl to determine if
#					;it is an unsupp or unimp exc
# No exceptions are to be reported.  If the instruction was 
# unimplemented, no FPU restore is necessary.  If it was
# unsupported, we must perform the restore.
no_exc:
	tst.b		UFLG_TMP(%a6)	# test flag for unsupp/unimp state
	beq.b		uni_no_exc
uns_no_exc:
	tst.b		RES_FLG(%a6)	# check if frestore is needed
	bne.w		do_clean	# if clear, no frestore needed
uni_no_exc:
	movm.l		USER_DA(%a6),&0x303	#  {%d0-%d1/%a0-%a1}
	fmovm.x		USER_FP0(%a6),&0xf0	#  {%fp0-%fp3}
	fmovm.l		USER_FPCR(%a6),%control,%status,%iaddr
	unlk		%a6
	bra		finish_up
#
# Unsupported Data Type Handler:
# Ovfl:
#   An fmoveout that results in an overflow is reported this way.
# Unfl:
#   An fmoveout that results in an underflow is reported this way.
#
# Unimplemented Instruction Handler:
# Ovfl:
#   Only scosh, setox, ssinh, stwotox, and scale can set overflow in 
#   this manner.
# Unfl:
#   Stwotox, setox, and scale can set underflow in this manner.
#   Any of the other Library Routines such that f(x)=x in which
#   x is an extended denorm can report an underflow exception. 
#   It is the responsibility of the exception-causing exception 
#   to make sure that WBTEMP is correct.
#
#   The exceptional operand is in FP_SCR1.
#
ovfl_unfl:
	tst.b		UFLG_TMP(%a6)	# test flag for unsupp/unimp state
	beq.b		ofuf_con
#
# The caller was from an unsupported data type trap.  Test if the
# caller set CU_ONLY.  If so, the exceptional operand is expected in
# FPTEMP, rather than WBTEMP.
#
	tst.b		CU_ONLY(%a6)	# test if inst is cu-only
	beq.w		unsE3
#	move.w	#$fe,CU_SAVEPC(a6)
	clr.b		CU_SAVEPC(%a6)
	bset		&E1,E_BYTE(%a6)	# set E1 exception flag
	mov.w		ETEMP_EX(%a6),FPTEMP_EX(%a6)
	mov.l		ETEMP_HI(%a6),FPTEMP_HI(%a6)
	mov.l		ETEMP_LO(%a6),FPTEMP_LO(%a6)
	bset		&fptemp15_bit,DTAG(%a6)	# set fpte15
	bclr		&UFLAG,T_BYTE(%a6)	# clr U flag from unimp
	bra.w		do_clean	# go clean and exit

ofuf_con:
	mov.b		(%sp),VER_TMP(%a6)	# save version number
	cmp.b		1(%sp),&BUSY_SIZE-4	# check for busy frame
	beq.b		busy_fr		# if unimp, grow to busy
	cmp.b		(%sp),&VER_40	# test for orig unimp frame
	bne.b		try_41		# if not, test for rev frame
	mov.l		&13,%d0		# need to zero 14 lwords
	bra.b		ofuf_fin
try_41:
	cmp.b		(%sp),&VER_41	# test for rev unimp frame
	bne		fpsp_fmt_error	# if neither, exit with error
	mov.l		&11,%d0		# need to zero 12 lwords

ofuf_fin:
	clr.l		(%sp)
loop1:
	clr.l		-(%sp)		# clear and dec a7
	dbra.w		%d0,loop1
	mov.b		VER_TMP(%a6),(%sp)
	mov.b		&BUSY_SIZE-4,1(%sp)	# write busy fmt word.
busy_fr:
	mov.l		FP_SCR1(%a6),WBTEMP_EX(%a6)	# write
	mov.l		FP_SCR1+4(%a6),WBTEMP_HI(%a6)	# execptional op to
	mov.l		FP_SCR1+8(%a6),WBTEMP_LO(%a6)	# wbtemp
	bset		&E3,E_BYTE(%a6)	# set E3 flag
	bclr		&E1,E_BYTE(%a6)	# make sure E1 is clear
	bclr		&UFLAG,T_BYTE(%a6)	# clr U flag
	mov.l		USER_FPSR(%a6),FPSR_SHADOW(%a6)
	or.l		&sx_mask,E_BYTE(%a6)
	mov.l		CMDREG1B(%a6),%d0	# fix cmd1b to make it
	and.l		&0x03c30000,%d0	# work for cmd3b
	bfextu		CMDREG1B(%a6){&13:&1},%d1	# extract bit 2
	lsl.l		&5,%d1
	swap		%d1
	or.l		%d1,%d0		# put it in the right place
	bfextu		CMDREG1B(%a6){&10:&3},%d1	# extract bit 3,4,5
	lsl.l		&2,%d1
	swap		%d1
	or.l		%d1,%d0		# put them in the right place
	mov.l		%d0,CMDREG3B(%a6)	# in the busy frame

#
# Check if the frame to be restored is busy or unimp.
#** NOTE *** Bug fix for errata (0d43b #3)
# If the frame is unimp, we must create a busy frame to 
# fix the bug with the nmnexc bits in cases in which they
# are set by a previous instruction and not cleared by
# the save. The frame will be unimp only if the final 
# instruction in an emulation routine caused the exception
# by doing an fmove <ea>,fp0.  The exception operand, in
# internal format, is in fptemp.
#
do_clean:
	cmp.b		1(%sp),&UNIMP_40_SIZE-4
	bne.b		do_con
	mov.l		&13,%d0		# in orig, need to zero 14 lwords
	bra.b		do_build
do_con:
	cmp.b		1(%sp),&UNIMP_41_SIZE-4
	bne.b		do_restore	# frame must be busy
	mov.l		&11,%d0		# in rev, need to zero 12 lwords

do_build:
	mov.b		(%sp),VER_TMP(%a6)
	clr.l		(%sp)
loop2:
	clr.l		-(%sp)		# clear and dec a7
	dbra.w		%d0,loop2
#
# Use a1 as pointer into new frame.  a6 is not correct if an unimp or
# busy frame was created as the result of an exception on the final
# instruction of an emulation routine.
#
# We need to set the nmcexc bits if the exception is E1. Otherwise,
# the exc taken will be inex2.
#
	lea.l		BUSY_SIZE+LOCAL_SIZE(%sp),%a1	# init a1 for new frame
	mov.b		VER_TMP(%a6),(%sp)	# write busy fmt word
	mov.b		&BUSY_SIZE-4,1(%sp)
	mov.l		FP_SCR1(%a6),WBTEMP_EX(%a1)	# write
	mov.l		FP_SCR1+4(%a6),WBTEMP_HI(%a1)	# exceptional op to
	mov.l		FP_SCR1+8(%a6),WBTEMP_LO(%a1)	# wbtemp
#	btst.b	#E1,E_BYTE(a1)
#	beq.b	do_restore
	bfextu		USER_FPSR(%a6){&17:&4},%d0	# get snan/operr/ovfl/unfl bits
	bfins		%d0,NMCEXC(%a1){&4:&4}	# and insert them in nmcexc
	mov.l		USER_FPSR(%a6),FPSR_SHADOW(%a1)	# set exc bits
	or.l		&sx_mask,E_BYTE(%a1)
do_restore:
	movm.l		USER_DA(%a6),&0x303	#  {%d0-%d1/%a0-%a1}
	fmovm.x		USER_FP0(%a6),&0xf0	#  {%fp0-%fp3}
	fmovm.l		USER_FPCR(%a6),%control,%status,%iaddr
	frestore	(%sp)+
	unlk		%a6
#
# If trace mode enabled, then go to trace handler.  This handler 
# cannot have any fp instructions.  If there are fp inst's and an 
# exception has been restored into the machine then the exception 
# will occur upon execution of the fp inst.  This is not desirable 
# in the kernel (supervisor mode).  See MC68040 manual Section 9.3.8.
#
finish_up:
	btst		&7,(%sp)	# test T1 in SR
	bne.b		g_trace
	btst		&6,(%sp)	# test T0 in SR
	bne.b		g_trace
	bra		fpsp_done
#
# Change integer stack to look like trace stack
# The address of the instruction that caused the
# exception is already in the integer stack (is
# the same as the saved friar)
#
# If the current frame is already a 6-word stack then all
# that needs to be done is to change the vector# to TRACE.
# If the frame is only a 4-word stack (meaning we got here
# on an Unsupported data type exception), then we need to grow
# the stack an extra 2 words and get the FPIAR from the FPU.
#
g_trace:
	bftst		EXC_VEC-4(%sp){&0:&4}
	bne		g_easy

	sub.w		&4,%sp		# make room
	mov.l		4(%sp),(%sp)
	mov.l		8(%sp),4(%sp)
	sub.w		&BUSY_SIZE,%sp
	fsave		(%sp)
	fmov		%iaddr,BUSY_SIZE+EXC_EA-4(%sp)
	frestore	(%sp)
	add.w		&BUSY_SIZE,%sp

g_easy:
	mov.w		&TRACE_VEC,EXC_VEC-4(%sp)
	bra		real_trace
#
#	end		
