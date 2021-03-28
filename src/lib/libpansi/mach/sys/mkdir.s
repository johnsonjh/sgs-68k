#ident	"@(#)mkdir.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)mkdir.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

# C library -- mkdir
#	.ident	"libc-m32:sys/mkdir.s	1.3"
	file	"mkdir.s"


# 	trap vectors
	_m4_include_(SYSTRAP_H)
	
# file = mkdir(string, mode);
# file == -1 if error

	global	mkdir
	global	cerror%

mkdir:
	MCOUNT
	mov.l	&mkdir%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	rts
