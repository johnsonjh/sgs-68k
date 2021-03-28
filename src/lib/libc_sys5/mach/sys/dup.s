#ident	"@(#)dup.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)dup.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	MOT:dup.s	2.1 
# C library -- dup


#	f = dup(of [ ,nf])
#	f == -1 for error

	file	"dup.s"
	set	dup%,41
	global	dup
	global	cerror%

dup:
	MCOUNT
	mov.l	&dup%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	rts
