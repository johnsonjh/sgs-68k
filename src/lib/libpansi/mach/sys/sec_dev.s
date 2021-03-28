#ident	"@(#)sec_dev.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)sec_dev.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1989 ARIX
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF ARIX
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	MER: sec_dev.s 
# C library-- sec_dev


	file	"sec_dev.s"
	global	sec_dev
	global  cerror%

sec_dev:
	MCOUNT
	mov.l	4(%sp), %d0
	mov.l	8(%sp), %a0
	trap	&0
	bcc.b	noerror
	jmp	cerror%

noerror:
	rts
