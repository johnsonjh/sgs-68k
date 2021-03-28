#ident	"@(#)plock.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)plock.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	MOT:plock.s	2.1 
# C library -- plock


# error = plock(op)

	file	"plock.s"
	set	plock%,45
	global	plock
	global	cerror%

plock:
	MCOUNT
	mov.l	&plock%,%d0
	trap	&0
	bcc.b	noerror
	jmp	cerror%
noerror:
	rts
