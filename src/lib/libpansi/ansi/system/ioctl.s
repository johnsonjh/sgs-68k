#	START NEW ARIX SCCS HEADER
#
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	ident	"@(#)ioctl.s	25.1	12/3/91 Copyright (c) 1990 by Arix Corporation"
#
#	END NEW ARIX SCCS HEADER
#
	ident	"@(#)ioctl.s	25.1	12/3/91 Copyright (c) 1990 by Arix Corporation"
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	MOT:ioctl.s	2.1 
# C library -- ioctl

# 	trap vectors
	_m4_include_(SYSTRAP_H)

	file	"ioctl.s"
	global	ioctl
	global	cerror%

ioctl:
	MCOUNT
	mov.l	&ioctl%,%d0
	trap	&0
	bcc.b	noerror
	jmp	cerror%
noerror:
	rts
