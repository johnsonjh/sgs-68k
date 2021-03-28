#ident	"@(#)cerror.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)cerror.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	OLD:cerror.s	2.1 
# C return sequence which
# sets errno, returns -1.


	file	"cerror.s"
	global	cerror%
	global	errno


	text
cerror%:
	mov.l	%d0,errno
	mov.l	&-1,%d0
	mov.l	%d0,%a0
	rts
