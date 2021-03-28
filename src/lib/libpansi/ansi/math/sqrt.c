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
#include <sys/types.h>
#include <sys/fpu.h>
#include <_math.h>

extern double sqrt ( register double x )
{
	double y;
#ifdef __STRICT_STDC__
	register unsigned int status = _sqrt881(x,&y);
#else
	register unsigned int status = sqrt881(x,&y);
#endif /* __STRICT_STDC__ */

	if (!(status&(EXC_OPER|FPCC_NAN|FPCC_I)))
		return (y); /* sqrt was okay */

	if (status&(EXC_OPER|FPCC_NAN)) {
	        /* Input was a NaN or negative */
		errno = EDOM;
		return ( 0.0 );
	}

	if (status & (FPCC_I)) {/* Input was +infinity. */
		errno = ERANGE;
		return ( HUGE_VAL );
	}

}
