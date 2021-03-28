#ident	"@(#)atan.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libm:atan.c	1.13"	*/
	

/*LINTLIBRARY*/
/*
 *	atan returns the arctangent of its double-precision argument,
 *	in the range [-pi/2, pi/2].
 *	There are no error returns.
 *
 *	atan2(y, x) returns the arctangent of y/x,
 *	in the range [-pi, pi].
 *	atan2 discovers what quadrant the angle is in and calls atan.
 *	atan2 returns EDOM error and value 0 if both arguments are zero.
 *
 *	Coefficients are #5077 from Hart & Cheney (19.56D).
 */
/* sw0
 * for atan2(0,-1) the result was 0 and should be pi ?
 */

#include <math.h>
#include <values.h>
#include <errno.h>
#include <_math.h>

#define _SQ2_M_1	0.41421356237309504880
#define _SQ2_P_1	2.41421356237309504880
#define _NEG	1
#define _INV	2
#define _MID	4

extern double atan ( register double x )
{
	register int type = 0;

	if (x < 0) {
		x = -x;
		type = _NEG;
	}
	if (x > _SQ2_P_1) {
		x = 1/x;
		type |= _INV;
	} else if (x > _SQ2_M_1) {
		x = (x - 1)/(x + 1);
		type |= _MID;
	}
	if (x < -X_EPS || x > X_EPS) {
		/* skip for efficiency and to prevent underflow */
		static double p[] = {
			0.161536412982230228262e2,
			0.26842548195503973794141e3,
			0.11530293515404850115428136e4,
			0.178040631643319697105464587e4,
			0.89678597403663861959987488e3,
		}, q[] = {
			1.0,
			0.5895697050844462222791e2,
			0.536265374031215315104235e3,
			0.16667838148816337184521798e4,
			0.207933497444540981287275926e4,
			0.89678597403663861962481162e3,
		};
		register double xsq = x * x;

		x *= _POLY4(xsq, p)/_POLY5(xsq, q);
	}
	if (type & _INV)
		x = M_PI_2 - x;
	else if (type & _MID)
		x += M_PI_4;
	return (type & _NEG ? -x : x);
}

extern double atan2 ( register double y, register double x )
{
	register int neg_y = 0;
	register double arg1;
	double at;

	arg1 = y;
	if (!x && !y) {				/* error, both arguments are zero */
		errno = EDOM;
		return ( 0.0 );
	}
	/*
	 * The next lines determine if |x| is negligible compared to |y|,
	 * without dividing, and without adding values of the same sign.
	 */
	if (arg1 < 0) {
		arg1 = -arg1;
		neg_y++;
	}
	if (arg1 - _ABS(x) == arg1) 
		return (neg_y ? -M_PI_2 : M_PI_2);
#ifdef before_sw0
/*	BUGGY found fix in  /usr/src/cmd/text stuff
*/
	/*
	 * The next line assumes that if y/x underflows the result
	 * is zero with no error indication, so it's safe to divide.
	 */
	return ((y = atan(y/x)) == 0 || x > 0 ? y :
	    neg_y ? y - M_PI : y + M_PI);
#endif /* before_sw0 */
	/*
	 * The next line assumes that if y/x underflows the result
	 * is zero with no error indication, so it's safe to divide.
	 */
/* sw0 */
	at = atan(y/x);
	if (x > 0)		     /* return arctangent directly */
		return (at);
	if (neg_y)		    /* x < 0, adjust arctangent for */
		return (at - M_PI);        /* correct quadrant */
	else
		return (at + M_PI);
}
