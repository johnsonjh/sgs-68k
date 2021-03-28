#ident	"@(#)read.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)read.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	MOT:read.s	2.1 

# C library -- read

# nread = read(file, buffer, count);
# nread ==0 means eof; nread == -1 means error

	file	"read.s"
	set	read%,3
	global	read
	global  cerror%

read:
	MCOUNT
	mov.l	&read%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	rts
