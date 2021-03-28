#ident	"@(#)div.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1989 ARIX Corp.	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF ARIX Corp.	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-port:gen/div.c 1.0 */
	
/* This function is not reentrant !!! */

#include <stdlib.h>

div_t div ( int numer, int denom )
{
	div_t res;

	res.quot = numer / denom;
	res.rem  = numer % denom;
	return ( res );
}
