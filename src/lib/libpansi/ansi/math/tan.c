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
#include <sys/types.h>
#include <sys/fpu.h>
#include <_math.h>

extern double tan ( register double x )
{
	register double q;
	register int exc;
	register double retval;

	exc = _NOEXC;
	q = x;
	if (x < 0) {
		q = -q;
	}
	if (q > X_TLOSS/2)
		exc = _TLOSS;
	else {
		double z;
#ifdef __STRICT_STDC__ 
		if (_tan881(x, &z) & (EXC_OPER|EXC_DZ|FPCC_NAN))
#else
		if (tan881(x, &z) & (EXC_OPER|EXC_DZ|FPCC_NAN))
#endif /* __STRICT_STDC__ */
			/* Range error when input is an infinity,
			   a NaN, or is pi/2+k*pi */
			exc = _TLOSS;
		else
			retval = z;
	}

	if (exc == _TLOSS) {
		errno = ERANGE;
		return ( 0.0 );
	}

	if (x > X_PLOSS/2 || x < -X_PLOSS/2) {
		errno = ERANGE;
	}
	return ( retval );  	
}
