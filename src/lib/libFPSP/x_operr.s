#
#	x_operr.sa 3.5 7/1/91
#
#	fpsp_operr --- FPSP handler for operand error exception
#
#	See 68040 User's Manual pp. 9-44f
#
# Note 1: For trap disabled 040 does the following:
# If the dest is a fp reg, then an extended precision non_signaling
# NAN is stored in the dest reg.  If the dest format is b, w, or l and
# the source op is a NAN, then garbage is stored as the result (actually
# the upper 32 bits of the mantissa are sent to the integer unit). If
# the dest format is integer (b, w, l) and the operr is caused by
# integer overflow, or the source op is inf, then the result stored is
# garbage.
# There are three cases in which operr is incorrectly signaled on the 
# 040.  This occurs for move_out of format b, w, or l for the largest 
# negative integer (-2^7 for b, -2^15 for w, -2^31 for l).
#
#	  On opclass = 011 fmove.(b,w,l) that causes a conversion
#	  overflow -> OPERR, the exponent in wbte (and fpte) is:
#		byte    56 - (62 - exp)
#		word    48 - (62 - exp)
#		long    32 - (62 - exp)
#
#			where exp = (true exp) - 1
#
#  So, wbtemp and fptemp will contain the following on erroneoulsy
#	  signalled operr:
#			fpts = 1
#			fpte = $4000  (15 bit externally)
#		byte	fptm = $ffffffff ffffff80
#		word	fptm = $ffffffff ffff8000
#		long	fptm = $ffffffff 80000000
#
# Note 2: For trap enabled 040 does the following:
# If the inst is move_out, then same as Note 1.
# If the inst is not move_out, the dest is not modified.
# The exceptional operand is not defined for integer overflow 
# during a move_out.
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

#	xref	mem_write	
#	xref	real_operr	
#	xref	real_inex	
#	xref	get_fline	
#	xref	fpsp_done	
#	xref	reg_dest	

	global		fpsp_operr
fpsp_operr:
#
	link		%a6,&-LOCAL_SIZE
	fsave		-(%sp)
	movm.l		&0x303,USER_DA(%a6)	#  {%d0-%d1/%a0-%a1}
	fmovm.x		&0xf0,USER_FP0(%a6)	#  {%fp0-%fp3}
	fmovm.l		%control,%status,%iaddr,USER_FPCR(%a6)

#
# Check if this is an opclass 3 instruction.
#  If so, fall through, else branch to operr_end
#
	btst		&TFLAG,T_BYTE(%a6)
	beq.b		operr_end

#
# If the destination size is B,W,or L, the operr must be 
# handled here.
#
	mov.l		CMDREG1B(%a6),%d0
	bfextu		%d0{&3:&3},%d0	# 0=long, 4=word, 6=byte
	cmp.b		%d0,&0		# determine size; check long
	beq.w		operr_long
	cmp.b		%d0,&4		# check word
	beq.w		operr_word
	cmp.b		%d0,&6		# check byte
	beq.w		operr_byte

#
# The size is not B,W,or L, so the operr is handled by the 
# kernel handler.  Set the operr bits and clean up, leaving
# only the integer exception frame on the stack, and the 
# fpu in the original exceptional state.
#
operr_end:
	bset		&operr_bit,FPSR_EXCEPT(%a6)
	bset		&aiop_bit,FPSR_AEXCEPT(%a6)

	movm.l		USER_DA(%a6),&0x303	#  {%d0-%d1/%a0-%a1}
	fmovm.x		USER_FP0(%a6),&0xf0	#  {%fp0-%fp3}
	fmovm.l		USER_FPCR(%a6),%control,%status,%iaddr
	frestore	(%sp)+
	unlk		%a6
	bra.l		real_operr

operr_long:
	mov.l		&4,%d1		# write size to d1
	mov.b		STAG(%a6),%d0	# test stag for nan
	and.b		&0xe0,%d0	# clr all but tag
	cmp.b		%d0,&0x60	# check for nan
	beq		operr_nan
	cmp.l		FPTEMP_LO(%a6),&0x80000000	# test if ls lword is special
	bne.b		chklerr		# if not equal, check for incorrect operr
	bsr		check_upper	# check if exp and ms mant are special
	tst.l		%d0
	bne.b		chklerr		# if d0 is true, check for incorrect operr
	mov.l		&0x80000000,%d0	# store special case result
	bsr		operr_store
	bra.w		not_enabled	# clean and exit
#
#	CHECK FOR INCORRECTLY GENERATED OPERR EXCEPTION HERE
#
chklerr:
	mov.w		FPTEMP_EX(%a6),%d0
	and.w		&0x7FFF,%d0	# ignore sign bit
	cmp.w		%d0,&0x3FFE	# this is the only possible exponent value
	bne.b		chklerr2
