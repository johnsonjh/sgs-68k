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

# 	trap vectors
	_m4_include_(SYSTRAP_H)
	
	file	"wait.s"

	global	wait
	global	wait2
	global	waitpid
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

waitpid:
	MCOUNT
	mov.l	&waitpid%,%d0
	trap	&0
	bcc.b 	wpidnoerror%
	jmp 	cerror%
wpidnoerror%:
	tst.l	8(%sp)		# status desired?
	beq.b	nowpidstat%	# no
	mov.l	8(%sp),%a0
	mov.l	%d1,(%a0)	# store child's status
nowpidstat%:
	rts

wait2:
	MCOUNT
	mov.l	(8.w,%sp),-(%sp)	# get options
	mov.l	(8.w,%sp),-(%sp)	# get stat_loc
	mov.l	&-1,-(%sp)		# look for any process
	mov.l	(12.w,%sp),-(%sp)	# return pc
	mov.l	&waitpid%,%d0		# wait2 makes use of waitpid
	trap	&0
	bcc.b 	w2noerror%
	add.l	&16,%sp
	jmp 	cerror%
w2noerror%:
	add.l	&16,%sp
	tst.l	4(%sp)		# status desired?
	beq.b	now2stat%	# no
	mov.l	4(%sp),%a0
	mov.l	%d1,(%a0)	# store child's status
now2stat%:
	rts
