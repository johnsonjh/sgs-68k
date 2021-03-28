#ident	"@(#)write.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)write.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	MOT:write.s	2.1 

# C library -- write

# nwritten = write(file, buffer, count);
# nwritten == -1 means error

	file	"write.s"
	set	write%,4
	global	write
	global  cerror%

write:
	MCOUNT
	mov.l	&write%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	rts
