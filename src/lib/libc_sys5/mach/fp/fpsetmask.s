	#ident	"@(#)fpsetmask.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
		ident	"@(#)fpsetmask.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	file	"fpsetmask.s"
#(#	)fpsetmask.s
#  C-library fpsetmask

	global	fpsetmask

fpsetmask:
	mov.l	(4.w,%sp),%d1
	fmov.l	%control,%d0
	and.w	&0xFF00,%d1		
	mov.l	%d2,-(%sp)
	mov.w	%d0,%d2
	and.w	&0xFF00,%d0		# old mask bits
	and.w	&0x00FF,%d2
	or.w	%d1,%d2
	fmov.l	%d2,%control		# put in new mask bits
	mov.l	(%sp)+,%d2
	rts
