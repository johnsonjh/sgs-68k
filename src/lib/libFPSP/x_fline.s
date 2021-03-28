#
#	x_fline.sa 3.3 1/10/91
#
#	fpsp_fline --- FPSP handler for fline exception
#
#	First determine if the exception is one of the unimplemented
#	floating point instructions.  If so, let fpsp_unimp handle it.
#	Next, determine if the instruction is an fmovecr with a non-zero
#	<ea> field.  If so, handle here and return.  Otherwise, it
#	must be a real F-line exception.
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

#	xref	real_fline	
#	xref	fpsp_unimp	
#	xref	uni_2	
#	xref	mem_read	
#	xref	fpsp_fmt_error	

	global		fpsp_fline
fpsp_fline:
#
#	check for unimplemented vector first.  Use EXC_VEC-4 because
#	the equate is valid only after a 'link a6' has pushed one more
#	long onto the stack.
#
	cmp.w		EXC_VEC-4(%sp),&UNIMP_VEC
	beq.l		fpsp_unimp

#
#	fmovecr with non-zero <ea> handling here
#
	sub.l		&4,%sp		# 4 accounts for 2-word difference
#				;between six word frame (unimp) and
#				;four word frame
	link		%a6,&-LOCAL_SIZE
	fsave		-(%sp)
	movm.l		&0x303,USER_DA(%a6)	#  {%d0-%d1/%a0-%a1}
	mov.l		EXC_PC+4(%a6),%a0	# get address of fline instruction
	lea.l		L_SCR1(%a6),%a1	# use L_SCR1 as scratch
	mov.l		&4,%d0
	add.l		&4,%a6		# to offset the sub.l #4,a7 above so that
#				;a6 can point correctly to the stack frame 
#				;before branching to mem_read
	bsr.l		mem_read
	sub.l		&4,%a6
	mov.l		L_SCR1(%a6),%d0	# d0 contains the fline and command word
	bfextu		%d0{&4:&3},%d1	# extract coprocessor id
	cmp.b		%d1,&1		# check if cpid=1
	bne.w		not_mvcr	# exit if not
	bfextu		%d0{&16:&6},%d1
	cmp.b		%d1,&0x17	# check if it is an FMOVECR encoding
	bne.w		not_mvcr
#				;if an FMOVECR instruction, fix stack
#				;and go to FPSP_UNIMP
fix_stack:
	cmp.b		(%sp),&VER_40	# test for orig unimp frame
	bne.b		ck_rev
	sub.l		&UNIMP_40_SIZE-4,%sp	# emulate an orig fsave
	mov.b		&VER_40,(%sp)
	mov.b		&UNIMP_40_SIZE-4,1(%sp)
	clr.w		2(%sp)
	bra.b		fix_con
ck_rev:
	cmp.b		(%sp),&VER_41	# test for rev unimp frame
	bne.l		fpsp_fmt_error	# if not $40 or $41, exit with error
	sub.l		&UNIMP_41_SIZE-4,%sp	# emulate a rev fsave
	mov.b		&VER_41,(%sp)
	mov.b		&UNIMP_41_SIZE-4,1(%sp)
	clr.w		2(%sp)
fix_con:
	mov.w		EXC_SR+4(%a6),EXC_SR(%a6)	# move stacked sr to new position
	mov.l		EXC_PC+4(%a6),EXC_PC(%a6)	# move stacked pc to new position
	fmov.l		EXC_PC(%a6),%iaddr	# point FPIAR to fline inst
	mov.l		&4,%d1
	add.l		%d1,EXC_PC(%a6)	# increment stacked pc value to next inst
	mov.w		&0x202c,EXC_VEC(%a6)	# reformat vector to unimp
	clr.l		EXC_EA(%a6)	# clear the EXC_EA field
	mov.w		%d0,CMDREG1B(%a6)	# move the lower word into CMDREG1B
	clr.l		E_BYTE(%a6)
	bset		&UFLAG,T_BYTE(%a6)
	movm.l		USER_DA(%a6),&0x303	# restore data registers {%d0-%d1/%a0-%a1}
	bra.l		uni_2

not_mvcr:
	movm.l		USER_DA(%a6),&0x303	# restore data registers {%d0-%d1/%a0-%a1}
	frestore	(%sp)+
	unlk		%a6
	add.l		&4,%sp
	bra.l		real_fline

#	end		
