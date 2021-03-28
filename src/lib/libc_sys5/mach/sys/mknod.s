#ident	"@(#)mknod.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)mknod.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	MOT:mknod.s	2.1 
# C library -- mknod


# error = mknod(string, mode, major.minor);

	file	"mknod.s"
	set	mknod%,14
	global	mknod
	global  cerror%

mknod:
	MCOUNT
	mov.l	&mknod%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	mov.l	&0,%d0
	rts
