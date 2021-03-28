#ident	"@(#)creat.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)creat.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	MOT:creat.s	2.1 
# C library -- creat


# file = creat(string, mode);
# file == -1 if error

	file	"creat.s"
	set	creat%,8
	global	creat
	global	cerror%

creat:
	MCOUNT
	mov.l	&creat%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	rts
