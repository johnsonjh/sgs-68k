#ident	"@(#)getegid.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)getegid.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	MOT:getegid.s	2.1 
# C library -- getegid


# gid = getegid();
# returns effective gid

	file	"getegid.s"
	set	getgid%,47
	global	getegid

getegid:
	MCOUNT
	mov.l	&getgid%,%d0
	trap	&0
	mov.l	%d1,%d0
	rts
