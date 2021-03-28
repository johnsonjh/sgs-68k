#	START NEW ARIX SCCS HEADER
#
#	@(#) getrlimit.s: version 25.1 created on 12/2/91 at 18:51:09
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	ident	"@(#)getrlimit.s	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
#
#	END NEW ARIX SCCS HEADER
#
	ident	"@(#)getrlimit.s	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"

	file	"getrlimit.s"
	set	getrlimit%,129
	global	getrlimit
	global	cerror%

getrlimit:
	MCOUNT
	mov.l	&getrlimit%,%d0
	trap 	&0
	bcc.b	noerror
	jmp	cerror%
noerror:
	rts
