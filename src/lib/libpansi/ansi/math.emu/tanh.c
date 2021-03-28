#ident	"@(#)tanh.c	23.4	8/27/91 Copyright (c) 1991 by Arix Corp."
/*		Copyright (c) 1985 AT&T		*/
/*		All Rights Reserved		*/
/*	ATT:#ident	"libm:tanh.c	1.10"	*/
	
/*LINTLIBRARY*/
/*
 *	tanh returns the hyperbolic tangent of its double-precision argument.
 */

#include <math.h>
#include <values.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/fpu.h>
#include <_math.h>

extern double tanh ( register double x )
{
	double y;
#ifdef __STRICT_STDC__ 
	register unsigned int status = _tanh881(x,&y);
#else
	register unsigned int status = tanh881(x,&y);
#endif /* __STRICT_STDC__ */

	if (!(status & FPCC_NAN))
		return(y); /* No important error conditions found */

	/* If input was a NaN, report a domain error! */

	/* to complete with if? !!! */
	errno = EDOM;
	return ( 0.0 );
}
