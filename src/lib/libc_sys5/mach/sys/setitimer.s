#ident	"@(#)setitimer.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)setitimer.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	Copyright (c) 1988 by Arix Corporation
#	All Rights Reserved

	file	"setitimer.s"
	set	setitimer%,110
	global	setitimer
	global	cerror%

setitimer:
	MCOUNT
	mov.l	&setitimer%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	rts
