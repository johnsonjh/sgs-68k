#ident	"@(#)exit.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)exit.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	MOT:exit.s	2.1 
# C library -- exit


# exit(code)
# code is %deturn in r0 to system
# Same as plain exit, for user who want to define their own exit.

	file	"exit.s"
	set	exit%,1
	global	_exit

_exit:
	MCOUNT
	mov.l	&exit%,%d0
	trap	&0
	stop	&0