fixlong:
	mov.l		FPTEMP_LO(%a6),%d0
	bsr		operr_store
	bra.w		not_enabled
chklerr2:
	mov.w		FPTEMP_EX(%a6),%d0
	and.w		&0x7FFF,%d0	# ignore sign bit
	cmp.w		%d0,&0x4000
	bhs.w		store_max	# exponent out of range

	mov.l		FPTEMP_LO(%a6),%d0
	and.l		&0x7FFF0000,%d0	# look for all 1's on bits 30-16
	cmp.l		%d0,&0x7FFF0000
	beq.b		fixlong

	tst.l		FPTEMP_LO(%a6)
	bpl.b		chklepos
	cmp.l		FPTEMP_HI(%a6),&0xFFFFFFFF
	beq.b		fixlong
	bra.w		store_max
chklepos:
	tst.l		FPTEMP_HI(%a6)
	beq.b		fixlong
	bra.w		store_max

operr_word:
	mov.l		&2,%d1		# write size to d1
	mov.b		STAG(%a6),%d0	# test stag for nan
	and.b		&0xe0,%d0	# clr all but tag
	cmp.b		%d0,&0x60	# check for nan
	beq.w		operr_nan
	cmp.l		FPTEMP_LO(%a6),&0xffff8000	# test if ls lword is special
	bne.b		chkwerr		# if not equal, check for incorrect operr
	bsr		check_upper	# check if exp and ms mant are special
	tst.l		%d0
	bne.b		chkwerr		# if d0 is true, check for incorrect operr
	mov.l		&0x80000000,%d0	# store special case result
	bsr		operr_store
	bra.w		not_enabled	# clean and exit
#
#	CHECK FOR INCORRECTLY GENERATED OPERR EXCEPTION HERE
#
chkwerr:
	mov.w		FPTEMP_EX(%a6),%d0
	and.w		&0x7FFF,%d0	# ignore sign bit
	cmp.w		%d0,&0x3FFE	# this is the only possible exponent value
	bne.b		store_max
	mov.l		FPTEMP_LO(%a6),%d0
	swap		%d0
	bsr		operr_store
	bra.w		not_enabled

operr_byte:
	mov.l		&1,%d1		# write size to d1
	mov.b		STAG(%a6),%d0	# test stag for nan
	and.b		&0xe0,%d0	# clr all but tag
	cmp.b		%d0,&0x60	# check for nan
	beq.b		operr_nan
	cmp.l		FPTEMP_LO(%a6),&0xffffff80	# test if ls lword is special
	bne.b		chkberr		# if not equal, check for incorrect operr
	bsr		check_upper	# check if exp and ms mant are special
	tst.l		%d0
	bne.b		chkberr		# if d0 is true, check for incorrect operr
	mov.l		&0x80000000,%d0	# store special case result
	bsr		operr_store
	bra.w		not_enabled	# clean and exit
#
#	CHECK FOR INCORRECTLY GENERATED OPERR EXCEPTION HERE
#
chkberr:
	mov.w		FPTEMP_EX(%a6),%d0
	and.w		&0x7FFF,%d0	# ignore sign bit
	cmp.w		%d0,&0x3FFE	# this is the only possible exponent value
	bne.b		store_max
	mov.l		FPTEMP_LO(%a6),%d0
	asl.l		&8,%d0
	swap		%d0
	bsr		operr_store
	bra.w		not_enabled

#
# This operr condition is not of the special case.  Set operr
# and aiop and write the portion of the nan to memory for the
# given size.
#
operr_nan:
	or.l		&opaop_mask,USER_FPSR(%a6)	# set operr & aiop

	mov.l		ETEMP_HI(%a6),%d0	# output will be from upper 32 bits
	bsr		operr_store
	bra		end_operr
#
# Store_max loads the max pos or negative for the size, sets
# the operr and aiop bits, and clears inex and ainex, incorrectly
# set by the 040.
#
store_max:
	or.l		&opaop_mask,USER_FPSR(%a6)	# set operr & aiop
	bclr		&inex2_bit,FPSR_EXCEPT(%a6)
	bclr		&ainex_bit,FPSR_AEXCEPT(%a6)
	fmov.l		&0,%status
	tst.w		FPTEMP_EX(%a6)	# check sign
	blt.b		load_neg
	mov.l		&0x7fffffff,%d0
	bsr		operr_store
	bra		end_operr
load_neg:
	mov.l		&0x80000000,%d0
	bsr		operr_store
	bra		end_operr

