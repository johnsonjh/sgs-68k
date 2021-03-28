#ident	"@(#)time.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)time.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	MOT:time.s	2.1 

# C library -- time

# tvec = time(tvec);

	file	"time.s"
	set	time%,13
global	time

time:
	MCOUNT
	link	%fp,&0
	mov.l	&time%,%d0
	trap	&0
	mov.l	8(%sp),%d1	# modified (+4) due to link
	beq.b	nostore
	mov.l	%d1,%a0
	mov.l	%d0,(%a0)
nostore:
	unlk	%fp
	rts
