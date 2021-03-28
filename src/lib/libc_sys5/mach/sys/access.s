#ident	"@(#)access.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)access.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	MOT:access.s	2.1 


# access(file, request)
#  test ability to access file in all indicated ways
#  1 - read
#  2 - write
#  4 - execute

	file	"access.s"
	set	access%,33
	global	access
	global	cerror%

access:
	MCOUNT
	mov.l	&access%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	rts
