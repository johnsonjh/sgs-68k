/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) bext_h.c: version 25.1 created on 12/2/91 at 18:30:15	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)bext_h.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libF77:bext_h.c	1.2"	*/
	
#ident	"@(#)libF77:bext_h.c	25.1"

#include "bit.h"

/* Extract a subfield of len bits from m starting with bit position i
 * and extending left for len bits. The right most bit is bit 0.
 */

short bext_h(m,i,len)
short *m, *i, *len;
{
	if ( (*i + *len > NBSI)  ||  (*i < 0)  ||  (*len < 0)  ||
	     (*i >= NBSI) || (*len > NBSI) ) return(*m);
	return((*m & F77mask[*i + *len]) >> *i);
}
