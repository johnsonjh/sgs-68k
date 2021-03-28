/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) basename.c: version 25.1 created on 12/2/91 at 19:31:40	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)basename.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libgen:basename.c	1.2"	*/
	
#ident	"@(#)libgen:basename.c	25.1"


/*
	Return pointer to the last element of a pathname.
*/

#include	<string.h>
#include	"libgen.h"


char *
basename( s )
char	*s;
{
	register char	*p;

	if( !s  ||  !*s )			/* zero or empty argument */
		return  ".";

	p = s + strlen( s );
	while( p != s  &&  *--p == '/' )	/* skip trailing /s */
		;
	
	while( p != s )
		if( *--p == '/' )
			return  ++p;

	return  p;
}
