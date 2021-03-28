#ident	"@(#)facl.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)facl.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1989 ARIX
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF ARIX
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	MOT:facl.s	1.1 

# 	trap vectors
	_m4_include_(SYSTRAP_H)
	
	file	"facl.s"
	global	facl
	global  cerror%

facl:
	MCOUNT
	mov.l	&facl%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	mov.l	&0,%d0
	rts
