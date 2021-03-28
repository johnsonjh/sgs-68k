#ident	"@(#)asin.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*		Copyright (c) 1985 AT&T		*/
/*		All Rights Reserved		*/
/* OLD MOT:asin.c	6.2	 */

/*	OLD MOT:asin.c	6.1		*/
/*	asin.c	1.12	*/
/*LINTLIBRARY*/
/*
 *	C program for double-precision asin/acos.
 *	Returns EDOM error and value 0 if |argument| > 1.
 */


#include <math.h>
#include <values.h>
#include <errno.h>
#include <sys/fpu.h>

double
asin(x)
register double x;
{
	double y;
	struct exception exc;
	register unsigned int status = asin881(x,&y);

	if (!(status & (EXC_OPER|FPCC_NAN)))
		return(y); /* No important error conditions found */

	/* If input was a NaN or sin/cos value was greater than 1,
	** report a domain error! */

	exc.arg1 = x;
	exc.name = "asin";

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


double
acos(x)
register double x;
{
	double y;
	struct exception exc;
	register unsigned int status = acos881(x,&y);

	if (!(status & (EXC_OPER|FPCC_NAN)))
		return(y); /* No important error conditions found */

	/* If input was a NaN or sin/cos value was greater than 1,
	** report a domain error! */

	exc.arg1 = x;
	exc.name = "acos";

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
