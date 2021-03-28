#ident	"@(#)ldiv.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."


/*	Copyright (c) 1989 ARIX Corp.	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF ARIX Corp.	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-port:gen/ldiv.c 1.0 */
	
/* This function is not reentrant !!! */

#include <stdlib.h>

ldiv_t ldiv ( long int numer, long int denom )
{
	ldiv_t res;

	res.quot = numer / denom;
	res.rem  = numer % denom;
	return ( res );
}
