#ident	"@(#)getgid.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)getgid.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	MOT:getgid.s	2.1 
# C library -- getgid


# 	trap vectors
	_m4_include_(SYSTRAP_H)
	
# gid = getgid();
# returns real gid

	file	"getgid.s"
	global	getgid

getgid:
	MCOUNT
	mov.l	&getgid%,%d0
	trap	&0
	rts
