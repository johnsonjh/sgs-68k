#	START NEW ARIX SCCS HEADER
#
#	@(#) readlink.s: version 25.1 created on 12/2/91 at 20:01:10
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	ident	"@(#)readlink.s	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
#
#	END NEW ARIX SCCS HEADER
#
	ident	"@(#)readlink.s	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	MOT:readlink.s	2.1 
# C library -- readlink


# retval = readlink(path, buffer, filesize);
# retval = number of bytes or -1 indicating an error

	file	"readlink.s"
	set	readlink%,90
	global	readlink
	global	cerror%

readlink:
	MCOUNT
	mov.l	&readlink%,%d0
	trap 	&0
	bcc.b	noerror
	jmp	cerror%
noerror:
	rts
