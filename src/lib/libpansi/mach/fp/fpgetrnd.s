	#ident	"@(#)fpgetrnd.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
		ident	"@(#)fpgetrnd.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	file	"fpgetrnd.s"
#(#	)fpgetround.s	
#  C-library fpgetround

	global	fpgetround

fpgetround:
	fmov.l	%control,%d0
	and.l	&0x00000030,%d0
	rts
