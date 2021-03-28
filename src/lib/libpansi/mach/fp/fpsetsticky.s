	#ident	"@(#)fpststcky.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
		ident	"@(#)fpststcky.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	file	"fpststcky.s"
#(#	)fpsetsticky.s
#  C-library fpsetsticky

	global	fpsetsticky

fpsetsticky:
	mov.l	(4.w,%sp),%d1
	fmov.l	%status,%d0
	and.l	&0x000000F8,%d1		# extract sticky bits
	mov.l	%d2,-(%sp)
	mov.b	%d0,%d2
	and.l	&0x000000F8,%d0		
	and.b	&0x00,%d2
	or.b	%d1,%d2
	fmov.l	%d2,%status		# put in sticky bits
	mov.l	(%sp)+,%d2
	rts
