#ident	"@(#)wait.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)wait.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	MOT:wait.s	2.1 

# C library -- wait

# pid = wait(0);
#   or,
# pid = wait(&status);

# pid == -1 if error
# status indicates fate of process, if given

	file	"wait.s"
	set	wait%,7
	global	wait
	global  cerror%

wait:
	MCOUNT
	mov.l	&wait%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	tst.l	4(%sp)		# status desired?
	beq.b	nostatus	# no
	mov.l	4(%sp),%a0
	mov.l	%d1,(%a0)	# store child's status
nostatus:
	rts
