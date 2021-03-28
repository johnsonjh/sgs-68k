#ident	"@(#)tanh.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
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
#include <sys/fpu.h>

double
tanh(x)
register double x;
{
	double y;
	struct exception exc;
	register unsigned int status = tanh881(x,&y);

	if (!(status & FPCC_NAN))
		return(y); /* No important error conditions found */

	/* If input was a NaN, report a domain error! */

	exc.arg1 = x;
	exc.name = "tanh";

	exc.type = DOMAIN;
	exc.retval = 0.0;
	if (!matherr(&exc)) 
		{
		(void) write(2, exc.name, 4);
		(void) write(2, ": DOMAIN error\n", 15);
		errno = EDOM;
		}
	return (exc.retval);
}
