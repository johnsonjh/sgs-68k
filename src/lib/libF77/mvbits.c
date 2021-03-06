/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) mvbits.c: version 25.1 created on 12/2/91 at 18:34:17	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)mvbits.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libF77:mvbits.c	1.2"	*/
	
#ident	"@(#)libF77:mvbits.c	25.1"

#include "bit.h"

/* Move len bits from position i through i+len-1 of argument m to
 *		      position j through j+len-1 of argument n.
 * The right most bit is bit 0.
 */

long mvbits_(m,i,len,n,j)
long *m, *i, *len, *n, *j;
{
	unsigned long b;

	if ( (*i + *len > NBLI) || (*j + *len > NBLI) ||
	     (*len <= 0) || (*i < 0) || (*j < 0) ) return;

	*n &= ~(F77mask[*j + *len] ^ F77mask[*j]);	/* clear dest field */
	b = (*m >> *i) & F77mask[*len];		/* extract bits from src */
	*n |= (b << *j);			/* position bits and insert */
}
