/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) trimt.c: version 25.1 created on 12/2/91 at 19:34:32	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)trimt.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libgen:trimt.c	1.2"	*/
	
#ident	"@(#)libgen:trimt.c	25.1"

/*
	Trim trailing characters from a string.
	Returns pointer to last character in string not in tc.
*/

#include	<string.h>
#include	"libgen.h"


char *
trimt( string, tc )
char	*string;
char	*tc;	/* characters to trim */
{
	char	*p;

	p = string + strlen( string );  
	while( p != string )
		if( !strchr( tc, *--p ) )
			return  ++p;
		
	return  p;
}
