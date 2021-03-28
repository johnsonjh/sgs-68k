/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) bclr_l.c: version 25.1 created on 12/2/91 at 18:30:12	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)bclr_l.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libF77:bclr_l.c	1.2"	*/
	
#ident	"@(#)libF77:bclr_l.c	25.1"

#include "bit.h"

/* The result is equal to the value of n with the ith bit set to 0.
 * The right most bit is bit 0.
 */

long bclr_l(n,i)
long *n, *i;
{
	if (*i < 0 || *i >= NBLI)
		return(0L);
	return(*n & ~(1L << *i));
}
