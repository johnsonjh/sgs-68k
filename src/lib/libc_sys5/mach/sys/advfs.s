#ident	"@(#)advfs.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)advfs.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT:.ident	"libc-m32:sys/advfs.s	1.3"


	file	"advfs.s"
	set	advfs%,70
	global	advfs
	global  cerror%

advfs:
	MCOUNT
	mov.l	&advfs%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	rts
