#ident	"@(#)umask.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)umask.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	MOT:umask.s	2.1 

#  C library -- umask
 
#  omask = umask(mode);
 
	file	"umask.s"
	
# 	trap vectors
	_m4_include_(SYSTRAP_H)
	
	global	umask
	global	cerror%

umask:
	MCOUNT
	mov.l	&umask%,%d0
	trap	&0
	bcc.b	noerror
	jmp	cerror%
noerror:
	rts
