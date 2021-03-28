#ident	"@(#)stime.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)stime.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	MOT:stime.s	2.1 

#  C - library stime

# 	trap vectors
	_m4_include_(SYSTRAP_H)
	
	file	"stime.s"
	global	stime
	global  cerror%

stime:
	MCOUNT
	mov.l	4(%sp),%a0		# copy time to set
	mov.l	(%a0),4(%sp)
	mov.l	&stime%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	mov.l	&0,%d0
	rts
