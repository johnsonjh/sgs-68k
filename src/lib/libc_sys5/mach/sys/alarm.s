#ident	"@(#)alarm.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)alarm.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	MOT:alarm.s	2.1 


# C library - alarm

	file	"alarm.s"
	set	alarm%,27
	global	alarm

alarm:
	MCOUNT
	mov.l	&alarm%,%d0
	trap	&0
	rts
