#ident	"@(#)floor.c	23.2	1/8/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libm:floor.c	1.8"	*/
	

/*LINTLIBRARY*/
/*
 *	floor(x) returns the largest integer (as a double-precision number)
 *	not greater than x.
 *	ceil(x) returns the smallest integer not less than x.
 */

#include <math.h> 			/* for modf() prototype */

extern double floor ( double x )
{
	double y; /* can't be in register because of modf() below */

	return (modf(x, &y) < 0 ? y - 1 : y);
}

extern double ceil ( double x )
{
	double y; /* can't be in register because of modf() below */

	return (modf(x, &y) > 0 ? y + 1 : y);
}
