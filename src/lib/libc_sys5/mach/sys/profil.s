#ident	"@(#)profil.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)profil.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	MOT:profil.s	2.1 

# profil

	file	"profil.s"
	set	prof%,44
	global	profil

profil:
	MCOUNT
	mov.l	&prof%,%d0
	trap	&0
	rts
