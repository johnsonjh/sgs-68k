#ident	"@(#)fmod.c	23.2	1/8/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libm:fmod.c	1.12"	*/
	

/*LINTLIBRARY*/
/*
 *	fmod(x, y) returns the remainder of x on division by y,
 *	with the same sign as x,
 *	except that if |y| << |x|, it returns 0.
 */
/*	Returns 0.0 if y=0,as opposed to the 5.3.1 manual
 *	
 */

#include <math.h>
#include "_math.h"

extern double fmod ( register double x, register double y)
{
	double d; /* can't be in register because of modf() below */


	if ( !y ) return ( 0.0 ); /* to conform with ANSI decision 8612 */

	/*
	 * The next lines determine if |y| is negligible compared to |x|,
	 * without dividing, and without adding values of the same sign.
	 
	d = _ABS(x);
	if (d - _ABS(y) == d)
		return (x); kd0*/

#if !defined(pdp11) && !defined(M32) /* pdp11 and M32 "cc" cant handle
					cast of a double to a void	*/
	(void)
#endif
	modf(x/y, &d); /* now it's safe to divide without overflow */
	return (x - d * y);
}
