#ident	"@(#)getuid.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)getuid.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	MOT:getuid.s	2.1 
# C library -- getuid


# uid = getuid();
#  returns real uid

# 	trap vectors
	_m4_include_(SYSTRAP_H)
	
	file	"getuid.s"
	global	getuid

getuid:
	MCOUNT
	mov.l	&getuid%,%d0
	trap	&0
	rts
