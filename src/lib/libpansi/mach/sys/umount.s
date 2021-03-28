#ident	"@(#)umount.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)umount.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	MOT:umount.s	2.1 

# C library -- umount

	file	"umount.s"

# 	trap vectors
	_m4_include_(SYSTRAP_H)
	
	global	umount
	global	cerror%

umount:
	MCOUNT
	mov.l	&umount%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	mov.l	&0,%d0
	rts
