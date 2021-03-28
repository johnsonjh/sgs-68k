#ident	"@(#)rfsys.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)rfsys.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT:.ident	"libc-m32:sys/rfsys.s	1.2"

	file	"rfsys.s"
	set	rfsys%,78
	global	rfsys
	global	cerror%

rfsys:
	MCOUNT
	mov.l	&rfsys%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	rts
