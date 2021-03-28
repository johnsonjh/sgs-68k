#	START NEW ARIX SCCS HEADER
#
#	@(#) getrlimit.s: version 25.1 created on 12/2/91 at 19:59:51
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

# 	trap vectors
	_m4_include_(SYSTRAP_H)

	global	getrlimit

getrlimit:
	MCOUNT
	mov.l	&getrlimit%,%d0
	trap 	&0
	bcc.b	noerror
	jmp	cerror%
noerror:
	rts
