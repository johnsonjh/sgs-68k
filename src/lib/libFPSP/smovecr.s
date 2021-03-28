#
#	smovecr.sa 3.1 12/10/90
#
#	The entry point sMOVECR returns the constant at the
#	offset given in the instruction field.
#
#	Input: An offset in the instruction word.
#
#	Output:	The constant rounded to the user's rounding
#		mode unchecked for overflow.
#
#	Modified: fp0.
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

#	xref	nrm_set	
#	xref	round	
#	xref	PIRN	
#	xref	PIRZRM	
#	xref	PIRP	
#	xref	SMALRN	
#	xref	SMALRZRM	
#	xref	SMALRP	
#	xref	BIGRN	
#	xref	BIGRZRM	
#	xref	BIGRP	

FZERO:
	long		00000000
#
#	FMOVECR 
#
	global		smovcr
smovcr:
	bfextu		CMDREG1B(%a6){&9:&7},%d0	# get offset
	bfextu		USER_FPCR(%a6){&26:&2},%d1	# get rmode
#
# check range of offset
#
	tst.b		%d0		# if zero, offset is to pi
	beq.b		PI_TBL		# it is pi
	cmp.b		%d0,&0x0a	# check range $01 - $0a
	ble.b		Z_VAL		# if in this range, return zero
	cmp.b		%d0,&0x0e	# check range $0b - $0e
	ble.b		SM_TBL		# valid constants in this range
	cmp.b		%d0,&0x2f	# check range $10 - $2f
	ble.b		Z_VAL		# if in this range, return zero 
	cmp.b		%d0,&0x3f	# check range $30 - $3f
	ble		BG_TBL		# valid constants in this range
Z_VAL:
	fmov.s		FZERO,%fp0
	rts
PI_TBL:
	tst.b		%d1		# offset is zero, check for rmode
	beq.b		PI_RN		# if zero, rn mode
	cmp.b		%d1,&0x3	# check for rp
	beq.b		PI_RP		# if 3, rp mode
PI_RZRM:
	lea.l		PIRZRM,%a0	# rmode is rz or rm, load PIRZRM in a0
	bra		set_finx
PI_RN:
	lea.l		PIRN,%a0	# rmode is rn, load PIRN in a0
	bra		set_finx
PI_RP:
	lea.l		PIRP,%a0	# rmode is rp, load PIRP in a0
	bra		set_finx
SM_TBL:
	sub.l		&0xb,%d0	# make offset in 0 - 4 range
	tst.b		%d1		# check for rmode
	beq.b		SM_RN		# if zero, rn mode
	cmp.b		%d1,&0x3	# check for rp
	beq.b		SM_RP		# if 3, rp mode
SM_RZRM:
	lea.l		SMALRZRM,%a0	# rmode is rz or rm, load SMRZRM in a0
	cmp.b		%d0,&0x2	# check if result is inex
	ble		set_finx	# if 0 - 2, it is inexact
	bra		no_finx		# if 3, it is exact
SM_RN:
	lea.l		SMALRN,%a0	# rmode is rn, load SMRN in a0
	cmp.b		%d0,&0x2	# check if result is inex
	ble		set_finx	# if 0 - 2, it is inexact
	bra		no_finx		# if 3, it is exact
SM_RP:
	lea.l		SMALRP,%a0	# rmode is rp, load SMRP in a0
	cmp.b		%d0,&0x2	# check if result is inex
	ble		set_finx	# if 0 - 2, it is inexact
	bra		no_finx		# if 3, it is exact
BG_TBL:
	sub.l		&0x30,%d0	# make offset in 0 - f range
	tst.b		%d1		# check for rmode
	beq.b		BG_RN		# if zero, rn mode
	cmp.b		%d1,&0x3	# check for rp
	beq.b		BG_RP		# if 3, rp mode
BG_RZRM:
	lea.l		BIGRZRM,%a0	# rmode is rz or rm, load BGRZRM in a0
	cmp.b		%d0,&0x1	# check if result is inex
	ble		set_finx	# if 0 - 1, it is inexact
	cmp.b		%d0,&0x7	# second check
	ble		no_finx		# if 0 - 7, it is exact
	bra		set_finx	# if 8 - f, it is inexact
BG_RN:
	lea.l		BIGRN,%a0	# rmode is rn, load BGRN in a0
	cmp.b		%d0,&0x1	# check if result is inex
	ble		set_finx	# if 0 - 1, it is inexact
	cmp.b		%d0,&0x7	# second check
	ble		no_finx		# if 0 - 7, it is exact
	bra		set_finx	# if 8 - f, it is inexact
BG_RP:
	lea.l		BIGRP,%a0	# rmode is rp, load SMRP in a0
	cmp.b		%d0,&0x1	# check if result is inex
	ble		set_finx	# if 0 - 1, it is inexact
	cmp.b		%d0,&0x7	# second check
	ble		no_finx		# if 0 - 7, it is exact
#	bra	set_finx	;if 8 - f, it is inexact
set_finx:
	or.l		&inx2a_mask,USER_FPSR(%a6)	# set inex2/ainex
no_finx:
	mulu.l		&12,%d0		# use offset to point into tables
	mov.l		%d1,L_SCR1(%a6)	# load mode for round call
	bfextu		USER_FPCR(%a6){&24:&2},%d1	# get precision
	tst.l		%d1		# check if extended precision
#
# Precision is extended
#
	bne.b		not_ext		# if extended, do not call round
	fmovm.x		(%a0,%d0),&0x80	# return result in fp0 {%fp0}
	rts
#
# Precision is single or double
#
not_ext:
	swap		%d1		# rnd prec in upper word of d1
	add.l		L_SCR1(%a6),%d1	# merge rmode in low word of d1
	mov.l		(%a0,%d0),FP_SCR1(%a6)	# load first word to temp storage
	mov.l		4(%a0,%d0),FP_SCR1+4(%a6)	# load second word
	mov.l		8(%a0,%d0),FP_SCR1+8(%a6)	# load third word
	clr.l		%d0		# clear g,r,s
	lea		FP_SCR1(%a6),%a0
	btst		&sign_bit,LOCAL_EX(%a0)
	sne		LOCAL_SGN(%a0)	# convert to internal ext. format
	bsr		round		# go round the mantissa

	bfclr		LOCAL_SGN(%a0){&0:&8}	# convert back to IEEE ext format
	beq.b		fin_fcr
	bset		&sign_bit,LOCAL_EX(%a0)
fin_fcr:
	fmovm.x		(%a0),&0x80	#  {%fp0}
	rts

#	end		
