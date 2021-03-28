#ident	"@(#)times.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)times.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	MOT:times.s	2.1 

# C library -- times

	file	"times.s"
	set	times%,43
	global	times
	global	cerror%

times:
	MCOUNT
	mov.l	&times%,%d0
	trap	&0
	bcc.b	noerror
	jmp	cerror%
noerror:
	rts
