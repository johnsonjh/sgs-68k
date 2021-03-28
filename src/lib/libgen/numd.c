/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) numd.c: version 25.1 created on 12/2/91 at 19:33:22	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)numd.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libgen:numd.c	1.2"	*/
	
#ident	"@(#)libgen:numd.c	25.1"

/*	
	true if string is numeric, with or without decimal point
*/

#include	<ctype.h>
#include	"libgen.h"


int
numd( string )
register char *string;
{
	register int	seen_a_digit = 0;

	if( !string  ||  !*string )
		return  0;	/* null pointer or null string */
	while( *string  &&  *string != '.' )
		if( isdigit( *(string++) ) )
			seen_a_digit = 1;
		else
			return  0;
	if( *string == '.' ) {	/* have found decimal */
		string++;	/* go to char. after decimal */
		while( *string )
			if( isdigit( *(string++) ) )
				seen_a_digit = 1;
			else
				return  0;
	}
	return  seen_a_digit;
}
