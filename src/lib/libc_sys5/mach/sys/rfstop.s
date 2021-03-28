#ident	"@(#)rfstop.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)rfstop.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT:.ident	"MOT:libc-m32:sys/rfstop.s	1.2"

	file	"rfstop.s"
	set	rfstop%,77
	global	rfstop
	global	cerror%

rfstop:
	MCOUNT
	mov.l	&rfstop%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	rts
