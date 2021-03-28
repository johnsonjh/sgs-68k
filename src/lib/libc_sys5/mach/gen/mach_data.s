#ident	"@(#)mach_data.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)mach_data.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	.ident	"ATT:libc-m32:gen/m32_data.s	1.5"

	file	"mach_data.s"
# This file contains
# the definition of the
# global symbol errno 
# 
# int errno = 0;

	global	errno

	data
	align	4
errno:	
	long 	0
