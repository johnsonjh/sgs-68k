#ident	"@(#)exp.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libm:exp.c	1.13"	*/
	

/*LINTLIBRARY*/
/*
 *	exp returns the exponential function of its double-precision argument.
 *	Returns ERANGE error and value 0 if argument too small,
 *	   value HUGE if argument too large.
 *	Algorithm and coefficients from Cody and Waite (1980).
 *	Calls ldexp.
 */

#include <values.h>
#include <errno.h>
#include <math.h>
#include <_math.h>

extern double exp ( register double x )
{
	static double p[] = {
		0.31555192765684646356e-4,
	        0.75753180159422776666e-2,
	        0.25000000000000000000e0,
	}, q[] = {
		0.75104028399870046114e-6,
	        0.63121894374398503557e-3,
	        0.56817302698551221787e-1,
	        0.50000000000000000000e0,
	};
	register double y;
	register int n;
	register double arg1;

	arg1 = x;
	if (x < 0)
		x = -x;
	if (x < X_EPS) /* use first term of Taylor series expansion */
		return (1.0 + arg1);

	if (arg1 <= LN_MINDOUBLE) {
		if (arg1 == LN_MINDOUBLE) /* protect against roundoff */
			return (MINDOUBLE); /* causing ldexp to underflow */
		errno = ERANGE;					/* NOT ANSI !!! */
		return ( 0.0 );					/* NOT ANSI !!! */
	}

	if (arg1 >= LN_MAXDOUBLE) {
		if (arg1 == LN_MAXDOUBLE) /* protect against roundoff */
			return (MAXDOUBLE); /* causing ldexp to overflow */
		errno = ERANGE;
		return ( HUGE_VAL);
	}

	n = (int)(x * M_LOG2E + 0.5);
	y = (double)n;
	_REDUCE(int, x, y, 0.693359375, -2.1219444005469058277e-4);
	if (arg1 < 0) {
		x = -x;
		n = -n;
	}
	y = x * x;
	x *= _POLY2(y, p);
	return (ldexp(0.5 + x/(_POLY3(y, q) - x), n + 1));
}
