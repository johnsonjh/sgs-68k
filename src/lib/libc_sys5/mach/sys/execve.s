#ident	"@(#)execve.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)execve.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	MOT:execve.s	2.1 
# C library -- execve


# execve(file, argv, env);

# where argv is a vector argv[0] ... argv[x], 0
# last vector element must be 0

	file	"execve.s"
	set	exece%,59
	global	execve
	global	cerror%

execve:
	MCOUNT
	mov.l	&exece%,%d0
	trap	&0
	jmp 	cerror%
