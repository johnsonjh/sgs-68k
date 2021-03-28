#ident	"@(#)sinh.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*		Copyright (c) 1985 AT&T		*/
/*		All Rights Reserved		*/
/*	ATT:#ident	"libm:sinh.c	1.11"	*/
	
/*LINTLIBRARY*/
/*
 *	sinh returns the hyperbolic sine of its double-precision argument.
 *
 *	cosh returns the hyperbolic cosine of its double-precision argument.
 *
 *	Returns ERANGE error and value HUGE (or -HUGE for sinh of
 *	negative argument) if the correct result would overflow.
 *
 */

#include <math.h>
#include <values.h>
#include <errno.h>
#include <sys/fpu.h>

double
sinh(x)
register double x;
{
	double y;
	struct exception exc;
	register unsigned int status = sinh881(x,&y);

	if (!(status & (EXC_OVFL|FPCC_NAN|FPCC_I)))
		return(y); /* No important error conditions found */

	exc.arg1 = x;
	exc.name = "sinh";

	if (status & (EXC_OVFL|FPCC_I)) {/* Overflow or input +inf. */
		{
		exc.retval = (x<0) ? -HUGE : HUGE;
					/* sinh of negative argument */
		exc.type = OVERFLOW;
		if (!matherr(&exc)) 
			errno = ERANGE;
		return (exc.retval);
		}
	}

	if (status & (FPCC_NAN)) {/* Input was a NaN! */
		exc.type = DOMAIN;
		exc.retval = 0.0;
		if (!matherr(&exc)) {
			(void) write(2, exc.name, 4);
			(void) write(2, ": DOMAIN error\n", 15);
			errno = EDOM;
		}
		return (exc.retval);
	}
}

double
cosh(x)
register double x;
{
	double y;
	struct exception exc;
	register unsigned int status = cosh881(x,&y);

	if (!(status & (EXC_OVFL|FPCC_NAN|FPCC_I)))
		return(y); /* No important error conditions found */

	exc.arg1 = x;
	exc.name = "cosh";

	if (status & (EXC_OVFL|FPCC_I)) {/* Overflow or input +inf. */
		{
		exc.retval = HUGE;
		exc.type = OVERFLOW;
		if (!matherr(&exc)) 
			errno = ERANGE;
		return (exc.retval);
		}
	}

	if (status & (FPCC_NAN)) {/* Input was a NaN! */
		exc.type = DOMAIN;
		exc.retval = 0.0;
		if (!matherr(&exc)) {
			(void) write(2, exc.name, 4);
			(void) write(2, ": DOMAIN error\n", 15);
			errno = EDOM;
		}
		return (exc.retval);
	}
}

