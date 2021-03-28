/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) d_sign.c: version 25.1 created on 12/2/91 at 18:32:03	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)d_sign.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libF77:d_sign.c	1.4"	*/
	
#ident	"@(#)libF77:d_sign.c	25.1"

double d_sign(a,b)
double *a, *b;
{
double x;
	x = *a;
	if (x <= 0)
		x = -x;
	if (*b < 0)
		x = -x;
	return (x);
/*x = (*a >= 0 ? *a : - *a);
 *return( *b >= 0 ? x : -x);
*/
}
