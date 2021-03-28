#ident	"@(#)tan.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*		Copyright (c) 1985 AT&T		*/
/*		All Rights Reserved		*/
/*	ATT:#ident	"libm:tan.c	1.14"	*/
	
/*LINTLIBRARY*/
/*
 *	tan returns the tangent of its double-precision argument.
 *	Returns ERANGE error and value 0 if argument too large.
 */

#include <math.h>
#include <values.h>
#include <errno.h>
#include <sys/fpu.h>

double
tan(x)
register double x;
{
	register double q;
	struct exception exc;

	exc.type = NOEXC;
	exc.name = "tan";
	exc.arg1 = x;
	q = x;
	if (x < 0) {
		q = -q;
	}
	if (q > X_TLOSS/2)
		exc.type = TLOSS;
	else {
		double z;
		if (tan881(x, &z) & (EXC_OPER|EXC_DZ|FPCC_NAN))
			/* Range error when input is an infinity,
			   a NaN, or is pi/2+k*pi */
			exc.type == TLOSS;
		else
			exc.retval = z;
	}

	if (exc.type == TLOSS) {
		exc.retval = 0.0;
		if (!matherr(&exc)) {
			(void) write(2, "tan: TLOSS error\n", 17);
			errno = ERANGE;
		}
		return (exc.retval);
	}

	if (exc.arg1 > X_PLOSS/2 || exc.arg1 < -X_PLOSS/2) {
		exc.type = PLOSS;
		if (!matherr(&exc))
			errno = ERANGE;
	}
	return (exc.retval);
}
