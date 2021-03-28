/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) num.c: version 25.1 created on 12/2/91 at 19:33:19	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)num.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libgen:num.c	1.2"	*/
	
#ident	"@(#)libgen:num.c	25.1"

/*	
	true if string is numeric characters
*/

#include	<ctype.h>
#include	"libgen.h"


int
num( string )
register char *string;
{
	if( !string  ||  !*string )
		return  0;	/* null pointer or null string */
	while( *string )
		if( !isdigit( *(string++) ) )
			return  0;
	return  1;
}
