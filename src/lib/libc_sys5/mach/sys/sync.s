#ident	"@(#)sync.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)sync.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	MOT:sync.s	2.1 

#  C library -- sync

	file	"sync.s"
	set	sync%,36
	global	sync

sync:
	MCOUNT
	mov.l	&sync%,%d0
	trap	&0
	rts
