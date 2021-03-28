#
#	x_store.sa 3.2 1/24/91
#
#	store --- store operand to memory or register
#
#	Used by underflow and overflow handlers.
#
#	a6 = points to fp value to be stored.
#

#		Copyright (C) Motorola, Inc. 1990
#			All Rights Reserved
#
#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF MOTOROLA 
#	The copyright notice above does not evidence any  
#	actual or intended publication of such source code.

#	idnt	2,1	Motorola 040 Floating Point Software Package

	text

fpreg_mask:
	byte		0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01

#	include	fpsp.h	

#	xref	mem_write	
#	xref	get_fline	
#	xref	g_opcls	
#	xref	g_dfmtou	
#	xref	reg_dest	

	global		dest_ext
	global		dest_dbl
	global		dest_sgl

	global		store
store:
	btst		&E3,E_BYTE(%a6)
	beq.b		E1_sto
E3_sto:
	mov.l		CMDREG3B(%a6),%d0
	bfextu		%d0{&6:&3},%d0	# isolate dest. reg from cmdreg3b
sto_fp:
	lea		fpreg_mask,%a1
	mov.b		(%a1,%d0.w),%d0	# convert reg# to dynamic register mask
	tst.b		LOCAL_SGN(%a0)
	beq.b		is_pos
	bset		&sign_bit,LOCAL_EX(%a0)
is_pos:
	fmovm.x		(%a0),%d0	# move to correct register
#
#	if fp0-fp3 is being modified, we must put a copy
#	in the USER_FPn variable on the stack because all exception
#	handlers restore fp0-fp3 from there.
#
	cmp.b		%d0,&0x80
	bne.b		not_fp0
	fmovm.x		&0x80,USER_FP0(%a6)	#  {%fp0}
	rts
not_fp0:
	cmp.b		%d0,&0x40
	bne.b		not_fp1
	fmovm.x		&0x40,USER_FP1(%a6)	#  {%fp1}
	rts
not_fp1:
	cmp.b		%d0,&0x20
	bne.b		not_fp2
	fmovm.x		&0x20,USER_FP2(%a6)	#  {%fp2}
	rts
not_fp2:
	cmp.b		%d0,&0x10
	bne.b		not_fp3
	fmovm.x		&0x10,USER_FP3(%a6)	#  {%fp3}
	rts
not_fp3:
	rts

E1_sto:
	bsr.l		g_opcls		# returns opclass in d0
	cmp.b		%d0,&3
	beq		opc011		# branch if opclass 3
	mov.l		CMDREG1B(%a6),%d0
	bfextu		%d0{&6:&3},%d0	# extract destination register
	bra.b		sto_fp

opc011:
	bsr.l		g_dfmtou	# returns dest format in d0
#				;ext=00, sgl=01, dbl=10
	mov.l		%a0,%a1		# save source addr in a1
	mov.l		EXC_EA(%a6),%a0	# get the address
	cmp.l		%d0,&0		# if dest format is extended
	beq.w		dest_ext	# then branch
	cmp.l		%d0,&1		# if dest format is single
	beq.b		dest_sgl	# then branch
#
#	fall through to dest_dbl
#

#
#	dest_dbl --- write double precision value to user space
#
#Input
#	a0 -> destination address
#	a1 -> source in extended precision
#Output
#	a0 -> destroyed
#	a1 -> destroyed
#	d0 -> 0
#
#Changes extended precision to double precision.
# Note: no attempt is made to round the extended value to double.
#	dbl_sign = ext_sign
#	dbl_exp = ext_exp - $3fff(ext bias) + $7ff(dbl bias)
#	get rid of ext integer bit
#	dbl_mant = ext_mant{62:12}
#
#	    	---------------   ---------------    ---------------
#  extended ->  |s|    exp    |   |1| ms mant   |    | ls mant     |
#	    	---------------   ---------------    ---------------
#	   	 95	    64    63 62	      32      31     11	  0
#				     |			     |
#				     |			     |
#				     |			     |
#		 	             v   		     v
#	    		      ---------------   ---------------
#  double   ->  	      |s|exp| mant  |   |  mant       |
#	    		      ---------------   ---------------
#	   	 	      63     51   32   31	       0
#
dest_dbl:
	clr.l		%d0		# clear d0
	mov.w		LOCAL_EX(%a1),%d0	# get exponent
	sub.w		&0x3fff,%d0	# subtract extended precision bias
	cmp.w		%d0,&0x4000	# check if inf
	beq.b		inf		# if so, special case
	add.w		&0x3ff,%d0	# add double precision bias
	swap		%d0		# d0 now in upper word
	lsl.l		&4,%d0		# d0 now in proper place for dbl prec exp
	tst.b		LOCAL_SGN(%a1)
	beq.b		get_mant	# if postive, go process mantissa
	bset		&31,%d0		# if negative, put in sign information
#				; before continuing
	bra.b		get_mant	# go process mantissa
