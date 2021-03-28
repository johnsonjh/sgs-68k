#ident	"@(#)syscall.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)syscall.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	MOT:syscall.s	2.1 

# C library -- syscall

#  Interpret a given system call

	file	"syscall.s"
	global	syscall
	global	cerror%

syscall:
	MCOUNT
	mov.l	4(%sp),%d0	# set up syscall number
	mov.l	(%sp)+,(%sp)	# one fewer arguments, but save rtn addr
	trap	&0
	bcc.b	noerror		# same number args as it stacked
	mov.l	(%sp),-(%sp)	# restore rtn addr, as caller will remove
	jmp	cerror%
noerror:
	mov.l	(%sp),-(%sp)	# restore rtn addr, as caller will remove
	rts
