#
#	x_bsun.sa 3.3 7/1/91
#
#	fpsp_bsun --- FPSP handler for branch/set on unordered exception
#
#	Copy the PC to FPIAR to maintain 881/882 compatability
#
#	The real_bsun handler will need to perform further corrective
#	measures as outlined in the 040 User's Manual on pages
#	9-41f, section 9.8.3.
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

#	xref	real_bsun	

	global		fpsp_bsun
fpsp_bsun:
#
	link		%a6,&-LOCAL_SIZE
	fsave		-(%sp)
	movm.l		&0x303,USER_DA(%a6)	#  {%d0-%d1/%a0-%a1}
	fmovm.x		&0xf0,USER_FP0(%a6)	#  {%fp0-%fp3}
	fmovm.l		%control,%status,%iaddr,USER_FPCR(%a6)

#
	mov.l		EXC_PC(%a6),USER_FPIAR(%a6)
#
	movm.l		USER_DA(%a6),&0x303	#  {%d0-%d1/%a0-%a1}
	fmovm.x		USER_FP0(%a6),&0xf0	#  {%fp0-%fp3}
	fmovm.l		USER_FPCR(%a6),%control,%status,%iaddr
	frestore	(%sp)+
	unlk		%a6
	bra.l		real_bsun
#
#	end		
