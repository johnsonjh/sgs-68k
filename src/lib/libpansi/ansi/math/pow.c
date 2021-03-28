#ident	"@(#)pow.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libm:pow.c	1.16"	*/
	

/*LINTLIBRARY*/
/*
 *	pow(x, y) returns x ** y.
 *	Returns EDOM error and value 0 for 0 to a non-positive power
 *	or negative to a non-integral power;
 *	ERANGE error and value HUGE or -HUGE when the correct value
 *	would overflow, or 0 when the correct value would underflow.
 *	uses log and exp
 *	This version accepts negative x and integral y,
 *	apparently in violation of the ANSI FORTRAN standard for x ** y.
 *	There is a much more accurate but much more elaborate algorithm
 *	in Cody and Waite, which should be substituted for this.
 */

#include <math.h>
#include <values.h>
#include <errno.h>
#include "_math.h"

extern double pow ( register double x, register double y )
{
	register int neg;

	if (y == 1) /* easy special case */
		return (x);

	if (!x) {
		if (y > 0)
			return (x); /* (0 ** pos) == 0 */
		goto domain;
	}

	neg = 0;
	if (x < 0) { /* test using integer arithmetic if possible */
		if (y >= -MAXLONG && y <= MAXLONG) {
			register long ly = (long)y;

			if ((double)ly != y)
				goto domain; /* y not integral */
			neg = ly % 2;
		} else {
			double fr, dum;

			if (fr = modf(0.5 * y, &dum)) {
				if (fr != 0.5)
					goto domain; /* y not integral */
				neg++; /* y is odd */
			}
		}
		x = -x;
	}
	if (x != 1) { /* x isn't the final result */
		/* the following code protects against multiplying x and y
		 * until there is no chance of multiplicative overflow */
		if ((x = log(x)) < 0) { /* preserve sign of product */
			x = -x;
			y = -y;
		}
		if (y > LN_MAXDOUBLE/x) {
			errno = ERANGE;
			return ( neg ? -HUGE_VAL : HUGE_VAL );
		}

		if (y < LN_MINDOUBLE/x) {
			errno = ERANGE;
			return ( 0.0 );
		}

		x = exp(x * y); /* finally; no mishap can occur */
	}
	return (neg ? -x : x);

domain:
	errno = EDOM;
	return ( 0.0 );
}