inf:
	mov.l		&0x7ff00000,%d0	# load dbl inf exponent
	clr.l		LOCAL_HI(%a1)	# clear msb
	tst.b		LOCAL_SGN(%a1)
	beq.b		dbl_inf		# if positive, go ahead and write it
	bset		&31,%d0		# if negative put in sign information
dbl_inf:
	mov.l		%d0,LOCAL_EX(%a1)	# put the new exp back on the stack
	bra.b		dbl_wrt
get_mant:
	mov.l		LOCAL_HI(%a1),%d1	# get ms mantissa
	bfextu		%d1{&1:&20},%d1	# get upper 20 bits of ms
	or.l		%d1,%d0		# put these bits in ms word of double
	mov.l		%d0,LOCAL_EX(%a1)	# put the new exp back on the stack
	mov.l		LOCAL_HI(%a1),%d1	# get ms mantissa
	mov.l		&21,%d0		# load shift count
	lsl.l		%d0,%d1		# put lower 11 bits in upper bits
	mov.l		%d1,LOCAL_HI(%a1)	# build lower lword in memory
	mov.l		LOCAL_LO(%a1),%d1	# get ls mantissa
	bfextu		%d1{&0:&21},%d0	# get ls 21 bits of double
	or.l		%d0,LOCAL_HI(%a1)	# put them in double result
dbl_wrt:
	mov.l		&0x8,%d0	# byte count for double precision number
	exg		%a0,%a1		# a0=supervisor source, a1=user dest
	bsr.l		mem_write	# move the number to the user's memory
	rts
#
#	dest_sgl --- write single precision value to user space
#
#Input
#	a0 -> destination address
#	a1 -> source in extended precision
#
#Output
#	a0 -> destroyed
#	a1 -> destroyed
#	d0 -> 0
#
#Changes extended precision to single precision.
#	sgl_sign = ext_sign
#	sgl_exp = ext_exp - $3fff(ext bias) + $7f(sgl bias)
#	get rid of ext integer bit
#	sgl_mant = ext_mant{62:12}
#
#	    	---------------   ---------------    ---------------
#  extended ->  |s|    exp    |   |1| ms mant   |    | ls mant     |
#	    	---------------   ---------------    ---------------
#	   	 95	    64    63 62	   40 32      31     12	  0
#				     |	   |
#				     |	   |
#				     |	   |
#		 	             v     v
#	    		      ---------------
#  single   ->  	      |s|exp| mant  |
#	    		      ---------------
#	   	 	      31     22     0
#
dest_sgl:
	clr.l		%d0
	mov.w		LOCAL_EX(%a1),%d0	# get exponent
	sub.w		&0x3fff,%d0	# subtract extended precision bias
	cmp.w		%d0,&0x4000	# check if inf
	beq.b		sinf		# if so, special case
	add.w		&0x7f,%d0	# add single precision bias
	swap		%d0		# put exp in upper word of d0
	lsl.l		&7,%d0		# shift it into single exp bits
	tst.b		LOCAL_SGN(%a1)
	beq.b		get_sman	# if positive, continue
	bset		&31,%d0		# if negative, put in sign first
	bra.b		get_sman	# get mantissa
sinf:
	mov.l		&0x7f800000,%d0	# load single inf exp to d0
	tst.b		LOCAL_SGN(%a1)
	beq.b		sgl_wrt		# if positive, continue
	bset		&31,%d0		# if negative, put in sign info
	bra.b		sgl_wrt

get_sman:
	mov.l		LOCAL_HI(%a1),%d1	# get ms mantissa
	bfextu		%d1{&1:&23},%d1	# get upper 23 bits of ms
	or.l		%d1,%d0		# put these bits in ms word of single

sgl_wrt:
	mov.l		%d0,L_SCR1(%a6)	# put the new exp back on the stack
	mov.l		&0x4,%d0	# byte count for single precision number
	tst.l		%a0		# users destination address
	beq.b		sgl_Dn		# destination is a data register
	exg		%a0,%a1		# a0=supervisor source, a1=user dest
	lea.l		L_SCR1(%a6),%a0	# point a0 to data
	bsr.l		mem_write	# move the number to the user's memory
	rts
sgl_Dn:
	bsr.l		get_fline	# returns fline word in d0
	and.w		&0x7,%d0	# isolate register number
	mov.l		%d0,%d1		# d1 has size:reg formatted for reg_dest
	or.l		&0x10,%d1	# reg_dest wants size added to reg#
	bra.l		reg_dest	# size is X, rts in reg_dest will
#				;return to caller of dest_sgl
dest_ext:
	tst.b		LOCAL_SGN(%a1)	# put back sign into exponent word
	beq.b		dstx_cont
	bset		&sign_bit,LOCAL_EX(%a1)
dstx_cont:
	clr.b		LOCAL_SGN(%a1)	# clear out the sign byte

	mov.l		&0x0c,%d0	# byte count for extended number
	exg		%a0,%a1		# a0=supervisor source, a1=user dest
	bsr.l		mem_write	# move the number to the user's memory
	rts

#	end		
