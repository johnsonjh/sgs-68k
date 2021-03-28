	#ident	"@(#)fpgtstcky.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
		ident	"@(#)fpgtstcky.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	file	"fpgtstcky.s"
#(#	)fpgetsticky.s
#  C-library fpgetsticky

	global	fpgetsticky

fpgetsticky:
	fmov.l	%status,%d0
	and.l	&0x000000F8,%d0		#get sticky bits
	rts
