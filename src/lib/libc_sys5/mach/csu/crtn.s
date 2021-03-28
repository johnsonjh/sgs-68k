#ident	"@(#)crtn.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)crtn.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	.ident	"ATT:libc-m32:csu/crtn.s	1.2"


#
# This code provides the end to the _istart function which is used for
# the initialization of shared libraries.
#

	section	init,"x"

	mov.l	&0,%d0
	rts
