#ident	"@(#)log.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*		Copyright (c) 1985 AT&T		*/
/*		All Rights Reserved		*/
/*	ATT:#ident	"libm:log.c	1.14"	*/
	
/*LINTLIBRARY*/
/*
 *	log returns the natural logarithm of its double-precision argument.
 *	log10 returns the base-10 logarithm of its double-precision argument.
 *	Returns EDOM error and value -HUGE if argument < 0.
 *	Returns ERANGE error and value -HUGE if argument = 0.
 *	Returns ERANGE error and value HUGE if result was an infinity.
 */
/* 	kd0	log was processing the wrong variable on error
 */

#include <math.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/fpu.h>
#include <_math.h>

static double _log_error ( register unsigned int exc )
{
	register double ret;

	ret = -HUGE_VAL;

	if (exc & EXC_DZ) { /* divide by 0, returns -inf. result */
		errno = ERANGE;
	}
	else if (exc & FPCC_I) {		/* result was an infinity */
		errno = ERANGE;
		return ( HUGE_VAL );
	}
	else {
					/* only other expected exceptions are
				           domain error (out of range or
					   a NaN) */
		errno = EDOM;	
	}
	return (ret);
}


extern double log ( register double x )
{
	double y;
	register unsigned int exc;

#ifdef __STRICT_STDC__ 
	exc = _logn881(x, &y) & (EXC_OPER|EXC_DZ|FPCC_NAN|FPCC_I);
#else
	exc = logn881(x, &y) & (EXC_OPER|EXC_DZ|FPCC_NAN|FPCC_I);
#endif /*__STRICT_STDC__ */
				/* mask all but the important status
				   bits */

	if (!(exc))
		return(y);	/* no exception, just return result */
	else
		return (_log_error(exc));
}

extern double log10 ( register double x )
{
	double y;
	register unsigned int exc;

#ifdef __STRICT_STDC__ 
	exc = _log881(x, &y) & (EXC_OPER|EXC_DZ|FPCC_NAN);
#else
	exc = log881(x, &y) & (EXC_OPER|EXC_DZ|FPCC_NAN);
#endif /* __STRICT_STDC__ */
				/* mask all but the important status
				   bits */

	if (!(exc))
		return(y);	/* no exception, just return result */
	else
		return (_log_error(exc));
}

