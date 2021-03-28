#ident	"@(#)setuid.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)setuid.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	MOT:setuid.s	2.1 

# C library -- setuid

# error = setuid(uid);

	file	"setuid.s"
	set	setuid%,23
	global	setuid
	global  cerror%

setuid:
	MCOUNT
	mov.l	&setuid%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	mov.l	&0,%d0
	rts
