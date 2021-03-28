#ident	"@(#)stat.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)stat.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	MOT:stat.s	2.1 

# C library -- stat

# error = stat(string, statbuf);
# char statbuf[36]

# 	trap vectors
	_m4_include_(SYSTRAP_H)
	
	file	"stat.s"
	global	stat
	global  cerror%

stat:
	MCOUNT
	mov.l	&stat%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	mov.l	&0,%d0
	rts
