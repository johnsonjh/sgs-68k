#ident	"@(#)rumount.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)rumount.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT:.ident	"libc-m32:sys/rumount.s	1.3"

	file	"rumount.s"
	set	rumount%,73
	global	rumount
	global	cerror%

rumount:
	MCOUNT
	mov.l	&rumount%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	rts
