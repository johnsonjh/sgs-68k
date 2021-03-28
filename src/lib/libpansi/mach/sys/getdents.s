#ident	"@(#)getdents.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)getdents.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

# C library -- getdents
#	ATT:.ident	"libc-m32:sys/getdents.s	1.2.1.1"
	file	"getdents.s"


# 	trap vectors
	_m4_include_(SYSTRAP_H)
	
# num = getdents(file, buffer, count, flag);
# num is number of bytes read; num == -1 means error

	global	getdents
	global  cerror%

getdents:
	MCOUNT
	mov.l	&getdents%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	rts
