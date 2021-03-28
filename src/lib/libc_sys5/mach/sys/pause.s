#ident	"@(#)pause.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)pause.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	MOT:pause.s	2.1 
# C library - pause


	file	"pause.s"
	set	pause%,29
	global	pause
	global	cerror%

	even
pause:
	MCOUNT
	mov.l	&pause%,%d0
	trap	&0
	bcc.b	noerror
	jmp	cerror%
noerror:
#	mov.l	&0,%d0
	rts
