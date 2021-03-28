#ident	"@(#)utime.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)utime.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	MOT:utime.s	2.1 

# C library -- utime
 
#  error = utime(string,timev);
 
	file	"utime.s"
	set	utime%,30
	global	utime
	global	cerror%
 
utime:
	MCOUNT
	mov.l	&utime%,%d0
	trap	&0
	bcc.b	noerror
	jmp	cerror%
noerror:
	rts
