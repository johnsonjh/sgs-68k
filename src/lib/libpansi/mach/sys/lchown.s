#	START NEW ARIX SCCS HEADER
#
#	@(#) lchown.s: version 25.1 created on 12/2/91 at 20:00:00
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	ident	"@(#)lchown.s	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
#
#	END NEW ARIX SCCS HEADER
#
	ident	"@(#)lchown.s	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	MOT:lchown.s	2.1 
# C library -- lchown


# error = lchown(string, owner);

	file	"lchown.s"
	set	lchown%,130
	global	lchown
	global	cerror%

lchown:
	MCOUNT
	mov.l	&lchown%,%d0
	trap 	&0
	bcc.b	noerror
	jmp	cerror%
noerror:
	mov.l	&0,%d0
	rts
