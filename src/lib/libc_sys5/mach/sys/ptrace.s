#ident	"@(#)ptrace.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)ptrace.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

# ptrace -- C library
#	MOT:ptrace.s	2.1 

# ptrace -- C library

#	result = ptrace(req, pid, addr, data);

	file	"ptrace.s"
	set	ptrace%,26
	global	ptrace
	global	cerror%
	global	errno

ptrace:
	MCOUNT
	clr.l	errno		# Mon Aug  5 15:42:03 PDT 1985
				# clear this as in old ptrace.s
				# Thu Sep 25 11:34:49 PDT 1986
				# 5.3 3b2 also clears this!
	mov.l	&ptrace%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	rts

