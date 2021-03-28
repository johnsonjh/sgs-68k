#ident	"@(#)geteuid.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)geteuid.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	MOT:geteuid.s	2.1 
# C library -- geteuid


# uid = geteuid();
#  returns effective uid

	file	"geteuid.s"
	set	getuid%,24
	global	geteuid

geteuid:
	MCOUNT
	mov.l	&getuid%,%d0
	trap	&0
	mov.l	%d1,%d0
	rts
