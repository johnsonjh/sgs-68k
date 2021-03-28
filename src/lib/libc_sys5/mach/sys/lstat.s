#	START NEW ARIX SCCS HEADER
#
#	@(#) lstat.s: version 25.1 created on 12/2/91 at 18:51:39
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	ident	"@(#)lstat.s	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
#
#	END NEW ARIX SCCS HEADER
#
	ident	"@(#)lstat.s	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	MOT:lstat.s	2.1 

# C library -- lstat

# error = lstat(string, statbuf);
# char statbuf[36]

	file	"lstat.s"
	set	lstat%,88
	global	lstat
	global	cerror%

lstat:
	MCOUNT
	mov.l	&lstat%,%d0
	trap 	&0
	bcc.b	noerror
	jmp	cerror%
noerror:
	mov.l	&0,%d0
	rts

