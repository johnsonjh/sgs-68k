#ident	"@(#)statfs.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)statfs.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#ATT:.ident	"libc-m32:sys/statfs.s	1.1"


	file	"statfs.s"
	set	statfs%,35
	global	statfs
	global	cerror%

statfs:
	MCOUNT
	mov.l	&statfs%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	rts
