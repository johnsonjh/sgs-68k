	#ident	"@(#)fpsetrnd.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
		ident	"@(#)fpsetrnd.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	file	"fpsetrnd.s"
#(#	)fpsetround.s
#  C-library fpsetround

	global	fpsetround

fpsetround:
	mov.l	(4.w,%sp),%d1
	fmov.l	%control,%d0
	and.l	&0x00000030,%d1		# extract rounding mode
	mov.l	%d2,-(%sp)
	mov.b	%d0,%d2
	and.l	&0x00000030,%d0
	and.b	&0xc0,%d2
	or.b	%d1,%d2
	fmov.l	%d2,%control		# put new rounding mode
	mov.l	(%sp)+,%d2
	rts
