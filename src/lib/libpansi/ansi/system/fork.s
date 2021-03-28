#ident	"@(#)fork.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)fork.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#
#	MOT:fork.s	2.1 
# C library -- fork


# pid = fork();

# %d1 == 0 in parent process, d1 == 1 in child process.
# %d0 == pid of child in parent, d0 == pid of parent in child.

# 	trap vectors
	_m4_include_(SYSTRAP_H)

	file	"fork.s"
	global	fork
	global	cerror%

fork:
	MCOUNT
	mov.l	&fork%,%d0
	trap	&0
	bcc.b	forkok
	jmp	cerror%
forkok:
	tst.b	%d1
	beq.b	parent
	mov.l	&0,%d0
parent:
	rts
