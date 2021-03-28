/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) strnlen.c: version 25.1 created on 12/2/91 at 19:34:03	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)strnlen.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libgen:strnlen.c	1.2"	*/
	
#ident	"@(#)libgen:strnlen.c	25.1"

/*LINTLIBRARY*/
/*
 * Returns the number of
 * non-NULL bytes in string argument
 * but no more than n.
 */

#include	"libgen.h"


int
strnlen(s, n)
char	*s;
int	n;
{
	register char	*sp = s;
	register char	*send = sp + n;

	for( ;  sp < send  &&  *sp != '\0';  sp++ )
		;
	return (sp - s);
}
