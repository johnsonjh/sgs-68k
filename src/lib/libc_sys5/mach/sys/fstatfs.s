#ident	"@(#)fstatfs.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)fstatfs.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT:ident	"libc-m32:sys/fstatfs.s	1.1"
# C library -- fstatfs


# int
# fstatfs(fd, sp, len, fstyp);
# int fd;
# struct statfs *sp;
# int len, fstyp;

	set	fstatfs%,38

	global	fstatfs
	global  cerror%

fstatfs:
	MCOUNT
	mov.l	&fstatfs%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	mov.l	&0,%d0
	rts
