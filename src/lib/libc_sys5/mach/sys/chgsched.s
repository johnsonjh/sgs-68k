#ident	"@(#)chgsched.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)chgsched.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."

	file	"chgsched.s"

	set	chgsched%,159

	global	chgsched
	global	cerror%

chgsched:
	MCOUNT
	mov.l	&chgsched%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	rts
