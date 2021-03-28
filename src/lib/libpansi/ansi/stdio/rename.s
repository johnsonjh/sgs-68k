#ident	"@(#)rename.as	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)rename.as	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF ARIX
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#
#	MOT:rename.s	2.1 
# C library -- rename


# 	trap vectors
	_m4_include_(SYSTRAP_H)

# rename();


	file	"rename.as"
	global	rename
	global	cerror%

rename:
	MCOUNT
	mov.l	&rename%,%d0
	trap	&0
	bcc.b	renameok
	jmp	cerror%
renameok:
	mov.l	&0,%d0
	rts
