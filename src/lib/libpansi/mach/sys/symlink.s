#	START NEW ARIX SCCS HEADER
#
#	@(#) symlink.s: version 25.1 created on 12/2/91 at 20:02:15
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	ident	"@(#)symlink.s	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
#
#	END NEW ARIX SCCS HEADER
#
	ident	"@(#)symlink.s	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	MOT:symlink.s	2.1 
# C library -- symlink


# error = symlink(old-file, new-file);

	file	"symlink.s"
	set	symlink%,89
	global	symlink
	global	cerror%

symlink:
	MCOUNT
	mov.l	&symlink%,%d0
	trap 	&0
	bcc.b	noerror
	jmp	cerror%
noerror:
	mov.l	&0,%d0
	rts
