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
#include <sys/types.h>
#include <sys/fpu.h>
#include <_math.h>

extern double sinh ( register double x )
{
	double y;
#ifdef __STRICT_STDC__
	register unsigned int status = _sinh881(x,&y);
#else
	register unsigned int status = sinh881(x,&y);
#endif /* _STRICT_STDC  */

	if (!(status & (EXC_OVFL|FPCC_NAN|FPCC_I)))
		return(y); /* No important error conditions found */

	if (status & (EXC_OVFL|FPCC_I)) {/* Overflow or input +inf. */
		errno = ERANGE;
		return ( (x<0) ? -HUGE_VAL : HUGE_VAL );
	}

	if (status & (FPCC_NAN)) {/* Input was a NaN! */
		errno = EDOM;
		return ( 0.0 );
	}
	/* if another error , return dummy !!! original error in sinh.c GOMBOS */
}

extern double cosh ( register double x )
{
	double y;
#ifdef __STRICT_STDC__ 
	register unsigned int status = _cosh881(x,&y);
#else
	register unsigned int status = cosh881(x,&y);
#endif /* __STRICT_STDC__ */

	if (!(status & (EXC_OVFL|FPCC_NAN|FPCC_I)))
		return(y); /* No important error conditions found */

	if (status & (EXC_OVFL|FPCC_I)) {/* Overflow or input +inf. */
		errno = ERANGE;
		return ( HUGE_VAL);
	}

	if (status & (FPCC_NAN)) {/* Input was a NaN! */
		errno = EDOM;
		return ( 0.0 );
	}
}

