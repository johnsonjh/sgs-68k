#ident	"@(#)open.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)open.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	MOT:open.s	2.1 
# C library -- open


# file = open(string, mode)

# file == -1 means error

# 	trap vectors
	_m4_include_(SYSTRAP_H)

	file	"open.s"
	global	open
	global  cerror%

open:
	MCOUNT
	mov.l	&open%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	rts
