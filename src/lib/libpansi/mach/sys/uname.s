#ident	"@(#)uname.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)uname.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	MOT:uname.s	2.1 

# C library -- uname
#
# int uname (name)
# struct utsname *name

	file	"uname.s"

# 	trap vectors
	_m4_include_(SYSTRAP_H)
	
	set	uname%,0
	global	uname
	global	cerror%

uname:
	MCOUNT
	mov.l	4(%sp),%d0
	mov.l	&uname%,%d1
	mov.l	%d1,-(%sp)
	sub.l	&4,%sp		# dummy arg for fill
	mov.l	%d0,-(%sp)
	bsr.b	sys
	add.w	&12,%sp
	rts

sys:
	mov.l	&utssys%,%d0
	trap	&0
	bcc.b	noerror
	jmp	cerror%
noerror:
	mov.l	&0,%d0
	rts