#
# This routine stores the data in d0, for the given size in d1,
# to memory or data register as required.  A read of the fline
# is required to determine the destination.
#
operr_store:
	mov.l		%d0,L_SCR1(%a6)	# move write data to L_SCR1
	mov.l		%d1,-(%sp)	# save register size
	bsr.l		get_fline	# fline returned in d0
	mov.l		(%sp)+,%d1
	bftst		%d0{&26:&3}	# if mode is zero, dest is Dn
	bne.b		dest_mem
#
# Destination is Dn.  Get register number from d0. Data is on
# the stack at (a7). D1 has size: 1=byte,2=word,4=long/single
#
	and.l		&7,%d0		# isolate register number
	cmp.l		%d1,&4
	beq.b		op_long		# the most frequent case
	cmp.l		%d1,&2
	bne.b		op_con
	or.l		&8,%d0
	bra.b		op_con
op_long:
	or.l		&0x10,%d0
op_con:
	mov.l		%d0,%d1		# format size:reg for reg_dest
	bra.l		reg_dest	# call to reg_dest returns to caller
#				;of operr_store
#
# Destination is memory.  Get <ea> from integer exception frame
# and call mem_write.
#
dest_mem:
	lea.l		L_SCR1(%a6),%a0	# put ptr to write data in a0
	mov.l		EXC_EA(%a6),%a1	# put user destination address in a1
	mov.l		%d1,%d0		# put size in d0
	bsr.l		mem_write
	rts
#
# Check the exponent for $c000 and the upper 32 bits of the 
# mantissa for $ffffffff.  If both are true, return d0 clr
# and store the lower n bits of the least lword of FPTEMP
# to d0 for write out.  If not, it is a real operr, and set d0.
#
check_upper:
	cmp.l		FPTEMP_HI(%a6),&0xffffffff	# check if first byte is all 1's
	bne.b		true_operr	# if not all 1's then was true operr
	cmp.w		FPTEMP_EX(%a6),&0xc000	# check if incorrectly signalled
	beq.b		not_true_operr	# branch if not true operr
	cmp.w		FPTEMP_EX(%a6),&0xbfff	# check if incorrectly signalled
	beq.b		not_true_operr	# branch if not true operr
true_operr:
	mov.l		&1,%d0		# signal real operr
	rts
not_true_operr:
	clr.l		%d0		# signal no real operr
	rts

#
# End_operr tests for operr enabled.  If not, it cleans up the stack
# and does an rte.  If enabled, it cleans up the stack and branches
# to the kernel operr handler with only the integer exception
# frame on the stack and the fpu in the original exceptional state
# with correct data written to the destination.
#
end_operr:
	btst		&operr_bit,FPCR_ENABLE(%a6)
	beq.b		not_enabled
enabled:
	movm.l		USER_DA(%a6),&0x303	#  {%d0-%d1/%a0-%a1}
	fmovm.x		USER_FP0(%a6),&0xf0	#  {%fp0-%fp3}
	fmovm.l		USER_FPCR(%a6),%control,%status,%iaddr
	frestore	(%sp)+
	unlk		%a6
	bra.l		real_operr

not_enabled:
#
# It is possible to have either inex2 or inex1 exceptions with the
# operr.  If the inex enable bit is set in the FPCR, and either
# inex2 or inex1 occured, we must clean up and branch to the
# real inex handler.
#
ck_inex:
	mov.b		FPCR_ENABLE(%a6),%d0
	and.b		FPSR_EXCEPT(%a6),%d0
	and.b		&0x3,%d0
	beq.w		operr_exit
#
# Inexact enabled and reported, and we must take an inexact exception.
#
take_inex:
	mov.b		&INEX_VEC,EXC_VEC+1(%a6)
	mov.l		USER_FPSR(%a6),FPSR_SHADOW(%a6)
	or.l		&sx_mask,E_BYTE(%a6)
	movm.l		USER_DA(%a6),&0x303	#  {%d0-%d1/%a0-%a1}
	fmovm.x		USER_FP0(%a6),&0xf0	#  {%fp0-%fp3}
	fmovm.l		USER_FPCR(%a6),%control,%status,%iaddr
	frestore	(%sp)+
	unlk		%a6
	bra.l		real_inex
#
# Since operr is only an E1 exception, there is no need to frestore
# any state back to the fpu.
#
operr_exit:
	movm.l		USER_DA(%a6),&0x303	#  {%d0-%d1/%a0-%a1}
	fmovm.x		USER_FP0(%a6),&0xf0	#  {%fp0-%fp3}
	fmovm.l		USER_FPCR(%a6),%control,%status,%iaddr
	unlk		%a6
	bra.l		fpsp_done

#	end		
