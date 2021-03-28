#	START NEW ARIX SCCS HEADER
#
#	@(#) setrlimit.s: version 25.1 created on 12/2/91 at 18:53:23
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	ident	"@(#)setrlimit.s	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
#
#	END NEW ARIX SCCS HEADER
#
	ident	"@(#)setrlimit.s	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"

	file	"setrlimit.s"
	set	setrlimit%,128
	global	setrlimit
	global	cerror%

setrlimit:
	MCOUNT
	mov.l	&setrlimit%,%d0
	trap 	&0
	bcc.b	noerror
	jmp	cerror%
noerror:
	rts
