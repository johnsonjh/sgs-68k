/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) triml.c: version 25.1 created on 12/2/91 at 19:34:29	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)triml.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libgen:triml.c	1.2"	*/
	
#ident	"@(#)libgen:triml.c	25.1"

/*
	Trim leading characters from a string.
	Returns pointer to first character in string not in tc.
*/

#include	<string.h>
#include	"libgen.h"


char *
triml( string, tc )
char	*string;
char	*tc;	/* characters to trim */
{
	int	l;

	l = strspn( string, tc );
	return  string + l;
}
