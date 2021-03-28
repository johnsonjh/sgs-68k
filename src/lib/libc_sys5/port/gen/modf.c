#ident	"@(#)modf.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-port:gen/modf.c	1.10"	*/
	

/*LINTLIBRARY*/
/*
 * modf(value, iptr) returns the signed fractional part of value
 * and stores the integer part indirectly through iptr.
 *
 */

#include <values.h>
#include <nan.h>

double
modf(value, iptr)
double value; /* don't declare register, because of KILLNaN! */
register double *iptr;
{
	register double absvalue;

/*	KILLNaN(value);  raise exception on Not-a-Number (3b only) */
 	/* detect infinities and turn them into large #'s */
 	/* hanna: let doprnt/doscan deal with infinity.
      	   if (INF(value))  CHNGinf(value);  sw1 */
	if (IsNANorINF(value))
	  {
	    if (!IsINF(value)) 
		{
	   	 KILLFPE();	
		}
	    else
		{
	         CHNGinf(value);
		}
	  }

	if ((absvalue = (value >= 0.0) ? value : -value) >= MAXPOWTWO)
		*iptr = value; /* it must be an integer */
	else {
		*iptr = absvalue + MAXPOWTWO; /* shift fraction off right */
		*iptr -= MAXPOWTWO; /* shift back without fraction */
		while (*iptr > absvalue) /* above arithmetic might round */
			*iptr -= 1.0; /* test again just to be sure */
		if (value < 0.0)
			*iptr = -*iptr;
	}
	return (value - *iptr); /* signed fractional part */
}
