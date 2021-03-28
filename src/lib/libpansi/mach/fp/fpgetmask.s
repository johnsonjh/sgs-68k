	#ident	"@(#)fpgetmask.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
		ident	"@(#)fpgetmask.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	file	"fpgetmask.s"
#(#	)fpgetmask.s
#  C-library fpgetmask

	global	fpgetmask

fpgetmask:
	fmov.l	%control,%d0
	and.l	&0x0000FF00,%d0		# old mask bits
	rts
