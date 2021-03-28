/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) shift_l.c: version 25.1 created on 12/2/91 at 18:36:40	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)shift_l.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libF77:shift_l.c	1.3"	*/
	
#ident	"@(#)libF77:shift_l.c	25.1"

#include "bit.h"

/* Logically shift m k-places. If k > NBLI, m = undefined.
 * k > 0 => left shift.
 * k < 0 => right shift.
 * k = 0 => no shift.
 */

long shft_l(m,k)
long *m, *k;
{
	long l;
	short left;

	left = (*k > 0);
	l = left ? *k : -*k;

	if (l > 0 && l < NBLI)
		if (left)
			return(*m << l);
		else
			return( (*m >> l) & F77mask[NBLI - l] );
	else if (l == 0)
		return(*m);
	else return(0L);
}
