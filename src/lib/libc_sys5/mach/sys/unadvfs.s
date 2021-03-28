#ident	"@(#)unadvfs.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)unadvfs.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#ATT	.ident	"libc-m32:sys/unadvfs.s	1.3"

	file	"unadvfs.s"
	set	unadvfs%,71
	global	unadvfs
	global	cerror%

unadvfs:
	MCOUNT
	mov.l	&unadvfs%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	rts
