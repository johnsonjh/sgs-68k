#
#	x_snan.sa 3.3 7/1/91
#
# fpsp_snan --- FPSP handler for signalling NAN exception
#
# SNAN for float -> integer conversions (integer conversion of
# an SNAN) is a non-maskable run-time exception.
#
# For trap disabled the 040 does the following:
# If the dest data format is s, d, or x, then the SNAN bit in the NAN
# is set to one and the resulting non-signaling NAN (truncated if
# necessary) is transferred to the dest.  If the dest format is b, w,
# or l, then garbage is written to the dest (actually the upper 32 bits
# of the mantissa are sent to the integer unit).
#
# For trap enabled the 040 does the following:
# If the inst is move_out, then the results are the same as for trap 
# disabled with the exception posted.  If the instruction is not move_
# out, the dest. is not modified, and the exception is posted.
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

#	xref	get_fline	
#	xref	mem_write	
#	xref	real_snan	
#	xref	real_inex	
#	xref	fpsp_done	
#	xref	reg_dest	

	global		fpsp_snan
fpsp_snan:
	link		%a6,&-LOCAL_SIZE
	fsave		-(%sp)
	movm.l		&0x303,USER_DA(%a6)	#  {%d0-%d1/%a0-%a1}
	fmovm.x		&0xf0,USER_FP0(%a6)	#  {%fp0-%fp3}
	fmovm.l		%control,%status,%iaddr,USER_FPCR(%a6)

#
# Check if trap enabled
#
	btst		&snan_bit,FPCR_ENABLE(%a6)
	bne.b		ena		# If enabled, then branch

	bsr.l		move_out	# else SNAN disabled
#
# It is possible to have an inex1 exception with the
# snan.  If the inex enable bit is set in the FPCR, and either
# inex2 or inex1 occured, we must clean up and branch to the
# real inex handler.
#
ck_inex:
	mov.b		FPCR_ENABLE(%a6),%d0
	and.b		FPSR_EXCEPT(%a6),%d0
	and.b		&0x3,%d0
	beq.w		end_snan
#
# Inexact enabled and reported, and we must take an inexact exception.
#
take_inex:
	mov.b		&INEX_VEC,EXC_VEC+1(%a6)
	movm.l		USER_DA(%a6),&0x303	#  {%d0-%d1/%a0-%a1}
	fmovm.x		USER_FP0(%a6),&0xf0	#  {%fp0-%fp3}
	fmovm.l		USER_FPCR(%a6),%control,%status,%iaddr
	frestore	(%sp)+
	unlk		%a6
	bra.l		real_inex
#
# SNAN is enabled.  Check if inst is move_out.
# Make any corrections to the 040 output as necessary.
#
ena:
	btst		&5,CMDREG1B(%a6)	# if set, inst is move out
	beq.w		not_out

	bsr.l		move_out

report_snan:
	mov.b		(%sp),VER_TMP(%a6)
	cmp.b		(%sp),&VER_40	# test for orig unimp frame
	bne.b		ck_rev
	mov.l		&13,%d0		# need to zero 14 lwords
	bra.b		rep_con
ck_rev:
	mov.l		&11,%d0		# need to zero 12 lwords
rep_con:
	clr.l		(%sp)
loop1:
	clr.l		-(%sp)		# clear and dec a7
	dbra.w		%d0,loop1
	mov.b		VER_TMP(%a6),(%sp)	# format a busy frame
	mov.b		&BUSY_SIZE-4,1(%sp)
	mov.l		USER_FPSR(%a6),FPSR_SHADOW(%a6)
	or.l		&sx_mask,E_BYTE(%a6)
	movm.l		USER_DA(%a6),&0x303	#  {%d0-%d1/%a0-%a1}
	fmovm.x		USER_FP0(%a6),&0xf0	#  {%fp0-%fp3}
	fmovm.l		USER_FPCR(%a6),%control,%status,%iaddr
	frestore	(%sp)+
	unlk		%a6
	bra.l		real_snan
#
# Exit snan handler by expanding the unimp frame into a busy frame
#
end_snan:
	bclr		&E1,E_BYTE(%a6)

	mov.b		(%sp),VER_TMP(%a6)
	cmp.b		(%sp),&VER_40	# test for orig unimp frame
	bne.b		ck_rev2
	mov.l		&13,%d0		# need to zero 14 lwords
	bra.b		rep_con2
ck_rev2:
	mov.l		&11,%d0		# need to zero 12 lwords
rep_con2:
	clr.l		(%sp)
