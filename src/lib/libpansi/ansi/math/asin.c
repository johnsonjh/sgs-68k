#ident	"@(#)asin.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*		Copyright (c) 1985 AT&T		*/
/*		All Rights Reserved		*/
/* OLD MOT:asin.c	6.2	 */

/*	OLD MOT:asin.c	6.1		*/
/*	asin.c	1.12	*/
/*LINTLIBRARY*/
/*
 *	C program for double-precision asin/acos.
 *  Returns zero and sets errno to EDOM if |argument| > 1
 *  If result is too large to fit in a double value,sets errno to ERANGE and
 *        - if the result overflows returns HUGE_VAL
 *		  - if the result underflows returns zero
 */


#include <math.h>
#include <sys/types.h>
#include <sys/fpu.h>
#include <_math.h>			/* for _matherr call */

extern double asin ( register double x )
{
	double y;
#ifdef __STRICT_STDC__
	register unsigned int status = _asin881(x,&y);
#else
	register unsigned int status = asin881(x,&y);
#endif /* __STRICT_STDC__ */

	if (!(status & (EXC_OPER|FPCC_NAN)))
		return(y); /* No important error conditions found */

   /* operr or overflow or underflow */
	return ( _matherr (status) );
}


extern double acos ( register double x )
{
	double y;
#ifdef __STRICT_STDC__ 
	register unsigned int status = _acos881(x,&y);
#else
	register unsigned int status = acos881(x,&y);
#endif /* __STRICT_STDC__ */

	if (!(status & (EXC_OPER|FPCC_NAN)))
		return(y); /* No important error conditions found */

   /* operr or overflow or underflow */
	return ( _matherr (status) );
}
