#ident	"@(#)getpid.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)getpid.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	MOT:getpid.s	2.1 


# getpid -- get process ID

	file	"getpid.s"
	set	getpid%,20
	global	getpid

getpid:
	MCOUNT
	mov.l	&getpid%,%d0
	trap	&0
	rts
