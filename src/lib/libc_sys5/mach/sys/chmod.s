#ident	"@(#)chmod.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)chmod.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	MOT:chmod.s	2.1 
# C library -- chmod


# error = chmod(string, mode);

	file	"chmod.s"
	set	chmod%,15
	global	chmod
	global	cerror%

chmod:
	MCOUNT
	mov.l	&chmod%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	mov.l	&0,%d0
	rts
