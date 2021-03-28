#ident	"@(#)frexp.c	23.2	1/8/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-port:gen/frexp.c	1.9"	*/
	

/*LINTLIBRARY*/
/*
 * frexp(value, eptr)
 * returns a double x such that x = 0 or 0.5 <= |x| < 1.0
 * and stores an integer n such that value = x * 2 ** n
 * indirectly through eptr.
 *
 */
#include <shlib.h>
#include <values.h>
#include <nan.h>
#include <syscall.h>

extern double frexp ( double value, register int *eptr )
/* double value :  don't declare register, because of KILLNan! */
{
	register double absvalue;

/*	KILLNaN(value);  raise exception on Not-a-Number (3b only) */
	/* detect infinities and turn them into large #'s */
 	/* let doprnt deal w/:	if (INF(value))  CHNGinf(value); /* sw1 */

	if (IsNANorINF(value))
	  {
	    if (!IsINF(value)) 
	  	KILLFPE();	
	    else
		CHNGinf(value);
	  }

	*eptr = 0;
	if (value == 0.0) /* nothing to do for zero */
		return (value);
	absvalue = (value > 0.0) ? value : -value;
	for ( ; absvalue >= 1.0; absvalue *= 0.5)
		++*eptr;
	for ( ; absvalue < 0.5; absvalue += absvalue)
		--*eptr;
	return (value > 0.0 ? absvalue : -absvalue);
}
