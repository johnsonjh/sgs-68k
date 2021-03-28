#ident	"@(#)getpgrp.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)getpgrp.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	MOT:getpgrp.s	2.1 

# C library -- getpgrp

# 	trap vectors
	_m4_include_(SYSTRAP_H)

	file	"getpgrp.s"
	global	getpgrp

getpgrp:
	MCOUNT
	mov.l	&0,%d0
	mov.l	%d0,-(%sp)
	bsr.b	pgrp
	add.l	&4,%sp
	rts

pgrp:
	mov.l	&setpgrp%,%d0
	trap	&0
	rts
