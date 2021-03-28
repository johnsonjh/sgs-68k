#ident	"@(#)sqrt.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*		Copyright (c) 1985 AT&T		*/
/*		All Rights Reserved		*/
/*	ATT:#ident	"libm:sqrt.c	1.11"	*/
	
/*LINTLIBRARY*/
/*
 *	sqrt returns the square root of its double-precision argument,
 *	using Newton's method.
 *	Returns EDOM error and value 0 if argument negative.
 *	Calls frexp and ldexp.
 */

#include <errno.h>
#include <math.h>
#include <sys/fpu.h>

double
sqrt(x)
register double x;
{
	double y;
	struct exception exc;
	register unsigned int status = sqrt881(x,&y);

	if (!(status&(EXC_OPER|FPCC_NAN|FPCC_I)))
		return (y); /* sqrt was okay */

	exc.name = "sqrt";
	exc.arg1 = x;

	if (status&(EXC_OPER|FPCC_NAN)) {
	        /* Input was a NaN or negative */
		exc.type = DOMAIN;
		exc.retval = 0.0;
		if (!matherr(&exc)) {
			(void) write(2, "sqrt: DOMAIN error\n", 19);
			errno = EDOM;
		}
		return (exc.retval);
	}

	if (status & (FPCC_I)) {/* Input was +infinity. */
		exc.type = OVERFLOW;
		exc.retval = HUGE;
		if (!matherr(&exc)) 
			errno = ERANGE;
		return (exc.retval);
	}

}
