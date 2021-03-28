#ident	"@(#)chroot.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)chroot.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	MOT:chroot.s	2.1 
#  C library -- chroot


#  error = chroot(string);
 
	file	"chroot.s"
	set	chroot%,61
 
	global	chroot
	global	cerror%

chroot:
	MCOUNT
	mov.l	&chroot%,%d0
	trap	&0
	bcc.b	noerror
	jmp	cerror%
noerror:
	rts
