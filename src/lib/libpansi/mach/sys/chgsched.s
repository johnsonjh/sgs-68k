#ident	"@(#)chgsched.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)chgsched.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."

	_m4_include_(SYSTRAP_H)

	file	"chgsched.s"

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