loop2:
	clr.l		-(%sp)		# clear and dec a7
	dbra.w		%d0,loop2
	mov.b		VER_TMP(%a6),(%sp)	# format a busy frame
	mov.b		&BUSY_SIZE-4,1(%sp)	# write busy size
	mov.l		USER_FPSR(%a6),FPSR_SHADOW(%a6)
	or.l		&sx_mask,E_BYTE(%a6)
	movm.l		USER_DA(%a6),&0x303	#  {%d0-%d1/%a0-%a1}
	fmovm.x		USER_FP0(%a6),&0xf0	#  {%fp0-%fp3}
	fmovm.l		USER_FPCR(%a6),%control,%status,%iaddr
	frestore	(%sp)+
	unlk		%a6
	bra.l		fpsp_done

#
# Move_out 
#
move_out:
	mov.l		EXC_EA(%a6),%a0	# get <ea> from exc frame

	bfextu		CMDREG1B(%a6){&3:&3},%d0	# move rx field to d0{2:0}
	cmp.l		%d0,&0		# check for long
	beq.b		sto_long	# branch if move_out long
	cmp.l		%d0,&4		# check for word
	beq.b		sto_word	# branch if move_out word
	cmp.l		%d0,&6		# check for byte
	beq.b		sto_byte	# branch if move_out byte
#
# Not byte, word or long
#
	rts
#	
# Get the 32 most significant bits of etemp mantissa
#
sto_long:
	mov.l		ETEMP_HI(%a6),%d1
	mov.l		&4,%d0		# load byte count
#
# Set signalling nan bit
#
	bset		&30,%d1
#
# Store to the users destination address
#
	tst.l		%a0		# check if <ea> is 0
	beq.b		wrt_dn		# destination is a data register
	mov.l		%d1,-(%sp)	# move the snan onto the stack
	mov.l		%a0,%a1		# load dest addr into a1
	mov.l		%sp,%a0		# load src addr of snan into a0
	bsr.l		mem_write	# write snan to user memory
	mov.l		(%sp)+,%d1	# clear off stack
	rts
#
# Get the 16 most significant bits of etemp mantissa
#
sto_word:
	mov.l		ETEMP_HI(%a6),%d1
	mov.l		&2,%d0		# load byte count
#
# Set signalling nan bit
#
	bset		&30,%d1
#
# Store to the users destination address
#
	tst.l		%a0		# check if <ea> is 0
	beq.b		wrt_dn		# destination is a data register

	mov.l		%d1,-(%sp)	# move the snan onto the stack
	mov.l		%a0,%a1		# load dest addr into a1
	mov.l		%sp,%a0		# point to low word
	bsr.l		mem_write	# write snan to user memory
	mov.l		(%sp)+,%d1	# clear off stack
	rts
#
# Get the 8 most significant bits of etemp mantissa
#
sto_byte:
	mov.l		ETEMP_HI(%a6),%d1
	mov.l		&1,%d0		# load byte count
#
# Set signalling nan bit
#
	bset		&30,%d1
#
# Store to the users destination address
#
	tst.l		%a0		# check if <ea> is 0
	beq.b		wrt_dn		# destination is a data register
	mov.l		%d1,-(%sp)	# move the snan onto the stack
	mov.l		%a0,%a1		# load dest addr into a1
	mov.l		%sp,%a0		# point to source byte
	bsr.l		mem_write	# write snan to user memory
	mov.l		(%sp)+,%d1	# clear off stack
	rts

#
#	wrt_dn --- write to a data register
#
#	We get here with D1 containing the data to write and D0 the
#	number of bytes to write: 1=byte,2=word,4=long.
#
wrt_dn:
	mov.l		%d1,L_SCR1(%a6)	# data
	mov.l		%d0,-(%sp)	# size
	bsr.l		get_fline	# returns fline word in d0
	mov.l		%d0,%d1
	and.l		&0x7,%d1	# d1 now holds register number
	mov.l		(%sp)+,%d0	# get original size
	cmp.l		%d0,&4
	beq.b		wrt_long
	cmp.l		%d0,&2
	bne.b		wrt_byte
wrt_word:
	or.l		&0x8,%d1
	bra.l		reg_dest
wrt_long:
	or.l		&0x10,%d1
	bra.l		reg_dest
wrt_byte:
	bra.l		reg_dest
#
# Check if it is a src nan or dst nan
#
not_out:
	mov.l		DTAG(%a6),%d0
	bfextu		%d0{&0:&3},%d0	# isolate dtag in lsbs

	cmp.b		%d0,&3		# check for nan in destination
	bne.b		issrc		# destination nan has priority
dst_nan:
	btst		&6,FPTEMP_HI(%a6)	# check if dest nan is an snan
	bne.b		issrc		# no, so check source for snan
	mov.w		FPTEMP_EX(%a6),%d0
	bra.b		cont
issrc:
	mov.w		ETEMP_EX(%a6),%d0
cont:
	btst		&15,%d0		# test for sign of snan
	beq.b		clr_neg
	bset		&neg_bit,FPSR_CC(%a6)
	bra.w		report_snan
clr_neg:
	bclr		&neg_bit,FPSR_CC(%a6)
	bra.w		report_snan

#	end		
