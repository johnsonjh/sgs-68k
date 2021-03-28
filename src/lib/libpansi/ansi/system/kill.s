#	START NEW ARIX SCCS HEADER
#
#	@(#) kill.s: version 25.1 created on 12/2/91 at 19:55:32
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	ident	"@(#)kill.s	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
#
#	END NEW ARIX SCCS HEADER
#
	ident	"@(#)kill.s	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
#ident	"@(#)ANSI.kill.s	1.2"
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	MOT:kill.s	2.1 
# C library -- kill

	ident	"@(#)libc/mach/sys:kill.s	1.2"

# 	trap vectors
	_m4_include_(SYSTRAP_H)

	file	"kill.s"
	global	kill
	global cerror%

kill:
	MCOUNT
	mov.l	&kill%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	mov.l	&0,%d0
	rts
