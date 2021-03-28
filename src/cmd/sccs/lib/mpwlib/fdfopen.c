/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) fdfopen.c: version 25.1 created on 12/2/91 at 17:11:41	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)fdfopen.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	AT&T: #ident	"sccs:lib/mpwlib/fdfopen.c	6.4"	*/

#ident	"@(#)sccs:fdfopen.c	25.1"

/*
	Interfaces with /lib/libS.a
	First arg is file descriptor, second is read/write mode (0/1).
	Returns file pointer on success,
	NULL on failure (no file structures available).
*/

# include	"stdio.h"
# include	"sys/types.h"
# include	"macros.h"

FILE *
fdfopen(fd, mode)
register int fd, mode;
{
	if (fstat(fd, &Statbuf) < 0)
		return(NULL);
	if (mode)
		return(fdopen(fd,"w"));
	else
		return(fdopen(fd,"r"));
}
